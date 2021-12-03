#include <memory.h>

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/struct/structcommon.h"

#include "globalconfig/globalconfig.h"

#include "engineadapter.h"
#include "equipmentmanager.h"

#include "protocal/msgrole.h"

#include "item/knapsack.h"
#include "item/itempool.h"

#include "gamelog.h"
#include "world.h"
#include "gamedef.h"

#include "scene/scenemanager.h"
#include "scene/scene.h"

#include "obj/character/role.h"

#include "servercommon/struct/shizhuangparam.hpp"

#include "skill/skillpool.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/zhuanzhiconfig.hpp"
#include "other/jingling/jinglingconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "other/runesystem/runesystem.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"
#include "config/activityconfig/opengameactivityconfig.hpp"
#include "other/capability/capability.hpp"
#include "other/equipsuit/equipsuit.hpp"
#include "task/newtaskmanager.h"
#include "config/activityconfig/chongzhiconfig.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/shenzhuang/shenzhuang.h"
#include "globalconfig/otherconfig.h"
#include "other/talent/talent.hpp"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "config/joinlimitconfig.hpp"
#include "task/taskrecorder.h"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////

EquipmentManager::EquipmentManager() 
	: m_role(NULL), m_equipment_is_change(false), m_total_strengthen_level(0), m_max_strengthen_level(0), m_min_strengthen_level(0), m_max_quality(0), m_max_equip_level(0),
	m_total_shen_op_level(0), m_total_up_star_level(0), m_max_up_star_level(0), m_puton_equipment_act_flag(0), m_cross_equip_itemid(0), m_cross_equip_gain_timestamp(0), m_cross_equip_invalid_timestamp(0),
	m_next_check_equip_fumo_timestamp(0), m_equip_grade_suit_cur_level(0), m_equip_grade_suit_active_flag(0)
{
	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));
	memset(m_old_state, 0, sizeof(m_old_state));
	memset(m_xianpin_value_list, 0, sizeof(m_xianpin_value_list));
	memset(m_equip_level_count, 0, sizeof(m_equip_level_count));
}

EquipmentManager::~EquipmentManager()
{
	for (int i = 0; i < Equipment::E_INDEX_MAX; ++i)
	{
		if (NULL != m_equipment_list[i].param)
		{
			delete m_equipment_list[i].param;
		}
	}
}

void EquipmentManager::SetRole(Role *role)
{
	m_role = role;
}

void EquipmentManager::Init(Role *role, int param_list_num, const ItemListParam::ItemListData *param_data_list[], ItemID cross_equip_itemid, unsigned int cross_equip_gain_timestamp, EquipmentGridParam equipment_grid_param)
{
	m_role = role;

	m_cross_equip_itemid = cross_equip_itemid;
	m_cross_equip_gain_timestamp = cross_equip_gain_timestamp;
	m_equipment_grid_param = equipment_grid_param;

	const ItemBase *item_base = ITEMPOOL->GetItem(m_cross_equip_itemid);
	if (NULL != item_base && item_base->HasInvalidTime())
	{
		m_cross_equip_invalid_timestamp = m_cross_equip_gain_timestamp + item_base->GetTimeLength();
	}

	for (int i = 0; i < param_list_num; i++)
	{
		if (NULL == param_data_list[i]) break;

		if (param_data_list[i]->index < ItemNamespace::EQUIPMENT_INDEX_BEGIN) continue;

		int equip_index = param_data_list[i]->index - ItemNamespace::EQUIPMENT_INDEX_BEGIN; // index经过转换则为装备部位
		if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) continue;

		if (!m_equipment_list[equip_index].Invalid())
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[EquipmentManager::Init Duplicate][[%s %d] equip_index:%d old_item_id:%d new_item_id:%d]", role->GetName(), UidToInt(role->GetUserId()),
				equip_index, (int)m_equipment_list[equip_index].item_id, (int)param_data_list[i]->item_wrapper.item_id);
			continue;
		}

		UInt16 item_id = param_data_list[i]->item_wrapper.item_id;
		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_id);
		if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[EquipmentManager::Init IsNotEquipment][[%s %d] equip_index:%d item_id:%d]", role->GetName(), UidToInt(role->GetUserId()),
				equip_index, (int)item_id);
			continue;
		}

		EquipmentParam *param = equip->CreateParamFromStruct(&(param_data_list[i]->item_wrapper.param_data));
		if (NULL == param) // struct_data有错
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[EquipmentManager::Init StructDataError][[%s %d] equip_index:%d item_id:%d]", role->GetName(), UidToInt(role->GetUserId()),
				equip_index, (int)item_id);
			continue;
		}

		m_equipment_list[equip_index].item_id = item_id;
		m_equipment_list[equip_index].num = param_data_list[i]->item_wrapper.num;
		m_equipment_list[equip_index].is_bind = param_data_list[i]->item_wrapper.is_bind != 0 ? true : false;
		m_equipment_list[equip_index].invalid_time = param_data_list[i]->item_wrapper.invalid_time;
		m_equipment_list[equip_index].gold_price = param_data_list[i]->item_wrapper.gold_price;
		m_equipment_list[equip_index].param = param;


		if (m_equipment_list[equip_index].invalid_time > 0 && !equip->HasInvalidTime())
		{
			m_equipment_list[equip_index].invalid_time = 0;
		}

		m_old_state[equip_index] = true;
	}

	this->RefreshPutOnInfo(-1, APRR_INIT);
}

void EquipmentManager::GetInitParam(ItemID *cross_equip_itemid, unsigned int *cross_equip_gain_timestamp, EquipmentGridParam *equipment_grid_param)
{
	*cross_equip_itemid = m_cross_equip_itemid;
	*cross_equip_gain_timestamp = m_cross_equip_gain_timestamp;
	*equipment_grid_param = m_equipment_grid_param;
}

void EquipmentManager::Update(time_t now_second)
{
// 	if (0 != m_cross_equip_itemid && 0 != m_cross_equip_invalid_timestamp && now_second >= m_cross_equip_invalid_timestamp)
// 	{
// 		gamelog::g_log_equipment.printf(LL_INFO, "CrossEquipTimeOut:: User[%d, %s], item_id %d, item_gain_timestamp %u, now_second %lld",
// 			m_role->GetUID(), m_role->GetName(), (int)m_cross_equip_itemid, m_cross_equip_gain_timestamp, (long long)now_second);
// 
// 		m_cross_equip_itemid = 0;
// 		m_cross_equip_invalid_timestamp = 0;
// 		m_cross_equip_gain_timestamp = 0;
// 
// 		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
// 		this->SendEquipmentInfoToRole();
// 	}

// 	// 检查附魔过期
// 	if (0 == m_next_check_equip_fumo_timestamp || now_second >= m_next_check_equip_fumo_timestamp)
// 	{
// 		m_next_check_equip_fumo_timestamp = static_cast<int>(now_second) + 60;  //1分钟检查一次，减轻压力
// 
// 		bool is_fumo_timeout = false;
// 
// 		for (int i = 0; i < Equipment::E_INDEX_MAX; ++ i)
// 		{
// 			ItemGridData *item_grid_data = &m_equipment_list[i];
// 			if (item_grid_data->Invalid())
// 			{
// 				continue;
// 			}
// 			
// 			EquipmentParam *equip_param = (EquipmentParam *)item_grid_data->param;
// 			if (equip_param->fumo_id > 0 && equip_param->param1 <= EngineAdapter::Instance().Time())
// 			{
// 				equip_param->fumo_id = 0;
// 				equip_param->param1 = 0;
// 
// 				this->SetEquipGridDirty(i);
// 				is_fumo_timeout = true;
// 			}
// 		}
// 
// 		if (is_fumo_timeout)
// 		{
// 			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
// 		}
// 	}
}

int EquipmentManager::PutOn(const ItemDataWrapper &item_wrapper, short equip_index)
{
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_wrapper.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()) return -1;
	
	if (0 == item_wrapper.has_param) return -2;

	short equip_type = equip->GetEquipType();
	if (!Equipment::IsValidType(equip_type)) return -3;

	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return -4;

	if (!Equipment::IsMatchIndex(equip_type, equip_index)) return -5;

	ItemID  old_equip_id = m_equipment_list[equip_index].item_id;

	if (!m_equipment_list[equip_index].Invalid()) // 该位置有装备 脱之
	{
		int ret = this->TakeOff(equip_index, true);
		if (0 != ret) 
		{
			return -4;
		}
	}

	EquipmentParam *equip_param = equip->CreateParamFromStruct(&item_wrapper.param_data);
	if (NULL == equip_param) return -5;

	m_equipment_list[equip_index].item_id = item_wrapper.item_id;
	m_equipment_list[equip_index].param = equip_param;
	m_equipment_list[equip_index].num = 1;
	m_equipment_list[equip_index].invalid_time = item_wrapper.invalid_time;
	if (0 == item_wrapper.is_bind)
	{
		m_equipment_list[equip_index].is_bind = false;
	}
	else
	{
		m_equipment_list[equip_index].is_bind = true;
	}
	

	this->SetEquipGridDirty(equip_index);
	this->AddEquipmentSkill(item_wrapper.item_id);

	this->RefreshPutOnInfo(equip_index, APRR_PUTON);

	// 新手保护期，装备属性重算恢复满血
	int xinshou_protect_level = GLOBALCONFIG->GetRoleXinShouProtectedLevel();
	if (m_role->GetLevel() < xinshou_protect_level)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT, false, true);
	}
	else
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
	}

	m_role->GetEquipSuit()->OnEquipPutOn(equip_index, old_equip_id, m_equipment_list[equip_index].item_id);

	EventHandler::Instance().OnPutonEquip(m_role, equip_type, equip->GetColor(), equip_param->quality, equip->GetLimitLevel(), this->GetXianPinHightQualityCount(equip_param->xianpin_type_list));
	
	TianShuXunZhu::CheckTuMoTianShu(m_role);

	return 0;
}

