#include "tianshenhuti.hpp"

#include <algorithm>
#include <functional>

#include "servercommon/errornum.h"
#include "servercommon/servercommon.h"
#include "servercommon/string/gameworldstr.h"

#include "config/logicconfigmanager.hpp"
#include "tianshenhuticonfig.hpp"

#include "protocal/msgtianshenhuti.h"
#include "protocal/msgfight.h"

#include "world.h"
#include "item/knapsack.h"
#include "skill/skillbase.hpp"
#include "obj/character/role.h"
#include "effect/skilleffect/effectinvisible.hpp"
#include "other/capability/capability.hpp"



static const int COMBINE_EQUIP_NUM = 3;		// 合成一个装备需要的装备数量
static const int MAX_COMBINE_TIMES = TIANSHENHUTI_BACKPACK_MAX_COUNT / COMBINE_EQUIP_NUM;

TianshenHuti::TianshenHuti() : m_backpack_num(0)
{
}

TianshenHuti::~TianshenHuti()
{

}

void TianshenHuti::OnInit()
{
	m_backpack_num = 0;
	for (int index = 0; index < TIANSHENHUTI_BACKPACK_MAX_COUNT; ++ index)
	{
		if (0 == m_param.Param().backpack_equip_list[index]) 
		{
			break;
		}
	
		++ m_backpack_num;
	}

	this->UpdateTaozhuangNum(true);
}

void TianshenHuti::ReCalcAttr(CharIntAttrs &base_add, int recalc_reason, bool recalc_all)
{
	if (CharIntAttrs::RECALC_REASON_TIANSHENHUTI == recalc_reason || recalc_all)
	{
		m_attrs_add.Reset();

		std::map<int, int> taozhuang_num_map;

		for (int inuse_equip_index = 0; inuse_equip_index < TIANSHENHUTI_EQUIP_MAX_COUNT; ++ inuse_equip_index)
		{
			const TianshenHutiEquipConfig *equip_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetEquipConfigWithID(m_param.Param().inuse_equip_list[inuse_equip_index]);
			if (nullptr != equip_cfg)
			{
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, equip_cfg->maxhp);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, equip_cfg->gongji);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, equip_cfg->fangyu);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, equip_cfg->baoji);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, equip_cfg->jianren);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, equip_cfg->mingzhong);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, equip_cfg->shanbi);

				if (taozhuang_num_map.find(equip_cfg->level_taozhuang_type) != taozhuang_num_map.end())
				{
					++ taozhuang_num_map[equip_cfg->level_taozhuang_type];
				}
				else
				{
					taozhuang_num_map[equip_cfg->level_taozhuang_type] = 1;
				}
			}
		}

		for (std::map<int, int>::iterator num_it = taozhuang_num_map.begin(); num_it != taozhuang_num_map.end(); ++ num_it)
		{
			// 小飞要求 属性叠加技术处理 策划不处理
			for (int taozhuang_num_tmp = 1; taozhuang_num_tmp <= num_it->second; ++taozhuang_num_tmp)
			{
				const TianshenHutiTaozhuangConfig *taozhuang_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetTaozhuangConfig(num_it->first, taozhuang_num_tmp);
				if (nullptr != taozhuang_cfg)
				{
					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, taozhuang_cfg->maxhp);
					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, taozhuang_cfg->gongji);
					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, taozhuang_cfg->fangyu);
					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, taozhuang_cfg->baoji);
					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, taozhuang_cfg->jianren);
					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, taozhuang_cfg->mingzhong);
					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, taozhuang_cfg->shanbi);

					m_attrs_add.AddPercentValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, static_cast<float>(taozhuang_cfg->per_gongji * ROLE_ATTR_PER));
					m_attrs_add.AddPercentValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, static_cast<float>(taozhuang_cfg->per_fangyu* ROLE_ATTR_PER));
					m_attrs_add.AddPercentValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, static_cast<float>(taozhuang_cfg->per_maxhp* ROLE_ATTR_PER));
				}
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_TIANSHENHUTI, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

void TianshenHuti::OnRoleLogin()
{
}

