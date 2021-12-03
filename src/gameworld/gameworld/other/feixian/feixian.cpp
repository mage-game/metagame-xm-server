#include <algorithm>

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"

#include "globalconfig/globalconfig.h"

#include "engineadapter.h"
#include "feixian.h"

#include "protocal/msgrole.h"

#include "item/knapsack.h"
#include "item/itempool.h"

#include "scene/scenemanager.h"

#include "gamelog.h"

#include "obj/character/role.h"

#include "other/event/eventhandler.hpp"

#include "config/logicconfigmanager.hpp"

#include "reallyequipconfig.hpp"
#include "other/sublock/sublock.hpp"
#include "equipment/equipmentmanager.h"
#include "protocal/msgfeixian.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////
FeixianManager::FeixianManager()
	: m_role(NULL)
{
	memset(m_xianpin_value_list, 0, sizeof(m_xianpin_value_list));
}

FeixianManager::~FeixianManager()
{

}

void FeixianManager::SetRole(Role *role)
{
	m_role = role;
}

void FeixianManager::Init(Role *role, const FeiXianParam &param)
{
	m_role = role;
	m_feixianparam = param;
}

void FeixianManager::GetInitParam(FeiXianParam *param)
{
	*param = m_feixianparam;
}

void FeixianManager::Update(time_t now_second)
{

}

int FeixianManager::GetEquipIndexByType(int equip_type)
{
// 	switch (equip_type)
// 	{
// 	case Equipment::E_TYPE_FEIXIAN_WUQI:
// 		return FeiXianParam::E_INDEX_WUQI;
// 	case Equipment::E_TYPE_FEIXIAN_YIFU:
// 		return FeiXianParam::E_INDEX_YIFU;
// 	case Equipment::E_TYPE_FEIXIAN_HUSHOU:
// 		return FeiXianParam::E_INDEX_HUSHOU;
// 	case Equipment::E_TYPE_FEIXIAN_YAODAI:
// 		return FeiXianParam::E_INDEX_YAODAI;
//	case Equipment::E_TYPE_FEIXIAN_HUTUI:
//		return FeiXianParam::E_INDEX_HUTUI;
// 	case Equipment::E_TYPE_FEIXIAN_TOUKUI:
// 		return FeiXianParam::E_INDEX_TOUKUI;
// 	case Equipment::E_TYPE_FEIXIAN_XIANGLIAN:
// 		return FeiXianParam::E_INDEX_XIANGLIAN;
// 	case Equipment::E_TYPE_FEIXIAN_SHOUZHUO:
// 		return FeiXianParam::E_INDEX_SHOUZHUO;
// 	case Equipment::E_TYPE_FEIXIAN_JIEZHI:
// 		return FeiXianParam::E_INDEX_JIEZHI;
// 	case Equipment::E_TYPE_FEIXIAN_XIEZI:
// 		return FeiXianParam::E_INDEX_XIEZI;
// 	case Equipment::E_TYPE_FEIXIAN_YUPEI:
// 		return FeiXianParam::E_INDEX_YUPEI;
// 	default: return -1;
// 	}

	return -1;
}		

