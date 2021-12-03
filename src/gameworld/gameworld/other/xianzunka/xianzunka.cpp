#include "xianzunka.hpp"
#include "xianzunkaconfig.hpp"
#include "gameworld/obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "gameworld/engineadapter.h"
#include "gameworld/item/knapsack.h"
#include "gameworld/item/money.h"
#include "gameworld/config/logicconfigmanager.hpp"
#include "gameworld/gamelog.h"
#include "gameworld/other/wing/wingmanager.hpp"
#include "other/route/mailroute.hpp"
#include "servercommon/string/gameworldstr.h"
#include "protocal/msgother.h"
#include "other/capability/capability.hpp"
#include "scene/activityshadow/activityshadow.hpp"

XianZunKa::XianZunKa() : m_other_capability(0)
{

}

XianZunKa::~XianZunKa()
{

}

void XianZunKa::InitParam(Role *role, const XianZunKaParam &param)
{
	m_role = role;
	m_param = param;
}

void XianZunKa::GetInitParam(XianZunKaParam *param)
{
	*param = m_param;
}

void XianZunKa::Update(unsigned int now_second)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	for(int i = 0; i < XIANZUNKA_TYPE_MAX; ++i)
	{
		if(m_param.temporary_valid_end_timestamp_list[i] != 0 && m_param.temporary_valid_end_timestamp_list[i] < now_second && (m_param.sent_emails_flag  & (1 << i)) == 0)
		{
			static MailContentParam contentparam; contentparam.Reset();

			const auto length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_xian_zun_ka_stale_dated);
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}

			m_param.sent_emails_flag |= (1 << i);

			{
				this->OnActiveFlagChange();
				this->ReCalcAttrBeLink();
			}
		}
	}
}

void XianZunKa::OnAddChongzhi(long long chongzhi_gold)
{
	if (chongzhi_gold <= 0)
	{
		return;
	}
	if (!m_param.is_first_recharge)
	{
		m_param.is_first_recharge = 1;
		for (int i = XIANZUNKA_TYPE_SILVER; i < XIANZUNKA_TYPE_MAX; ++i)
		{
			this->OnGiveTemporaryXianZunKa(i);
		}
	}
	OnXianZunKaActivityChongZhi(chongzhi_gold);
}

void XianZunKa::ReCalcAttr(CharIntAttrs & base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		int active_count = this->GetActiviteCount();
		if (active_count >= XIANZUNKA_TYPE_MAX)
		{
			const XianZunKaOtherCfg &other_cfg = LOGIC_CONFIG->GetXianZunKaConfig().GetOtherCfg();
			m_attrs_add.AddValue(other_cfg.active_attribute_type, other_cfg.active_attribute_value);
		}
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_XIANZUNKA, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_XIANZUNKA, m_other_capability);

	m_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_XIANZUNKA);

	base_add.Add(m_attrs_add.m_attrs, true);
}

void XianZunKa::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	m_param.daily_reward_fetch_flag = 0;

	this->SendAllInfo();
}