int EquipmentManager::TakeOff(short equip_index, bool is_from_puton)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return -1;

	if (m_equipment_list[equip_index].Invalid()) return -2;

	EquipmentParam *equip_param = (EquipmentParam *)m_equipment_list[equip_index].param;
	if (NULL == equip_param) return -3;

	const Equipment *equip = (const Equipment*)ITEMPOOL->GetItem(m_equipment_list[equip_index].item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT) return -4;

	Knapsack *knapsack = m_role->GetKnapsack();
	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return -5;
	}

	ItemDataWrapper item_wrapper(m_equipment_list[equip_index].item_id, 1, (short)m_equipment_list[equip_index].is_bind, 
		m_equipment_list[equip_index].param, m_equipment_list[equip_index].invalid_time, 0);
	
	int put_index = -1;
	if (!knapsack->Put(item_wrapper, PUT_REASON_NO_NOTICE, &put_index))
	{
		return -6;
	}

	m_equipment_list[equip_index].Clear();		// 指针已无用 删除之

	this->DelEquipmentSkill(item_wrapper.item_id);
	this->SetEquipGridDirty(equip_index, !is_from_puton);

	if (!is_from_puton)
	{
		this->RefreshPutOnInfo(equip_index, APRR_TAKEOFF);

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);

		m_role->GetEquipSuit()->OnEquipTakeOff(equip_index, item_wrapper.item_id);
	}

	EventHandler::Instance().OnTakeOffEquip(m_role);

	return 0;
}

void EquipmentManager::UseEternityLevel(short eternity_level)
{
	if (eternity_level <= m_min_eternity_level)
	{
		m_equipment_grid_param.use_eternity_level = eternity_level;
	}
}

void EquipmentManager::RefreshPutOnInfo(short equip_index, short refresh_reason)
{
	int temp_min_eternity_level = m_min_eternity_level;
	this->RefreshEquipInfo();
	this->NotifyStrengLevel(m_role);

	bool to_refresh = false;

	if (APRR_INIT != refresh_reason)
	{
		//嘉年华
		m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_EQUIP_STRENGTHEN);
		m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_WEAR_EQUIPMENT); 
	}

	{
		// 这些原因操作武器或者衣服，都要更新外观
		if (APRR_EXPIRED == refresh_reason || APRR_PUTON == refresh_reason || APRR_TAKEOFF == refresh_reason ||
			APRR_CHANGE_ID == refresh_reason || APRR_STRENGTH == refresh_reason || APRR_NAIJIU == refresh_reason)
		{
			if (Equipment::E_INDEX_WUQI == equip_index)
			{
				to_refresh = true;
			}
		}

		if (temp_min_eternity_level != m_min_eternity_level)
		{
			to_refresh = true;
		}
	}

	{
		// 初始化不用更新外观
		if (APRR_INIT == refresh_reason)
		{
			to_refresh = false;
		}
	}

	if (to_refresh)
	{
		m_role->RefreshApperance(true);
	}

	this->ReCalcEquipUplevelSuitCount();
	this->SendEquipGradeSuitInfo();
}

// 发送装备格子属性
void EquipmentManager::SendEquipmentGridInfoToRole()
{
	//装备强化大小目标检查
	this->CheckFuncOpen();

	static Protocol::SCEquipmentGridInfo egi;
	egi.use_eternity_level = m_equipment_grid_param.use_eternity_level;
	egi.min_eternity_level = m_min_eternity_level;

	egi.count = 0;
	for (int i = 0; i < Equipment::E_INDEX_MAX; ++i)
	{
		egi.equipment_grid_list[egi.count].index = i;
		egi.equipment_grid_list[egi.count].strengthen_level = m_equipment_grid_param.equi_grid_item_list[i].strengthen_level;
		egi.equipment_grid_list[egi.count].shenzhu_level = m_equipment_grid_param.equi_grid_item_list[i].shenzhu_level;
		egi.equipment_grid_list[egi.count].star_level = m_equipment_grid_param.equi_grid_item_list[i].star_level;
		egi.equipment_grid_list[egi.count].eternity_level = m_equipment_grid_param.equi_grid_item_list[i].eternity_level;
		egi.equipment_grid_list[egi.count].star_exp = m_equipment_grid_param.equi_grid_item_list[i].star_exp;

		egi.count++;
	}

	unsigned int sendlen = sizeof(egi) - sizeof(egi.equipment_grid_list[0]) * (ItemNamespace::MAX_EQUIPMENT_GRID_NUM - egi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&egi, sendlen);
}

void EquipmentManager::SendEquipmentInfoToRole()
{
	static Protocol::SCEquipmentInfoAck eia;
	eia.is_ban_shizhuang_appe = 0;	//m_to_ban_dress_appe ? 1 : 0;
	eia.total_dress_score = 0;
	eia.cross_equip_item_id = m_cross_equip_itemid;
	eia.cross_equip_gain_timestamp = m_cross_equip_gain_timestamp;
	eia.min_strengthen_level = m_min_strengthen_level;

	int count = 0;

	for (int i = 0; i < Equipment::E_INDEX_MAX; ++i)
	{
		if (m_equipment_list[i].Invalid() || NULL == m_equipment_list[i].param) continue;

		eia.equipment_list[count].index = i;
		m_equipment_list[i].WrapTo(&eia.equipment_list[count].item_wrapper);
		
		count++;
	}

	eia.count = count;

	unsigned int sendlen = sizeof(eia) - sizeof(eia.equipment_list[0]) * (ItemNamespace::MAX_EQUIPMENT_GRID_NUM - count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&eia, sendlen);

	this->SendEquipmentGridInfoToRole();
}

void EquipmentManager::SendEquipmentInfoToRoleByIndex(short equip_index, short send_reason)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return;

	static Protocol::SCEquipmentItemChange eic;
	eic.index = equip_index;
	eic.send_reason = send_reason;
	m_equipment_list[equip_index].WrapTo(&eic.item_wrapper);
	eic.min_strengthen_level = m_min_strengthen_level;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&eic, sizeof(Protocol::SCEquipmentItemChange));

	this->SendEquipmentGridInfoToRole();
}

bool EquipmentManager::TransformToNewEquip(short equip_index, UInt16 new_equip_id)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return false;

	if (m_equipment_list[equip_index].Invalid() || NULL == m_equipment_list[equip_index].param) return false;
	
	const Equipment *equip_old = (const Equipment *)ITEMPOOL->GetItem(m_equipment_list[equip_index].item_id);
	const Equipment *equip_new = (const Equipment *)ITEMPOOL->GetItem(new_equip_id);
	if (NULL == equip_old || NULL == equip_new) return false;
	if (equip_old->GetEquipType() != equip_new->GetEquipType()) return false;

	if (equip_old == equip_new) return false;
	
	m_equipment_list[equip_index].item_id = new_equip_id;

	this->SetEquipGridDirty(equip_index);

	this->RefreshPutOnInfo(equip_index, APRR_CHANGE_ID);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);

	return true;
}

void EquipmentManager::SetEquipGridDirty(short equip_index, bool to_notice_client, short send_reason)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return;

	m_dirty_mark[equip_index] = true;
	m_equipment_is_change = true;

	if (to_notice_client) this->SendEquipmentInfoToRoleByIndex(equip_index, send_reason);
}

void EquipmentManager::GridSetBind(short equip_index, bool need_set_dirty)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return;

	if (!m_equipment_list[equip_index].Invalid() && !m_equipment_list[equip_index].is_bind)
	{
		m_equipment_list[equip_index].is_bind = true;

		if (need_set_dirty) this->SetEquipGridDirty(equip_index);
	}
}

void EquipmentManager::GridClearInvalidTime(short equip_index, ItemID equip_id, bool need_set_dirty)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return;

	if (!m_equipment_list[equip_index].Invalid() && m_equipment_list[equip_index].invalid_time > 0)
	{
		m_equipment_list[equip_index].invalid_time = 0;

		if (need_set_dirty) this->SetEquipGridDirty(equip_index);
	}
}