int FeixianManager::PutOn(const ItemDataWrapper &item_wrapper)
{
// 	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_wrapper.item_id);
// 	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()) return -1;
// 	
// 	if (0 == item_wrapper.has_param) return -2;
// 
// 	short equip_type = equip->GetEquipType();
// 	if (Equipment::E_TYPE_FEIXIAN_MIN > equip_type || Equipment::E_TYPE_FEIXIAN_MAX <= equip_type) return -3;
// 
// 	int real_index = GetEquipIndexByType(equip_type);
// 	if (real_index < FeiXianParam::E_INDEX_WUQI || real_index >= FeiXianParam::E_INDEX_MAX) return -4;
// 
// 
// 	ItemDataWrapper &itemdata = m_feixianparam.equip_list[real_index];
// 	if (!itemdata.Invalid()) // 该位置有装备 脱之
// 	{
// 		int ret = this->TakeOff(real_index, true);
// 		if (0 != ret) 
// 		{
// 			return -4;
// 		}
// 	}
// 
// 	if (!itemdata.Invalid() || item_wrapper.Invalid())	// 最后再加强检查
// 	{
// 		return -5;
// 	}
// 
// 	itemdata.Reset(item_wrapper);
// 
// 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FEIXIAN);
// 	this->SyncEquipFeixianInfo();
// 
// 	EventHandler::Instance().OnPutonEquip(m_role, equip_type, equip->GetColor(), item_wrapper.param_data.quality, equip->GetLimitLevel(), 0);

	return 0;
}
int FeixianManager::TakeOff(short equip_index, bool is_from_puton)
{
// 	if (equip_index < FeiXianParam::E_INDEX_WUQI || equip_index >= FeiXianParam::E_INDEX_MAX) return -1;
// 
// 	ItemDataWrapper &itemdata = m_feixianparam.equip_list[equip_index];
// 	if (itemdata.Invalid()) return -2;
// 
// 	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);
// 	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() ||
// 		equip->GetEquipType() < Equipment::E_TYPE_FEIXIAN_MIN || equip->GetEquipType() >= Equipment::E_TYPE_FEIXIAN_MAX)
// 	{
// 		return -3;
// 	}
// 
// 	Knapsack *knapsack = m_role->GetKnapsack();
// 	if (!knapsack->HaveSpace())
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 		return -5;
// 	}
// 
// 	if (!knapsack->Put(itemdata, PUT_REASON_NO_NOTICE))
// 	{
// 		return -6;
// 	}
// 	 
// 	itemdata.Reset();
// 
// 	if (!is_from_puton)
// 	{
// 		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FEIXIAN);
// 	}
// 	this->SyncEquipFeixianInfo();
// 
// 	EventHandler::Instance().OnTakeOffEquip(m_role);

	return 0;
}

void FeixianManager::SyncEquipFeixianInfo()
{
	static Protocol::SCFeixianEquipInfo eia;

	int count = 0;

	for (int i = 0; i < FeiXianParam::E_INDEX_MAX; ++i)
	{
		if (m_feixianparam.equip_list[i].Invalid()) continue;

		eia.equip_list[count].Reset(m_feixianparam.equip_list[i]);

		count++;
	}

	//unsigned int sendlen = sizeof(eia) - sizeof(eia.equip_list[0]) * (ItemNamespace::MAX_EQUIPMENT_GRID_NUM - count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&eia, sizeof(eia));
}

void FeixianManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
// 	if (is_recalc)
// 	{
// 		m_attrs_add.Reset();
// 
// 		// 装备属性
// 		{
// 			for (int equip_index = 0; equip_index < FeiXianParam::E_INDEX_MAX; ++equip_index)
// 			{
// 				ItemDataWrapper &itemdata = m_feixianparam.equip_list[equip_index];
// 				if (!itemdata.Invalid())
// 				{
// 					const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);
// 
// 					if (NULL != equip && ItemBase::I_TYPE_EQUIPMENT == equip->GetItemType() && Equipment::E_TYPE_FEIXIAN_MIN <= equip->GetEquipType() && Equipment::E_TYPE_FEIXIAN_MAX > equip->GetEquipType())
// 					{
// 						const ComposeListConfig *pComposeListCfg = LOGIC_CONFIG->GetReallyequipComposeCfg().GetCompostListCfg(itemdata.item_id);
// 						float fAdd = 0.0f;
// 						if (pComposeListCfg != NULL)
// 						{
// 							fAdd = static_cast<float>(pComposeListCfg->shuxing / 100);
// 						}
// 						CharIntAttrs base_attrs;
// 						equip->GetBaseAttr(base_attrs);
// 						for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++i)
// 						{
// 							if (base_attrs.m_attrs[i] > 0)
// 							{
// 								m_attrs_add.m_attrs[i] += static_cast<Attribute>(base_attrs.m_attrs[i] * (1 + fAdd));
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 		
// 		this->ReCalcXianpinAttr(m_attrs_add, m_role->GetPutOnLevel());
// 	}
// 
// 	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_FEIXIAN, m_attrs_add);
// 
// 	base_add.Add(m_attrs_add.m_attrs);
// 	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

