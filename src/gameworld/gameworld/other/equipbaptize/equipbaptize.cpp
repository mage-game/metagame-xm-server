#include "equipbaptize.hpp"
#include "equipbaptizeconfig.hpp"
#include "servercommon/equipbaptizedef.hpp"
#include "globalconfig/equipconfig.h"

#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "config/logicconfigmanager.hpp"
#include "other/event/eventhandler.hpp"
#include "gamelog.h"
#include "world.h"
#include "other/capability/capability.hpp"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "other/zhuanzhiequip/zhuanzhiequip.h"

#include <vector>

EquipBaptize::EquipBaptize() : m_role(NULL)
{
}

EquipBaptize::~EquipBaptize()
{
}

void EquipBaptize::InitParam(Role *role, const EquipBaptizeParam &param)
{
	m_role = role;
	m_param = param;
}

void EquipBaptize::GetInitParam(EquipBaptizeParam *param)
{
	*param = m_param; 
}

void EquipBaptize::OnDayChange(int old_dayid, int now_dayid)
{
	m_param.used_free_times = 0;
}

void EquipBaptize::OnRoleLogin()
{
	this->CheckOpenPartAndSlot();
}

void EquipBaptize::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		for (int equip_part = 0; equip_part < EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM; ++equip_part)
		{
			for (int slot_index = 0; slot_index < EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM; ++slot_index)
			{
				int seq = m_param.part_info_list[equip_part].attr_seq_list[slot_index];
				if (seq > 0)
				{
					int attr_value = m_param.part_info_list[equip_part].baptize_list[slot_index];
					if (attr_value > 0)
					{
						const EquipBaptizeAttrConfig *cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetReCalcAttrCfg(EQUIP_BAPTIZE_SPECIAL_TYPE_NONE, seq);
						if (NULL == cfg)
						{
							continue;
						}

						if (cfg->attr_type >= 0 && cfg->attr_type < CharIntAttrs::CHARINTATTR_TYPE_MAX)
						{
//							m_attrs_add.m_attrs[cfg->attr_type] += attr_value;
							m_attrs_add.AddValue(cfg->attr_type, attr_value);
						}
					}
				}
			}

			// 洗炼套装  万分比属性
			int baptize_color = 0;
			Attribute suit_attr_value = this->GetBaptizeSuitAttrValue(equip_part, baptize_color);
			if (suit_attr_value > 0)
			{
				int suit_attr_type = LOGIC_CONFIG->GetEquipBaptizeConfig().GetBaptizeSuitAttrType(equip_part, baptize_color);
				if (suit_attr_type >= 0 && suit_attr_type < CharIntAttrs::CHARINTATTR_TYPE_MAX)
				{
					if ((suit_attr_type >= CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP && suit_attr_type <= CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI) || 
						(suit_attr_type >= CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG && suit_attr_type <= CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI))
					{
						m_attrs_add.m_percent_attrs[suit_attr_type] += static_cast<float>(suit_attr_value * ROLE_ATTR_PER);
					}
					else if ((suit_attr_type >= CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN && suit_attr_type <= CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG) ||
						(suit_attr_type >= CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER && suit_attr_type <= CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI_PER))
					{
//						m_attrs_add.m_attrs[suit_attr_type] += suit_attr_value;
						m_attrs_add.AddValue(suit_attr_type, suit_attr_value);
					}
				}
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_EQUIP_BAPTIZE, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

void EquipBaptize::CheckOpenPartAndSlot()
{
	// 先检查角色等级
	for (int equip_part = 0; equip_part < EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM; ++equip_part)
	{
		const EquipBaptizeOpenConfig *open_cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetOpenCfg(equip_part);
		if (NULL != open_cfg)
		{
			if (m_role->GetLevel() >= open_cfg->role_level)
			{
				m_param.part_open_flag |= (1 << equip_part);
			}
		}
	}

	// 部位开启，有装备再开启第一条槽
	for (int equip_part = 0; equip_part < EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM; ++equip_part)
	{
		if (0 != (m_param.part_open_flag & (1 << equip_part)))
		{
			const ItemDataWrapper *equip_data = m_role->GetZhuanzhiEquip()->GetEquipmentGridData(equip_part);
			if (equip_data != nullptr)
			{
				const Equipment *equip = (const Equipment*)ITEMPOOL->GetItem(equip_data->item_id);
				if (NULL != equip && ItemBase::I_TYPE_EQUIPMENT == equip->GetItemType())
				{
					m_param.open_flag[equip_part] |= (1 << 0);
				}
			}	
		}
	}

	this->SendEquipBaptizeAllInfo();
}

void EquipBaptize::OnLockOrUnLockSlot(int equip_part, int slot_index)
{
	if (equip_part < 0 || equip_part >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM)
	{
		return;
	}

	int baptize_num = LOGIC_CONFIG->GetEquipBaptizeConfig().GetBaptizeCount();
	if (slot_index < 0 || slot_index >= EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM || slot_index >= baptize_num)
	{
		return;
	}

	if (0 == (m_param.part_open_flag & (1 << equip_part)))
	{
		m_role->NoticeNum(errornum::EN_EQUIP_BAPTIZE_PART_NOT_OPEN);
		return;
	}

	if (0 == (m_param.open_flag[equip_part] & (1 << slot_index)))
	{
		m_role->NoticeNum(errornum::EN_EQUIP_BAPTIZE_SLOT_NOT_OPEN);
		return;
	}

	int temp_flag = (1 << slot_index);
	if (0 == (m_param.lock_flag[equip_part] & temp_flag))
	{
		m_param.lock_flag[equip_part] |= temp_flag;
	}
	else if (0 != (m_param.lock_flag[equip_part] & temp_flag))
	{
		m_param.lock_flag[equip_part] &= (~temp_flag);
	}

	this->SendEquipBaptizeAllInfo();
}

void EquipBaptize::OnOpenBaptizeSlot(int equip_part, int index)
{
	int baptize_num = LOGIC_CONFIG->GetEquipBaptizeConfig().GetBaptizeCount();
	if (equip_part < 0 || equip_part >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM ||
		index < 0 || index >= EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM || index >= baptize_num)
	{
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (0 == (m_param.part_open_flag & (1 << equip_part)))
	{
		m_role->NoticeNum(errornum::EN_EQUIP_BAPTIZE_PART_NOT_OPEN);
		return;
	}

	if (0 != (m_param.open_flag[equip_part] & (1 << index)))
	{
		m_role->NoticeNum(errornum::EN_EQUIP_BAPTIZE_SLOT_ALREADY_OPEN);
		return;
	}

	// 检查装备
	const ItemDataWrapper *equip_data = m_role->GetZhuanzhiEquip()->GetEquipmentGridData(equip_part);
	if (equip_data == nullptr)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_NOT_EXIST);
		return;
	}

	const Equipment *equip = (const Equipment*)ITEMPOOL->GetItem(equip_data->item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_NOT_EXIST);
		return;
	}

	const EquipBaptizeConsumeConfig *consume_cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetConsumeCfg(index);
	if (NULL == consume_cfg)
	{
		return;
	}

	Money *money = m_role->GetKnapsack()->GetMoney();
	if (!money->AllGoldMoreThan(consume_cfg->consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 优先消耗绑元
	if (!money->UseAllGold(consume_cfg->consume_gold, "EquipBaptize::OnOpenBaptizeSlot"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 给予
	m_param.open_flag[equip_part] |= (1 << index);

	// 除第一条外其他开启给予属性
	if (index > 0 && index < baptize_num)
	{
		this->SetOpenSlotAttr(equip_part, index);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIP_BAPTIZE);
	}

	m_role->NoticeNum(noticenum::NT_EQUIP_BAPTIZE_OPEN_SUCC);

	this->SendEquipBaptizeAllInfo();

	// 日志
	gamelog::g_log_equip_baptize.printf(LL_INFO, "EquipBaptize::OnOpenBaptizeSlot Succ user[%d %s], equip_part[%d], slot_index[%d], consume_gold[%d]",
		m_role->GetUID(), m_role->GetName(), equip_part, index, consume_cfg->consume_gold);
}

void EquipBaptize::SetOpenSlotAttr(int equip_part, int slot_index)
{
	int baptize_num = LOGIC_CONFIG->GetEquipBaptizeConfig().GetBaptizeCount();
	if (equip_part < 0 || equip_part >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM ||
		slot_index < 0 || slot_index >= EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM || slot_index >= baptize_num)
	{
		return;
	}

	const EquipBigTypeConfig *big_type_cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetEquipBigType(equip_part);
	if (NULL == big_type_cfg)
	{
		return;
	}

	std::set<int> seq_record_list; // 记录属性seq，保让不重复
	for (int slot_idx = 0; slot_idx < baptize_num && slot_idx < EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM; ++slot_idx)
	{
		if (m_param.part_info_list[equip_part].attr_seq_list[slot_idx] > 0)
		{
			seq_record_list.insert(m_param.part_info_list[equip_part].attr_seq_list[slot_idx]);
		}
	}

	bool is_find = false;
	int loop_times = 0;
	while (!is_find && loop_times < 1000)
	{
		++loop_times;

		int attr_seq = this->GetRandAttrSeq(equip_part);
		if (attr_seq > 0 && seq_record_list.find(attr_seq) == seq_record_list.end())
		{
			is_find = true;
			seq_record_list.insert(attr_seq);

			const EquipBaptizeAttrConfig *attr_cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetAttrCfg(attr_seq, big_type_cfg->equip_big_type);
			if (NULL != attr_cfg)
			{
				m_param.part_info_list[equip_part].attr_seq_list[slot_index] = attr_seq;
				int value = this->GetColorAndAttrValBySeq(attr_seq);
				m_param.part_info_list[equip_part].baptize_list[slot_index] = value;

				// 日志
				gamelog::g_log_equip_baptize.printf(LL_INFO, "EquipBaptize::SetOpenSlotAttr Succ user[%d %s], equip_part[%d], slot_index[%d], attr_seq[%d], attr_value[%d]",
					m_role->GetUID(), m_role->GetName(), equip_part, slot_index, attr_seq, value);
			}
		}
	}
}

void EquipBaptize::BeginBaptize(int equip_part, bool is_auto_buy, int specila_type)
{
	if (equip_part < 0 || equip_part >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM || specila_type < EQUIP_BAPTIZE_SPECIAL_TYPE_NONE || specila_type >= EQUIP_BAPTIZE_SPECIAL_TYPE_MAX)
	{
		return;
	}

	int baptize_num = LOGIC_CONFIG->GetEquipBaptizeConfig().GetBaptizeCount();
	baptize_num = (baptize_num >= EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM) ? EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM : baptize_num;

	const ItemDataWrapper *equip_data = m_role->GetZhuanzhiEquip()->GetEquipmentGridData(equip_part);
	if (equip_data == nullptr)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_NOT_EXIST);
		return;
	}

	const Equipment *equip_item = (const Equipment *)ITEMPOOL->GetItem(equip_data->item_id);
	if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType()) return;

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	// 记录旧的颜色
	int old_color_num[EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM]; memset(old_color_num, 0, sizeof(old_color_num));			// 每种颜色对应的属性条数
	for (int i = 0; i < baptize_num; ++i)
	{
		int seq = m_param.part_info_list[equip_part].attr_seq_list[i];
		int attr_val = m_param.part_info_list[equip_part].baptize_list[i];
		int color = LOGIC_CONFIG->GetEquipBaptizeConfig().GetAttrColorBySeqAndValue(seq, attr_val);
		if (color >= 0 && color < EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM)
		{
			++old_color_num[color];
		}
	}

	std::set<int> seq_record_list;// 记录已锁住和洗出来属性seq，保证不重复

	int lock_num = 0;
	for (int slot_index = 0; slot_index < baptize_num; ++slot_index)
	{
		if (0 != (m_param.lock_flag[equip_part] & (1 << slot_index)))
		{
			++ lock_num;
			if (m_param.part_info_list[equip_part].attr_seq_list[slot_index] > 0)
			{
				seq_record_list.insert(m_param.part_info_list[equip_part].attr_seq_list[slot_index]);
			}
		}
	}

	if (lock_num >= baptize_num)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_BAPTIZE_CANT_LOCK_ALL_SLOT);
		return;
	}

	const EquipBaptizeLockConfig *consume_cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetLockCfg(lock_num);
	if (NULL == consume_cfg)
	{
		return;
	}

	const EquipBigTypeConfig *big_type_cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetEquipBigType(equip_part);
	if (NULL == big_type_cfg)
	{
		return;
	}

	// 免费不消耗材料
	bool is_free = false;
	if (m_param.used_free_times < LOGIC_CONFIG->GetEquipBaptizeConfig().GetOtherCfg().everyday_free_times)
	{
		is_free = true;
	}

	{
		static ItemExtern::ItemConsumeList consume_list;
		consume_list.Reset();
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		int item_count = 0;
		// 消耗
		if (!is_free)
		{
			// 普通附灵石
			stuff_list[item_count].item_id = consume_cfg->consume_stuff_id;
			stuff_list[item_count].num = consume_cfg->consume_stuff_num;
			stuff_list[item_count].buyable = is_auto_buy;
			item_count++;
		}

		if (specila_type == EQUIP_BAPTIZE_SPECIAL_TYPE_PURPLE && item_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
		{
			stuff_list[item_count].item_id = consume_cfg->purple_stuff_id;
			stuff_list[item_count].num = consume_cfg->purple_stuff_num;
			stuff_list[item_count].buyable = is_auto_buy;
			item_count++;
		}
		else if (specila_type == EQUIP_BAPTIZE_SPECIAL_TYPE_ORANGE && item_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
		{
			stuff_list[item_count].item_id = consume_cfg->orange_stuff_id;
			stuff_list[item_count].num = consume_cfg->orange_stuff_num;
			stuff_list[item_count].buyable = is_auto_buy;
			item_count++;
		}
		else if (specila_type == EQUIP_BAPTIZE_SPECIAL_TYPE_RED && item_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
		{
			stuff_list[item_count].item_id = consume_cfg->red_stuff_id;
			stuff_list[item_count].num = consume_cfg->red_stuff_num;
			stuff_list[item_count].buyable = is_auto_buy;
			item_count++;
		}

		if (item_count > 0)
		{
			if (!ItemExtern::GetConsumeListByOrder(m_role, false, item_count, stuff_list, &consume_list))
			{
				m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
				return;
			}

			// 消耗
			if (!ItemExtern::ConsumeItem(m_role, consume_list, "EquipBaptize::BeginBaptize"))
			{
				return;
			}
		}

		if (is_free)
		{
			++m_param.used_free_times;
		}
	}
	
	bool is_special_orange = false, is_special_red = false, is_has_purple = false;
	if (specila_type == EQUIP_BAPTIZE_SPECIAL_TYPE_PURPLE)
	{
		is_has_purple = true;
	}
	if (specila_type == EQUIP_BAPTIZE_SPECIAL_TYPE_ORANGE)
	{
		is_special_orange = true;
	}
	else if(specila_type == EQUIP_BAPTIZE_SPECIAL_TYPE_RED)
	{
		is_special_red = true;
	}

	/*
		以前洗炼第一条必出想要的颜色
		策划需求随机位置出想要的颜色
		不改动原算法的情况下记录一下发生改变的slot索引，洗炼完再随机一个Index与第一条进行交换
	*/
	std::vector<int> change_slot_index_vec; change_slot_index_vec.clear();
	// 给予
	for (int slot_index = 0; slot_index < baptize_num; ++slot_index)
	{
		if (0 == (m_param.open_flag[equip_part] & (1 << slot_index)))// 未开启部位
		{
			continue;
		}

		if (0 != (m_param.lock_flag[equip_part] & (1 << slot_index)))// 已锁定槽
		{
			continue;
		}

		// 洗出紫色
		if (is_has_purple && specila_type == EQUIP_BAPTIZE_SPECIAL_TYPE_PURPLE)
		{
			is_has_purple = false;
			
			// 在不重复条件下，给一条紫色属性
			bool is_find_purple = false;
			int loop_count = 0;
			while (!is_find_purple && loop_count < 1000)
			{
				++ loop_count;

				int attr_seq = this->GetRandAttrSeq(equip_part);
				if (attr_seq > 0 && seq_record_list.find(attr_seq) == seq_record_list.end())
				{
					is_find_purple = true;
					seq_record_list.insert(attr_seq);

					const EquipBaptizeAttrConfig *attr_cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetAttrCfg(attr_seq, big_type_cfg->equip_big_type);
					if (NULL != attr_cfg)
					{
						m_param.part_info_list[equip_part].attr_seq_list[slot_index] = attr_seq;
						int value = this->GetColorAndAttrValBySeq(attr_seq, EQUIP_BAPTIZE_SPECIAL_TYPE_PURPLE);
						m_param.part_info_list[equip_part].baptize_list[slot_index] = value;

						change_slot_index_vec.push_back(slot_index);
						// 日志
						gamelog::g_log_equip_baptize.printf(LL_INFO, "EquipBaptize::BeginBaptize_1 Succ user[%d %s], equip_part[%d], slot_index[%d], attr_seq[%d], attr_value[%d]",
							m_role->GetUID(), m_role->GetName(), equip_part, slot_index, attr_seq, value);
					}
				}
			}

			continue; // 防止紫色以上属性被覆盖
		}

		// 洗出橙色
		if (is_special_orange && specila_type == EQUIP_BAPTIZE_SPECIAL_TYPE_ORANGE)
		{
			is_special_orange = false;

			// 在不重复条件下，给一条紫色属性
			bool is_find_purple = false;
			int loop_count = 0;
			while (!is_find_purple && loop_count < 1000)
			{
				++loop_count;

				int attr_seq = this->GetRandAttrSeq(equip_part);
				if (attr_seq > 0 && seq_record_list.find(attr_seq) == seq_record_list.end())
				{
					is_find_purple = true;
					seq_record_list.insert(attr_seq);

					const EquipBaptizeAttrConfig *attr_cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetAttrCfg(attr_seq, big_type_cfg->equip_big_type);
					if (NULL != attr_cfg)
					{
						m_param.part_info_list[equip_part].attr_seq_list[slot_index] = attr_seq;
						int value = this->GetColorAndAttrValBySeq(attr_seq, EQUIP_BAPTIZE_SPECIAL_TYPE_ORANGE);
						m_param.part_info_list[equip_part].baptize_list[slot_index] = value;

						change_slot_index_vec.push_back(slot_index);
						// 日志
						gamelog::g_log_equip_baptize.printf(LL_INFO, "EquipBaptize::BeginBaptize_1 Succ user[%d %s], equip_part[%d], slot_index[%d], attr_seq[%d], attr_value[%d]",
							m_role->GetUID(), m_role->GetName(), equip_part, slot_index, attr_seq, value);
					}
				}
			}

			continue; // 防止橙色属性被覆盖
		}

		// 洗出红色
		if (is_special_red && specila_type == EQUIP_BAPTIZE_SPECIAL_TYPE_RED)
		{
			is_special_red = false;

			// 在不重复条件下，给一条紫色属性
			bool is_find_purple = false;
			int loop_count = 0;
			while (!is_find_purple && loop_count < 1000)
			{
				++loop_count;

				int attr_seq = this->GetRandAttrSeq(equip_part);
				if (attr_seq > 0 && seq_record_list.find(attr_seq) == seq_record_list.end())
				{
					is_find_purple = true;
					seq_record_list.insert(attr_seq);

					const EquipBaptizeAttrConfig *attr_cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetAttrCfg(attr_seq, big_type_cfg->equip_big_type);
					if (NULL != attr_cfg)
					{
						m_param.part_info_list[equip_part].attr_seq_list[slot_index] = attr_seq;
						int value = this->GetColorAndAttrValBySeq(attr_seq, EQUIP_BAPTIZE_SPECIAL_TYPE_RED);
						m_param.part_info_list[equip_part].baptize_list[slot_index] = value;

						change_slot_index_vec.push_back(slot_index);
						// 日志
						gamelog::g_log_equip_baptize.printf(LL_INFO, "EquipBaptize::BeginBaptize_1 Succ user[%d %s], equip_part[%d], slot_index[%d], attr_seq[%d], attr_value[%d]",
							m_role->GetUID(), m_role->GetName(), equip_part, slot_index, attr_seq, value);
					}
				}
			}

			continue; // 防止红色属性被覆盖
		}

		bool is_find = false;
		int loop_times = 0;
		while (!is_find && loop_times < 1000)
		{
			++ loop_times;

			int attr_seq = this->GetRandAttrSeq(equip_part);
			if (attr_seq > 0 && seq_record_list.find(attr_seq) == seq_record_list.end())
			{
				is_find = true;
				seq_record_list.insert(attr_seq);

				const EquipBaptizeAttrConfig *attr_cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetAttrCfg(attr_seq, big_type_cfg->equip_big_type);
				if (NULL != attr_cfg)
				{
					m_param.part_info_list[equip_part].attr_seq_list[slot_index] = attr_seq;
					int value = this->GetColorAndAttrValBySeq(attr_seq);
					m_param.part_info_list[equip_part].baptize_list[slot_index] = value;

					change_slot_index_vec.push_back(slot_index);
					// 日志
					gamelog::g_log_equip_baptize.printf(LL_INFO, "EquipBaptize::BeginBaptize_2 Succ user[%d %s], equip_part[%d], slot_index[%d], attr_seq[%d], attr_value[%d]",
						m_role->GetUID(), m_role->GetName(), equip_part, slot_index, attr_seq, value);
				}
			}
		}
	}
	seq_record_list.clear();
	if ((int)change_slot_index_vec.size() > 1)  // 只洗一条的不需要再进行交换了
	{
		int index = RandomNum((int)change_slot_index_vec.size());
		if (index > 0 && index < (int)change_slot_index_vec.size())   // 随机到1~size()-1再进行交换
		{
			int first_index = change_slot_index_vec[0];
			int slot_index = change_slot_index_vec[index];
			if ((first_index >= 0 && first_index < baptize_num && first_index < EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM) &&
				(slot_index >= 0 && slot_index < baptize_num && slot_index < EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM))
			{
				int first_seq = m_param.part_info_list[equip_part].attr_seq_list[first_index];
				int first_value = m_param.part_info_list[equip_part].baptize_list[first_index];
				m_param.part_info_list[equip_part].attr_seq_list[first_index] = m_param.part_info_list[equip_part].attr_seq_list[slot_index];
				m_param.part_info_list[equip_part].baptize_list[first_index] = m_param.part_info_list[equip_part].baptize_list[slot_index];
				m_param.part_info_list[equip_part].attr_seq_list[slot_index] = first_seq;
				m_param.part_info_list[equip_part].baptize_list[slot_index] = first_value;
			}
		}
	}

	EventHandler::Instance().OnEquipBaptize(m_role);
		
	this->SendEquipBaptizeAllInfo();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIP_BAPTIZE);

	// 日志
	gamelog::g_log_equip_baptize.printf(LL_INFO, "EquipBaptize::BeginBaptize Succ user[%d %s], equip_part[%d], used_free_times[%d], special_type[%d], consume_item_id[%d], item_num[%d]",
		m_role->GetUID(), m_role->GetName(), equip_part, m_param.used_free_times, specila_type, consume_cfg->consume_stuff_id, consume_cfg->consume_stuff_num);

	int color_num[EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM]; memset(color_num, 0, sizeof(color_num));			// 每种颜色对应的属性条数
	int extra_color_num[EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM]; memset(extra_color_num, 0, sizeof(color_num)); // 同上，只是高等颜色同时也会算入低级颜色的数量里面
	for (int i = 0; i < baptize_num; ++i)
	{
		int seq = m_param.part_info_list[equip_part].attr_seq_list[i];
		int attr_val = m_param.part_info_list[equip_part].baptize_list[i];
		int color = LOGIC_CONFIG->GetEquipBaptizeConfig().GetAttrColorBySeqAndValue(seq, attr_val);
		if (color >= 0 && color < EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM)
		{
			++color_num[color];

			for (int tmp_color = 0; tmp_color <= color; ++tmp_color)
			{
				++extra_color_num[tmp_color];
			}
		}
	}

	// 传闻
	// 传闻		
	// --------------------------------------------------------------		//int role_id, ItemID equip_id, bool is_bind, UInt32 invalid_time, int str_level, const EquipmentParam *equip_param,
	// ---------------------------------------------------------------------//const EquipBaptizePartInfo *info = NULL, const ItemID *stone_p = NULL, int stone_size = 0, int stone_baptize_level = 0, int suit_index = -1, int suit_count = 0
	EquipmentParam *equip_param = equip_item->CreateParamFromStruct(&equip_data->param_data);
	if (equip_param != nullptr)
	{
		const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(m_role->GetUserId()), equip_item->GetItemId(), true, equip_data->invalid_time, equip_param, &m_param.part_info_list[equip_part]);
		// 有3条紫色
		if (3 == color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_PURPLE] && NULL != equip_param_string)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_equip_baptize_four_purple_notice,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), equip_data->item_id, equip_param_string, color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_PURPLE]);
			if (sendlen > 0)
			{
				//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
		}

		// 只有橙
		if (color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_ORANGE] - old_color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_ORANGE] > 0 &&
			color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_RED] == old_color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_RED] && NULL != equip_param_string)
		{
			int add_orange_num = color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_ORANGE] - old_color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_ORANGE];
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_equip_baptize_orange_notice,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), equip_data->item_id, equip_param_string, add_orange_num);
			if (sendlen > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
		}
		// 只有红
		else if (color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_ORANGE] == old_color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_ORANGE] &&
			color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_RED] - old_color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_RED] > 0 && NULL != equip_param_string)
		{
			int add_red_num = color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_RED] - old_color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_RED];
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_equip_baptize_red_notice,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), equip_data->item_id, equip_param_string, add_red_num);
			if (sendlen > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
		}
		// 橙红都有
		else if (color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_ORANGE] - old_color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_ORANGE] > 0 &&
			color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_RED] - old_color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_RED] > 0 && NULL != equip_param_string)
		{
			int add_orange_num = color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_ORANGE] - old_color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_ORANGE];
			int add_red_num = color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_RED] - old_color_num[EQUIP_BAPTIZE_ATTR_COLOR_TYPE_RED];

			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_equip_baptize_orange_and_red_notice,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), equip_data->item_id, equip_param_string, add_orange_num, add_red_num);
			if (sendlen > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	this->PrintOperationLog();
}

