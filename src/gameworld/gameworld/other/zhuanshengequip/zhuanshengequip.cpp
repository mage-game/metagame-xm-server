#include "zhuanshengequip.hpp"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "config/zhuanzhiconfig.hpp"
#include "zhuanshengequipconfig.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "effect/effectbase.hpp"
#include "monster/drop/droppool.hpp"
#include "globalconfig/globalconfig.h"
#include "protocal/msgzhuanshengequip.h"
#include "gamelog.h"
#include "servercommon/noticenum.h"
#include "skill/skillbase.hpp"

#include "item/itempool.h"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"

ZhuanShengEquip::ZhuanShengEquip() : m_role(NULL), m_next_allow_autoconbine_time(0), m_add_exp_per(0), m_add_coin_per(0),
						m_per_to_role_shang(0), m_per_to_role_mianshang(0)
{

}

ZhuanShengEquip::~ZhuanShengEquip()
{

}

void ZhuanShengEquip::InitParam(Role *role, const ZhuanShengEquipParam &param)
{
	m_role = role;
	m_param = param;
}


void ZhuanShengEquip::GetInitParam(ZhuanShengEquipParam *param)
{
	*param = m_param;
}

void ZhuanShengEquip::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	return;
	if (is_recalc)
	{
		m_attrs_add.Reset();
	
		m_add_exp_per = 0;
		m_add_coin_per = 0;
		int min_equip_grade = ZHUANSHENG_EQUIP_GRADE_LIMIT;

		{
			// 转生装备
			for (int equip_index = 0; equip_index < ZHUANSHENG_EQUIP_TYPE_MAX; ++ equip_index)
			{
				ItemDataWrapper &itemdata = m_param.zhuansheng_equip_list[equip_index];
				if (itemdata.Invalid())
				{
					min_equip_grade = 0;
					continue;
				}

				const Equipment *equip_item = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);

				if (NULL != equip_item && ItemBase::I_TYPE_EQUIPMENT == equip_item->GetItemType() &&
					equip_item->GetEquipType() >= Equipment::E_TYPE_ZHUANSHENG_MIN && equip_item->GetEquipType() < Equipment::E_TYPE_ZHUANSHENG_MAX)
				{
					equip_item->GetBaseAttr(m_attrs_add);

					if (itemdata.param_data.rand_attr_type_1 > 0 && itemdata.param_data.rand_attr_val_1 > 0 && CharIntAttrs::IsRoleBaseAttr(itemdata.param_data.rand_attr_type_1))
					{
//						m_attrs_add.m_attrs[itemdata.param_data.rand_attr_type_1] += itemdata.param_data.rand_attr_val_1;
						m_attrs_add.AddValue(itemdata.param_data.rand_attr_type_1, itemdata.param_data.rand_attr_val_1);
					}

					if (itemdata.param_data.rand_attr_type_2 > 0 && itemdata.param_data.rand_attr_val_2 > 0 && CharIntAttrs::IsRoleBaseAttr(itemdata.param_data.rand_attr_type_2))
					{
//						m_attrs_add.m_attrs[itemdata.param_data.rand_attr_type_2] += itemdata.param_data.rand_attr_val_2;
						m_attrs_add.AddValue(itemdata.param_data.rand_attr_type_2, itemdata.param_data.rand_attr_val_2);
					}

					if (itemdata.param_data.rand_attr_type_3 > 0 && itemdata.param_data.rand_attr_val_3 > 0 && CharIntAttrs::IsRoleBaseAttr(itemdata.param_data.rand_attr_type_3))
					{
//						m_attrs_add.m_attrs[itemdata.param_data.rand_attr_type_3] += itemdata.param_data.rand_attr_val_3;
						m_attrs_add.AddValue(itemdata.param_data.rand_attr_type_3, itemdata.param_data.rand_attr_val_3);
					}
					
					if (ZRS_TYPE_ADDEXP_PER == itemdata.param_data.rand_attr_type_1) m_add_exp_per += itemdata.param_data.rand_attr_val_1;
					if (ZRS_TYPE_ADDEXP_PER == itemdata.param_data.rand_attr_type_2) m_add_exp_per += itemdata.param_data.rand_attr_val_2;
					if (ZRS_TYPE_ADDEXP_PER == itemdata.param_data.rand_attr_type_3) m_add_exp_per += itemdata.param_data.rand_attr_val_3;
				
					if (ZRS_TYPE_ADDCOIN_PER == itemdata.param_data.rand_attr_type_1) m_add_coin_per += itemdata.param_data.rand_attr_val_1;
					if (ZRS_TYPE_ADDCOIN_PER == itemdata.param_data.rand_attr_type_2) m_add_coin_per += itemdata.param_data.rand_attr_val_2;
					if (ZRS_TYPE_ADDCOIN_PER == itemdata.param_data.rand_attr_type_3) m_add_coin_per += itemdata.param_data.rand_attr_val_3;

					if (equip_item->GetOrder() < min_equip_grade)
					{
						min_equip_grade = equip_item->GetOrder();
					}
				}
			}
		}

		{
			const ZhuanShengEquipSuitAddAttr *suit_add_attr = LOGIC_CONFIG->GetZhuanShengCfg().GetEquipSuitAddAttr(min_equip_grade);
			if (NULL != suit_add_attr)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_add_attr->max_hp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_add_attr->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += suit_add_attr->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += suit_add_attr->baoji;
// 				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += suit_add_attr->per_pofang;
// 				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += suit_add_attr->per_mianshang;

				// 下面特殊属性用乘法
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, suit_add_attr->per_mianshang);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, suit_add_attr->per_pofang);

				m_per_to_role_shang = suit_add_attr->per_to_role_shang;
				m_per_to_role_mianshang = suit_add_attr->per_to_role_mianshang;
			}
		}

		//{
		//	const ZhuanshengOtherConfig other_cfg = LOGIC_CONFIG->GetZhuanShengCfg().GetOtherCfg();

		//	// 套装属性加成百分比  8件套 这个只计算装备战力加成
		//	if (grade_5_equip_count >= 8)
		//	{
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (other_cfg.grade_5_attr_add_percent_8 / 100.0f + 1));
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (other_cfg.grade_5_attr_add_percent_8 / 100.0f + 1));
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (other_cfg.grade_5_attr_add_percent_8 / 100.0f + 1));
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (other_cfg.grade_5_attr_add_percent_8 / 100.0f + 1));
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (other_cfg.grade_5_attr_add_percent_8 / 100.0f + 1));
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (other_cfg.grade_5_attr_add_percent_8 / 100.0f + 1));
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (other_cfg.grade_5_attr_add_percent_8 / 100.0f + 1));
		//	}
		//	else if (grade_3_equip_count >= 8)
		//	{
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (other_cfg.grade_3_attr_add_percent_8 / 100.0f + 1));
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (other_cfg.grade_3_attr_add_percent_8 / 100.0f + 1));
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (other_cfg.grade_3_attr_add_percent_8 / 100.0f + 1));
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (other_cfg.grade_3_attr_add_percent_8 / 100.0f + 1));
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (other_cfg.grade_3_attr_add_percent_8 / 100.0f + 1));
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (other_cfg.grade_3_attr_add_percent_8 / 100.0f + 1));
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] = static_cast<int>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (other_cfg.grade_3_attr_add_percent_8 / 100.0f + 1));
		//	}
		//}

		{
			// 转生等级
			const ZhuanshengLevelConfig *level_cfg = LOGIC_CONFIG->GetZhuanShengCfg().GetLevelCfg(m_param.zhuansheng_level);
			if (NULL != level_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += level_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += level_cfg->jianren;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_ZHUANSHENGEQUIP, m_attrs_add);
		m_role->GetCapability()->SetCap(CAPABILITY_TYPE_ZHUANSHEN_RAND_ATTR, m_add_exp_per * 200 + m_add_coin_per * 70);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void ZhuanShengEquip::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid)
	{
		return;
	}

	m_param.day_change_times = 0;
}

