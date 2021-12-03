#include "qingyuanglobal.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"
#include "other/baby/babyconfig.hpp"
#include "servercommon/errornum.h"
#include "obj/character/role.h"
#include "item/itempool.h"
#include "scene/scene.h"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/usercache/usercache.hpp"
#include "world.h"
#include "gamecommon.h"
#include "gstr.h"
#include "servercommon/noticenum.h"
#include "other/capability/capability.hpp"
#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include "gameworld/gameworld/config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/crossconfig.hpp"


QingyuanGlobal::QingyuanGlobal(GlobalRole *g_role) : IGlobalRoleItem(g_role)
{
}

QingyuanGlobal::~QingyuanGlobal()
{
}

void QingyuanGlobal::Init(const QingyuanGlobalParam &attr_data)
{
	m_param = attr_data;
}

void QingyuanGlobal::GetAttr(QingyuanGlobalParam &attr_data)
{
	attr_data = m_param;
}

void QingyuanGlobal::Update(unsigned int now_second)
{

}

void QingyuanGlobal::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	m_param.today_buy_gift_flag = 0;

	Role *role = this->GetRole();
	if (nullptr != role)
	{
		this->SendQingyuanGlobalInfo(role, true);
	}
}

void QingyuanGlobal::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc, Role *role)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		this->GetTotalAttr(m_attrs_add, true);

		QingyuanGlobal * qingyuuan_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_param.lover_id);
		if (NULL != qingyuuan_global)
		{
			qingyuuan_global->GetTotalAttr(m_attrs_add, false);
		}

		int qingyuan_global_capability = Capability::GetCapByAttr(m_attrs_add);
		int qingyuan_capability = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_QINGYUAN);
		role->GetCapability()->SetCap(CAPABILITY_TYPE_QINGYUAN, qingyuan_global_capability + qingyuan_capability);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void QingyuanGlobal::OnRoleLogin(Role *role)
{
	this->SetLoverId(role->GetLoverUid(), false);
}

int QingyuanGlobal::GetLoverId()
{
	return m_param.lover_id;
}