int EquipBaptize::GetRandAttrSeq(int equip_part)
{
	if (equip_part < 0 || equip_part >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM)
	{
		return -1;
	}

	const EquipBigTypeConfig *big_type_cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetEquipBigType(equip_part);
	if (NULL == big_type_cfg)
	{
		return -1;
	}

	int total_weight = 0;
	for (int i = 1; i < CharIntAttrs::CHARINTATTR_TYPE_MAX; ++i)
	{
		const EquipBaptizeAttrConfig *attr_cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetAttrCfg(i, big_type_cfg->equip_big_type);
		if (NULL != attr_cfg)
		{
			total_weight += attr_cfg->weight;
		}
	}

	if (total_weight <= 0) return -1;

	int real_seq = 0;
	int rand_value = RandomNum(total_weight);
	for (int i = 1; i < CharIntAttrs::CHARINTATTR_TYPE_MAX; ++i)
	{
		const EquipBaptizeAttrConfig *attr_cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetAttrCfg(i, big_type_cfg->equip_big_type);
		if (NULL == attr_cfg) continue;

		if (rand_value < attr_cfg->weight)
		{
			real_seq = i;
			break;
		}

		rand_value -= attr_cfg->weight;
	}

	return real_seq;
}

int EquipBaptize::GetColorAndAttrValBySeq(int seq, int special_type)
{
	if (special_type < EQUIP_BAPTIZE_SPECIAL_TYPE_NONE || special_type >= EQUIP_BAPTIZE_SPECIAL_TYPE_MAX)
	{
		return 0;
	}

	const EquipBaptizeAttrConfig *cfg = LOGIC_CONFIG->GetEquipBaptizeConfig().GetReCalcAttrCfg(special_type, seq);
	if (NULL == cfg)
	{
		return 0;
	}

	int total_weight = 0;
	int color_seq = 0;

	for (int i = 0; i < EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM; ++i)
	{
		total_weight += cfg->color_list[i].color_weight;
	}

	if (total_weight <= 0) return 0;

	int rand_val = RandomNum(total_weight);

	for (int i = 0; i < EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM; ++i)
	{
		if (rand_val < cfg->color_list[i].color_weight)
		{
			color_seq = i;
			break;
		}

		rand_val -= cfg->color_list[i].color_weight;
	}

	// 随机属性值
	int rand_attr_val = 0;
	if (color_seq < 0 || color_seq >= EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM)
	{
		return rand_attr_val;
	}

	if (cfg->color_list[color_seq].attr_val_low >= 0 && cfg->color_list[color_seq].attr_val_high > 0)
	{
		rand_attr_val = RandomNum(cfg->color_list[color_seq].attr_val_low, cfg->color_list[color_seq].attr_val_high);
	}

	return rand_attr_val;
}

