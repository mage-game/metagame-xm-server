#include "welfare.hpp"
#include "welfareconfig.hpp"
#include "other/welfare/sevendayloginconfig.hpp"
#include "other/fb/patafbconfig.hpp"

#include "obj/character/role.h"
#include "protocal/msgrole.h"
#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "item/itempool.h"
#include "item/itembase.h"
#include "other/event/eventhandler.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "task/taskpool.h"

#include "config/restconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"

#include "globalconfig/globalconfig.h"

#include "gameworld/gamelog.h"
#include "world.h"
#include "global/worldstatus/worldstatus.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "servercommon/welfaredef.hpp"
#include "other/shop/roleshop.hpp"
#include "config/activityconfig/questionconfig.hpp"
#include "config/taskconfig/husongconfig.hpp"
#include "other/fb/phasefbconfig.hpp"
#include "other/fb/storyfbconfig.hpp"
#include "item/knapsack.h"
#include "task/taskrecorder.h"
#include "other/vip/vip.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/sublock/sublock.hpp"
#include "task/other/guildtask.hpp"
#include "other/fb/rolestoryfb.hpp"
#include "other/fb/rolefbphase.hpp"
#include "other/fb/rolepatafb.hpp"
#include "other/fb/roledailyfb.hpp"
#include "monster/drop/dropconfig.hpp"
#include "monster/drop/droppool.hpp"
#include <algorithm>
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "effect/skilleffect/effectotherbuff.hpp"
#include "servercommon/activitydef.hpp"
#include "config/dropextendconfig.hpp"
#include "servercommon/serverconfig/openserverconfig.hpp"
#include "crossserver/config/sharedconfig.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/fb/rolefbequip.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "other/fb/rolefbchallenge.hpp"
#include "other/fb/rolefbarmordefend.hpp"
#include "other/fb/fbchallengeconfig.hpp"
#include "other/fb/armordefendconfig.hpp"
#include "other/fb/fbequipconfig.hpp"
#include "other/fb/towerdefendteamconfig.hpp"
#include "other/xiannv/xiannvmanager.hpp"

Welfare::Welfare() : m_role(NULL), m_accumulation_signin_days(0), m_online_reward_mark(0), m_offline_second(0), m_offline_exp(0), m_offline_mojing(0),
	m_green_item_count(0), m_green_item_resolve_count(0), m_blue_item_count(0), m_blue_item_resolve_count(0),
	m_purple_item_count(0), m_purple_item_resolve_count(0), m_orange_item_count(0), m_orange_item_resolve_count(0), 
	m_red_item_count(0), m_red_item_resolve_count(0), m_pink_item_count(0), m_pink_item_resolve_count(0),
	m_chongzhi_count(0), m_account_total_login_daycount(1),
	m_last_signin_time(0), m_sign_in_days(0), m_sign_in_reward_mark(0), m_is_chongzhi_today(0), m_continuity_signin_days(0),
	m_activity_find_flag(0), m_activity_join_flag(0), m_activity_auto_flag(0), m_offline_pass_days(0), m_offline_days(0), m_change_flag(false), m_happy_tree_level(0), m_happy_tree_reward(0), m_chongjihaoli_reward_mark(0),
	m_total_happy_tree_growth_val(0), m_happy_tree_online_time(0), m_today_chongzhi_gold_num(0), m_seven_day_login_fetch_reward_mark(0), m_sign_in_today_times(0),
	m_role_level_after_fetch(0),
	m_collect_left_time_s(0), m_collect_offline_time_s(0), m_collect_item_count(0), m_is_not_first_send(0)
{
	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));
	memset(m_old_state, 0, sizeof(m_old_state));
	memset(m_happy_tree_grouth_val, 0, sizeof(m_happy_tree_grouth_val));
}

Welfare::~Welfare()
{

}

void Welfare::Init(Role *role, const WelfareParam &p, const DailyFindListParam &daily_find_param)
{
	m_role = role;

	m_offline_second = p.offline_second;
	m_offline_exp = p.offline_exp;
	m_offline_mojing = p.offline_mojing;

	m_chongzhi_count = p.chongzhi_count;
	m_account_total_login_daycount = p.account_total_login_daycount;

	m_sign_in_days = p.sign_in_days;
	m_sign_in_today_times = p.sign_in_today_times;
	m_is_not_first_send = p.is_not_first_send;

	m_sign_in_reward_mark = p.sign_in_reward_mark;
	m_activity_find_flag = p.activity_find_flag;
	m_activity_join_flag = p.activity_join_flag;
	m_activity_auto_flag = p.auto_activity_flag;
	m_is_chongzhi_today = p.is_chongzhi_today;
	m_continuity_signin_days = p.continuity_signin_days;
	m_last_signin_time = p.last_signin_time;
	m_happy_tree_reward = p.happy_tree_reward;
	m_chongjihaoli_reward_mark = p.chongjihaoli_reward_mark;
	m_online_reward_mark = p.online_reward_mark;
	m_happy_tree_level = p.happy_tree_level;
	m_total_happy_tree_growth_val = p.total_happy_tree_growth_val;
	m_happy_tree_online_time = p.happy_tree_online_times;
	m_today_chongzhi_gold_num = p.today_chongzhi_gold_num;
	m_seven_day_login_fetch_reward_mark = p.seven_day_login_fetch_reward_mark;

	if (m_happy_tree_level <= 0)
	{
		m_happy_tree_level = 1;
	}

	memcpy(m_happy_tree_grouth_val, p.happy_tree_grouth_val, sizeof(m_happy_tree_grouth_val));

	this->CalcAccumulationSignInDays();

	int count = daily_find_param.count;
	if (count > DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS) count = DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS;

	for (int i = count - 1; i >= 0; i--)
	{
		int index = (int)daily_find_param.daily_find_list[i].index;
		if (index < 0 || index >= DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS) 
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[DailyFindParam::Init][user[%d %s], daily find index is invalid:%d]", 
				UidToInt(m_role->GetUserId()), m_role->GetName(), index);
			continue;
		}

		if (!m_daily_find_list[index].Invalid()) 
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[DailyFindParam::Init][user[%d %s], daily find duplicate index:%d]", 
				UidToInt(m_role->GetUserId()), m_role->GetName(), index);
			continue;
		}

		m_daily_find_list[index].find_time = daily_find_param.daily_find_list[i].find_time;
		m_daily_find_list[index].role_level = daily_find_param.daily_find_list[i].role_level;
		m_daily_find_list[index].param = daily_find_param.daily_find_list[i].param;
		m_daily_find_list[index].param1 = daily_find_param.daily_find_list[i].param1;
		m_daily_find_list[index].find_count = daily_find_param.daily_find_list[i].find_count;

		m_old_state[index] = true;
	}

	m_green_item_count = 0;
	m_blue_item_count = 0;
	m_purple_item_count = 0;
	m_orange_item_count = 0;
	m_red_item_count = 0;
	m_pink_item_count = 0;

	m_role_login_level = p.login_level;
	m_collect_left_time_s = p.collect_left_time_s;

	m_exp_buff_info = p.exp_buff_info;
}

void Welfare::GetInitParam(WelfareParam *p)
{
	p->offline_second = m_offline_second;
	p->offline_exp = m_offline_exp;
	p->offline_mojing = m_offline_mojing;

	p->chongzhi_count = m_chongzhi_count;

	p->account_total_login_daycount = m_account_total_login_daycount;
	p->sign_in_days = m_sign_in_days;
	p->sign_in_today_times = m_sign_in_today_times;
	p->is_not_first_send = m_is_not_first_send;
	p->sign_in_reward_mark = m_sign_in_reward_mark;
	p->activity_find_flag = m_activity_find_flag;
	p->activity_join_flag = m_activity_join_flag;
	p->auto_activity_flag = m_activity_auto_flag;
	p->is_chongzhi_today = m_is_chongzhi_today;
	p->continuity_signin_days = m_continuity_signin_days;
	p->last_signin_time = m_last_signin_time;
	p->happy_tree_reward = m_happy_tree_reward;
	p->online_reward_mark = m_online_reward_mark;
	p->happy_tree_level = m_happy_tree_level;
	p->chongjihaoli_reward_mark = m_chongjihaoli_reward_mark;
	p->total_happy_tree_growth_val = m_total_happy_tree_growth_val;
	p->happy_tree_online_times = m_happy_tree_online_time;
	p->today_chongzhi_gold_num = m_today_chongzhi_gold_num;
	p->seven_day_login_fetch_reward_mark = m_seven_day_login_fetch_reward_mark;
	
	memcpy(p->happy_tree_grouth_val, m_happy_tree_grouth_val, sizeof(p->happy_tree_grouth_val));

	p->login_level = m_role_login_level;
	p->collect_left_time_s = m_collect_left_time_s;

	p->exp_buff_info = m_exp_buff_info;
}

void Welfare::Update(unsigned int interval, unsigned int now_second)
{
	static const int ADD_HAPPY_TREE_GROWTH_VALUE_TIME = 3600;
	if (m_role->GetTodayRealTimeOnlineS() - (int)m_happy_tree_online_time >= ADD_HAPPY_TREE_GROWTH_VALUE_TIME)
	{
		m_happy_tree_online_time += ADD_HAPPY_TREE_GROWTH_VALUE_TIME;
		this->AddHappytreeGrowthvalue(GET_HAPPYTREE_VALUE_TYPE_ONLINE);
	}

	this->CheckMergeExpBuff(now_second);
}

void Welfare::OnRoleLogin(long long last_save_time)
{
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_second < last_save_time) return;

	m_role_login_level = m_role->GetLevel();
	//////////////////////////////////////////////////////////////////////////
	// 这里开始计算本次的经验

	if (now_second - last_save_time > SECOND_PER_MIN) // 离线超过一分钟，才叠加离线经验
	{
		// 离线经验
		const WelfareOtherConfig &welfare_other_cfg = LOGIC_CONFIG->GetWelfareConfig().GetOtherCfg();
		if (m_role->GetLevel() >= welfare_other_cfg.offline_exp_level_limit)
		{	
			if (0 == m_offline_exp /*&& 0 == m_offline_mojing*/) m_offline_second = 0;
			int offline_second = m_offline_second + (int)(now_second - last_save_time);

			if (0 != m_role->GetOnlineTime())													// 只计算旧角色离线经验
			{
				
				if (offline_second > OFFLINE_EXP_GET_MAX_TIME) offline_second = OFFLINE_EXP_GET_MAX_TIME;	

				if (offline_second > m_offline_second)
				{
					// 策划需求，现在不使用L-离线经验.xls产出离线经验
					//int offline_exp = 0; //int offline_mojing = 0;  // 屏蔽奖励魔晶
					//LOGIC_CONFIG->GetRestConfig().GetRestReward(m_role->GetLevel(), &offline_exp, NULL, NULL, NULL /*&offline_mojing*/);
					//m_offline_exp += offline_exp * (offline_second - m_offline_second);

					//策划需求，福利经验读取D-等级奖励配置，并根据对应经验系数，计算出离线经验
					long long offline_exp_1 = static_cast<long long>(welfare_other_cfg.offline_exp_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)));
					m_offline_exp += offline_exp_1 * static_cast<long long>((offline_second - m_offline_second) / SECOND_PER_MIN);

					// 计算经验buff的加成
					m_offline_exp += this->CalcExpBuffAddOfflineExp(last_save_time, offline_exp_1);

					//// 计算可双倍追加的部分经验
					//m_add_double_offline_exp = 0;
					//int left_time = offline_second - m_exp_double_effect_time - m_exp_triple_effect_time;
					//if (left_time > 0)
					//{
					//	m_add_double_offline_exp = static_cast<long long>((left_time / SECOND_PER_MIN) * offline_exp_1);
					//	if (m_add_double_offline_exp < 0) m_add_double_offline_exp = 0;
					//}

					// 屏蔽离线挂机奖励魔晶 2019-01-21
					//m_offline_mojing += offline_mojing * (offline_second - m_offline_second);
					m_offline_second = offline_second;
				}
			}
			else
			{
				m_offline_second = 0;
			}
		}
	}
	else
	{
		int buff_type = m_exp_buff_info.GetCurMaxExpBuffType();
		if (buff_type > EXP_BUFF_TYPE_INVALID && buff_type < EXP_BUFF_TYPE_MAX)
		{
			m_exp_buff_info.exp_buff_list[buff_type].Reset();
			this->SendExpBuffInfo();
		}
	}


	//离线物品奖励计算
	CalcOfflineItemReward(m_offline_second);

	//////////////////////////////////////////////////////////////////////////
	// 这里直接下发离线获取的经验和装备
	this->GetOfflineExp(0);  // 只按照一倍领取

	int tmp_offline_days = GetDayIndex(last_save_time, now_second) - 1;

	{
		// 日常找回
		m_offline_days = tmp_offline_days;
		if (m_offline_days < 0) m_offline_days = 0;
		
		if (m_offline_days > 0)
		{
			if (m_offline_days > WELFARE_DAILY_FIND_MAXDAYS) m_offline_days = WELFARE_DAILY_FIND_MAXDAYS;
			for (int find_type = 0; find_type < DAILYFIND_TYPE_MAX; ++ find_type)
			{
				const DailyFindConfigItem *find_item = LOGIC_CONFIG->GetWelfareConfig().GetDailyFindItem(find_type);
				if (NULL == find_item)
				{
					continue;
				}

				for (int days = 1; days <= m_offline_days; ++days)
				{
					unsigned int find_time = now_second - days * WELFARE_SECONDS_ONE_DAY;
					this->OnDailyFindChange(find_type, find_time, find_item->count_perday);
				}
			}
		}
	}

	{
		// 检查活动能否找回
		m_offline_pass_days = tmp_offline_days;
		if (m_offline_pass_days < 0) m_offline_pass_days = 0;

		if (m_offline_pass_days > 0)
		{
			for (int i = 1; i <= m_offline_pass_days && i < ACTIVITY_OPEN_RECORD_MAX_COUNT; ++ i)
			{
				this->CheckActvityCanFind(i);
			}
		}
	}

	this->SendExpBuffInfo();
}