void TianshenHuti::PutOnEquip(int backpack_slot_index)
{
	if (backpack_slot_index < 0 || backpack_slot_index >= TIANSHENHUTI_BACKPACK_MAX_COUNT || backpack_slot_index >= m_backpack_num)
	{
		return;
	}

	TianshenEquipID puton_id = m_param.Param().backpack_equip_list[backpack_slot_index];
	if (INVALID_TIANSHEN_EQUIP_ID == puton_id)
	{
		m_role->NoticeNum(errornum::EN_TIANSHENHUTI_SLOT_EMPTY);
		return;
	}

	const TianshenHutiEquipConfig *equip_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetEquipConfigWithID(puton_id);
	if (nullptr == equip_cfg || equip_cfg->slot_index < 0 || equip_cfg->slot_index >= TIANSHENHUTI_EQUIP_MAX_COUNT)
	{
		return;
	}

	TianshenEquipID inuse_equip_id = m_param.Param().inuse_equip_list[equip_cfg->slot_index];
	if (INVALID_TIANSHEN_EQUIP_ID != inuse_equip_id) // 原来部位有装备，先放进背包
	{
		if (this->PutEuipToBackpack(inuse_equip_id) < 0)
		{
			return;
		}
	}

	this->RemoveEquipFromBackpack(backpack_slot_index);
	m_param.Param().inuse_equip_list[equip_cfg->slot_index] = puton_id;

	this->UpdateTaozhuangNum();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TIANSHENHUTI);
	this->SendAllInfoToRole();
}

void TianshenHuti::TakeOffEquip(int slot_index)
{
	if (slot_index < 0 || slot_index >= TIANSHENHUTI_EQUIP_MAX_COUNT)
	{
		return;
	}

	TianshenEquipID takeoff_id = m_param.Param().inuse_equip_list[slot_index];
	if (0 == takeoff_id)
	{
		m_role->NoticeNum(errornum::EN_TIANSHENHUTI_SLOT_EMPTY);
		return;
	}

	if (this->PutEuipToBackpack(takeoff_id) >= 0)
	{
		m_param.Param().inuse_equip_list[slot_index] = 0;

		this->UpdateTaozhuangNum();
		this->SendAllInfoToRole();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TIANSHENHUTI);
	}
}

void TianshenHuti::TransformEquip(int slot_index1, int slot_index2, int new_equip_slot)
{
	if (slot_index1 < 0 || slot_index1 >= TIANSHENHUTI_BACKPACK_MAX_COUNT || 
		slot_index2 < 0 || slot_index2 >= TIANSHENHUTI_BACKPACK_MAX_COUNT ||
		new_equip_slot < 0 || new_equip_slot >= TIANSHENHUTI_EQUIP_MAX_COUNT)
	{
		return;
	}

	TianshenEquipID equip_id1 = m_param.Param().backpack_equip_list[slot_index1];
	TianshenEquipID equip_id2 = m_param.Param().backpack_equip_list[slot_index2];
	const TianshenHutiEquipConfig *equip1_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetEquipConfigWithID(equip_id1);
	const TianshenHutiEquipConfig *equip2_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetEquipConfigWithID(equip_id2);

	if (nullptr == equip1_cfg || nullptr == equip2_cfg)
	{
		m_role->NoticeNum(errornum::EN_TIANSHENHUTI_SLOT_EMPTY);
		return;
	}

	if (equip1_cfg->level != equip2_cfg->level)
	{
		m_role->NoticeNum(errornum::EN_TIANSHENHUTI_EQUIP_LEVEL_DIFFERENT);
		return;
	}

	const TianshenHutiEquipConfig *new_equip_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetRandomEquipConfigWithLevel(equip1_cfg->level, new_equip_slot);
	if (nullptr == new_equip_cfg)
	{
		return;
	}

	{
		// 消耗物品
		std::vector<int> tmp_index_list;
		tmp_index_list.push_back(slot_index1);
		tmp_index_list.push_back(slot_index2);
		std::sort(tmp_index_list.begin(), tmp_index_list.end(), std::greater<int>());

		// 先删后面的物品，再删前面的物品，保证先删不会影响后删的下标有效性
		for (std::vector<int>::iterator index_it = tmp_index_list.begin(); index_it != tmp_index_list.end(); ++ index_it)
		{
			this->RemoveEquipFromBackpack(*index_it);
		}
	}

	this->PutEuipToBackpack(new_equip_cfg->equip_id);

	this->SendAllInfoToRole();

	Protocol::SCTianshenhutiReqResult tsrr;
	tsrr.req_type = Protocol::CS_TIANSHENHUTI_REQ_TYPE_TRANSFORM;
	tsrr.new_equip_id = new_equip_cfg->equip_id;
	tsrr.param_1 = 0;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tsrr, sizeof(tsrr));

	gamelog::g_log_tianshenhuti.printf(LL_INFO, "TransformEquip user %d %s, equip1: id %d slot %d, equip2: id %d slot %d, new_equip: id %d, slot %d",
		m_role->GetUID(), m_role->GetName(), equip_id1, slot_index1, equip_id2, slot_index2, new_equip_cfg->equip_id, new_equip_slot);
}