EquipBaptizePartInfo *EquipBaptize::GetEquipBaptizePartInfo(int equip_part)
 {
	if (equip_part < 0 || equip_part >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM) return nullptr;

	return &m_param.part_info_list[equip_part]; 
}

void EquipBaptize::SendEquipBaptizeAllInfo()
{
	static Protocol::SCEquipBaptizeAllInfo ebai;

	UNSTD_STATIC_CHECK(sizeof(m_param.part_info_list) == sizeof(ebai.part_info_list));
	UNSTD_STATIC_CHECK(sizeof(m_param.open_flag) == sizeof(ebai.open_flag));
	UNSTD_STATIC_CHECK(sizeof(m_param.lock_flag) == sizeof(ebai.lock_flag));

	memcpy(ebai.part_info_list, m_param.part_info_list, sizeof(ebai.part_info_list));
	memcpy(ebai.open_flag, m_param.open_flag, sizeof(ebai.open_flag));
	memcpy(ebai.lock_flag, m_param.lock_flag, sizeof(ebai.lock_flag));
	ebai.used_free_times = m_param.used_free_times;
	ebai.reserve_ch = 0;
	ebai.reserve_sh = 0;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ebai, sizeof(ebai));
}


int EquipBaptize::GetColorAttrCount(int color)
{
	int attr_num = 0;

	for (int equip_part = 0; equip_part < EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM; ++equip_part)
	{
		for (int slot_index = 0; slot_index < EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM; ++slot_index)
		{
			int seq = m_param.part_info_list[equip_part].attr_seq_list[slot_index];
			int attr_val = m_param.part_info_list[equip_part].baptize_list[slot_index];
			int attr_color = LOGIC_CONFIG->GetEquipBaptizeConfig().GetAttrColorBySeqAndValue(seq, attr_val);
			if (attr_color >= color)
			{
				attr_num++;
			}
		}
	}

	return attr_num;
}

