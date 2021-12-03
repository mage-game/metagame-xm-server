#include "impguard.hpp"
#include "world.h"
#include "obj/character/role.h"
#include "protocal/msgimpguard.h"
#include "servercommon/commondata.hpp"
#include "config/logicconfigmanager.hpp"
#include "scene/scene.h"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "impguardconfig.hpp"
#include "other/capability/capability.hpp"
#include "effect/skilleffect/effectotherbuff.hpp"
#include "../route/mailroute.hpp"
#include "gamelog.h"
#include "other/tianshuxunzhu/tianshuxunzhuconfig.hpp"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"

ImpGuard::ImpGuard(): m_role(NULL), m_invalid_update(false)
{
}

ImpGuard::~ImpGuard()
{
}

void ImpGuard::Init(Role *role)
{
	m_role = role;

	m_grid_data->Reset();

	int imp_type = 0;
	int item_id = 0;         // 之前的小鬼只记录了类型没记录id，但是送的免费小鬼走的是类型1的逻辑，导致身上小鬼切场景会变成非免费小鬼id（时限不会改变）
	for (int i = 0; i < IMP_GUARD_GRID_INDEX_MAX; ++i)
	{
		imp_type = m_role->GetCommonDataParam()->imp_guard_type[i];
		item_id = m_role->GetCommonDataParam()->imp_guard_itemid[i];
		const ImpConfig *imp_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetImpCfgByImpType(imp_type);
		if (NULL != imp_cfg)
		{
			m_grid_data[i].item_data.item_id = item_id > 0 ? item_id : imp_cfg->item_id;
			m_grid_data[i].item_data.invalid_time = m_role->GetCommonDataParam()->imp_guard_end_timestamp[i];
			m_grid_data[i].item_data.param = new EquipmentParam();
		}
	}
}

void ImpGuard::Update(unsigned long interval, unsigned int now_second)
{
	for (int i = 0; i < IMP_GUARD_GRID_INDEX_MAX; ++i)
	{
		if (!m_grid_data[i].item_data.Invalid() && now_second >= m_grid_data[i].item_data.invalid_time && !m_grid_data[i].invalid_notice)// 失效   属性/buff消失
		{
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMP_GUARD);
			// 现在有两个格子所以需要判断一下失效的是不是经验小鬼再移除buff
			if (IMP_GUARD_TYPE_01 == m_role->GetCommonDataParam()->imp_guard_type[i] || IMP_GUARD_TYPE_03 == m_role->GetCommonDataParam()->imp_guard_type[i])
			{
				m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFEECT_TYPE_OTHER, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_IMP_GURAD_EXP, EffectBase::PRODUCT_ID_IMP_GURAD_EXP);
			}
			m_grid_data[i].invalid_notice = true;

			this->OnImpGuardChangeNotice(i, 0);
			this->SendImpGuardAllInfo();
			//this->SendImpGuardExpireTime(i);
		}
	}
}

void ImpGuard::ReCalcAttr(CharIntAttrs & base_add, bool is_recalc)
{
	unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();

	if (is_recalc)
	{
		m_attrs_add.Reset();

		m_attr_per_mianshang = 0;

		int imp_type = 0;
		for (int i = 0; i < IMP_GUARD_GRID_INDEX_MAX; ++i)
		{
			imp_type = m_role->GetCommonDataParam()->imp_guard_type[i];
			const ImpConfig *imp_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetImpCfgByImpType(imp_type);
			if (NULL != imp_cfg && m_role->GetCommonDataParam()->imp_guard_end_timestamp[i] > now_time)  // 失效小鬼不计算属性
			{
				// 策划需求，伤害减免属性不加战斗力
				//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += imp_cfg->per_mianshang;
				m_attr_per_mianshang += imp_cfg->per_mianshang;

				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += imp_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += imp_cfg->max_hp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] += imp_cfg->fujia_shanghai;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(
				m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN) * (imp_cfg->per_kangbao * ROLE_ATTR_PER));//抗暴
//				m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += imp_cfg->pvp_reduce_hurt_per;

				m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, imp_cfg->pvp_reduce_hurt_per);
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_IMP_GUARD, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void ImpGuard::OnRoleLogin()
{
	this->CheckImpGuardMail();
	this->AutoPutOn();
	this->OnCheckImpGuardBuff();
	this->CheckImpGuardExpireTime();
}

void ImpGuard::OnImpGuardOperaReq(int opera_type, int param1, int param2)
{
	if (opera_type < 0 || opera_type >= Protocol::IMP_GUARD_REQ_TYPE_MAX)
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::IMP_GUARD_REQ_TYPE_RENEW_PUTON:
	{
		this->OnImpGuardRenewPutOn(param1, param2 == 1, false);
	}
	break;

	case Protocol::IMP_GUARD_REQ_TYPE_RENEW_KNAPSACK:
	{
		this->OnImpGuardRenewKnapsack(param1, param2 == 1);
	}
	break;

	case Protocol::IMP_GUARD_REQ_TYPE_TAKEOFF:
	{
		this->TakeOff(param1);
	}
	break;

	case Protocol::IMP_GUARD_REQ_TYPE_ALL_INFO:
	{
		this->SendImpGuardAllInfo();
	}
	break;

	default:
		break;
	}
}

