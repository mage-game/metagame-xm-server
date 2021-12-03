#include "combineserveractivitymanager.hpp"
#include "world.h"
#include "gamelog.h"
#include <algorithm>
#include "config/logicconfigmanager.hpp"
#include "global/combineserveractivity/combineserveractivityconfig.hpp"
#include "engineadapter.h"
#include "global/specialactivity/specialactivitymanager.hpp"
#include "global/specialactivity/impl/specialactivitycombineserver.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "global/usercache/usercache.hpp"
#include "obj/character/role.h"
#include "limits.h"
#include <vector>
#include "item/itempool.h"
#include "item/itembase.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "scene/speciallogic/worldspecial/specialcombineserverboss.hpp"
#include "global/combineserveractivity/combineserveractivitybossconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "other/roleactivity/roleactivity.hpp"

int ComparaCSAI(const void *arg1, const void *arg2)
{
	CombineServerActivityRankUser* ci1 = (CombineServerActivityRankUser*)arg1;
	CombineServerActivityRankUser* ci2 = (CombineServerActivityRankUser*)arg2;

	if (ci1->rank_value < ci2->rank_value)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

CombineServerActivityManager & CombineServerActivityManager::Instance()
{
	static CombineServerActivityManager _instance;
	return _instance;
}

CombineServerActivityManager::CombineServerActivityManager() 
{

}

CombineServerActivityManager::~CombineServerActivityManager()
{

}

void CombineServerActivityManager::Init(const CombineServerActivityRankParam &cbs_rank_param, const CombineServerActivityOtherParam &cbs_other_param,
		const CombineServerActivityBossRankParam &csa_boss_rank_param)
{
	m_csa_rank_param = cbs_rank_param;
	m_csa_other_param = cbs_other_param;
	m_csa_boss_rank_param = csa_boss_rank_param;
}

void CombineServerActivityManager::GetInitParam(CombineServerActivityRankParam *cbs_rank_param, CombineServerActivityOtherParam *cbs_other_param,
	CombineServerActivityBossRankParam *csa_boss_rank_param)
{
	*cbs_rank_param = m_csa_rank_param;
	*cbs_other_param = m_csa_other_param;
	*csa_boss_rank_param = m_csa_boss_rank_param;
}

void CombineServerActivityManager::Update(unsigned long interval, time_t now_second)
{
	// this->CheckRankReward(CSA_RANK_TYPE_QIANGGOU);
}

bool CombineServerActivityManager::SubActivityOpen(int sub_type)
{
	// 活动开启时也清一遍数据，防止已经合服的服务器没清掉数据
	switch (sub_type)
	{
	case CSA_SUB_TYPE_GONGCHENGZHAN:
	{
		m_csa_other_param.csa_gcz_winner_roleid = 0;
		m_csa_other_param.cas_gcz_winner_lover_id = 0;
	}
	break;

	case CSA_SUB_TYPE_XIANMENGZHAN:
	{
		m_csa_other_param.csa_xmz_winner_roleid = 0;
	}
	break;
	}

	return true;
}

bool CombineServerActivityManager::SubActivityFinish(int sub_type)
{
	int rank_type = LOGIC_CONFIG->GetCombineServerActivityConfig().GetRelatedRankType(sub_type);
	if (rank_type > CSA_RANK_TYPE_INVALID && rank_type < CSA_RANK_TYPE_MAX)
	{
		//if (0 == m_csa_rank_param.rank_item_list[rank_type].is_finish)
		{
			// 发奖励 
			this->CheckRankReward(rank_type);

			//m_csa_rank_param.rank_item_list[rank_type].is_finish = 1;
			this->ResetRankList(rank_type);
		}
	}

	switch (sub_type)
	{
	case CSA_SUB_TYPE_GONGCHENGZHAN:
	{
		m_csa_other_param.csa_gcz_winner_roleid = 0;
		m_csa_other_param.cas_gcz_winner_lover_id = 0;
	}
	break;

	case CSA_SUB_TYPE_XIANMENGZHAN:
	{
		m_csa_other_param.csa_xmz_winner_roleid = 0;
	}
	break;

	case CSA_SUB_TYPE_BOSS:
	{
		SpecialCombineServerBoss *csa_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCSABoss(1);
		if (NULL != csa_boss)
		{
			csa_boss->OnActivityStatusChange(CSA_SUB_ACTIVITY_STATE_FINISH);
		}
	}
	break;
			
	default:
		break;
	}

	return true;
}

void CombineServerActivityManager::OnSendActivityInfo(Role *user)
{
	static Protocol::SCCSAActivityInfo csari;
	memcpy(csari.qianggou_buynum_list, m_csa_other_param.rank_qianggou_buynum_list, sizeof(csari.qianggou_buynum_list));
	csari.csa_xmz_winner_roleid  = m_csa_other_param.csa_xmz_winner_roleid;
	csari.csa_gcz_winner_roleid = m_csa_other_param.csa_gcz_winner_roleid;
	memcpy(csari.server_panic_buy_num_list, m_csa_other_param.server_panic_buy_num_list, sizeof(csari.server_panic_buy_num_list));

	for (int i = 0; i < CSA_RANK_TYPE_MAX; ++i)
	{
		Protocol::SCCSAActivityInfo::RankItem &rank_item = csari.rank_item_list[i];
		CombineServerActivityRankItem cbs_rank_item = m_csa_rank_param.rank_item_list[i];
		qsort(cbs_rank_item.user_list, COMBINE_SERVER_CONSUME_RANK_COUNT, sizeof(cbs_rank_item.user_list[0]), ComparaCSAI);

		rank_item.is_finish = cbs_rank_item.is_finish;

		for (int j = 0; j < COMBINE_SERVER_ACTIVITY_RANK_REWARD_ROLE_NUM; ++ j)
		{
			rank_item.user_list[j].role_id = cbs_rank_item.user_list[j].role_id;
			rank_item.user_list[j].rank_value = cbs_rank_item.user_list[j].rank_value;
			rank_item.user_list[j].rank_reserved = cbs_rank_item.user_list[j].rank_reserve;

			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(cbs_rank_item.user_list[j].role_id);
			if (NULL != user_node)
			{
				rank_item.user_list[j].camp = user_node->camp;
				rank_item.user_list[j].sex = user_node->sex;
				rank_item.user_list[j].prof = user_node->prof;
				user_node->GetName(rank_item.user_list[j].role_name);
			}
		}
	}

	if (NULL != user)
	{
		EngineAdapter::Instance().NetSend(user->GetNetId(), (const char*)&csari, sizeof(csari));
	}
	else
	{
		World::GetInstWorld()->SendToAllGateway((const char *)&csari, sizeof(csari));
	}
}

void CombineServerActivityManager::SyncInfoToGameWorld()
{
	SpecialActivityCombineServer *special_activity_combine_server = SpecialActivityManager::Instance().GetSpecialActivityCombineServer();
	if (NULL != special_activity_combine_server)
	{
		static gglobalprotocal::CombineServerActivitySyncState csass;
		special_activity_combine_server->GetSubActivitState(CSA_SUB_TYPE_MAX, csass.sub_activity_state_list);

		ActivityShadow::Instance().OnCSASubActivitySyncState(&csass);
	}
}

int CombineServerActivityManager::GetRankQianggouBuyNum(int item_idx)
{
	if (item_idx >= 0 && item_idx < COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE)
	{
		return m_csa_other_param.rank_qianggou_buynum_list[item_idx];
	}

	return INT_MAX;
}

void CombineServerActivityManager::OnBuyRankQianggouItem(Role *role, int item_idx)
{
	if (item_idx >= 0 && item_idx < COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE)
	{
		++ m_csa_other_param.rank_qianggou_buynum_list[item_idx];

		if (NULL != role)
		{
			this->OnSendActivityInfo(role);
		}
	}
}

void CombineServerActivityManager::OnChangeXianmengzhanWiner(int old_winer, int new_winer)
{
	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		return;
	}

	if (0 != m_csa_other_param.csa_xmz_winner_roleid)
	{
		return;
	}

	UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(new_winer);
	if (NULL == user_node)
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(IntToUid(new_winer));
	if (NULL == guild)
	{
		return;
	}

	m_csa_other_param.csa_xmz_winner_roleid = new_winer;

	const CSAOtherConfig &other_cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetOtherCfg();
	{
		// 盟主奖励
		// 发邮件
		static MailContentParam contentparam; contentparam.Reset();
		int item_count = 0;
		for (int i = 0; i < other_cfg.xmz_winner_item_count && i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			const ItemBase *item_base = ITEMPOOL->GetItem(other_cfg.xmz_winner_item_list[i].item_id);
			if (item_base != nullptr)
			{
				contentparam.item_list[item_count].item_id = other_cfg.xmz_winner_item_list[i].item_id;
				contentparam.item_list[item_count].num = other_cfg.xmz_winner_item_list[i].num;
				contentparam.item_list[item_count].is_bind = (other_cfg.xmz_winner_item_list[i].is_bind) ? 1 : 0;
				contentparam.item_list[item_count].invalid_time = item_base->CalInvalidTime();
				++item_count;
			}
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_csa_xmz_winner_reward_content);
		if (length > 0)
		{
			MailRoute::MailToUser(IntToUid(new_winer), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_combine_server.printf(LL_INFO, "CSAChangeXianmengzhanWiner user %d %s, item_detail_1 %s, item_detail_2 %s, item_detail_3 %s",
				user_node->uid, user_node->user_name, ItemConfigDataLog(&other_cfg.xmz_winner_item_list[0], NULL), 
				ItemConfigDataLog(&other_cfg.xmz_winner_item_list[1], NULL), ItemConfigDataLog(&other_cfg.xmz_winner_item_list[2], NULL));
		}
	}

	{
		// 仙盟成员奖励
		// 发邮件
		static MailContentParam contentparam; contentparam.Reset();
		int item_count = 0;
		for (int i = 0; i < other_cfg.xmz_guild_item_count && i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			const ItemBase *item_base = ITEMPOOL->GetItem(other_cfg.xmz_guild_item_list[i].item_id);
			if (item_base != nullptr)
			{
				contentparam.item_list[item_count].item_id = other_cfg.xmz_guild_item_list[i].item_id;
				contentparam.item_list[item_count].num = other_cfg.xmz_guild_item_list[i].num;
				contentparam.item_list[item_count].is_bind = (other_cfg.xmz_guild_item_list[i].is_bind) ? 1 : 0;
				contentparam.item_list[item_count].invalid_time = item_base->CalInvalidTime();
				++item_count;
			}
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_csa_xmz_guild_reward_content);
		if (length > 0)
		{
			guild->GetGuildMemberManager().SystemMailToAll(contentparam);
		}
	}

	{
		// 传闻
		int notice_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_csa_xmz_winner_notice, 
			user_node->uid, user_node->user_name, user_node->prof);

		if (notice_len > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, notice_len, SYS_MSG_CENTER_ROLL);
		}
	}

	this->OnSendActivityInfo(NULL);
}