void ZhuanShengEquip::CalcInjureOnAttack(long long &injure)
{
	if (m_per_to_role_shang > 0 && m_per_to_role_shang < 10000)
	{
		injure = static_cast<long long>(injure * (1.0 + m_per_to_role_shang * SKILL_ATTR_PER));
	}
}

void ZhuanShengEquip::CalcInjureOnBeAttacked(long long &injure)
{
	if (m_per_to_role_mianshang > 0 && m_per_to_role_mianshang < 10000)
	{
		injure = static_cast<long long>(injure * (1.0 - m_per_to_role_mianshang * SKILL_ATTR_PER));
	}
}

int ZhuanShengEquip::GetAddExpPerOnKillMonster()
{
	return m_add_exp_per > 100 ? 100 : m_add_exp_per;
}

int ZhuanShengEquip::GetAddCoinPerOnKillMonster()
{
	return m_add_coin_per > 100 ? 100 : m_add_coin_per;
}

bool ZhuanShengEquip::OnUseXiuweiDan(int xiuwei_count, int num)
{
	if (xiuwei_count <= 0 || num <= 0)
	{
		return false;
	}

	int add_xiuwei = xiuwei_count * num;
	m_param.personal_xiuwei += add_xiuwei;

	this->SendOtherInfo();
	this->SendXiuWeiNotice(add_xiuwei);

	gamelog::g_log_zhuansheng_equip.printf(LL_INFO, "ZhuanShengEquip::OnUseXiuweiDan user[%d %s] xiuwei_count[%d] num[%d]",
		m_role->GetUID(), m_role->GetName(), xiuwei_count, num);

	return true;
}