void FeixianManager::ReCalcXianpinTotalStar(int &xianpin_total_star)
{
	int total_star = 0;

	for (int i = 0; i < FeiXianParam::E_INDEX_MAX; ++i)
	{
		int total_capbility = 0;

		ItemDataWrapper &itemdata = m_feixianparam.equip_list[i];
		if (itemdata.Invalid())
		{
			continue;
		}

		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);
		if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
		{
			continue;
		}

		int attr_type = 0;

		for (int k = 0; k < ItemNamespace::MAX_XIANPIN_NUM; ++k)
		{
			attr_type = itemdata.param_data.xianpin_type_list[k];
			const EquipXianPinConfig *xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(attr_type);
			if (NULL == xianpin_cfg)
			{
				continue;
			}

			total_capbility += xianpin_cfg->capability;
		}

		if (total_capbility > 0)
		{
			int xianpin_star_level = GLOBALCONFIG->GetEquipConfig().GetXianPinStarLevel(total_capbility);

			total_star += xianpin_star_level;
		}
	}

	xianpin_total_star = total_star;
}

void FeixianManager::ReCalcXianpinAttr(CharIntAttrs &attrs_add, int puton_level)
{
	memset(m_xianpin_value_list, 0, sizeof(m_xianpin_value_list));

	for (int i = 0; i < FeiXianParam::E_INDEX_MAX; ++i)
	{
		ItemDataWrapper &itemdata = m_feixianparam.equip_list[i];
		if (itemdata.Invalid())
		{
			continue;
		}

		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);
		if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
		{
			continue;
		}

		if (puton_level < equip->GetLimitLevel())
		{
			continue;
		}

		int attr_type = 0;
		Attribute attr_value = 0;

		for (int k = 0; k < ItemNamespace::MAX_XIANPIN_NUM; ++k)
		{
			attr_type = itemdata.param_data.xianpin_type_list[k];
			const EquipXianPinConfig *xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(attr_type);
			if (NULL == xianpin_cfg)
			{
				continue;
			}
			attr_value = xianpin_cfg->add_value;

			m_xianpin_value_list[xianpin_cfg->shuxing_type] += attr_value;
		}
	}

	for (int shuxing_type = Equipment::E_XIANPIN_ADD_BASE_MAXHP_PER; shuxing_type <= Equipment::E_XIANPIN_MAX; ++shuxing_type)
	{
		switch (shuxing_type)
		{
			// +X%气血上限
		case Equipment::E_XIANPIN_ADD_BASE_MAXHP_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_MAXHP_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += m_xianpin_value_list[shuxing_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +X%攻击力
		case Equipment::E_XIANPIN_ADD_BASE_GONGJI_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_GONGJI_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += m_xianpin_value_list[shuxing_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +X%防御力
		case Equipment::E_XIANPIN_ADD_BASE_FANGYU_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_FANGYU_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += m_xianpin_value_list[shuxing_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +X%命中
		case Equipment::E_XIANPIN_ADD_BASE_MINGZHAONG_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_MINGZHAONG_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += m_xianpin_value_list[shuxing_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +X%闪避
		case Equipment::E_XIANPIN_ADD_BASE_SHANBI_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_SHANBI_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += m_xianpin_value_list[shuxing_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +X%暴击
		case Equipment::E_XIANPIN_ADD_BASE_BAOJI_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_BAOJI_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += m_xianpin_value_list[shuxing_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +X%坚韧
		case Equipment::E_XIANPIN_ADD_BASE_JIANREN_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_JIANREN_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += m_xianpin_value_list[shuxing_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +角色等级*X气血上限
		case Equipment::E_XIANPIN_ADD_BASE_MAXHP_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_MAXHP_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[shuxing_type]);
				}
			}
			break;

			// +角色等级*X攻击力
		case Equipment::E_XIANPIN_ADD_BASE_GONGJI_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_GONGJI_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[shuxing_type]);
				}
			}
			break;

			// +角色等级*X防御力
		case Equipment::E_XIANPIN_ADD_BASE_FANGYU_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_FANGYU_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[shuxing_type]);
				}
			}
			break;

			// +角色等级*X命中
		case Equipment::E_XIANPIN_ADD_BASE_MINGZHONG_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_MINGZHONG_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[shuxing_type]);
				}
			}
			break;

			// +角色等级*X闪避
		case Equipment::E_XIANPIN_ADD_BASE_SHANBI_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_SHANBI_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[shuxing_type]);
				}
			}
			break;

			// +角色等级*X暴击
		case Equipment::E_XIANPIN_ADD_BASE_BAOJI_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_BAOJI_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[shuxing_type]);
				}
			}
			break;

			// +角色等级*X坚韧
		case Equipment::E_XIANPIN_ADD_BASE_JIANREN_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_JIANREN_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[shuxing_type]);
				}
			}
			break;
		}
	}

	for (int i = 0; i < FeiXianParam::E_INDEX_MAX; ++i)
	{
		ItemDataWrapper &itemdata = m_feixianparam.equip_list[i];
		if (itemdata.Invalid())
		{
			continue;
		}

		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);
		if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
		{
			continue;
		}

		if (puton_level < equip->GetLimitLevel())
		{
			continue;
		}

		CharIntAttrs equip_base_attrs;
		equip->GetBaseAttr(equip_base_attrs);

		if (equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] > 0 && m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_GONGJI_PER] > 0) // 所有装备基础攻击增加  X %
		{
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_GONGJI_PER] / 10000.0));
		}

		if (equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] > 0 && m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_FANGYU_PER] > 0) // 所有装备基础防御增加  X %
		{
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_FANGYU_PER] / 10000.0));
		}

		if (equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] > 0 && m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_MAXHP_PER] > 0) // 所有装备基础气血增加  X %
		{
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_MAXHP_PER] / 10000.0));
		}
	}
}

