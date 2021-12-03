#include "rolefbtuitu.h"
#include "servercommon/errornum.h"

#include "engineadapter.h"
#include "obj/character/role.h"

#include "gameworld/other/fb/fbtuituconfig.h"
#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "monster/drop/dropconfig.hpp"
#include "monster/monsterpool.h"

#include "gameworld/gameworld/protocal/msgfb.h"
#include "item/knapsack.h"
#include "other/vip/vip.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/event/eventhandler.hpp"

RoleTuituFb::RoleTuituFb() : m_role(NULL)
{
	
}

RoleTuituFb::~RoleTuituFb()
{
	
}

void RoleTuituFb::Init(Role *role, const TuituFbParam &param)
{
	m_role = role;
	m_param = param;
}

void RoleTuituFb::GetInitParam(TuituFbParam *param)
{
	*param = m_param;
}

void RoleTuituFb::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	m_param.ResetData();
}

void RoleTuituFb::OnTuituOperaReq(Protocol::CSTuituFbOperaReq *tfor)
{
	switch (tfor->opera_type)
	{
	case Protocol::TUITU_FB_OPERA_REQ_TYPE_ALL_INFO:
		{
			this->SendFbInfo();
		}
		break;

	case Protocol::TUITU_FB_OPERA_REQ_TYPE_BUY_TIMES:
		{
			this->OnBuyTimes(tfor->param_1, tfor->param_2);
		}
		break;

	case Protocol::TUITU_FB_OPERA_REQ_TYPE_FETCH_STAR_REWARD:
		{
			this->OnFetchStarReward(tfor->param_1, tfor->param_2);
		}
		break;

	case Protocol::TUITU_FB_OPERA_REQ_TYPE_SAODANG:
		{
			this->OnSaodangFb(tfor->param_1, tfor->param_2, tfor->param_3);
		}
		break;
	}
}

bool RoleTuituFb::CanEnterFb(int fb_type, int chapter, int level)
{
	const TuituFbInfoConfig::Layer *layer_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetLayerCfg(fb_type, chapter, level);
	if (NULL == layer_cfg)
	{
		return false;
	}

	if (m_role->GetLevel() < layer_cfg->enter_level_limit)
	{
		m_role->NoticeNum(errornum::EN_FB_ROLE_LEVEL_LIMIT);
		return false;
	}
	
	if (fb_type < 0 || fb_type >= TUTUI_FB_TYPE_NUM_MAX
		|| chapter < 0 || chapter >= TUITU_FB_CHAPER_NUM_MAX
		|| level < 0 || level >= TUITU_FB_LEVLE_NUM_MAX)
	{
		return false;
	}

	TuituFbParam::FbInfo &fb_info = m_param.fb_list[fb_type];

	//普通副本第一章第一关不检查
	if (chapter != 0 || level != 0)
	{
		const int need_fb_type = layer_cfg->need_pass_fb_type;
		const int need_fb_chapter = layer_cfg->need_pass_chapter;
		const int need_fb_level = layer_cfg->need_pass_level;

		if (need_fb_type >= 0 && need_fb_type < TUTUI_FB_TYPE_NUM_MAX
			&& need_fb_chapter >= 0 && need_fb_chapter < TUITU_FB_CHAPER_NUM_MAX
			&& need_fb_level >= 0 && need_fb_level < TUITU_FB_LEVLE_NUM_MAX
			&& m_param.fb_list[need_fb_type].chapter_list[need_fb_chapter].level_list[need_fb_level].pass_star <= 0)
		{
			m_role->NoticeNum(errornum::EN_TUITU_FB_UNLOCK_LAST_LEVEL_LIMIT);
			return false;
		}

		if (TUITU_FB_TYPE_HARD == fb_type && level > 0)
		{
			if (m_param.fb_list[fb_type].chapter_list[chapter].level_list[level - 1].pass_star < TUITU_HARD_FB_PASS_STAR)
			{
				m_role->NoticeNum(errornum::EN_TUITU_FB_FULL_STAR_PASS_FIRST);
				return false;
			}
		}
	}

	const TuituFbOtherConfig &other_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetOtherCfg();
	//通关过才扣除次数，检查次数前先检查是否通关
	if (TUITU_FB_TYPE_NORMAL == fb_type)
	{
		if (m_param.fb_list[fb_type].chapter_list[chapter].level_list[level].pass_star > 0)
		{
			if (fb_info.today_join_times >= fb_info.buy_join_times + other_cfg.normal_free_join_times)
			{
				m_role->NoticeNum(errornum::EN_TUITU_FB_ENTER_TIMES_LIMIT);
				return false;
			}
		}
	}
	else
	{
		if (m_param.fb_list[fb_type].chapter_list[chapter].level_list[level].pass_star > 0)
		{
			if (fb_info.today_join_times >= fb_info.buy_join_times + other_cfg.hard_free_join_times)
			{
				m_role->NoticeNum(errornum::EN_TUITU_FB_ENTER_TIMES_LIMIT);
				return false;
			}
		}
	}

	m_param.enter_fb_type = static_cast<char>(fb_type);
	m_param.enter_chapter = static_cast<char>(chapter);
	m_param.enter_level = static_cast<char>(level);

	return true;
}

