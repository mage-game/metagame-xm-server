#include "roleguild.hpp"

#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/guilddef.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/redpaper/redpapermanager.h"

#include "protocal/msgscguild.hpp"
#include "item/knapsack.h"
#include "other/event/eventhandler.hpp"
#include "other/capability/capability.hpp"
#include "servercommon/crossroleglobaldef.hpp"
#include "item/itempool.h"

RoleGuild::RoleGuild() : m_role(NULL)
{

}

RoleGuild::~RoleGuild() 
{

}

void RoleGuild::Init(Role *role, const RoleGuildParam &role_guild_param)
{
	m_role = role;
	m_guild_param = role_guild_param;
}

void RoleGuild::GetInitParam(RoleGuildParam *role_guild_param)
{
	*role_guild_param = m_guild_param;
}

void RoleGuild::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		// 技能
		for (int i = 0; i < GUILD_SKILL_COUNT; i++)
		{
			const GuildSkillCfg::ConfigItem *cfg = LOGIC_CONFIG->GetGuildConfig().GetSkillConfig(i, m_guild_param.skill_level_list[i]);
			if (NULL != cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg->jianren;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_ZHUFUYIJI_PER] += cfg->zhufuyiji_per;
			}
		}


		// 图腾
		Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
		if (NULL != guild)
		{
			const GuildTotemCfg::ConfigItem *cfg = LOGIC_CONFIG->GetGuildConfig().GetTutemConfig(guild->GetTotemLevel());
			if (NULL != cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg->jianren;

				if (guild->GetGuildMemberManager().GetTuanZhangUid() == m_role->GetUID())
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->leader_maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->leader_gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->leader_fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg->leader_mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg->leader_shanbi;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg->leader_baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg->leader_jianren;
				}
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_GUILD, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void RoleGuild::GetRoleCrossData(RoleCrossGlobalData &data)
{
	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL != guild)
	{
		static CharIntAttrs tmp_attr;
		tmp_attr.Reset();

		const GuildTotemCfg::ConfigItem *cfg = LOGIC_CONFIG->GetGuildConfig().GetTutemConfig(guild->GetTotemLevel());
		if (NULL != cfg)
		{
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->maxhp;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->gongji;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->fangyu;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg->mingzhong;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg->shanbi;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg->baoji;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg->jianren;

			if (guild->GetGuildMemberManager().GetTuanZhangUid() == m_role->GetUID())
			{
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->leader_maxhp;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->leader_gongji;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->leader_fangyu;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg->leader_mingzhong;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg->leader_shanbi;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg->leader_baoji;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg->leader_jianren;
			}
		}

		for (int i = 0; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX - CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; i++)
		{
			data.attrs[i] += tmp_attr.m_attrs[i + CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN];
		}
	}
}

void RoleGuild::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;
	
	m_guild_param.ClearExchangeRecord();
	this->SetDailyGongXian(0);
	this->ResetTerritoryRewardDailyFlag();
	this->ResetTotemExp();
	m_guild_param.maze_reward_layer = 0;
	m_guild_param.guild_tianci_tongbi_reward_limit = 0;
	m_guild_param.join_tiancitongbi_guild_id = 0;
	m_guild_param.today_juanxian_gold = 0;

	m_guild_param.kill_count = 0;						//击杀次数每日重置
	m_guild_param.killed_count = 0;						//被击杀次数每日重置

	this->ResetTodayGuildPaoSaiziTimes();

	this->SendRoleGuildInfo();
}

void RoleGuild::OnWeekChange()
{

}