void TianshenHuti::CombineEquip(int slot_index1, int slot_index2, int slot_index3, int client_param)
{
	if (slot_index1 < 0 || slot_index1 >= TIANSHENHUTI_BACKPACK_MAX_COUNT 
		|| slot_index2 < 0 || slot_index2 >= TIANSHENHUTI_BACKPACK_MAX_COUNT 
		|| slot_index3 < 0 || slot_index3 >= TIANSHENHUTI_BACKPACK_MAX_COUNT)
	{
		return;
	}

	int new_equip_id = 0;

	if (this->CombioneOneEquip(slot_index1, slot_index2, slot_index3, &new_equip_id))
	{
		Protocol::SCTianshenhutiReqResult tsrr;
		tsrr.req_type = Protocol::CS_TIANSHENHUTI_REQ_TYPE_COMBINE;
		tsrr.new_equip_id = new_equip_id;
		tsrr.param_1 = client_param;
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tsrr, sizeof(tsrr));

		this->SendAllInfoToRole();
	}
}

bool TianshenHuti::CombioneOneEquip(int slot_index1, int slot_index2, int slot_index3, int *new_equip_id)
{
	if (slot_index1 < 0 || slot_index1 >= TIANSHENHUTI_BACKPACK_MAX_COUNT || 
		slot_index2 < 0 || slot_index2 >= TIANSHENHUTI_BACKPACK_MAX_COUNT ||
		slot_index3 < 0 || slot_index3 >= TIANSHENHUTI_BACKPACK_MAX_COUNT || 
		nullptr == new_equip_id)
	{
		return false;
	}

	TianshenEquipID equip_id1 = m_param.Param().backpack_equip_list[slot_index1];
	TianshenEquipID equip_id2 = m_param.Param().backpack_equip_list[slot_index2];
	TianshenEquipID equip_id3 = m_param.Param().backpack_equip_list[slot_index3];
	const TianshenHutiEquipConfig *equip1_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetEquipConfigWithID(equip_id1);
	const TianshenHutiEquipConfig *equip2_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetEquipConfigWithID(equip_id2);
	const TianshenHutiEquipConfig *equip3_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetEquipConfigWithID(equip_id3);

	if (nullptr == equip1_cfg || nullptr == equip2_cfg || nullptr == equip3_cfg)
	{
		m_role->NoticeNum(errornum::EN_TIANSHENHUTI_SLOT_EMPTY);
		return false;
	}

	if ((equip1_cfg->level != equip2_cfg->level) || (equip2_cfg->level != equip3_cfg->level) || (equip1_cfg->level != equip3_cfg->level))
	{
		m_role->NoticeNum(errornum::EN_TIANSHENHUTI_EQUIP_LEVEL_DIFFERENT);
		return false;
	}

	if (equip1_cfg->level >= TIANSHENHUTI_MAX_EQUIP_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_TIANSHENHUTI_EQUIP_ALEADY_MAX_LEVEL);
		return false;
	}

	const TianshenHutiEquipConfig *new_equip_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetRandomEquipConfigWithLevel(equip1_cfg->level + 1);
	if (nullptr == new_equip_cfg)
	{
		return false;
	}

	{
		// 消耗物品
		std::vector<int> tmp_index_list;
		tmp_index_list.push_back(slot_index1);
		tmp_index_list.push_back(slot_index2);
		tmp_index_list.push_back(slot_index3);
		std::sort(tmp_index_list.begin(), tmp_index_list.end(), std::greater<int>());

		// 先删后面的物品，再删前面的物品，保证先删不会影响后删的下标有效性
		for (std::vector<int>::iterator index_it = tmp_index_list.begin(); index_it != tmp_index_list.end(); ++index_it)
		{
			this->RemoveEquipFromBackpack(*index_it);
		}
	}

	int new_equip_slot = this->PutEuipToBackpack(new_equip_cfg->equip_id);

	if (new_equip_cfg->level >= 6)
	{
		// 发传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_tianshenhuti_combine_notice,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), new_equip_cfg->equip_id);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	gamelog::g_log_tianshenhuti.printf(LL_INFO, "CombineEquip user %d %s, equip1: id %d slot %d, equip2: id %d slot %d, equip3: id %d slot %d, new_equip: id %d, slot %d",
		m_role->GetUID(), m_role->GetName(), equip_id1, slot_index1, equip_id2, slot_index2, equip_id3, slot_index3, new_equip_cfg->equip_id, new_equip_slot);

	*new_equip_id = new_equip_cfg->equip_id;

	return true;
}