void EquipmentManager::ChangeAllItemToOtherSex()
{
	//bool has_changed = false;

	for (int i = 0; i < Equipment::E_INDEX_MAX; i++)
	{
		if (!m_equipment_list[i].Invalid())
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(m_equipment_list[i].item_id);
			if (NULL != itembase && 0 != itembase->GetOtherSexItemId())
			{
				const ItemBase *other_sex_item = ITEMPOOL->GetItem(itembase->GetOtherSexItemId());
				if (NULL != other_sex_item) 
				{
					m_equipment_list[i].item_id = other_sex_item->GetItemId();

					this->SetEquipGridDirty(i);

					//has_changed = true;
				}
			}
		}
	}

	this->RefreshPutOnInfo(Equipment::E_INDEX_WUQI, APRR_CHANGE_ID);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
}

bool EquipmentManager::GetEquipmentGridData(short equip_index, ItemGridData *equip_data)
{
	if (NULL == equip_data || equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return false;

	if (m_equipment_list[equip_index].Invalid()) return false;

	equip_data->Set(m_equipment_list[equip_index]);

	return true;
}

bool EquipmentManager::SetEquipmentGridData(short equip_index, ItemGridData *equip_data)
{
	if (NULL == equip_data || equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return false;

	m_equipment_list[equip_index].Set(*equip_data);
	this->SetEquipGridDirty(equip_index);

	this->RefreshPutOnInfo(equip_index, APRR_CHANGE_ID);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
	return true;
}

void EquipmentManager::GetEquipmentList(int *count, ItemListParam::ItemListData *save_list)
{
	int tmp_count = 0;

	for (int i = 0; i < Equipment::E_INDEX_MAX; ++i)
	{
		if (m_equipment_list[i].Invalid() || NULL == m_equipment_list[i].param) continue;
		{
			save_list[tmp_count].index = i + ItemNamespace::EQUIPMENT_INDEX_BEGIN; // 装备存储时的index从这个开始
			m_equipment_list[i].WrapTo(&save_list[tmp_count].item_wrapper);

			save_list[tmp_count].change_state_item = structcommon::CS_NONE;

			if (++tmp_count >= *count) break;
		}
	}

	*count = tmp_count;
}

void EquipmentManager::GetChangeEquipmentList(int *count, ItemListParam::ItemListData *save_list)
{
	if (!m_equipment_is_change)
	{
		*count = 0;
		return;
	}

	int tmp_count = 0;

	for (int i = 0; i < Equipment::E_INDEX_MAX; ++i)
	{
		if (m_dirty_mark[i])
		{
			save_list[tmp_count].index = i + ItemNamespace::EQUIPMENT_INDEX_BEGIN;

			if (!m_equipment_list[i].Invalid() && NULL != m_equipment_list[i].param)
			{
				m_equipment_list[i].WrapTo(&save_list[tmp_count].item_wrapper);

				save_list[tmp_count].change_state_item = (m_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT);
			}
			else
			{
				if (!m_old_state[i]) continue;

				save_list[tmp_count].change_state_item = structcommon::CS_DELETE;
			}

			if (++tmp_count >= *count) break;
		}
	}

	*count = tmp_count;
}

void EquipmentManager::ClearDirtyMark()
{
	if (!m_equipment_is_change) return;

	for (int i = 0; i < Equipment::E_INDEX_MAX; ++i)
	{
		if (m_dirty_mark[i])
		{
			m_old_state[i] = !m_equipment_list[i].Invalid();
		}
	}

	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));

	m_equipment_is_change = false;
}

void EquipmentManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		CharIntAttrs total_equip_base_attrs;
		total_equip_base_attrs.Reset();

		CharIntAttrs total_equip_strengthen_attrs;	// 强化总属性

		CharIntAttrs total_equip_fangju_attrs; 		// 防具总属性
		CharIntAttrs total_equip_wuqi_attrs; 		// 武器总属性
		CharIntAttrs total_equip_shoushi_attrs;		// 首饰总属性

		int puton_level = m_role->GetPutOnLevel();
		for (int i = 0; i < Equipment::E_INDEX_MAX; ++i)
		{
			if (m_equipment_list[i].Invalid()) continue;

			this->AttachAttr(m_equipment_list[i], m_attrs_add, puton_level);

			if (i < MAX_EQUIP_PART)
			{
				const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_equipment_list[i].item_id);
				if (NULL != equip)
				{
					CharIntAttrs equip_base_attrs;
					equip->GetBaseAttr(equip_base_attrs);

					total_equip_base_attrs.AddAttrs(&equip_base_attrs);

					if (equip->IsFangJuIndex(i))
					{
						total_equip_fangju_attrs.Add(equip_base_attrs.m_attrs);
					}

					else if (equip->IsWuQiIndex(i))
					{
						total_equip_wuqi_attrs.Add(equip_base_attrs.m_attrs);
					}

					else if (equip->IsShouShiIndex(i))
					{
						total_equip_shoushi_attrs.Add(equip_base_attrs.m_attrs);
					}

					// 神铸，装备基础属性加成
					const ShenOPConfig *shen_cfg = GLOBALCONFIG->GetEquipConfig().GetShenOPCfg(i, m_equipment_grid_param.equi_grid_item_list[i].shenzhu_level);
					if (NULL != shen_cfg)
					{
						double shen_rate = shen_cfg->attr_percent * 0.01;

						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(shen_cfg->add_maxhp + equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * shen_rate);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(shen_cfg->add_gongji + equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * shen_rate);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(shen_cfg->add_fangyu + equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * shen_rate);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(shen_cfg->add_mingzhong + equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * shen_rate);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(shen_cfg->add_shanbi + equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * shen_rate);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(shen_cfg->add_baoji + equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * shen_rate);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(shen_cfg->add_jianren + equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * shen_rate);

						// 神铸后不会有特殊属性，但装备本身的特殊属性需要有加成
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] * shen_rate);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] * shen_rate);
//						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] * shen_rate);
//						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] * shen_rate);

						// 特殊属性用乘法
						Attribute per_pofang = static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] * shen_rate);
						Attribute per_mianshang = static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] * shen_rate);
						m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, per_pofang);
						m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, per_mianshang);
					}

					// 升星，角色基础属性加成
					const UpStarConfig *star_cfg = GLOBALCONFIG->GetEquipConfig().GetUpStarCfg(i, m_equipment_grid_param.equi_grid_item_list[i].star_level);
					if (NULL != star_cfg)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(star_cfg->add_maxhp);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(star_cfg->add_gongji);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(star_cfg->add_fangyu);
					}

					// 强化，装备基础属性加成
					int strengthen_level = m_equipment_grid_param.equi_grid_item_list[i].strengthen_level;

					int equip_strengthen_max = GLOBALCONFIG->GetEquipConfig().GetStrengthMaxValue(i, equip->GetOrder());	// 当前装备阶数能达到的最大强度
					if (equip_strengthen_max <= 0) continue;

					// 如果换了的装备的阶数对应的最高强化数比当前格子的强化数要低，那么就取装备的阶数对应的最高强化数
					if (equip_strengthen_max < strengthen_level)
					{
						strengthen_level = equip_strengthen_max;
					}

					if (strengthen_level > 0)
					{
						const StrengthBaseConfig *strength_cfg = GLOBALCONFIG->GetEquipConfig().GetStrengthBaseCfg(i, strengthen_level);
						if (NULL != strength_cfg)
						{
							total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(strength_cfg->add_maxhp);
							total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(strength_cfg->add_gongji);
							total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(strength_cfg->add_fangyu);
							total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(strength_cfg->add_mingzhong);
							total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(strength_cfg->add_shanbi);
							total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(strength_cfg->add_baoji);
							total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(strength_cfg->add_jianren);
						}
					}

					// 永恒装备
					int eternity_level = m_equipment_grid_param.equi_grid_item_list[i].eternity_level;
					if (eternity_level > 0)
					{
						const EquipEternityCfg *eternity_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipEternityCfg(i, eternity_level);
						if (NULL != eternity_cfg)
						{
							CharIntAttrs eternity_attr; eternity_attr.Reset();

							eternity_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += eternity_cfg->maxhp;
							eternity_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += eternity_cfg->gongji;
							eternity_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += eternity_cfg->fangyu;
							eternity_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += eternity_cfg->mingzhong;
							eternity_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += eternity_cfg->shanbi;
							eternity_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += eternity_cfg->baoji;
							eternity_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += eternity_cfg->jianren;

							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += eternity_cfg->maxhp;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += eternity_cfg->gongji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += eternity_cfg->fangyu;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += eternity_cfg->mingzhong;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += eternity_cfg->shanbi;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += eternity_cfg->baoji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += eternity_cfg->jianren;

							// 天赋加成 - 永恒套装加的是永恒装备属性并不是套装属性
							m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_YONGHENG_SUIT, eternity_attr, m_attrs_add);
						}
					}

					// 红色神装加成
					//if (ItemBase::I_COLOR_RED == equip->GetColor())
					//{
					//	int red_star = 0;

					//	EquipmentParam *equip_param = static_cast<EquipmentParam*>(m_equipment_list[i].param);
					//	for (int k = 0; k < ItemNamespace::MAX_XIANPIN_NUM; ++k)
					//	{
					//		const EquipXianPinConfig *xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(equip_param->xianpin_type_list[k]);
					//		if (NULL != xianpin_cfg) red_star++;
					//	}

					//	if (red_star > 0)
					//	{
					//		double shenzhuang_rate = m_role->GetShenZhuang()->GetRedRatio(i, red_star) * EQUIP_ATTR_PER;
					//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<int>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * shenzhuang_rate);
					//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<int>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * shenzhuang_rate);
					//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<int>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * shenzhuang_rate);
					//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<int>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * shenzhuang_rate);
					//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<int>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * shenzhuang_rate);
					//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<int>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * shenzhuang_rate);
					//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<int>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * shenzhuang_rate);
					//	}
					//}
					// 粉色神装加成
					if (ItemBase::I_COLOR_PINK == equip->GetColor())
					{
						double shenzhuang_rate = m_role->GetShenZhuang()->GetPinkRatio(i) * EQUIP_ATTR_PER;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * shenzhuang_rate);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * shenzhuang_rate);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * shenzhuang_rate);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * shenzhuang_rate);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * shenzhuang_rate);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * shenzhuang_rate);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * shenzhuang_rate);
					}
				}
			}
		}

		// 符文基础属性百分比加成
		{
			// 防具
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += m_role->GetRuneSystem()->RecalcFangJuAddMaxHp(total_equip_fangju_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP]);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += m_role->GetRuneSystem()->RecalcFangJuAddShanbi(total_equip_fangju_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI]);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += m_role->GetRuneSystem()->RecalcFangJuAddJianren(total_equip_fangju_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN]);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += m_role->GetRuneSystem()->RecalcFangJuAddFangyu(total_equip_fangju_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU]);

			// 武器
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += m_role->GetRuneSystem()->RecalcWuQiAddGongJi(total_equip_wuqi_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI]);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += m_role->GetRuneSystem()->RecalcWuQiAddBaoji(total_equip_wuqi_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI]);

			// 首饰
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += m_role->GetRuneSystem()->RecalcShouShiAddGongji(total_equip_shoushi_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI]);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += m_role->GetRuneSystem()->RecalcShouShiAddBaoji(total_equip_shoushi_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI]);

			// 全身装备攻击加成
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += m_role->GetRuneSystem()->RecalcEquipAddGongJi(total_equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI]);
		}

		// 天赋加成
		m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_NORMAL_EQUIP_BASE, total_equip_base_attrs, m_attrs_add);

		// 强化总属性
		m_attrs_add.Add(total_equip_strengthen_attrs.m_attrs);
		// 激活大目标
		{
			int add_value = m_role->GetRoleBigSmallGoal()->GetRoleBigSmallGoalAddAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_EQUIP_STRENGTHEN);
			if (add_value > 0)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(round(total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (add_value * ROLE_ATTR_PER)));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(round(total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (add_value * ROLE_ATTR_PER)));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(round(total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (add_value * ROLE_ATTR_PER)));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(round(total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (add_value * ROLE_ATTR_PER)));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(round(total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (add_value * ROLE_ATTR_PER)));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(round(total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (add_value * ROLE_ATTR_PER)));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(round(total_equip_strengthen_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (add_value * ROLE_ATTR_PER)));
			}
		}

		// 全身强化
		//if (m_total_strengthen_level > 0)
		//{
		//	const StrengthTotalLvlAddConfig *cfg = GLOBALCONFIG->GetEquipConfig().GetStrengthTotallvlAddCfg(m_total_strengthen_level);
		//	if (NULL != cfg)
		//	{
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->add_maxhp;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->add_gongji;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->add_fangyu;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg->add_mingzhong;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg->add_shanbi;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg->add_baoji;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg->add_jianren;
		//	}
		//}

		// 全身神铸
		//if (m_total_shen_op_level > 0)
		//{
		//	const ShenOPTotalConfig *cfg = GLOBALCONFIG->GetEquipConfig().GetShenOPTotalCfg(m_total_shen_op_level);
		//	if (NULL != cfg)
		//	{
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->add_maxhp;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->add_gongji;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->add_fangyu;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg->add_mingzhong;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg->add_shanbi;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg->add_baoji;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg->add_jianren;
		//	}
		//}

		// 全身升星
		if (m_total_up_star_level > 0)
		{
			const UpStarTotalConfig *cfg = GLOBALCONFIG->GetEquipConfig().GetUpStarTotalCfg(m_total_up_star_level, false);
			if (NULL != cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->add_maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->add_gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->add_fangyu;
			}
		}

		// 永恒套装
		if (m_min_eternity_level > 0)
		{
			const EquipEternitySuitCfg *eternity_suit_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipEternitySuitCfg(m_min_eternity_level);
			if (NULL != eternity_suit_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += eternity_suit_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += eternity_suit_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += eternity_suit_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += eternity_suit_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += eternity_suit_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += eternity_suit_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += eternity_suit_cfg->jianren;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI] += eternity_suit_cfg->hxyj;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER] += eternity_suit_cfg->hxyj_hurt_per;
			}
		}

		//等级套装属性
		if (m_equipment_grid_param.cur_grade_suit_level >= 0 || m_equipment_grid_param.cur_grade_suit_level < EquipConfig::UPLEVEL_SUIT_COUNT)
		{
			const EquipUpLevelSuitAttrCfg *suit_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipUplevelSuitCfg(m_equipment_grid_param.cur_grade_suit_level, m_equip_level_count[m_equipment_grid_param.cur_grade_suit_level]);
			if (suit_cfg != NULL)
			{
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, suit_cfg->maxhp);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, suit_cfg->gongji);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, suit_cfg->fangyu);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, suit_cfg->baoji);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, suit_cfg->jianren);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, suit_cfg->mingzhong);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, suit_cfg->shanbi);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_ZHUFUYIJI_PER, suit_cfg->per_zhufuyiji);
			}
		}

		this->ReCalcXianpinAttr(m_attrs_add, total_equip_base_attrs);

		// 人物大目标固定属性
		m_role->GetRoleBigSmallGoal()->RoleBigSmallGoalAddSpecialAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_EQUIP_STRENGTHEN, m_attrs_add);

		// 天赋加成
		m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_NORMAL_EQUIP, total_equip_base_attrs, m_attrs_add);

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_EQUIPMENT, m_attrs_add);
	}
	
	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