void RoleTuituFb::OnEnterFb(int fb_type, int chapter, int level)
{
	if (fb_type < 0 || fb_type > TUTUI_FB_TYPE_NUM_MAX || chapter < 0 || chapter >= TUITU_FB_CHAPER_NUM_MAX
		|| level < 0 || level >= TUITU_FB_LEVLE_NUM_MAX)
	{
		return;
	}

	//首次通关不扣除次数,已通关过的才扣除次数
	if (m_param.fb_list[fb_type].chapter_list[chapter].level_list[level].pass_star > 0)
	{
		++m_param.fb_list[fb_type].today_join_times;
	}

	this->SendSingleLayerInfo(fb_type, chapter, level);
}

void RoleTuituFb::OnFbFinish(int fb_type, int chapter, int level, int star)
{
	const TuituFbInfoConfig *fb_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetFbinfoCfg(fb_type);
	if (NULL == fb_cfg)
	{
		return;
	}

	if (fb_type < 0 || fb_type >= TUTUI_FB_TYPE_NUM_MAX || chapter < 0 || chapter >= TUITU_FB_CHAPER_NUM_MAX
		|| level < 0 || level >= TUITU_FB_LEVLE_NUM_MAX)
	{
		return;
	}

	TuituFbParam::FbInfo &fb_info = m_param.fb_list[fb_type];
	TuituFbParam::ChapterInfo &chapter_info = fb_info.chapter_list[chapter];

	if (chapter_info.level_list[level].pass_star < star)
	{
		//首次通关:更新已通关大章节关卡
		if ((TUITU_FB_TYPE_NORMAL == fb_type && chapter_info.level_list[level].pass_star <= 0)
			|| (TUITU_FB_TYPE_HARD == fb_type && chapter_info.level_list[level].pass_star < TUITU_HARD_FB_PASS_STAR && star >= TUITU_HARD_FB_PASS_STAR))
		{
			++fb_info.pass_level;
			if (fb_info.pass_level >= fb_cfg->chapter_cfg_list[chapter].max_level)
			{
				++fb_info.pass_chapter;
				if (fb_info.pass_chapter > fb_cfg->max_chapter)
				{
					fb_info.pass_chapter = fb_cfg->max_chapter;
					fb_info.pass_level = fb_cfg->chapter_cfg_list[chapter].max_level;
				}
				else
				{
					fb_info.pass_level = -1;
				}
			}

			const TuituFbInfoConfig::Layer *layer_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetLayerCfg(fb_type, chapter, level);
			if (layer_cfg != NULL)
			{
				m_role->GetKnapsack()->PutListOrMail(layer_cfg->reward_item_list, PUT_REASON_TUITU_FB_FIRST_PASS_REWARD);
			}
		}

		short delta_star = star - chapter_info.level_list[level].pass_star;
		chapter_info.total_star += delta_star;
		chapter_info.level_list[level].pass_star = star;
	}

	//通关：更新下一章节关卡(请求副本里进入下一层用)
	if ((TUITU_FB_TYPE_NORMAL == fb_type && chapter_info.level_list[level].pass_star > 0)
		|| (TUITU_FB_TYPE_HARD == fb_type && chapter_info.level_list[level].pass_star >= TUITU_HARD_FB_PASS_STAR))
	{
		++m_param.enter_level;
		if (m_param.enter_level > fb_cfg->chapter_cfg_list[chapter].max_level)
		{
			m_param.fb_list[fb_type].chapter_list[chapter].is_pass_chapter = 1;
			++m_param.enter_chapter;
			if (m_param.enter_chapter > fb_cfg->max_chapter)
			{
				m_param.enter_chapter = fb_cfg->max_chapter;
				m_param.enter_level = fb_cfg->chapter_cfg_list[chapter].max_level;
			}
			else
			{
				m_param.enter_level = 0;
			}
		}

		EventHandler::Instance().OnPassFbTuitu(m_role);
	}

	this->SendSingleLayerInfo(fb_type, chapter, level);
}

