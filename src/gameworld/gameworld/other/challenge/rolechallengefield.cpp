#include "rolechallengefield.hpp"
#include "obj/character/role.h"
#include "protocal/msgactivity.hpp"
#include "servercommon/commondata.hpp"
#include "other/capability/capability.hpp"
//#include "other/card/cardconfig.h"
#include "config/logicconfigmanager.hpp"
#include "global/challengefield/challengefieldconfig.hpp"
#include "item/knapsack.h"
#include "global/challengefield/challengefield.hpp"
#include "global/challengefield/challengeuser.hpp"
#include "servercommon/errornum.h"
#include "other/shop/roleshop.hpp"
#include "engineadapter.h"
#include "config/sharedconfig/sharedconfig.h"
#include "other/cross/rolecross.h"

static const int BEST_RANK_BREAK_MAX_LEVEL = 10;       // 开放的最大等级

RoleChallengeField::RoleChallengeField()
{

}

RoleChallengeField::~RoleChallengeField()
{

}

void RoleChallengeField::SendRoleCFBestRankBreakInfo()
{
	ChallengeUser *challenge_user = ChallengeField::Instance().GetChallengeUser(m_role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == challenge_user)
	{
		return; 
	}

	static Protocol::SCChallengeFieldBestRankBreakInfo cfbrbr;
	cfbrbr.best_rank_break_level = m_role->GetCommonDataParam()->challenge_field_best_rank_break_level;
	cfbrbr.best_rank_pos = challenge_user->GetBestRankPos();

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cfbrbr, sizeof(cfbrbr));
}

void RoleChallengeField::RoleCFBestRankBreak()
{
	if (!ChallengeField::Instance().IsChallengeOpen())
	{
		m_role->NoticeNum(errornum::EN_CHALLENGE_FIELD_IS_CLOSED);
		return;
	}

	//策划需求，只在开服前三天开放突破功能
	//int open_day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
	//if (open_day_index > 3) 
	//{
	//	return;
	//}

	if ((m_role->GetCommonDataParam()->challenge_field_best_rank_break_level + 1) > BEST_RANK_BREAK_MAX_LEVEL)
	{
		return;
	}

	const ChallengeFieldBestRankPosCfg *break_level_cfg = LOGIC_CONFIG->GetChallengeFieldCfg().GetBestRankPosCfgByLeveL(
		m_role->GetCommonDataParam()->challenge_field_best_rank_break_level);
	if (nullptr == break_level_cfg)
	{
		return;
	}

	const ChallengeFieldBestRankPosCfg *next_break_level_cfg = LOGIC_CONFIG->GetChallengeFieldCfg().GetBestRankPosCfgByLeveL(
		m_role->GetCommonDataParam()->challenge_field_best_rank_break_level + 1);
	if (nullptr == next_break_level_cfg)
	{
		return;
	}

	ChallengeUser *challenge_user = ChallengeField::Instance().GetChallengeUser(m_role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == challenge_user)
	{
		return;
	}

	if (challenge_user->GetBestRankPos() > next_break_level_cfg->rank_pos)
	{
		m_role->NoticeNum(errornum::EN_CHALLENGE_FIELD_RANK_POS_LIMIT);
		return;
	}

	++m_role->GetCommonDataParam()->challenge_field_best_rank_break_level;

	m_role->GetRoleShop()->AddChestShopGuanghui(next_break_level_cfg->reward_guanghui, "BestRankPosBreakReward");
	m_role->GetKnapsack()->PutListOrMail(break_level_cfg->reward_item, PUT_REASON_CHALLENGE_FIELD_BEST_RANK_BREAK);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHALLENGE_FIELD_BEST_RANK_BREAK);
	this->SendRoleCFBestRankBreakInfo();
}

void RoleChallengeField::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		{
			// 突破等级属性
			const ChallengeFieldBestRankPosCfg *break_level_cfg = LOGIC_CONFIG->GetChallengeFieldCfg().GetBestRankPosCfgByLeveL(
				m_role->GetCommonDataParam()->challenge_field_best_rank_break_level);
			if (nullptr != break_level_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += break_level_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += break_level_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += break_level_cfg->fangyu;
				//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += break_level_cfg->mingzhong;
				//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += break_level_cfg->shanbi;
				//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += break_level_cfg->baoji;
				//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += break_level_cfg->jianren;
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_CF_BEST_RANK_BREAK, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}
