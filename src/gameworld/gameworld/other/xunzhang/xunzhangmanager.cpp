#include "xunzhangmanager.hpp"
#include "servercommon/configcommon.h"
#include "obj/character/role.h"
#include "xunzhangconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "scene/scene.h"
#include "servercommon/namefilter.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "other/event/eventhandler.hpp"
#include "gameworld/scene/scenemanager.h"
#include "servercommon/string/gameworldstr.h"
#include "gameworld/world.h"
#include "protocal/msgxunzhang.h"
#include "gameworld/item/knapsack.h"
#include "other/capability/capability.hpp"

XunZhangManager::XunZhangManager() : m_role(NULL), m_total_level(0), m_mount_attr_add(0), m_wing_attr_add(0), 
	m_halo_attr_add(0), m_shengong_attr_add(0), m_shenyi_attr_add(0), m_footprint_attr_add(0)
{
}

XunZhangManager::~XunZhangManager()
{
}

void XunZhangManager::Init(Role *role, const XunZhangParam &xunzhang_param)
{
	m_role = role;
	m_xunzhang_param = xunzhang_param;

	this->ReCalcTotalLevel();
}

void XunZhangManager::GetInitParam(XunZhangParam *xunzhang_param)
{
	*xunzhang_param = m_xunzhang_param;
}

void XunZhangManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		for (int i = 0; i < MAX_XUNZHANG_COUNT; i++)
		{
			const XunZhangLevelCfg *level_cfg = LOGIC_CONFIG->GetXunZhangConfig().GetXunZhangLevelCfg(i, m_xunzhang_param.level_list[i]);
			if (NULL != level_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += level_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += level_cfg->jianren;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += level_cfg->per_jingzhun;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += level_cfg->per_baoji;
// 				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += level_cfg->per_pofang;
// 				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += level_cfg->per_mianshang;

				// 下面特殊属性用乘法
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, level_cfg->per_mianshang);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, level_cfg->per_pofang);
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_XUNZHANG, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

int XunZhangManager::XunZhangGetAttributeValue(int attr_type)
{
	if (attr_type < 0 || attr_type >=XUNZHANG_ADD_TYPE_MAX)
	{
		return 0;
	}

	int value = 0;
	for (int i = 0; i < MAX_XUNZHANG_COUNT; i++)
	{
		const XunZhangLevelCfg *level_cfg = LOGIC_CONFIG->GetXunZhangConfig().GetXunZhangLevelCfg(i, m_xunzhang_param.level_list[i]);
		if (NULL != level_cfg)
		{
			if (XUNZHANG_ADD_KILL_MONSTER_EXP_REWARD_PER == attr_type)
			{
				value += level_cfg->per_monster_exp;
			}
			else if(XUNZHANG_ADD_HURT_TO_BOSS_PER == attr_type)
			{
				value += level_cfg->per_boss_hurt;
			}
			else if(XUNZHANG_ADD_HURT_TO_MONSTER_PER == attr_type)
			{
				value += level_cfg->per_monster_hurt;
			}
		}
	}
	return value;
}

int XunZhangManager::GetMountAddPer()
{
	return m_mount_attr_add;
}

int XunZhangManager::GetWingAddPer()
{
	return m_wing_attr_add;
}

int XunZhangManager::GetHaloAddPer()
{
	return m_halo_attr_add;
}

int XunZhangManager::GetShengongAddPer()
{
	return m_shengong_attr_add;
}

int XunZhangManager::GetShenyiAddPer()
{
	return m_shenyi_attr_add;
}

int XunZhangManager::GetFootprintAddPer()
{
	return m_footprint_attr_add;
}

void XunZhangManager::SendAllXunZhangInfo()
{
	static Protocol::SCAllXunZhangInfo pro;
	memcpy(pro.level_list, m_xunzhang_param.level_list, sizeof(pro.level_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void XunZhangManager::OnUplevelReq(int xunzhang_id, int is_only_bind)
{
	if (xunzhang_id < 0 || xunzhang_id >= MAX_XUNZHANG_COUNT)
	{
		return;
	}

	int next_level = m_xunzhang_param.level_list[xunzhang_id] + 1;

	const XunZhangLevelCfg *level_cfg = LOGIC_CONFIG->GetXunZhangConfig().GetXunZhangLevelCfg(xunzhang_id, next_level);
	if (NULL == level_cfg)
	{
		return;
	}

	int is_succ = 0;
	if(is_only_bind)
	{
		int curr_num = 0;
		Knapsack *knapsack = m_role->GetKnapsack();
		int valid_num = knapsack->GetCurrKnapsackValidNum();

		ItemGridData grid_data;
		for (int i = 0; i < valid_num; i++)
		{
			if (!knapsack->GetItemGridData(i, &grid_data))
			{
				continue;
			}

			if (grid_data.item_id != level_cfg->uplevel_stuff_id)
			{
				continue;
			}

			if (!grid_data.is_bind)
			{
				continue;
			}

			if (grid_data.num + curr_num < level_cfg->uplevel_stuff_num)
			{
				curr_num += grid_data.num;
				continue;
			}

			is_succ = 1;
			break;
		}

		if (is_succ == 0)
		{
			m_role->NoticeNum(errornum::EN_OTHER_XUNZHANG_STUFF_NOT_ENOUGH);
			return;
		}
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[0].item_id = level_cfg->uplevel_stuff_id;
	stuff_list[0].num = level_cfg->uplevel_stuff_num;
	stuff_list[0].buyable = false;

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
	{
		return;
	}


	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "XunZhangUpLevel"))
	{
		return;
	}

	m_xunzhang_param.level_list[xunzhang_id]++;

	this->SendAllXunZhangInfo();
	
	this->ReCalcTotalLevel();
	
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XUNZHANG);

	EventHandler::Instance().OnXunzhangUpLevel(m_role, m_total_level);
}

void XunZhangManager::ReCalcTotalLevel()
{
	m_total_level = 0;
	for (int i = 0; i < MAX_XUNZHANG_COUNT; i++)
	{
		m_total_level += m_xunzhang_param.level_list[i];
	}
	const XunZhangSuitCfg *suit_cfg = LOGIC_CONFIG->GetXunZhangConfig().GetXunZhangSuitCfg(m_total_level);
	if (NULL != suit_cfg)
	{
		m_mount_attr_add = suit_cfg->mount_attr_add;
		m_wing_attr_add = suit_cfg->wing_attr_add;
		m_halo_attr_add = suit_cfg->halo_attr_add;
		m_shengong_attr_add = suit_cfg->magic_bow_attr_add;
		m_shenyi_attr_add = suit_cfg->magic_wing_attr_add;
		m_footprint_attr_add = suit_cfg->footprint_attr_add;
	}
}