void RoleGuild::GuildExchange(ItemID item_id, short exchange_num)
{
	if (item_id <= 0 || exchange_num <= 0)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (INVALID_GUILD_ID == m_role->GetGuildID())
	{
		m_role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL == guild)
	{
		m_role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	const GuildExchangeConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetGuildConfig().GetExchangeConfig(item_id);
	if (NULL == cfg_item)
	{
		return;
	}

	if (guild->GetGuildLevel() < cfg_item->guild_level_limit)
	{
		m_role->NoticeNum(errornum::EN_GUILD_LEVEL_LIMIT);
		return;
	}

	if (m_role->GetGuildGongxian() < cfg_item->cost_gongxian * exchange_num)
	{
		m_role->NoticeNum(errornum::EN_GUILD_EXCHANGE_NOT_ENOUGH_GONG_XIAN);
		return;
	}

	//检查并使用今日次数限制
	{
		short history_exchange_num = 0;
		int record_index = -1;

		for (int i = 0; i < m_guild_param.exchange_record_total_num && i < GUILD_MAX_EXCHANGE_ITEM_COUNT; ++i)
		{
			if (item_id == m_guild_param.exchange_record_list[i].item_id)
			{
				history_exchange_num = m_guild_param.exchange_record_list[i].item_num;
				record_index = i;
				break;
			}
		}

		if (history_exchange_num + exchange_num > short(cfg_item->max_day_exchange_num))
		{
			m_role->NoticeNum(errornum::EN_GUILD_OVER_EXCHANGE_TIMES);
			return;
		}

		if (-1 == record_index && m_guild_param.exchange_record_total_num < GUILD_MAX_EXCHANGE_ITEM_COUNT)
		{
			m_guild_param.exchange_record_list[m_guild_param.exchange_record_total_num].item_id = item_id;
			m_guild_param.exchange_record_list[m_guild_param.exchange_record_total_num].item_num = exchange_num;
			++ m_guild_param.exchange_record_total_num;
		}
		else if (record_index >= 0 && record_index < GUILD_MAX_EXCHANGE_ITEM_COUNT)
		{
			m_guild_param.exchange_record_list[record_index].item_num += exchange_num;
		}
	}
	
	m_role->AddGuildGongxian(-1 * abs(cfg_item->cost_gongxian * exchange_num), "ReduceByExchange");
	
	ItemDataWrapper item_wrapper(cfg_item->item_id, 1, 1);
	knapsack->Put(item_wrapper, PUT_REASON_NO_NOTICE);

	this->SendRoleGuildInfo();
}

void RoleGuild::SendRoleGuildInfo()
{
	static Protocol::SCGuildRoleGuildInfo msg;

	msg.guild_gongxian = m_role->GetGuildGongxian();
	msg.territorywar_reward_flag = m_role->GetRoleGuild()->GetTerritoryWelfFlag();
	msg.reserve = 0;
	msg.daily_gongxian = m_role->GetRoleGuild()->GetDailyGongXian();
	msg.day_juanxian_gold = m_guild_param.today_juanxian_gold;
	memcpy(msg.skill_level_list, m_guild_param.skill_level_list, sizeof(msg.skill_level_list));
	msg.item_count = m_guild_param.exchange_record_total_num;
	memcpy(msg.exchange_list, m_guild_param.exchange_record_list, sizeof(msg.exchange_list));
	UInt32 sendlen = sizeof(Protocol::SCGuildRoleGuildInfo) - sizeof(RoleGuildParam::ExchangeRecordItem) * (GUILD_MAX_EXCHANGE_ITEM_COUNT - msg.item_count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&msg, sendlen);
}

void RoleGuild::OnRoleLogin()
{
	this->OpenServerSyncRedPapaerInfo();
}

void RoleGuild::OnSkillUplevel(int skill_index)
{
	if (skill_index < 0 || skill_index >= GUILD_SKILL_COUNT)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (INVALID_GUILD_ID == m_role->GetGuildID())
	{
		m_role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL == guild)
	{
		m_role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	const GuildSkillCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetGuildConfig().GetSkillConfig(skill_index, m_guild_param.skill_level_list[skill_index]);
	if (NULL == cfg_item)
	{
		return;
	}

	const GuildSkillCfg::ConfigItem *next_cfg_item = LOGIC_CONFIG->GetGuildConfig().GetSkillConfig(skill_index, m_guild_param.skill_level_list[skill_index] + 1);
	if (NULL == next_cfg_item)
	{
		return;
	}

	if (guild->GetGuildLevel() < cfg_item->guild_level_limit)
	{
		m_role->NoticeNum(errornum::EN_XIANMENGZHAN_GUILD_LEVEL_LIMIT);
		return;
	}

	if (m_role->GetGuildGongxian() < cfg_item->uplevel_gongxian)
	{
		return;
	}
	//由配置决定是否消耗物品
	if (nullptr != ITEMPOOL->GetItem(cfg_item->uplevel_stuff_id))
	{
		static ItemExtern::ItemConsumeList consume_list;
		consume_list.Reset();

		int stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		short uplevel_stuff_num = m_role->GetKnapsack()->Count(cfg_item->uplevel_stuff_id);

		if (uplevel_stuff_num >= cfg_item->uplevel_stuff_count && stuff_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
		{
			stuff_list[stuff_count].item_id = cfg_item->uplevel_stuff_id;
			stuff_list[stuff_count].num = cfg_item->uplevel_stuff_count;
			stuff_list[stuff_count].buyable = false;

			++stuff_count;
		}
		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, false))
		{
			return;
		}
		// 道具扣除
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "roleguild::OnSkillUplevel"))
		{
			return;
		}
	}

	m_role->AddGuildGongxian(-cfg_item->uplevel_gongxian, "GuildSkillUplevel");

	m_guild_param.skill_level_list[skill_index]++;

	this->SendRoleGuildInfo();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GUILD);
}