void QingyuanGlobal::GetTotalAttr(CharIntAttrs &tmp_attr, bool is_self)
{
	// 自己情缘加成
	if (is_self)
	{
		// 结婚等级
		const QingyuanMarryLevelConfig *marry_level_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetMarryLevelCfg(m_param.marry_level);
		if (NULL != marry_level_cfg)
		{
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += marry_level_cfg->maxhp;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += marry_level_cfg->gongji;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += marry_level_cfg->fangyu;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += marry_level_cfg->mingzhong;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += marry_level_cfg->shanbi;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += marry_level_cfg->baoji;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += marry_level_cfg->jianren;
		}

		// 装备套装
		for (int suit_type = 0; suit_type < QINGYUAN_EQUIPSUIT_MAX_COUNT; suit_type++)
		{
			unsigned char suit_flag = m_param.qingyuan_suit_flag[suit_type];
			for (int suit_solt = 0; suit_solt < QINGYUAN_EQUIPSUIT_SLOT_MAX_COUNT; suit_solt++)
			{
				if (0 == (suit_flag & (1 << suit_solt))) continue;

				const QingyuanEquipSuitConfig *suit_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanEquipSuitCfg(suit_type, suit_solt);
				if (NULL == suit_cfg) continue;

				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_cfg->maxhp;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_cfg->gongji;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += suit_cfg->fangyu;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += suit_cfg->mingzhong;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += suit_cfg->shanbi;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += suit_cfg->baoji;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += suit_cfg->jianren;
			}
		}

		// 结婚装备
		for (int equip_index = 0; equip_index < QINGYUAN_EQUIP_TYPE_MAX; ++equip_index)
		{
			ItemDataWrapper &itemdata = m_param.qingyuan_equip_list[equip_index];
			if (itemdata.Invalid())
			{
				continue;
			}

			const Equipment *equip_item = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);

			if (NULL != equip_item && ItemBase::I_TYPE_EQUIPMENT == equip_item->GetItemType() &&
				equip_item->GetEquipType() >= Equipment::E_TYPE_QINGYUAN_MIN && equip_item->GetEquipType() < Equipment::E_TYPE_QINGYUAN_MAX)
			{
				equip_item->GetBaseAttr(tmp_attr);
			}
		}

		// 情比金坚装备
		for (int equip_index = 0; equip_index < QINGYUAN_EQUIP_TYPE_MAX; ++equip_index)
		{
			static  AttrConfig attr_cfg; attr_cfg.Reset();
			const QingyuanEquip2Cfg *upgrade_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanEquip2Cfg(equip_index, m_param.qingyuan_equip2_list[equip_index].equip2_level);
			if (NULL != upgrade_cfg)
			{
				attr_cfg.Add(upgrade_cfg->attr_cfg, 1);
			}

			tmp_attr.AddAttrFromConfig(attr_cfg);
		}

		// 特殊宝宝
		for (unsigned int baby_type = 0; baby_type < SPECIAL_BABY_TYPE_MAX && baby_type < static_array_size(m_param.special_baby_list); ++baby_type)
		{
			int baby_quality = m_param.special_baby_list[baby_type].quality;
			int baby_lv = m_param.special_baby_list[baby_type].special_baby_level;
			if (baby_lv <= 0)
			{
				continue;
			}

			const SpecialBabyLvCfg *baby_lv_cfg = LOGIC_CONFIG->GetBabyCfg().GetSpecialBabyLvCfg(baby_type, baby_quality, baby_lv);
			if (nullptr != baby_lv_cfg)
			{
				tmp_attr.AddAttrFromConfig(baby_lv_cfg->attr_cfg);
			}
		}

		{
			// 表白等级属性
			const QingyuanProfessGradeConfig *profess_grade_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetProfessGrade(m_param.profess_level);
			if (nullptr != profess_grade_cfg)
			{
				tmp_attr.AddAttrFromConfig(profess_grade_cfg->attr_cfg);
			}
		}
	}
	// 伴侣情缘加成
	else
	{
		// 装备套装
		for (int suit_type = 0; suit_type < QINGYUAN_EQUIPSUIT_MAX_COUNT; suit_type++)
		{
			unsigned char suit_flag = m_param.qingyuan_suit_flag[suit_type];
			for (int suit_solt = 0; suit_solt < QINGYUAN_EQUIPSUIT_SLOT_MAX_COUNT; suit_solt++)
			{
				if (0 == (suit_flag & (1 << suit_solt))) continue;

				const QingyuanEquipSuitConfig *suit_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanEquipSuitCfg(suit_type, suit_solt);
				if (NULL == suit_cfg) continue;

				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(suit_cfg->maxhp * (suit_cfg->banglv_add_per * ROLE_ATTR_PER));
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(suit_cfg->gongji * (suit_cfg->banglv_add_per * ROLE_ATTR_PER));
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(suit_cfg->fangyu * (suit_cfg->banglv_add_per * ROLE_ATTR_PER));
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(suit_cfg->mingzhong * (suit_cfg->banglv_add_per * ROLE_ATTR_PER));
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(suit_cfg->shanbi * (suit_cfg->banglv_add_per * ROLE_ATTR_PER));
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(suit_cfg->baoji * (suit_cfg->banglv_add_per * ROLE_ATTR_PER));
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(suit_cfg->jianren * (suit_cfg->banglv_add_per * ROLE_ATTR_PER));
			}
		}

		// 结婚装备
		CharIntAttrs temp_attrs_add;
		temp_attrs_add.Reset();

		const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
		for (int equip_index = 0; equip_index < QINGYUAN_EQUIP_TYPE_MAX; ++equip_index)
		{
			ItemDataWrapper &itemdata = m_param.qingyuan_equip_list[equip_index];
			if (itemdata.Invalid())
			{
				continue;
			}

			const Equipment *equip_item = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);

			if (NULL != equip_item && ItemBase::I_TYPE_EQUIPMENT == equip_item->GetItemType() &&
				equip_item->GetEquipType() >= Equipment::E_TYPE_QINGYUAN_MIN && equip_item->GetEquipType() < Equipment::E_TYPE_QINGYUAN_MAX)
			{
				temp_attrs_add.Reset();
				equip_item->GetBaseAttr(temp_attrs_add);

				for (int i = 0; i < CharIntAttrs::CHARINTATTR_TYPE_MAX; i++)
				{
//					tmp_attr.m_attrs[i] += static_cast<Attribute>(temp_attrs_add.m_attrs[i] * (other_cfg.equip_add_per * ROLE_ATTR_PER));
					Attribute attr_value = static_cast<Attribute>(temp_attrs_add.m_attrs[i] * (other_cfg.equip_add_per * ROLE_ATTR_PER));
					tmp_attr.AddValue(i, attr_value);
				}
			}
		}

		// 情比金坚装备
		for (int equip_index = 0; equip_index < QINGYUAN_EQUIP_TYPE_MAX; ++equip_index)
		{
			static  AttrConfig attr_cfg; attr_cfg.Reset();
			const QingyuanEquip2Cfg *upgrade_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanEquip2Cfg(equip_index, m_param.qingyuan_equip2_list[equip_index].equip2_level);
			if (NULL != upgrade_cfg)
			{
				tmp_attr.AddAttrFromConfig(upgrade_cfg->attr_cfg, (other_cfg.equip_add_per * ROLE_ATTR_PER));
			}
		}

		// 特殊宝宝
		for (unsigned int baby_type = 0; baby_type < SPECIAL_BABY_TYPE_MAX && baby_type < static_array_size(m_param.special_baby_list); ++baby_type)
		{
			int baby_quality = m_param.special_baby_list[baby_type].quality;
			int baby_lv = m_param.special_baby_list[baby_type].special_baby_level;
			if (baby_lv <= 0)
			{
				continue;
			}

			const SpecialBabyLvCfg *baby_lv_cfg = LOGIC_CONFIG->GetBabyCfg().GetSpecialBabyLvCfg(baby_type, baby_quality, baby_lv);
			if (nullptr != baby_lv_cfg)
			{
				tmp_attr.AddAttrFromConfig(baby_lv_cfg->attr_cfg);
			}
		}

		// 对方表白等级属性
		{
			double add_per = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanProfessAttrAddPer() * ROLE_ATTR_PER;

			// 判断是否有额外加成
			{
				bool is_self_add_extra_per = IsAddProfessSharePer();
				bool is_lover_add_extra_per = false;

				QingyuanGlobal * qingyuuan_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_param.lover_id);
				if (NULL != qingyuuan_global)
				{
					is_lover_add_extra_per = qingyuuan_global->IsAddProfessSharePer();
				}

				if (is_self_add_extra_per && is_lover_add_extra_per)
				{
					double add_profess_share_per = LOGIC_CONFIG->GetQingyuanCfg().GetProfessOtherCfg().add_profess_share_per * ROLE_ATTR_PER;
					add_per += add_profess_share_per;
				}
			}

			if (add_per > 0)
			{
				const QingyuanProfessGradeConfig *profess_grade_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetProfessGrade(m_param.profess_level);
				if (nullptr != profess_grade_cfg)
				{
					tmp_attr.AddAttrFromConfig(profess_grade_cfg->attr_cfg, add_per);
				}
			}
		}
	}
}