void CombineServerActivityManager::OnChangeGongchengzhanWiner(int old_winer, int new_winer)
{
	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		return;
	}

	if (0 != m_csa_other_param.csa_gcz_winner_roleid)
	{
		return;
	}

	UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(new_winer);
	if (NULL == user_node)
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(IntToUid(new_winer));
	if (NULL == guild)
	{
		return;
	}

	{
		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(new_winer));
		if (nullptr != role)
		{
			role->GetRoleActivity()->OnAddCSAGongChengZhanWinTimes();
			//role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);
		}

		Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(user_node->lover_id));
		if (nullptr != lover_role)
		{
			lover_role->GetRoleActivity()->OnAddCSAGongChengZhanWinTimes();
			//lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);
		}
	}

	//if (user_node->camp <= CAMP_TYPE_INVALID || user_node->camp >= CAMP_TYPE_MAX)
	//{
	//	return;
	//}

	m_csa_other_param.csa_gcz_winner_roleid = new_winer;
	m_csa_other_param.cas_gcz_winner_lover_id = user_node->lover_id;

	const CSAOtherConfig &other_cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetOtherCfg();
	{
		// 城主奖励
		// 发邮件
		static MailContentParam contentparam; contentparam.Reset();
		int item_count = 0;
		for (int i = 0; i < other_cfg.gcz_winner_item_count && i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			const ItemBase *item_base = ITEMPOOL->GetItem(other_cfg.gcz_winner_item_list[i].item_id);
			if (item_base != NULL)
			{
				contentparam.item_list[item_count].item_id = other_cfg.gcz_winner_item_list[i].item_id;
				contentparam.item_list[item_count].num = other_cfg.gcz_winner_item_list[i].num;
				contentparam.item_list[item_count].is_bind = (other_cfg.gcz_winner_item_list[i].is_bind) ? 1 : 0;
				contentparam.item_list[item_count].invalid_time = item_base->CalInvalidTime();
				++item_count;
			}
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_csa_gcz_winner_reward_content);
		if (length > 0)
		{
			MailRoute::MailToUser(IntToUid(new_winer), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_combine_server.printf(LL_INFO, "CSAChangeGongchengzhanWiner user %d %s, item_detail_1 %s, item_detail_2 %s, item_detail_3 %s",
				user_node->uid, user_node->user_name, ItemConfigDataLog(&other_cfg.gcz_winner_item_list[0], NULL),
				ItemConfigDataLog(&other_cfg.gcz_winner_item_list[1], NULL), ItemConfigDataLog(&other_cfg.gcz_winner_item_list[2], NULL));
		}
	}

	{
		// 仙盟成员奖励
		// 发邮件
		static MailContentParam contentparam; contentparam.Reset();
		int item_count = 0;
		for (int i = 0; i < other_cfg.gcz_camp_item_count && i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			const ItemBase *item_base = ITEMPOOL->GetItem(other_cfg.gcz_camp_item_list[i].item_id);
			if (item_base != NULL)
			{
				contentparam.item_list[item_count].item_id = other_cfg.gcz_camp_item_list[i].item_id;
				contentparam.item_list[item_count].num = other_cfg.gcz_camp_item_list[i].num;
				contentparam.item_list[item_count].is_bind = (other_cfg.gcz_camp_item_list[i].is_bind) ? 1 : 0;
				contentparam.item_list[item_count].invalid_time = item_base->CalInvalidTime();
				++item_count;
			}

		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_csa_gcz_camp_reward_content);
		if (length > 0)
		{

			guild->GetGuildMemberManager().SystemMailToAll(contentparam);
		}
	}

	{
		// 传闻
		int notice_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_csa_gcz_winner_notice, 
			user_node->uid, user_node->user_name, user_node->prof);

		if (notice_len > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, notice_len, SYS_MSG_CENTER_ROLL);
		}
	}

	this->OnSendActivityInfo(NULL);
}