void TianshenHuti::CombineEquipOneKey(int target_level)
{
	// 至少合成2阶装备
	if (target_level <= 1 || target_level > TIANSHENHUTI_MAX_EQUIP_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return;
	}

	static Protocol::SCTianshenhutiCombineOneKeyResult msg;
	msg.combine_count = 0;

	int src_level = target_level - 1;
	int combine_slot_index[COMBINE_EQUIP_NUM] = { 0 };
	int slot_count = 0;
	int combine_times = 0;
	for (int slot_idx = 0; slot_idx < TIANSHENHUTI_BACKPACK_MAX_COUNT && slot_idx < m_backpack_num && combine_times < MAX_COMBINE_TIMES; ++slot_idx)
	{
		if (0 != m_param.Param().backpack_equip_list[slot_idx])
		{
			TianshenEquipID equip_id = m_param.Param().backpack_equip_list[slot_idx];
			const TianshenHutiEquipConfig *equip1_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetEquipConfigWithID(equip_id);
			if (nullptr != equip1_cfg)
			{
				if (equip1_cfg->level == src_level) // 低一级的（COMBINE_EQUIP_NUM）个装备合成一个高一级的装备
				{
					combine_slot_index[slot_count++] = slot_idx;
				}
				if (slot_count >= COMBINE_EQUIP_NUM)
				{
					int new_equip_id = 0;
					if (this->CombioneOneEquip(combine_slot_index[0], combine_slot_index[1], combine_slot_index[2], &new_equip_id))
					{
						msg.new_equip_id[msg.combine_count++] = new_equip_id;
						// 背包的排序比较怪，消耗物品的时候把最后面的挪到消耗的位置，然后 --m_backpack_num 
						// 所以每合成一件 slot_idx都要从0开始
						slot_idx = 0;
						++combine_times;
					}
					memset(combine_slot_index, 0, sizeof(combine_slot_index));
					slot_count = 0;
				}
			}
		}
	}

	SEND_TO_ROLE(m_role, msg);

	this->SendAllInfoToRole();
}

int TianshenHuti::GetBackpackEmptyGridNum()
{
	return TIANSHENHUTI_BACKPACK_MAX_COUNT - m_backpack_num;
}

int TianshenHuti::PutEuipToBackpack(TianshenEquipID equip_id)
{
	if (0 == this->GetBackpackEmptyGridNum())
	{
		m_role->NoticeNum(errornum::EN_TIANSHENHUTI_BACKPACK_FULL);
		return -1;
	}

	int insert_idx = m_backpack_num;

	if (insert_idx < 0 || insert_idx >= TIANSHENHUTI_BACKPACK_MAX_COUNT)
	{
		return -1;
	}
	else
	{
		m_param.Param().backpack_equip_list[insert_idx] = equip_id;
		++m_backpack_num;

		return insert_idx;
	}
}

TianshenEquipID TianshenHuti::GetBackpackEquip(int slot_index)
{
	if (slot_index < 0 || slot_index >= TIANSHENHUTI_BACKPACK_MAX_COUNT)
	{
		return 0;
	}

	return m_param.Param().backpack_equip_list[slot_index];
}