void QingyuanGlobal::SetLoverId(int lover_id, bool is_calc)
{
	Role * role = this->GetRole();
	if (NULL == role) return;

	if (m_param.lover_id != lover_id)
	{
		m_param.lover_id = lover_id;
		this->OnDataChange();
	}
	
	if (is_calc)
	{
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LOVE_TREE);
	}
}

void QingyuanGlobal::QingyuanEquipOperate(Role *role, int operate_type, int param_1, int param_2, int param_3)
{
	if (NULL == role) return;

	switch (operate_type)
	{
	case Protocol::QINGYUAN_EQUIP_REQ_SELF_EQUIP_INFO:
	{
		this->SendQingyuanGlobalInfo(role, true);
	}
	break;

	case Protocol::QINGYUAN_EQUIP_REQ_OTHER_EQUIP_INFO:
	{
		this->SendQingyuanGlobalInfo(role, false);
	}
	break;

	case Protocol::QINGYUAN_EQUIP_REQ_ACTIVE_SUIT:
	{
		this->ActiveQingyuanSuitSlot(role, param_1, param_2, param_3);
	}
	break;

	case Protocol::QINGYUAN_EQUIP_REQ_TAKE_OFF:
	{
		this->TakeOffQingyuanEquip(role, param_1);
	}
	break;

	case Protocol::QINGYUAN_EQUIP_REQ_EQUIP2_UPLEVEL:
	{
		this->OnUpQingyuanEquip2Level(role, param_1);
	}
	break;

	case Protocol::QINGYUAN_EQUIP_REQ_ACTIVE_SPECIAL_BABY:
	{
		this->OnActiveSpecialBaby(role, param_1, param_2);
	}
	break;

	case Protocol::QINGYUAN_EQUIP_REQ_BUY_GIFT:
	{
		this->OnBuyGift(role, param_1);
	}
	break;

	default:
		break;
	}
}

