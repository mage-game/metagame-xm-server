#include "huashen.hpp"
#include "huashenconfig.hpp"
#include "protocal/msghuashen.h"

#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "config/logicconfigmanager.hpp"
#include "other/specialappearance/specialappearance.hpp"
#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"

HuaShen::HuaShen() : m_role(NULL)
{
}

HuaShen::~HuaShen()
{
}

void HuaShen::Init(Role *role, const HuashenParam &param)
{
	m_role = role; 
	m_param = param; 
}

void HuaShen::GetInitParam(HuashenParam *param)
{
	*param = m_param; 
}

void HuaShen::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		for (int id = 0; id <= HUASHEN_MAX_ID_LIMIT; ++ id)
		{
			if (this->IsHuaShenIDActive(id))
			{
				const HuaShenInfoConfig *info_cfg = LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenInfoCfg(id);
				if (NULL != info_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += info_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += info_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += info_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += info_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += info_cfg->shanbi;
				}
			}

			const HuaShenLevelConfig *level_cfg = LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenLevelCfg(id, m_param.level_info_list[id].level);
			if (NULL != level_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;
			}

			// 精灵战力
			for (int spirit_id = 0; spirit_id <= HUASHEN_SPIRIT_MAX_ID_LIMIT; ++ spirit_id)
			{
				int spirit_level = m_param.spirit_info_list[id].spirit_list[spirit_id].level;

				const HuaShenSpiritConfig::SpiritLevelConfig * spirit_level_cfg = LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenSpiritLevelCfg(spirit_id, spirit_level);
				if (NULL != spirit_level_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += spirit_level_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += spirit_level_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += spirit_level_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += spirit_level_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += spirit_level_cfg->shanbi;
				}
			}

			const HuaShenGradeConfig *grade_cfg = LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenGradeCfg(id, m_param.grade_list[id]);
			if (NULL != grade_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += grade_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += grade_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += grade_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += grade_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += grade_cfg->shanbi;
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_HUASHEN, m_attrs_add);
	}
	base_add.Add(m_attrs_add.m_attrs, true);
}