int ImpGuard::PutOn(const ItemDataWrapper & item_wrapper)
{
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_wrapper.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()) return -1;

	if (0 == item_wrapper.has_param) return -2;

	if (equip->GetEquipType() != Equipment::E_TYPE_SPECIAL_IMP_GUARD) return -3;

	unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
	if (item_wrapper.invalid_time < now_time) // 失效小鬼
	{
		m_role->NoticeNum(errornum::EN_INVALID_IMP_GUARD);
		return -6;
	}

	EquipmentParam *equip_param = equip->CreateParamFromStruct(&item_wrapper.param_data);
	if (NULL == equip_param) return -4;

	const ImpConfig *imp_cfg = NULL;
	ImpGuardOtherCfg other_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetOtherCfg();
	if (item_wrapper.item_id == other_cfg.free_imp_item_id)
	{
		imp_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetImpCfgByImpType(other_cfg.free_imp_type);
	}
	else
	{
		imp_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetImpCfgByItemId(item_wrapper.item_id);
	}
	
	if (NULL == imp_cfg)
	{
		return -5;
	}

	int index = this->GetGridIndexByImpType(imp_cfg->imp_type);

	if (index < IMP_GUARD_GRID_INDEX_0 || index >= IMP_GUARD_GRID_INDEX_MAX)
	{
		return -10000;
	}

	// 如果已经激活了小鬼
	if (!m_grid_data[index].item_data.Invalid())
	{
// 		if (m_grid_data[index].item_data.invalid_time >= item_wrapper.invalid_time)
// 		{
// 			return -100;
// 		}
// 
// 		int ret = this->TakeOff(index);
// 		if (0 != ret)
// 		{
// 			return -6;
// 		}

		// 在现在的策划要求下，如果之前已经激活了，理论上不该继续激活，而是只能续费。这里直接走续费的处理
		// 为了防止客户端在激活后，又发了激活的请求，此时客户端自己请求了购买，为了不造成玩家购买东西损失，所以直接续费处理，但是不能继续扣钱
		this->OnImpGuardRenewPutOn(index, true, true);

		gamelog::g_log_serious_error.printf(LL_WARNING, "ImpGuard::PutOn Repeat put on. role[%d, %s] grid_index[%d]", 
			m_role->GetUID(), m_role->GetName(), index);

		return 0;
	}

	// 检查是否满足使用等级
	if (m_role->GetLevel() < imp_cfg->level_limit)
	{
		m_role->NoticeNum(errornum::EN_ITEM_LEVEL_LIMIT);
		return -7;
	}

	m_grid_data[index].item_data.item_id = item_wrapper.item_id;
	m_grid_data[index].item_data.param = equip_param;
	m_grid_data[index].item_data.num = 1;
	m_grid_data[index].item_data.invalid_time = item_wrapper.invalid_time;
	m_grid_data[index].item_data.is_bind = (item_wrapper.is_bind == 1);
	m_grid_data[index].invalid_notice = false;

	m_role->GetCommonDataParam()->imp_guard_type[index] = imp_cfg->imp_type;
	m_role->GetCommonDataParam()->imp_guard_end_timestamp[index] = item_wrapper.invalid_time;
	m_role->GetCommonDataParam()->imp_guard_itemid[index] = item_wrapper.item_id;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMP_GUARD);

	this->SendImpGuardAllInfo();

	// 加buff
	if (IMP_GUARD_TYPE_01 == m_role->GetCommonDataParam()->imp_guard_type[index] || IMP_GUARD_TYPE_03 == m_role->GetCommonDataParam()->imp_guard_type[index])
	{
		m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFEECT_TYPE_OTHER, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_IMP_GURAD_EXP, EffectBase::PRODUCT_ID_IMP_GURAD_EXP);

		EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_IMP_GURAD_EXP, 0, 1, EffectBase::PRODUCT_METHOD_IMP_GURAD_EXP);
		effect->IncExtraExpPer(imp_cfg->add_per_exp);
		effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);
		if (IMP_GUARD_TYPE_01 == m_role->GetCommonDataParam()->imp_guard_type[index])
		{
			effect->SetClientEffectType(EffectBase::ECT_IMP_GURAD_EXP1);
		}
		else if (IMP_GUARD_TYPE_03 == m_role->GetCommonDataParam()->imp_guard_type[index])
		{
			effect->SetClientEffectType(EffectBase::ECT_IMP_GURAD_EXP2);
		}
		effect->SetSave(true);
		m_role->AddEffect(effect);
	}
	this->OnImpGuardChangeNotice(index, imp_cfg->imp_type);

	if (item_wrapper.item_id != other_cfg.free_imp_item_id)
	{
		TianShuXunZhu::CheckShiXueTianShu(m_role, CHENGZHANG_TIANSHU_TYPE_BUY_IMPGUARD);
	}

	return 0;
}