int CombineServerActivityManager::GetServerPanicBuyNum(int item_idx)
{
	if (item_idx >= 0 && item_idx < COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT)
	{
		return m_csa_other_param.server_panic_buy_num_list[item_idx];
	}

	return INT_MAX;
}

void CombineServerActivityManager::OnServerPanicBuy(Role *role, int item_idx)
{
	if (item_idx >= 0 && item_idx < COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT)
	{
		++ m_csa_other_param.server_panic_buy_num_list[item_idx];
		
		if (NULL != role)
		{
			this->OnSendActivityInfo(role);
		}
	}
}

void CombineServerActivityManager::OnCSABossCheckRank(int rank_type, int id, int rank_value)
{
	bool is_change = false;
	switch (rank_type)
	{
	case BOSS_RANK_TYPE_PERSONAL:
		{
			is_change = this->UpdateBossRank(m_csa_boss_rank_param.personal_rank, COMBINE_SERVER_BOSS_RANK_NUM, id, rank_value);
		}
		break;

	case BOSS_RANK_TYPE_GUILD:
		{
			is_change = this->UpdateBossRank(m_csa_boss_rank_param.guild_rank, COMBINE_SERVER_BOSS_RANK_NUM, id, rank_value);
		}
		break;

	default:
		break;
	}

	if (is_change)
	{
		this->OnSendCSABossRankInfo(NULL);
	}
}