bool XianZunKa::OnUseXianZunKa(int card_type)
{
	if (card_type < XIANZUNKA_TYPE_SILVER || card_type >= XIANZUNKA_TYPE_MAX)
	{
		return false;
	}

	if (0 != (m_param.forever_active_flag & (1 << card_type)))
	{
		m_role->NoticeNum(errornum::EN_XIANZUNKA_HAS_ACTIVE);
		return false;
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (m_param.temporary_valid_end_timestamp_list[card_type] < now_second)
	{
		m_role->NoticeNum(errornum::EN_XIANZUNKA_NOT_ACTIVE);
		return false;
	}

	m_param.forever_active_flag |= (1 << card_type);

	this->OnActiveFlagChange();
	this->ReCalcAttrBeLink();

	gamelog::g_log_xianzunka.printf(LL_INFO, "XianZunKa::OnUseXianZunKa User[%d %s] card_type[%d], active_flag[%d]",
		m_role->GetUID(), m_role->GetName(), card_type, m_param.forever_active_flag);

	this->SendAllInfo();

	return true;
}

void XianZunKa::OnBuyXianZunKa(int card_type)		// 策划需求 去除首次奖励 每次购买续费都发奖励
{
	if (card_type < XIANZUNKA_TYPE_SILVER || card_type >= XIANZUNKA_TYPE_MAX)
	{
		return;
	}

	// 检查消耗
	if (0 != (m_param.forever_active_flag & (1 << card_type)))
	{
		m_role->NoticeNum(errornum::EN_XIANZUNKA_HAS_ACTIVE);
		return;
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (m_param.temporary_valid_end_timestamp_list[card_type] > now_second)
	{
		m_role->NoticeNum(errornum::EN_XIANZUNKA_HAS_ACTIVE);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const XianZunKaBaseConfig *xianzunka_cfg = LOGIC_CONFIG->GetXianZunKaConfig().GetXianZunKaBaseCfg(card_type);
	if (nullptr == xianzunka_cfg)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(xianzunka_cfg->need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(xianzunka_cfg->need_gold, "XianZunKa::OnBuyXianZunKa"))
	{
		return;
	}

	m_role->GetKnapsack()->Put(xianzunka_cfg->first_reward_item, PUT_REASON_XIANZUNKA_FIRST_ACTIVE_REWARD);	//只要购买续费就发

	m_param.sent_emails_flag &= ~(1 << card_type);

	m_param.temporary_valid_end_timestamp_list[card_type] = now_second + xianzunka_cfg->valid_day * SECOND_PER_DAY;

	this->OnActiveFlagChange();
	this->ReCalcAttrBeLink();

	gamelog::g_log_xianzunka.printf(LL_INFO, "XianZunKa::OnBuyXianZunKa User[%d %s] card_type[%d] consume_gold[%d] valid_end_timestamp[%u]",
		m_role->GetUID(), m_role->GetName(), card_type, xianzunka_cfg->need_gold, m_param.temporary_valid_end_timestamp_list[card_type]);

	this->SendAllInfo();
}

void XianZunKa::OnGiveTemporaryXianZunKa(int card_type)
{
	if (card_type < XIANZUNKA_TYPE_SILVER || card_type >= XIANZUNKA_TYPE_MAX)
	{
		return;
	}

	const XianZunKaBaseConfig *xianzunka_cfg = LOGIC_CONFIG->GetXianZunKaConfig().GetXianZunKaBaseCfg(card_type);
	if (nullptr == xianzunka_cfg || xianzunka_cfg->gift_day == 0 || xianzunka_cfg->free_time_limit == 0)
	{
		return;
	}

	const time_t now_sec = EngineAdapter::Instance().Time();
	const time_t role_create_time = m_role->GetCreateRoleTimestamp();
	if (now_sec > role_create_time + xianzunka_cfg->free_time_limit)
	{
		return;
	}

	if (0 != (m_param.forever_active_flag & (1 << card_type)))
	{
		m_role->NoticeNum(errornum::EN_XIANZUNKA_HAS_ACTIVE);
		return;
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (m_param.temporary_valid_end_timestamp_list[card_type] > now_second)
	{
		m_role->NoticeNum(errornum::EN_XIANZUNKA_HAS_ACTIVE);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_role->GetKnapsack()->Put(xianzunka_cfg->first_reward_item, PUT_REASON_XIANZUNKA_FIRST_ACTIVE_REWARD);	//只要购买续费就发

	m_param.sent_emails_flag &= ~(1 << card_type);

	m_param.temporary_valid_end_timestamp_list[card_type] = now_second + xianzunka_cfg->gift_day * SECOND_PER_DAY;

	this->OnActiveFlagChange();
	this->ReCalcAttrBeLink();

	gamelog::g_log_xianzunka.printf(LL_INFO, "XianZunKa::OnGiveTemporaryXianZunKa User[%d %s] card_type[%d] valid_end_timestamp[%u]",
		m_role->GetUID(), m_role->GetName(), card_type, m_param.temporary_valid_end_timestamp_list[card_type]);

	this->SendAllInfo();
}

void XianZunKa::OnFetchDailyReward(int card_type)
{
	if (card_type < XIANZUNKA_TYPE_SILVER || card_type >= XIANZUNKA_TYPE_MAX)
	{
		return;
	}

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (m_param.temporary_valid_end_timestamp_list[card_type] < now_sec && (0 == (m_param.forever_active_flag & (1 << card_type))))
	{
		m_role->NoticeNum(errornum::EN_XIANZUNKA_NOT_ACTIVE);
		return;
	}

	if (0 != (m_param.daily_reward_fetch_flag & (1 << card_type)))
	{
		m_role->NoticeNum(errornum::EN_XIANZUNKA_DAILY_REWARD_HAS_FETCH);
		return;
	}

	const XianZunKaBaseConfig *xianzunka_cfg = LOGIC_CONFIG->GetXianZunKaConfig().GetXianZunKaBaseCfg(card_type);
	if (nullptr == xianzunka_cfg || xianzunka_cfg->daily_reward_bind_gold == 0)
	{
		return;
	}

	m_param.daily_reward_fetch_flag |= (1 << card_type);

	m_role->GetKnapsack()->GetMoney()->AddGoldBind(xianzunka_cfg->daily_reward_bind_gold, "XianZunKa::OnFetchDailyReward");

	this->SendAllInfo();

	gamelog::g_log_xianzunka.printf(LL_INFO, "XianZunKa::OnFetchDailyReward User[%d %s] card_type[%d], fetch_flag[%d]",
		m_role->GetUID(), m_role->GetName(), card_type, m_param.daily_reward_fetch_flag);
}

void XianZunKa::OnActiveXianZunKa(int card_type)
{
	if (card_type < XIANZUNKA_TYPE_SILVER || card_type >= XIANZUNKA_TYPE_MAX)
	{
		return;
	}

	const XianZunKaBaseConfig *xianzun_cfg = LOGIC_CONFIG->GetXianZunKaConfig().GetXianZunKaBaseCfg(card_type);
	if (nullptr == xianzun_cfg)
	{
		return;
	}

	if (m_role->GetKnapsack()->Count(xianzun_cfg->active_item_id) <= 0)
	{
		m_role->NoticeNum(errornum::EN_XIANZUNKA_ACTIVE_ITEM_LACK);
		return;
	}

	if (0 != (m_param.forever_active_flag & (1 << card_type)))
	{
		m_role->NoticeNum(errornum::EN_XIANZUNKA_HAS_ACTIVE);
		return;
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (m_param.temporary_valid_end_timestamp_list[card_type] < now_second)
	{
		m_role->NoticeNum(errornum::EN_XIANZUNKA_NOT_ACTIVE);
		return;
	}

	if (!m_role->GetKnapsack()->ConsumeItem(xianzun_cfg->active_item_id, 1, "XianZunKa::OnActiveXianZunKa"))
	{
		return;
	}

	m_param.forever_active_flag |= (1 << card_type);

	this->OnActiveFlagChange();
	this->ReCalcAttrBeLink();

	gamelog::g_log_xianzunka.printf(LL_INFO, "XianZunKa::OnUseXianZunKa User[%d %s] card_type[%d], active_flag[%d]",
		m_role->GetUID(), m_role->GetName(), card_type, m_param.forever_active_flag);

	this->SendAllInfo();
}

void XianZunKa::OnActiveFlagChange()
{
	m_role->UpdateUserCache();
}

short XianZunKa::SetAndGetHasXianzunkaFlag()
{
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	for (int card_type = XIANZUNKA_TYPE_SILVER; card_type < XIANZUNKA_TYPE_MAX; ++card_type)
	{
		if (0 == (m_param.forever_active_flag & (1 << card_type)) && now_second > m_param.temporary_valid_end_timestamp_list[card_type])
		{
			continue;
		}
		m_role->SetHasXianzunkaFlag(card_type);
	}
	return m_role->GetHasXianzunkaFlag();
}

void XianZunKa::ReCalcAttrBeLink()
{
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANZUNKA);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);
}

const int XianZunKa::GetAddExpPer()
{
	unsigned int now_second = static_cast<unsigned>(EngineAdapter::Instance().Time());

	int addexp_per = 0;

	for (int i = XIANZUNKA_TYPE_SILVER; i < XIANZUNKA_TYPE_MAX; ++i)
	{
		if (0 == (m_param.forever_active_flag & (1 << i)) && now_second >= m_param.temporary_valid_end_timestamp_list[i])
		{
			continue;
		}

		const XianZunKaAdditionConfig *addition_cfg = LOGIC_CONFIG->GetXianZunKaConfig().GetXianZunKaAdditionCfg(i);
		if (nullptr == addition_cfg) continue;

		addexp_per += addition_cfg->add_exp_per;
	}

	return addexp_per;
}

const int XianZunKa::GetAddEquipStrengthSuccRate()
{
	unsigned int now_second = static_cast<unsigned>(EngineAdapter::Instance().Time());

	int strength_succ_rate = 0;

	for (int i = XIANZUNKA_TYPE_SILVER; i < XIANZUNKA_TYPE_MAX; ++i)
	{
		if (0 == (m_param.forever_active_flag & (1 << i)) && now_second >= m_param.temporary_valid_end_timestamp_list[i])
		{
			continue;
		}

		const XianZunKaAdditionConfig *addition_cfg = LOGIC_CONFIG->GetXianZunKaConfig().GetXianZunKaAdditionCfg(i);
		if (nullptr != addition_cfg)
		{
			strength_succ_rate += addition_cfg->add_equip_strength_succ_rate;
		}
	}

	return strength_succ_rate / 100;
}

const int XianZunKa::GetAddMountGradeAttrsPer()
{
	unsigned int now_second = static_cast<unsigned>(EngineAdapter::Instance().Time());

	int mount_grade_attrs_per = 0;

	for (int i = XIANZUNKA_TYPE_SILVER; i < XIANZUNKA_TYPE_MAX; ++i)
	{
		if (0 == (m_param.forever_active_flag & (1 << i)) && now_second > m_param.temporary_valid_end_timestamp_list[i])
		{
			continue;
		}

		const XianZunKaAdditionConfig *addition_cfg = LOGIC_CONFIG->GetXianZunKaConfig().GetXianZunKaAdditionCfg(i);
		if (nullptr != addition_cfg)
		{
			mount_grade_attrs_per += addition_cfg->add_mount_grade_attrs_per;
		}
	}

	return mount_grade_attrs_per;
}

const int XianZunKa::GetAddWingGradeAttrsPer()
{
	unsigned int now_second = static_cast<unsigned>(EngineAdapter::Instance().Time());

	int wing_grade_attrs_per = 0;

	for (int i = XIANZUNKA_TYPE_SILVER; i < XIANZUNKA_TYPE_MAX; ++i)
	{
		if (0 == (m_param.forever_active_flag & (1 << i)) && now_second > m_param.temporary_valid_end_timestamp_list[i])
		{
			continue;
		}

		const XianZunKaAdditionConfig *addition_cfg = LOGIC_CONFIG->GetXianZunKaConfig().GetXianZunKaAdditionCfg(i);
		if (nullptr != addition_cfg)
		{
			wing_grade_attrs_per += addition_cfg->add_wing_grade_attrs_per;
		}
	}

	return wing_grade_attrs_per;
}

const int XianZunKa::GetAddShenbingGradeAttrsPer()
{
	unsigned int now_second = static_cast<unsigned>(EngineAdapter::Instance().Time());

	int grade_attrs_per = 0;

	for (int i = XIANZUNKA_TYPE_SILVER; i < XIANZUNKA_TYPE_MAX; ++i)
	{
		if (0 == (m_param.forever_active_flag & (1 << i)) && now_second > m_param.temporary_valid_end_timestamp_list[i])
		{
			continue;
		}

		const XianZunKaAdditionConfig *addition_cfg = LOGIC_CONFIG->GetXianZunKaConfig().GetXianZunKaAdditionCfg(i);
		if (nullptr != addition_cfg)
		{
			grade_attrs_per += addition_cfg->add_shenbing_grade_attrs_per;
		}
	}

	return grade_attrs_per;
}

const int XianZunKa::GetAddShizhuangGradeAttrsPer()
{
	unsigned int now_second = static_cast<unsigned>(EngineAdapter::Instance().Time());

	int grade_attrs_per = 0;

	for (int i = XIANZUNKA_TYPE_SILVER; i < XIANZUNKA_TYPE_MAX; ++i)
	{
		if (0 == (m_param.forever_active_flag & (1 << i)) && now_second > m_param.temporary_valid_end_timestamp_list[i])
		{
			continue;
		}

		const XianZunKaAdditionConfig *addition_cfg = LOGIC_CONFIG->GetXianZunKaConfig().GetXianZunKaAdditionCfg(i);
		if (nullptr != addition_cfg)
		{
			grade_attrs_per += addition_cfg->add_shizhuang_grade_attrs_per;
		}
	}

	return grade_attrs_per;
}

int XianZunKa::GetActiviteCount()
{
	unsigned int now_second = static_cast<unsigned>(EngineAdapter::Instance().Time());
	int count = 0;
	for (int i = XIANZUNKA_TYPE_SILVER; i < XIANZUNKA_TYPE_MAX; ++i)
	{
		if (0 == (m_param.forever_active_flag & (1 << i)) && now_second > m_param.temporary_valid_end_timestamp_list[i])
		{
			continue;
		}
		++count;
	}
	return count;
}

void XianZunKa::SendAllInfo()
{
	Protocol::SCXianZunKaAllInfo xzkai;

	xzkai.forever_active_flag = m_param.forever_active_flag;
	xzkai.first_active_reward_flag = m_param.first_active_reward_flag;
	xzkai.daily_reward_fetch_flag = m_param.daily_reward_fetch_flag;

	UNSTD_STATIC_CHECK(sizeof(xzkai.temporary_valid_end_timestamp_list) == sizeof(m_param.temporary_valid_end_timestamp_list));
	memcpy(xzkai.temporary_valid_end_timestamp_list, m_param.temporary_valid_end_timestamp_list, sizeof(xzkai.temporary_valid_end_timestamp_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&xzkai, sizeof(xzkai));
}

void XianZunKa::OnXianZunKaActivityChongZhi(long long chongzhi_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_XIAN_ZUN_KA))
	{
		return;
	}
	
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	
	bool have_active = false;
	for (int i = XIANZUNKA_TYPE_SILVER; i < XIANZUNKA_TYPE_MAX; ++i)
	{
		if (now_sec <= m_param.temporary_valid_end_timestamp_list[i])
		{
			const XianZunKaBaseConfig *xianzunka_cfg = LOGIC_CONFIG->GetXianZunKaConfig().GetXianZunKaBaseCfg(i);
			if (NULL != xianzunka_cfg && chongzhi_gold >= xianzunka_cfg->active_chong_zhi)
			{
				m_param.forever_active_flag |= (1 << i);
				have_active = true;
			}
		}
	}

	if (have_active)
	{
		this->SendAllInfo();
	}
}