void HuaShen::SendHuaShenAllInfo()
{
	static Protocol::SCHuaShenAllInfo hsai;

	hsai.active_flag = m_param.active_flag;
	hsai.cur_huashen_id = m_param.cur_huashen_id;
	hsai.dirty_sh = 0;
	memcpy(hsai.level_info_list, m_param.level_info_list, sizeof(hsai.level_info_list));
	memcpy(hsai.grade_list, m_param.grade_list, sizeof(hsai.grade_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&hsai, sizeof(hsai));
}

void HuaShen::SendHuashenSpiritAllInfo()
{
	for (int huashen_id = 0; huashen_id <= HUASHEN_MAX_ID_LIMIT; ++ huashen_id)
	{
		this->SendHuaShenSpiritInfo(huashen_id);
	}
}

void HuaShen::SendHuaShenSpiritInfo(int huashen_id)
{
	if (huashen_id < 0 || huashen_id > LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenMaxID() || huashen_id > HUASHEN_MAX_ID_LIMIT)
	{
		return;
	}

	static Protocol::SCHuaShenSpiritInfo hssi;
	hssi.huashen_id = huashen_id;
	memcpy(hssi.spirit_list, m_param.spirit_info_list[huashen_id].spirit_list, sizeof(hssi.spirit_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&hssi, sizeof(hssi));
}

void HuaShen::ChangeImage(int id)			// id是要切换的化神
{
	if (!this->IsHuaShenIDActive(id))
	{
		m_role->NoticeNum(errornum::EN_HUASHEN_ID_NOT_ACTIVE);
		return;
	}

	int old_id = m_param.cur_huashen_id;

	if (m_param.cur_huashen_id == id)		// 表示要取消化神形象
	{
		m_param.cur_huashen_id = HuashenParam::HUASHEN_ID_INVALID;

		m_role->GetSpecialAppearance()->SetHuaShenAppearance(SPECIAL_APPEARANCE_TYPE_NORMAL, 0, 0, true);
		m_role->NoticeNum(noticenum::NT_HUASHEN_PUTOFF_IMAGE);
	}
	else
	{
		m_param.cur_huashen_id = id;

		const HuaShenInfoConfig *info_cfg = LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenInfoCfg(id);
		if (NULL == info_cfg)
		{
			return;
		}

		m_role->GetSpecialAppearance()->SetHuaShenAppearance(SPECIAL_APPERANCE_TYPE_CHANGE_HUASHEN_IMAGE, info_cfg->monster_id, 0, true);
		m_role->NoticeNum(noticenum::NT_HUASHEN_CHANGE_IMAGE);
	}

	this->SendHuaShenAllInfo();

	gamelog::g_log_huashen.printf(LL_INFO, "HuaShen::ChangeImage user[%d, %s], id[%d->%d]",
		m_role->GetUID(), m_role->GetName(), old_id, m_param.cur_huashen_id);
}

bool HuaShen::UpLevel(int id, bool is_auto_buy)
{
	if (id < 0 || id > HUASHEN_MAX_ID_LIMIT)
	{
		return false;
	}

	HuaShenLevelInfo &level_info = m_param.level_info_list[id];

	const HuaShenInfoConfig *info_cfg = LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenInfoCfg(id);
	if (NULL == info_cfg)
	{
		return false;
	}

	if (level_info.level >= info_cfg->max_level)
	{
		m_role->NoticeNum(errornum::EN_HUASHEN_LEVEL_FULL);
		return false;
	}

	const HuaShenLevelConfig *level_cfg = LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenLevelCfg(id, level_info.level + 1);
	if (NULL == level_cfg)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	{
		// 获取可消耗的背包材料列表
		short stuff_count = 0;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = level_cfg->stuff_id;
		stuff_list[stuff_count].num = level_cfg->stuff_num;
		stuff_list[stuff_count].buyable = is_auto_buy; 

		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list))
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "HuaShen::UpLevel"))
	{
		return false;
	}

	// 升级
	level_info.jinhua_val += LOGIC_CONFIG->GetHuaShenCfg().GetOtherCfg().level_once_val;

	if (level_info.jinhua_val >= level_cfg->jinhua_val)
	{
		level_info.jinhua_val -= level_cfg->jinhua_val;
		++ level_info.level;

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HUASHEN);
	}

	this->SendHuaShenAllInfo();

	// 日志
	gamelog::g_log_huashen.printf(LL_INFO, "HuaShen::UpLevel user[%d, %s], id[%d], level[%d], jinhua_val[%d], stuff_id[%d], stuff_num[%d], is_auto_up[%d], need+gold_num[%d]",
		m_role->GetUID(), m_role->GetName(), id, level_info.level, level_info.jinhua_val, level_cfg->stuff_id, level_cfg->stuff_num, is_auto_buy, consume_list.need_gold_num);

	return true;
}

bool HuaShen::UpGrade(int id, bool is_auto_buy)
{
	if (id < 0 || id > HUASHEN_MAX_ID_LIMIT)
	{
		return false;
	}

	short grade = m_param.grade_list[id];

	const HuaShenInfoConfig *info_cfg = LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenInfoCfg(id);
	if (NULL == info_cfg)
	{
		return false;
	}

	if (grade >= info_cfg->max_grade)
	{
		m_role->NoticeNum(errornum::EN_HUASHEN_GRADE_FULL);
		return false;
	}

	const HuaShenGradeConfig *grade_cfg = LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenGradeCfg(id, grade + 1);
	if (NULL == grade_cfg)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	{
		// 获取可消耗的背包材料列表
		short stuff_count = 0;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = grade_cfg->stuff_id;
		stuff_list[stuff_count].num = grade_cfg->stuff_num;
		stuff_list[stuff_count].buyable = is_auto_buy; 

		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list))
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "HuaShen::UpGrade"))
	{
		return false;
	}

	++ m_param.grade_list[id];
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HUASHEN);

	this->SendHuaShenAllInfo();

	// 日志
	gamelog::g_log_huashen.printf(LL_INFO, "HuaShen::UpGrade user[%d, %s], id[%d], grade[%d], stuff_id[%d], stuff_num[%d], is_auto_up[%d], need+gold_num[%d]",
		m_role->GetUID(), m_role->GetName(), id, m_param.grade_list[id], grade_cfg->stuff_id, grade_cfg->stuff_num, is_auto_buy, consume_list.need_gold_num);

	return true;
}

bool HuaShen::OnUseHuaShenCard(int id)
{
	if (this->IsHuaShenIDActive(id))
	{
		m_role->NoticeNum(errornum::EN_HUASHEN_ID_HAVE_ACTIVE);
		return false;
	}

	if (id < 0 || id > LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenMaxID() || id > HUASHEN_MAX_ID_LIMIT)		// 就算没激活，也不能激活超过范围的
	{
		return false;
	}

	m_param.active_flag |= (1 << id);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HUASHEN);

	this->SendHuaShenAllInfo();

	// 日志
	gamelog::g_log_huashen.printf(LL_INFO, "HuaShen::OnUseHuaShenCard user[%d, %s], id[%d], active_flag[%d]",
		m_role->GetUID(), m_role->GetName(), id, m_param.active_flag);

	return true;
}

