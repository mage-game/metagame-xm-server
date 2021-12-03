#include "offlineregister.hpp"

#include "other/cmdhandler/cmdhandler.hpp"
#include "servercommon/servercommon.h"
#include "config/touzijihuaconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/chongzhiconfig.hpp"
#include "obj/character/role.h"
#include "item/knapsack.h"
#include "systemcmddef.h"

OfflineRegister::OfflineRegister()
	: m_id_offline_register(0), m_user_id(INVALID_USER_ID), m_increase_charm(0), m_last_divorce_time(0), m_chongzhi_cmd_flag(0), m_red_paper_consume_gold(0), m_red_paper_lucky_value(0),
	m_space_cai_count(0), m_space_gift_count(0), m_qingyuan_bless_days(0)
{
}

OfflineRegister::OfflineRegister(const UserID &user_id)
	: m_id_offline_register(0), m_user_id(user_id), m_increase_charm(0), m_last_divorce_time(0), m_chongzhi_cmd_flag(0), m_red_paper_consume_gold(0), m_red_paper_lucky_value(0),
	m_space_cai_count(0), m_space_gift_count(0), m_qingyuan_bless_days(0)
{
}

OfflineRegister::~OfflineRegister()
{

}

void OfflineRegister::Init(const OfflineRegisterParam::OfflineRegisterAttr &attr)
{
	m_id_offline_register = attr.id_offline_register;
	m_user_id = IntToUid(attr.role_id);
	m_increase_charm = attr.increase_charm;
	m_last_divorce_time = attr.last_divorce_time;
	m_chongzhi_cmd_flag = attr.chongzhi_cmd_flag;
	m_red_paper_consume_gold = attr.red_paper_consume_gold;
	m_red_paper_lucky_value = attr.red_paper_lucky_value;
	m_space_cai_count = attr.space_cai_count;
	m_space_gift_count = attr.space_gift_count;
	m_qingyuan_bless_days = attr.qingyuan_bless_days;
	m_guild_box_info = attr.guild_box_info;
	m_common_param = attr.common_param;
}

void OfflineRegister::GetAttr(OfflineRegisterParam::OfflineRegisterAttr *attr)
{
	attr->role_id = UidToInt(m_user_id);
	attr->increase_charm = m_increase_charm;
	attr->last_divorce_time = m_last_divorce_time;
	attr->chongzhi_cmd_flag = m_chongzhi_cmd_flag;
	attr->red_paper_consume_gold = m_red_paper_consume_gold;
	attr->red_paper_lucky_value = m_red_paper_lucky_value;
	attr->space_cai_count = m_space_cai_count;
	attr->space_gift_count = m_space_gift_count;
	attr->qingyuan_bless_days = m_qingyuan_bless_days;
	attr->guild_box_info = m_guild_box_info;
	attr->common_param = m_common_param;
}

void OfflineRegister::OnChongzhiCmd(int chongzhi_num)
{
	UNSTD_STATIC_CHECK(CHONGZHI_REWARD_CONFIG_MAX_NUM <= (int)sizeof(m_chongzhi_cmd_flag) * 8);

	const ChongzhiReward *reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardByNum(chongzhi_num);
	if (NULL != reward_cfg)
	{
		m_chongzhi_cmd_flag |= (1 << reward_cfg->seq);
	}
}

void OfflineRegister::OnSetConsumeItem(unsigned short item_id, unsigned short item_num)
{
	m_common_param.item_id = item_id;
	m_common_param.item_num = item_num;
}

bool OfflineRegister::ConsumeItem(Role* role)
{
	if (!role)
	{
		return false;
	}

	if (m_common_param.item_id == 0 || m_common_param.item_num == 0)
	{
		return false;
	}

	ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	ItemExtern::ItemConsumeConfig stuff(m_common_param.item_id, false, m_common_param.item_num);

	if (ItemExtern::GetConsumeListByOrder(role, false, 1, &stuff, &consume_list))
	{
		if (consume_list.count > 0)
		{
			role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "system::cmdtorole");
		}
	}
	m_common_param.item_id = 0;
	m_common_param.item_num = 0;
	return true;
}

void OfflineRegister::OnSetConsumeGold(int num)
{
	m_common_param.change_gold = num;
}

bool OfflineRegister::ConsumeGold(Role* role)
{
	if (!role)
	{
		return false;
	}
	bool ret = false;
	if (m_common_param.change_bind_gold != 0)
	{
		CmdHandler::Instance().OnCmdToRole(role, systemcmd::EFFECT_USE_GOLD_BIND, m_common_param.change_bind_gold, 0);
		m_common_param.change_bind_gold = 0;
		ret = true;
	}

	if (m_common_param.change_gold != 0)
	{
		CmdHandler::Instance().OnCmdToRole(role, systemcmd::EFFECT_USE_GOLD, m_common_param.change_gold, 0);
		m_common_param.change_gold = 0;
		ret = true;
	}
	return ret;
}

void OfflineRegister::OnSetConsumeBindGold(int num)
{
	m_common_param.change_bind_gold = num;
}

bool OfflineRegister::CanRemove(unsigned int now_second)
{
	if (0 != m_increase_charm || 0 != m_last_divorce_time || 0 != m_chongzhi_cmd_flag || 0 != m_red_paper_consume_gold || 0 != m_red_paper_lucky_value || 
		0 != m_space_cai_count || 0 != m_space_gift_count || 0 != m_qingyuan_bless_days || 0 != m_guild_box_info.dayid || !m_common_param.CanRemove()/* || 0 != m_profess_param.profess_count*/)
	{
		return false;
	}

	return true;
}

// void OfflineRegister::OnProfessFrom(Role* profess_role, PersonProfessItem& item)
// {
// 	// 当前要覆盖的表白索引
// 	short current_index = m_profess_param.earliest_profess_index;
// 	if (current_index < 0 || current_index >= PERSON_PROFESS_MAX_NUM)
// 		return;
// 
// 	// 覆盖最早表白的数据
// 	PersonProfessItem& offline_profess_item = m_profess_param.profess_list[current_index];
// 	offline_profess_item.other_role_id = UidToInt(profess_role->GetUserId());
// 	offline_profess_item.profess_time = item.profess_time;
// 	memcpy(&offline_profess_item.content, &item.content, sizeof(PersonProfessItem));
// 
// 	// 记录数量
// 	if (m_profess_param.profess_count < PERSON_PROFESS_MAX_NUM)
// 	{
// 		m_profess_param.profess_count++;
// 	}
// 
// 	// 指向下一个告白（最早的）
// 	m_profess_param.earliest_profess_index++;
// 	m_profess_param.earliest_profess_index %= PERSON_PROFESS_MAX_NUM;
// }
// 
// PersonProfessItem* OfflineRegister::GetProfessByOther()
// {
// 	if (m_profess_param.profess_count > 0)
// 		return m_profess_param.profess_list;
// 	return nullptr;
// }
// 
// void OfflineRegister::ClearProfess()
// {
// 	m_profess_param.Reset();
// }