void TianshenHuti::RollReq(int roll_type)
{
	if (roll_type <= TIANSHENHUTI_ROLL_TYPE_INVALID || roll_type >= TIANSHENHUTI_ROLL_TYPE_MAX)
	{
		return;
	}

	//const tm *local_time = EngineAdapter::Instance().LocalTime();
	//if (nullptr == local_time)
	//{
	//	return;
	//}

	//if (SATURDAY != local_time->tm_wday && 0 != local_time->tm_wday)
	//{
	//	m_role->NoticeNum(errornum::EN_TIANSHENHUTI_ROLL_NOT_OPEN);
	//	return;
	//}

	const TianshenhutiOtherConfig & other_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetOtherConfig();

	int roll_times = 0, need_gold = 0, need_score = 0, add_accumulate_times = 0; 
	ItemID need_item_id = 0;
	int need_item_num = 0;

	switch (roll_type)
	{
	case TIANSHENHUTI_ROLL_TYPE_COMMON:
		{
			roll_times = 1;
			need_score = other_cfg.common_roll_cost;
			need_item_id = other_cfg.common_roll_item_id;
			need_item_num = other_cfg.common_roll_item_num;
		}
		break;

	case TIANSHENHUTI_ROLL_TYPE_SUPER:
		{
			roll_times = 1;
			add_accumulate_times = 1;
			need_gold = other_cfg.super_roll_cost;
			need_item_id = other_cfg.super_roll_item_id;
			need_item_num = other_cfg.super_roll_item_num;
		}
		break;

	case TIANSHENHUTI_ROLL_TYPE_BATCH:
		{
			roll_times = Protocol::TIANSHENHUTI_BATCH_ROLL_TIMES;
			add_accumulate_times = Protocol::TIANSHENHUTI_BATCH_ROLL_TIMES;
			need_gold = other_cfg.batch_roll_cost;
			need_item_id = other_cfg.batch_roll_item_id;
			need_item_num = other_cfg.batch_roll_item_num;
		}
		break;
	}
	
	// 屏蔽折扣功能
	//TianshenHutiDiscountConfig *discount_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetDiscountCfg();
	//if (need_gold > 0 && nullptr != discount_cfg)
	//{
	//	need_gold = static_cast<int>(ceil(need_gold * 0.01f * discount_cfg->discount_percent));
	//
	//	need_item_num = static_cast<int>(ceil(need_item_num * 0.01f * discount_cfg->discount_percent));
	//}

	if (roll_times <= 0 || (need_score <= 0 && need_gold <= 0))
	{
		return;
	}

	if (this->GetBackpackEmptyGridNum() < roll_times)
	{
		m_role->NoticeNum(errornum::EN_TIANSHENHUTI_BACKPACK_NO_SAPCE);
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	bool use_free_chance = false;
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (TIANSHENHUTI_ROLL_TYPE_COMMON == roll_type)
	{
		short free_times = LOGIC_CONFIG->GetTianshenHutiConfig().GetFreeTimes();
		if (free_times > m_param.Param().used_free_times && now > m_param.Param().next_free_roll_time)
		{
			use_free_chance = true;
			need_gold = 0;
			need_score = 0;
		}
	}

	if (!use_free_chance)
	{
		if (need_gold <= 0 && need_score <= 0)
		{
			return;
		}
	
		if (m_role->GetKnapsack()->GetItemNum(need_item_id) < need_item_num)
		{
			if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}

			if (m_param.Param().roll_score < need_score)
			{
				m_role->NoticeNum(errornum::EN_TIANSHENHUTI_SCORE_NOT_ENOUGH);
				return;
			}
		}
	}

	UNSTD_STATIC_CHECK(TIANSHENHUTI_ROLL_REWARD_EQUIP_MIN_LEVEL >= 1);
	UNSTD_STATIC_CHECK(TIANSHENHUTI_ROLL_REWARD_EQUIP_MIN_LEVEL <= TIANSHENHUTI_ROLL_REWARD_EQUIP_MAX_LEVEL);
	UNSTD_STATIC_CHECK(TIANSHENHUTI_ROLL_REWARD_EQUIP_MAX_LEVEL <= TIANSHENHUTI_MAX_EQUIP_LEVEL);

	std::set<int> allow_level_set;  // 抽取有等级限制，只能抽取等级在[min，max]之间的装备
	for (int i = TIANSHENHUTI_ROLL_REWARD_EQUIP_MIN_LEVEL; i <= TIANSHENHUTI_ROLL_REWARD_EQUIP_MAX_LEVEL; ++ i)
	{
		allow_level_set.insert(i);
	}

	int reward_equip_count = 0;
	TianshenEquipID reward_equip_list[Protocol::TIANSHENHUTI_BATCH_ROLL_TIMES]; 
	memset(reward_equip_list, 0, sizeof(reward_equip_list));

	if (TIANSHENHUTI_ROLL_TYPE_BATCH != roll_type)
	{
		TianshenEquipID equip_id = LOGIC_CONFIG->GetTianshenHutiConfig().RollEquip(roll_type, allow_level_set);
		if (0 == equip_id)
		{
			return;
		}

		reward_equip_list[0] = equip_id;
		reward_equip_count = 1;
	}
	else
	{
		// 批量抽取
		std::set<int> four_level_set;
		four_level_set.insert(TIANSHENHUTI_ROLL_BATCH_FIX_EQUIP_LEVEL);  // 必出的等级

		TianshenEquipID four_level_equip_id = LOGIC_CONFIG->GetTianshenHutiConfig().RollEquip(roll_type, four_level_set); // 保证抽到一个X级的装备
		if (0 == four_level_equip_id)
		{
			return;
		}

		reward_equip_list[0] = four_level_equip_id;
		reward_equip_count = 1;
		-- roll_times;

		for (int i = 0; i < roll_times && reward_equip_count < Protocol::TIANSHENHUTI_BATCH_ROLL_TIMES; ++ i)
		{
			TianshenEquipID equip_id = LOGIC_CONFIG->GetTianshenHutiConfig().RollEquip(roll_type, allow_level_set); // 保证抽到一个X级的装备
			if (0 == equip_id)
			{
				return;
			}

			reward_equip_list[reward_equip_count] = equip_id;
			++ reward_equip_count;

			const TianshenHutiEquipConfig *equip_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetEquipConfigWithID(equip_id);
			if (nullptr == equip_cfg)
			{
				return;
			}

			if (equip_cfg->level >= TIANSHENHUTI_ROLL_BATCH_FIX_EQUIP_LEVEL) // 若再抽到一个X级的装备，那么之后不能抽到X级或以上的装备
			{
				UNSTD_STATIC_CHECK(TIANSHENHUTI_ROLL_REWARD_EQUIP_MAX_LEVEL >= TIANSHENHUTI_ROLL_BATCH_FIX_EQUIP_LEVEL);
				UNSTD_STATIC_CHECK(TIANSHENHUTI_ROLL_BATCH_FIX_EQUIP_LEVEL >= TIANSHENHUTI_ROLL_REWARD_EQUIP_MIN_LEVEL);

				for (int i = TIANSHENHUTI_ROLL_REWARD_EQUIP_MAX_LEVEL; i >= TIANSHENHUTI_ROLL_BATCH_FIX_EQUIP_LEVEL; -- i)
				{
					allow_level_set.erase(i);
				}
			}
		}
	}

	if (!use_free_chance)
	{
		bool consume_succ = false;
		if (need_item_num > 0 && need_item_id > 0)
		{
			if (m_role->GetKnapsack()->ConsumeItem(need_item_id, need_item_num, "TianshenHutiRollEquip"))
			{
				consume_succ = true;
			}
		}

		if (!consume_succ)
		{
			if (need_gold > 0)
			{
				if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "TianshenHutiRollEquip"))
				{
					return;
				}
			}
			if (need_score > 0)
			{
				if (m_param.Param().roll_score < need_score)
				{
					return;
				}
				m_param.Param().roll_score -= need_score;
			}
		}
	}
	else
	{
		++m_param.Param().used_free_times;
		m_param.Param().next_free_roll_time = now + LOGIC_CONFIG->GetTianshenHutiConfig().GetFreeCD();
	}

	gamelog::g_log_tianshenhuti.printf(LL_INFO, "RoleReq user %d %s, role_type %d, use(gold %d or item %d,%d), use_free_chance %d, free_times %d, reward_equip_count %d", 
		m_role->GetUID(), m_role->GetName(), roll_type, need_gold, (int)need_item_id, need_item_num, (use_free_chance ? 1 : 0), m_param.Param().used_free_times, reward_equip_count);

	for (int i = 0; i < reward_equip_count && i < Protocol::TIANSHENHUTI_BATCH_ROLL_TIMES; ++ i)
	{
		int insert_slot = this->PutEuipToBackpack(reward_equip_list[i]);
		if (insert_slot >= 0)
		{
			gamelog::g_log_tianshenhuti.printf(LL_INFO, "RoleReq Put Succ : user %d %s, insert_slot %d, reward_equip_id %d", 
				m_role->GetUID(), m_role->GetName(), insert_slot, reward_equip_list[i]);
		}
		else
		{
			gamelog::g_log_tianshenhuti.printf(LL_INFO, "RoleReq Put Fail : user %d %s, insert_slot %d, reward_equip_id %d", 
				m_role->GetUID(), m_role->GetName(), insert_slot, reward_equip_list[i]);
		}
	}

	Protocol::SCTianshenhutiRollResult tshtrr;
	tshtrr.reward_count = static_cast<short>(reward_equip_count);
	memcpy(tshtrr.reward_list, reward_equip_list, sizeof(tshtrr.reward_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tshtrr, sizeof(tshtrr));

	this->AddAccumulateTime(add_accumulate_times);

	this->SendAllInfoToRole();
}