//void RoleGuild::OnSkillUplevelALL()
//{
//	Knapsack *knapsack = m_role->GetKnapsack();
//	if (knapsack->IsLock())
//	{
//		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
//		return;
//	}
//
//	if (INVALID_GUILD_ID == m_role->GetGuildID())
//	{
//		m_role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
//		return;
//	}
//
//	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
//	if (NULL == guild)
//	{
//		m_role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
//		return;
//	}
//
//	int need_gongxian = 0;
//	const int role_gongxian = m_role->GetGuildGongxian();
//
//	for (int i = 0; i <= MAX_GUILD_SKILL_LEVEL; i++)
//	{
//		bool up_finished = true;
//		for (int j = 0; j < GUILD_SKILL_COUNT; j++)
//		{
//			const GuildSkillCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetGuildConfig().GetSkillConfig(j, m_guild_param.skill_level_list[j]);
//			if (NULL == cfg_item)
//			{
//				continue;
//			}
//
//			const GuildSkillCfg::ConfigItem *next_cfg_item = LOGIC_CONFIG->GetGuildConfig().GetSkillConfig(j, m_guild_param.skill_level_list[j] + 1);
//			if (NULL == next_cfg_item)
//			{
//				continue;
//			}
//
//			if (guild->GetGuildLevel() < cfg_item->guild_level_limit)
//			{
//				m_role->NoticeNum(errornum::EN_XIANMENGZHAN_GUILD_LEVEL_LIMIT);
//				continue;
//			}
//
//			need_gongxian += cfg_item->uplevel_gongxian;
//			if (role_gongxian < need_gongxian)
//			{
//				need_gongxian -= cfg_item->uplevel_gongxian;
//				up_finished = true;
//				continue;
//			}
//
//			m_guild_param.skill_level_list[j]++;
//			up_finished = false;
//		}
//		if (up_finished)
//			break;
//	}
//
//	if (need_gongxian > 0)
//	{
//		m_role->AddGuildGongxian(-need_gongxian, "GuildSkillUplevel");
//	}
//
//	this->SendRoleGuildInfo();
//
//	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GUILD);
//}