void CombineServerActivityManager::OnCSABossDismissGuild(int guild_id)
{
	bool is_reset = false;
	for (int i = 0; i < COMBINE_SERVER_BOSS_RANK_NUM; ++i)
	{
		if (m_csa_boss_rank_param.guild_rank[i].id == guild_id)
		{
			m_csa_boss_rank_param.guild_rank[i].id = 0;
			m_csa_boss_rank_param.guild_rank[i].rank_value = 0;
			is_reset = true;
		}

		if (is_reset && i != COMBINE_SERVER_BOSS_RANK_NUM - 1)
		{
			std::swap(m_csa_boss_rank_param.guild_rank[i], m_csa_boss_rank_param.guild_rank[i + 1]);
		}
	}

	if (is_reset)
	{
		this->OnSendCSABossRankInfo(NULL);
	}
}

void CombineServerActivityManager::OnCSABossClearRank()
{
	m_csa_boss_rank_param.Reset();
	this->OnSendCSABossRankInfo(NULL);
}

void CombineServerActivityManager::OnCSABossRankReward()
{
	int day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealCombineTime(), EngineAdapter::Instance().Time() - 3) + 1;
	const CombineServerActivityBossRankReward *cfg_item = LOGIC_CONFIG->GetCSABossConfig().GetCSABossRankRewardCfg(day_index);
	if (NULL == cfg_item)
	{
		return;
	}

	//个人奖励
	for (int i = 0; i < COMBINE_SERVER_ACTIVITY_RANK_REWARD_ROLE_NUM; i++)
	{
		int role_id = m_csa_boss_rank_param.personal_rank[i].id;
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(role_id);
		if (NULL != user_node)
		{
			const ItemConfigData &reward_item = cfg_item->personal_rank_reward[i];
			const ItemBase *item_base = ITEMPOOL->GetItem(reward_item.item_id);

			if (NULL != item_base)
			{
				// 发邮件
				static MailContentParam contentparam; contentparam.Reset();
				contentparam.item_list[0].item_id = reward_item.item_id;
				contentparam.item_list[0].num = reward_item.num;
				contentparam.item_list[0].is_bind = (reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_csa_rank_reward_content,
					CSA_SUB_TYPE_BOSS, i + 1);
				if (length > 0)
				{
					MailRoute::MailToUser(IntToUid(role_id), SYSTEM_MAIL_REASON_INVALID, contentparam);

					gamelog::g_log_combine_server.printf(LL_INFO, "OnCSABossRankReward rank_type %d, user %d %s, "
						"rank_value %d, rank %d, item_detail %s",
						CSA_SUB_TYPE_BOSS, user_node->uid, user_node->user_name,
						m_csa_boss_rank_param.personal_rank[i].rank_value, i, ItemConfigDataLog(&cfg_item->personal_rank_reward[i], NULL));
				}
			}
		}
	}

	int guild_id = m_csa_boss_rank_param.guild_rank[0].id;
	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
	if (NULL != guild)
	{
		int master_id = guild->GetGuildMemberManager().GetTuanZhangUid();
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(master_id);
		if (NULL != user_node)
		{
			{	// 盟主奖励
				const ItemConfigData &reward_item = cfg_item->master_reward;
				const ItemBase *item_base = ITEMPOOL->GetItem(reward_item.item_id);

				if (NULL != item_base)
				{
					// 发邮件
					static MailContentParam contentparam; contentparam.Reset();
					contentparam.item_list[0].item_id = reward_item.item_id;
					contentparam.item_list[0].num = reward_item.num;
					contentparam.item_list[0].is_bind = (reward_item.is_bind) ? 1 : 0;
					contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();


					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_csa_boss_guild_rank_master_reward_content);
					if (length > 0)
					{
						MailRoute::MailToUser(IntToUid(master_id), SYSTEM_MAIL_REASON_INVALID, contentparam);

						gamelog::g_log_combine_server.printf(LL_INFO, "CSAChangeXianmengzhanWiner user %d %s, item_detail %s",
							user_node->uid, user_node->user_name, ItemConfigDataLog(&reward_item, NULL));
					}
				}
			}

			{
				// 仙盟成员奖励
				const ItemConfigData &reward_item = cfg_item->member_reward;
				const ItemBase *item_base = ITEMPOOL->GetItem(reward_item.item_id);

				if (NULL != item_base)
				{
					// 发邮件
					static MailContentParam contentparam; contentparam.Reset();
					contentparam.item_list[0].item_id = reward_item.item_id;
					contentparam.item_list[0].num = reward_item.num;
					contentparam.item_list[0].is_bind = (reward_item.is_bind) ? 1 : 0;
					contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_csa_boss_guild_rank_member_reward_content);
					if (length > 0)
					{
						guild->GetGuildMemberManager().SystemMailToAll(contentparam, false);
					}
				}
			}
		}
	}
}

