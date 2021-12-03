#include "stdafx.h"

#include "zodiac.hpp"
#include "other/zodiac/zodiacconfig.hpp"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "gameworld/gameworld/gstr.h"
#include "gamelog.h"
#include "config/logicconfigmanager.hpp"
#include "other/capability/capability.hpp"
#include "common/bintohex.h"

Zodiac::Zodiac() : m_role(NULL), m_empty_grids(0)
{
}

Zodiac::~Zodiac()
{
}

void Zodiac::Init(Role *role, const ZodiacParam &param)
{
	m_role = role;
	m_param = param;
}

void Zodiac::GetInitParam(ZodiacParam *param) const
{
	*param = m_param;
}

void Zodiac::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		int open_level = LOGIC_CONFIG->GetZodiacConfig().GetOtherCfg().open_level;
		if (m_role->GetLevel() < open_level)
		{
			return;
		}

		for (int zodiac_index = 0; zodiac_index < ZODIAC_MAX_NUM; ++zodiac_index)
		{
			// 从未激活过
			int zodiac_num = this->CheckActivateZodiacNum(zodiac_index);
			if (zodiac_num <= 0)
			{
				continue;
			}

			if (m_param.item_list[zodiac_index].level > 0)	// 激活成功并且已经升级的生肖
			{
				for (int attr_index = 0; attr_index < ZODIAC_ATTR_MAX_NUM; ++attr_index)
				{
					const ZodiacLevelupCfg *levelup_cfg = LOGIC_CONFIG->GetZodiacConfig().GetLevelupCfg(zodiac_index, m_param.item_list[zodiac_index].level);
					if (NULL == levelup_cfg)
					{
						continue;
					}
					m_attrs_add.AddValue(levelup_cfg->attr_type[attr_index], levelup_cfg->attr_value[attr_index]);
				}
			}
			else	// 开始激活并且未升级的生肖
			{
				for (int i = 1; i <= zodiac_num; ++i)
				{
					const ZodiacActivateCfg *activate_cfg = LOGIC_CONFIG->GetZodiacConfig().GetActivateCfg(zodiac_index, i);
					if (NULL == activate_cfg)
					{
						continue;
					}
					m_attrs_add.AddValue(activate_cfg->attr_type, activate_cfg->attr_value);
				}
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_ZODIAC, m_attrs_add);
	}
	base_add.Add(m_attrs_add.m_attrs, true);
}

void Zodiac::ZodiacOpera(int opera_type, int param1, int param2)
{
	int open_level = LOGIC_CONFIG->GetZodiacConfig().GetOtherCfg().open_level;
	if (m_role->GetLevel() < open_level || opera_type < 0 || opera_type >= Protocol::ZODIAC_OPERA_TYPE_MAX)
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::ZODIAC_OPERA_TYPE_ALL_INFO:
	{
		this->SendAllInfo();
	}
	break;

	case Protocol::ZODIAC_OPERA_TYPE_ACTIVATE:
	{
		this->ZodiacActivate(param1);
	}
	break;

	case Protocol::ZODIAC_OPERA_TYPE_LEVELUP:
	{
		this->ZodiacLevelup(param1);
	}
	break;

	case Protocol::ZODIAC_OPERA_TYPE_DECOMPOSE:
	{
		this->ZodiacDecompose(1, &param2);
	}
	break;
	}
}

void Zodiac::SendAllInfo()
{
	this->SendBaseInfo();
	this->SendZodiacInfo();
	this->SendBackpackInfo();
}