void Welfare::OnRoleLogout()
{
	// 记录经验药水
	this->SetSaveExpBuff();

	// 记录集字活动剩余时间
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ITEM_COLLECTION))
	{
		m_collect_left_time_s = (int)(ActivityShadow::Instance().GetRandActivityEndTime(RAND_ACTIVITY_TYPE_ITEM_COLLECTION) - (unsigned int)EngineAdapter::Instance().Time());
		if (m_collect_left_time_s < 0)
		{
			m_collect_left_time_s = 0;
		}
	}
	else
	{
		m_collect_left_time_s = 0;
	}
}

void Welfare::CalcOfflineItemReward(int second)
{
	static_assert(SECOND_PER_MIN > 0, "Welfare::GiveOfflineItemReward");
	if (second < SECOND_PER_MIN)
		return;

	auto rest_cfg = LOGIC_CONFIG->GetRestConfig().GetRestConfig(m_role->GetLevel());
	if (!rest_cfg)
	{
		return;
	}

	CommonDataParam *common_data = m_role->GetCommonDataParam();
	if (NULL == common_data)
	{
		return;
	}

	auto this_offline_minute = second / SECOND_PER_MIN;
	DropItemCfg drop_item[DropConfig::DROP_ITEM_PROB_MAX];
	item_list.clear();
	collect_item_list.clear();

	//从怪物身上获取drop_ids;	
	auto monster_info = MonsterPool::Instance()->GetMonsterParam(rest_cfg->monster_id);
	if (!monster_info)
	{
		return;
	}
	// 产生掉落
	for (auto drop_id : monster_info->dropid_list)
	{
		auto drop_config = DROPPOOL->GetDropConfig(drop_id);
		if (!drop_config)
		{
			continue;
		}

		auto drop_item_count = drop_config->RandDropItemN(this_offline_minute * rest_cfg->minute_drop_count, drop_item);
		if (drop_item_count < 1)
		{
			continue;
		}
		for (auto i = 0; i < drop_item_count; i++)
		{//聚合到map
		 // 随机获取的数量，随机范围(num - num/5, num + num/5)
			int tmp_item_num = drop_item[i].item_config_data.num / 5;
			int item_num = RandomNum(-tmp_item_num, tmp_item_num);
			item_num += drop_item[i].item_config_data.num;
			if (item_num <= 0) item_num = 1;

			item_list[drop_item[i].item_config_data.item_id] += item_num;
		}
	}

	// 计算集字活动掉落有效时间，(简单做，不考虑离线前后是不是同一个活动了，太麻烦)
	{
		m_collect_offline_time_s = 0;
		m_collect_item_count = 0;

		if (0 != m_role->GetOnlineTime())
		{
			if (m_collect_left_time_s > 0)
			{
				// 1. 如果之前下线时正在集字活动中，那么只算上一次剩余活动时间的掉落
				m_collect_offline_time_s = (second > m_collect_left_time_s) ? m_collect_left_time_s : second;

				m_collect_left_time_s = 0;
			}
			else
			{
				// 2. 否则，如果上线的时候集字活动正在开启，那么只算本次的时间的掉落
				if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ITEM_COLLECTION))
				{
					int open_time = (int)((unsigned int)EngineAdapter::Instance().Time() - ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_ITEM_COLLECTION));
					if (open_time > 0)
					{
						m_collect_offline_time_s = (second > open_time) ? open_time : second;
					}
				}
			}
		}
	}

	// 产生集字掉落
	{
		int DROP_INTERVAL_S =  LOGIC_CONFIG->GetDropExtendCfg().GetOtherCfg()->extradrop_time_gap;  // 30秒
		if (m_collect_offline_time_s > 0 && DROP_INTERVAL_S > 0)
		{
			UInt16 drop_id = LOGIC_CONFIG->GetDropExtendCfg().GetRandomCollectExtraDropId(monster_info->level);

			auto drop_config = DROPPOOL->GetDropConfig(drop_id);
			if (drop_config)
			{
				int total_drop_num = 0;			// 总掉落
				int total_drop_num_after_reduce = 0;	// 衰减后的总掉落
				auto drop_item_count = drop_config->RandDropItemN(m_collect_offline_time_s / DROP_INTERVAL_S, drop_item);
				for (auto i = 0; i < drop_item_count; i++)
				{
					int item_num = drop_item[i].item_config_data.num;

					total_drop_num += item_num;
					collect_item_list[drop_item[i].item_config_data.item_id] += item_num;
				}

				auto collect_cut_cfg = LOGIC_CONFIG->GetDropExtendCfg().GetRandomCollectCutCfg(total_drop_num + common_data->collect_drop_num);
				if (collect_cut_cfg)
				{
					// 有配置说明已经到了衰减限制
					// 若掉落字帖数大于177，则最终掉落=177+（理论值-177）*50%
					if (common_data->collect_drop_num >= collect_cut_cfg->drop_num)
					{
						total_drop_num_after_reduce = (int)(total_drop_num * collect_cut_cfg->drop_per * 0.0001f);  // 本次全部衰减
					}
					else
					{
						int left_num = collect_cut_cfg->drop_num - common_data->collect_drop_num;  // 没有超出的
						int temp_num = (int)((total_drop_num - left_num) * collect_cut_cfg->drop_per * 0.0001f);  // 超出的部分做衰减
						total_drop_num_after_reduce = left_num + temp_num;
					}
				}

				if (total_drop_num > 0 && total_drop_num_after_reduce > 0)
				{
					for (auto &item : collect_item_list)
					{
						if (item.second > 0)
						{
							int item_num = static_cast<int>(((double)item.second / (double)total_drop_num) * (double)total_drop_num_after_reduce);
							if (item_num < 0) item_num = 0;

							item.second = item_num;
							common_data->collect_drop_num += item_num;
							m_collect_item_count += item_num;				// 统计数量
						}
					}
				}
			}

			m_collect_offline_time_s = 0;  // 统计完掉落就清除收集时间
		}
	}


	m_green_item_count = 0;
	m_blue_item_count = 0;
	m_purple_item_count = 0;
	m_orange_item_count = 0;
	m_red_item_count = 0;
	m_pink_item_count = 0;
	//计算客户端显示数据
	for (auto& item : item_list)
	{
		auto item_info = ITEMPOOL->GetItem(item.first);
		if (!item_info)
		{
			continue;
		}
		switch (item_info->GetColor())
		{
		case ItemBase::I_COLOR_GREEN:
			m_green_item_count += item.second;
			break;
		case ItemBase::I_COLOR_BLUE:
			m_blue_item_count += item.second;
			break;
		case ItemBase::I_COLOR_PURPLE:
			m_purple_item_count += item.second;
			break;
		case ItemBase::I_COLOR_ORANGE:
			m_orange_item_count += item.second;
			break;
		case ItemBase::I_COLOR_RED:
			m_red_item_count += item.second;
			break;
		case ItemBase::I_COLOR_PINK:
			m_pink_item_count += item.second;
			break;
		default:
			break;
		}
	}
}

void Welfare::GiveOfflineItemReward()
{
	std::vector<ItemConfigData> sort_item_list;
	for (auto& item : item_list)
	{
		sort_item_list.push_back(ItemConfigData(item.first, true, item.second));
	}
	for (auto& item : collect_item_list)
	{
		sort_item_list.push_back(ItemConfigData(item.first, true, item.second));
	}
	//根据物品类型，非装备优先，然后再按品质排序,高品质优先放背包
	std::sort(sort_item_list.begin(), sort_item_list.end(), [](const ItemConfigData& a, const ItemConfigData& b)->bool {
		auto item_base_a = ITEMPOOL->GetItem(a.item_id);
		auto item_base_b = ITEMPOOL->GetItem(b.item_id);
		if (item_base_a && item_base_b)
		{
			if (item_base_a->GetItemType() != item_base_b->GetItemType())
			{
				return ItemBase::I_TYPE_EQUIPMENT != item_base_a->GetItemType();
			}
			else
			{
				return item_base_a->GetColor() > item_base_b->GetColor();
			}
		}
		return false;
	});

	m_green_item_resolve_count = 0;
	m_blue_item_resolve_count = 0;
	m_purple_item_resolve_count = 0;
	m_orange_item_resolve_count = 0;
	m_red_item_resolve_count = 0;
	m_pink_item_resolve_count = 0;

	int add_mojing_count = 0;
	for (auto& item : sort_item_list)
	{
		auto item_base = ITEMPOOL->GetItem(item.item_id);
		if (!item_base || item.num <= 0)
		{
			continue;
		}

		int empty_grid_count = m_role->GetKnapsack()->GetCurrKnapscakEmptyGridNum();
		ItemConfigData data = item;
		data.num = 0;
		if (empty_grid_count > 0)
		{
			if (item_base->GetPileLimit() > 1)  // 可堆叠物品
			{
				int need_grid_count = (item.num / item_base->GetPileLimit()) + 1;
				if (empty_grid_count >= need_grid_count)  // 空格子够数
				{
					data.num = item.num;
				}
				else
				{
					data.num = empty_grid_count * item_base->GetPileLimit();
				}
			}
			else
			{
				data.num = (std::min)(empty_grid_count, item.num);
			}
			if (data.num > 0)
			{
				m_role->GetKnapsack()->Put(data, PUT_REASON_NO_NOTICE);
			}
		}

		if (item.num > data.num)
		{//放不进背包的分解
			//auto item_base = ITEMPOOL->GetItem(item.item_id);
			if (!item_base || ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType())
			{
				continue;
			}

			auto last_count = item.num - data.num;
			auto recyclget = item_base->GetRecyclGet(nullptr);
			add_mojing_count += recyclget * (last_count);
			switch (item_base->GetColor())
			{
			case ItemBase::I_COLOR_GREEN:
				m_green_item_resolve_count += last_count;
				break;
			case ItemBase::I_COLOR_BLUE:
				m_blue_item_resolve_count += last_count;
				break;
			case ItemBase::I_COLOR_PURPLE:
				m_purple_item_resolve_count += last_count;
				break;
			case ItemBase::I_COLOR_ORANGE:
				m_orange_item_resolve_count += last_count;
				break;
			case ItemBase::I_COLOR_RED:
				m_red_item_resolve_count += last_count;
				break;
			case ItemBase::I_COLOR_PINK:
				m_pink_item_resolve_count += last_count;
				break;
			default:
				break;
			}
		}
	}
	
	if (add_mojing_count > 0)
	{
		m_role->GetRoleShop()->AddChestShopMojing(add_mojing_count, "Welfare::GiveOfflineItemReward");
	}
}

long long Welfare::CalcExpBuffAddOfflineExp(long long last_save_time, long long base_offline_exp)
{
	// 判断离线前有无buff
	int buff_type = m_exp_buff_info.GetCurMaxExpBuffType();
	if (buff_type == EXP_BUFF_TYPE_INVALID)
	{
		return 0;
	}

	m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFEECT_TYPE_OTHER, EBT_INVALID, INVALID_OBJ_ID,
		EffectBase::PRODUCT_METHOD_ITEM, EffectBase::PRODUCT_ID_INC_EXTRAL_EXP_PER);

	long long total_offline_exp_buff_exp = 0;
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	int offline_exp_buff_time = now_second - static_cast<unsigned int>(last_save_time);
	m_offline_buff_time = 0;

	for (; buff_type >= EXP_BUFF_TYPE_1; buff_type --)
	{
		ExpBuffInfo::ExpBuffItem &buff_info = m_exp_buff_info.exp_buff_list[buff_type];
		if (buff_info.exp_buff_left_time_s <= 0)
		{
			continue;
		}

		int left_offline_exp_buff_time = offline_exp_buff_time - buff_info.exp_buff_left_time_s;
		int real_exp_buff_offline_time_s = left_offline_exp_buff_time > 0 ? buff_info.exp_buff_left_time_s : offline_exp_buff_time;

		if (real_exp_buff_offline_time_s > OFFLINE_EXP_GET_MAX_TIME)
		{
			real_exp_buff_offline_time_s = OFFLINE_EXP_GET_MAX_TIME;
		}
		long long tmp_offline_exp_buff_exp = static_cast<long long>((real_exp_buff_offline_time_s / SECOND_PER_MIN) * base_offline_exp * ((double)buff_info.exp_buff_rate / 100.0f));

		if (tmp_offline_exp_buff_exp < 0)
		{
			tmp_offline_exp_buff_exp = 0;
		}
		total_offline_exp_buff_exp += tmp_offline_exp_buff_exp;

		if (left_offline_exp_buff_time >= 0)
		{
			buff_info.Reset();
		}

		offline_exp_buff_time -= real_exp_buff_offline_time_s;
		m_offline_buff_time += real_exp_buff_offline_time_s;

		// 如果还有buff时间,加上
		if (left_offline_exp_buff_time < 0)
		{
			EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_INC_EXTRAL_EXP_PER, -left_offline_exp_buff_time * 1000, buff_info.exp_buff_level, EffectBase::PRODUCT_METHOD_ITEM);
			effect->IncExtraExpPer(buff_info.exp_buff_rate);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);
			if (1 == buff_info.exp_buff_level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP1);
			if (2 == buff_info.exp_buff_level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP2);
			if (3 == buff_info.exp_buff_level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP3);
			if (4 == buff_info.exp_buff_level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP4);

			m_role->AddEffect(effect);

			buff_info.exp_buff_left_time_s = -left_offline_exp_buff_time;
			buff_info.Reset();
			break;
		}
	}

	return total_offline_exp_buff_exp;
}