void FeixianManager::EquipXianPinFixValue(int attr_type, int &fix_value)
{
	if (attr_type <= Equipment::E_XIANPIN_INVALID || attr_type >= Equipment::E_XIANPIN_MAX)
	{
		return;
	}

	Attribute attr_value = m_xianpin_value_list[attr_type];

	if (Equipment::E_XIANPIN_ADD_MONSTER_DROP_COIN_PER == attr_type && attr_value > 0)					// 野怪掉落金币增加  X %  （对副本、活动内的怪物无效）
	{
		fix_value = static_cast<int>(fix_value * (1 + attr_value / 10000.0));
	}
	else if (Equipment::E_XIANPIN_ADD_DAMAGE_TO_MONSTER_OF_ROLE_LEVEL == attr_type && attr_value > 0)	// 对怪物攻击额外造成（等级 * X）点伤害
	{
		fix_value = static_cast<int>(fix_value + m_role->GetLevel() * attr_value);
	}
	else if (Equipment::E_XIANPIN_DEC_DAMAGE_FROM_MONSTER_OF_ROLE_LEVEL == attr_type && attr_value > 0)	// 怪物对玩家伤害额外减少（等级 * X）点伤害
	{
		fix_value = static_cast<int>(fix_value - m_role->GetLevel() * attr_value);
	}
	else if (Equipment::E_XIANPIN_DEC_DAMAGE_FROM_ROLE_PER == attr_type && attr_value > 0)				// 被玩家造成伤害减少 X % （玩家造成的最终伤害 * （1 - X %）才是最终伤害）
	{
		fix_value = static_cast<int>(fix_value * (1 - attr_value / 10000.0));
	}
	else if (Equipment::E_XIANPIN_ADD_DAMAGE_TO_ROLE_PER == attr_type && attr_value > 0)					// 对玩家造成伤害增加 X % （玩家造成的最终伤害 * （1 + X %）才是最终伤害）
	{
		fix_value = static_cast<int>(fix_value * (1 + attr_value / 10000.0));
	}
	else if (Equipment::E_XIANPIN_ADD_MONSTER_DROP_EXP_PER == attr_type && attr_value > 0)				// 击杀野怪经验额外增加  X %
	{
		fix_value = static_cast<int>(fix_value * (1 + attr_value / 10000.0));
	}
}

bool FeixianManager::AttachAttr(const ItemGridData &item_data, CharIntAttrs &base_add, int puton_level, int &flush_maxhp_count, int &flush_gongji_count)
{
	if (item_data.Invalid()) return false;

	const EquipmentParam *param = (const EquipmentParam *)item_data.param;
	if (NULL == param) return false;

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_data.item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT) return false;

	// 基础与强化属性
	if (puton_level >= equip->GetLimitLevel())
	{
		equip->GetAttr(param, base_add, m_role->GetProf());
	}

	return true;
}

