#include "crosschallengeuser.hpp"

#include "config/logicconfigmanager.hpp"
#include "servercommon/string/globalstr.h"
#include "servercommon/struct/global/challengeuserparam.hpp"
#include "item/itempool.h"
#include "global/crosschallengefield/crosschallengefield.hpp"
#include "global/challengefield/challengefieldconfig.hpp"
#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "gamelog.h"
#include "world.h"
#include "protocal/msgactivity.hpp"
#include "obj/character/role.h"
#include "servercommon/string/gameworldstr.h"
#include "item/knapsack.h"
#include "other/shop/roleshop.hpp"
#include "other/role_module.hpp"

CrossChallengeUser::CrossChallengeUser(CrossChallengeField *challenge_field)
	: m_challenge_field(challenge_field), m_rankpos(-1), m_user_id(INVALID_USER_ID), m_unique_user_id(INVALID_UNIQUE_USER_ID), m_curr_opponent_idx(0)
{

}

CrossChallengeUser::~CrossChallengeUser()
{

}

void CrossChallengeUser::Init(const ChallengeUserListParam::ChallengeUserAttr &attr)
{
	m_rankpos = attr.rankpos;
	m_unique_user_id = LongLongToUniqueUserID(attr.role_id);
	m_user_id = (m_unique_user_id.user_id);
	m_challenge_p = attr.challenge_p;
	m_common_data = attr.common_data;
	m_report_info = attr.report_info;
	m_role_appearance = attr.role_appearance;
}

void CrossChallengeUser::GetAttr(ChallengeUserListParam::ChallengeUserAttr *attr)
{	
	attr->rankpos = m_rankpos;
	attr->role_id = UniqueUserIDToLongLong(m_unique_user_id);
	attr->challenge_p = m_challenge_p;
	attr->common_data = m_common_data;
	attr->report_info = m_report_info;
	attr->role_appearance = m_role_appearance;
}

void CrossChallengeUser::SetRankPos(int rankpos, bool is_init, bool need_reset_opponen)
{
	m_rankpos = rankpos;

	if (need_reset_opponen)
	{
		//
		CrossChallengeField::Instance().RandOpponentHelper(m_unique_user_id);
	}
	else
	{
		this->SendChallengeUserInfo();
	}
}

void CrossChallengeUser::SetLastOnlineTime()
{
	m_common_data.last_online_time = static_cast<long long>(EngineAdapter::Instance().Time());
	m_challenge_field->SetDirty(m_rankpos);
}

void CrossChallengeUser::OnJoinChallengeField()
{
	++ m_common_data.join_times;
	m_challenge_field->SetDirty(m_rankpos);

	this->SendChallengeUserInfo();

	Role *role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(UniqueUserIDToLongLong(m_unique_user_id));
	if (nullptr != role)
	{
		role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_CHALLENGEFIELD_TIMES, 1);
	}
}

void CrossChallengeUser::AddBuyJoinTimes(int add_value)
{
	if (add_value > 0)
	{
		m_common_data.buy_join_times += add_value;
		m_challenge_field->SetDirty(m_rankpos);

		this->SendChallengeUserInfo();
	}
}

void CrossChallengeUser::AddJifen(int add_value)
{
	if (IsRobot())
	{
		return;
	}

	if (add_value > 0)
	{
		m_common_data.jifen += add_value;
		m_challenge_field->SetDirty(m_rankpos);

		this->SendChallengeUserInfo();
	}
}