void Zodiac::ZodiacActivate(int backpack_index)
{
	if (backpack_index < 0 || backpack_index >= ZODIAC_BACKPACK_MAX_GRIDS)
	{
		return;
	}

	ZodiacBackpackItem &bag_item = m_param.backpack_zodiac_list[backpack_index];
	if (bag_item.IsEmpty())
	{
		return;
	}

	int open_level = LOGIC_CONFIG->GetZodiacConfig().GetOtherCfg().open_level;
	if (m_role->GetLevel() < open_level)
	{
		return;
	}

	// 判断背包物品是否是生肖物品
	const ZodiacRealIdCfg *zodiac_cfg = LOGIC_CONFIG->GetZodiacConfig().GetRealIdCfg(bag_item.item_id);
	if (NULL == zodiac_cfg)
	{
		return;
	}

	// 判断是否已经激活生肖图片
	bool activate_flag = m_param.item_list[bag_item.zodiac_index].IsActivateSuipian(bag_item.suipian_index);
	if (activate_flag)
	{
		gstr::SendError(m_role, "zodiac_already_activate");
		return;
	}

	m_param.item_list[bag_item.zodiac_index].ActivateSuipian(bag_item.suipian_index);
	int activate_count = this->CheckActivateZodiacNum(bag_item.zodiac_index);
	if (ZODIAC_SUIPIAN_MAX_NUM == activate_count)
	{
		m_param.item_list[bag_item.zodiac_index].level = 0;
	}

	gamelog::g_log_zodiac.printf(LL_INFO, "Zodiac::ZodiacActivate user[%d, %s] backpack_index[%d] item_id[%d] zodiac_index[%d] suipian_index[%d] activate_count[%d])",
		m_role->GetUID(), m_role->GetName(), backpack_index, (int)bag_item.item_id, (int)bag_item.zodiac_index, (int)bag_item.suipian_index, activate_count);

	// 消耗
	bag_item.Reset();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZODIAC);
	gstr::SendNotice(m_role, "zodiac_activate_succ");

	this->SendZodiacInfo();
}

void Zodiac::ZodiacLevelup(int zodiac_index)
{
	if (zodiac_index < 0 || zodiac_index >= ZODIAC_MAX_NUM)
	{
		return;
	}

	ZodiacItem *zodiac_item = NULL;
	const ZodiacOtherCfg &other_cfg = LOGIC_CONFIG->GetZodiacConfig().GetOtherCfg();
	if (m_role->GetLevel() < other_cfg.open_level)
	{
		return;
	}

	zodiac_item = &m_param.item_list[zodiac_index];

	if (NULL == zodiac_item || zodiac_item->IsEmpty())
	{
		return;
	}

	if (zodiac_item->level >= other_cfg.max_level)
	{
		gstr::SendError(m_role, "zodiac_already_max_level");
		return;
	}

	const ZodiacLevelupCfg *next_levelup_cfg = LOGIC_CONFIG->GetZodiacConfig().GetLevelupCfg(zodiac_index, zodiac_item->level + 1);
	const ZodiacLevelupCfg *levelup_cfg = LOGIC_CONFIG->GetZodiacConfig().GetLevelupCfg(zodiac_index, zodiac_item->level);
	if (NULL == next_levelup_cfg || NULL == levelup_cfg)
	{
		gstr::SendError(m_role, "zodiac_can_not_levelup");
		return;
	}

	int old_jinghua_num = m_param.jinghua_num;
	int old_level = zodiac_item->level;
	if (!this->AddJinghua(-levelup_cfg->jinghua_num, "Zodiac::ZodiacLevelup"))
	{
		gstr::SendError(m_role, "zodiac_jinghua_num_no_enough");
		return;
	}
	++zodiac_item->level;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZODIAC);
	gstr::SendNotice(m_role, "zodiac_levelup_succ");

	this->SendZodiacInfo();

	gamelog::g_log_zodiac.printf(LL_INFO, "Zodiac::ZodiacLevelup user[%d, %s] index[%d] jinghua_num(%d -> %d) level(%d -> %d)",
		m_role->GetUID(), m_role->GetName(), zodiac_index, old_jinghua_num, m_param.jinghua_num, old_level, zodiac_item->level);
}