// 仙品计算加成
void EquipmentManager::ReCalcXianpinAttr(CharIntAttrs &attrs_add, CharIntAttrs &total_equip_base_add)
{
	memset(m_xianpin_value_list, 0, sizeof(m_xianpin_value_list));

	for (int i = 0; i < Equipment::E_INDEX_MAX; ++i)
	{
		ItemGridData *item_grid_data = &m_equipment_list[i];
		if (NULL == item_grid_data || item_grid_data->Invalid())
		{
			continue;
		}

		EquipmentParam *equip_param = static_cast<EquipmentParam*>(item_grid_data->param);
		if (NULL == equip_param) continue;

		for (int k = 0; k < ItemNamespace::MAX_XIANPIN_NUM; ++k)
		{
			int attr_type = equip_param->xianpin_type_list[k];
			
			const EquipXianPinConfig *xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(attr_type);
			if (NULL == xianpin_cfg)
			{
				continue;
			}

			Attribute attr_value  = xianpin_cfg->add_value;

			m_xianpin_value_list[xianpin_cfg->shuxing_type] += attr_value;
		}
	}

	for (int xianpin_type = Equipment::E_XIANPIN_ADD_BASE_MAXHP_PER; xianpin_type < Equipment::E_XIANPIN_MAX; ++xianpin_type)
	{
		switch(xianpin_type)
		{
			// +X%气血上限
		case Equipment::E_XIANPIN_ADD_BASE_MAXHP_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_MAXHP_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += m_xianpin_value_list[xianpin_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +X%攻击力
		case Equipment::E_XIANPIN_ADD_BASE_GONGJI_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_GONGJI_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += m_xianpin_value_list[xianpin_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +X%防御力
		case Equipment::E_XIANPIN_ADD_BASE_FANGYU_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_FANGYU_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += m_xianpin_value_list[xianpin_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +X%命中
		case Equipment::E_XIANPIN_ADD_BASE_MINGZHAONG_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_MINGZHAONG_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += m_xianpin_value_list[xianpin_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +X%闪避
		case Equipment::E_XIANPIN_ADD_BASE_SHANBI_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_SHANBI_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += m_xianpin_value_list[xianpin_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +X%暴击
		case Equipment::E_XIANPIN_ADD_BASE_BAOJI_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_BAOJI_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += m_xianpin_value_list[xianpin_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +X%坚韧
		case Equipment::E_XIANPIN_ADD_BASE_JIANREN_PER:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_JIANREN_PER] > 0)
				{
					attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += m_xianpin_value_list[xianpin_type] * EQUIP_ATTR_PER;
				}
			}
			break;

			// +角色等级*X气血上限
		case Equipment::E_XIANPIN_ADD_BASE_MAXHP_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_MAXHP_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[xianpin_type]);
				}
			}
			break;

			// +角色等级*X攻击力
		case Equipment::E_XIANPIN_ADD_BASE_GONGJI_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_GONGJI_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[xianpin_type]);
				}
			}
			break;

			// +角色等级*X防御力
		case Equipment::E_XIANPIN_ADD_BASE_FANGYU_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_FANGYU_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[xianpin_type]);
				}
			}
			break;

			// +角色等级*X命中
		case Equipment::E_XIANPIN_ADD_BASE_MINGZHONG_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_MINGZHONG_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[xianpin_type]);
				}
			}
			break;

			// +角色等级*X闪避
		case Equipment::E_XIANPIN_ADD_BASE_SHANBI_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_SHANBI_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[xianpin_type]);
				}
			}
			break;

			// +角色等级*X暴击
		case Equipment::E_XIANPIN_ADD_BASE_BAOJI_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_BAOJI_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[xianpin_type]);
				}
			}
			break;

			// +角色等级*X坚韧
		case Equipment::E_XIANPIN_ADD_BASE_JIANREN_ROLE_LEVEL:
			{
				if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_BASE_JIANREN_ROLE_LEVEL] > 0)
				{
					attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(m_role->GetLevel() * m_xianpin_value_list[xianpin_type]);
				}
			}
			break;

			// +X%全身装备防御
		case Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_FANGYU_PER:
		{
			if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_FANGYU_PER] > 0)
			{
				attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(total_equip_base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (m_xianpin_value_list[xianpin_type] * ROLE_ATTR_PER));
			}
		}
		break;

		// +X%全身装备气血
		case Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_MAXHP_PER:
		{
			if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_MAXHP_PER] > 0)
			{
				attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(total_equip_base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (m_xianpin_value_list[xianpin_type] * ROLE_ATTR_PER));
			}
		}
		break;

		// +X%全身装备攻击
		case Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_GONGJI_PER:
		{
			if (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_GONGJI_PER] > 0)
			{
				attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(total_equip_base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (m_xianpin_value_list[xianpin_type] * ROLE_ATTR_PER));
			}
		}
		break;
		}
	}
}

Attribute EquipmentManager::EquipXianPinGetValue(int attr_type)
{
	if (attr_type <= Equipment::E_XIANPIN_INVALID || attr_type >= Equipment::E_XIANPIN_MAX)
	{
		return 0;
	}

	return m_xianpin_value_list[attr_type];
}

void EquipmentManager::NotifyStrengLevel(Role *role)
{
	if (NULL == role || NULL == role->GetScene()) return;

	bool broadcast_color = false;
	const DailyChongzhiRewradConfig *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiRewradConfig(role->GetLevel(), 0);
	if (NULL != cfg)
	{
		if (role->GetRoleActivity()->GetParam()->history_chongzhi >= static_cast<long long>(cfg->need_total_chongzhi))
		{
			broadcast_color = true;
		}
	}

	// 广播武器颜色
	Protocol::SCNoticeWuqiColor cmd;
	cmd.obj_id = role->GetId();
	cmd.reserve = 0;
	cmd.wuqi_color = (broadcast_color && this->GetEquipOrder(Equipment::E_INDEX_WUQI) > 3) ? this->GetEquipColor(Equipment::E_INDEX_WUQI) : 0;
	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
}

bool EquipmentManager::IsSatisfyMagicalPrecious(int order, int color, int part)
{
	if (part < Equipment::E_INDEX_TOUKUI || part >= Equipment::E_INDEX_MAX) return false;

	if (color <= ItemBase::I_COLOR_INVALID || color >= ItemBase::I_COLOR_MAX) return false;

	if (order <= 0 || order >= ItemPool::MAX_EQUIPMENT_ORDER) return false;
	
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_equipment_list[part].item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()) return false;

	if (equip->GetColor() < color) return false;

	if (equip->GetOrder() < order) return false;
	
	return true;
}

int EquipmentManager::GetSatisfyMagicalPrecious2Count(int order, int eternity_level)
{
	int suit_equip_count = 0;
	for (int index = 0; index < Equipment::E_INDEX_MAX; index ++)
	{
		const ItemGridData &equip_grid = m_equipment_list[index];
		int equip_eternity_level = GetEquipGridEternityLevel(index);
		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(equip_grid.item_id);
		if (nullptr != equip && ItemBase::I_TYPE_EQUIPMENT == equip->GetItemType())
		{
			if (equip_eternity_level >= eternity_level && equip->GetOrder() >= order)
			{
				++suit_equip_count;
			}
		}
	}

	return suit_equip_count;
}

bool EquipmentManager::AttachAttr(const ItemGridData &item_data, CharIntAttrs &base_add, int puton_level)
{
	if (item_data.Invalid()) return false;

	const EquipmentParam *param = (const EquipmentParam *)item_data.param;
	if (NULL == param) return false;

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_data.item_id);	// 配置表里面的装备的强化属性
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT) return false;

	// 基础与强化属性
	if (puton_level >= equip->GetLimitLevel())
	{
		equip->GetAttr(param, base_add, m_role->GetLevel());
	}

	return true;
}

void EquipmentManager::RefreshEquipInfo()
{
	m_total_strengthen_level = 0;
	m_max_strengthen_level = 0;
	m_min_strengthen_level = 0;
	m_max_quality = 0;
	m_max_equip_level = 0;
	m_total_up_star_level = 0;
	m_max_up_star_level = 0;
	m_total_shen_op_level = 0;
	int temp_strengthen_level = 999;
	m_min_eternity_level = 0;
	short eternity_level_count_list[EquipConfig::MAX_ETERNITY_LEVEL + 1] = {0};

	m_puton_equipment_act_flag = LOGIC_CONFIG->GetOpenGameActivityCfg().GetPutonEquipmentActFlag(m_equipment_list);

	for (int i = 0; i < Equipment::E_INDEX_MAX; ++i)
	{
		if (m_equipment_list[i].Invalid())
		{
			temp_strengthen_level = 0;
			continue;
		}

		const ItemBase *item_base = ITEMPOOL->GetItem(m_equipment_list[i].item_id);
		if (NULL == item_base || ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType())
		{
			continue;
		}

		// 强化
		{
			int strengthen_level = m_equipment_grid_param.equi_grid_item_list[i].strengthen_level;

			const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_equipment_list[i].item_id);

			int equip_strengthen_max = GLOBALCONFIG->GetEquipConfig().GetStrengthMaxValue(i, equip->GetOrder());	// 当前装备阶数能达到的最大强化等级

			if(0 < equip_strengthen_max)
			{
				// 如果换了的装备的阶数对应的最高强化等级比当前格子的强化等级要低，那么就取装备的阶数对应的最高强化等级
				if(equip_strengthen_max < strengthen_level)
				{
					strengthen_level = equip_strengthen_max;
				}

				m_total_strengthen_level += strengthen_level;

				if (strengthen_level > m_max_strengthen_level)
				{
					m_max_strengthen_level = strengthen_level;
				}

				if (strengthen_level < temp_strengthen_level)
				{
					temp_strengthen_level = strengthen_level;
				}
			}
		}

		// 装备等级
		{
			if (item_base->GetLimitLevel() > m_max_equip_level)
			{
				m_max_equip_level = item_base->GetLimitLevel();
			}
		}

		// 神铸
		{
			m_total_shen_op_level += m_equipment_grid_param.equi_grid_item_list[i].shenzhu_level;
		}

		//星星
		{
			m_total_up_star_level += m_equipment_grid_param.equi_grid_item_list[i].star_level;

			if (m_equipment_grid_param.equi_grid_item_list[i].star_level > m_max_up_star_level)
			{
				m_max_up_star_level = m_equipment_grid_param.equi_grid_item_list[i].star_level;
			}
		}

		{
			for (int k = 1; k <= m_equipment_grid_param.equi_grid_item_list[i].eternity_level && k <= EquipConfig::MAX_ETERNITY_LEVEL; k++)
			{
				eternity_level_count_list[k]++;
			}
		}
	}

	m_role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_EQUIP_TOTAL_STRENGTH_LEVEL);
	m_role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_EQUIP_TOTAL_STAR_LEVEL);
	m_role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_RED_EQUIP_WEAR_COUNT);
	m_role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_PUT_ON_EQUIP);

	if (temp_strengthen_level <= EquipConfig::MAX_STRENGTH_LEVEL)
	{
		m_min_strengthen_level = temp_strengthen_level;
	}

	for (int i = 1; i <= EquipConfig::MAX_ETERNITY_LEVEL; i++)
	{
		if (eternity_level_count_list[i] < EquipConfig::ETERNITY_SUIT_COUNT) break;
		const EquipEternitySuitCfg *eternity_suit_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipEternitySuitCfg(i);
		if (NULL != eternity_suit_cfg)
		{
			m_min_eternity_level = i;
		}
	}
	if (m_min_eternity_level < m_equipment_grid_param.use_eternity_level)
	{
		m_equipment_grid_param.use_eternity_level = m_min_eternity_level;
	}
}

void EquipmentManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL == appearance) return;

	if (!m_equipment_list[Equipment::E_INDEX_WUQI].Invalid())
	{
		appearance->wuqi_id = m_equipment_list[Equipment::E_INDEX_WUQI].item_id;
	}
	appearance->use_eternity_level = m_equipment_grid_param.use_eternity_level;
}

bool EquipmentManager::GMPuton(int equip_index, int equip_id)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) 
	{
		return false;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(equip_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
	{
		return false;
	}

	m_equipment_list[equip_index].Clear();		// 指针已无用 删除之

	EquipmentParam *equip_param = new EquipmentParam();
	if (NULL == equip_param) return false;

	m_equipment_list[equip_index].item_id = equip_id;
	m_equipment_list[equip_index].param = equip_param;
	m_equipment_list[equip_index].num = 1;
	m_equipment_list[equip_index].invalid_time = 0;
	m_equipment_list[equip_index].is_bind = true;

	this->SetEquipGridDirty(equip_index);

	this->RefreshPutOnInfo(equip_index, APRR_PUTON);

	return true;
}

void EquipmentManager::GetEquipMinInfo(int *min_equip_color, int *min_equip_level)
{
	int temp_min_equip_color = ItemBase::I_COLOR_MAX;
	int temp_min_equip_level = MAX_ROLE_LEVEL;

	for (int i = 0; i < Equipment::E_INDEX_MAX; ++i)
	{
		if (m_equipment_list[i].Invalid())
		{
			temp_min_equip_color = 0;
			temp_min_equip_level = 0;
			break;
		}

		const ItemBase *item = ITEMPOOL->GetItem(m_equipment_list[i].item_id);
		if (NULL == item)
		{
			temp_min_equip_color = 0;
			temp_min_equip_level = 0;
			break;
		}

		if (temp_min_equip_color > item->GetColor())
		{
			temp_min_equip_color = item->GetColor();
		}

		if (temp_min_equip_level > item->GetLimitLevel())
		{
			temp_min_equip_level = item->GetLimitLevel();
		}
	}

	if (NULL != min_equip_color)
	{
		*min_equip_color = temp_min_equip_color;
	}

	if (NULL != min_equip_level)
	{
		*min_equip_level = temp_min_equip_level;
	}
}

void EquipmentManager::OnChangeProf()
{
	for (int i = 0; i < Equipment::E_INDEX_MAX; i++)
	{
		if (!m_equipment_list[i].Invalid())
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(m_equipment_list[i].item_id);
			if (NULL != itembase)
			{
				ItemID change_itemid = 0;
				if (LOGIC_CONFIG->GetZhuanzhiCfg().GetChangeItemID(itembase->GetItemId(), m_role->GetProf() % PROF_TYPE_PROF_ZHUANZHI_BASE, &change_itemid))
				{
					const ItemBase *change_item = ITEMPOOL->GetItem(change_itemid);
					if (NULL != change_item) 
					{
						m_equipment_list[i].item_id = change_itemid;
						this->SetEquipGridDirty(i);

						m_role->RefreshApperance(true);
					}
				}
			}
		}
	}
}

void EquipmentManager::PutOnCrossEquip(int grid_idx)
{
// 	if (0 != m_cross_equip_itemid)
// 	{
// 		m_role->NoticeNum(errornum::EN_EQUIP_CROSS_EQUIP_ALREAD_EQUIP);
// 		return;
// 	}
// 
// 	ItemGridData item_grid_data;
// 	bool ret = m_role->GetKnapsack()->GetItemGridData(grid_idx, &item_grid_data);
// 	if (!ret)
// 	{
// 		return;
// 	}
// 
// 	const EquipCrossEquipConfig *cross_equip_cfg = GLOBALCONFIG->GetEquipConfig().GetCrossEquipCfg(item_grid_data.item_id);
// 	if (NULL == cross_equip_cfg)
// 	{
// 		m_role->NoticeNum(errornum::EN_EQUIP_CROSS_EQUIP_CANNOT_PUTON);
// 		return;
// 	}
// 
// 	const ItemBase *item_base = ITEMPOOL->GetItem(item_grid_data.item_id);
// 	if (NULL == item_base)
// 	{
// 		return;
// 	}
// 
// 	if (m_role->GetKnapsack()->ConsumeItemByIndex(grid_idx, 1, "PutOnCrossEquip"))
// 	{
// 		m_cross_equip_itemid = item_grid_data.item_id;
// 		m_cross_equip_gain_timestamp = item_grid_data.invalid_time - item_base->GetTimeLength();
// 		m_cross_equip_invalid_timestamp = item_grid_data.invalid_time;
// 
// 		gamelog::g_log_equipment.printf(LL_INFO, "PutOnCrossEquip:: User[%d, %s], item_id %d, item_gain_timestamp %u, invalid_timestamp %u",
// 			m_role->GetUID(), m_role->GetName(), (int)m_cross_equip_itemid, m_cross_equip_gain_timestamp, m_cross_equip_invalid_timestamp);
// 
// 		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
// 		this->SendEquipmentInfoToRole();
// 	}
}

void EquipmentManager::TakeOffCrossEquip()
{
// 	if (!m_role->GetKnapsack()->HaveSpace())
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 		return;
// 	}
// 
// 	const ItemBase *item_base = ITEMPOOL->GetItem(m_cross_equip_itemid);
// 	if (NULL == item_base)
// 	{
// 		return;
// 	}
// 
// 	ItemDataWrapper item_data_wrapper;
// 	item_data_wrapper.item_id = m_cross_equip_itemid;
// 	item_data_wrapper.num = 1;
// 	item_data_wrapper.is_bind = 1;
// 
// 	if (item_base->HasInvalidTime())
// 	{
// 		item_data_wrapper.invalid_time = m_cross_equip_gain_timestamp + item_base->GetTimeLength();
// 	}
// 
// 	if (m_role->GetKnapsack()->Put(item_data_wrapper, PUT_REASON_NO_NOTICE))
// 	{
// 		gamelog::g_log_equipment.printf(LL_INFO, "TakeOffCrossEquip:: User[%d, %s], item_id %d, item_gain_timestamp %u",
// 			m_role->GetUID(), m_role->GetName(), (int)m_cross_equip_itemid, m_cross_equip_gain_timestamp);
// 
// 		m_cross_equip_itemid = 0;
// 		m_cross_equip_gain_timestamp = 0;
// 		m_cross_equip_invalid_timestamp = 0;
// 		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
// 		this->SendEquipmentInfoToRole();
// 	}
}

void EquipmentManager::IsCanActShiZhuang(int *is_act_shengbin, int *is_act_fushi)
{
	if (NULL == is_act_shengbin || NULL == is_act_fushi) return;

	for (int i = 0; i < Equipment::E_INDEX_MAX; ++ i)
	{
		ItemGridData *item_grid_data = &m_equipment_list[i];
		if (NULL == item_grid_data || item_grid_data->Invalid())
		{
			continue;
		}

		const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_grid_data->item_id);
		if (NULL == equip_item)
		{
			continue;
		}

		if (m_role->GetPutOnLevel() < equip_item->GetLimitLevel())
		{
			continue;
		}

		if (SZ_ACT_SPECIAL_TYPE_SHENBIN == equip_item->GetActShizhuangType())
		{
			*is_act_shengbin = 1;
		}

		else if (SZ_ACT_SPECIAL_TYPE_FUSHI == equip_item->GetActShizhuangType())
		{
			*is_act_fushi = 1;
		}
	}
}