void CombineServerActivityManager::OnSendCSABossRankInfo(Role *role)
{
	static Protocol::SCCSABossRankInfo rank_info;
	for (int i = 0; i < COMBINE_SERVER_BOSS_RANK_NUM; ++i)
	{
		int role_id = m_csa_boss_rank_param.personal_rank[i].id;
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(role_id);
		if (NULL != user_node)
		{
			rank_info.personal_rank[i].id = role_id;
			rank_info.personal_rank[i].rank_value = m_csa_boss_rank_param.personal_rank[i].rank_value;
			user_node->GetName(rank_info.personal_rank[i].name);
		}
		else
		{
			rank_info.personal_rank[i].id = 0;
			rank_info.personal_rank[i].rank_value = 0;
			memset(rank_info.personal_rank[i].name, 0, sizeof(GameName));
		}

		int guild_id = m_csa_boss_rank_param.guild_rank[i].id;
		Guild *guild = GuildManager::Instance().GetGuild(guild_id);
		if (NULL != guild)
		{
			rank_info.guild_rank[i].id = guild_id;
			rank_info.guild_rank[i].rank_value = m_csa_boss_rank_param.guild_rank[i].rank_value;
			guild->GetGuildName(rank_info.guild_rank[i].name);
		}
		else
		{
			rank_info.guild_rank[i].id = 0;
			rank_info.guild_rank[i].rank_value = 0;
			memset(rank_info.guild_rank[i].name, 0, sizeof(GameName));
		}
	}

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rank_info, sizeof(rank_info));
	}
	else
	{
		SpecialCombineServerBoss *csa_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCSABoss(1);
		if (NULL != csa_boss)
		{
			Scene *csa_boss_scene = csa_boss->GetScene();
			if (NULL != csa_boss_scene)
			{
				csa_boss_scene->SendToRole((const char *)&rank_info, sizeof(rank_info));
			}
		}
	}
}