void FeixianManager::OnRoleLevelChange()
{
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FEIXIAN);
}

bool FeixianManager::GetRandomSpeicalAttr(ItemGridData *item_grid_data)
{
	if (NULL == item_grid_data)
	{
		return false;
	}

	const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_grid_data->item_id);
	if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType())
	{
		return false;
	}

	if (equip_item->GetEquipType() < Equipment::E_TYPE_MAX)
	{
		return GLOBALCONFIG->GetEquipConfig().GetRandomXianpin(item_grid_data);
	}

	return false;
}

bool FeixianManager::GetFixSpecialAttr(ItemGridData *item_grid_data, int getway, int getway_param1)
{
	if (NULL == item_grid_data)
	{
		return false;
	}

	const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_grid_data->item_id);
	if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType())
	{
		return false;
	}

	if (equip_item->GetEquipType() >= Equipment::E_TYPE_MIN && equip_item->GetEquipType() < Equipment::E_TYPE_MAX)
	{
		return GLOBALCONFIG->GetEquipConfig().GetRandomXianpin(item_grid_data, getway, getway_param1);
	}

	return false;
}

int FeixianManager::ComposeEquipInKnapsack(short index1, short index2)
{
	// 检查背包是否锁住
	if (!CheckKnapsack())
	{
		return -1;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (NULL == knapsack)
	{
		return -2;
	}

	bool bind = false;

	// 装备1
	ItemGridData equip_data;
	EquipmentParam *equip_param = NULL;
	{
		int ret1 = CheckEquipInKnapsack(index1, &equip_data, false);
		equip_param = dynamic_cast<EquipmentParam*>(equip_data.param);
		if (NULL == equip_param)
		{
			m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
			return -13;
		}
		if (ret1 < 0)
		{
			return ret1;
		}
		if (equip_data.is_bind) bind = true;
	}

	// 装备2
	ItemGridData equip_data2;
	EquipmentParam *equip2_param = NULL;
	{
		int ret2 = CheckEquipInKnapsack(index2, &equip_data2, false);
		equip2_param = dynamic_cast<EquipmentParam*>(equip_data2.param);
		if (NULL == equip_param)
		{
			m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
			return -13;
		}
		if (ret2 < 0)
		{
			return ret2;
		}
		
		if (equip_data2.is_bind) bind = true;
	}

	if (equip_data.item_id != equip_data2.item_id)
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_COMP_DIFF);
		return -21;
	}

	const ComposeListConfig *pComposeListCfg = LOGIC_CONFIG->GetReallyequipComposeCfg().GetCompostListCfg(equip_data.item_id);
	if (pComposeListCfg == NULL)
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
		return -30;
	}

	// 成功
	ReCalcXianpinParam(equip_param, equip2_param);

	// 装备1扣除
	ItemExtern::ItemConsumeList consume_list;
	consume_list.count = 1;
	consume_list.index_list[0] = index1;
	consume_list.num_list[0] = 1;
	knapsack->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "FeixianCompose");
	// 装备合成 增加一个真属性
	equip2_param->really = 1;
	equip_data2.is_bind = bind;
	knapsack->SetItemDirty(index2);

	return 0;
}

int FeixianManager::ComposeEquipInBody(short index1, short index2)
{
	// 检查背包
	Knapsack *knapsack = m_role->GetKnapsack();
	if (!CheckKnapsack())
	{
		return -1;
	}
	if (NULL == knapsack)
	{
		return -2;
	}

	// 检查身上的装备
	int res = CheckEquipInBody(index1, false);
	if (res < 0)
	{
		return res;
	}

	// 检查背包里的装备
	bool bind = false;
	ItemGridData equip_data2;
	EquipmentParam *equip2_param = NULL;
	int ret = CheckEquipInKnapsack(index2, &equip_data2, false);
	equip2_param = dynamic_cast<EquipmentParam*>(equip_data2.param);
	if (NULL == equip2_param)
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
		return -13;
	}
	if (ret < 0)
	{
		return ret;
	}
	if (m_feixianparam.equip_list[index1].item_id != equip_data2.item_id)
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_COMP_DIFF);
		return -35;
	}
	if (equip_data2.is_bind)
	{
		bind = true;
	}

	const ComposeListConfig *pComposeListCfg = LOGIC_CONFIG->GetReallyequipComposeCfg().GetCompostListCfg(equip_data2.item_id);
	if (pComposeListCfg == NULL)
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
		return -30;
	}

	// 成功
	ReCalcXianpinParamInBody(m_feixianparam.equip_list[index1], equip2_param);

	// 装备2扣除
	ItemExtern::ItemConsumeList consume_list;
	consume_list.count = 1;
	consume_list.index_list[0] = index2;
	consume_list.num_list[0] = 1;
	knapsack->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "FeixianCompose");

	// 装备合成 增加一个真属性
	m_feixianparam.equip_list[index1].param_data.really = 1;
	m_feixianparam.equip_list[index1].is_bind = bind;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FEIXIAN);
	this->SyncEquipFeixianInfo();

	return 0;
}


