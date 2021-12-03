#include <assert.h>

#include "equipment.h"
#include "equipmentmanager.h"
#include "other/jingling/jinglingmanager.h"

#include "obj/character/role.h"
#include "servercommon/errornum.h"

#include "globalconfig/globalconfig.h"
#include "protocal/msgequipment.h"

#include "engineadapter.h"

#include "item/itempool.h"
#include "global/globalrole/globalrolemanager.hpp"
#include "other/qingyuan/qingyuan.hpp"
#include "global/globalrole/qingyuanglobal/qingyuanglobal.hpp"
#include "other/zhuanshengequip/zhuanshengequip.hpp"
#include "other/impguard/impguard.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"
#include "other/lieming/liemingmanager.h"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/shenshou/shenshou.hpp"
#include "other/greatesoldier/greatesoldier.hpp"
#include "other/baizhanequip/baizhanequip.h"
#include "other/crossequip/crossequip.hpp"

EquipmentParam::EquipmentParam() : ItemParam(ItemBase::I_TYPE_EQUIPMENT), quality(0),strengthen_level(0), shen_level(0),
	fuling_level(0), star_level(0), has_lucky(0), fumo_id(0), xianpin_equip(0), param1(0), param2(0), param3(0),
	rand_attr_type_1(0),rand_attr_type_2(0),rand_attr_type_3(0),reserve_type(0),rand_attr_val_1(0),rand_attr_val_2(0),
	rand_attr_val_3(0),rand_attr_val_reserve(0),really(0),xianpin_star_num(0)
{
	memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
}

bool EquipmentParam::SetInStructData(ItemParamDataStruct *equip_data) const
{
	if (NULL == equip_data) return false;

	equip_data->quality = quality;
	equip_data->strengthen_level = strengthen_level;
	equip_data->shen_level = shen_level;
	equip_data->fuling_level = fuling_level;
	equip_data->star_level = star_level;
	equip_data->has_lucky = has_lucky;
	equip_data->fumo_id = fumo_id;
	equip_data->xianpin_equip = xianpin_equip;

	memcpy(equip_data->xianpin_type_list, xianpin_type_list, sizeof(equip_data->xianpin_type_list));

	equip_data->param1 = param1;
	equip_data->param2 = param2;
	equip_data->param3 = param3;

	equip_data->rand_attr_type_1 =  rand_attr_type_1;										
	equip_data->rand_attr_type_2 = 	rand_attr_type_2;							
	equip_data->rand_attr_type_3 =	rand_attr_type_3;									
	equip_data->reserve_type = reserve_type;

	equip_data->rand_attr_val_1 = rand_attr_val_1;										
	equip_data->rand_attr_val_2 = rand_attr_val_2;							
	equip_data->rand_attr_val_3 = rand_attr_val_3;											
	equip_data->rand_attr_val_reserve = rand_attr_val_reserve;

	memcpy(equip_data->jing_ling_skill_list, jing_ling_skill_list, sizeof(equip_data->jing_ling_skill_list));
	equip_data->really = really;
	equip_data->xianpin_star_num = xianpin_star_num;

	return true;
}

//////////////////////////////////////////////////////////////////////////

Equipment::Equipment(short equip_type)
	: ItemBase(I_TYPE_EQUIPMENT),
	m_equipment_type(equip_type),
	//m_quality(0),
	m_can_strengthen(false),
	m_can_flush(false),
	m_is_shen(0),
	m_hole_num(0),
	m_suit_id(0),
	m_act_shizhung_type(0),
	m_order(0),
	m_sub_type(0),
	m_guild_storage_score(0),
	god_attr(0)
{
	memset(m_base_attr_type_list, 0, sizeof(m_base_attr_type_list));
	memset(m_base_attr_value_list, 0, sizeof(m_base_attr_value_list));
}

