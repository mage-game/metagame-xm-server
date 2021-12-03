#include "combineserverequip.hpp"
#include "combineserverequipconfig.hpp"

#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "protocal/msgrole.h"
#include "scene/scene.h"
#include "scene/activityshadow/activityshadow.hpp"

#include "gameworld/engineadapter.h"
#include "servercommon/errornum.h"

#include "item/itemextern.hpp"
#include "item/knapsack.h"

CombineServerActivityEquip::CombineServerActivityEquip()
{

}

CombineServerActivityEquip::~CombineServerActivityEquip()
{

}

void CombineServerActivityEquip::SetRole(Role *role)
{
	m_role = role;
}

void CombineServerActivityEquip::Init(Role *role, const CSAEquipParam &csa_equip_param)
{
	m_role = role;
	m_param = csa_equip_param;

	if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		m_param.is_active_sys = 1;
	}
}

void CombineServerActivityEquip::GetInitParam(CSAEquipParam *csa_equip_param)
{
	*csa_equip_param = m_param;
}

void CombineServerActivityEquip::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		for (int i = 0; i < CSA_EQUIP_MAX_PART; ++i)
		{
			const CSACommonEquipUplevelItemCfg *common_item_cfg = LOGIC_CONFIG->GetCSAEquipConfig().GetCommonlevelItemCfg(i, m_param.common_level_list[i]);
			if (NULL != common_item_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += common_item_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += common_item_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += common_item_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += common_item_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += common_item_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += common_item_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += common_item_cfg->jianren;

				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += common_item_cfg->extra_gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += common_item_cfg->extra_fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += common_item_cfg->extra_maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += common_item_cfg->extra_mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += common_item_cfg->extra_shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += common_item_cfg->extra_baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += common_item_cfg->extra_jianren;
			}

			const CSAGreatEquipUplevelItemCfg *great_item_cfg = LOGIC_CONFIG->GetCSAEquipConfig().GetGreatlevelItemCfg(i, m_param.great_level_list[i]);
			if (NULL != great_item_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += great_item_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += great_item_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += great_item_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += great_item_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += great_item_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += great_item_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += great_item_cfg->jianren;
// 				m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += great_item_cfg->pvp_shanghai_jiacheng;
// 				m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += great_item_cfg->pvp_shanghai_jianmian;
				m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, great_item_cfg->pvp_shanghai_jiacheng);
				m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, great_item_cfg->pvp_shanghai_jianmian);
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_CSA_EQUIP, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void CombineServerActivityEquip::OnUpCommonlevel(int part_index)
{
	if (part_index < 0 || part_index >= CSA_EQUIP_MAX_PART)
	{
		return;
	}

	const int old_level = m_param.common_level_list[part_index];

	if (old_level >= CSA_EQUIP_MAX_LEVEL || old_level >= LOGIC_CONFIG->GetCSAEquipConfig().GetCommonMaxLevel())
	{
		m_role->NoticeNum(errornum::EN_CSA_EQUIP_MAX_LEVEL);
		return;
	}

	const CSACommonEquipUplevelItemCfg *uplevel_cfg = LOGIC_CONFIG->GetCSAEquipConfig().GetCommonlevelItemCfg(part_index, old_level + 1);
	if (NULL == uplevel_cfg)
	{
		return;
	}

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	{
		short stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

		stuff_list[stuff_count].item_id = uplevel_cfg->stuff_id;
		stuff_list[stuff_count].num = uplevel_cfg->stuff_num;
		stuff_list[stuff_count].buyable = false;
		++stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list))
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}
	}

	if (!ItemExtern::ConsumeItem(m_role, consume_list, "ShenZhangUplevel"))
	{
		return;
	}

	++m_param.common_level_list[part_index];

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CSA_EQUIP);
	this->SyncCSAEquipInfo();
}

void CombineServerActivityEquip::OnUpGreatlevel(int part_index)
{
	if (part_index < 0 || part_index >= CSA_EQUIP_MAX_PART)
	{
		return;
	}

	const int old_level = m_param.great_level_list[part_index];

	if (old_level >= CSA_EQUIP_MAX_LEVEL || old_level >= LOGIC_CONFIG->GetCSAEquipConfig().GetGreatMaxLevel())
	{
		m_role->NoticeNum(errornum::EN_CSA_EQUIP_MAX_LEVEL);
		return;
	}

	const CSAGreatEquipUplevelItemCfg *uplevel_cfg = LOGIC_CONFIG->GetCSAEquipConfig().GetGreatlevelItemCfg(part_index, old_level + 1);
	if (NULL == uplevel_cfg)
	{
		return;
	}

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	{
		short stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

		stuff_list[stuff_count].item_id = uplevel_cfg->stuff_id;
		stuff_list[stuff_count].num = uplevel_cfg->stuff_num;
		stuff_list[stuff_count].buyable = false;
		++stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list))
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}
	}

	if (consume_list.count > 0 && !m_role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "ShenZhangUplevel"))
	{
		return;
	}

	++m_param.great_level_list[part_index];
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CSA_EQUIP);

	if (this->CheckActiveSpecial())
	{
		this->SyncCSAEquipActiveSpecial();
	}
	this->SyncCSAEquipInfo();
}

void CombineServerActivityEquip::SyncCSAEquipInfo()
{
	static Protocol::SCCSAEquipInfo pro;
	UNSTD_STATIC_CHECK(sizeof(pro.common_level_list) == sizeof(m_param.common_level_list));
	UNSTD_STATIC_CHECK(sizeof(pro.great_level_list) == sizeof(m_param.great_level_list));
	memcpy(pro.common_level_list, m_param.common_level_list, sizeof(pro.common_level_list));
	memcpy(pro.great_level_list, m_param.great_level_list, sizeof(pro.great_level_list));
	pro.active_special = m_param.active_special;
	pro.is_active_sys = m_param.is_active_sys;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

bool CombineServerActivityEquip::CheckActiveSpecial()
{
	if (1 == m_param.active_special)
	{
		return false;
	}

	for (int i = 0; i < CSA_EQUIP_MAX_PART; i++)
	{
		if (0 == m_param.great_level_list[i])
		{
			return false;
		}
	}

	m_param.active_special = 1;
	return true;
}

void CombineServerActivityEquip::SyncCSAEquipActiveSpecial()
{
	static Protocol::SCCSAActivePower pro;
	pro.obj_id = m_role->GetId();
	pro.csa_active_special = m_param.active_special;
	m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void*)&pro, sizeof(pro));
}