void RoleTuituFb::GetEnterFbParam(int &fb_type, int &chapter, int &level)
{
	fb_type = m_param.enter_fb_type;
	chapter = m_param.enter_chapter;
	level = m_param.enter_level;
}

bool RoleTuituFb::GetNextLayerParam(int &fb_type, int &chapter, int &level)
{
	if (m_param.enter_fb_type < 0 || m_param.enter_fb_type >= TUTUI_FB_TYPE_NUM_MAX) return false;

	const TuituFbInfoConfig *fb_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetFbinfoCfg(m_param.enter_fb_type);
	if (NULL == fb_cfg)
	{
		return false;
	}

	const int enter_chapter = m_param.enter_chapter;
	if (enter_chapter == fb_cfg->max_chapter && enter_chapter < TUITU_FB_CHAPER_NUM_MAX
		&& m_param.enter_level >= fb_cfg->chapter_cfg_list[enter_chapter].max_level)
	{
		m_role->NoticeNum(errornum::EN_TUITU_FB_ALL_COMPLETE);
		return false;
	}

	fb_type = m_param.enter_fb_type;
	chapter = m_param.enter_chapter;
	level = m_param.enter_level;

	return true;
}

bool RoleTuituFb::IsPassFb(int fb_type, int chapter, int level)
{
	if (fb_type < 0 || fb_type >= TUTUI_FB_TYPE_NUM_MAX
		|| chapter < 0 || chapter >= TUITU_FB_CHAPER_NUM_MAX
		|| level < 0 || level >= TUITU_FB_LEVLE_NUM_MAX)
	{
		return false;
	}
	
	if (TUITU_FB_TYPE_NORMAL == fb_type && m_param.fb_list[fb_type].chapter_list[chapter].level_list[level].pass_star > 0)
	{
		return true;
	}
	else if (TUITU_FB_TYPE_HARD == fb_type && m_param.fb_list[fb_type].chapter_list[chapter].level_list[level].pass_star >= TUITU_HARD_FB_PASS_STAR)
	{
		return true;
	}

	return false;
}