bool EquipmentManager::OnFumo(short fumo_id)
{
	return false;
// 	const EquipFumoConfig *fumo_cfg = GLOBALCONFIG->GetEquipConfig().GetFumoEquipCfg(fumo_id);
// 	if (NULL == fumo_cfg)
// 	{
// 		return false;
// 	}
// 	
// 	int find_same_type_num = 0;
// 	int fumo_index = -1;
// 	
// 	for (int i = 0; i < Equipment::E_INDEX_MAX; ++ i)
// 	{
// 		ItemGridData *item_grid_data = &m_equipment_list[i];
// 		if (item_grid_data->Invalid())
// 		{
// 			continue;
// 		}
// 
// 		const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_grid_data->item_id);
// 		if (NULL == equip_item)
// 		{
// 			continue;
// 		}
// 		
// 		EquipmentParam *item_param = (EquipmentParam*)item_grid_data->param;
// 
// 		if (fumo_cfg->equip_sub_type == equip_item->GetEquipType())
// 		{
// 			if (Equipment::E_TYPE_JIEZHI == fumo_cfg->equip_sub_type || Equipment::E_TYPE_XIANGLIAN == fumo_cfg->equip_sub_type)  //这些装备有两个。优先附在之前没附的装备上
// 			{
// 				if (0 == item_param->fumo_id || find_same_type_num >= 1)
// 				{
// 					fumo_index = i;
// 					item_param->fumo_id = fumo_cfg->id;
// 					item_param->param1 = static_cast<int>(EngineAdapter::Instance().Time()) + fumo_cfg->time;
// 				}
// 				else
// 				{
// 					++ find_same_type_num;
// 				}
// 
// 				continue;
// 			}
// 			else
// 			{
// 				++ find_same_type_num;
// 				fumo_index = i;
// 				item_param->fumo_id = fumo_cfg->id;
// 				item_param->param1 = static_cast<int>(EngineAdapter::Instance().Time()) + fumo_cfg->time;
// 
// 				break;
// 			}
// 		}
// 	}
// 
// 	if (fumo_index >= 0 && fumo_index < Equipment::E_INDEX_MAX)
// 	{
// 		this->SetEquipGridDirty(fumo_index);
// 		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
// 	}
// 
// 	return fumo_index >= 0 && fumo_index < Equipment::E_INDEX_MAX;
}

bool EquipmentManager::GetRandomSpeicalAttr(ItemGridData *item_grid_data)
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

// 	if ( equip_item->GetEquipType() > Equipment::E_TYPE_FEIXIAN_MIN || equip_item->GetEquipType() < Equipment::E_TYPE_FEIXIAN_MAX)
// 	{
// 		return GLOBALCONFIG->GetEquipConfig().GetRandomXianpin(item_grid_data);
// 	}

	if (equip_item->GetEquipType() >= Equipment::E_TYPE_ZHUANZHI_MIN && equip_item->GetEquipType() < Equipment::E_TYPE_ZHUANZHI_MAX)
	{
		return GLOBALCONFIG->GetEquipConfig().GetRandomXianpin(item_grid_data);
	}

	else if (Equipment::E_TYPE_SPECIAL_HUNJIE == equip_item->GetEquipType())
	{
		return LOGIC_CONFIG->GetQingyuanCfg().SetHunJieInitLevel(item_grid_data);
	}

	return false;
}

bool EquipmentManager::GetFixSpecialAttr(ItemGridData *item_grid_data, int getway, int getway_param1)
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

	if (Equipment::E_TYPE_SPECIAL_JINGLING == equip_item->GetEquipType())
	{
		return LOGIC_CONFIG->GetJingLingCfg().SetJingLingInitLevel(item_grid_data);
	}
	else if (Equipment::E_TYPE_SPECIAL_HUNJIE == equip_item->GetEquipType())
	{
		return LOGIC_CONFIG->GetQingyuanCfg().SetHunJieInitLevel(item_grid_data);
	}
	else if (equip_item->GetEquipType() >= Equipment::E_TYPE_ZHUANZHI_MIN && equip_item->GetEquipType() < Equipment::E_TYPE_ZHUANZHI_MAX)
	{
		return GLOBALCONFIG->GetEquipConfig().GetRandomXianpin(item_grid_data, getway, getway_param1);
	}

// 	else if (equip_item->GetEquipType() > Equipment::E_TYPE_FEIXIAN_MIN && equip_item->GetEquipType() < Equipment::E_TYPE_FEIXIAN_MAX)
// 	{
// 		return GLOBALCONFIG->GetEquipConfig().GetRandomXianpin(item_grid_data, getway, getway_param1);
// 	}

	return false;
}

// 改变装备格子强化数
bool EquipmentManager::AddEquiGridStrengthen(int equip_index, int value)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return false;

	if (m_equipment_grid_param.equi_grid_item_list[equip_index].strengthen_level >= EquipConfig::MAX_STRENGTH_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_FUNC_STRENGTHEN_REACH_LEVELBOUND);
		return false;
	}

	if (m_equipment_grid_param.equi_grid_item_list[equip_index].strengthen_level + value >=0 && 
		m_equipment_grid_param.equi_grid_item_list[equip_index].strengthen_level + value < EquipConfig::MAX_STRENGTH_LEVEL)
	{
		m_equipment_grid_param.equi_grid_item_list[equip_index].strengthen_level += value;
	}
	else
	{
		return false;
	}
	
	this->RefreshEquipInfo();

	//嘉年华
	m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_EQUIP_STRENGTHEN);
	m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_WEAR_EQUIPMENT); // 嘉年华

	return true;
}

bool EquipmentManager::AddEquiGridUplevel(int equip_index, int value)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return false;

	if (m_equipment_grid_param.equi_grid_item_list[equip_index].equip_level >= EquipConfig::MAX_STRENGTH_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_FUNC_STRENGTHEN_REACH_LEVELBOUND);
		return false;
	}

	if (m_equipment_grid_param.equi_grid_item_list[equip_index].equip_level + value >= 0 &&
		m_equipment_grid_param.equi_grid_item_list[equip_index].equip_level + value < EquipConfig::MAX_STRENGTH_LEVEL)
	{
		m_equipment_grid_param.equi_grid_item_list[equip_index].equip_level += value;
	}
	else
	{
		return false;
	}

	this->RefreshEquipInfo();
	m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_WEAR_EQUIPMENT); // 嘉年华
	return true;
}

// 装备格子加神铸
bool EquipmentManager::AddEquiGridShenzhu(int equip_index)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return false;
	m_equipment_grid_param.equi_grid_item_list[equip_index].shenzhu_level += 1;
	return true;
}

// 装备格子加星星
bool EquipmentManager::AddEquiGridStar(int equip_index)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return false;
	m_equipment_grid_param.equi_grid_item_list[equip_index].star_level += 1;
	return true;
}

// 装备格子星星加经验
bool EquipmentManager::SetEquiGridStarExp(int equip_index, int exp)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX || exp < 0) return false;
	m_equipment_grid_param.equi_grid_item_list[equip_index].star_exp = exp;
	return true;
}

// 装备格子加永恒等级
bool EquipmentManager::AddEquiGridEternityLevel(int equip_index, int value)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return false;
	m_equipment_grid_param.equi_grid_item_list[equip_index].eternity_level += (short)value;
	return true;
}

// 返回格子强化等级
int EquipmentManager::GetEquipGridStrengthenLevel(int equip_index)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return 0;
	return m_equipment_grid_param.equi_grid_item_list[equip_index].strengthen_level;
}

// 返回格子装备等级
int EquipmentManager::GetEquipGridUpLevel(int equip_index)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return 0;
	return m_equipment_grid_param.equi_grid_item_list[equip_index].equip_level;
}

// 返回格子神铸等级
int EquipmentManager::GetEquipGridShenzhuLevel(int equip_index)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return 0;
	return m_equipment_grid_param.equi_grid_item_list[equip_index].shenzhu_level;
}

// 返回格子星星等级
int EquipmentManager::GetEquipGridStarLevel(int equip_index)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return 0;
	return m_equipment_grid_param.equi_grid_item_list[equip_index].star_level;
}

// 返回格子星星经验
int EquipmentManager::GetEquipGridStarExp(int equip_index)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return 0;
	return m_equipment_grid_param.equi_grid_item_list[equip_index].star_exp;
}

// 返回格子永恒等级
int EquipmentManager::GetEquipGridEternityLevel(int equip_index)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return 0;
	return m_equipment_grid_param.equi_grid_item_list[equip_index].eternity_level;
}

// 获取装备高品质仙品属性的数量(星星数)
short EquipmentManager::GetXianPinHightQualityCount(const unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM], bool is_calc)
{
	if (NULL == xianpin_type_list) return 0;

	short xian_pin_count = 0;
	if (is_calc)
	{
		for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; i++)
		{
			if (xianpin_type_list[i] > 0)
			{
				const EquipXianPinConfig *xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(xianpin_type_list[i]);
						
				if (NULL != xianpin_cfg && QUALITY_COLOR_TYPE_BLUE == xianpin_cfg->color)
				{
					xian_pin_count++;
				}
			}
		}
	}

	return xian_pin_count;
}

int EquipmentManager::GetStatisfyColorAndGradeEquipCount(int color, int order)
{
	if (ItemBase::I_COLOR_INVALID >= color || ItemBase::I_COLOR_MAX <= color)
	{
		return 0;
	}

	if (order < 0 || order >= ItemPool::MAX_EQUIPMENT_ORDER)
	{
		return 0;
	}

	int statisfy_equip_count = 0;
	for(int equip_part = Equipment::E_INDEX_TOUKUI; equip_part < Equipment::E_INDEX_MAX; equip_part++)
	{
		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_equipment_list[equip_part].item_id);
		if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()) continue;

		if (equip->GetColor() < color) continue;
		if (equip->GetOrder() < order) continue;

		statisfy_equip_count++;
	}

	return statisfy_equip_count;
}