void ZhuanShengEquip::AddXiuwei(int add_xiuwei)
{
	return;
	if (add_xiuwei <= 0)
	{
		return;
	}

	m_param.personal_xiuwei += add_xiuwei;

	this->SendOtherInfo();
	this->SendXiuWeiNotice(add_xiuwei);

	gamelog::g_log_zhuansheng_equip.printf(LL_INFO, "ZhuanShengEquip::AddXiuwei user[%d %s] add_xiuwei[%d]",
		m_role->GetUID(), m_role->GetName(), add_xiuwei);
}

void ZhuanShengEquip::OnUpZhuanShengLevel()
{
	return;
	const ZhuanshengLevelConfig *next_level_cfg = LOGIC_CONFIG->GetZhuanShengCfg().GetLevelCfg(m_param.zhuansheng_level + 1);
	if (NULL == next_level_cfg)
	{
		m_role->NoticeNum(errornum::EN_ZHUANSHENG_LEVEL_FULL);
		return;
	}

	const ZhuanshengLevelConfig *level_cfg = LOGIC_CONFIG->GetZhuanShengCfg().GetLevelCfg(m_param.zhuansheng_level);
	if (NULL == level_cfg)
	{
		return;
	}

	if (m_param.personal_xiuwei < level_cfg->consume_xiuwei)
	{
		this->SendOtherInfo(Protocol::ZHUANSHENG_NOTICE_XIUWEI_NOT_ENOUGH);
		m_role->NoticeNum(errornum::EN_ZHUANSHENG_XIUWEI_NOT_ENOUGH);
		return;
	}

	m_param.personal_xiuwei -= level_cfg->consume_xiuwei;
	++ m_param.zhuansheng_level;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANSHENGEQUIP);

	this->SendOtherInfo();

	{
		//传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_zhuansheng_level_up_notice, 
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), m_param.zhuansheng_level);
		if (length > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	//log
	gamelog::g_log_zhuansheng_equip.printf(LL_INFO, "ZhuanShengEquip::OnUpZhuanShengLevel user[%d %s] zhuansheng_level[%d] personal_xiuwei[%d]",
		m_role->GetUID(), m_role->GetName(), m_param.zhuansheng_level, m_param.personal_xiuwei);
}