bool QingyuanGlobal::AddMarryLevelExp(Role *role ,int exp)
{
	if (NULL == role || exp <= 0) return false;

	const QingyuanMarryLevelConfig *cur_level_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetMarryLevelCfg(m_param.marry_level);
	if (NULL == cur_level_cfg)
	{
		return false;
	}

	const QingyuanMarryLevelConfig *next_level_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetMarryLevelCfg(m_param.marry_level + 1);
	if (NULL == next_level_cfg)
	{
		return false;
	}

	int old_marry_level = m_param.marry_level;
	m_param.marry_level_exp += exp;

	int up_level_need_exp = cur_level_cfg->up_level_exp;
	while (m_param.marry_level_exp > 0 && m_param.marry_level_exp >= up_level_need_exp)
	{
		next_level_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetMarryLevelCfg(m_param.marry_level + 1);
		if (NULL == next_level_cfg)
		{
			m_param.marry_level_exp = 0;
			break;
		}

		++m_param.marry_level;
		m_param.marry_level_exp -= up_level_need_exp;
		up_level_need_exp = next_level_cfg->up_level_exp;
	}

	if (old_marry_level != m_param.marry_level)
	{
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);
		role->NoticeNum(noticenum::NT_MARRY_UP_LEVEL_SUCC);
	}
	
	this->SendQingyuanGlobalInfo(role, true);

	gamelog::g_log_qingyuan.printf(LL_INFO, "MarryLevel: old_level[%d], cur_level[%d], add_exp_value[%d]", old_marry_level, m_param.marry_level, exp);

	return true;
}

void QingyuanGlobal::OnUpQingyuanEquip2Level(Role *role, int equip_index)
{
	if (NULL == role || equip_index < 0 || equip_index >= QINGYUAN_EQUIP_TYPE_MAX)
	{
		return ;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	const QingyuanEquip2Cfg *upgrade_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanEquip2Cfg(equip_index, m_param.qingyuan_equip2_list[equip_index].equip2_level);
	if (NULL == upgrade_cfg)
	{
		role->NoticeNum(errornum::EN_JINGLING_EQUIP_MAX_STRENGTH);
		return ;
	}

	const QingyuanEquip2Cfg *next_upgrade_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanEquip2Cfg(equip_index, m_param.qingyuan_equip2_list[equip_index].equip2_level + 1);
	if (NULL == next_upgrade_cfg)
	{
		role->NoticeNum(errornum::EN_JINGLING_EQUIP_MAX_STRENGTH);
		return;
	}

	//检查
	if (role->GetKnapsack()->Count(upgrade_cfg->stuff_id) < upgrade_cfg->stuff_num)
	{
		role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return;
	}

	// 消耗
	if (!role->GetKnapsack()->ConsumeItem(upgrade_cfg->stuff_id, upgrade_cfg->stuff_num, "QingyuanGlobal::OnUpQingyuanEquip2Level"))
	{
		return;
	}
	//生效
	++m_param.qingyuan_equip2_list[equip_index].equip2_level;
	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);

	Role *love_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_param.lover_id));
	if (NULL != love_role)
	{
		love_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);

		QingyuanGlobal * lover_qingyuan_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_param.lover_id);
		if (NULL != lover_qingyuan_global)
		{
			lover_qingyuan_global->SendQingyuanGlobalInfo(love_role, false);
		}
	}

	EventHandler::Instance().OnQingyuanEquip2UpLevel(role);

	// 发送
	this->SendQingyuanGlobalInfo(role, true);

	{
		// 日志

		gamelog::g_log_qingyuan.printf(LL_INFO, "QingyuanGlobal::OnUpQingyuanEquip2Level user[%d %s], equip_index[%d], level[%d], consume_id[%d], consume_num[%d]",
			role->GetUID(), role->GetName(), equip_index, (int)m_param.qingyuan_equip2_list[equip_index].equip2_level, upgrade_cfg->stuff_id, upgrade_cfg->stuff_num);
	}
}