void TianshenHuti::SendAllInfoToRole()
{
	Protocol::SCTianshenhutiALlInfo tsaf;
	UNSTD_STATIC_CHECK(sizeof(tsaf.equip_list) == sizeof(m_param.Param().inuse_equip_list));
	memcpy(tsaf.equip_list, m_param.Param().inuse_equip_list, sizeof(tsaf.equip_list));
	UNSTD_STATIC_CHECK(sizeof(tsaf.backpack_list) == sizeof(m_param.Param().backpack_equip_list));
	memcpy(tsaf.backpack_list, m_param.Param().backpack_equip_list, sizeof(tsaf.backpack_list));
	tsaf.used_free_times = m_param.Param().used_free_times;
	tsaf.backpack_num = m_backpack_num;
	tsaf.roll_score = m_param.Param().roll_score;
	tsaf.next_free_roll_time = m_param.Param().next_free_roll_time;
	tsaf.accumulate_roll_times = m_param.Param().accumulate_times;
	int send_len = sizeof(tsaf) - sizeof(TianshenEquipID)*(TIANSHENHUTI_BACKPACK_MAX_COUNT - m_backpack_num);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tsaf, send_len);
}

void TianshenHuti::OnDayChange(const ClockReturnDayItem &old_day, const ClockReturnDayItem &new_day)
{
	m_param.Param().used_free_times = 0;
	m_param.Param().next_free_roll_time = 0;

	m_param.Param().accumulate_times = 0;
	m_param.Param().accumulate_reward_flag = 0;

	this->SendAllInfoToRole();
}