void ZhuanShengEquip::OnChangeXiuwei()
{
	return;
	if (m_param.day_change_times >= ZHUANSHENG_XIUWEI_CFG_LIMIT || m_param.day_change_times >= LOGIC_CONFIG->GetZhuanShengCfg().GetXiuweiChangeTimes())
	{
		m_role->NoticeNum(errornum::EN_ZHUANSHENG_CHANGE_TIMES_FULL);
		return;
	}

	// 转生等级满了不能兑换
	const ZhuanshengLevelConfig *next_level_cfg = LOGIC_CONFIG->GetZhuanShengCfg().GetLevelCfg(m_param.zhuansheng_level + 1);
	if (NULL == next_level_cfg)
	{
		return;
	}

	const ZhuanshengXiuweiConfig *xiuwei_cfg = LOGIC_CONFIG->GetZhuanShengCfg().GetXiuweiCfg(m_param.day_change_times);
	if (NULL == xiuwei_cfg)
	{
		return;
	}

	if (xiuwei_cfg->consume_coin > 0 && !m_role->GetKnapsack()->GetMoney()->UseCoin(xiuwei_cfg->consume_coin, "ZhuanShengEquip::OnChangeXiuwei"))
	{
		m_role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
		return;
	}

	if (xiuwei_cfg->consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(xiuwei_cfg->consume_gold, "ZhuanShengEquip::OnChangeXiuwei"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	++ m_param.day_change_times;
	m_param.personal_xiuwei += xiuwei_cfg->reward_xiuwei;

	this->SendOtherInfo();
	this->SendXiuWeiNotice(xiuwei_cfg->reward_xiuwei);

	// log
	gamelog::g_log_zhuansheng_equip.printf(LL_INFO, "ZhuanShengEquip::OnChangeXiuwei user[%d %s] day_change_times[%d] personal_xiuwei[%d]",
		m_role->GetUID(), m_role->GetName(), m_param.day_change_times, m_param.personal_xiuwei);

}

void ZhuanShengEquip::TakeOutEquip(int backpack_index)
{
	
}

int ZhuanShengEquip::PutOnEquip(const ItemDataWrapper &item_wrapper)
{
	return -1;
	const Equipment* equip = (const Equipment*)ITEMPOOL->GetItem(item_wrapper.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType())
	{
		return -1;
	}

	if (0 == item_wrapper.has_param)
	{
		return -2;
	}

	// 阶数是否足够
// 	if (m_param.zhuansheng_level < equip->GetOrder())
// 	{
// 		m_role->NoticeNum(errornum::EN_JINJIE_EQUIP_LEVEL_LIMIT);
// 		return -3;
// 	}

	// 等级是否足够
	if (m_role->GetLevel() < equip->GetLimitLevel())
	{
		m_role->NoticeNum(errornum::EN_ZHUANSHENG_LEVEL_NOT_ENOUGH);
		return -4;
	}

	if (equip->GetEquipType() < Equipment::E_TYPE_ZHUANSHENG_MIN || equip->GetEquipType() >= Equipment::E_TYPE_ZHUANSHENG_MAX)
	{
		return -5;
	}

	const int equip_index = this->GetEquipIndexByType(equip->GetEquipType());
	if (equip_index < ZHUANSHENG_EQUIP_TYPE_1 || equip_index >= ZHUANSHENG_EQUIP_TYPE_MAX)
	{
		return -6;
	}

	ItemDataWrapper &itemdata = m_param.zhuansheng_equip_list[equip_index];
	if (!itemdata.Invalid())
	{
		if (this->TakeOffEquip(equip_index) < 0)
		{
			return -7;
		}
	}

	// 加强判断
	if (!itemdata.Invalid() || item_wrapper.Invalid())
	{
		return -8;
	}

	itemdata.Reset(item_wrapper);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANSHENGEQUIP);
	this->SendAllInfo();

	return 0;
}

int ZhuanShengEquip::TakeOffEquip(short equip_index)
{
	return -1;
	// 判断传入的下标以及用其取出的装备
	if (equip_index < 0 || equip_index >= ZHUANSHENG_EQUIP_TYPE_MAX)
	{ 
		return -1;
	}

	const ItemDataWrapper &item_data = m_param.zhuansheng_equip_list[equip_index];
	if (item_data.Invalid())
	{
		return -2;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_data.item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT ||
		equip->GetEquipType() < Equipment::E_TYPE_ZHUANSHENG_MIN || equip->GetEquipType() >= Equipment::E_TYPE_ZHUANSHENG_MAX)
	{
		return -3;
	}

	// 判断背包
	Knapsack *knapsack = m_role->GetKnapsack();
	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return -4;
	}

	if (!knapsack->Put(item_data, PUT_REASON_NO_NOTICE))
	{
		return -5;
	}

	m_param.zhuansheng_equip_list[equip_index].Reset();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANSHENGEQUIP);
	this->SendAllInfo();

	return 0;
}

int ZhuanShengEquip::GetEquipIndexByType(int equip_type)
{
	switch(equip_type)
	{
	case Equipment::E_TYPE_ZHUANSHENG_1:
		{
			return ZHUANSHENG_EQUIP_TYPE_1;
		}
		break;

	case Equipment::E_TYPE_ZHUANSHENG_2:
		{
			return ZHUANSHENG_EQUIP_TYPE_2;
		}
		break;

	case Equipment::E_TYPE_ZHUANSHENG_3:
		{
			return ZHUANSHENG_EQUIP_TYPE_3;
		}
		break;

	case Equipment::E_TYPE_ZHUANSHENG_4:
		{
			return ZHUANSHENG_EQUIP_TYPE_4;
		}
		break;

	case Equipment::E_TYPE_ZHUANSHENG_5:
		{
			return ZHUANSHENG_EQUIP_TYPE_5;
		}
		break;

	case Equipment::E_TYPE_ZHUANSHENG_6:
		{
			return ZHUANSHENG_EQUIP_TYPE_6;
		}
		break;

	case Equipment::E_TYPE_ZHUANSHENG_7:
		{
			return ZHUANSHENG_EQUIP_TYPE_7;
		}
		break;

	case Equipment::E_TYPE_ZHUANSHENG_8:
		{
			return ZHUANSHENG_EQUIP_TYPE_8;
		}
		break;

	default:
		{
			return -1;
		}
	}
}

ItemID ZhuanShengEquip::GetZhuanshengEquipID(int equip_index) 
{ 
	if (equip_index < 0 || equip_index >= ZHUANSHENG_EQUIP_TYPE_MAX)
	{
		return 0;
	}

	return m_param.zhuansheng_equip_list[equip_index].item_id;
}