void QingyuanGlobal::OnBuyGift(Role *role, int seq)
{
	if (NULL == role || seq < 0 || seq >= QINGYUAN_GIFT_MAX_COUNT)
	{
		return;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		role->NoticeNum(errornum::EN_HIDDEN_SERVER_OPERA_ERROR);
		return;
	}
	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
	const QingyuanGiftReward *gift_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetGiftCfg(opengame_day, seq);
	if (NULL == gift_cfg)
	{
		return;
	}

	// 检查
	if (IS_BIT_SET(m_param.today_buy_gift_flag, seq))
	{
		gstr::SendError(role, "already_buy");
		return;
	}

	if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(gift_cfg->item_count))
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(gift_cfg->need_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 消耗
	if (!role->GetKnapsack()->GetMoney()->UseGold(gift_cfg->need_gold, "QingyuanGlobal::OnBuyGift"))
	{
		return;
	}

	// 生效
	SET_BIT(m_param.today_buy_gift_flag, seq);

	{
		role->GetKnapsack()->PutList(gift_cfg->item_count, gift_cfg->reward_item_list, PUT_REASON_QINGYUAN_GIFT);

		gamecommon::SendMultiMailToRole(IntToUid(role->GetLoverUid()), gift_cfg->reward_item_list, gift_cfg->item_count, NULL, false, "buy_qingyuan_gift");
	}

	// 发送
	this->SendQingyuanGlobalInfo(role, true);

	{
		// 日志
		gamelog::g_log_qingyuan.printf(LL_INFO, "QingyuanGlobal::OnBuyGift user[%d %s], seq[%d], consumegold[%d]",
			role->GetUID(), role->GetName(), seq, gift_cfg->need_gold);
	}
}

void QingyuanGlobal::SetIsAllTaskComplete(short is_all_task_complete)
{
	if (m_param.is_all_task_complete != is_all_task_complete)
	{
		m_param.is_all_task_complete = is_all_task_complete;
		this->OnDataChange();
	}
}

int QingyuanGlobal::PutOnQingyuanEquip(Role *role, const ItemDataWrapper &item_wrapper)
{
	const Equipment *equip = (const Equipment*)ITEMPOOL->GetItem(item_wrapper.item_id);
	if (NULL == role || NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType())
	{
		return -1;
	}

	if (0 == item_wrapper.has_param)
	{
		return -2;
	}

	// 结婚等级是否满足
	if (m_param.marry_level < equip->GetLimitLevel())
	{
		role->NoticeNum(errornum::EN_QINGYUAN_MARRY_LEVEL_NOT_ENOUGH);
		return -3;
	}

	// 性别是否满足
	if (role->GetSex() != equip->GetLimitSex())
	{
		return -4;
	}

	if (equip->GetEquipType() < Equipment::E_TYPE_QINGYUAN_MIN || equip->GetEquipType() >= Equipment::E_TYPE_QINGYUAN_MAX)
	{
		return -5;
	}

	const int equip_index = this->GetEquipIndexByType(equip->GetEquipType());
	if (equip_index < QINGYUAN_EQUIP_TYPE_1 || equip_index >= QINGYUAN_EQUIP_TYPE_MAX)
	{
		return -6;
	}

	ItemDataWrapper &itemdata = m_param.qingyuan_equip_list[equip_index];
	if (!itemdata.Invalid())
	{
		if (this->TakeOffQingyuanEquip(role, equip_index) < 0)
		{
			return -8;
		}
	}

	// 加强判断
	if (!itemdata.Invalid() || item_wrapper.Invalid())
	{
		return -7;
	}

	itemdata.Reset(item_wrapper);

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);
	this->SendQingyuanGlobalInfo(role, true);
	this->OnDataChange();

	Role *love_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_param.lover_id));
	if (NULL != love_role)
	{
		love_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);
	}

	return 0;
}