int Equipment::Use(Role *role, const ItemGridData &item_data, short &num, short equip_index) const
{
	if (NULL == role || NULL == item_data.param) return -1;

	num = 1;  // 装备的消耗数量肯定为1

	ItemDataWrapper item_wrapper(m_item_id, 1, (item_data.is_bind ? 1 : 0));
	item_wrapper.invalid_time = item_data.invalid_time;

	EquipmentParam* equip_param = (EquipmentParam*)item_data.param;
	equip_param->SetInStructData(&item_wrapper.param_data);
	item_wrapper.has_param = 1;

	if (E_TYPE_SPECIAL_HUNJIE == this->GetEquipType())
	{
		return role->GetQingyuan()->PutOn(item_wrapper);
	}
	else if(E_TYPE_SPECIAL_JINGLING == this->GetEquipType())
	{
		if (E_TYPE_SPECIAL_MINGHUN == this->GetSubType())
		{
			return role->GetLieMingMgr()->PutOn(item_wrapper);
		}
		else
		{
			return role->GetJingLingMgr()->PutOn(item_wrapper, equip_index);
		}
	}
	else if (this->GetEquipType() >= E_TYPE_ZHUANSHENG_MIN && this->GetEquipType() < E_TYPE_ZHUANSHENG_MAX)
	{
		return role->GetZhuanShengEquip()->PutOnEquip(item_wrapper);
	}
	else if (this->GetEquipType() == E_TYPE_SPECIAL_IMP_GUARD)
	{
		return role->GetImpGuard()->PutOn(item_wrapper);
	}
	else if (this->GetEquipType() >= E_TYPE_QINGYUAN_MIN && this->GetEquipType() < E_TYPE_QINGYUAN_MAX)
	{
		QingyuanGlobal *qingyuan_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(role->GetUID());
		if (NULL != qingyuan_global)
		{
			return qingyuan_global->PutOnQingyuanEquip(role, item_wrapper);
		}

		return -1;
	}

// 	else if (this->GetEquipType() >= E_TYPE_FEIXIAN_MIN && this->GetEquipType() < E_TYPE_FEIXIAN_MAX)
// 	{
// 		return role->GetFeixianManager()->PutOn(item_wrapper);
// 	}

	else if (this->GetEquipType() >= E_TYPE_ZHUANZHI_MIN && this->GetEquipType() < E_TYPE_ZHUANZHI_MAX)
	{
		return role->GetZhuanzhiEquip()->PutOn(item_wrapper);
	}
	else if(this->GetEquipType() >= E_TYPE_CHINESE_ZODIAC_MIN && this->GetEquipType() < E_TYPE_CHINESE_ZODIAC_MAX)
	{
		return role->GetChineseZodiac()->PutOnEquip(item_wrapper, equip_index);
	}
	else if(this->GetEquipType()>= E_TYPE_SHENSHOU_MIN && this->GetEquipType()< E_TYPE_SHENSHOU_MAX)
	{
		return role->GetShenshou()->PutOnEquip(item_wrapper,equip_index);
	}
	else if (this->GetEquipType() >= E_TYPE_GREATESOLDIER_MIN && this->GetEquipType() < E_TYPE_GREATESOLDIER_MAX)
	{
		return role->GetGreateSoldier()->PutOnEquip(item_wrapper, equip_index);
	}
	else if (this->GetEquipType() >= E_TYPE_BAIZHAN_MIN && this->GetEquipType() < E_TYPE_BAIZHAN_MAX)
	{
		return role->GetBaizhanEquip()->PutOn(item_wrapper);
	}
	else if (this->GetEquipType() > E_TYPE_CHUANSHI_MIN && this->GetEquipType() < E_TYPE_CHUANSHI_MAX)
	{
		return role->GetCrossEquip()->PutOn(item_wrapper);
	}

	else
	{
		return role->GetEquipmentManager()->PutOn(item_wrapper, equip_index);
	}
}

int Equipment::GetSellPrice(ItemParam *param) const
{
	int sell_price = ItemBase::GetSellPrice(param);

	if (NULL != param)
	{
		EquipmentParam *equip_param = (EquipmentParam *)param;
		sell_price += (sell_price * equip_param->strengthen_level / 10);
	}

	return sell_price;
}

bool Equipment::CanDiscard(ItemParam *param, int *reason) const
{
	if (NULL == param) return false;

	return ItemBase::CanDiscard(param, reason);
}

EquipmentParam* Equipment::CreateParam() const
{
	EquipmentParam *new_param = new EquipmentParam();
	if (NULL == new_param) return NULL;

	return new_param;
}