void Welfare::CheckMergeExpBuff(unsigned int now_s)
{
	if (m_nex_check_buff_time <= 0)
	{
		m_nex_check_buff_time = now_s + 5;
	}

	if (now_s < m_nex_check_buff_time)
	{
		return;
	}

	m_nex_check_buff_time = now_s + 5;

	int buff_type = m_exp_buff_info.GetCurMaxExpBuffType();
	if (buff_type == EXP_BUFF_TYPE_INVALID)
	{
		return;
	}

	EffectOtherBuff *buff = m_role->GetEffectList()->FindOtherEffectBuff(EffectBase::PRODUCT_ID_INC_EXTRAL_EXP_PER);
	if (NULL != buff && EffectBase::PRODUCT_METHOD_ITEM == buff->GetProductMethod() && buff->GetLeftTimeMS() > 0)
	{
		return;
	}

	// 当前buff类型消失重置数据, 尝试取上一个buff类型

//	ExpBuffInfo::ExpBuffItem buff_info;
// 	for (int pre_buff_type = buff_type; pre_buff_type > EXP_BUFF_TYPE_INVALID; pre_buff_type --)
// 	{
// 		if (m_exp_buff_info.exp_buff_list[pre_buff_type].exp_buff_left_time_s > 0)
// 		{
// 			buff_info = m_exp_buff_info.exp_buff_list[pre_buff_type];
// 			break;
// 		}
// 	}

	ExpBuffInfo::ExpBuffItem  &buff_info = m_exp_buff_info.exp_buff_list[buff_type];
	if (buff_info.exp_buff_left_time_s <= 0)
	{
		return;
	}

	EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_INC_EXTRAL_EXP_PER, buff_info.exp_buff_left_time_s * 1000, buff_info.exp_buff_level, EffectBase::PRODUCT_METHOD_ITEM);
	effect->IncExtraExpPer(buff_info.exp_buff_rate);
	effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);

	if (1 == buff_info.exp_buff_level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP1);
	if (2 == buff_info.exp_buff_level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP2);
	if (3 == buff_info.exp_buff_level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP3);
	if (4 == buff_info.exp_buff_level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP4);

	m_role->AddEffect(effect);

	buff_info.Reset();
	this->SendExpBuffInfo();
}

void Welfare::FetchOnlineReward(int part)
{
	if (part < 0 || part >= OnlineRewardConfig::MAX_ONLINE_REWARD_COUNT)
	{
		return;
	}
	const OnlineRewardConfig::RewardConfig * online_reward = LOGIC_CONFIG->GetWelfareConfig().GetOnlineRewardCfg(part);
	if (NULL == online_reward)
	{
		return;
	}

	if (m_role->GetTodayRealTimeOnlineS() < online_reward->minutes * 60)
	{
		return;
	}

	if (0 != (m_online_reward_mark & (1 << part)))
	{
		return;
	}

	m_online_reward_mark |= (1 << part);

	m_role->GetKnapsack()->PutListOrMail(online_reward->reward_item, PUT_REASON_ONLINE_REWARD);

	this->SendWelfareInfo();
}

void Welfare::OnAddChongZhi(long long chongzhi_gold)
{
	++m_chongzhi_count;
	m_is_chongzhi_today = 1;
	m_today_chongzhi_gold_num += chongzhi_gold;

	int add_happytree_growth_val_count = (int)(m_today_chongzhi_gold_num / 10);

	this->AddHappytreeGrowthvalue(GET_HAPPYTREE_VALUE_TYPE_CHONGZHI, add_happytree_growth_val_count);

	this->SendWelfareInfo(Protocol::SCWelfareInfo::ACCOUNT_LOGIN_INFO);
}

void Welfare::GetDailyFindList(DailyFindListParam *daily_find_param)
{
	if (NULL == daily_find_param) return;

	int count = 0;

	for (int i = 0; i < DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS; ++i)
	{
		if (!m_daily_find_list[i].Invalid())
		{
			daily_find_param->daily_find_list[count].change_state = structcommon::CS_NONE;
			daily_find_param->daily_find_list[count].index = i;

			daily_find_param->daily_find_list[count].find_time = m_daily_find_list[i].find_time;
			daily_find_param->daily_find_list[count].role_level = m_daily_find_list[i].role_level;
			daily_find_param->daily_find_list[count].param = m_daily_find_list[i].param;
			daily_find_param->daily_find_list[count].param1 = m_daily_find_list[i].param1;
			daily_find_param->daily_find_list[count].find_count = m_daily_find_list[i].find_count;

			++count;
		}
	}

	daily_find_param->count = count;
}

void Welfare::GetChangeDailyFindList(DailyFindListParam *daily_find_param)
{
	if (NULL == daily_find_param) return;

	if (!m_change_flag)
	{
		daily_find_param->count = 0;
		return;
	}

	int count = 0;
	for (int i = 0; i < DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS; ++i)
	{
		if (m_dirty_mark[i])
		{
			if (m_daily_find_list[i].Invalid())
			{
				if (!m_old_state[i]) continue;

				daily_find_param->daily_find_list[count].change_state = structcommon::CS_DELETE;
				daily_find_param->daily_find_list[count].index = i;

				daily_find_param->daily_find_list[count].find_time = 0;
				daily_find_param->daily_find_list[count].role_level = 0;
				daily_find_param->daily_find_list[count].find_count = 0;
				daily_find_param->daily_find_list[count].param = 0;
				daily_find_param->daily_find_list[count].param1 = 0;
			}
			else
			{
				daily_find_param->daily_find_list[count].change_state = m_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT;
				daily_find_param->daily_find_list[count].index = i;

				daily_find_param->daily_find_list[count].find_time = m_daily_find_list[i].find_time;
				daily_find_param->daily_find_list[count].role_level = m_daily_find_list[i].role_level;
				daily_find_param->daily_find_list[count].find_count = m_daily_find_list[i].find_count;
				daily_find_param->daily_find_list[count].param = m_daily_find_list[i].param;
				daily_find_param->daily_find_list[count].param1 = m_daily_find_list[i].param1;
			}

			++count;
		}
	}

	daily_find_param->count = count;
}

void Welfare::ClearDirtyMark()
{
	if (!m_change_flag) return;

	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));

	for (int i = 0; i < DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS; ++i)
	{
		m_old_state[i] = !m_daily_find_list[i].Invalid();
	}

	m_change_flag = false;
}

void Welfare::FetchSignInRewardHandle(int day, int is_quick)
{
	if(0 == is_quick)
	{
		this->FetchSignInReward(day, is_quick);
	}
	else
	{	
		bool sign_success = false;
		for (int i = 1; i < 32; i++)
		{
			if (FetchSignInReward(i, is_quick))
			{
				sign_success = true;
			}
		}

		this->CalcAccumulationSignInDays();
		this->SendWelfareInfo();
		if (sign_success)
		{
			m_role->NoticeNum(noticenum::NT_WELFARE_QUICK_SIGN_SUCCESS);
		}
		else
		{
			m_role->NoticeNum(errornum::EN_WELF_FILL_SIGNIN_LIMIT);
		}
	}
}

// 领取就是每日签到
bool Welfare::FetchSignInReward(int day, int is_quick)
{
	const tm *t = EngineAdapter::Instance().LocalTime();
	if (NULL == t) return false;

	int curr_day = t->tm_mday;
	int curr_month = t->tm_mon;

	if (day > curr_day) return false;

	if (-1 == day)
	{
		day = curr_day;
	}
	
	// 一键补签，不能签当天的
	if (1 == is_quick && day == curr_day)
	{
		return false;
	}

	if (0 != (m_sign_in_days & (1 << day)))
	{
		return false;
	}

	const WelfareSignInConfig::DayConfig * day_cfg = LOGIC_CONFIG->GetWelfareConfig().GetSignInCfg(curr_month, day);
	if (NULL == day_cfg)
	{
		return false;
	}
	
	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
		return false;
	}

	if (day == curr_day)
	{
		// 屏蔽充值后可以再次签到
		//if (m_sign_in_today_times > 0 && 1 != m_is_chongzhi_today)
		if (m_sign_in_today_times > 0)
		{
			m_role->NoticeNum(errornum::EN_WELF_SIGNIN_DAYLIMIT);
			return false;
		}
	}
	else
	{
		if (day_cfg->diamond > 0)
		{
			if (!m_role->GetKnapsack()->GetMoney()->UseGold(day_cfg->diamond, "FetchSignInReward"))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return false;
			}
		}
	}

	ItemConfigData reward_item;
	reward_item.item_id = day_cfg->reward_item.item_id;
	reward_item.num = day_cfg->reward_item.num;
	reward_item.is_bind = day_cfg->reward_item.is_bind;
	if (m_role->GetVip()->GetVipLevel() >= day_cfg->vip && day_cfg->multiple > 1)
	{
		reward_item.num *= day_cfg->multiple;
	}

	if (m_role->GetKnapsack()->Put(reward_item, PUT_REASON_FETCH_SIGN_IN_REWARD))
	{

// 		// 如果没连续签，将连续签到标志清除
// 		time_t now_time = EngineAdapter::Instance().Time();
// 		int interval = EngineAdapter::Instance().NextDayInterval(0, 0, 0);
// 		if (now_time + interval - m_last_signin_time >= 48 * 60 * 60)
// 		{
// 			m_continuity_signin_days = 0;
// 		}
// 
// 		m_last_signin_time = static_cast<unsigned int>(now_time);
// 
// 		if (0 == m_sign_in_today_times)	//今天签到次数
// 		{
// 			m_continuity_signin_days++;	//连续签到次数
// 		}
		
		m_sign_in_days |= 1 << day;

		if (0 == is_quick)
		{
			if (day == curr_day)
			{
				++m_sign_in_today_times;
				m_role->NoticeNum(noticenum::NT_WELFARE_SIGN_SUCCESS);
			}
			else
			{
				m_role->NoticeNum(noticenum::NT_WELFARE_BEFORE_SIGN_SUCCESS);
			}

			this->CalcAccumulationSignInDays();
			this->SendWelfareInfo();
		}

		gamelog::g_log_welfare.printf(LL_INFO, "SignIn::FetchSignInReward : user[%d, %s], fetch_day[month:%d, day:%d], get_item[id:%d, num:%d]", 
			m_role->GetUID(), m_role->GetName(), curr_month, day, (int)day_cfg->reward_item.item_id, day_cfg->reward_item.num);
	}

	return true;
}

void Welfare::FetchAccumulationSignInReward(int seq)
{
	if (seq < 0 || seq >= WelfareAccumulationSignConfig::ACCUMULATION_SIGNIN_COUNT)
	{
		return;
	}

	const tm *t = EngineAdapter::Instance().LocalTime();
	if (NULL == t) return;

	int curr_month = t->tm_mon;

	const WelfareAccumulationSignConfig::AccumulationConfig *acc_cfg = LOGIC_CONFIG->GetWelfareConfig().GetAccumulationSignInCfg(curr_month, seq);
	if (NULL == acc_cfg)
	{
		return;
	}

	if (m_accumulation_signin_days < acc_cfg->accumulation_days)
	{
		return;
	}

	if ((m_sign_in_reward_mark & (1 << seq)) != 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
		return;
	}

	m_sign_in_reward_mark |= 1 << seq;

	m_role->GetKnapsack()->Put(acc_cfg->accumulation_reward, PUT_REASON_FETCH_SIGN_IN_REWARD);

	this->SendWelfareInfo();

	gamelog::g_log_welfare.printf(LL_INFO, "SignIn::FetchAccumulationSignInReward : user[%d, %s], get_item[id:%d, num:%d]", 
		m_role->GetUID(), m_role->GetName(), (int)acc_cfg->accumulation_reward.item_id, acc_cfg->accumulation_reward.num);
}

void Welfare::CalcAccumulationSignInDays()
{
	m_accumulation_signin_days = 0;
	for(int i = 1; i <= WelfareSignInConfig::DAY_PER_MONTH; ++i)
	{
		if(m_sign_in_days & (1 << i))
		{
			++m_accumulation_signin_days;
		}
	}
}

void Welfare::SignInFindBack()
{
// 	if (m_sign_in_days >= WelfareSignInConfig::DAY_PER_MONTH)
// 	{
// 		return;
// 	}
// 
// 	const struct tm *local_time = EngineAdapter::Instance().LocalTime();
// 	if (NULL == local_time)
// 	{
// 		return;
// 	}
// 
// 	if (m_sign_in_days >= local_time->tm_mday)
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
// 	if (!m_role->GetKnapsack()->HaveSpace())
// 	{
// 		m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
// 		return;
// 	}
// 
// 	int find_back_day = m_sign_in_days + 1;
// 	int month = GetMonthIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
// 	const WelfareSignInConfig::DayConfig * day_cfg = LOGIC_CONFIG->GetWelfareConfig().GetSignInCfg(month, find_back_day);
// 	if (NULL == day_cfg)
// 	{ 
// 		return;
// 	}
// 
// 	int need_gold = LOGIC_CONFIG->GetWelfareConfig().GetOtherCfg().find_sign_in_cost;
// 	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "SignInFindBack"))
// 	{
// 		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 		return;
// 	}
// 
// 	m_role->GetKnapsack()->Put(day_cfg->reward_item, PUT_REASON_SIGN_IN_FIND_BACK);
// 
// 	m_sign_in_reward_mark |= (1 << find_back_day);
// 	++ m_sign_in_days;
// 
// 	this->SendWelfareInfo();
// 
// 	gamelog::g_log_welfare.printf(LL_INFO, "SignIn::SignInFindBack : user[%d, %s], find_back_day[month:%d, day:%d],  comsume_all_gold[%d], get_item[id:%d, num:%d]", 
// 		m_role->GetUID(), m_role->GetName(), month, find_back_day, need_gold, (int)day_cfg->reward_item.item_id, day_cfg->reward_item.num);
}

