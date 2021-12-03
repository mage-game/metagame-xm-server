#include "equipsuit.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/equipsuit/equipsuitconfig.hpp"
#include "../capability/capability.hpp"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "obj/character/role.h"
#include "protocal/msgequipment.h"
#include "world.h"
#include "equipment/equipmentmanager.h"
#include "item/itemextern.hpp"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"

EquipSuit::EquipSuit(): m_role(NULL)
{
	
}

EquipSuit::~EquipSuit()
{
	
}

void EquipSuit::SetRole(Role *role)
{
	m_role = role;
}

void EquipSuit::Init(Role *role, const EquipSuitParam &equipsuit_param)
{
	m_role = role;
	m_equipsuit_param = equipsuit_param;
}

void EquipSuit::GetInitParam(EquipSuitParam *equipsuit_param)
{
	if (NULL != equipsuit_param)
	{
		*equipsuit_param = m_equipsuit_param;
	}
}

void EquipSuit::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		this->ReCalcSuitCount();

		std::map<short, SuitTypeCount>::iterator it;
		for (it = m_suit_ss_map.begin(); m_suit_ss_map.end() != it; ++ it)
		{
			const SuitTypeCount suit_info = it->second;

			const EquipSuitAttrCfg * suit_attr_cfg = LOGIC_CONFIG->GetEquipSuitConfig().GetEquipShiShiSuitCfg(suit_info.suit_id, suit_info.suit_count);
			if (NULL != suit_attr_cfg)
			{
				// 属性比例加成
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_attr_cfg->maxhp_attr * EQUIP_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_attr_cfg->gongji_attr * EQUIP_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += suit_attr_cfg->fangyu_attr * EQUIP_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += suit_attr_cfg->mingzhong_attr * EQUIP_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += suit_attr_cfg->shanbi_attr * EQUIP_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += suit_attr_cfg->baoji_attr * EQUIP_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += suit_attr_cfg->jianren_attr * EQUIP_ATTR_PER;

				// 强化属性
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_attr_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_attr_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += suit_attr_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += suit_attr_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += suit_attr_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += suit_attr_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += suit_attr_cfg->jianren;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] += suit_attr_cfg->fujiashanghai;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI] += suit_attr_cfg->dikangshanghai;
			}
		}

		std::map<short, SuitTypeCount>::iterator iter;
		for (iter = m_suit_cq_map.begin(); m_suit_cq_map.end() != iter; ++ iter)
		{
			const SuitTypeCount suit_info = iter->second;

			const EquipSuitAttrCfg * suit_attr_cfg = LOGIC_CONFIG->GetEquipSuitConfig().GetEquipChuanQiSuitCfg(suit_info.suit_id, suit_info.suit_count);
			if (NULL != suit_attr_cfg)
			{
				// 属性比例加成
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_attr_cfg->maxhp_attr * EQUIP_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_attr_cfg->gongji_attr * EQUIP_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += suit_attr_cfg->fangyu_attr * EQUIP_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += suit_attr_cfg->mingzhong_attr * EQUIP_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += suit_attr_cfg->shanbi_attr * EQUIP_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += suit_attr_cfg->baoji_attr * EQUIP_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += suit_attr_cfg->jianren_attr * EQUIP_ATTR_PER;

				// 强化属性
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_attr_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_attr_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += suit_attr_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += suit_attr_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += suit_attr_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += suit_attr_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += suit_attr_cfg->jianren;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] += suit_attr_cfg->fujiashanghai;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI] += suit_attr_cfg->dikangshanghai;
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_EQUIPSUIT, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