bool Equipment::SetParamFromStruct(ItemParam *_param, const ItemParamDataStruct *equip_data) const
{
	EquipmentParam *param = (EquipmentParam *)_param;
	if (NULL == param || NULL == equip_data) return false;

	param->quality = equip_data->quality;
	param->strengthen_level = equip_data->strengthen_level;
	param->shen_level = equip_data->shen_level;
	param->fuling_level = equip_data->fuling_level;
	param->star_level = equip_data->star_level;
	param->has_lucky = equip_data->has_lucky;
	param->fumo_id = equip_data->fumo_id;
	param->xianpin_equip = equip_data->xianpin_equip;

	memcpy(param->xianpin_type_list, equip_data->xianpin_type_list, sizeof(param->xianpin_type_list));

	param->param1 = equip_data->param1;
	param->param2 = equip_data->param2;
	param->param3 = equip_data->param3;

	param->rand_attr_type_1 = equip_data->rand_attr_type_1;
	param->rand_attr_type_2 = equip_data->rand_attr_type_2;
	param->rand_attr_type_3 = equip_data->rand_attr_type_3;
	param->reserve_type = equip_data->reserve_type;

	param->rand_attr_val_1 = equip_data->rand_attr_val_1;
	param->rand_attr_val_2 = equip_data->rand_attr_val_2;
	param->rand_attr_val_3 = equip_data->rand_attr_val_3;
	param->rand_attr_val_reserve = equip_data->rand_attr_val_reserve;

	memcpy(param->jing_ling_skill_list, equip_data->jing_ling_skill_list, sizeof(param->jing_ling_skill_list));
	param->really = equip_data->really;
	param->xianpin_star_num = equip_data->xianpin_star_num;

	return true;
}
	
EquipmentParam* Equipment::CreateParamFromStruct(const ItemParamDataStruct *equip_data) const
{
	EquipmentParam* param = this->CreateParam();
	if (NULL == param) return NULL;

	if (!this->SetParamFromStruct(param, equip_data))
	{
		delete param;	
		return NULL;
	}

	return param;
}

// 获取装备基础属性
void Equipment::GetBaseAttr(CharIntAttrs &int_attrs) const
{
	for (int i = 0; i < BASE_ATTR_NUM; ++ i)
	{
		if (m_base_attr_value_list[i] > 0)
		{
			if (!CharIntAttrs::IsRoleBaseAttr(m_base_attr_type_list[i])) continue;

//			int_attrs.m_attrs[m_base_attr_type_list[i]] += m_base_attr_value_list[i];	
			int_attrs.AddValue(m_base_attr_type_list[i], m_base_attr_value_list[i]);
		}
	}
}

// 获取装备属性（包含强化等附加属性）
void Equipment::GetAttr(const EquipmentParam *param, CharIntAttrs &int_attrs, int role_level) const
{
	if (NULL == param) return;

	//添加品质的影响
	float rate = 1;
	const UpQualityConfig *up_quality_cfg = GLOBALCONFIG->GetEquipConfig().GetUpQualityCfg(m_equipment_type, param->quality);
	if (NULL != up_quality_cfg)
	{
		rate += up_quality_cfg->attr_percent * EQUIP_ATTR_PER;
	}

	// 基础属性
	for (int i = 0; i < BASE_ATTR_NUM; ++i)
	{
		int attr_value = m_base_attr_value_list[i];
		if (attr_value > 0)
		{
			int attr_type = m_base_attr_type_list[i];

			if (!CharIntAttrs::IsRoleBaseAttr(attr_type)) continue;

//			int_attrs.m_attrs[attr_type] += static_cast<Attribute>(attr_value * rate);
			int_attrs.AddValue(attr_type, static_cast<Attribute>(attr_value * rate));
		}
	}
}

int Equipment::GetAttrIndexByAttrType(short attr_type) const
{
	for (int i = 0; i < BASE_ATTR_NUM; ++ i)
	{
		if (m_base_attr_type_list[i] == attr_type)
		{
			return i;
		}
	}
	
	return -1;
}

bool Equipment::IsWuQiIndex(short equip_index)
{
	return E_INDEX_WUQI == equip_index;
}

bool Equipment::IsFangJuIndex(short equip_index)
{
	return E_INDEX_TOUKUI == equip_index || E_INDEX_YIFU  == equip_index || 
		E_INDEX_XIEZI == equip_index ||	E_INDEX_HUSHOU == equip_index ||
		E_INDEX_YAODAI == equip_index || E_INDEX_KUZI == equip_index;
}