void Welfare::ActivityFind(int find_type, bool is_free_find)
{
	if (find_type < 0 || find_type >= ACTIVITY_FIND_TYPE_MAX)
	{
		return;
	}

	if (0 == (m_activity_find_flag & (1 << find_type)))
	{
		m_role->NoticeNum(errornum::EN_ACTIVITY_CANNOT_FIND);
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

	const WelfareActivityFindConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetWelfareConfig().GetActivityFindConfig(find_type);
	if (NULL == cfg_item || cfg_item->is_open <= ACTIVITY_FIND_OPEN_INVALID)
	{
		return;
	}

	if (is_free_find && cfg_item->is_open < ACTIVITY_FIND_OPEN_FREE)
	{
		return;
	}

	const WelfareActivityFindRewardConfig::ConfigItem *cfg_reward = LOGIC_CONFIG->GetWelfareConfig().GetActivityFindReward(find_type, m_role->GetLevel());
	if (NULL == cfg_reward)
	{
		return;
	}

	double reward_percent = 1.0;
	if (is_free_find)
	{
		reward_percent = cfg_item->free_find_percent / 100.0;
	}

	long long exp = static_cast<long long>(cfg_reward->find_exp_factor * LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role_login_level, 0)* reward_percent);
	int honor = static_cast<int>(cfg_reward->honor * reward_percent);
	int bind_coin = static_cast<int>(cfg_reward->bind_coin * reward_percent);
	int guild_gongxian = static_cast<int>(cfg_reward->guild_gongxian * reward_percent);
	int cross_honor = static_cast<int>(cfg_reward->cross_honor * reward_percent);
	int mo_jing = static_cast<int>(cfg_reward->mo_jing * reward_percent);

	int reward_item_count = 0;
	ItemConfigData reward_item_list[WelfareActivityFindRewardConfig::REWARD_ITEM_MAX_COUNT];
	{
		for (int i = 0; i < cfg_reward->item_count && reward_item_count < WelfareActivityFindRewardConfig::REWARD_ITEM_MAX_COUNT; ++ i)
		{
			int reward_num = static_cast<int>(cfg_reward->item_list[i].num * reward_percent);
			if (reward_num > 0)
			{
				reward_item_list[reward_item_count].item_id = cfg_reward->item_list[i].item_id;
				reward_item_list[reward_item_count].is_bind = cfg_reward->item_list[i].is_bind;
				reward_item_list[reward_item_count].num = reward_num;
				++ reward_item_count;
			}
		}
	}
	
	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(reward_item_count))
	{
		m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
		return;
	}

	if (exp <= 0 && honor <= 0 && bind_coin <= 0 && guild_gongxian <= 0 && cross_honor <= 0 && reward_item_count <= 0)
	{
		return;
	}

	if (!is_free_find && !m_role->GetKnapsack()->GetMoney()->UseGold(cfg_reward->cost, "WelfareFindActivity"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_activity_find_flag &= ~(1 << find_type);

	m_role->GetKnapsack()->PutList(reward_item_count, reward_item_list, PUT_REASON_ACTIVITY_FIND);
	m_role->AddExp(exp, "WelfareFindActivity", Role::EXP_ADD_REASON_DEFAULT);
	m_role->GetKnapsack()->GetMoney()->AddCoinBind(bind_coin, "WelfareFindActivity");
	m_role->GetRoleShop()->AddChestShopShengwang(honor, "WelfareFindActivity");
	m_role->AddGuildGongxian(guild_gongxian, "WelfareFindActivity");
	m_role->AddCrossHonor(cross_honor, "WelfareFindActivity");
	m_role->GetRoleShop()->AddChestShopMojing(mo_jing, "WelfareFindActivity");

	this->SendWelfareInfo();

	{
		// 日志
		std::string get_item_log;
		ItemConfigDataListLog(get_item_log, reward_item_count, reward_item_list);

		gamelog::g_log_welfare.printf(LL_INFO, "ActivityFind user[%d, %s], activity_find_type[%d], use_all_gold[%d], "
			"add_exp[%lld], add_coin[%d], add_gongxian[%d], add_honor[%d], add_cross_honor[%d],add_mojing[%d], item_list[%s]",
			m_role->GetUID(), m_role->GetName(), find_type, (is_free_find ? 0 : cfg_reward->cost), exp, 
			bind_coin, guild_gongxian, honor, cross_honor, mo_jing,get_item_log.c_str());
	}
}

void Welfare::OnJoinActivity(unsigned int activity_find_type)
{
	unsigned int old_join_flag = m_activity_join_flag;

	m_activity_join_flag |= 1 << activity_find_type;

	if (old_join_flag != m_activity_join_flag)
	{
		this->SendWelfareInfo();
	}
}

int Welfare::HasAutoActivity(int activity_type, int hold_times)
{
	int activity_find_type = -1;
	if (!LOGIC_CONFIG->GetWelfareConfig().GetFindType(activity_type, &activity_find_type, hold_times))
	{
		return false;
	}

	return m_activity_auto_flag & (1 << activity_find_type);
}

void Welfare::OnAutoActivityReq(int activity_find_type)
{
// 	if (activity_find_type < 0 || activity_find_type >= ACTIVITY_FIND_TYPE_MAX) 
// 	{
// 		return;
// 	}
// 
// 	if (!this->IsCanAutoActivityType(activity_find_type))
// 	{
// 		return;
// 	}
// 
// 	ActivityOpenRecords &open_records = WorldStatus::Instance().GetActivityOpenRecords();
// 	if (!open_records.IsActivityOpen(0, activity_find_type))
// 	{
// 		m_role->NoticeNum(errornum::EN_ACTIVITY_ACTIVITY_NOT_START);
// 		return;
// 	}
// 
// 	if (0 != (m_activity_auto_flag & (1 << activity_find_type)))
// 	{
// 		m_role->NoticeNum(errornum::EN_ACTIVITY_FIND_REWARED);
// 		return;
// 	}
// 
// 	if (0 != (m_activity_join_flag & (1 << activity_find_type)))
// 	{
// 		m_role->NoticeNum(errornum::EN_ACTIVITY_HAS_ENTER);
// 		return;
// 	}
// 
// 	const WelfareActivityFindConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetWelfareConfig().GetActivityFindConfig(activity_find_type);
// 	if (NULL == cfg_item)
// 	{
// 		return;
// 	}
// 
// 	if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinActivity(cfg_item->activity_type, m_role))
// 	{
// 		m_role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
// 		return;
// 	}
// 
// 	const WelfareActivityFindRewardConfig::ConfigItem *cfg_reward = LOGIC_CONFIG->GetWelfareConfig().GetActivityFindReward(activity_find_type, m_role->GetLevel());
// 	if (NULL == cfg_reward)
// 	{
// 		return;
// 	}
// 
// 	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg_reward->item_count))
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 		return;
// 	}
// 
// 	if (!m_role->GetKnapsack()->GetMoney()->UseGold(cfg_reward->cost, "WelfareAutoActivity"))
// 	{
// 		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 		return;
// 	}
// 
// 	m_activity_auto_flag |= (1 << activity_find_type);
// 
// 	m_role->GetKnapsack()->PutList(cfg_reward->item_count, cfg_reward->item_list, PUT_REASON_WELFARE);
// 	m_role->AddExp(cfg_reward->exp, "WelfareFindActivity");
// 	m_role->AddYuanli(cfg_reward->yuanli, "WelfareFindActivity");
// 	m_role->GetRoleActivity()->AddBattleFieldHonor(cfg_reward->honor, "WelfareFindActivity", true);
// 	m_role->GetKnapsack()->GetMoney()->AddCoinBind(cfg_reward->bind_coin, "WelfareFindActivity");
// 	m_role->AddNvWaShi(cfg_reward->nv_wa_shi, "WelfareFindActivity");
// 	m_role->AddGuildGongxian(cfg_reward->guild_gongxian, "WelfareFindActivity");
// 
// 	this->SendWelfareInfo();
// 	m_role->NoticeNum(noticenum::NT_AUTO_ACTIIVTY_SUCC);

	//this->CheckRewardActiveDegree(activity_find_type);
}

void Welfare::SendWelfareInfo(int notify_reason, int durlogin_lottory_item_index)
{
	static Protocol::SCWelfareInfo dwi;

	dwi.notify_reson = notify_reason;

	dwi.offline_second = m_offline_second;
	dwi.offline_exp = m_offline_exp;
	dwi.offline_mojing = m_offline_mojing;

	dwi.exp_buff_effect_second = m_offline_buff_time;
	
	dwi.add_double_exp = 0;// m_add_double_offline_exp;
	dwi.exp_buff_effect_rate = 0;
	dwi.role_level_after_fetch = m_role_level_after_fetch;// this->CalcOfflineExpBuffLeftCost();

	dwi.green_item_count			= m_green_item_count;
	dwi.green_item_resolve_count	= m_green_item_resolve_count;
	dwi.blue_item_count				= m_blue_item_count;
	dwi.blue_item_resolve_count		= m_blue_item_resolve_count;
	dwi.purple_item_count			= m_purple_item_count;
	dwi.purple_item_resolve_count	= m_purple_item_resolve_count;
	dwi.orange_item_count			= m_orange_item_count;
	dwi.orange_item_resolve_count	= m_orange_item_resolve_count;
	dwi.red_item_count				= m_red_item_count;
	dwi.red_item_resolve_count		= m_red_item_resolve_count;
	dwi.pink_item_count				= m_pink_item_count;
	dwi.pink_item_resolve_count		= m_pink_item_resolve_count;
	dwi.collect_item_count			= m_collect_item_count;

	dwi.sign_in_days = m_sign_in_days;
	dwi.sign_in_reward_mark = m_sign_in_reward_mark;
	dwi.activity_find_flag = m_activity_find_flag;
	dwi.auto_activity_flag = m_activity_auto_flag;
	dwi.activity_join_flag = m_activity_join_flag;
	dwi.today_online_time = m_role->GetTodayRealTimeOnlineS();
	dwi.online_reward_mark = m_online_reward_mark;
	dwi.is_chongzhi_today = m_is_chongzhi_today;
	dwi.continuity_signin_days = m_continuity_signin_days;
	dwi.happy_tree_level = m_happy_tree_level;
	dwi.happy_tree_reward = m_happy_tree_reward;
	dwi.max_growth_value_get_type = MAX_GROWTH_VALUE_GET_TYPE + 1;
	dwi.total_happy_tree_growth_val = m_total_happy_tree_growth_val;
	dwi.chongjihaoli_reward_mark = m_chongjihaoli_reward_mark;
	dwi.sign_in_today_times = m_sign_in_today_times;
	dwi.accumulation_signin_days = m_accumulation_signin_days;

	memcpy(dwi.happy_tree_grouth_val, m_happy_tree_grouth_val, sizeof(dwi.happy_tree_grouth_val));
	memcpy(dwi.total_chongjihaoli_record_list, WorldStatus::Instance().GetCommonData().total_chongjihaoli_record_list, sizeof(dwi.total_chongjihaoli_record_list));

	dwi.role_login_level = m_role_login_level;
	for (int i = 0; i < ACTIVITY_FIND_TYPE_MAX && i < Protocol::SCWelfareInfo::ACTIVITY_FIND_TYPE_EXP_MAX; ++i)
	{
		const long long base_exp = LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role_login_level, 0);
		dwi.exp_activityfind[i] = static_cast<long long>(base_exp * LOGIC_CONFIG->GetWelfareConfig().GetActivityFindFactor(i,m_role_login_level));
	}

	int count = 0;
	for (int i = 0; i < DAILYFIND_TYPE_MAX; i++)
	{
		for (int j = 0; j < WELFARE_DAILY_FIND_MAXDAYS; j++)
		{
			int cur_pos = i * WELFARE_DAILY_FIND_MAXDAYS + j;
			if (cur_pos < 0 || cur_pos >= DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS) break;

			if (!m_daily_find_list[cur_pos].Invalid())
			{
				if (count < 0 || count >= DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS) break;

				dwi.daily_find_list[count].Reset();
				this->CalcReward(cur_pos, &dwi.daily_find_list[count]);

				++ count;
			}
		}
	}

	dwi.daily_find_count = count;

	if (m_is_not_first_send == 0)
	{
		if (notify_reason == Protocol::SCWelfareInfo::OFFLINE_EXP_NOTICE)
		{
			dwi.is_first_send = 1;
			m_is_not_first_send = 1;
		}
		else
		{
			dwi.is_first_send = 0;
		}
	}
	else
	{
		dwi.is_first_send = 0;
	}

	dwi.reserve_sh = 0;

	dwi.red_item_count = 0;

	int sendlen = sizeof(dwi) - (DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS - count) * sizeof(DailyFindRewardItem);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&dwi, sendlen);
}

void Welfare::SendWelfareItemChange(char dailyfind_type)
{
	static Protocol::SCDailyFindItemChange dfic;
	dfic.dailyfind_type = dailyfind_type;
	dfic.result = 1;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&dfic, sizeof(dfic));
}