int QingyuanGlobal::GetSatisfyMagicalPreciousEquipCount(int order, int color)
{
	int suit_equip_count = 0;
	for (const auto &equip_warpper : m_param.qingyuan_equip_list)
	{
		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(equip_warpper.item_id);
		if (nullptr != equip && ItemBase::I_TYPE_EQUIPMENT == equip->GetItemType())
		{
			// 策划需求，粉装以上直接满足条件
			if (equip->GetColor() >= ItemBase::I_COLOR_PINK)
			{
				++suit_equip_count;
				continue;
			}

			if (equip->GetColor() >= color && equip->GetOrder() >= order)
			{
				++suit_equip_count;
			}
		}
	}

	return suit_equip_count;
}

int QingyuanGlobal::GetActiveQingyuanEquip2Count()
{
	int active_num = 0;
	for (int i = 0; i < QINGYUAN_EQUIP_TYPE_MAX; i ++)
	{
		if (m_param.qingyuan_equip2_list[i].equip2_level > 0)
		{
			active_num++;
		}
	}

	return active_num;
}

void QingyuanGlobal::CheckSetAddProfessSharePer(int shizhuang_id)
{
	if (shizhuang_id <= 0)
	{
		return;
	}

	const QingyuanProfessOtherCfg &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetProfessOtherCfg();
	if (other_cfg.add_per_shizhuang_id == shizhuang_id)
	{
		this->SetAddProfessSharePer();
	}
}

bool QingyuanGlobal::OnActiveSpecialBaby(Role *role, int special_baby_type, int quality)
{
	if (nullptr == role) return false;

	if (special_baby_type < 0 || special_baby_type >= SPECIAL_BABY_TYPE_MAX || quality < 0 || quality >= SPECIAL_BABY_QUALITY_MAX)
	{
		gstr::SendError(role, "illegal_opera");
		return false;
	}

	if (!role->IsMarry())
	{
		role->NoticeNum(errornum::EN_QINGYUAN_EQUIP_NO_MARRY);
		return false;
	}

	int baby_lv = m_param.special_baby_list[special_baby_type].special_baby_level;
	if (baby_lv > 0)
	{
		gstr::SendError(role, "special_baby_already_active");
		return false;
	}

	const SpecialBabyLvCfg *baby_lv_cfg = LOGIC_CONFIG->GetBabyCfg().GetSpecialBabyLvCfg(special_baby_type, quality, 1);
	if (nullptr == baby_lv_cfg)
	{
		gstr::SendError(role, "get_cfg_error");
		return false;
	}

	// 消耗
	if (!role->GetKnapsack()->ConsumeItem(baby_lv_cfg->active_item_id, 1, "QingyuanGlobal::OnActiveSpecialBaby"))
	{
		return false;
	}

	m_param.special_baby_list[special_baby_type].special_baby_level = 1;
	m_param.special_baby_list[special_baby_type].quality = baby_lv_cfg->speical_baby_quality;

	Role *love_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_param.lover_id));
	if (NULL != love_role)
	{
		love_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);

		QingyuanGlobal * lover_qingyuan_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_param.lover_id);
		if (NULL != lover_qingyuan_global)
		{
			lover_qingyuan_global->SendQingyuanGlobalInfo(love_role, false);
		}
	}

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);

	// 发送
	this->SendQingyuanGlobalInfo(role, true);

	// 日志
	{
		gamelog::g_log_qingyuan.printf(LL_INFO, "QingyuanGlobal::OnActiveSpecialBaby user[%d %s], special_baby_type[%d]",
			role->GetUID(), role->GetName(), special_baby_type);
	}

	gstr::SendNotice(role, "active_succ");

	return true;
}