void CrossChallengeUser::OnFetchJifenReward(int seq)
{
	Role *role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(UniqueUserIDToLongLong(m_unique_user_id));
	if (NULL == role)
	{
		return;
	}

	UNSTD_STATIC_CHECK(CHALLENGE_FIELD_JIFEN_REWARD_MAX_COUNT <= (int)sizeof(m_common_data.jifen_reward_flag) * 8);

	if (seq < 0 || seq >= CHALLENGE_FIELD_JIFEN_REWARD_MAX_COUNT || seq >= (int)sizeof(m_common_data.jifen_reward_flag) * 8)
	{
		return;
	}

	if (0 != (m_common_data.jifen_reward_flag & (1 << seq)))
	{
		role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
		return;
	}

	const ChallengeFieldJifenRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetChallengeFieldCfg().GetJifenCfg(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_common_data.jifen < cfg_item->need_jifen)
	{
		role->NoticeNum(errornum::EN_CHALLENGE_FIELD_JIFEN_LIMIT);
		return;
	}

	const ChallengeFieldJifenRewardConfig::DetailRewardConfig *detail_reward = cfg_item->GetDetailReward(role->GetLevel());
	if (NULL == detail_reward)
	{
		return;
	}

	bool has_item = false;
	if (NULL != ITEMPOOL->GetItem(detail_reward->reward_item.item_id))
	{
		has_item = true;
	}

	if (has_item && !role->GetKnapsack()->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_common_data.jifen_reward_flag |= (1 << seq);
	m_challenge_field->SetDirty(m_rankpos);

	if (detail_reward->reward_guanghui > 0)
	{
		role->GetRoleShop()->AddChestShopGuanghui(detail_reward->reward_guanghui, "ChallengeFieldJifenReward");
	}
	
	if (has_item)
	{
		role->GetKnapsack()->Put(detail_reward->reward_item, PUT_REASON_CHALLENGE_FIELD);
	}

	this->SendChallengeUserInfo();

	gamelog::g_log_challengefield.printf(LL_INFO, "OnFetchJifenReward::user[%d %s] seq[%d] flag[%d], "
		"add_guanghui[%d], has_item[%d], item[%s]", 
		role->GetUID(), role->GetName(), seq, m_common_data.jifen_reward_flag, 
		detail_reward->reward_guanghui, (has_item ? 1 : 0), ItemConfigDataLog(&detail_reward->reward_item, NULL));
}

void CrossChallengeUser::AddGuangHui(int add_value, const char *reason)
{
	if (IsRobot())
	{
		return;
	}

	if (NULL == reason)
	{
		return;
	}

	int old_guanghui = m_common_data.reward_guanghui;

	m_common_data.reward_guanghui += add_value;
	if (m_common_data.reward_guanghui < 0)
	{
		m_common_data.reward_guanghui = 0;
	}

	m_challenge_field->SetDirty(m_rankpos);

	if (!IsRobot())
	{
		LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_CHALLENGE_FIELD_GUANGHUI, UidToInt(this->GetUserID()), this->GetName(), NULL, 
			add_value, m_common_data.reward_guanghui, reason, NULL, old_guanghui, 0, 0, 0);
	}
}

void CrossChallengeUser::AddBindGold(int add_value, const char *reason)
{
	if (IsRobot())
	{
		return;
	}

	if (NULL == reason)
	{
		return;
	}

	int old_bind_gold = m_common_data.reward_bind_gold;

	m_common_data.reward_bind_gold += add_value;
	if (m_common_data.reward_bind_gold < 0)
	{
		m_common_data.reward_bind_gold = 0;
	}

	m_challenge_field->SetDirty(m_rankpos);

	if (!IsRobot())
	{
		LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_CHALLENGE_FIELD_BIND_GOLD, UidToInt(this->GetUserID()), this->GetName(), NULL, 
			add_value, m_common_data.reward_bind_gold, reason, NULL, old_bind_gold, 0, 0, 0);
	}
}

void CrossChallengeUser::OnWin(bool is_broadcast)
{
	++ m_common_data.liansheng;
//	m_challenge_field->SetDirty(m_rankpos);
 
	if (is_broadcast && m_common_data.liansheng >= 5)
	{
		int len = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "cross_challengefield_liansheng_notice",
			UidToInt(this->GetUserID()), this->GetName(), (int)this->GetCamp(), m_common_data.liansheng);
		if (len > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
		}
	}
}

void CrossChallengeUser::OnFail()
{
	m_common_data.liansheng = 0;
//	m_challenge_field->SetDirty(m_rankpos);
}

void CrossChallengeUser::AddBuyBuffTimes(int add_value)
{
	if (add_value > 0)
	{
		m_common_data.buy_buff_times += add_value;
		m_challenge_field->SetDirty(m_rankpos);

		this->SendChallengeUserInfo();
	}
}

void CrossChallengeUser::AddRewradDay(int add_value)
{
	m_common_data.reward_day += add_value;
	m_challenge_field->SetDirty(m_rankpos);
}

void CrossChallengeUser::AddRewardItem(ItemID item_id, short item_num, bool is_bind)
{
	const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
	if (NULL == itembase) return;

	for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
	{
		if (m_common_data.reward_item_list[i].item_id == item_id && m_common_data.reward_item_list[i].is_bind == is_bind)
		{
			m_common_data.reward_item_list[i].num += item_num;
			if (m_common_data.reward_item_list[i].num > itembase->GetPileLimit())
			{
				item_num = m_common_data.reward_item_list[i].num - itembase->GetPileLimit();
				m_common_data.reward_item_list[i].num = itembase->GetPileLimit();
			}
			else
			{
				item_num = 0;
				break;
			}
		}
		else if (0 == m_common_data.reward_item_list[i].item_id)
		{
			m_common_data.reward_item_list[i].item_id = item_id;
			m_common_data.reward_item_list[i].num = item_num;
			m_common_data.reward_item_list[i].is_bind = is_bind;
			break;
		}
	}

	m_challenge_field->SetDirty(m_rankpos);
}