//领取离线经验
void Welfare::GetOfflineExp(int type)
{
	if (type < 0 || m_offline_second <= 0 || (m_offline_exp <= 0 /*&& m_offline_mojing <= 0*/)) return;			// 1：是否有离线时间或离线经验

	//const OfflineExpFetchConfig::ExpConfig *oefc = LOGIC_CONFIG->GetWelfareConfig().GetOfflineExpConfig(type);	// 2：取离线经验获取方式配置
	//if (NULL == oefc) return;

	//////////////////////////////////////////////////////////////////////////
	// 修改为经验BUFF加倍经验值。可使用元宝补足BUFF不足的经验

	const WelfareOtherConfig &welfare_other_cfg = LOGIC_CONFIG->GetWelfareConfig().GetOtherCfg();
	if (m_role->GetLevel() < welfare_other_cfg.offline_exp_level_limit) return;

	//int hour = m_offline_second / 3600 + 1;
	//if (oefc->diamond > 0)
	//{
	//	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(oefc->diamond * hour, "GetOfflineExp")) return;
	//}

//	long long add_exp = 0;
	int cost_gold = 0;

	if (0 != type)
	{
		if (m_offline_second <= m_exp_buff_info.exp_buff_list[EXP_BUFF_TYPE_2].exp_buff_left_time_s)
		{
			// 没有双倍领取的数值则不能双倍
			return;
		}

		if (m_offline_second <= GetCurExpBuffLeftTime())  // 不需要双倍领取
		{
			return;
		}

// 		if (m_offline_second <= m_exp_double_effect_time + m_exp_triple_effect_time)  // 不需要双倍领取
// 		{
// 			return;
// 		}

		// 需要消耗
		cost_gold = this->CalcOfflineExpBuffLeftCost();
		if (cost_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseAllGold(cost_gold, "GetOfflineExp"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

//		add_exp = m_add_double_offline_exp;
	}

	m_role->AddExp(m_offline_exp, "OfflineExp", Role::EXP_ADD_REASON_DEFAULT);
	// 屏蔽离线挂机奖励魔晶 2019-01-21
	//m_role->GetRoleShop()->AddChestShopMojing(m_offline_mojing * (oefc->factor), "OfflineExp");

	// 物品奖励
	this->GiveOfflineItemReward();

	// 下发客户端，用于客户端展示本次的离线获取的经验和装备信息
	m_role_level_after_fetch = m_role->GetLevel();
	this->SendWelfareInfo(Protocol::SCWelfareInfo::OFFLINE_EXP_NOTICE);

	m_offline_second = 0;
	m_offline_exp = 0;
	m_offline_mojing = 0;

//	m_exp_double_effect_time = 0;
//	m_exp_double_effect_rate = 0;
//	m_exp_triple_effect_time = 0;
//	m_add_double_offline_exp = 0;

	m_role->NoticeNum(noticenum::NT_GET_OFFLINE_EXP_SUCCESS);
	this->SendWelfareInfo(Protocol::SCWelfareInfo::OFFLINE_EXP_INFO);

	m_role_level_after_fetch = 0;

	gamelog::g_log_welfare.printf(LL_INFO, "GetOfflineExp : user[%d, %s], get_offline_timestamp[%d], get_offline_exp[%lld], get_offline_mojing[%d]", 
		m_role->GetUID(), m_role->GetName(),m_offline_second, m_offline_exp, m_offline_mojing);	

	gamelog::g_log_welfare.printf(LL_INFO, "GetOfflineItem: user[%d, %s], blue_item[%d], blue_item_resolve[%d], purple_item[%d], purple_item_resolve[%d], orange_item[%d], orange_item_resolve[%d], red_item[%d], red_item_resolve[%d]", 
		m_role->GetUID(), m_role->GetName(), m_blue_item_count, m_blue_item_resolve_count,
		m_purple_item_count, m_purple_item_resolve_count,
		m_orange_item_count, m_orange_item_resolve_count,
		m_red_item_count, m_red_item_resolve_count);
}

void Welfare::GetDailyFind(int dailyfind_type, int get_type)					
{
	if (dailyfind_type < 0 || dailyfind_type >= DAILYFIND_TYPE_MAX) return;

	if (get_type < DAILYFIND_GET_FREE || get_type >= DAILYFIND_GET_MAX) return;

	long long all_exp = 0, all_bind_coin = 0, all_need_gold = 0, all_need_coin = 0;
	int all_honor = 0, all_banggong = 0;
	
	const static int DAILY_FIND_NEED_GRID_MAX_COUNT = 64;
	int need_grid = 0;
	int xiannv_shengwu = 0;
	ItemConfigData daily_find_reward_item_list[DAILY_FIND_NEED_GRID_MAX_COUNT];

	for (int i = 0; i < WELFARE_DAILY_FIND_MAXDAYS; i++)
	{
		int pos = dailyfind_type * WELFARE_DAILY_FIND_MAXDAYS + i;
		if (pos < 0 || pos >= DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS) continue;

		if (m_daily_find_list[pos].Invalid()) continue;

		const DailyFindConfigItem *config_item = LOGIC_CONFIG->GetWelfareConfig().GetDailyFindItem(dailyfind_type);
		if (NULL == config_item) continue;

		DailyFindRewardItem reward_item;
		this->CalcReward(pos, &reward_item, &xiannv_shengwu);

		all_exp += static_cast<long long>(reward_item.exp * config_item->exp_percent_list[get_type] * 0.01);
		all_bind_coin += static_cast<long long>(reward_item.bind_coin * config_item->bind_coin_percent_list[get_type] * 0.01);
		all_honor += static_cast<int>(reward_item.honor * config_item->honor_percent_list[get_type] * 0.01);
		all_banggong += static_cast<int>(reward_item.banggong *config_item->gongxian_percent_list[get_type] * 0.01);

		for (int i = 0; i < reward_item.item_count; ++ i)
		{
			ItemConfigData &reward_stuff = reward_item.item_list[i];
			int real_item_num = reward_stuff.num * config_item->item_percent_list[get_type] / 100;
			if (real_item_num > 0)
			{
				ItemExtern::PutInItemConfigList(DAILY_FIND_NEED_GRID_MAX_COUNT, daily_find_reward_item_list, &need_grid, 
					reward_stuff.item_id, real_item_num, reward_stuff.is_bind);
			}
		}

		if (DAILYFIND_GET_GOLD == get_type) 
		{
			all_need_gold += reward_item.gold_need;
		}
		else
		{
			all_need_coin += reward_item.coin_need;
		}
	}

	if (all_exp <= 0 && all_bind_coin <= 0 && all_honor <= 0 && all_banggong <= 0 && all_need_gold <= 0 && all_need_coin <= 0 && need_grid <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_grid))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (all_need_gold > 0)
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(all_need_gold, "DailyFind"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}
	else if (all_need_coin > 0)
	{
 		if (!m_role->GetKnapsack()->GetMoney()->UseCoin(all_need_coin, "DailyFind"))
 		{
 			m_role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
 			return;
 		}
	}

	int find_times = 0;
	for (int i = 0; i < WELFARE_DAILY_FIND_MAXDAYS; i++)
	{
		int pos = dailyfind_type * WELFARE_DAILY_FIND_MAXDAYS + i;
		if (pos < 0 || pos >= DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS) continue;

		find_times += m_daily_find_list[pos].find_count;

		m_daily_find_list[pos].Reset();

		m_dirty_mark[pos] = true;
		m_change_flag = true;
	}

	if (all_exp > 0) m_role->AddExp(all_exp, "DailyFind", Role::EXP_ADD_REASON_DEFAULT);
	if (all_bind_coin > 0) m_role->GetKnapsack()->GetMoney()->AddCoinBind(all_bind_coin, "DailyFind");
	if (all_honor > 0) m_role->GetRoleShop()->AddChestShopShengwang(all_honor ,"Welfare");
	if (all_banggong > 0) m_role->AddGuildGongxian(all_banggong,"Welfare");
	if (xiannv_shengwu > 0) m_role->GetXiannvManager()->AddShengwuEssence(xiannv_shengwu, "Welfare::DailyFind");

	if (need_grid > 0)
	{
		m_role->GetKnapsack()->PutList(need_grid, daily_find_reward_item_list, PUT_REASON_GET_DAILY_FIND);
	}

	m_role->NoticeNum(noticenum::NT_WELFARE_GET_DAILYFIND_SUCCESS);

	this->SendWelfareItemChange(dailyfind_type);

	{
		// 日志
		std::string get_item_log;
		ItemConfigDataListLog(get_item_log, need_grid, daily_find_reward_item_list);

		gamelog::g_log_welfare.printf(LL_INFO, "GetDailyFind user[%d, %s], daily_find_type[%d], find_times[%d], "
			"get_type[%d], use_all_gold[%lld], use_coin[%lld], add_exp[%lld], add_coin[%lld], item_list[%s]",
			m_role->GetUID(), m_role->GetName(), dailyfind_type, find_times, get_type, all_need_gold, all_need_coin, 
			all_exp, all_bind_coin, get_item_log.c_str());
	}
}

int Welfare::GetDailyFindDayCount(int dailyfind_type)
{
	int day_count = 0;
	switch (dailyfind_type)
	{
	case  DAILYFIND_TYPE_EQUIP_FB:
	{
		day_count = m_role->GetFBEquip()->GetTodayFightAllTimes();
	}
	break;
	case  DAILYFIND_TYPE_TOWER_DEFEND_FB:
	{
		day_count = m_role->GetFBTowerDefend()->GetPersonalFbEnterTimes();
	}
	break;
	case  DAILYFIND_TYPE_CHALLENGE_FB:
	{
		day_count = m_role->GetFBChallenge()->GetEnterCount();
	}
	break;
	case  DAILYFIND_TYPE_ARMOR_FB:
	{
		day_count = m_role->GetRoleFbArmorDefend()->GetPersonalJoinTimes();
	}
	break;

	default:
		break;
	}
	return day_count;
}

void Welfare::OnDailyFindChange(int dailyfind_type, unsigned int find_time, int find_count)
{
	if (dailyfind_type < 0 || dailyfind_type >= DAILYFIND_TYPE_MAX || find_count <= 0) return;

	int real_find_days = GLOBALCONFIG->GetWelfareDailyFindDays();
	
	int min_pos = 0;
	if (this->FindMinPos(dailyfind_type, &min_pos, real_find_days) && min_pos >= 0 && min_pos < DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS)
	{
		if (m_daily_find_list[min_pos].find_time < find_time)
		{
			DailyFindItem daily_find_item; daily_find_item.Reset();

			daily_find_item.find_time = find_time;
			daily_find_item.role_level = m_role->GetLevel();
			daily_find_item.find_count = find_count;

			switch (dailyfind_type)
			{
			case DAILYFIND_TYPE_FB_EXP:
				{
					const DailyFbItem *fb_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetDailyFbItemCfg(DAILYFB_TYPE_EXP);
					if (NULL == fb_cfg || m_role->GetLevel() < fb_cfg->role_level)
					{
						return;
					}

					if (m_role->GetRoleDailyFb()->GetExpFbHistoryEnterTimes() <= 0 || m_role->GetRoleDailyFb()->GetExpFbLastExp() <= 0)
					{
						return;
					}
				}
				break;

			case DAILYFIND_TYPE_PATA:
				{
					if (m_role->GetRolePataFB()->GetPassLevel() <= 0)
					{
						return;
					}
				}
				break;

			case DAILYFIND_TYPE_STORY_FB:
				{
					const StoryFBLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetStoryFBConfig().GetLevelCfg(0);
					if (NULL == level_cfg || m_role->GetLevel() < level_cfg->role_level)
					{
						return;
					}

					if (!m_role->GetFBStory()->IsPassLevel(0))
					{
						return;
					}
				}
				break;

			case DAILYFIND_TYPE_UPGRADE_MOUNT_FB:
				{
					bool can_find = false;
					for(int i = 0; i < FB_PHASE_MAX_COUNT; i ++)
					{
						int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
						const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
						if (NULL == level_cfg) continue;

						if (PHASE_FB_TYPE_MOUNT != level_cfg->fb_type) continue;

						if (m_role->GetLevel() < level_cfg->role_level) break;
						if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;
						
						can_find = true;
						break;
					}

					if (!can_find)
					{
						return;
					}
				}
				break;

			case DAILYFIND_TYPE_UPGRADE_SHIZHUANG_BODY_FB:
				{
					bool can_find = false;
					for(int i = 0; i < FB_PHASE_MAX_COUNT; i ++)
					{
						int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
						const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
						if (NULL == level_cfg) continue;

						if (PHASE_FB_TYPE_SHIZHUANG_BODY != level_cfg->fb_type) continue;

						if (m_role->GetLevel() < level_cfg->role_level) break;
						if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;

						can_find = true;
						break;
					}

					if (!can_find)
					{
						return;
					}
				}
				break;

			case DAILYFIND_TYPE_UPGRADE_SHIZHUANG_WUQI_FB:
				{
					bool can_find = false;
					for(int i = 0; i < FB_PHASE_MAX_COUNT; i ++)
					{
						int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
						const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
						if (NULL == level_cfg) continue;

						if (PHASE_FB_TYPE_SHIZHUANG_WUQI != level_cfg->fb_type) continue;

						if (m_role->GetLevel() < level_cfg->role_level) break;
						if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;

						can_find = true;
						break;
					}

					if (!can_find)
					{
						return;
					}
				}
				break;

			case DAILYFIND_TYPE_UPGRADE_LINGTONG_FB:
				{
					bool can_find = false;
					for(int i = 0; i < FB_PHASE_MAX_COUNT; i ++)
					{
						int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
						const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
						if (NULL == level_cfg) continue;

						if (PHASE_FB_TYPE_LINGTONG != level_cfg->fb_type) continue;

						if (m_role->GetLevel() < level_cfg->role_level) break;
						if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;

						can_find = true;
						break;
					}

					if (!can_find)
					{
						return;
					}
				}
				break;

			case DAILYFIND_TYPE_UPGRADE_FIGHT_MOUNT_FB:
				{
					bool can_find = false;
					for(int i = 0; i < FB_PHASE_MAX_COUNT; i ++)
					{
						int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
						const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
						if (NULL == level_cfg) continue;

						if (PHASE_FB_TYPE_FIGHT_MOUNT != level_cfg->fb_type) continue;

						if (m_role->GetLevel() < level_cfg->role_level) break;
						if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;

						can_find = true;
						break;
					}

					if (!can_find)
					{
						return;
					}
				}
				break;

			case DAILYFIND_TYPE_UPGRADE_LINGGONG_FB:
				{
					bool can_find = false;
					for (int i = 0; i < FB_PHASE_MAX_COUNT; i++)
					{
						int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
						const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
						if (NULL == level_cfg) continue;

						if (PHASE_FB_TYPE_LINGGONG != level_cfg->fb_type) continue;

						if (m_role->GetLevel() < level_cfg->role_level) break;
						if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;

						can_find = true;
						break;
					}

					if (!can_find)
					{
						return;
					}
				}
				break;

			case DAILYFIND_TYPE_UPGRADE_LINGQI_FB:
				{
					bool can_find = false;
					for (int i = 0; i < FB_PHASE_MAX_COUNT; i++)
					{
						int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
						const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
						if (NULL == level_cfg) continue;

						if (PHASE_FB_TYPE_LINGQI != level_cfg->fb_type) continue;

						if (m_role->GetLevel() < level_cfg->role_level) break;
						if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;

						can_find = true;
						break;
					}

					if (!can_find)
					{
						return;
					}
				}
				break;

			case DAILYFIND_TYPE_TUMO:
				{
					JoinLimitUnit limit_unit;
					bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_DAILY_TASK, &limit_unit);

					if (is_limit && m_role->GetLevel() < limit_unit.level)
					{
						return;
					}

					if (!m_role->GetTaskRecorder()->IsFinished(TASKPOOL->GetDailyPreTask())) 
					{
						return;
					}
				}
				break;

			case DAILYFIND_TYPE_GUILD_TASK:
				{
					if (INVALID_GUILD_ID == m_role->GetGuildID())
					{
						return;
					}

					if (m_role->GetGuildTask()->GetCompleteTaskCount() > 0)
					{
						return;
					}
				}
				break;

			case DAILYFIND_TYPE_QUESTION:
				{
					return;
				}
				break;

			case DAILYFIND_TYPE_HUSONG:
				{
					JoinLimitUnit limit_unit;
					bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_HUSONG_TASK, &limit_unit);

					if (is_limit && m_role->GetLevel() < limit_unit.level)
					{
						return;
					}

					if (!m_role->GetTaskRecorder()->IsFinished(LOGIC_CONFIG->GetHuSongConfig().GetPreTaskID()))
					{
						return;
					}
				}
				break;

			case DAILYFIND_TYPE_PAOHUAN:
				{
					if (m_role->GetLevel() < LOGIC_CONFIG->GetWelfareConfig().GetOtherCfg().paohuan_level_limit)
					{
						return;
					}
				}
				break;

// 			case DAILYFIND_TYPE_EQUIP_FB:
// 			{
// 				if (!m_role->GetFBEquip()->CanDailyFind())
// 				{
// 					return;
// 				}
// 			}
// 			break;
// 
// 			case DAILYFIND_TYPE_TOWER_DEFEND_FB:
// 			{
// 				if (m_role->GetFBTowerDefend()->GetPersonalFBPassLevel()< 0)
// 				{
// 					return;
// 				}
// 			}
// 			break;
// 
// 			case DAILYFIND_TYPE_CHALLENGE_FB:
// 			{
// 				if (m_role->GetFBChallenge()->GetPassLevel() < 0)
// 				{
// 					return;
// 				}
// 			}
// 			break;
// 			case DAILYFIND_TYPE_ARMOR_FB:
// 			{
// 				if (m_role->GetRoleFbArmorDefend()->GetArmoedefendPassLayer() < 0)
// 				{
// 					return;
// 				}
// 			}
// 			break;
// 			case DAILYFIND_TYPE_TEAM_EQUIP_FB:
// 			{
// 				if (m_role->GetFBEquip()->GetTeamFBPassMaxLevel() <= 0)
// 				{
// 					return;
// 				}
// 			}
// 			break;
// 			case DAILYFIND_TYPE_TEAM_TOWER_DEFEND_FB:
// 			{
// 				if (m_role->GetFBTowerDefend()->GetTeamPassdeWave() <= 0)
// 				{
// 					return;
// 				}
// 			}
// 			break;		
			
			default:
				return;
			}

			m_daily_find_list[min_pos] = daily_find_item;
			m_dirty_mark[min_pos] = true;
			m_change_flag = true;
		}
	}
}