void CombineServerActivityManager::OnCombineServerActivitySyncValue(int rank_type, int role_id, long long rank_value, long long rank_reserve)
{
	if (!SpecialActivityManager::Instance().IsSpecialActivityOpening(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER)) return;
	if (rank_type <= CSA_RANK_TYPE_INVALID || rank_type >= CSA_RANK_TYPE_MAX) return;

	const CSARankRewardConfig *rank_reward_cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetCSARankRewardCfgByRankType(rank_type);
	if (NULL == rank_reward_cfg || rank_value < rank_reward_cfg->rank_limit) return;

	//if (0 == m_csa_rank_param.rank_item_list[rank_type].is_finish)
	{
		this->UpdateRankUser(rank_type, role_id, rank_value, rank_reserve);
	}
}

bool MaxRankUser(const CombineServerActivityRankUser &item_1, const CombineServerActivityRankUser &item_2)
{
	if (item_1.rank_value != item_2.rank_value)
	{
		return item_1.rank_value > item_2.rank_value;
	}

	return item_1.rank_reserve > item_2.rank_reserve;
}

void CombineServerActivityManager::UpdateRankUser(int rank_type, int role_id, long long rank_value, long long rank_reserve)
{
	if (rank_type <= CSA_RANK_TYPE_INVALID || rank_type >= CSA_RANK_TYPE_MAX || 0 == role_id || rank_value <= 0) return;

	CombineServerActivityRankItem &rank_item = m_csa_rank_param.rank_item_list[rank_type];
	
	//if (0 != rank_item.is_finish) return;

//	int free_slot_idx = COMBINE_SERVER_CONSUME_RANK_COUNT;
	int min_slot_idx = 0;
	bool is_need_change_minslot = true;   // 是否需要更新该玩家信息到哨卡位置，ture为需要，false为不需要

	for (int i = 0; i < COMBINE_SERVER_CONSUME_RANK_COUNT; ++ i)
	{
		CombineServerActivityRankUser &rank_user = rank_item.user_list[i];

		// 原来榜里有该玩家，更新数据
		if (rank_user.role_id == role_id)
		{
			if (rank_value > rank_user.rank_value)
			{
				rank_user.rank_value = rank_value;
				rank_user.rank_reserve = rank_reserve;
			}
			is_need_change_minslot = false;
			break;   // 应该用break，后续用到这个排行榜时就不用再排序了,后续再优化
		}
	
//		if (free_slot_idx < 0 || free_slot_idx >= COMBINE_SERVER_CONSUME_RANK_COUNT)
//		{
//			if (rank_user.IsFree())
//			{
//				free_slot_idx = i;
//			}
//		}

        // 如果榜里没满，就往该位置更新该玩家数据
		if (rank_user.IsFree())
		{
			rank_user.role_id = role_id;
			rank_user.rank_value = rank_value;
			rank_user.rank_reserve = rank_reserve;
			is_need_change_minslot = false;
			break;
		}

		// 如果当前位置的值比哨卡位置的值要小，变更哨卡位置为当前的位置，哨卡位置的值是榜上最低名次
		if (MaxRankUser(rank_item.user_list[min_slot_idx], rank_user))
		{
			min_slot_idx = i;
		}
	}

//	if (free_slot_idx >= 0 && free_slot_idx < COMBINE_SERVER_CONSUME_RANK_COUNT)
//	{
//		rank_item.user_list[free_slot_idx].role_id = role_id;
//		rank_item.user_list[free_slot_idx].rank_value = rank_value;
//		rank_item.user_list[free_slot_idx].rank_reserve = rank_reserve;
//		return;
//	}

	// 如果该值比最小名次的值要大并且该玩家信息没有被跟新到榜里，变更该玩家信息到该名次
	if (is_need_change_minslot)
	{
		if (rank_value > rank_item.user_list[min_slot_idx].rank_value)
		{
			rank_item.user_list[min_slot_idx].role_id = role_id;
			rank_item.user_list[min_slot_idx].rank_value = rank_value;
			rank_item.user_list[min_slot_idx].rank_reserve = rank_reserve;
			//		return;
		}
	}
//	if (rank_value > rank_item.user_list[min_slot_idx].rank_value)
//	{
//		rank_item.user_list[min_slot_idx].role_id = role_id;
//		rank_item.user_list[min_slot_idx].rank_value = rank_value;
//		rank_item.user_list[min_slot_idx].rank_reserve = rank_reserve;
////		return;
//	}

	qsort(rank_item.user_list, COMBINE_SERVER_CONSUME_RANK_COUNT, sizeof(rank_item.user_list[0]), ComparaCSAI);
}