void TianshenHuti::OnWeekChange()
{
}

void TianshenHuti::AddRollScore(int add_score, const char *reason)
{
	if (nullptr != reason && add_score > 0)
	{
		int old_score = m_param.Param().roll_score;
		m_param.Param().roll_score += add_score;

		Protocol::SCTianshenhutiScoreChange msg;
		msg.roll_score = m_param.Param().roll_score;

		SEND_TO_ROLE(m_role, msg);

		gamelog::g_log_tianshenhuti.printf(LL_INFO, "add roll score : user %d %s, old %d, new %d",
			m_role->GetUID(), m_role->GetName(), old_score, m_param.Param().roll_score);
	}
}

bool TianshenHuti::RemoveEquipFromBackpack(int slot_index)
{
	if (slot_index < 0 || slot_index >= TIANSHENHUTI_BACKPACK_MAX_COUNT || slot_index >= m_backpack_num || m_backpack_num <= 0)
	{
		return false;
	}

	m_param.Param().backpack_equip_list[slot_index] = m_param.Param().backpack_equip_list[m_backpack_num - 1];
	m_param.Param().backpack_equip_list[m_backpack_num - 1] = INVALID_TIANSHEN_EQUIP_ID;
	-- m_backpack_num;

	return true;
}

void TianshenHuti::UpdateTaozhuangNum(bool is_init)
{
	m_taozhuang_num_map.clear();

	int need_level = LOGIC_CONFIG->GetTianshenHutiConfig().GetTaoZhuangSkillNeedLevel();
	std::map<int, int> taozhuang_conform_level_num;

	for (int inuse_equip_index = 0; inuse_equip_index < TIANSHENHUTI_EQUIP_MAX_COUNT; ++inuse_equip_index)
	{
		const TianshenHutiEquipConfig *equip_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetEquipConfigWithID(m_param.Param().inuse_equip_list[inuse_equip_index]);
		if (nullptr != equip_cfg)
		{
			if (m_taozhuang_num_map.find(equip_cfg->level_taozhuang_type) != m_taozhuang_num_map.end())
			{
				++m_taozhuang_num_map[equip_cfg->level_taozhuang_type];
			}
			else
			{
				m_taozhuang_num_map[equip_cfg->level_taozhuang_type] = 1;
			}

			// 满足套装技能阶数的装备数量
			if (equip_cfg->level >= need_level)
			{
				if (taozhuang_conform_level_num.find(equip_cfg->taozhuang_type) != taozhuang_conform_level_num.end())
				{
					++taozhuang_conform_level_num[equip_cfg->taozhuang_type];
				}
				else
				{
					taozhuang_conform_level_num[equip_cfg->taozhuang_type] = 1;
				}
			}
		}
	}

	// 周末装备主动技能，暂时屏蔽
// 	if (!is_init)
// 	{
// 		std::vector<TianshenHutiTaoZhuangSkillCfg> &skill_cfg_vec = LOGIC_CONFIG->GetTianshenHutiConfig().GetTaoZhuangSkillCfg();
// 		for (auto &skill_cfg : skill_cfg_vec)
// 		{
// 			std::map<int, int>::iterator taozhuang_conform_num_it = taozhuang_conform_level_num.find(skill_cfg.taozhuang_type);
// 			if (taozhuang_conform_level_num.end() != taozhuang_conform_num_it && taozhuang_conform_num_it->second >= TIANSHENHUTI_EQUIP_MAX_COUNT)
// 			{
// 				m_role->GetSkillManager()->AddSkill(skill_cfg.active_skill_id, 1, nullptr, false, false);
// 			}
// 			else
// 			{
// 				m_role->GetSkillManager()->RemoveSkill(skill_cfg.active_skill_id, false);
// 			}
// 		}
// 
// 		m_role->GetSkillManager()->SendSkillListInfo();
// 	}
}