// 找到real_find_days天数之内最早的找回位置，如果要替代总是替代最早的找回
bool Welfare::FindMinPos(int dailyfind_type, int *p_min_pos, int real_find_days)
{
	if (dailyfind_type < 0 || dailyfind_type >= DAILYFIND_TYPE_MAX || NULL == p_min_pos) return false;

	int cur_index = dailyfind_type * WELFARE_DAILY_FIND_MAXDAYS;

	int min_pos = 0;
	for (int i = 1; i < WELFARE_DAILY_FIND_MAXDAYS && i < real_find_days; i++)
	{
		if (m_daily_find_list[cur_index + min_pos].find_time > m_daily_find_list[cur_index + i].find_time)
		{
			min_pos = i;
		}
	}

	*p_min_pos = cur_index + min_pos;

	return true;
}

void Welfare::CalcReward(int pos, DailyFindRewardItem *reward_item,int *xiannv_shengwu)
{
	if (pos < 0 || pos >= DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS || NULL == reward_item) 
	{
		return;
	}

	UNSTD_STATIC_CHECK(WELFARE_DAILY_FIND_MAXDAYS > 0);

	int dailyfind_type = pos / WELFARE_DAILY_FIND_MAXDAYS;
	if (dailyfind_type < 0 || dailyfind_type >= DAILYFIND_TYPE_MAX) 
	{
		return;
	}

	DailyFindItem &daily_find_item = m_daily_find_list[pos];
	if (daily_find_item.Invalid()) 
	{
		return;
	}

	const DailyFindConfigItem *config_item = LOGIC_CONFIG->GetWelfareConfig().GetDailyFindItem(dailyfind_type);
	if (NULL == config_item) 
	{
		return;
	}

	reward_item->Reset();
	reward_item->find_type = static_cast<short>(dailyfind_type);
	reward_item->role_level = daily_find_item.role_level;
	reward_item->find_times = daily_find_item.find_count;
	reward_item->reserve_sh = 0;

	short role_level = m_daily_find_list[pos].role_level;

	switch (dailyfind_type)
	{
	case DAILYFIND_TYPE_FB_EXP:
		{
			reward_item->exp = static_cast<long long>(LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role_level, 0) * config_item->find_exp_factor );
			if (reward_item->exp > 0 && daily_find_item.find_count > 0)
			{
				reward_item->exp *= daily_find_item.find_count;

				reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
				reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
			}
		}
		break;

	case DAILYFIND_TYPE_PATA:
		{
			short pass_level = m_role->GetRolePataFB()->GetPassLevel();
			for (short i = 1; i <= pass_level; ++i)
			{
				const PataLevelConfig *level_cfg = LOGIC_CONFIG->GetPataFbCfg().GetLevelCfgByLevel(i);
				if (NULL != level_cfg)
				{
					reward_item->exp += level_cfg->reward_exp * daily_find_item.find_count;

					for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && 0 != level_cfg->normal_reward_list[i].item_id; ++ i)
					{
						const ItemConfigData &item_cfg = level_cfg->normal_reward_list[i];
						ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count, 
							item_cfg.item_id, item_cfg.num * daily_find_item.find_count, item_cfg.is_bind);
					}
				}
			}

			if (reward_item->exp > 0 || reward_item->item_count > 0)
			{
				reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
				reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
			}
		}
		break;

	case DAILYFIND_TYPE_STORY_FB:
		{
			for(int i = 0; i <= FB_STORY_MAX_COUNT; i ++)
			{
				const StoryFBLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetStoryFBConfig().GetLevelCfg(i);
				if (NULL == level_cfg) break;

				if (!m_role->GetFBStory()->IsPassLevel(i)) continue;

				int reward_factor = level_cfg->free_times * daily_find_item.find_count;
				reward_item->exp += level_cfg->reward_exp * reward_factor;

				for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && 0 != level_cfg->normal_reward_list[i].item_id; ++ i)
				{
					const ItemConfigData &item_cfg = level_cfg->normal_reward_list[i];
					ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count, 
						item_cfg.item_id, item_cfg.num * reward_factor, item_cfg.is_bind);
				}
			}

			if (reward_item->exp > 0 || reward_item->item_count > 0)
			{
				reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
				reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
			}
		}
		break;

	case DAILYFIND_TYPE_UPGRADE_MOUNT_FB:
		{
			for(int i = 0; i < FB_PHASE_MAX_COUNT; i ++)
			{
				int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
				const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
				if (NULL == level_cfg) continue;

				if (PHASE_FB_TYPE_MOUNT != level_cfg->fb_type) continue;
				if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;

				int reward_factor = level_cfg->free_times * daily_find_item.find_count;
				//reward_item->exp += level_cfg->reward_exp * reward_factor;

				for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && 0 != level_cfg->reset_reward_list[i].item_id; ++ i)
				{
					const ItemConfigData &item_cfg = level_cfg->reset_reward_list[i];
					ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count, 
						item_cfg.item_id, item_cfg.num * reward_factor, item_cfg.is_bind);
				}
			}

			if (reward_item->exp > 0 || reward_item->item_count > 0)
			{
				reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
				reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
			}
		}
		break;

	case DAILYFIND_TYPE_UPGRADE_SHIZHUANG_BODY_FB:
		{
			for(int i = 0; i < FB_PHASE_MAX_COUNT; i ++)
			{
				int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
				const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
				if (NULL == level_cfg) continue;

				if (PHASE_FB_TYPE_SHIZHUANG_BODY != level_cfg->fb_type) continue;
				if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;

				int reward_factor = level_cfg->free_times * daily_find_item.find_count;
				//reward_item->exp += level_cfg->reward_exp * reward_factor;

				for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && 0 != level_cfg->reset_reward_list[i].item_id; ++ i)
				{
					const ItemConfigData &item_cfg = level_cfg->reset_reward_list[i];
					ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count, 
						item_cfg.item_id, item_cfg.num * reward_factor, item_cfg.is_bind);
				}
			}

			if (reward_item->exp > 0 || reward_item->item_count > 0)
			{
				reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
				reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
			}
		}
		break;

	case DAILYFIND_TYPE_UPGRADE_SHIZHUANG_WUQI_FB:
		{
			for(int i = 0; i < FB_PHASE_MAX_COUNT; i ++)
			{
				int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
				const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
				if (NULL == level_cfg) continue;

				if (PHASE_FB_TYPE_SHIZHUANG_WUQI != level_cfg->fb_type) continue;
				if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;

				int reward_factor = level_cfg->free_times * daily_find_item.find_count;
				//reward_item->exp += level_cfg->reward_exp * reward_factor;

				for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && 0 != level_cfg->reset_reward_list[i].item_id; ++ i)
				{
					const ItemConfigData &item_cfg = level_cfg->reset_reward_list[i];
					ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count, 
						item_cfg.item_id, item_cfg.num * reward_factor, item_cfg.is_bind);
				}
			}

			if (reward_item->exp > 0 || reward_item->item_count > 0)
			{
				reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
				reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
			}
		}
		break;

	case DAILYFIND_TYPE_UPGRADE_LINGTONG_FB:
		{
			for(int i = 0; i < FB_PHASE_MAX_COUNT; i ++)
			{
				int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
				const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
				if (NULL == level_cfg) continue;

				if (PHASE_FB_TYPE_LINGTONG != level_cfg->fb_type) continue;
				if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;

				int reward_factor = level_cfg->free_times * daily_find_item.find_count;
				//reward_item->exp += level_cfg->reward_exp * reward_factor;

				for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && 0 != level_cfg->reset_reward_list[i].item_id; ++ i)
				{
					const ItemConfigData &item_cfg = level_cfg->reset_reward_list[i];
					ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count, 
						item_cfg.item_id, item_cfg.num * reward_factor, item_cfg.is_bind);
				}
			}

			if (reward_item->exp > 0 || reward_item->item_count > 0)
			{
				reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
				reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
			}
		}
		break;

	case DAILYFIND_TYPE_UPGRADE_FIGHT_MOUNT_FB:
		{
			for(int i = 0; i < FB_PHASE_MAX_COUNT; i ++)
			{
				int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
				const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
				if (NULL == level_cfg) continue;

				if (PHASE_FB_TYPE_FIGHT_MOUNT != level_cfg->fb_type) continue;
				if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;

				int reward_factor = level_cfg->free_times * daily_find_item.find_count;
				//reward_item->exp += level_cfg->reward_exp * reward_factor;

				for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && 0 != level_cfg->reset_reward_list[i].item_id; ++ i)
				{
					const ItemConfigData &item_cfg = level_cfg->reset_reward_list[i];
					ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count, 
						item_cfg.item_id, item_cfg.num * reward_factor, item_cfg.is_bind);
				}
			}

			if (reward_item->exp > 0 || reward_item->item_count > 0)
			{
				reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
				reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
			}
		}
		break;

	case DAILYFIND_TYPE_UPGRADE_LINGGONG_FB:
	{
		for (int i = 0; i < FB_PHASE_MAX_COUNT; i++)
		{
			int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
			const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
			if (NULL == level_cfg) continue;

			if (PHASE_FB_TYPE_LINGGONG != level_cfg->fb_type) continue;
			if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;

			int reward_factor = level_cfg->free_times * daily_find_item.find_count;
			//reward_item->exp += level_cfg->reward_exp * reward_factor;

			for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && 0 != level_cfg->reset_reward_list[i].item_id; ++i)
			{
				const ItemConfigData &item_cfg = level_cfg->reset_reward_list[i];
				ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count,
					item_cfg.item_id, item_cfg.num * reward_factor, item_cfg.is_bind);
			}
		}

		if (reward_item->exp > 0 || reward_item->item_count > 0)
		{
			reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
			reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
		}
	}
	break;

	case DAILYFIND_TYPE_UPGRADE_LINGQI_FB:
	{
		for (int i = 0; i < FB_PHASE_MAX_COUNT; i++)
		{
			int fb_pass_level = m_role->GetRoleFBPhase()->GetPassLevel(i);
			const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(i, fb_pass_level);
			if (NULL == level_cfg) continue;

			if (PHASE_FB_TYPE_LINGQI != level_cfg->fb_type) continue;
			if (!m_role->GetRoleFBPhase()->IsPassLevel(i)) break;

			int reward_factor = level_cfg->free_times * daily_find_item.find_count;
			//reward_item->exp += level_cfg->reward_exp * reward_factor;

			for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && 0 != level_cfg->reset_reward_list[i].item_id; ++i)
			{
				const ItemConfigData &item_cfg = level_cfg->reset_reward_list[i];
				ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count,
					item_cfg.item_id, item_cfg.num * reward_factor, item_cfg.is_bind);
			}
		}

		if (reward_item->exp > 0 || reward_item->item_count > 0)
		{
			reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
			reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
		}
	}
	break;

	case DAILYFIND_TYPE_TUMO:
	case DAILYFIND_TYPE_GUILD_TASK:
	case DAILYFIND_TYPE_HUSONG:
	case DAILYFIND_TYPE_PAOHUAN:
		{
			long long real_reward_exp = static_cast<long long>( config_item->find_exp_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role_level, 0)));

			reward_item->exp = real_reward_exp * daily_find_item.find_count;
		
			if (reward_item->exp > 0 || config_item->item_count > 0)
			{
				reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
				reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;

				for (int i = 0; i < config_item->item_count; ++i)
				{
					const ItemConfigData &item_cfg = config_item->reward_list[i];
					ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count,
						item_cfg.item_id, item_cfg.num, item_cfg.is_bind);
				}
			}
		}
		break;

// 	case DAILYFIND_TYPE_GUILD_TASK:
// 		{
// 			long long reward_exp = static_cast<long long>(TASKPOOL->GetDailyTaskOtherCfg().exp_factor_guildtask * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role_level, 0)));
// 			const GuildTaskRewardCfg *task_reward = TASKPOOL->GetGuildReward(role_level);
// 			if (NULL != task_reward)
// 			{
// 				reward_item->exp = reward_exp * daily_find_item.find_count;
// 				reward_item->banggong = task_reward->gongxian * daily_find_item.find_count;
// 				for (int i = 0; i < task_reward->item_count; ++ i)
// 				{
// 					const ItemConfigData &item_cfg = task_reward->item_list[i];
// 					ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count, 
// 						item_cfg.item_id, item_cfg.num * daily_find_item.find_count, item_cfg.is_bind);
// 				}
// 
// 				if (reward_item->exp > 0 || reward_item->item_count > 0)
// 				{
// 					reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
// 					reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
// 				}
// 			}
// 
// 			// 完成所有公会任务奖励
// 			{
// 				const ItemConfigData &all_finish_item = TASKPOOL->GetGuildTaskAllCompleteReward();
// 				ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count, 
// 					all_finish_item.item_id, all_finish_item.num, all_finish_item.is_bind);
// 			}
// 		}
// 		break;

	case DAILYFIND_TYPE_QUESTION:
		{
			const HSQuestionRewardCfg *cfg = LOGIC_CONFIG->GetQuestionConfig().GetHotSpringRewardCfg(role_level);
			if (NULL != cfg)
			{
				reward_item->exp = cfg->exp * daily_find_item.find_count;

				if (reward_item->exp > 0)
				{
					reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
					reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
				}
			}
		}
		break;