void RoleGuild::OnSkillUplevelALLByLowestLevel()
{
	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (INVALID_GUILD_ID == m_role->GetGuildID())
	{
		m_role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL == guild)
	{
		m_role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	int need_gongxian = 0;
	const int role_gongxian = m_role->GetGuildGongxian();

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();
	
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	std::map<ItemID, short> consume_map;
	const int max_up_level_count = MAX_GUILD_SKILL_LEVEL * GUILD_SKILL_COUNT;
	for (int i = 0; i <= max_up_level_count; i++)
	{
		int lowest_level_index = 0;	//最低级的技能索引
		for (int k = 1; k < GUILD_SKILL_COUNT; k++)
		{
			if (m_guild_param.skill_level_list[lowest_level_index] > m_guild_param.skill_level_list[k])
			{
				lowest_level_index = k;
			}
		}
		//最低级的技能都满了，剩下的不用判断，直接break
		if (m_guild_param.skill_level_list[lowest_level_index] >= MAX_GUILD_SKILL_LEVEL)
		{
			break;
		}

		const GuildSkillCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetGuildConfig().GetSkillConfig(lowest_level_index, m_guild_param.skill_level_list[lowest_level_index]);
		if (NULL == cfg_item)
		{
			break;
		}

		const GuildSkillCfg::ConfigItem *next_cfg_item = LOGIC_CONFIG->GetGuildConfig().GetSkillConfig(lowest_level_index, m_guild_param.skill_level_list[lowest_level_index] + 1);
		if (NULL == next_cfg_item)
		{
			break;
		}

		if (guild->GetGuildLevel() < cfg_item->guild_level_limit)
		{
			m_role->NoticeNum(errornum::EN_XIANMENGZHAN_GUILD_LEVEL_LIMIT);
			break;
		}
		//判断贡献是否足够
		if (role_gongxian < need_gongxian + cfg_item->uplevel_gongxian)
		{
			break;
		}
		//由配置决定是否消耗物品
		if (nullptr != ITEMPOOL->GetItem(cfg_item->uplevel_stuff_id) && cfg_item->uplevel_stuff_count > 0)
		{
			short uplevel_stuff_num = m_role->GetKnapsack()->Count(cfg_item->uplevel_stuff_id);
			//背包物品数量不足
			if (uplevel_stuff_num < cfg_item->uplevel_stuff_count + consume_map[cfg_item->uplevel_stuff_id])
			{
				break;
			}
			//消耗列表已经满了，并且即将消耗新的物品时结束循环
			if (consume_map.size() == ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM && consume_map.find(cfg_item->uplevel_stuff_id) == consume_map.end())
			{
				break;
			}
			consume_map[cfg_item->uplevel_stuff_id] += cfg_item->uplevel_stuff_count;
		}
		
		need_gongxian += cfg_item->uplevel_gongxian;
		m_guild_param.skill_level_list[lowest_level_index]++;
	}

	if (need_gongxian > 0)
	{
		m_role->AddGuildGongxian(-need_gongxian, "GuildSkillUplevel");
	}
	int stuff_count = 0;
	for (auto &consume_temp : consume_map)
	{
		short uplevel_stuff_num = m_role->GetKnapsack()->Count(consume_temp.first);
		if (uplevel_stuff_num >= consume_temp.second && stuff_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
		{
			stuff_list[stuff_count].item_id = consume_temp.first;
			stuff_list[stuff_count].num = consume_temp.second;
			stuff_list[stuff_count].buyable = false;

			++stuff_count;
		}
	}
	if (stuff_count > 0 && !ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, false))
	{
		gamelog::g_log_guild.printf(LL_DEBUG, "[RoleGuild::OnSkillUplevelALLByLowestLevel] : !GetConsumeListByOrder [%d, %d, %s] -> [%d]", m_role->GetGuildID(),
			UidToInt(m_role->GetUserId()), m_role->GetName(), stuff_count);
	}

	// 道具扣除
	if (consume_list.count > 0 && !m_role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "roleguild::OnSkillUplevel"))
	{
		gamelog::g_log_guild.printf(LL_DEBUG, "[RoleGuild::OnSkillUplevelALLByLowestLevel] : !ConsumeItemByIndexList [%d, %d, %s] -> [%hd]", m_role->GetGuildID(),
			UidToInt(m_role->GetUserId()), m_role->GetName(), consume_list.count);
	}
	
	this->SendRoleGuildInfo();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GUILD);
}

void RoleGuild::OnLeaveGuild()
{
	m_guild_param.storage_score = 0;
	m_guild_param.last_leave_guild_time = (unsigned int)(EngineAdapter::Instance().Time());
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GUILD);
}

void RoleGuild::OnJoinGuild()
{
	this->SyncRedPaperInfoToGuild();
	m_guild_param.last_join_guild_time = (unsigned int)(EngineAdapter::Instance().Time());
}

unsigned int RoleGuild::GetLastLeaveGuildTime()
{
	return m_guild_param.last_leave_guild_time;
}

unsigned int RoleGuild::GetLastJoinGuildTime()
{
	return m_guild_param.last_join_guild_time;
}