void ImpGuard::OnRoleLogout()
{
}

void ImpGuard::AutoPutOn()
{
	int grid_index = this->GetGridIndexByImpType(IMP_GUARD_TYPE_03);
	if (grid_index < IMP_GUARD_GRID_INDEX_0 || grid_index >= IMP_GUARD_GRID_INDEX_MAX)
	{
		return;
	}

	if (!m_grid_data[grid_index].item_data.Invalid() && IMP_GUARD_TYPE_03 == m_role->GetCommonDataParam()->imp_guard_type[grid_index])
	{
		return;
	}
	
	int item_count = ItemNamespace::MAX_GRID_NUM;
	ItemListParam::ItemListData item_list[ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM];

	bool have_low_not_have_high = false;
	int index = -1;

	m_role->GetKnapsack()->GetItemList(&item_count, &item_list[0]);

	if (item_count <= 0) return;

	for (int i = 0; i < item_count && i < ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM; i++)
	{
		if (0 == item_list[i].item_wrapper.has_param) continue;
		
		const Equipment *equip_item = (const Equipment *)ITEMPOOL->GetItem(item_list[i].item_wrapper.item_id);
		if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType() || 
			equip_item->GetEquipType() != Equipment::E_TYPE_SPECIAL_IMP_GUARD) continue;

		const ImpConfig *imp_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetImpCfgByItemId(equip_item->GetItemId());
		if (NULL != imp_cfg && 3 == imp_cfg->imp_type)
		{
			unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			if (item_list[i].item_wrapper.invalid_time > now)
			{
				m_role->GetKnapsack()->Use(item_list[i].index, 1);
				have_low_not_have_high = false;
				break;
			}
		}

		if (NULL != imp_cfg && 1 == imp_cfg->imp_type)
		{
			unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			if (item_list[i].item_wrapper.invalid_time > now)
			{
				have_low_not_have_high = true;
				index = i;
			}
		}
	}

	if(have_low_not_have_high && index >= 0)
	{
		m_role->GetKnapsack()->Use(item_list[index].index, 1);
	}
}