void ZhuanShengEquip::CombineEquip(int index1, int index2, int index3, short is_auto_combine)
{
	//if (index1 < 0 || index1 >= ZHUANSHENG_EQUIP_BACKPACK_MAX_COUNT 
	// || index2 < 0 || index2 >= ZHUANSHENG_EQUIP_BACKPACK_MAX_COUNT
	// || index3 < 0 || index3 >= ZHUANSHENG_EQUIP_BACKPACK_MAX_COUNT)
	//{
	//	return;
	//}

	//if (index1 == index2 || index1 == index3 || index2 == index3)
	//{
	//	return;
	//}

	//if (m_param.backpack_equip_list[index1].equip_id <= 0 || m_param.backpack_equip_list[index2].equip_id <= 0 || m_param.backpack_equip_list[index3].equip_id <= 0)
	//{
	//	m_role->NoticeNum(errornum::EN_ZHUANSHENG_NOT_HAVE_EQUIP);
	//	return;
	//}

	//const ZhuanshengEquipInfoConfig * equip1 = LOGIC_CONFIG->GetZhuanShengCfg().GetEquipCfgByID(m_param.backpack_equip_list[index1].equip_id);
	//const ZhuanshengEquipInfoConfig * equip2 = LOGIC_CONFIG->GetZhuanShengCfg().GetEquipCfgByID(m_param.backpack_equip_list[index2].equip_id);
	//const ZhuanshengEquipInfoConfig * equip3 = LOGIC_CONFIG->GetZhuanShengCfg().GetEquipCfgByID(m_param.backpack_equip_list[index3].equip_id);

	//if (NULL == equip1 || NULL == equip2 || NULL == equip3)
	//{
	//	return;
	//}

	//if (equip1->grade != equip2->grade || equip1->grade != equip3->grade || equip2->grade != equip3->grade)
	//{
	//	return;
	//}

	//if (equip1->part_index != equip2->part_index || equip1->part_index != equip3->part_index || equip2->part_index != equip3->part_index)
	//{
	//	return;
	//}

	//if (equip1->type != ZHUANSHENG_EQUIP_INFO_TYPE_EQUIP || equip2->type != ZHUANSHENG_EQUIP_INFO_TYPE_EQUIP || equip3->type != ZHUANSHENG_EQUIP_INFO_TYPE_EQUIP)
	//{
	//	m_role->NoticeNum(errornum::EN_ZHUANSHENG_IS_XIUWEI_DAN);
	//	return;
	//}

	//char is_result_bind = 0;

	//// 有一个绑定 合成出来的就绑定
	//if (m_param.backpack_equip_list[index1].is_bind || m_param.backpack_equip_list[index2].is_bind || m_param.backpack_equip_list[index3].is_bind)
	//{
	//	is_result_bind = 1;
	//}

	//const ZhuanshengEquipInfoConfig *new_equip = LOGIC_CONFIG->GetZhuanShengCfg().GetEquipCfgByIndexGrade(equip1->part_index, equip1->grade + 1);
	//if (NULL == new_equip)
	//{
	//	return;
	//}

	//m_param.backpack_equip_list[index1].equip_id = new_equip->equip_id;
	//m_param.backpack_equip_list[index2].equip_id = 0;
	//m_param.backpack_equip_list[index3].equip_id = 0;

	//m_param.backpack_equip_list[index1].is_bind = is_result_bind;
	//m_param.backpack_equip_list[index2].is_bind = 0;
	//m_param.backpack_equip_list[index3].is_bind = 0;

	//m_empty_grid_index_list.push(index2);
	//m_empty_grid_index_list.push(index3);

	//this->SendBackpackInfo();

	//static Protocol::SCZhuanShengCombineResult sczscr;
	//sczscr.new_equip_id = new_equip->equip_id;
	//sczscr.is_auto_combine = is_auto_combine;
	//EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&sczscr, sizeof(sczscr));

	//// log
	//gamelog::g_log_zhuansheng_equip.printf(LL_INFO, "ZhuanShengEquip::CombineEquip user[%d %s] index1[%d] index2[%d] index3[%d] new_equip_id[%d]",
	//	m_role->GetUID(), m_role->GetName(), index1, index2, index3, m_param.backpack_equip_list[index1].equip_id);
}