bool RoleGuild::AddGuildRedPaperAuthority(int paper_type, int cond_param)
{
	if (paper_type <= GUILD_RED_PAPER_INVALID || paper_type >= GUILD_RED_PAPER_MAX) return false;

	int red_paper_seq = LOGIC_CONFIG->GetGuildConfig().GetGuildRedPaperSeq(paper_type, cond_param);
	if (red_paper_seq <= 0) return false;

	std::map<int, GuildRedPaperCfg> &red_paper_map =  LOGIC_CONFIG->GetGuildConfig().GetGuildRedPaperCfgMap();
	if (red_paper_map.find(red_paper_seq) == red_paper_map.end()) return false;

	bool is_in_guild = (INVALID_GUILD_ID != m_role->GetGuildID());

	int min_paper_gold = 0;
	int replace_index = -1;
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for(int i = 0; i < MAX_ROLE_GUILD_RED_PAPER; i++)
	{
		RoleGuildParam::GuildRedPaper &paper = m_guild_param.guild_red_paper_list[i];
		if (ROLE_RED_PAPER_INVALID == paper.red_paper_state)
		{
			replace_index = i;
			break;
		}
		else
		{
			std::map<int, GuildRedPaperCfg>::iterator iter = red_paper_map.find(paper.red_paper_seq);
			if (iter == red_paper_map.end() || (m_guild_param.guild_red_paper_list[i].invalid_time > 0  && now_sec > m_guild_param.guild_red_paper_list[i].invalid_time))
			{
				replace_index = i;
				break;
			}
			else
			{
				if (min_paper_gold < iter->second.bind_gold)
				{
					min_paper_gold = iter->second.bind_gold;
					replace_index = i;
				}
			}
		}
	}

	if (replace_index < 0 || replace_index >= MAX_ROLE_GUILD_RED_PAPER)
	{
		return false;
	}

	if (is_in_guild)
	{
		m_guild_param.guild_red_paper_list[replace_index].red_paper_seq = red_paper_seq;
		m_guild_param.guild_red_paper_list[replace_index].red_paper_state = ROLE_RED_PAPER_CAN_CREATE;
		m_guild_param.guild_red_paper_list[replace_index].invalid_time = now_sec + SECOND_PER_DAY;

		Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
		if (NULL != guild)
		{
			guild->AddGuildRedPaper(m_role, red_paper_seq, RED_PAPER_CAN_CREATE, 0, now_sec + SECOND_PER_DAY);
		}

		this->NoticeGuildRedPaperInfo(Protocol::SCNoticeGuildPaperInfo::HAS_CAN_CREATE_RED_PAPER);
	}
	else
	{
		m_guild_param.guild_red_paper_list[replace_index].red_paper_seq = red_paper_seq;
		m_guild_param.guild_red_paper_list[replace_index].red_paper_state = ROLE_RED_PAPER_HAS_AUTHORITY;
		m_guild_param.guild_red_paper_list[replace_index].invalid_time = 0;
	}

	return true;
}

void RoleGuild::CreateGuildRedPaper(int red_paper_seq, int can_fetch_time, int red_paper_index)
{
	if (red_paper_seq <= 0) return;

	if (INVALID_GUILD_ID == m_role->GetGuildID())
	{
		m_role->NoticeNum(errornum::EN_XIANMENGZHAN_NO_GUILD);
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL == guild)
	{
		m_role->NoticeNum(errornum::EN_XIANMENGZHAN_NO_GUILD);
		return;
	}

	if (can_fetch_time < 10 || can_fetch_time > guild->GetGuildMaxMemberCount())
	{
		m_role->NoticeNum(errornum::EN_GUILD_RED_PAPER_FETCH_LIMIT);
		return;
	}

	if (!guild->CanCreateGuildPaper(m_role, red_paper_index))
	{
		m_role->NoticeNum(errornum::EN_GUILD_RED_PAPER_CREATE_TIME_LIMIT);
		return;
	}

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	std::map<int, GuildRedPaperCfg> &red_paper_map =  LOGIC_CONFIG->GetGuildConfig().GetGuildRedPaperCfgMap();

	for (int i = 0; i < MAX_ROLE_GUILD_RED_PAPER; i++)
	{
		RoleGuildParam::GuildRedPaper &paper = m_guild_param.guild_red_paper_list[i];
		if (ROLE_RED_PAPER_CAN_CREATE != paper.red_paper_state || paper.red_paper_seq != red_paper_seq)
		{
			continue;
		}

		if(now_sec > paper.invalid_time || red_paper_map.find(red_paper_seq) == red_paper_map.end())
		{
			paper.red_paper_state = ROLE_RED_PAPER_INVALID;
			paper.red_paper_seq = 0;
			paper.invalid_time = 0;

			m_role->NoticeNum(errornum::EN_GUILD_RED_PAPER_CREATE_TIME_LIMIT);
			return;
		}
		else
		{
			GuildRedPaperCfg &paper_cfg = red_paper_map[red_paper_seq];

			if (paper_cfg.bind_gold < can_fetch_time)
			{
				can_fetch_time = paper_cfg.bind_gold;
			}

			int red_paper_id = RedPaperManager::Instance().OnCreateRedPaper(m_role, RED_PAPER_TYPE_GUILD_PAPER, paper_cfg.bind_gold, can_fetch_time, true, 1);
			if (0 != red_paper_id)
			{
				paper.red_paper_state = ROLE_RED_PAPER_INVALID;
				paper.red_paper_seq = 0;
				paper.invalid_time = 0;
				
				Protocol::SCNoticeGuildPaperInfo cmd;
				cmd.notice_reson = Protocol::SCNoticeGuildPaperInfo::HAS_CAN_FETCH_RED_PAPER;
				guild->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_REP_PAPER, (const char *)&cmd, sizeof(cmd));
				
				guild->ClearRedPaperByIndex(m_role, red_paper_index);
				guild->AddGuildRedPaper(m_role, red_paper_seq, RED_PAPER_CAN_FETCH, red_paper_id, now_sec + SECOND_PER_DAY);
				GuildManager::Instance().SendGuildRedPaperlistInfo(m_role);
			}

			return;
		}
	}
}