int ImpGuard::TakeOff(int grid_index)
{
	if (grid_index < IMP_GUARD_GRID_INDEX_0 || grid_index >= IMP_GUARD_GRID_INDEX_MAX)
	{
		return -1;
	}

	if (m_grid_data[grid_index].item_data.Invalid())
	{
		return -2;
	}

	EquipmentParam *equip_param = (EquipmentParam *)m_grid_data[grid_index].item_data.param;
	if (NULL == equip_param) return -3;

	const Equipment *equip = (const Equipment*)ITEMPOOL->GetItem(m_grid_data[grid_index].item_data.item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT
		|| equip->GetEquipType() != Equipment::E_TYPE_SPECIAL_IMP_GUARD)
	{
		return -4;
	}
    //直接销毁不放背包
// 	Knapsack *knapsack = m_role->GetKnapsack();
// 	if (!knapsack->HaveSpace())
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 		return -5;
// 	}
// 
// 	ItemDataWrapper item_wrapper(m_grid_data[grid_index].item_data.item_id, 1, (short)m_grid_data[grid_index].item_data.is_bind,
// 		m_grid_data[grid_index].item_data.param, m_grid_data[grid_index].item_data.invalid_time, 0);
// 
// 	int put_index = -1;
// 	if (!knapsack->Put(item_wrapper, PUT_REASON_NO_NOTICE, &put_index))
// 	{
// 		return -6;
// 	}

	// 移除buff
	if (IMP_GUARD_TYPE_01 == m_role->GetCommonDataParam()->imp_guard_type[grid_index] || IMP_GUARD_TYPE_03 == m_role->GetCommonDataParam()->imp_guard_type[grid_index])
	{
		m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFEECT_TYPE_OTHER, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_IMP_GURAD_EXP, EffectBase::PRODUCT_ID_IMP_GURAD_EXP);
	}

	m_grid_data[grid_index].Reset();
	m_role->GetCommonDataParam()->imp_guard_type[grid_index] = IMP_GUARD_TYPE_INVALID;
	m_role->GetCommonDataParam()->imp_guard_itemid[grid_index] = 0;
	m_role->GetCommonDataParam()->imp_guard_end_timestamp[grid_index] = 0;
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMP_GUARD);

	this->SendImpGuardAllInfo();
	this->OnImpGuardChangeNotice(grid_index, 0);
	
	return 0;
}

int ImpGuard::GetAddPerExp()
{
	if (NULL == m_role) return 0;
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	int add_per_exp = 0;
	int imp_type = 0;

	for (int i = 0; i < IMP_GUARD_GRID_INDEX_MAX; ++i)
	{
		imp_type = m_role->GetCommonDataParam()->imp_guard_type[i];
		const ImpConfig *imp_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetImpCfgByImpType(imp_type);
		if (NULL == imp_cfg || now >= m_role->GetCommonDataParam()->imp_guard_end_timestamp[i])
		{
			continue;
		}

		add_per_exp += imp_cfg->add_per_exp;
	}

	return add_per_exp;
}

void ImpGuard::GetImpUseAppeId(short imp_appe_id[IMP_GUARD_GRID_INDEX_MAX])
{
	if (NULL == m_role) return;
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	int imp_type = 0;
	for (int i = 0; i < IMP_GUARD_GRID_INDEX_MAX; ++i)
	{
		imp_type = m_role->GetCommonDataParam()->imp_guard_type[i];
		const ImpConfig *imp_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetImpCfgByImpType(imp_type);
		if (NULL == imp_cfg)
		{
			continue;
		}

		if (now >= m_role->GetCommonDataParam()->imp_guard_end_timestamp[i])
		{
			imp_appe_id[i] = 0;
		}
		else
		{
			imp_appe_id[i] = imp_cfg->appe_image_id;
		}
	}
}

void ImpGuard::ResetImpMailStautsByType(int imp_type)
{
	if (imp_type <= 0 || imp_type > IMP_GUARD_MAX_IMP_TYPE)
	{
		return;
	}

	m_role->GetCommonDataParam()->imp_guard_mail_flag &= ~(1 << imp_type);
}

void ImpGuard::OnImpGuardRenewPutOn(int grid_index, bool is_bind, bool from_puton)
{
	if (grid_index < IMP_GUARD_GRID_INDEX_0 || grid_index >= IMP_GUARD_GRID_INDEX_MAX)
	{
		return;
	}

	if (m_grid_data[grid_index].item_data.Invalid())
	{
		return;
	}

	int imp_type = m_role->GetCommonDataParam()->imp_guard_type[grid_index];
	const ImpConfig *imp_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetImpCfgByImpType(imp_type);
	if (NULL == imp_cfg)
	{
		return;
	}

	if (!from_puton)  // 激活调用过来的不用再扣钱
	{
		if (is_bind)
		{
			if (!m_role->GetKnapsack()->GetMoney()->UseGoldBind(imp_cfg->bind_gold_price, "OnImpGuardRenewPutOn"))
			{
				m_role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
				return;
			}
		}
		else
		{
			if (!m_role->GetKnapsack()->GetMoney()->UseGold(imp_cfg->gold_price, "OnImpGuardRenewPutOn"))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
		}
	}
	
	unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
	if (m_grid_data[grid_index].item_data.invalid_time > now_time)
	{
		m_grid_data[grid_index].item_data.invalid_time = m_grid_data[grid_index].item_data.invalid_time + imp_cfg->use_day * SECOND_PER_DAY;
	}
	else
	{
		m_grid_data[grid_index].item_data.invalid_time = now_time + imp_cfg->use_day * SECOND_PER_DAY;
	}
	m_grid_data[grid_index].invalid_notice = false;
	m_role->GetCommonDataParam()->imp_guard_end_timestamp[grid_index] = m_grid_data[grid_index].item_data.invalid_time;

	this->ResetImpMailStautsByType(imp_type);

	this->OnCheckImpGuardBuff();

	if (IMP_GUARD_TYPE_01 == imp_cfg->imp_type)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_get_imp_type_1_notice,
			UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), (int)imp_cfg->item_id);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}
	else if (IMP_GUARD_TYPE_02 == imp_cfg->imp_type)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_get_imp_type_2_notice,
			UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), (int)imp_cfg->item_id);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMP_GUARD);
	this->OnImpGuardChangeNotice(grid_index, imp_type);
	this->SendImpGuardAllInfo();
	TianShuXunZhu::CheckShiXueTianShu(m_role, CHENGZHANG_TIANSHU_TYPE_BUY_IMPGUARD);
}