void Zodiac::ZodiacDecompose(int decompose_num, int *decompose_backpack_index_list)
{
	gamelog::g_log_zodiac.buff_printf("Zodiac::ZodiacDecompose user[%d, %s] decomposelists[", m_role->GetUID(), m_role->GetName());
	int total_jinghua_num = 0;

	for (int i = 0; i < decompose_num && i < ZODIAC_BACKPACK_MAX_GRIDS; ++i)
	{
		int backpack_index = decompose_backpack_index_list[i];
		if (backpack_index < 0 || backpack_index >= ZODIAC_BACKPACK_MAX_GRIDS)
		{
			continue;
		}

		ZodiacBackpackItem &backpack_item = m_param.backpack_zodiac_list[backpack_index];
		if (backpack_item.IsEmpty())
		{
			continue;
		}

		const ZodiacRealIdCfg *zodiac_cfg = LOGIC_CONFIG->GetZodiacConfig().GetRealIdCfg(backpack_item.item_id);
		if (NULL == zodiac_cfg)
		{
			continue;
		}
		int zodiac_index = zodiac_cfg->zodiac_index;
		int suipian_index = zodiac_cfg->suipian_index;

		const ZodiacDecomposeCfg *decompose_cfg = LOGIC_CONFIG->GetZodiacConfig().GetDecomposeCfg(zodiac_index, suipian_index);
		if (NULL == decompose_cfg)
		{
			continue;
		}

		if (!this->AddJinghua(decompose_cfg->jinghua_num, "Zodiac::ZodiacDecompose"))
		{
			continue;
		}

		total_jinghua_num += decompose_cfg->jinghua_num;

		backpack_item.Reset();
		++m_empty_grids;

		gamelog::g_log_zodiac.buff_printf("(%d, %d), ", zodiac_index, suipian_index);
	}

	gstr::SendNotice(m_role, "zodiac_decompose_succ");
	this->SendBackpackInfo();

	gamelog::g_log_zodiac.buff_printf("] total_jinghua_num[%d]", total_jinghua_num);
	gamelog::g_log_zodiac.commit_buff(LL_INFO);
}

bool Zodiac::AddItemsToBackpack(ItemID item_id, int num)
{
	if (!this->CheckHasSpace(num))
	{
		gstr::SendError(m_role, "zodiac_backpack_no_enough_space");
		return false;
	}

	const ZodiacRealIdCfg * zodiac_cfg = LOGIC_CONFIG->GetZodiacConfig().GetRealIdCfg(item_id);
	if (NULL == zodiac_cfg)
	{
		return false;
	}

	gamelog::g_log_zodiac.buff_printf("Zodiac::AddItemsToBackpack user[%d, %s] item_id[%d] item_index_list[",
		m_role->GetUID(), m_role->GetName(), item_id);

	int left_num = num;
	for (int index = 0; index < ZODIAC_BACKPACK_MAX_GRIDS; ++index)
	{
		if (m_param.backpack_zodiac_list[index].IsEmpty())
		{
			m_param.backpack_zodiac_list[index].item_id = item_id;
			m_param.backpack_zodiac_list[index].zodiac_index = zodiac_cfg->zodiac_index;
			m_param.backpack_zodiac_list[index].suipian_index = zodiac_cfg->suipian_index;

			gamelog::g_log_zodiac.buff_printf("%d, ", index);

			--left_num;
			--m_empty_grids;
			if (left_num <= 0)
			{
				break;
			}
		}
	}

	gamelog::g_log_zodiac.commit_buff(LL_INFO);

	this->SendBackpackInfo();
	return true;
}

bool Zodiac::AddJinghua(int jinghua_num, const char *reason)
{
	if (NULL == reason) return false;

	long long target_jinghua_num = (long long)m_param.jinghua_num + jinghua_num;
	if (target_jinghua_num < 0 || target_jinghua_num >= INT_MAX)
	{
		return false;
	}

	m_param.jinghua_num = static_cast<int>(target_jinghua_num);

	this->SendBaseInfo();

	gamelog::g_log_zodiac.printf(LL_INFO, "Zodiac::AddJinghua user[%d, %s] add_jinghua[%d] jinghua_num[%d] reason[%s]",
		m_role->GetUID(), m_role->GetName(), jinghua_num, m_param.jinghua_num, reason);

	return true;
}

int Zodiac::CheckActivateZodiacNum(int zodiac_index)
{
	if (zodiac_index < 0 || zodiac_index >= ZODIAC_MAX_NUM)
	{
		return 0;
	}

	int activate_count = 0;
	for (int i = 0; i < ZODIAC_SUIPIAN_MAX_NUM; ++i)
	{
		if (m_param.item_list[zodiac_index].IsActivateSuipian(i))
		{
			++activate_count;
		}
	}

	return activate_count;
}