// 	case DAILYFIND_TYPE_EQUIP_FB:
// 	{
// 		int chapter = -1; int level = -1;
// 		m_role->GetFBEquip()->CanDailyFind(&chapter, &level);
// 		const NeqFBLevelCfg * level_cfg = LOGIC_CONFIG->GetNeqFBConfig().GetLevelCfg(chapter, level);
// 		if (NULL != level_cfg)
// 		{
// 			for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < DailyFindRewardItem::ITEM_MAX_COUNT; ++i)
// 			{
// 				ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count,
// 					level_cfg->find_reward[i].item_id, level_cfg->find_reward[i].num * daily_find_item.find_count, level_cfg->find_reward[i].is_bind);
// 			}
// 		}
// 		if (reward_item->exp > 0 || reward_item->item_count > 0)
// 		{
// 			reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
// 			reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
// 		}
// 	}
// 	break;
// 
// 	case DAILYFIND_TYPE_TOWER_DEFEND_FB:
// 	{
// 		int level = m_role->GetFBTowerDefend()->GetPersonalFBPassLevel();
// 		const TowerDefendLevelCfg::ConfigItem * cfg_item= LOGIC_CONFIG->GetTowerDefendConfig().GetLevelCfg(level);
// 		if (NULL != cfg_item)
// 		{
// 			for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < DailyFindRewardItem::ITEM_MAX_COUNT; ++i)
// 			{
// 				ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count,
// 					cfg_item->find_reward[i].item_id, cfg_item->find_reward[i].num * daily_find_item.find_count, cfg_item->find_reward[i].is_bind);
// 			}
// 		}
// 		if (reward_item->exp > 0 || reward_item->item_count > 0)
// 		{
// 			reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
// 			reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
// 		}
// 	}
// 	break;
// 	case DAILYFIND_TYPE_CHALLENGE_FB:
// 	{
// 		int pass_level = m_role->GetFBChallenge()->GetPassLevel();
// 		const ChallengeFBLevelCfg::ConfigItem * cfg_item = LOGIC_CONFIG->GetChallengeFBConfig().GetLevelCfg(pass_level);
// 		if (cfg_item != NULL)
// 		{
// 			for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < DailyFindRewardItem::ITEM_MAX_COUNT; ++i)
// 			{
// 				ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count,
// 					cfg_item->find_reward[i].item_id, cfg_item->find_reward[i].num * daily_find_item.find_count, cfg_item->find_reward[i].is_bind);
// 			}
// 		}
// 		if (reward_item->exp > 0 || reward_item->item_count > 0)
// 		{
// 			reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
// 			reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
// 		}
// 	}
// 	break;
// 	case DAILYFIND_TYPE_ARMOR_FB:
// 	{
// 		int level = m_role->GetRoleFbArmorDefend()->GetArmoedefendPassLayer();
// 		const ArmorDefendLevelCfg::ConfigItem * cfg_item = LOGIC_CONFIG->GetArmorDefendConfig().GetLevelCfg(level);
// 		if (NULL != cfg_item)
// 		{
// 			for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < DailyFindRewardItem::ITEM_MAX_COUNT; ++i)
// 			{
// 				ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count,
// 					cfg_item->find_reward[i].item_id, cfg_item->find_reward[i].num * daily_find_item.find_count, cfg_item->find_reward[i].is_bind);
// 			}
// 		}
// 		if (reward_item->exp > 0 || reward_item->item_count > 0)
// 		{
// 			reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
// 			reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
// 		}
// 	}
// 	break;
// 	case DAILYFIND_TYPE_TEAM_EQUIP_FB:
// 	{
// 		ItemConfigData reward_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
// 		long long reward_exp = 0;
// 		int count = m_role->GetFBEquip()->GetDailyFindRewardTeam(ItemNamespace::MAX_KNAPSACK_GRID_NUM,reward_list,&reward_exp);
// 		if (count > 0 || reward_exp > 0)
// 		{
// 			for (int i = 0; i < count && i < DailyFindRewardItem::ITEM_MAX_COUNT; ++i)
// 			{
// 				ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count,
// 					reward_list[i].item_id, reward_list[i].num * daily_find_item.find_count, reward_list[i].is_bind);
// 			}
// 		}
// 
// 		if (reward_item->exp > 0 || reward_item->item_count > 0)
// 		{
// 			reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
// 			reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
// 		}
// 	}
// 	break;
// 	case DAILYFIND_TYPE_TEAM_TOWER_DEFEND_FB:
// 	{
// 		ItemConfigData reward_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
// 		int get_xiannv_shengwu = 0;
// 		int count = m_role->GetFBTowerDefend()->GetDailyFindRewardTeam(ItemNamespace::MAX_KNAPSACK_GRID_NUM,reward_list, &get_xiannv_shengwu);
// 		if (count > 0 || get_xiannv_shengwu > 0)
// 		{
// 			if (NULL != xiannv_shengwu)
// 			{
// 				*xiannv_shengwu = get_xiannv_shengwu;
// 			}
// 
// 			for (int i = 0; i < count && i < DailyFindRewardItem::ITEM_MAX_COUNT; ++i)
// 			{
// 				ItemExtern::PutInItemConfigList(DailyFindRewardItem::ITEM_MAX_COUNT, reward_item->item_list, &reward_item->item_count,
// 					reward_list[i].item_id, reward_list[i].num * daily_find_item.find_count, reward_list[i].is_bind);
// 			}
// 		}
// 
// 		if (reward_item->exp > 0 || reward_item->item_count > 0)
// 		{
// 			reward_item->gold_need = config_item->gold_regular_cost + config_item->gold_cost_per_count * daily_find_item.find_count;
// 			reward_item->coin_need = config_item->coin_cost_per_count * daily_find_item.find_count;
// 		}
// 	}
// 	break;
	}		
}

void Welfare::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	{
		if (0 < m_role->GetCommonDataParam()->millionaire_gather_count)
		{
			m_role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_MILLIONAIRE);
		}

		// 检查活动能否找回
		this->CheckActvityCanFind(m_offline_pass_days + 1, m_activity_join_flag, m_activity_auto_flag);
		m_activity_join_flag = 0;
		m_activity_auto_flag = 0;
		m_offline_pass_days = 0;		
	}

	{
		// 日常找回
		for (int i = 0; i < DAILYFIND_TYPE_MAX; i++)
		{
			const DailyFindConfigItem *item = LOGIC_CONFIG->GetWelfareConfig().GetDailyFindItem(i);
			if (NULL == item) continue;

			int day_count = 0;

			if (0 != item->daycount_id)
			{
				day_count = m_role->GetDayCounter()->GetDayCount(item->daycount_id);
			}
			else
			{
				day_count = this->GetDailyFindDayCount(i);
			}

			if (day_count >= 0)
			{
				int find_count = item->count_perday - day_count;
				if (find_count > 0)
				{
					unsigned int find_time = now - (m_offline_days + 1) * WELFARE_SECONDS_ONE_DAY;
					this->OnDailyFindChange(i, find_time, find_count);
				}
			}
		}

		m_offline_days = 0;
	}

	m_happy_tree_online_time = 0;
	m_today_chongzhi_gold_num = 0;

	{ 
		// 一生累计登录天数
		++ m_account_total_login_daycount;
		this->SendTotalLoginDays();
		this->SendSevenDayLoginRewardInfo();

		m_sign_in_today_times = 0; 
		m_is_chongzhi_today = 0;
		m_online_reward_mark = 0;
		
		memset(m_happy_tree_grouth_val, 0 ,sizeof(m_happy_tree_grouth_val));

		EventHandler::Instance().OnAccountTotalLoginDayCountChange(m_role, m_account_total_login_daycount);
	}

	this->SendWelfareInfo();
}

void Welfare::OnMonthChange()
{
	m_sign_in_days = 0;			// 先ondaychange后onMonthChange，有时序问题。

	m_accumulation_signin_days = 0;
	m_sign_in_reward_mark = 0;
	m_continuity_signin_days = 0;
	this->SendWelfareInfo();
}

void Welfare::CheckSignIn()
{
// 	if (0 == m_sign_in_today)
// 	{
// 		m_sign_in_today = 1;
// 		++ m_sign_in_days;
// 
// 		this->SendWelfareInfo();
// 
// 		gamelog::g_log_welfare.printf(LL_INFO, "SignIn::CheckSignIn, user[%d %s], sign_in_days[%d]", 
// 			m_role->GetUID(), m_role->GetName(), m_sign_in_days);
// 	}
}

void Welfare::CheckActvityCanFind(int check_day, int activity_join_flag, int auto_activity_flag)
{
	if (check_day <= 0 || check_day >= ACTIVITY_OPEN_RECORD_MAX_COUNT)
	{
		return;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	ActivityOpenRecords &open_records = WorldStatus::Instance().GetActivityOpenRecords();

	for (int i = 0; i < ACTIVITY_FIND_TYPE_MAX; ++ i)
	{
		if (0 != (m_activity_find_flag & (1 << i)))	
		{
			continue;
		}

		if (0 != (m_activity_join_flag & (1 << i)))
		{
			continue;
		}

		if (0 != (m_activity_auto_flag & (1 << i)))
		{
			continue;
		}

		if ( ACTIVITY_FIND_TYPE_GONGCHENGZHAN == i || ACTIVITY_FIND_TYPE_GUILDBATTLE == i || ACTIVITY_FIND_TYPE_TERRITORYWAR == i||
			ACTIVITY_FIND_TYPE_BANGPAIDATI == i || ACTIVITY_FIND_TYPE_GUILD_YUNBIAO == i || ACTIVITY_FIND_TYPE_GUILD_FB == i || 
			ACTIVITY_FIND_TYPE_GUILD_YANHUI == i || ACTIVITY_FIND_TYPE_CROSS_GUILDBATTLE == i)
		{
			if (INVALID_GUILD_ID == m_role->GetGuildID())
			{
				continue;
			}
		}

		const WelfareActivityFindConfig::ConfigItem * cfg_item = LOGIC_CONFIG->GetWelfareConfig().GetActivityFindConfig(i);
		if (NULL == cfg_item || cfg_item->is_open <= ACTIVITY_FIND_OPEN_INVALID)
		{
			continue;
		}

		if (m_role->GetLevel() < cfg_item->level_min || m_role->GetLevel() > cfg_item->level_max)
		{
			continue;
		}
		
		int open_gam_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		if (open_gam_day < cfg_item->open_day_min || open_gam_day > cfg_item->open_day_max)
		{
			continue;
		}

// 		JoinLimitUnit join_limit_unit;
// 		if (LOGIC_CONFIG->GetJoinLimitConfig().GetActivityJoinLimit(cfg_item->activity_type, &join_limit_unit))
// 		{
// 			if (m_role->GetLevel() < join_limit_unit.level || m_role->GetLevel() > join_limit_unit.max_level)
// 			{
// 				continue;
// 			}
// 		}

		if (open_records.IsActivityOpen(check_day, i))
		{
			m_activity_find_flag |= 1 << i;
		}
	}
}


void Welfare::CheckRewardActiveDegree(int activity_find_type)
{
// 	switch(activity_find_type)
// 	{
// 	case ACTIVITY_FIND_TYPE_GONGCHENGZHAN:
// 		{
// 			const ActiveDegreeInfo *active_degree_info = LOGIC_CONFIG->GetActiveDegreeConfig().GetActiveDegreeInfo(ACTIVEDEGREE_TYPE_GONGCHENGZHAN);
// 			if (NULL != active_degree_info)
// 			{
// 				for (int i = 0; i < active_degree_info->need_count; i ++)
// 				{
// 					EventHandler::Instance().OnEnterXianmengzhan(m_role);
// 				}
// 			}
// 		}
// 		break;
// 
// 	case ACTIVITY_FIND_TYPE_ZHUXIE:
// 		{
// // 			EventHandler::Instance().OnEnterZhuXieZhanChang(m_role);
// // 
// // 			const ActiveDegreeInfo *active_degree_info = LOGIC_CONFIG->GetActiveDegreeConfig().GetActiveDegreeInfo(ACTIVEDEGREE_TYPE_ZHUXIE);
// // 			if (NULL != active_degree_info)
// // 			{
// // 				for (int i = 0; i < active_degree_info->need_count; i ++)
// // 				{
// // 					EventHandler::Instance().OnCompleteZhuXieTask(m_role);
// // 				}
// // 			}
// 		}
// 		break;
// 
// // 	case ACTIVITY_FIND_TYPE_NATIONAL_BOSS:
// // 		{
// // 			EventHandler::Instance().OnEnterNationalBoss(m_role);
// // 		}
// // 		break; 
// 
// 	default:
// 		break;
// 	}
}

bool Welfare::IsCanAutoActivityType(int activity_find_type)
{
	return false;
}

int Welfare::GetTotalLoginDays()
{
	return m_account_total_login_daycount;
}

void Welfare::SendTotalLoginDays()
{
	Protocol::SCTotalLoginDays tld;
	tld.total_login_day = m_account_total_login_daycount;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tld, sizeof(tld));
}

//gm命令
void Welfare::SignInByDay(int day)
{
	if (day <= 0 || day > 31)
	{
		return;
	}

	m_sign_in_days |= 1 << (day - 1);
	m_continuity_signin_days++;

	this->CalcAccumulationSignInDays();
	this->SendWelfareInfo();
}