void ZhuanShengEquip::AutoCombineEquip()
{
	//unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();
	//if (now_second < m_next_allow_autoconbine_time)
	//{
	//	return;
	//}
	//m_next_allow_autoconbine_time = now_second + 5;

	//int total_conbine_times = 0;

	//std::map<unsigned short, std::vector<int> > equip_list;

	//// 按装备id分别存储装备列表
	//for (int index = 0; index < ZHUANSHENG_EQUIP_BACKPACK_MAX_COUNT; ++ index)
	//{
	//	unsigned short equip_id = m_param.backpack_equip_list[index].equip_id;
	//	const ZhuanshengEquipInfoConfig * equip = LOGIC_CONFIG->GetZhuanShengCfg().GetEquipCfgByID(equip_id);
	//	if (NULL != equip && ZHUANSHENG_EQUIP_INFO_TYPE_EQUIP == equip->type && equip->grade < 10)
	//	{
	//		equip_list[equip_id].push_back(index);
	//	}
	//}

	//for (std::map<unsigned short, std::vector<int> >::iterator it = equip_list.begin(); it != equip_list.end(); ++ it)
	//{
	//	if (it->second.size() < 3) continue; // 不足3个，无法合并

	//	unsigned short equip_id = m_param.backpack_equip_list[it->second[0]].equip_id;
	//	const ZhuanshengEquipInfoConfig * equip = LOGIC_CONFIG->GetZhuanShengCfg().GetEquipCfgByID(equip_id);
	//	if (NULL == equip || equip->grade >= 10) continue;  // 第10阶无法继续往上合并了

	//	int need_conbine_bind_count = 0;	// 需要融合的绑定物品数量
	//	int need_conbine_unbind_count = 0;	// 需要融合的非绑物品数量

	//	// 先融合非绑
	//	for (std::vector<int>::iterator sub_it = it->second.begin(); sub_it != it->second.end(); ++ sub_it)
	//	{
	//		ZhuanshenEuipGridItem &grid_item = m_param.backpack_equip_list[*sub_it];
	//		if (grid_item.is_bind)
	//		{
	//			++ need_conbine_bind_count;
	//		}
	//		else
	//		{
	//			++ need_conbine_unbind_count;
	//		}
	//	}

	//	need_conbine_bind_count  = int(need_conbine_bind_count / 3) * 3;
	//	need_conbine_unbind_count  = int(need_conbine_unbind_count / 3) * 3;

	//	// 先融合非绑
	//	int cur_conbine_count = 0;
	//	for (std::vector<int>::iterator sub_it = it->second.begin(); sub_it != it->second.end() && cur_conbine_count < need_conbine_unbind_count; ++ sub_it)
	//	{
	//		ZhuanshenEuipGridItem &grid_item = m_param.backpack_equip_list[*sub_it];
	//		if (grid_item.is_bind) continue;

	//		++ cur_conbine_count;

	//		if (cur_conbine_count % 3 == 0) // 删除了3个，就合并成一个高阶的，放到该格子
	//		{
	//			grid_item.equip_id = it->first + 1;
	//			grid_item.is_bind = 0;
	//			equip_list[grid_item.equip_id].push_back(*sub_it); // index放进去下一个vector里，理论上不会导致当前迭代器失效

	//			++ total_conbine_times;
	//		}
	//		else
	//		{
	//			grid_item.Reset();
	//			m_empty_grid_index_list.push(*sub_it);
	//		}
	//	}

	//	// 再融合绑定
	//	cur_conbine_count = 0;
	//	for (std::vector<int>::iterator sub_it = it->second.begin(); sub_it != it->second.end() && cur_conbine_count < need_conbine_bind_count; ++ sub_it)
	//	{
	//		ZhuanshenEuipGridItem &grid_item = m_param.backpack_equip_list[*sub_it];
	//		if (!grid_item.is_bind) continue;

	//		++ cur_conbine_count;

	//		if (cur_conbine_count % 3 == 0) // 删除了3个，就合并成一个高阶的，放到该格子
	//		{
	//			grid_item.equip_id = it->first + 1;
	//			grid_item.is_bind = 1;
	//			equip_list[grid_item.equip_id].push_back(*sub_it); // index放进去下一个vector里，理论上不会导致当前迭代器失效

	//			++ total_conbine_times;
	//		}
	//		else
	//		{
	//			grid_item.Reset();
	//			m_empty_grid_index_list.push(*sub_it);
	//		}
	//	}
	//}

	//if (total_conbine_times > 0)
	//{
	//	this->SendBackpackInfo();
	//	m_role->NoticeNum(noticenum::NT_ZHUANSHENG_AUTO_CONBIME_SUCC);
	//}
	//else
	//{
	//	m_role->NoticeNum(errornum::EN_ZHUANSHENG_AUTO_CONBIME_EQUIP_NOT_ENOUGTH);
	//}

	//// log
	//gamelog::g_log_zhuansheng_equip.printf(LL_INFO, "ZhuanShengEquip::AutoCombineEquip user[%d %s] total_conbine_times[%d]",
	//	m_role->GetUID(), m_role->GetName(), total_conbine_times);
}