void EquipSuit::ReCalcSuitCount()
{
	std::map<short, SuitTypeCount> suit_ss_map;					// 史诗套装列表
	std::map<short, SuitTypeCount> suit_cq_map;					// 传奇套装列表

	for (int i = 0; i < Equipment::E_INDEX_MAX; i++)
	{
		if (m_equipsuit_param.equip_suit_level_list[i] > 0)
		{
			ItemGridData equip_data;
			if (!m_role->GetEquipmentManager()->GetEquipmentGridData(i, &equip_data))
			{
				continue;
			}

			const SuitEquipUplevelCfg *uplevel_cfg = LOGIC_CONFIG->GetEquipSuitConfig().GetSuitUpLevelCfg(equip_data.item_id);
			if (NULL == uplevel_cfg)
			{
				continue;
			}
			
			if (EQUIP_SUIT_LEVEL_TYPE_SHISHI == m_equipsuit_param.equip_suit_level_list[i])
			{
				suit_ss_map[uplevel_cfg->suit_id].suit_id = uplevel_cfg->suit_id;
				suit_ss_map[uplevel_cfg->suit_id].suit_count ++;

				const EquipSuitAttrCfg * suit_attr_cfg = LOGIC_CONFIG->GetEquipSuitConfig().GetEquipShiShiSuitCfg(suit_ss_map[uplevel_cfg->suit_id].suit_id, suit_ss_map[uplevel_cfg->suit_id].suit_count);
				if (NULL == suit_attr_cfg)
				{
					continue;
				}

				if (suit_attr_cfg->is_broacast)
				{	
					std::map<short, SuitTypeCount>::const_iterator iter = m_suit_ss_map.find(uplevel_cfg->suit_id);
					if (iter != m_suit_ss_map.end())
					{
						if (suit_ss_map[uplevel_cfg->suit_id].suit_count > iter->second.suit_count)
						{
							int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_equip_suit_shishi_active_content, 
								UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), (int)uplevel_cfg->suit_id);
							if (length > 0)
							{
								World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
							}
						}
					}
				}
			}
			else if (EQUIP_SUIT_LEVEL_TYPE_CHUANSHUO == m_equipsuit_param.equip_suit_level_list[i])
			{
				// 策划要求改成史诗和传说的属性加成不叠加
// 				suit_ss_map[uplevel_cfg->suit_id].suit_id = uplevel_cfg->suit_id;
// 				suit_ss_map[uplevel_cfg->suit_id].suit_count ++;

				suit_cq_map[uplevel_cfg->suit_id].suit_id = uplevel_cfg->suit_id;
				suit_cq_map[uplevel_cfg->suit_id].suit_count ++;
				
				const EquipSuitAttrCfg * suit_attr_cfg = LOGIC_CONFIG->GetEquipSuitConfig().GetEquipChuanQiSuitCfg(suit_cq_map[uplevel_cfg->suit_id].suit_id, suit_cq_map[uplevel_cfg->suit_id].suit_count);
				if (NULL == suit_attr_cfg)
				{
					continue;
				}

				if (suit_attr_cfg->is_broacast)
				{
					std::map<short, SuitTypeCount>::const_iterator iter = m_suit_cq_map.find(uplevel_cfg->suit_id);
					if (iter != m_suit_cq_map.end())
					{
						if (suit_cq_map[uplevel_cfg->suit_id].suit_count > iter->second.suit_count)
						{
							int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_equip_suit_chuanshuo_active_content, 
								UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), (int)uplevel_cfg->suit_id);
							if (length > 0)
							{
								World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
							}
						}
					}
				}
			}
		}
	}

	m_suit_ss_map.clear();
	m_suit_ss_map = suit_ss_map;

	m_suit_cq_map.clear();
	m_suit_cq_map = suit_cq_map;
}

UNSTD_STATIC_CHECK(Equipment::E_INDEX_MAX == MAX_SUIT_EQUIP_PART);