void ImpGuard::OnImpGuardRenewKnapsack(int index, bool is_bind)
{
	ItemGridData *item_data = m_role->GetKnapsack()->GetItemGridData(index);
	if (NULL == item_data)
	{
		return;
	}

	const ImpConfig *imp_cfg = NULL;
	ImpGuardOtherCfg other_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetOtherCfg();
	if (item_data->item_id == other_cfg.free_imp_item_id)
	{
		imp_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetImpCfgByImpType(other_cfg.free_imp_type);
	}
	else
	{
		imp_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetImpCfgByItemId(item_data->item_id);
	}

	if (NULL == imp_cfg)
	{
		return;
	}

	if (is_bind)
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGoldBind(imp_cfg->bind_gold_price, "OnImpGuardRenewKnapsack"))
		{
			m_role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
			return;
		}
	}
	else
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(imp_cfg->gold_price, "OnImpGuardRenewKnapsack"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (item_data->invalid_time > now)
	{
		item_data->invalid_time = item_data->invalid_time + imp_cfg->use_day * SECOND_PER_DAY;
	}
	else
	{
		item_data->invalid_time = now + imp_cfg->use_day * SECOND_PER_DAY;
	}

	m_role->GetKnapsack()->SetItemDirty(index, false);
	m_role->GetKnapsack()->SendKnapsackInfoToRole();

	if (IMP_GUARD_TYPE_01 == imp_cfg->imp_type)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_get_imp_type_1_notice,
			UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), (int)imp_cfg->item_id);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}
	else if (IMP_GUARD_TYPE_02 == imp_cfg->imp_type)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_get_imp_type_2_notice,
			UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), (int)imp_cfg->item_id);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	this->ResetImpMailStautsByType(imp_cfg->imp_type);

	this->OnCheckImpGuardBuff();
}

void ImpGuard::OnCheckImpGuardBuff()
{
	int imp_type = 0;
	unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
	m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFEECT_TYPE_OTHER, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_IMP_GURAD_EXP, EffectBase::PRODUCT_ID_IMP_GURAD_EXP);
	for (int i = 0; i < IMP_GUARD_GRID_INDEX_MAX; ++i)
	{
		imp_type = m_role->GetCommonDataParam()->imp_guard_type[i];
		const ImpConfig *imp_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetImpCfgByImpType(imp_type);
		if (NULL == imp_cfg)
		{
			return;
		}

		if (m_role->GetCommonDataParam()->imp_guard_end_timestamp[i] < now_time) // 失效小鬼不计算buff
		{
			return;
		}

		if (IMP_GUARD_TYPE_01 == m_role->GetCommonDataParam()->imp_guard_type[i] || IMP_GUARD_TYPE_03 == m_role->GetCommonDataParam()->imp_guard_type[i])
		{
			EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_IMP_GURAD_EXP, 0, 1, EffectBase::PRODUCT_METHOD_IMP_GURAD_EXP);
			effect->IncExtraExpPer(imp_cfg->add_per_exp);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);
			if (IMP_GUARD_TYPE_01 == m_role->GetCommonDataParam()->imp_guard_type[i])
			{
				effect->SetClientEffectType(EffectBase::ECT_IMP_GURAD_EXP1);
			}
			else if (IMP_GUARD_TYPE_03 == m_role->GetCommonDataParam()->imp_guard_type[i])
			{
				effect->SetClientEffectType(EffectBase::ECT_IMP_GURAD_EXP2);
			}
			effect->SetSave(true);
			m_role->AddEffect(effect);
		}
	}
}