bool QingyuanGlobal::GetRandActiveSpecialBaby(Role *role, int &special_baby_type)
{
	if (NULL == role) return false;
	special_baby_type = -1;

	if (m_param.special_baby_list[SPECIAL_BABY_TYPE_0].special_baby_level > 0 && m_param.special_baby_list[SPECIAL_BABY_TYPE_1].special_baby_level > 0)
	{
		special_baby_type = RandomNum(SPECIAL_BABY_TYPE_MAX);
	}
	else if (m_param.special_baby_list[SPECIAL_BABY_TYPE_0].special_baby_level > 0)
	{
		special_baby_type = SPECIAL_BABY_TYPE_0;
	}
	else if (m_param.special_baby_list[SPECIAL_BABY_TYPE_1].special_baby_level > 0)
	{
		special_baby_type = SPECIAL_BABY_TYPE_1;
	}

	if (special_baby_type >= SPECIAL_BABY_TYPE_0 && special_baby_type < SPECIAL_BABY_TYPE_MAX)
	{
		return true;
	}

	return false;
}

void QingyuanGlobal::SendQingyuanGlobalInfo(Role *role, bool is_self)
{
	if (NULL == role) return;

	Protocol::SCQingyuanEquipInfo qyei;
	qyei.is_self = is_self ? 1 : 0;

	if (is_self)
	{
		qyei.marry_level = m_param.marry_level;
		qyei.marry_level_exp = m_param.marry_level_exp;
		memcpy(qyei.qingyuan_suit_flag, m_param.qingyuan_suit_flag, sizeof(qyei.qingyuan_suit_flag));
		qyei.buy_gift_flag = m_param.today_buy_gift_flag;
		qyei.reserve = 0;
		memcpy(qyei.qingyuan_equip_list, m_param.qingyuan_equip_list, sizeof(qyei.qingyuan_equip_list));
		memcpy(qyei.qingyuan_equip2_list, m_param.qingyuan_equip2_list, sizeof(qyei.qingyuan_equip2_list));
		memcpy(qyei.special_baby_list, m_param.special_baby_list, sizeof(qyei.special_baby_list));
	}
	else
	{
		QingyuanGlobal * self_love_tree = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_param.lover_id);
		if (NULL == self_love_tree)
		{
			role->NoticeNum(errornum::EN_QINGYUAN_EQUIP_NO_MARRY);
			return;
		}

		qyei.marry_level = self_love_tree->m_param.marry_level;
		qyei.marry_level_exp = self_love_tree->m_param.marry_level_exp;
		memcpy(qyei.qingyuan_suit_flag, self_love_tree->m_param.qingyuan_suit_flag, sizeof(qyei.qingyuan_suit_flag));
		qyei.buy_gift_flag = self_love_tree->m_param.today_buy_gift_flag;
		qyei.reserve = 0;
		memcpy(qyei.qingyuan_equip_list, self_love_tree->m_param.qingyuan_equip_list, sizeof(qyei.qingyuan_equip_list));
		memcpy(qyei.qingyuan_equip2_list, self_love_tree->m_param.qingyuan_equip2_list, sizeof(qyei.qingyuan_equip2_list));
		memcpy(qyei.special_baby_list, self_love_tree->m_param.special_baby_list, sizeof(qyei.special_baby_list));
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&qyei, sizeof(qyei));
}