void EquipSuit::OnUpSuitEquip(int equip_part)
{
	if (equip_part < 0 || equip_part >= Equipment::E_INDEX_MAX)
	{
		return;
	}

	if (m_equipsuit_param.equip_suit_level_list[equip_part] >= MAX_EQUIP_SUIT_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_SUIT_UPTO_MAX_SUIT_LEVEL);
		return;
	}

	int old_suit_level = m_equipsuit_param.equip_suit_level_list[equip_part];

	ItemGridData equip_data;
	if (!m_role->GetEquipmentManager()->GetEquipmentGridData(equip_part, &equip_data))
	{
		return;
	}

	const SuitEquipUplevelCfg *uplevel_cfg = LOGIC_CONFIG->GetEquipSuitConfig().GetSuitUpLevelCfg(equip_data.item_id);
	if (NULL == uplevel_cfg)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_SUIT_CANNOT_UP_TO_SUIT);
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	short stuff_count = 0;
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	
	if (m_equipsuit_param.equip_suit_level_list[equip_part] <= 0)
	{
		m_equipsuit_param.equip_suit_level_list[equip_part] = 0;

		stuff_list[stuff_count].item_id = uplevel_cfg->need_stuff_id_ss;
		stuff_list[stuff_count].num = uplevel_cfg->need_stuff_count_ss;
		stuff_list[stuff_count].buyable = false;
		++ stuff_count;
	}
	else if (m_equipsuit_param.equip_suit_level_list[equip_part] < MAX_EQUIP_SUIT_LEVEL)
	{
		stuff_list[stuff_count].item_id = uplevel_cfg->need_stuff_id_cq1;
		stuff_list[stuff_count].num = uplevel_cfg->need_stuff_count_cq1;
		stuff_list[stuff_count].buyable = false;
		++ stuff_count;

		stuff_list[stuff_count].item_id = uplevel_cfg->need_stuff_id_cq2;
		stuff_list[stuff_count].num = uplevel_cfg->need_stuff_count_cq2;
		stuff_list[stuff_count].buyable = false;
		++ stuff_count;
	}

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
	{
		return;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "EquipSuit::OnUpSuitEquip"))
	{
		return;
	}

	m_equipsuit_param.equip_suit_level_list[equip_part] ++;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIP_SUIT);
	
	this->SendToClient();

	gamelog::g_log_equip_suit.printf(LL_INFO, "EquipSuit::OnUpSuitEquip user %d %s equip_part %d old_suit_level %d new_suit_level %d",
		m_role->GetUID(), m_role->GetName(), equip_part, old_suit_level, m_equipsuit_param.equip_suit_level_list[equip_part]);
}