bool Equipment::IsShouShiIndex(short equip_index)
{
	return E_INDEX_XIANGLIAN == equip_index || E_INDEX_JIEZHI == equip_index ||
		E_INDEX_SHOUZHUO == equip_index || E_INDEX_YUPEI == equip_index;
}

bool Equipment::IsCommonEquipIndex(short equip_index)
{
	return E_INDEX_WUQI == equip_index || (E_INDEX_TOUKUI <= equip_index && equip_index <= E_INDEX_HUSHOU);
}

bool Equipment::IsMatchIndex(short equip_type, short equip_index)
{
	switch (equip_index)
	{
	case E_INDEX_TOUKUI:// 头盔
		if (equip_type == E_TYPE_TOUKUI) { return true; }
	case E_INDEX_YIFU:// 衣服
		if (equip_type == E_TYPE_YIFU) { return true; }
	case E_INDEX_KUZI:// 裤子
		if (equip_type == E_TYPE_KUZI) { return true; }
	case E_INDEX_XIEZI:// 鞋子
		if (equip_type == E_TYPE_XIEZI) { return true; }
	case E_INDEX_HUSHOU:// 护手
		if (equip_type == E_TYPE_HUSHOU) { return true; }
	case E_INDEX_XIANGLIAN:// 项链
		if (equip_type == E_TYPE_XIANGLIAN) { return true; }
	case E_INDEX_WUQI:// 武器
		if (equip_type == E_TYPE_WUQI) { return true; }
	case E_INDEX_JIEZHI:// 戒指
		if (equip_type == E_TYPE_JIEZHI) { return true; }
	case E_INDEX_YAODAI:// 腰带
		if (equip_type == E_TYPE_YAODAI) { return true; }
	//case E_INDEX_JIEZHI_2:// 戒指
	//	if (equip_type == E_TYPE_JIEZHI) { return true; }
	case E_INDEX_YUPEI:// 玉佩
		if (equip_type == E_TYPE_YUPEI) { return true; }
	case E_INDEX_SHOUZHUO:// 手镯
		if (equip_type == E_TYPE_SHOUZHUO) { return true; }
	default:
		return false;
	}
	return false;
}

bool Equipment::IsValidType(short equip_type)
{
	if (equip_type < Equipment::E_TYPE_MIN || equip_type >= Equipment::E_TYPE_MAX) return false;

	return true;
}

bool Equipment::CheckIsSameItemExceptSex(const ItemBase *other_item) const
{
	if (!ItemBase::CheckIsSameItemExceptSex(other_item)) return false;

	const Equipment *other_equip = (const Equipment *)other_item;
	if (NULL == other_equip) return false;

	bool ret = (m_suit_id == other_equip->GetSuitId() && m_hole_num == other_equip->GetHoleNum() && m_equipment_type == other_equip->GetEquipType() && m_can_strengthen == other_equip->CanStrengthen() && m_can_flush == other_equip->CanFlush());

	if (!ret) return false;
	
	return false;
}

//  防具
bool Equipment::IsFangJuType(short equip_type)
{
	return E_TYPE_TOUKUI == equip_type || E_TYPE_YIFU  == equip_type || E_TYPE_YAODAI == equip_type || E_TYPE_KUZI == equip_type || E_TYPE_XIEZI  == equip_type;
}

// 饰品
bool Equipment::IsShipinType(short equip_type)
{
	return E_TYPE_JIEZHI == equip_type || E_TYPE_XIANGLIAN  == equip_type;
}

// 武器
bool Equipment::IsWuqiType(short equip_type)
{
	return E_TYPE_WUQI == equip_type;
}

// 护手
bool Equipment::IsHuShouType(short equip_type)
{
	return E_TYPE_HUSHOU == equip_type;
}

// 攻击类型：武器，手镯，戒指，项链，玉佩,是返回true
bool Equipment::IsGongjiType(short equip_type)
{
	return equip_type == E_TYPE_ZHUANZHI_WUQI || equip_type == E_TYPE_ZHUANZHI_XIANGLIAN || equip_type == E_TYPE_ZHUANZHI_SHOUZHUO || equip_type == E_TYPE_ZHUANZHI_JIEZHI || equip_type == E_TYPE_ZHUANZHI_YUPEI;
}