int FeixianManager::LevelUpInKnapsack(short index1, int& itemindex)
{
	// 检查背包
	Knapsack *knapsack = m_role->GetKnapsack();
	if (!CheckKnapsack())
	{
		return -1;
	}
	if (NULL == knapsack)
	{
		return -2;
	}

	// 检查背包里的装备
	ItemGridData equip_data;
	EquipmentParam* equip_param = NULL;
	int ret2 = CheckEquipInKnapsack(index1, &equip_data, true);
	equip_param = dynamic_cast<EquipmentParam*>(equip_data.param);
	if (NULL == equip_param)
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
		return -13;
	}
	if (ret2 < 0)
	{
		return ret2;
	}

	// 消耗物品
	const LevelupListConfig* pLevelUpListCfg = LOGIC_CONFIG->GetReallyequipComposeCfg().GetLevelupListCfg((unsigned int)equip_data.item_id);
	const Equipment* equip = NULL;
	int ret3 = LevelUpConsumeItem(equip_data.item_id, pLevelUpListCfg);
	if (ret3 < 0)
	{
		return ret3;
	}
	equip = (const Equipment *)ITEMPOOL->GetItem(pLevelUpListCfg->dirid);
	if (equip == NULL)
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
		return -96;
	}

	// 仙品属性类型
	unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];
	for (unsigned short i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; ++i)
	{
		if (equip_param->xianpin_type_list[i] > 0)
		{
			xianpin_type_list[i] = equip_param->xianpin_type_list[i];
		}
	}

	// 扣除原来的装备
	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();
	consume_list.count = 1;
	consume_list.index_list[0] = index1;
	consume_list.num_list[0] = 1;
	knapsack->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "FeixianCompose");

	// 生成新装备并加到背包里，物品在背包索索引为grid_index
	bool bind = equip_data.is_bind ? true : false;
	int grid_index = -1;
	if (!knapsack->Put(ItemConfigData(pLevelUpListCfg->dirid, bind, 1), PUT_REASON_FEIXIAN_UP, &grid_index))
	{
		gamelog::g_log_item.printf(LL_INFO, "[FeixianManager::LevelUp FAILED][USER %s, %d][item %d]", m_role->GetName(), UidToInt(m_role->GetUserId()), equip_data.item_id);
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_UP_ERR);
		return -11;
	}

	// 新装备保留原来的仙品属性
	if (ItemBase::I_TYPE_EQUIPMENT == equip->GetItemType())
	{
		if (grid_index >= 0)
		{
			ItemGridData item_grid_data;
			m_role->GetKnapsack()->GetItemGridData(grid_index, &item_grid_data);
			if (item_grid_data.param == NULL)
			{
				gamelog::g_log_item.printf(LL_INFO, "[FeixianManager::LevelUp ATT ERROR][USER %s, %d][item %d]", m_role->GetName(), UidToInt(m_role->GetUserId()), equip_data.item_id);
				m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_UP_ERR);
				return -12;
			}
			EquipmentParam* equip_param = dynamic_cast<EquipmentParam*>(equip_data.param);
			memset(equip_param->xianpin_type_list, 0, sizeof(equip_param->xianpin_type_list));
			for (unsigned short i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; ++i)
			{
				equip_param->xianpin_type_list[i] = xianpin_type_list[i];
			}
			// 根据天神属性给红妆增加多一条仙品属性
			const EquipXianPinConfig* xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(equip->GetGodAttr());
			if (NULL == xianpin_cfg)
			{
				m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
				return -10;
			}
			equip_param->xianpin_type_list[XIANPIN_SHUXING_NUM_MAX] = (unsigned short)xianpin_cfg->xianpin_type;
			m_role->GetKnapsack()->SetItemDirty(grid_index);
			itemindex = grid_index;
		}
	}

	return 0;
}