void EquipSuit::OnEquipPutOn(int equip_part, ItemID old_equip_id, ItemID new_equip_id)
{
	if (equip_part < 0 || equip_part >= Equipment::E_INDEX_MAX)
	{
		return;
	}

	UNSTD_STATIC_CHECK(2 == MAX_EQUIP_SUIT_LEVEL);

	const SuitEquipUplevelCfg *old_uplevel_cfg = LOGIC_CONFIG->GetEquipSuitConfig().GetSuitUpLevelCfg(old_equip_id);
	if (NULL == old_uplevel_cfg)
	{
		return;
	}

	int old_suit_level = m_equipsuit_param.equip_suit_level_list[equip_part];
	
	short remain_stuff_count_ss = -1;
	short remain_stuff_count_qc1 = -1;
	short remain_stuff_count_qc2 = -1;

	const SuitEquipUplevelCfg *new_uplevel_cfg = LOGIC_CONFIG->GetEquipSuitConfig().GetSuitUpLevelCfg(new_equip_id);
	if (NULL != new_uplevel_cfg)
	{
		remain_stuff_count_ss = old_uplevel_cfg->need_stuff_count_ss - new_uplevel_cfg->need_stuff_count_ss;
		remain_stuff_count_qc1 = old_uplevel_cfg->need_stuff_count_cq1 - new_uplevel_cfg->need_stuff_count_cq1;
		remain_stuff_count_qc2 = old_uplevel_cfg->need_stuff_count_cq2 - new_uplevel_cfg->need_stuff_count_cq2;
	}

	ItemConfigData return_item_list[MAX_ATTACHMENT_ITEM_NUM];

	if (EQUIP_SUIT_LEVEL_TYPE_SHISHI == m_equipsuit_param.equip_suit_level_list[equip_part])
	{
		if (remain_stuff_count_ss < 0)
		{
			m_equipsuit_param.equip_suit_level_list[equip_part] = 0;

			return_item_list[0].item_id = old_uplevel_cfg->need_stuff_id_ss;
			return_item_list[0].num = old_uplevel_cfg->need_stuff_count_ss;
			return_item_list[0].is_bind = true;

			this->PutInKnapsack(return_item_list);
		}
		else if (remain_stuff_count_ss > 0)
		{
			return_item_list[0].item_id = old_uplevel_cfg->need_stuff_id_ss;
			return_item_list[0].num = remain_stuff_count_ss;
			return_item_list[0].is_bind = true;
			this->PutInKnapsack(return_item_list);
		}
	}
	else if (EQUIP_SUIT_LEVEL_TYPE_CHUANSHUO == m_equipsuit_param.equip_suit_level_list[equip_part])
	{
		if (remain_stuff_count_ss < 0 || remain_stuff_count_qc1 < 0 || remain_stuff_count_qc2 < 0)
		{
			m_equipsuit_param.equip_suit_level_list[equip_part] = 0;

			return_item_list[0].item_id = old_uplevel_cfg->need_stuff_id_ss;
			return_item_list[0].num = old_uplevel_cfg->need_stuff_count_ss;
			return_item_list[0].is_bind = true;

			return_item_list[1].item_id = old_uplevel_cfg->need_stuff_id_cq1;
			return_item_list[1].num = old_uplevel_cfg->need_stuff_count_cq1;
			return_item_list[1].is_bind = true;

			return_item_list[2].item_id = old_uplevel_cfg->need_stuff_id_cq2;
			return_item_list[2].num = old_uplevel_cfg->need_stuff_count_cq2;
			return_item_list[2].is_bind = true;

			this->PutInKnapsack(return_item_list);
		}
		else
		{
			return_item_list[0].item_id = old_uplevel_cfg->need_stuff_id_ss;
			return_item_list[0].num = remain_stuff_count_ss;
			return_item_list[0].is_bind = true;

			return_item_list[1].item_id = old_uplevel_cfg->need_stuff_id_cq1;
			return_item_list[1].num = remain_stuff_count_qc1;
			return_item_list[1].is_bind = true;

			return_item_list[2].item_id = old_uplevel_cfg->need_stuff_id_cq2;
			return_item_list[2].num = remain_stuff_count_qc2;
			return_item_list[2].is_bind = true;

			this->PutInKnapsack(return_item_list);
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIP_SUIT);

	this->SendToClient();

	gamelog::g_log_equip_suit.printf(LL_INFO, "EquipSuit::OnEquipPutOn user %d %s equip_part %d old_suit_level %d new_suit_level %d",
		m_role->GetUID(), m_role->GetName(), equip_part, old_suit_level, m_equipsuit_param.equip_suit_level_list[equip_part]);
}

void EquipSuit::OnEquipTakeOff(int equip_part, ItemID equip_id)
{
	if (equip_part < 0 || equip_part >= Equipment::E_INDEX_MAX)
	{
		return;
	}
	
	UNSTD_STATIC_CHECK(2 == MAX_EQUIP_SUIT_LEVEL);

	if (m_equipsuit_param.equip_suit_level_list[equip_part] <= 0)
	{
		return;
	}

	int old_suit_level = m_equipsuit_param.equip_suit_level_list[equip_part];

	const SuitEquipUplevelCfg *old_uplevel_cfg = LOGIC_CONFIG->GetEquipSuitConfig().GetSuitUpLevelCfg(equip_id);
	if (NULL == old_uplevel_cfg)
	{
		return;
	}
	
	ItemConfigData return_item_list[MAX_ATTACHMENT_ITEM_NUM];
	//计算返还
	if (EQUIP_SUIT_LEVEL_TYPE_SHISHI == m_equipsuit_param.equip_suit_level_list[equip_part])
	{
		return_item_list[0].item_id = old_uplevel_cfg->need_stuff_id_ss;
		return_item_list[0].num = old_uplevel_cfg->need_stuff_count_ss;
		return_item_list[0].is_bind = true;
	}
	else if (EQUIP_SUIT_LEVEL_TYPE_CHUANSHUO == m_equipsuit_param.equip_suit_level_list[equip_part])
	{
		return_item_list[0].item_id = old_uplevel_cfg->need_stuff_id_ss;
		return_item_list[0].num = old_uplevel_cfg->need_stuff_count_ss;
		return_item_list[0].is_bind = true;

		return_item_list[1].item_id = old_uplevel_cfg->need_stuff_id_cq1;
		return_item_list[1].num = old_uplevel_cfg->need_stuff_count_cq1;
		return_item_list[1].is_bind = true;

		return_item_list[2].item_id = old_uplevel_cfg->need_stuff_id_cq2;
		return_item_list[2].num = old_uplevel_cfg->need_stuff_count_cq2;
		return_item_list[2].is_bind = true;
	}

	m_equipsuit_param.equip_suit_level_list[equip_part] = 0;

	this->PutInKnapsack(return_item_list);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIP_SUIT);

	this->SendToClient();

	gamelog::g_log_equip_suit.printf(LL_INFO, "EquipSuit::OnEquipTakeOff user %d %s equip_part %d old_suit_level %d new_suit_level %d",
		m_role->GetUID(), m_role->GetName(), equip_part, old_suit_level, m_equipsuit_param.equip_suit_level_list[equip_part]);
}