void ZhuanShengEquip::Chou(int type)
{
	//if (type <= ZHUANSHENG_CHOU_TYPE_MIN || type >= ZHUANSHENG_CHOU_TYPE_MAX)
	//{
	//	return;
	//}

	//const ZhuanshengOtherConfig other_cfg = LOGIC_CONFIG->GetZhuanShengCfg().GetOtherCfg();

	//ItemID consume_item_id = ItemBase::INVALID_ITEM_ID;
	//int times = 0;
	//int comsume_gold = 0;
	//unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//int need_knapsack_grid = 0;

	//if (ZHUANSHENG_CHOU_TYPE_COMMON_ROLL == type)
	//{
	//	times = 1;
	//	need_knapsack_grid = 1;

	//	if (m_param.last_time_free_chou_timestamp + other_cfg.free_cold_down_time < now_time)
	//	{
	//		comsume_gold = 0;
	//	}
	//	else
	//	{
	//		comsume_gold = other_cfg.common_roll_cost;
	//	}
	//}
	//else if (ZHUANSHENG_CHOU_TYPE_SUPER_ROLL == type)
	//{
	//	times = 10;
	//	need_knapsack_grid = ZHUANSHENG_EQUIP_XIUWEI_DAN_COUNT;

	//	comsume_gold = other_cfg.super_roll_cost;
	//	consume_item_id = other_cfg.super_roll_cost_item_id;
	//}
	//else
	//{
	//	return;
	//}

	//if ((int)m_empty_grid_index_list.size() < times)
	//{
	//	m_role->NoticeNum(errornum::EN_ZHUANSHENG_BACKPACK_FULL);
	//	return;
	//}

	//if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_knapsack_grid))
	//{
	//	m_role->NoticeNum(errornum::EN_ZHUANSHENG_KNAPSACK_NOT_ENOUGH);
	//	return;
	//}

	//// 保存抽奖结果
	//int reward_seq_list[Protocol::SC_ZHUANSHENG_CHOU_RESULT_MAX];
	//ZhuanshengEquipInfoConfig reward_info_list[Protocol::SC_ZHUANSHENG_CHOU_RESULT_MAX];

	//for (int i = 0; i < times; ++ i)
	//{
	//	const ZhuanshengChouConfig *chou_cfg = LOGIC_CONFIG->GetZhuanShengCfg().GetRandomChouResult();
	//	if (NULL == chou_cfg)
	//	{
	//		return;
	//	}

	//	const ZhuanshengEquipInfoConfig *info_cfg = LOGIC_CONFIG->GetZhuanShengCfg().GetEquipCfgByID(chou_cfg->reward_equip_id);
	//	if (NULL == info_cfg)
	//	{
	//		return;
	//	}

	//	reward_seq_list[i] = chou_cfg->seq;
	//	reward_info_list[i] = *info_cfg;
	//}

	//// 消耗
	//if (comsume_gold <= 0)
	//{
	//	m_param.last_time_free_chou_timestamp = now_time;
	//}
	//else
	//{
	//	//消耗
	//	if (m_role->GetKnapsack()->HasItem(consume_item_id))
	//	{
	//		if (!m_role->GetKnapsack()->ConsumeItem(consume_item_id, 1, "ZhuanShengEquip::Chou"))
	//		{
	//			if (!m_role->GetKnapsack()->GetMoney()->UseGold(comsume_gold, "ZhuanShengEquip::Chou"))
	//			{
	//				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
	//				return;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (!m_role->GetKnapsack()->GetMoney()->UseGold(comsume_gold, "ZhuanShengEquip::Chou"))
	//		{
	//			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
	//			return;
	//		}
	//	}
	//}

	//gamelog::g_log_zhuansheng_equip.buff_printf("ZhuanShengEquip::Chou user[%d %s] type[%d] comsule_gold[%d] last_time_free_chou_timestamp[%d] equip_id[", 
	//	m_role->GetUID(), m_role->GetName(), type, comsume_gold, m_param.last_time_free_chou_timestamp);

	//// 给予
	//static Protocol::SCZhuanShengChouResult sczscr;
	//sczscr.result_count  = 0;

	//for (int i = 0; i < times; ++ i)
	//{
	////	m_role->GetKnapsack()->Put(reward_list[i].reward_item, PUT_REASON_ZHUANSHENG_EQUIP);

	//	if (ZHUANSHENG_EQUIP_INFO_TYPE_XIUWEI_DAN != reward_info_list[i].type)
	//	{
	//		int temp_index = m_empty_grid_index_list.top();
	//		m_param.backpack_equip_list[temp_index].equip_id = reward_info_list[i].equip_id;
	//		m_param.backpack_equip_list[temp_index].is_bind = 0;
	//		m_empty_grid_index_list.pop();
	//	}
	//	else
	//	{
	//		ItemConfigData item_data;
	//		item_data.item_id = reward_info_list[i].item_id;
	//		item_data.num = 1;
	//		item_data.is_bind = 0;

	//		m_role->GetKnapsack()->Put(item_data, PUT_REASON_ZHUANSHENG_EQUIP);
	//	}

	//	if (sczscr.result_count < Protocol::SC_ZHUANSHENG_CHOU_RESULT_MAX)
	//	{
	//		sczscr.result_list[sczscr.result_count ++] = reward_seq_list[i];
	//	}
	//	
	//	gamelog::g_log_zhuansheng_equip.buff_printf("(%d)", reward_info_list[i].equip_id);
	//}

	//if (sczscr.result_count > 0) // 有抽到奖励才发
	//{
	//	int len = sizeof(sczscr) - (Protocol::SC_ZHUANSHENG_CHOU_RESULT_MAX - sczscr.result_count) * sizeof(sczscr.result_list[0]); 
	//	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&sczscr, len);
	//}
	//
	//this->SendAllInfo();
	//// log
	//gamelog::g_log_zhuansheng_equip.buff_printf("]");
	//gamelog::g_log_zhuansheng_equip.commit_buff(LL_INFO);

	//ROLE_LOG_QUICK10(LOG_TYPE_ZHUANSHENG, m_role, NULL, times, NULL, NULL, comsume_gold, now_time, 0, 0);
}