void Zodiac::GMClearAll()
{
	const int hex_len = sizeof(m_param) * 2 + 1;
	char hex_buff[hex_len] = { 0 };
	BinToHex((const char*)&m_param, sizeof(m_param), hex_buff);

	const int hex_len1 = sizeof(m_attrs_add) * 2 + 1;
	char hex_buff1[hex_len1] = { 0 };
	BinToHex((const char*)&m_attrs_add, sizeof(m_attrs_add), hex_buff1);

	gamelog::g_log_zodiac.printf(LL_INFO, "Zodiac::GMClearAll user[%d, %s] m_param[%s] m_attrs_add[%s] m_empty_grids[%d]",
		m_role->GetUID(), m_role->GetName(), hex_buff, hex_buff1, m_empty_grids);

	m_param.Reset();
	m_attrs_add.Reset();
	m_empty_grids = 0;
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZODIAC);
	this->SendAllInfo();
}

void Zodiac::SendBaseInfo()
{
	static Protocol::SCZodiacBaseInfo zbi;
	zbi.jinghua_num = m_param.jinghua_num;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&zbi, sizeof(zbi));
}

void Zodiac::SendZodiacInfo()
{
	static Protocol::SCZodiacInfo zi;

	for (int i = 0; i < ZODIAC_MAX_NUM; ++i)
	{
		zi.zodiac_item[i].level = m_param.item_list[i].level;
		zi.zodiac_item[i].activate_flag = m_param.item_list[i].activate_flag;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&zi, sizeof(zi));
}

void Zodiac::SendBackpackInfo()
{
	this->SortBackpack();

	static Protocol::SCZodiacBackpackInfo zbi;
	zbi.grid_num = 0;

	for (int index = 0; index < ZODIAC_BACKPACK_MAX_GRIDS; ++index)
	{
		ZodiacBackpackItem *backpack_item = &m_param.backpack_zodiac_list[index];
		if (NULL == backpack_item || backpack_item->IsEmpty()) // 有效ID才下发（0也下发，因为消耗后也要通知一下）
		{
			continue;
		}

		zbi.grid_list[zbi.grid_num].item_id = backpack_item->item_id;
		zbi.grid_list[zbi.grid_num].zodiac_index = backpack_item->zodiac_index;
		zbi.grid_list[zbi.grid_num].suipian_index = backpack_item->suipian_index;
		++zbi.grid_num;
	}

	int send_len = int(sizeof(zbi) - sizeof(zbi.grid_list[0]) * (ZODIAC_BACKPACK_MAX_GRIDS - zbi.grid_num));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&zbi, send_len);
}

bool Zodiac::CheckHasSpace(int count) const
{
	if (count <= 0)
	{
		return true;
	}

	for (int index = 0; index < ZODIAC_BACKPACK_MAX_GRIDS; ++index)
	{
		if (m_param.backpack_zodiac_list[index].IsEmpty())
		{
			if (--count <= 0)
			{
				return true;
			}
		}
	}

	return false;
}

int CompareZodiacStruc(const void *arg1, const void *arg2)
{
	ZodiacBackpackItem *s1 = (ZodiacBackpackItem*)arg1;
	ZodiacBackpackItem *s2 = (ZodiacBackpackItem*)arg2;

	if (s1->item_id <= 0 && s2->item_id <= 0)
	{
		return 0;
	}
	else if (s1->item_id <= 0 && s2->item_id > 0)
	{
		return 1;
	}
	else if (s1->item_id > 0 && s2->item_id <= 0)
	{
		return -1;
	}
	else if (s1->item_id > 0 && s2->item_id > 0)
	{
		if (s1->item_id > s2->item_id)
		{
			return 1;
		}
		else if (s1->item_id < s2->item_id)
		{
			return -1;
		}
	}

	return 0;
}

void Zodiac::SortBackpack()
{
	qsort(m_param.backpack_zodiac_list, ZODIAC_BACKPACK_MAX_GRIDS, sizeof(ZodiacBackpackItem), CompareZodiacStruc);
}