// 在身上升级装备
int FeixianManager::LevelUpInBody(short index1)
{
	// 检查背包锁情况
	if (!CheckKnapsack())
	{
		return -1;
	}

	// 检查身上的装备
	int ret1 = CheckEquipInBody(index1);
	if (ret1 < 0)
	{
		return ret1;
	}

	// 消耗物品
	const LevelupListConfig *pLevelUpListCfg = LOGIC_CONFIG->GetReallyequipComposeCfg().GetLevelupListCfg((unsigned int)m_feixianparam.equip_list[index1].item_id);
	const Equipment *equip = NULL;
	int ret2 = LevelUpConsumeItem(m_feixianparam.equip_list[index1].item_id, pLevelUpListCfg);
	if (ret2 < 0)
	{
		return ret2;
	}
	equip = (const Equipment *)ITEMPOOL->GetItem(pLevelUpListCfg->dirid);
	if (equip == NULL)
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
		return -96;
	}

	// 升级成功后增加属性
	const EquipXianPinConfig* xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(equip->GetGodAttr());
	if (NULL == xianpin_cfg)
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
		return -94;
	}
	m_feixianparam.equip_list[index1].item_id = pLevelUpListCfg->dirid;
	m_feixianparam.equip_list[index1].param_data.xianpin_type_list[XIANPIN_SHUXING_NUM_MAX] = xianpin_cfg->xianpin_type;
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FEIXIAN);

	SyncEquipFeixianInfo();
	return 0;
}

void FeixianManager::ReCalcXianpinParam(EquipmentParam* param1, EquipmentParam* param2)
{
	// 先把两个装备属性加到一起再进行排序
	// 先代码写死3，后面需要改再来改动
	if (NULL == param1 || NULL == param2)
	{
		return;
	}
	std::vector<const EquipXianPinConfig*> cfg_vec;
	cfg_vec.clear();
	for (int i = 0; i < XIANPIN_SHUXING_NUM_MAX; i++)
	{
		const EquipXianPinConfig* equip_cfg1 = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(param1->xianpin_type_list[i]);
		if (NULL != equip_cfg1)
		{
			cfg_vec.push_back(equip_cfg1);
		}

		const EquipXianPinConfig* equip_cfg2 = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(param2->xianpin_type_list[i]);
		if (NULL != equip_cfg2)
		{
			cfg_vec.push_back(equip_cfg2);
		}
	}

	// 排序
	int xp_type = 0;
	std::sort(cfg_vec.begin(), cfg_vec.end(), Compare);
	for (std::vector<const EquipXianPinConfig*>::iterator iter = cfg_vec.begin(); iter != cfg_vec.end(); iter++)
	{
		if (xp_type >= XIANPIN_SHUXING_NUM_MAX)
		{
			break;
		}
		param2->xianpin_type_list[xp_type] = (unsigned short)(*iter)->xianpin_type;
		xp_type++;
	}
}

bool FeixianManager::FindFeixianEquipByIndex(short index)
{
	if (index < FeiXianParam::E_INDEX_WUQI || index >= FeiXianParam::E_INDEX_MAX)
	{
		return false;
	}

	return true;
}

void FeixianManager::ReCalcXianpinParamInBody(ItemDataWrapper& param1, EquipmentParam* param2)
{
	if (param1.item_id <= 0 || NULL == param2)
	{
		return;
	}
	std::vector<const EquipXianPinConfig*> cfg_vec;
	cfg_vec.clear();
	// 先代码写死3，后面需要改再来改动
	for (int i = 0; i < XIANPIN_SHUXING_NUM_MAX; i++)
	{
		const EquipXianPinConfig* equip_cfg1 = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(param1.param_data.xianpin_type_list[i]);
		if (NULL != equip_cfg1)
		{
			cfg_vec.push_back(equip_cfg1);
		}

		const EquipXianPinConfig* equip_cfg2 = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(param2->xianpin_type_list[i]);
		if (NULL != equip_cfg2)
		{
			cfg_vec.push_back(equip_cfg2);
		}
	}

	int xp_type = 0;
	std::sort(cfg_vec.begin(), cfg_vec.end(), Compare);
	for (std::vector<const EquipXianPinConfig*>::iterator iter = cfg_vec.begin(); iter != cfg_vec.end(); iter++)
	{
		if (xp_type >= XIANPIN_SHUXING_NUM_MAX)
		{
			break;
		}
		param1.param_data.xianpin_type_list[xp_type] = (unsigned short)(*iter)->xianpin_type;
		xp_type++;
	}
}