void RoleTuituFb::OnBuyTimes(int fb_type, int buy_times)
{
	if (fb_type < 0 || fb_type >= TUTUI_FB_TYPE_NUM_MAX)
	{
		return;
	}

	TuituFbParam::FbInfo &fb_info = m_param.fb_list[fb_type];

	int vip_buy_times = 0;
	if (TUITU_FB_TYPE_NORMAL == fb_type)
	{
		vip_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_TUITU_FB_NORMAL_BUY_TIMES);
	}
	else if (TUITU_FB_TYPE_HARD == fb_type)
	{
		vip_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_TUITU_FB_HARD_BUY_TIMES);
	}

	if (fb_info.buy_join_times >= vip_buy_times)
	{
		m_role->NoticeNum(errornum::EN_FB_NEQ_BUY_TIMES_UP);
		return;
	}

	if (buy_times > vip_buy_times - fb_info.buy_join_times)
	{
		m_role->NoticeNum(errornum::EN_TUITU_FB_BUY_TIMES_NOT_ENOUGH);
		return;
	}
	
	const TuituFbOtherConfig &other_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetOtherCfg();
	int total_need_gold = 0;
	if (TUITU_FB_TYPE_NORMAL == fb_type)
	{
		total_need_gold = buy_times * other_cfg.normal_buy_times_need_gold;
	}
	else
	{
		total_need_gold = buy_times * other_cfg.hard_buy_times_need_gold;
	}

	if (total_need_gold <= 0 || !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(total_need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(total_need_gold, "RoleTuituFb::OnBuyTimes"))
	{
		return;
	}

	m_role->GetRoleActivity()->AddTurntableScore(total_need_gold);
	
	fb_info.buy_join_times += buy_times;

	this->SendSingleLayerInfo(fb_type, m_param.enter_chapter, m_param.enter_level);
}