void EquipSuit::OnSuitEquipOperate(short operate_typ, short equip_index)
{
	if (Protocol::CSDuanzaoSuitReq::EQUIPMENT_SUIT_OPERATE_TYPE_INFO_REQ == operate_typ)
	{
		this->SendToClient();
	}
	else if (Protocol::CSDuanzaoSuitReq::EQUIPMENT_SUIT_OPERATE_TYPE_EQUIP_UP == operate_typ)
	{
		this->OnUpSuitEquip(equip_index);
	}
}

void EquipSuit::SendToClient()
{
	Protocol::SCDuanzaoSuitInfo esi;

	for (int i = 0; i < MAX_SUIT_EQUIP_PART; ++ i)
	{
		esi.suit_level_list[i] = m_equipsuit_param.equip_suit_level_list[i];
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&esi, sizeof(esi));	
}

bool EquipSuit::IsSatisfyMagicalPrecious(int order, int equip_part)
{
	if (equip_part < 0 || equip_part >= MAX_SUIT_EQUIP_PART) return false;

	if (order <= 0 || order >= ItemPool::MAX_EQUIPMENT_ORDER) return false;

	if (m_equipsuit_param.equip_suit_level_list[equip_part] <= 0) return false;

	ItemGridData equip_data;
	if (!m_role->GetEquipmentManager()->GetEquipmentGridData(equip_part, &equip_data))
	{
		return false;
	}

	const SuitEquipUplevelCfg *uplevel_cfg = LOGIC_CONFIG->GetEquipSuitConfig().GetSuitUpLevelCfg(equip_data.item_id);
	if (NULL == uplevel_cfg)
	{
		return false;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(equip_data.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()) return false;

	if (equip->GetOrder() < order) return false;

	return true;
}

int EquipSuit::GetSuitPartCount(int suit_id)
{
	int suit_count = 0;

	std::map<short, SuitTypeCount>::iterator it = m_suit_ss_map.find(suit_id);
	if (m_suit_ss_map.end() != it)
	{
		suit_count += it->second.suit_count;
	}

	it = m_suit_cq_map.find(suit_id);
	if (m_suit_cq_map.end() != it)
	{
		suit_count += it->second.suit_count;
	}

	return suit_count;
}

void EquipSuit::PutInKnapsack(ItemConfigData item_list[MAX_ATTACHMENT_ITEM_NUM])
{
	int item_count[MAX_ATTACHMENT_ITEM_NUM];
	for(int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
	{
		item_count[i] = 0;
		if (0 < item_list[i].num)
		{
			item_count[i] = item_list[i].num;
		}
	}

	while(0 < item_count[0] || 0 < item_count[1] || 0 < item_count[2])
	{
		for(int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(item_list[i].item_id);
			if (NULL == itembase)
			{
				item_count[i] = 0;
				continue;
			}

			int pile_count = itembase->GetPileLimit();
			if (pile_count <= 0)
			{
				item_count[i] = 0;
				continue;
			}

			item_count[i] = item_list[i].num;
			if (item_count[i] > pile_count)
			{
				item_list[i].num = pile_count;
				item_count[i] -= pile_count;
			}
			else
			{
				item_count[i] = 0;
			}
		}

		m_role->GetKnapsack()->PutListOrMail(item_list, PUT_REASON_TAKEOFF_SUIT_RETURN);

		for(int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			item_list[i].num = item_count[i];
			if (0 == item_count[i])
			{
				item_list[i].item_id = 0;
				item_list[i].is_bind = 0;
			}
		}
	}
}