void ImpGuard::CheckImpGuardExpireTime()
{
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	for (int i = 0; i < IMP_GUARD_GRID_INDEX_MAX; ++i)
	{
		if (!m_grid_data[i].item_data.Invalid() && now_second >= m_grid_data[i].item_data.invalid_time) // 失效
		{
			this->SendImpGuardAllInfo();
			//this->SendImpGuardExpireTime(i);
		}
	}
}

void ImpGuard::SendImpGuardAllInfo()
{
	static Protocol::SCImpGuardInfo igi;

	for (int i = 0; i < IMP_GUARD_GRID_INDEX_MAX; ++i)
	{
		igi.imp_guard_data[i].grid_index = (char)i;
		igi.imp_guard_data[i].used_imp_type = m_role->GetCommonDataParam()->imp_guard_type[i];
		m_grid_data[i].item_data.WrapTo(&igi.imp_guard_data[i].item_wrapper);
		igi.imp_guard_data[i].is_expire = m_grid_data[i].invalid_notice ? (char)1 : (char)0;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&igi, sizeof(igi));
}

// void ImpGuard::SendImpGuardExpireTime(int index)
// {
// 	Protocol::SCRoleImpExpireTime riet;
// 	riet.grid_index = (short)index;
// 	riet.res_sh = 0;
// 
// 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&riet, sizeof(riet));
// }

void ImpGuard::CheckImpGuardMail()
{
	time_t now_time = EngineAdapter::Instance().Time();
	// 身上的小鬼
	for (int i = 0; i < IMP_GUARD_GRID_INDEX_MAX; ++i)
	{
		if (!m_grid_data[i].item_data.Invalid())
		{
			if (0 == (m_role->GetCommonDataParam()->imp_guard_mail_flag & (1 << m_role->GetCommonDataParam()->imp_guard_type[i])))
			{
				if ((m_role->GetCommonDataParam()->imp_guard_end_timestamp[i] > 0) &&
					(m_role->GetCommonDataParam()->imp_guard_end_timestamp[i] > (unsigned int)now_time) &&
					(m_role->GetCommonDataParam()->imp_guard_end_timestamp[i] < ((unsigned int)now_time + SECOND_PER_HOUR * TWELVE_HOURS)))
				{
					//邮件提醒
					static MailContentParam contentparam; contentparam.Reset();

					unsigned int left_time = m_role->GetCommonDataParam()->imp_guard_end_timestamp[i] - (unsigned int)now_time;
					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_impguard_remind_mail_content, m_grid_data[i].item_data.item_id, left_time);
					if (length > 0)
					{
						MailRoute::MailToUser(IntToUid(m_role->GetUID()), SYSTEM_MAIL_REASON_INVALID, contentparam);
						m_role->GetCommonDataParam()->imp_guard_mail_flag |= (1 << m_role->GetCommonDataParam()->imp_guard_type[i]);
					}
				}
			}
		}
	}

	m_role->GetKnapsack()->CheckImpGuardMailInKnapsack(now_time);
}

int ImpGuard::GetGridIndexByImpType(short imp_type)
{
	if (imp_type == IMP_GUARD_TYPE_01 || imp_type == IMP_GUARD_TYPE_03)   // 经验小鬼
	{
		return IMP_GUARD_GRID_INDEX_0;
	}
	else if (imp_type == IMP_GUARD_TYPE_02 || imp_type == IMP_GUARD_TYPE_04)  // 守护
	{
		return IMP_GUARD_GRID_INDEX_1;
	}

	return IMP_GUARD_GRID_INDEX_MAX;
}

void ImpGuard::OnImpGuardChangeNotice(int grid_index, int imp_id)
{
	if (grid_index < IMP_GUARD_GRID_INDEX_0 || grid_index >= IMP_GUARD_GRID_INDEX_MAX)
	{
		return;
	}

	static Protocol::SCRoleImpAppeChange riac;
	riac.objid = m_role->GetId();
	riac.appe_index = (short)grid_index;
	riac.appe_id = (short)imp_id;

	if (NULL != m_role->GetScene())
	{
		m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&riac, sizeof(riac));
	}
}