void TianshenHuti::AddAccumulateTime(int add_times)
{
	return;  // 屏蔽功能

	if (add_times <= 0)
	{
		return;
	}

	const tm *local_time = EngineAdapter::Instance().LocalTime();
	if (nullptr == local_time)
	{
		return;
	}

	if (SATURDAY == local_time->tm_wday || SUNDAY == local_time->tm_wday)
	{
		m_param.Param().accumulate_times += add_times;

		// 发奖
		TianshenHutiAccumulateRewardConfig *reward_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetAccumulateRewardCfg();
		if (nullptr != reward_cfg)
		{
			for (auto &item_cfg : reward_cfg->reward_vec)
			{
				if (m_param.Param().accumulate_times >= item_cfg.accumulate_times && !IS_BIT_SET(m_param.Param().accumulate_reward_flag, item_cfg.index))
				{
					ItemID reward_item_id = LOGIC_CONFIG->GetTianshenHutiConfig().GetAccumulateRandomRewardItem(item_cfg.reward_type);

					ItemConfigData reward_item(reward_item_id, true, 1);

					m_role->GetKnapsack()->PutOrMail(reward_item, PUT_REASON_WEEKEND_EQUIP_ACCUMULATE_REWARD);
					SET_BIT(m_param.Param().accumulate_reward_flag, item_cfg.index);
					break; // 注意break
				}
			}
		}
	}
}

void TianshenHuti::GetTaozhuangInjureAdd(int skill_index, double *rate_injure, int *static_injure)
{
	if (nullptr == rate_injure || nullptr == static_injure)
	{
		return;
	}

	for (auto &taozhuang_num_it : m_taozhuang_num_map)
	{
		// 小飞要求 伤害叠加且技术处理
		for (int taozhuang_num_tmp = 1; taozhuang_num_tmp <= taozhuang_num_it.second; ++taozhuang_num_tmp)
		{
			const TianshenHutiTaozhuangConfig *taozhuang_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetTaozhuangConfig(taozhuang_num_it.first, taozhuang_num_tmp);
			if (nullptr != taozhuang_cfg)
			{
				if (taozhuang_cfg->skill_num == skill_index)
				{
					*rate_injure += taozhuang_cfg->rate_injure * SKILL_ATTR_PER;
					*static_injure += taozhuang_cfg->static_injure;
				}
			}
		}
	}
}

void TianshenHuti::OnHurtOther(Character * target, int skill_id, long long & injure, int target_index, int & passive_flag)
{
	return; // 暂时屏蔽技能

	// 攻击一次清除技能三的隐身和伤害加深
	m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_INVISIBLE, EBT_INVISIBLE_HURT_ABLE, m_role->GetId(), 0, EffectBase::PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL3);
	m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_INC_HURT, EBT_INVALID, m_role->GetId(), 0, EffectBase::PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL3);

	// 广播隐身
	static Protocol::SCInvisibleView cmd;
	cmd.obj_id = m_role->GetId();
	cmd.is_invisible = INVISIBLE_SATTUS_CAN_SEE;
	m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
}

bool TianshenHuti::OnAddEquipByItem(int equip_id, const char *reason)
{
	if (nullptr == reason)
	{
		return false;
	}

	const TianshenHutiEquipConfig *equip_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetEquipConfigWithID(equip_id);
	
	if (nullptr != equip_cfg)
	{
		if (this->PutEuipToBackpack(equip_id) >= 0)
		{
			this->SendAllInfoToRole();

			// log
			gamelog::g_log_tianshenhuti.printf(LL_INFO, "OnAddEquipByItem : user(%d, %s), equip_id %d, reason %s",
				m_role->GetUID(), m_role->GetName(), equip_id, reason);

			return true;
		}
	}
	
	return false;
}

bool TianshenHuti::OnAutoAddEquipByItem(int equip_id, const char *reason)
{
	if (nullptr == reason)
	{
		return false;
	}

	const TianshenHutiEquipConfig *equip_cfg = LOGIC_CONFIG->GetTianshenHutiConfig().GetEquipConfigWithID(equip_id);
	if (nullptr == equip_cfg)
	{
		return false;
	}

	// 3阶及3阶以下的周末装备自动Put到虚拟背包
	const static int TIANSHENHUTI_AUTO_PUT_LEVEL = 3;

	if (equip_cfg->level <= TIANSHENHUTI_AUTO_PUT_LEVEL)
	{
		if (this->PutEuipToBackpack(equip_id) >= 0)
		{
			this->SendAllInfoToRole();

			// log
			gamelog::g_log_tianshenhuti.printf(LL_INFO, "OnAutoAddEquipByItem : user(%d, %s), equip_id %d, reason %s",
				m_role->GetUID(), m_role->GetName(), equip_id, reason);

			return true;
		}
	}

	return false;
}