const ItemConfigData *CrossChallengeUser::GetRewardItem(int *count)
{
	if (NULL == count) return NULL;

	*count = 0;
	for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
	{
		if (0 != m_common_data.reward_item_list[i].item_id)
		{
			(*count)++;
		}
	}
	return m_common_data.reward_item_list;
}

void CrossChallengeUser::ClearRewardItem()
{
	memset(m_common_data.reward_item_list, 0, sizeof(m_common_data.reward_item_list));
}

void CrossChallengeUser::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		m_common_data.OnResetData();
		m_challenge_field->SetDirty(m_rankpos);
		this->SendChallengeUserInfo();
	}
}

void CrossChallengeUser::OnSyncChallengeParam(const ChallengeParam &challenge_p, const RoleAppearance &apparance)
{
	m_challenge_p = challenge_p;
	m_role_appearance = apparance;

	m_challenge_field->SetDirty(m_rankpos);
}

void CrossChallengeUser::ResetOpponentUserList(CrossChallengeFieldOpponentInfo user_list[CHALLENGE_FIELD_OPPONENT_MAX])
{
	m_curr_opponent_idx = 0;
	memcpy(m_opponent_info_list, user_list, sizeof(m_opponent_info_list));

	this->SendChallengeUserInfo();
}

bool CrossChallengeUser::SetOpponentUser(int index)
{
	if (index >= 0 && index < CHALLENGE_FIELD_OPPONENT_MAX && INVALID_UNIQUE_USER_ID != m_opponent_info_list[index].user_id)
	{
		m_curr_opponent_idx = index;
		this->SendChallengeUserInfo();

		return true;
	}

	return false;
}

UniqueUserID CrossChallengeUser::GetCurrOpponentUserID()
{
	if (m_curr_opponent_idx >= 0 && m_curr_opponent_idx < CHALLENGE_FIELD_OPPONENT_MAX)
	{
		return m_opponent_info_list[m_curr_opponent_idx].user_id;
	}

	return INVALID_UNIQUE_USER_ID;
}

void CrossChallengeUser::SendChallengeReportInfo()
{
	Role *role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(UniqueUserIDToLongLong(m_unique_user_id));
	if (NULL != role)
	{
		Protocol::SCCrossChallengeFieldReportInfo cfri;
		cfri.report_count = 0;

		for (int i = 0; i < m_report_info.reprot_count && cfri.report_count < CHALLENGE_FIELD_REPORT_MAX_COUNT; ++ i)
		{
			Protocol::SCCrossChallengeFieldReportInfo::ReportItem &report_item = cfri.report_list[cfri.report_count];
			report_item.challenge_time = m_report_info.report_list[i].challenge_time;
			report_item.target_uid = m_report_info.report_list[i].target_uid;
			report_item.is_sponsor = m_report_info.report_list[i].is_sponsor;
			report_item.is_win = m_report_info.report_list[i].is_win;
			report_item.reserve_sh = m_report_info.report_list[i].reserve_sh;
			report_item.old_rankpos = m_report_info.report_list[i].old_rankpos;
			report_item.new_rankpos = m_report_info.report_list[i].new_rankpos;
			F_STRNCPY(report_item.target_name, m_report_info.report_list[i].target_name, sizeof(report_item.target_name));
			++ cfri.report_count;
		}

		int send_len = sizeof(cfri) - sizeof(cfri.report_list[0]) * (CHALLENGE_FIELD_REPORT_MAX_COUNT - cfri.report_count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cfri, send_len);
	}
}