void EquipBaptize::PrintOperationLog()
{
	int orange_num = 0;
	int ren_num = 0;
	int baptize_num = LOGIC_CONFIG->GetEquipBaptizeConfig().GetBaptizeCount();
	for (int equip_part = 0; equip_part < E_INDEX_ZHUANZHI::E_INDEX_MAX; equip_part++)
	{
		for (int i = 0; i < baptize_num && i < EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM; ++i)
		{
			int seq = m_param.part_info_list[equip_part].attr_seq_list[i];
			int attr_val = m_param.part_info_list[equip_part].baptize_list[i];
			int color = LOGIC_CONFIG->GetEquipBaptizeConfig().GetAttrColorBySeqAndValue(seq, attr_val);
			if (color == EQUIP_BAPTIZE_ATTR_COLOR_TYPE_ORANGE)
			{
				++orange_num;
			}
			else if (color == EQUIP_BAPTIZE_ATTR_COLOR_TYPE_RED)
			{
				++ren_num;
			}
		}
	}

	char log_quick_str[LOG_BASE_LENGTH] = { 0 };
	SNPRINTF(log_quick_str, LOG_BASE_LENGTH, "[%d]_[%d]", ren_num, orange_num);
	ROLE_LOG_QUICK6(LOG_TYPE_WASH, m_role, 0, 0, log_quick_str, NULL);
}

Attribute EquipBaptize::GetBaptizeSuitAttrValue(int equip_index, int &baptize_color)
{
	Attribute attr_value = 0;
	if (equip_index < 0 || equip_index >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM) return attr_value;
	int baptize_color_num[EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM]; memset(baptize_color_num, 0, sizeof(baptize_color_num));

	for (int i = 0; i < EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM; ++i)
	{
		int seq = m_param.part_info_list[equip_index].attr_seq_list[i];
		int attr_val = m_param.part_info_list[equip_index].baptize_list[i];
		int color = LOGIC_CONFIG->GetEquipBaptizeConfig().GetAttrColorBySeqAndValue(seq, attr_val);
		if (color >= 0 && color < EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM)
		{
			for (int tmp_color = 0; tmp_color <= color; ++tmp_color)
			{
				++baptize_color_num[tmp_color];
			}
		}
	}

	attr_value = LOGIC_CONFIG->GetEquipBaptizeConfig().GetBaptizeSuitAttrValue(equip_index, baptize_color_num, baptize_color);

	return attr_value;
}