bool HuaShen::OnUpgradeSpirit(int huashen_id, int spirit_id, bool is_auto_upgrade)
{
	// 检查
	if (huashen_id < 0 || huashen_id > HUASHEN_MAX_ID_LIMIT || spirit_id < 0 || spirit_id > HUASHEN_SPIRIT_MAX_ID_LIMIT)
	{
		return false;
	}

	HuaShenSpiritInfo::SingleSpirit &spirit_info = m_param.spirit_info_list[huashen_id].spirit_list[spirit_id];

	if (spirit_info.level >= LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenSpiritMaxLevel(spirit_id))
	{
		m_role->NoticeNum(errornum::EN_HUASHEN_SPIRIT_LEVEL_FULL);
		return false;
	}

	const HuaShenSpiritConfig::SpiritLevelConfig * spirit_cfg = LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenSpiritLevelCfg(spirit_id, spirit_info.level);
	if (NULL == spirit_cfg)
	{
		return false;
	}

	// 计算需要升级几次
	if (spirit_cfg->consume_item_count <= 0 || spirit_cfg->add_exp_val <= 0)
	{
		return false;
	}

	int real_upgrade_times = m_role->GetKnapsack()->Count(spirit_cfg->consume_item_id) / spirit_cfg->consume_item_count;

	{
		int need_upgrade_times = 1;
		if (is_auto_upgrade)
		{
			int last_exp_val = spirit_cfg->need_exp_val - spirit_info.exp_val;

			need_upgrade_times = last_exp_val / spirit_cfg->add_exp_val;
			if ((last_exp_val % spirit_cfg->add_exp_val) != 0)
			{
				++ need_upgrade_times;
			}
		}

		if (real_upgrade_times <= 0)
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return false;
		}
		else if (real_upgrade_times >= need_upgrade_times)
		{
			real_upgrade_times = need_upgrade_times;
		}
	}

	// 消耗
	if (!m_role->GetKnapsack()->ConsumeItem(spirit_cfg->consume_item_id, real_upgrade_times * spirit_cfg->consume_item_count, "HuaShen::OnUpgradeSpirit"))
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return false;
	}

	// 给予
	spirit_info.exp_val += real_upgrade_times * spirit_cfg->add_exp_val;
	if (spirit_info.exp_val >= spirit_cfg->need_exp_val)
	{
		++ spirit_info.level;
		spirit_info.exp_val -= spirit_cfg->need_exp_val;

		m_role->NoticeNum(noticenum::NT_HUASHEN_SPIRIT_UPGRADE_SUCC);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HUASHEN);
	}

	this->SendHuaShenSpiritInfo(huashen_id);

	// 日志
	gamelog::g_log_huashen.printf(LL_INFO, "HuaShen::OnUpgradeSpirit user[%d %s], huashen_id[%d], spirit_id[%d], spirit_level[%d], spirit_exp_val[%d], is_auto_upgrade[%d], upgrade_times[%d]", 
		m_role->GetUID(), m_role->GetName(), huashen_id, spirit_id, spirit_info.level, spirit_info.exp_val, (is_auto_upgrade ? 1 : 0), real_upgrade_times);

	return true;
}

bool HuaShen::IsHuaShenIDActive(int id)
{
	if (id < 0 || id > LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenMaxID() || id > HUASHEN_MAX_ID_LIMIT)
	{
		return false;
	}

	return (0 != (m_param.active_flag & (1 << id)));  
}

int HuaShen::GetLevel(int id)
{
	if (id < 0 || id > LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenMaxID() || id > HUASHEN_MAX_ID_LIMIT)
	{
		return -1;
	}

	return m_param.level_info_list[id].level;
}

int HuaShen::GetSpiritLevel(int huashen_id, int spirit_id)
{
	if (huashen_id < 0 || huashen_id > HUASHEN_MAX_ID_LIMIT || spirit_id < 0 || spirit_id > HUASHEN_SPIRIT_MAX_ID_LIMIT)
	{
		return -1;
	}

	return m_param.spirit_info_list[huashen_id].spirit_list[spirit_id].level;
}