void CombineServerActivityManager::CheckRankReward(int rank_type)
{
	if (rank_type <= CSA_RANK_TYPE_INVALID || rank_type >= CSA_RANK_TYPE_MAX) return;

	std::vector<CombineServerActivityRankUser> rank_user_vector;

	for (int i = 0; i < COMBINE_SERVER_ACTIVITY_RANK_REWARD_ROLE_NUM; ++ i)
	{
		if (!m_csa_rank_param.rank_item_list[rank_type].user_list[i].IsFree())
		{
			rank_user_vector.push_back(m_csa_rank_param.rank_item_list[rank_type].user_list[i]);
		}
	}

	std::sort(rank_user_vector.begin(), rank_user_vector.end(), &MaxRankUser);

	const CSARankRewardConfig *cfg_item = LOGIC_CONFIG->GetCombineServerActivityConfig().GetCSARankRewardCfgByRankType(rank_type);
	if (NULL == cfg_item) 
	{
		return;
	}

	int rank = 0;
	int all_name_len = 0;
	char all_name_str[512] = {0};

	for (std::vector<CombineServerActivityRankUser>::iterator it = rank_user_vector.begin(); it != rank_user_vector.end() && rank < COMBINE_SERVER_ACTIVITY_RANK_REWARD_ROLE_NUM; ++ it)
	{
		if (CSA_RANK_TYPE_QIANGGOU == rank_type && rank > 0) //策划需求，抢购排行仅仅发给第一名奖励
		{
			break;
		}

		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(it->role_id);
		if (NULL != user_node)
		{
			const ItemConfigData &reward_item = cfg_item->rank_reward_items[rank];
			const ItemBase *item_base = ITEMPOOL->GetItem(reward_item.item_id);

			if (NULL != item_base)
			{
				// 发邮件
				static MailContentParam contentparam; contentparam.Reset();
				contentparam.item_list[0].item_id = reward_item.item_id;
				contentparam.item_list[0].num = reward_item.num;
				contentparam.item_list[0].is_bind = (reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_csa_rank_reward_content, 
					cfg_item->sub_type, rank + 1);
				if (length > 0)
				{
					MailRoute::MailToUser(IntToUid(it->role_id), SYSTEM_MAIL_REASON_INVALID, contentparam);

					gamelog::g_log_combine_server.printf(LL_INFO, "CheckRankReward rank_type %d, user %d %s, "
						"rank_value %lld, rank_reserve %lld, rank %d, item_detail %s",
						rank_type, user_node->uid, user_node->user_name, 
						it->rank_value, it->rank_reserve, rank, ItemConfigDataLog(&cfg_item->rank_reward_items[rank], NULL));
				}
			}

			{
				// 拼接名字
				char member_name_str[64] = {0};
				int namelen = SNPRINTF(member_name_str, sizeof(member_name_str), "{r;%d;%s;%d} ", user_node->uid, user_node->user_name, user_node->camp);
				if (namelen > 0 && all_name_len + namelen < (int)sizeof(all_name_str))
				{
					STRNCPY(all_name_str + all_name_len, member_name_str, sizeof(all_name_str) - all_name_len);
					all_name_len += namelen;
				}
			}

			++ rank;
		}
	}

	if (all_name_len > 0)
	{
		all_name_str[sizeof(all_name_str) - 1] = '\0';

		int notice_len = 0;
		{
			switch (cfg_item->sub_type)
			{
			case CSA_SUB_TYPE_RANK_QIANGGOU:
				{
					notice_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_csa_rank_reward_notice_qianggou, 
						all_name_str, cfg_item->rank_reward_items[0].item_id);
				}
				break;
			}
		}
		if (notice_len > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, notice_len, SYS_MSG_CENTER_ROLL);
		}
	}
}


void CombineServerActivityManager::ResetData()
{
	m_csa_other_param.Reset();
	m_csa_rank_param.Reset();
}

void CombineServerActivityManager::ResetRankList(int rank_type)
{
	CombineServerActivityRankUser *rank_list = m_csa_rank_param.rank_item_list[rank_type].user_list;
	if (NULL != rank_list)
	{
		for (int index = 0; index < COMBINE_SERVER_CONSUME_RANK_COUNT; ++ index)
		{
			rank_list[index].Reset();
		}
	}
}