void RoleGuild::SyncRedPaperInfoToGuild()
{
	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL == guild) return;

	std::map<int, GuildRedPaperCfg> &red_paper_map =  LOGIC_CONFIG->GetGuildConfig().GetGuildRedPaperCfgMap();
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	for(int i = 0; i < MAX_ROLE_GUILD_RED_PAPER; i++)
	{
		RoleGuildParam::GuildRedPaper &paper = m_guild_param.guild_red_paper_list[i];
		if (red_paper_map.find(paper.red_paper_seq) == red_paper_map.end())
		{
			paper.red_paper_state = ROLE_RED_PAPER_INVALID;
			paper.red_paper_seq = 0;
			paper.invalid_time = 0;
			continue;
		}

		if (ROLE_RED_PAPER_INVALID == paper.red_paper_state)
		{
			continue;
		}
		else if (ROLE_RED_PAPER_HAS_AUTHORITY == paper.red_paper_state)
		{
			paper.red_paper_state = ROLE_RED_PAPER_CAN_CREATE;
			paper.invalid_time = now_sec + SECOND_PER_DAY;
			guild->AddGuildRedPaper(m_role, paper.red_paper_seq, RED_PAPER_CAN_CREATE, 0, paper.invalid_time);
		}
		else if (ROLE_RED_PAPER_CAN_CREATE == paper.red_paper_state)
		{
			if (now_sec < paper.invalid_time)
			{
				guild->AddGuildRedPaper(m_role, paper.red_paper_seq, RED_PAPER_CAN_CREATE, 0, paper.invalid_time);
			}
			else
			{
				paper.red_paper_state = ROLE_RED_PAPER_INVALID;
				paper.invalid_time = 0;
				paper.red_paper_seq = 0;
			}
		}
	}

	guild->AddSyncRedPaperMember(m_role->GetUID());
}

bool RoleGuild::IsCanJoinTianCiTongBi()
{
	if (m_guild_param.join_tiancitongbi_guild_id != 0 && m_role->GetGuildID() != m_guild_param.join_tiancitongbi_guild_id)
	{
		return false;
	}

	return true;
}

void RoleGuild::AddTodayJuanxianGold(int gold)
{
	if (gold <= 0) return;

	m_guild_param.today_juanxian_gold += gold;

	this->SendRoleGuildInfo();
}

void RoleGuild::ResetTodayGuildPaoSaiziTimes()
{
	m_guild_param.today_paosaizi_times = 0;

	Guild* guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL == guild) return;

	guild->GetGuildMemberManager().SyncSCGulidSaiziInfo(m_role);
}

void RoleGuild::OpenServerSyncRedPapaerInfo()
{
	Guild* guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL == guild) return;

	if (!guild->IsRoleSyncRedPaperInfo(m_role->GetUID()))
	{
		this->SyncRedPaperInfoToGuild();
	}
}

void RoleGuild::OnLoginNoticeGuildRedPaperInfo()
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	for (int i = 0; i < MAX_ROLE_GUILD_RED_PAPER; i++)
	{
		RoleGuildParam::GuildRedPaper &paper = m_guild_param.guild_red_paper_list[i];
		if (ROLE_RED_PAPER_CAN_CREATE != paper.red_paper_state)
		{
			continue;
		}

		if(now_sec > paper.invalid_time)
		{
			paper.red_paper_state = ROLE_RED_PAPER_INVALID;
			paper.red_paper_seq = 0;
			paper.invalid_time = 0;
			continue;
		}

		this->NoticeGuildRedPaperInfo(Protocol::SCNoticeGuildPaperInfo::HAS_CAN_CREATE_RED_PAPER);
		break;
	}
}

void RoleGuild::NoticeGuildRedPaperInfo(int notice_reason)
{
	if (notice_reason < Protocol::SCNoticeGuildPaperInfo::HAS_CAN_CREATE_RED_PAPER ||
		notice_reason > Protocol::SCNoticeGuildPaperInfo::MAX_NOTICE_REASON)
	{
		return;
	}

	Protocol::SCNoticeGuildPaperInfo cmd;
	cmd.notice_reson = notice_reason;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}