void ZhuanShengEquip::OnChangeProf()
{
	for (int equip_index = 0; equip_index < ZHUANSHENG_EQUIP_TYPE_MAX; ++ equip_index)
	{
		if (!m_param.zhuansheng_equip_list[equip_index].Invalid())
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(m_param.zhuansheng_equip_list[equip_index].item_id);
			if (NULL != itembase)
			{
				ItemID change_itemid = 0;
				if (LOGIC_CONFIG->GetZhuanzhiCfg().GetChangeItemID(itembase->GetItemId(), m_role->GetProf() % PROF_TYPE_PROF_ZHUANZHI_BASE, &change_itemid))
				{
					const ItemBase *change_item = ITEMPOOL->GetItem(change_itemid);
					if (NULL != change_item) 
					{
						m_param.zhuansheng_equip_list[equip_index].item_id = change_itemid;
					}
				}
			}
		}
	}

	this->SendAllInfo();
}

void ZhuanShengEquip::SendAllInfo()
{
	return;
	static Protocol::SCZhuanShengAllInfo sczsai;
	memcpy(sczsai.zhuansheng_equip_list, m_param.zhuansheng_equip_list, sizeof(sczsai.zhuansheng_equip_list));
	
	sczsai.last_time_free_chou_timestamp = m_param.last_time_free_chou_timestamp;
	sczsai.personal_xiuwei = m_param.personal_xiuwei;
	sczsai.zhuansheng_level = m_param.zhuansheng_level;
	sczsai.day_change_times = m_param.day_change_times;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&sczsai, sizeof(sczsai));
}

void ZhuanShengEquip::SendBackpackInfo()
{
	static Protocol::SCZhuanShengBackpackInfo sczsbi;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&sczsbi, sizeof(sczsbi));
}

void ZhuanShengEquip::SendOtherInfo(int notice_reason)
{
	static Protocol::SCZhuanShengOtherInfo sczsoi;
	
	sczsoi.notice_reason = notice_reason;
	sczsoi.last_time_free_chou_timestamp = m_param.last_time_free_chou_timestamp;
	sczsoi.personal_xiuwei = m_param.personal_xiuwei;
	sczsoi.zhuansheng_level = m_param.zhuansheng_level;
	sczsoi.day_change_times = m_param.day_change_times;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&sczsoi, sizeof(sczsoi));
}

void ZhuanShengEquip::SendXiuWeiNotice(int xiuwei, int notice_reason)
{
	static Protocol::SCZhuanShengXiuweiNotice sczsoi;

	sczsoi.notice_reason = notice_reason;
	sczsoi.add_xiuwei = xiuwei;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&sczsoi, sizeof(sczsoi));
}

void ZhuanShengEquip::GMClearBackpack()
{

}

void ZhuanShengEquip::GMSetZhuanShengLevel(int level)
{
	const ZhuanshengLevelConfig *level_cfg = LOGIC_CONFIG->GetZhuanShengCfg().GetLevelCfg(level);
	if (NULL == level_cfg)
	{
		return;
	}

	m_param.zhuansheng_level = level;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANSHENGEQUIP);

	this->SendOtherInfo();
}