bool FeixianManager::Compare(const EquipXianPinConfig* a, const EquipXianPinConfig* b)
{
	return a->score > b->score;
}


// 检查背包锁的情况
bool FeixianManager::CheckKnapsack()
{
	if (m_role->GetSublock()->CheckHasLock())
	{
		return false;
	}
	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	return true;
}

// 检查装备在背包里的情况
int FeixianManager::CheckEquipInKnapsack(short index, ItemGridData* equip_data, bool levelup)
{
	Knapsack *knapsack = m_role->GetKnapsack();
	if (!knapsack->GetItemGridData((int)index, equip_data) || equip_data->Invalid())
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_NOEXIT);
		return -10;
	}

	const ItemBase* equip_base = ITEMPOOL->GetItem(equip_data->item_id);
	if (NULL == equip_base || equip_base->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_NOEXIT);
		return -11;
	}
	if (ItemBase::I_COLOR_ORANGE != equip_base->GetColor())
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_COMP_COLORERR);
		return -12;
	}
	EquipmentParam* equip_param = dynamic_cast<EquipmentParam*>(equip_data->param);
	if (NULL == equip_param)
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
		return -13;
	}
	if (levelup)
	{
		if (!equip_param->really)
		{
			m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
			return -6;
		}
	}
	else
	{
		if (equip_param->really)
		{
			m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_COMP_TWICE);
			return -14;
		}
		if (equip_param->xianpin_star_num < FeixianManager::STAR_COUNT_LIMIT)
		{
			m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_COMP_STARERR);
			return -15;
		}
	}

	return 0;
}

// 检查装备在身上的情况
int FeixianManager::CheckEquipInBody(short index,bool levelquip)
{
	if (!FindFeixianEquipByIndex(index))
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_NOEXIT);
		return -9;
	}

	if (levelquip)
	{
		if (!m_feixianparam.equip_list[index].param_data.really)
		{
			m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
			return -99;
		}
	}
	else
	{
		if (m_feixianparam.equip_list[index].param_data.really)
		{
			m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_COMP_TWICE);
			return -33;
		}
	}

	if (m_feixianparam.equip_list[index].param_data.xianpin_star_num < FeixianManager::STAR_COUNT_LIMIT)
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_COMP_STARERR);
		return -34;
	}

	return 0;
}

// 消耗升级物品
int FeixianManager::LevelUpConsumeItem(ItemID id, const LevelupListConfig* pLevelUpListCfg)
{
	if (pLevelUpListCfg == NULL)
	{
		m_role->NoticeNum(errornum::EN_FEIXIAN_EQUIP_TYPEERR);
		return -97;
	}
	// 消耗
	int stuff_count = 0;
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	UNSTD_STATIC_CHECK(ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM >= 4);
	if (ITEMPOOL->GetItem(pLevelUpListCfg->stuff_id_1) && pLevelUpListCfg->stuff_count_1 > 0)
	{
		stuff_list[stuff_count].item_id = pLevelUpListCfg->stuff_id_1;
		stuff_list[stuff_count].num = pLevelUpListCfg->stuff_count_1;
		stuff_list[stuff_count].buyable = false;
		++stuff_count;
	}
	if (ITEMPOOL->GetItem(pLevelUpListCfg->stuff_id_2) && pLevelUpListCfg->stuff_count_2 > 0)
	{
		stuff_list[stuff_count].item_id = pLevelUpListCfg->stuff_id_2;
		stuff_list[stuff_count].num = pLevelUpListCfg->stuff_count_2;
		stuff_list[stuff_count].buyable = false;
		++stuff_count;
	}
	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();
	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list))
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return -95;
	}

	return 0;
}