const Equipment * EquipmentManager::GetEquipBaseInfoByIndex(int equip_index)
{
	if (equip_index < Equipment::E_INDEX_TOUKUI || equip_index >= Equipment::E_INDEX_MAX)
	{
		return NULL;
	}

	/*const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_equipment_list[equip_index].item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType())
	{
		return 0;
	}

	return equip;*/
	return dynamic_cast<const Equipment*>(ITEMPOOL->GetItem(m_equipment_list[equip_index].item_id));
}

int EquipmentManager::GetEquipColor(int equip_index)
{
	if (equip_index < Equipment::E_INDEX_TOUKUI || equip_index >= Equipment::E_INDEX_MAX)
	{
		return 0;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_equipment_list[equip_index].item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType())
	{
		return 0;
	}

	return equip->GetColor();
}

int EquipmentManager::GetEquipNoticeColor(int equip_index)
{
	bool broadcast_color = false;
	const DailyChongzhiRewradConfig *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiRewradConfig(m_role->GetLevel(), 0);
	if (NULL != cfg)
	{
		if (m_role->GetRoleActivity()->GetParam()->history_chongzhi >= static_cast<long long>(cfg->need_total_chongzhi))
		{
			broadcast_color = true;
		}
	}

	if (broadcast_color && this->GetEquipOrder(equip_index) > 3)
	{
		return this->GetEquipColor(equip_index);
	}

	return 0;
}

int EquipmentManager::GetEquipOrder(int equip_index)
{
	if (equip_index < Equipment::E_INDEX_TOUKUI || equip_index >= Equipment::E_INDEX_MAX)
	{
		return 0;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_equipment_list[equip_index].item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType())
	{
		return 0;
	}

	return equip->GetOrder();
}

void EquipmentManager::SetItemGridData(int equip_index, ItemGridData grid_data)
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return;

	const ItemBase *item_base = ITEMPOOL->GetItem(grid_data.item_id);
	if (NULL == item_base || item_base->GetItemType() != ItemBase::I_TYPE_EQUIPMENT) return;

	const Equipment *equipment = (const Equipment *)(item_base);
	if (!Equipment::IsMatchIndex(equipment->GetEquipType(), equip_index)) return;

	m_equipment_list[equip_index].item_id = grid_data.item_id;
	m_equipment_list[equip_index].num = grid_data.num;
	m_equipment_list[equip_index].is_bind = grid_data.is_bind;
	m_equipment_list[equip_index].invalid_time = grid_data.invalid_time;
	m_equipment_list[equip_index].gold_price = grid_data.gold_price;

	EquipmentParam *equip_param = static_cast<EquipmentParam*>(m_equipment_list[equip_index].param);
	EquipmentParam *grid_equip_param = static_cast<EquipmentParam*>(grid_data.param);
	if (NULL != equip_param && NULL != grid_equip_param)
	{
		for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; i++)
		{
			equip_param->xianpin_type_list[i] = grid_equip_param->xianpin_type_list[i];
		}
	}

	this->SetEquipGridDirty(equip_index, true, Protocol::SCEquipmentItemChange::SEND_REASON_COMPOUND);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
}

int EquipmentManager::GetRedEquipCount(int grade)
{
	int count = 0;
	for (int i = 0; i < Equipment::E_INDEX_MAX; ++i)
	{
		if (m_equipment_list[i].Invalid())
		{
			continue;
		}

		const ItemBase *item_base = ITEMPOOL->GetItem(m_equipment_list[i].item_id);
		if (NULL == item_base || ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType())
		{
			continue;
		}

		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_equipment_list[i].item_id);
		if (ItemBase::I_COLOR_RED <= item_base->GetColor() && equip->GetOrder() >= grade)
		{
			++count;
		}
	}
	return count;
}

int EquipmentManager::GetEquipCount(int level)
{
	int count = 0;
	for (int i = 0; i < Equipment::E_INDEX_MAX; ++i)
	{
		if (m_equipment_list[i].Invalid())
		{
			continue;
		}

		const ItemBase *item_base = ITEMPOOL->GetItem(m_equipment_list[i].item_id);
		if (NULL == item_base || ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType())
		{
			continue;
		}

		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_equipment_list[i].item_id);
		if (equip->GetLimitLevel() >= level)
		{
			++count;
		}
	}
	return count;
}

void EquipmentManager::AddEquipmentSkill(const ItemID item_id)
{
 	if (NULL == ITEMPOOL->GetItem(item_id))
 	{
 		return;
 	}
 
	int skill_id = LOGIC_CONFIG->GetOtherCfg().GetSkillIDWithItemId(item_id);
	if (NULL != SKILLPOOL->GetSkill(skill_id))
	{
		m_role->GetSkillManager()->AddEquipmentSkill(skill_id, 1);
	}
}

void EquipmentManager::DelEquipmentSkill(const ItemID item_id)
{
 	if (NULL == ITEMPOOL->GetItem(item_id))
 	{
 		return;
 	}
	
	int skill_id = LOGIC_CONFIG->GetOtherCfg().GetSkillIDWithItemId(item_id);
	if (NULL != SKILLPOOL->GetSkill(skill_id))
	{
 		m_role->GetSkillManager()->RemoveSkill(skill_id);
 	}
}

void EquipmentManager::CheckFuncOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (m_role->GetRoleBigSmallGoal()->IsFuncActive(ROLE_BIG_SMALL_GOAL_SYSTEM_EQUIP_STRENGTHEN))
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_EQUIPMENT_FORGE);
	if (fun_open_cfg != nullptr && opengame_day >= fun_open_cfg->opengame_day)
	{
		if (m_role->GetLevel() >= fun_open_cfg->role_level || m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
		{
			m_role->GetRoleBigSmallGoal()->OnOpenFunc(ROLE_BIG_SMALL_GOAL_SYSTEM_EQUIP_STRENGTHEN);
		}
	}
}

void EquipmentManager::ReCalcEquipUplevelSuitCount()
{
	memset(m_equip_level_count, 0, sizeof(m_equip_level_count));

	for (int i = 0; i < Equipment::E_INDEX_MAX; ++i)
	{
		if (m_equipment_list[i].Invalid())
		{
			continue;
		}

		const ItemBase *item_base = ITEMPOOL->GetItem(m_equipment_list[i].item_id);
		if (NULL == item_base || ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType())
		{
			continue;
		}

		const Equipment *equip = (const Equipment *)item_base;
		if (equip->GetOrder() >= 0 && equip->GetOrder() < EquipConfig::UPLEVEL_SUIT_COUNT)
		{
			if (equip->GetOrder() >= 2)
			{
				for (int j = equip->GetOrder(); j >= 1; j--)		//套装向下兼容
				{
					++m_equip_level_count[j];
				}
			}
			else if (equip->GetOrder() == 1)
			{
				++m_equip_level_count[equip->GetOrder()];
			}
		}
	}

	//for (int suit_level = 1; suit_level < EquipConfig::UPLEVEL_SUIT_COUNT; suit_level++)
	//{
	//	const EquipUpLevelSuitAttrCfg *suit_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipUplevelSuitCfg(suit_level, m_equip_level_count[suit_level]);
	//	if (suit_cfg != NULL)
	//	{
	//		m_equip_grade_suit_cur_level = suit_level;
	//	}
	//}
}

void EquipmentManager::SendEquipGradeSuitInfo()
{
	Protocol::SCEquipUplevelSuitInfo info;
	info.suit_level = m_equipment_grid_param.cur_grade_suit_level;
	info.suit_active_flag = SET_BIT(m_equip_grade_suit_active_flag, m_equipment_grid_param.cur_grade_suit_level);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}

int EquipmentManager::GetEquipNumLevelBigger(int level)
{
	int count = 0;
	for (int i = Equipment::E_INDEX_TOUKUI; i < Equipment::E_INDEX_MAX; i++)
	{
		if (m_equipment_list[i].Invalid()) continue;
	
		const ItemBase *new_itembase = (const Equipment *)ITEMPOOL->GetItem(m_equipment_list[i].item_id);
		if (NULL == new_itembase || new_itembase->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
		{
			continue;
		}
		const Equipment *new_equip = (const Equipment *)new_itembase;
		if (new_equip != NULL && new_equip->GetLimitLevel() >= level)
		{
			count++;
		}
	}
	return count;
}

void EquipmentManager::ActiveEquipGradeSuit(int level)
{
	if (level < 0 || level >= EquipConfig::UPLEVEL_SUIT_COUNT)
	{
		return;
	}

	const EquipUpLevelSuitAttrCfg *suit_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipUplevelSuitCfg(level, m_equip_level_count[level]);
	if (suit_cfg != NULL)
	{
		m_equipment_grid_param.cur_grade_suit_level = level;

		this->SendEquipGradeSuitInfo();

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
	}
}