int QingyuanGlobal::TakeOffQingyuanEquip(Role *role, int equip_index)
{
	// 判断传入的下标以及用其取出的装备
	if (NULL == role || equip_index < 0 || equip_index >= QINGYUAN_EQUIP_TYPE_MAX)
	{
		return -1;
	}

	const ItemDataWrapper &item_data = m_param.qingyuan_equip_list[equip_index];
	if (item_data.Invalid())
	{
		return -2;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_data.item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT ||
		equip->GetEquipType() < Equipment::E_TYPE_QINGYUAN_MIN || equip->GetEquipType() >= Equipment::E_TYPE_QINGYUAN_MAX)
	{
		return -3;
	}

	// 判断背包
	Knapsack *knapsack = role->GetKnapsack();
	if (!knapsack->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return -4;
	}

	if (!knapsack->Put(item_data, PUT_REASON_NO_NOTICE))
	{
		return -5;
	}

	m_param.qingyuan_equip_list[equip_index].Reset();

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);
	this->SendQingyuanGlobalInfo(role, true);
	this->OnDataChange();

	Role *love_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_param.lover_id));
	if (NULL != love_role)
	{
		love_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);
	}

	return 0;
}

int QingyuanGlobal::GetEquipIndexByType(int equip_type)
{
	return equip_type - Equipment::E_TYPE_QINGYUAN_MIN;
}

void QingyuanGlobal::ActiveQingyuanSuitSlot(Role *role, int type, int slot, int knapack_index)
{
	if (NULL == role) return;

	if (knapack_index < 0 || knapack_index >= ItemNamespace::MAX_KNAPSACK_GRID_NUM) return;

	if (type < 0 || type >= QINGYUAN_EQUIPSUIT_MAX_COUNT) return;

	if (slot < 0 || slot >= QINGYUAN_EQUIPSUIT_SLOT_MAX_COUNT) return;

	unsigned char &slot_flag = m_param.qingyuan_suit_flag[type];

	if (0 != (slot_flag & 1 << slot))
	{
		role->NoticeNum(errornum::EN_QINGYUAN_EQUIP_SUIT_ACTIVE_LIMIT);
		return;
	}

	const QingyuanEquipSuitConfig *suit_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanEquipSuitCfg(type, slot);
	if (NULL == suit_cfg) return;

	if (!CheckSuitCondition(role, type, slot))
	{
		return;
	}

	ItemGridData item_grid_data;
	role->GetKnapsack()->GetItemGridData(knapack_index, &item_grid_data);

	int consume_item_id = MALE == role->GetSex() ? suit_cfg->man_item : suit_cfg->woman_item;
	if (item_grid_data.Invalid() || item_grid_data.item_id != consume_item_id)
	{
		return;
	}

	if (!role->GetKnapsack()->ConsumeItemByIndex(knapack_index, 1, "ActiveEquipSuit")) return;

	slot_flag |= (1 << slot);

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);
	this->SendQingyuanGlobalInfo(role, true);
	this->OnDataChange();

	Role *love_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_param.lover_id));
	if (NULL != love_role)
	{
		love_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);
	}
}

bool QingyuanGlobal::CheckSuitCondition(Role *role, int type, int slot)
{
	const QingyuanEquipSuitConfig *suit_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanEquipSuitCfg(type, slot);
	if (NULL == suit_cfg) return false;

	for (int equip_index = 0; equip_index < QINGYUAN_EQUIP_TYPE_MAX; equip_index++)
	{
		ItemDataWrapper &itemdata = m_param.qingyuan_equip_list[equip_index];
		if (itemdata.Invalid())
		{
			role->NoticeNum(errornum::EN_QINGYUAN_EQUIP_SUIT_EQUIP_ATTR_LIMIT);
			return false;
		}

		const Equipment *equip = (const Equipment*)ITEMPOOL->GetItem(itemdata.item_id);
		if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType())
		{
			return false;
		}

		if (equip->GetColor() < suit_cfg->color_limit || equip->GetOrder() < suit_cfg->order_limit)
		{
			role->NoticeNum(errornum::EN_QINGYUAN_EQUIP_SUIT_EQUIP_ATTR_LIMIT);
			return false;
		}
	}

	return true;
}