void CrossChallengeUser::SendChallengeUserInfo()
{
	Role *role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(UniqueUserIDToLongLong(m_unique_user_id));
	if (NULL != role)
	{
		Protocol::SCCrossChallengeFieldUserInfo cfui;
		cfui.rank_pos = m_rankpos;
		cfui.curr_opponent_idx = m_curr_opponent_idx;
		cfui.join_times = m_common_data.join_times;
		cfui.buy_join_times = m_common_data.buy_join_times;
		cfui.jifen = m_common_data.jifen;
		cfui.jifen_reward_flag = m_common_data.jifen_reward_flag;
		cfui.reward_guanghui = m_common_data.reward_guanghui;
		cfui.reward_bind_gold = m_common_data.reward_bind_gold;
		cfui.liansheng = m_common_data.liansheng;
		cfui.buy_buff_times = m_common_data.buy_buff_times;
		cfui.best_rank_pos = m_common_data.best_rankpos;
		cfui.free_day_times = LOGIC_CONFIG->GetChallengeFieldCfg().GetRoleFreeJoinTimesByCreateTime(role->GetCreateRoleTimestamp());

		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
		{
			cfui.reward_item_list[i].item_id = m_common_data.reward_item_list[i].item_id;
			cfui.reward_item_list[i].num = m_common_data.reward_item_list[i].num;
		}
		
		for (int i = 0; i < CHALLENGE_FIELD_OPPONENT_MAX; ++ i)
		{
			if (INVALID_UNIQUE_USER_ID == m_opponent_info_list[i].user_id)
			{
				cfui.opponent_userid_list[i].user_id = 0;
			}
			else
			{
				cfui.opponent_userid_list[i].user_id = UniqueUserIDToLongLong(m_opponent_info_list[i].user_id);
			}

			cfui.opponent_userid_list[i].rank_pos = m_opponent_info_list[i].rank_pos;
		}

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cfui, sizeof(cfui));

		this->OnGetOpponentDetailInfo();
	}
}

void CrossChallengeUser::OnGetOpponentDetailInfo() 
{
	Role *role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(UniqueUserIDToLongLong(m_unique_user_id));
	if (NULL != role)
	{
		Protocol::SCCrossChallengeFieldOpponentInfo cfoi;
		cfoi.opponent_count = 0;

		for (int i = 0; i < CHALLENGE_FIELD_OPPONENT_MAX && cfoi.opponent_count < CHALLENGE_FIELD_OPPONENT_MAX; ++ i)
		{
			if (INVALID_UNIQUE_USER_ID == m_opponent_info_list[i].user_id)
			{
				continue;
			}

			CrossChallengeUser *opponent_user = m_challenge_field->GetChallengeUser(m_opponent_info_list[i].user_id);
			if (NULL == opponent_user)
			{
				continue;
			}

			Protocol::SCCrossChallengeFieldOpponentInfo::OpponentInfo &opponent_info = cfoi.opponent_info_list[cfoi.opponent_count];
			const ChallengeParam &opponent_param = opponent_user->GetChallengeParam();
			const RoleAppearance &opponent_appearance = opponent_user->GetChallengeAppearance();

			opponent_info.server_id = opponent_param.server_id;
			opponent_info.user_id = UniqueUserIDToLongLong(opponent_user->GetUniqueUserID());
			opponent_info.avatar_timestamp = opponent_param.avatar_timestamp;
			opponent_info.avatar = opponent_param.avatar;
			opponent_user->GetName(opponent_info.name);
			opponent_info.prof = opponent_param.prof;
			opponent_info.sex = opponent_param.sex;
			opponent_info.camp = opponent_param.camp;
			opponent_info.capability = opponent_param.capability;

			opponent_info.appearance = opponent_appearance;

			if (!opponent_user->IsRobot())
			{
				Role *opponent_role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(UniqueUserIDToLongLong(m_unique_user_id));
				if (NULL != opponent_role)
				{
					opponent_info.best_rank_break_level = opponent_role->GetCommonDataParam()->challenge_field_best_rank_break_level;
				}
				else
				{
					opponent_info.best_rank_break_level = 0;
				}
			}
			else
			{
				opponent_info.best_rank_break_level = opponent_user->GetRankPos();	//??????????????
			}

			++ cfoi.opponent_count;
		}

		int send_len = sizeof(cfoi) - (CHALLENGE_FIELD_OPPONENT_MAX - cfoi.opponent_count) * sizeof(cfoi.opponent_info_list[0]);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cfoi, send_len);
	}
}

void CrossChallengeUser::AddReport(const ChallengeUserReport::ReportItem &report_item)
{
	if (m_report_info.reprot_count < CHALLENGE_FIELD_REPORT_MAX_COUNT)
	{
		m_report_info.report_list[m_report_info.reprot_count] = report_item;
		++ m_report_info.reprot_count;
	}
	else
	{
		int oldest_report_idx = 0;

		for (int i = 1; i < CHALLENGE_FIELD_REPORT_MAX_COUNT; ++ i)
		{
			if (m_report_info.report_list[i].challenge_time < m_report_info.report_list[oldest_report_idx].challenge_time)
			{
				oldest_report_idx = i;
			}
		}

		if (oldest_report_idx >= 0 && oldest_report_idx < CHALLENGE_FIELD_REPORT_MAX_COUNT)
		{
			m_report_info.report_list[oldest_report_idx] = report_item;
		}
	}

	m_challenge_field->SetDirty(m_rankpos);

	this->SendChallengeReportInfo();
}