void Welfare::FetchChongjihaoliReward(int level)
{
	if (level <= 0) return;

	const WelfaceChongjihaoli * reward_cfg = LOGIC_CONFIG->GetWelfareConfig().GetChongjihaoliItemCfg(level);
	if (NULL == reward_cfg) return;
	
	if (0 != (m_chongjihaoli_reward_mark & (1 << reward_cfg->index)))
	{
		return;
	}

	m_chongjihaoli_reward_mark |= (1 << reward_cfg->index);
	
	if (1 == reward_cfg->is_limit_num)
	{
		if(WorldStatus::Instance().GetCommonData().total_chongjihaoli_record_list[reward_cfg->index] >= reward_cfg->limit_num)
		{
			m_role->NoticeNum(errornum::EN_WELFARE_CHONGJIHAOLI_NOT_ENOUGH);
			return;
		}
	}

	WorldStatus::Instance().GetCommonData().total_chongjihaoli_record_list[reward_cfg->index] ++;

	m_role->GetKnapsack()->PutListOrMail(reward_cfg->reward_item_list, PUT_REASON_WELFARE);

	{
		// 传闻
		if (200 == level)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_chongjihaoli,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
			if (length > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	this->SendWelfareInfo();
}

void Welfare::AddHappytreeGrowthvalue(int type, int add_value)
{
// 	if (type < 0 || type >= GET_HAPPYTREE_VALUE_TYPE_MAX)
// 	{
// 		return;
// 	}
// 
// 	const WelfareGetGrowthConfig::configitem * value_cfg = LOGIC_CONFIG->GetWelfareConfig().GetGrowthCfg(type);
// 	if (NULL == value_cfg && value_cfg->add_value <= 0)
// 	{
// 		return;
// 	}
// 
// 	const WelfareGrowthValueConfig::GrowthCfg *growth_cfg = LOGIC_CONFIG->GetWelfareConfig().GetGrowthValueCfg(m_happy_tree_level, type);
// 	if (NULL == growth_cfg)
// 	{
// 		return;
// 	}
// 
// 	if (add_value > 0)
// 	{
// 		if (add_value + m_happy_tree_grouth_val[type] > growth_cfg->max_growth_value)
// 		{
// 			add_value = growth_cfg->max_growth_value - m_happy_tree_grouth_val[type];
// 		}
// 
// 		if (GET_HAPPYTREE_VALUE_TYPE_CHONGZHI == type)
// 		{
// 			m_today_chongzhi_gold_num -= 10 * add_value;
// 		}
// 
// 		m_happy_tree_grouth_val[type] += add_value;
// 		m_total_happy_tree_growth_val += add_value;
// 		m_role->GetRoleShop()->AddChestShopHappyTreeGrow(add_value, "Welfare");
// 
// 		this->SendWelfareInfo();
// 	}
// 	else
// 	{
// 		add_value = value_cfg->add_value;
// 		if (m_happy_tree_grouth_val[type] + add_value > growth_cfg->max_growth_value)
// 		{
// 			add_value = growth_cfg->max_growth_value - m_happy_tree_grouth_val[type];
// 		}
// 
// 		m_happy_tree_grouth_val[type] += add_value;
// 		m_total_happy_tree_growth_val += add_value;
// 		m_role->GetRoleShop()->AddChestShopHappyTreeGrow(add_value, "Welfare");
// 
// 		this->SendWelfareInfo();
// 	}
}

void Welfare::FetchHappyTreeReward(int type)
{
// 	const WelfareHappyTreeConfig * level_cfg = LOGIC_CONFIG->GetWelfareConfig().GetHappyTreeLevelCfg(m_happy_tree_level);
// 	if (type < 0 || type >= level_cfg->item_count)
// 	{
// 		return;
// 	}
// 
// 	const WelfareHappyTreeConfig::RewardCfg * happy_reward = LOGIC_CONFIG->GetWelfareConfig().GetHappyTreeReward(m_happy_tree_level, type + 1);
// 	if (happy_reward == NULL)
// 	{
// 		return;
// 	}
// 
// 	if (happy_reward->growth_val > m_total_happy_tree_growth_val)
// 	{
// 		m_role->NoticeNum(errornum::NT_WELFARE_GROWTH_VAL_NOT_ENOUGTH);
// 		return;
// 	}
// 
// 	if (0 != (m_happy_tree_reward & (1 << type)))
// 	{
// 		m_role->NoticeNum(errornum::NT_WELFARE_ALREADY_FETCH_HAPPYTREE_REWARD);
// 		return;
// 	}
// 
// 	if (!m_role->GetKnapsack()->HaveSpace())
// 	{
// 		m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
// 		return;
// 	}
// 
// 	if (m_role->GetKnapsack()->Put(happy_reward->reward_item, PUT_REASON_HAPPY_TREE_REWARD))
// 	{
// 		m_happy_tree_reward |= (1 << type);
// 
// 		int already_fetch_num = 0;
// 		for(int i = 0; i < level_cfg->item_count && i < WelfareHappyTreeConfig::MAX_FETCH_ITEM_COUT; ++ i)
// 		{
// 			if (0 != (m_happy_tree_reward & (1 << i)))
// 			{
// 				++ already_fetch_num;
// 			}
// 		}
// 
// 		if (already_fetch_num == level_cfg->item_count)
// 		{
// 			m_happy_tree_reward = 0;
// 			m_happy_tree_level += 1;
// 			m_role->NoticeNum(noticenum::NT_WELFARE_HAPPY_TREE_UPLEVEL);
// 		}
// 
// 		this->SendWelfareInfo();
// 	}
}

void Welfare::ExchangeReward(int part)
{
	if (part < 0 || part > WelfareExchangeConfig::MAX_CAN_EXCHANGEITEM_REWARD_ITEM_NUM)
	{
		return;
	}

	const WelfareExchangeConfig::ExchangeCfg *ex_cfg = LOGIC_CONFIG->GetWelfareConfig().GetExchangeCfg(part);	
	if (ex_cfg == NULL)
	{
		return;
	}

	if (m_total_happy_tree_growth_val < ex_cfg->exchange_growth_val)
	{
		m_role->NoticeNum(errornum::NT_WELFARE_GROWTH_VAL_NOT_ENOUGTH);
		return;
	}

	if (m_role->GetKnapsack()->Put(ex_cfg->reward_item, PUT_REASON_EXCHANG_REWARD))
	{
		m_total_happy_tree_growth_val -= ex_cfg->exchange_growth_val;
		m_role->NoticeNum(noticenum::NT_WELFARE_EXCHANG_SUCCESS);
	}
}

void Welfare::GMAddTotalGrouthVal(int value)
{
	m_total_happy_tree_growth_val = value;

	this->SendWelfareInfo();
}

void Welfare::FetchSevenDayLoginReward(int fetch_day)
{
	SevenDayLoginConfig &seven_day_cfg = LOGIC_CONFIG->GetSevenDayLoginConfig();

	if (fetch_day <= 0 || fetch_day >= (int)sizeof(m_seven_day_login_fetch_reward_mark) * 8 || 
		fetch_day > seven_day_cfg.GetMaxRewardDay()) return;

	if (fetch_day > m_account_total_login_daycount)
	{
		m_role->NoticeNum(errornum::EN_LOGIN_DAY_COUNT_DISSATISFY);
		return;
	}

	if ((m_seven_day_login_fetch_reward_mark & (1 << fetch_day)) != 0)
	{
		m_role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
		return;
	}

	const ItemConfigData *reward_item = seven_day_cfg.getRewardItem(fetch_day);
	if (NULL == reward_item) return;

	if (m_role->GetKnapsack()->Put(*reward_item, PUT_REASON_SEVEN_DAY_LOGIN_REWARD))
	{
		m_seven_day_login_fetch_reward_mark |= (1 << fetch_day);
		this->SendSevenDayLoginRewardInfo(Protocol::SCSevenDayLoginRewardInfo::SDLRI_FETCH_REWARD);

		gamelog::g_log_welfare.printf(LL_INFO, "FetchSevenDayLogin user[%d, %s], reward_item[%s]", m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(reward_item, NULL));
	}
}

void Welfare::SendSevenDayLoginRewardInfo(int notify_reason)
{
	Protocol::SCSevenDayLoginRewardInfo sdlri;
	sdlri.notify_reason = notify_reason;
	sdlri.account_total_login_daycount = m_account_total_login_daycount;
	sdlri.seven_day_login_fetch_reward_mark = m_seven_day_login_fetch_reward_mark;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sdlri, sizeof(sdlri));
}

void Welfare::SetSaveExpBuff()
{
	EffectOtherBuff *buff = m_role->GetEffectList()->FindOtherEffectBuff(EffectBase::PRODUCT_ID_INC_EXTRAL_EXP_PER);
	if (NULL != buff && EffectBase::PRODUCT_METHOD_ITEM == buff->GetProductMethod())
	{
		int buff_type = buff->GetProductLevel() - 1;
		if (EXP_BUFF_TYPE_INVALID < buff_type && buff_type < EXP_BUFF_TYPE_MAX)
		{
			ExpBuffInfo::ExpBuffItem &buff_info = m_exp_buff_info.exp_buff_list[buff_type];
			buff_info.exp_buff_left_time_s = buff->GetLeftTimeMS() / 1000;
			buff_info.exp_buff_rate = buff->GetIncExtraExpPer();
			buff_info.exp_buff_level = buff->GetProductLevel();

			SendExpBuffInfo();
		}
	}
}

int Welfare::GetCurExpBuffLeftTime()
{
	int cur_exp_buff_left_time_s = 0;
	int buff_type = m_exp_buff_info.GetCurMaxExpBuffType();
	if (buff_type > EXP_BUFF_TYPE_INVALID && buff_type < EXP_BUFF_TYPE_MAX)
	{
		cur_exp_buff_left_time_s = m_exp_buff_info.exp_buff_list[buff_type].exp_buff_left_time_s;
	}

	return cur_exp_buff_left_time_s;
}

void Welfare::SendExpBuffInfo()
{
	Protocol::SCExpBuffInfo ebi;
	
	UNSTD_STATIC_CHECK(sizeof(ebi.exp_buff_list) == sizeof(m_exp_buff_info.exp_buff_list));
	memcpy(ebi.exp_buff_list, m_exp_buff_info.exp_buff_list, sizeof(ebi.exp_buff_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ebi, sizeof(ebi));
}

void Welfare::CalcOfflineExpBuffTime(unsigned int now_second, long long last_save_time, int &offline_exp_buff_time, int &offline_exp_buff_rate)
{
// 	if (m_exp_buff_left_time_s > 0)
// 	{
// 		m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFEECT_TYPE_OTHER, EBT_INVALID, INVALID_OBJ_ID,
// 			EffectBase::PRODUCT_METHOD_ITEM, EffectBase::PRODUCT_ID_INC_EXTRAL_EXP_PER);
// 
// 		if (now_second - last_save_time < m_exp_buff_left_time_s)
// 		{
// 			// BUFF持续尚未结束
// 			int time = (m_exp_buff_left_time_s - (int)(now_second - last_save_time)) * 1000;
// 			if (time > 0 && m_exp_buff_level > 0 && m_exp_buff_rate > 0)
// 			{
// 				EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_INC_EXTRAL_EXP_PER, time, m_exp_buff_level, EffectBase::PRODUCT_METHOD_ITEM);
// 				effect->IncExtraExpPer(m_exp_buff_rate);
// 				effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);
// 				if (1 == m_exp_buff_level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP1);
// 				if (2 == m_exp_buff_level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP2);
// 				if (3 == m_exp_buff_level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP3);
// 				if (4 == m_exp_buff_level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP4);
// 				m_role->AddEffect(effect);
// 			}
// 
// 			//if (2 == m_exp_buff_level)
// 			//{
// 				m_exp_double_effect_time += (int)(now_second - last_save_time);  // 只利用这一个存储，反正每次上线都会自动发放并且清空，玩家不可能同时拥有两个buff
// 				m_exp_double_effect_rate = m_exp_buff_rate;
// 			//}
// 			//else if (3 == m_exp_buff_level)
// 			//{
// 			//	m_exp_triple_effect_time += (int)(now_second - last_save_time);
// 			//}
// 
// 			offline_exp_buff_time = (int)(now_second - last_save_time);
// 		}
// 		else
// 		{
// 			//if (2 == m_exp_buff_level)
// 			//{
// 				m_exp_double_effect_time += m_exp_buff_left_time_s;
// 				m_exp_double_effect_rate = m_exp_buff_rate;
// 			//}
// 			//else if (3 == m_exp_buff_level)
// 			//{
// 			//	m_exp_triple_effect_time += m_exp_buff_left_time_s;
// 			//}
// 
// 			offline_exp_buff_time = m_exp_buff_left_time_s;
// 		}
// 
// 		offline_exp_buff_rate = m_exp_buff_rate;
// 
// 		if (offline_exp_buff_time > OFFLINE_EXP_GET_MAX_TIME) offline_exp_buff_time = OFFLINE_EXP_GET_MAX_TIME;
// 		if (m_exp_double_effect_time > OFFLINE_EXP_GET_MAX_TIME) m_exp_double_effect_time = OFFLINE_EXP_GET_MAX_TIME;
// 		if (m_exp_triple_effect_time > OFFLINE_EXP_GET_MAX_TIME) m_exp_triple_effect_time = OFFLINE_EXP_GET_MAX_TIME;
// 
// 	}
// 		
// 	m_exp_buff_left_time_s = 0; // 重置BUFF记录
// 	m_exp_buff_level = 0;
// 	m_exp_buff_rate = 0;
}

int Welfare::CalcOfflineExpBuffLeftCost()
{
	int cur_exp_buff_left_times = GetCurExpBuffLeftTime();
	if (m_offline_second <= cur_exp_buff_left_times)  // 不需要双倍领取
	{
		return 0;
	}

	if (m_offline_second <= cur_exp_buff_left_times)  // 不需要双倍领取
	{
		return 0;
	}

	if (m_offline_second <= cur_exp_buff_left_times)  // 不需要双倍领取
	{
		return 0;
	}

	const WelfareOtherConfig &welfare_other_cfg = LOGIC_CONFIG->GetWelfareConfig().GetOtherCfg();

	// 需要消耗
	static const ItemID EXP_BUFF_ITEM = welfare_other_cfg.exp_itemid; //双倍经验药水
	int gold_price = 0;
	int bind_gold_price = 0;
	int need_count = static_cast<int>((m_offline_second - cur_exp_buff_left_times) / 1800 + 1);  // 半小时

	if (need_count <= 0)
	{
		return 0;
	}

	if (!LOGIC_CONFIG->GetShop().GetShopItemGoldPrice(EXP_BUFF_ITEM, &gold_price, &bind_gold_price) || gold_price <= 0)
	{
		return 0;
	}

	int cost_gold = need_count * gold_price;

	return cost_gold;
}