void RoleTuituFb::OnFetchStarReward(int chapter, int seq)
{
	Protocol::SCTuituFbFetchResultInfo  ttffri;

	ttffri.is_success = 1;
	ttffri.fb_type = TUITU_FB_TYPE_NORMAL;
	ttffri.chapter = static_cast<short>(chapter);
	ttffri.seq = static_cast<short>(seq);

	if (chapter < 0 || chapter >= TUITU_FB_CHAPER_NUM_MAX || seq < 0 || seq >= TUITU_FB_CHAPTER_STAR_REWARD_NUM_MAX)
	{
		ttffri.is_success = 0;
		return;
	}

	TuituFbParam::FbInfo &fb_info = m_param.fb_list[TUITU_FB_TYPE_NORMAL];
	if ((fb_info.chapter_list[chapter].star_reward_flag & (1 << seq)) != 0)
	{
		m_role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
		ttffri.is_success = 0;
	}

	const TuituFbStarRewardConfig::ItemCfg *reward_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetStarRewardCfg(TUITU_FB_TYPE_NORMAL, chapter, seq);
	if (NULL == reward_cfg)
	{
		ttffri.is_success = 0;
	}
	else
	{
		if (fb_info.chapter_list[chapter].total_star < reward_cfg->need_star)
		{
			m_role->NoticeNum(errornum::EN_TUITU_FB_CHAPTER_STAR_NUM_NOT_ENOUGH);
			ttffri.is_success = 0;
		}

		if (ttffri.is_success > 0)
		{
			fb_info.chapter_list[chapter].star_reward_flag |= 1 << seq;

			m_role->GetKnapsack()->PutListOrMail(reward_cfg->reward_list, PUT_REASON_TUITU_FB_STAR_REWARD);

			this->SendSingleLayerInfo(TUITU_FB_TYPE_NORMAL, chapter, m_param.enter_level);
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ttffri, sizeof(ttffri));
}

void RoleTuituFb::OnSaodangFb(int fb_type, int chapter, int level)
{	
	const TuituFbInfoConfig::Layer *layer_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetLayerCfg(fb_type, chapter, level);
	if (NULL == layer_cfg)
	{
		return;
	}

	if (fb_type < 0 || fb_type >= TUTUI_FB_TYPE_NUM_MAX || chapter < 0 || chapter >= TUITU_FB_CHAPER_NUM_MAX
		|| level < 0 || level >= TUITU_FB_LEVLE_NUM_MAX)
	{
		return;
	}

	TuituFbParam::LayerInfo &layer_info = m_param.fb_list[fb_type].chapter_list[chapter].level_list[level];
	if (layer_info.pass_star < layer_cfg->saodang_star_num)
	{
		m_role->NoticeNum(errornum::EN_TUITU_FB_CANT_SAODANG);
		return;
	}

	const TuituFbOtherConfig &other_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetOtherCfg();

	TuituFbParam::FbInfo &fb_info = m_param.fb_list[fb_type];
	if (TUITU_FB_TYPE_NORMAL == fb_type)
	{
		if (fb_info.today_join_times >= fb_info.buy_join_times + other_cfg.normal_free_join_times)
		{
			m_role->NoticeNum(errornum::EN_TUITU_FB_CANT_SAODANG_ENTER_TIMES_LIMIT);
			return;
		}
	}
	else
	{
		if (fb_info.today_join_times >= fb_info.buy_join_times + other_cfg.hard_free_join_times)
		{
			m_role->NoticeNum(errornum::EN_TUITU_FB_CANT_SAODANG_ENTER_TIMES_LIMIT);
			return;
		}
	}

	int index_count = 0;
	static ItemConfigData item_list[DropConfig::DROP_ITEM_PROB_MAX];

	for (int i = 0; i < layer_cfg->monster_count && i < TUITU_FB_GUANQIA_MONSTER_NUM_MAX; ++i)
	{
		const MonsterInitParam *monster_param = MonsterPool::Instance()->GetMonsterParam(layer_cfg->monster_list[i].monster_id);
		if (NULL == monster_param || monster_param->monster_type != MonsterInitParam::MONSTER_TYPE_BOSS)
		{
			continue;
		}
		
		MonsterPool::Instance()->GetMonsterDrop(layer_cfg->monster_list[i].monster_id, item_list, &index_count, NULL, NULL, NULL);
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(index_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	++fb_info.today_join_times;

	m_role->GetKnapsack()->PutList(index_count, item_list, PUT_REASON_TUITU_FB_SAODANG);

	this->SendSingleLayerInfo(fb_type, chapter, level);
}

void RoleTuituFb::SendFbInfo()
{
	Protocol::SCTuituFbInfo ttfi;

	UNSTD_STATIC_CHECK(sizeof(ttfi.fb_info_list) == sizeof(m_param.fb_list));
	memcpy(ttfi.fb_info_list, m_param.fb_list, sizeof(ttfi.fb_info_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ttfi, sizeof(ttfi));
}

void RoleTuituFb::SendSingleLayerInfo(int fb_type, int chapter, int level)
{
	if (fb_type < 0 || fb_type >= TUTUI_FB_TYPE_NUM_MAX || chapter < 0 || chapter >= TUITU_FB_CHAPER_NUM_MAX
		|| level < 0 || level >= TUITU_FB_LEVLE_NUM_MAX)
	{
		return;
	}

	TuituFbParam::LayerInfo& layer_info = m_param.fb_list[fb_type].chapter_list[chapter].level_list[level];

	Protocol::SCTuituFbSingleInfo  ttfsi;

	ttfsi.fb_type = static_cast<short>(fb_type);
	ttfsi.chapter = static_cast<char>(chapter);
	ttfsi.level = static_cast<char>(level);
	ttfsi.cur_chapter = m_param.enter_chapter;
	ttfsi.cur_level = m_param.enter_level;
	ttfsi.today_join_times = m_param.fb_list[fb_type].today_join_times;
	ttfsi.buy_join_times = m_param.fb_list[fb_type].buy_join_times;
	ttfsi.chapter_total_star = m_param.fb_list[fb_type].chapter_list[chapter].total_star;
	ttfsi.chapter_star_reward_flag = m_param.fb_list[fb_type].chapter_list[chapter].star_reward_flag;
	UNSTD_STATIC_CHECK(sizeof(ttfsi.layer_info) == sizeof(layer_info));
	memcpy(&ttfsi.layer_info, &layer_info, sizeof(ttfsi.layer_info));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ttfsi, sizeof(ttfsi));
}

