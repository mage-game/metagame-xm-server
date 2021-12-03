#include "other/camp/gongxun.h"
#include "global/rank/personrank.hpp"
#include "global/rank/rankmanager.hpp"
#include "obj/character/role.h"
#include "global/usercache/usercache.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/camp/campconfig.hpp"
#include "other/route/mailroute.hpp"
#include "servercommon/string/gameworldstr.h"
#include "other/rank/rankshadow.h"

GongXun::GongXun()
{
	
}

GongXun::~GongXun()
{
	
}

void GongXun::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	bool is_syncrank_now = m_role->GetGongXun() > 0;
	
	this->CheckGongXunGiveReward();
	m_role->ResetGongXun();

	if (is_syncrank_now)
	{
		// 需要马上同步，避免玩家在最后一秒登录，排行结算奖励时排行榜的里数据用的是reserdata之前的
		m_role->GetRankShadow()->UpdateToGlobal();	
	}
}

void GongXun::CheckGongXunGiveReward()
{
	const int gongxun = m_role->GetGongXun();
	
	const GongXunValueRewardCfg *cfg_item = LOGIC_CONFIG->GetCampCfg().GetGongXunValueRewardCfg(gongxun);
	if (NULL == cfg_item)
	{
		return;
	}

	UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(m_role->GetUserId());
	if (NULL == user_node)
	{
		return;
	}

	// 发邮件
	static MailContentParam contentparam; 
	contentparam.Reset();

	int reward_count = 0;
	for (int i = 0; i < cfg_item->item_count && i < HONOR_REWARD_ITEM_NUM; ++ i)
	{
		const ItemConfigData &item_data = cfg_item->reward_item_list[i];
		const ItemBase *item_base = ITEMPOOL->GetItem(item_data.item_id);

		if (NULL != item_base && reward_count < MAX_ATTACHMENT_ITEM_NUM)
		{
			contentparam.item_list[reward_count].item_id = item_data.item_id;
			contentparam.item_list[reward_count].num = item_data.num;
			contentparam.item_list[reward_count].is_bind = (item_data.is_bind) ? 1 : 0;
			contentparam.item_list[reward_count].invalid_time = item_base->CalInvalidTime();
			++ reward_count;
		}
	}

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_camp_gongxun_value_content, gongxun);
	if (length > 0) 
	{
		//MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
	}

	std::string reward_str;
	ItemConfigDataListLog(reward_str, cfg_item->item_count, cfg_item->reward_item_list);

	gamelog::g_log_other.printf(LL_INFO, "GongXun::CheckGongXunGiveReward user %d %s, gongxun %d, reward_item_count %d, reward_items %s",
		UidToInt(m_role->GetUserId()), m_role->GetName(), gongxun, reward_count, reward_str.c_str());
}