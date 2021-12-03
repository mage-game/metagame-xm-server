

#include "greatesoldier.hpp"
#include "greatesoldierconfig.hpp"
#include "gamecommon.h"
#include "protocal/msggreatesoldier.h"
#include "config/logicconfigmanager.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "gameworld/other/event/eventhandler.hpp"
#include "other/chinesezodiac/chinesezodiacconfig.hpp"
#include "servercommon/errornum.h"
#include "gstr.h"
#include "servercommon/string/gameworldstr.h"
#include "other/capability/capability.hpp"
#include "item/knapsack.h"
#include "other/specialappearance/specialappearance.hpp"
#include "other/chestshop/rolechestshop.hpp"
#include "servercommon/msgcommonstruct.h"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "config/joinlimitconfig.hpp"
#include "task/taskrecorder.h"
#include "servercommon/serverconfig/crossconfig.hpp"

void GreateSoldier::InitParam(const GreateSoldierParam &param)
{
	m_param = param;

	m_skill.Init(m_role, this);
	m_goal.Init(m_role, &m_param.goal_data);

	this->CheckCombineFlag();
}


void GreateSoldier::OnRoleLogin(unsigned int last_logout_time)
{
	unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();

	// ���������ʱ��
	{
		bool force_end_bianshen = (m_role->IsInCross() && this->IsBianshenStatus()); // �������״̬�½�������ǿ����ֹ

		UInt32 &bianshen_end_time = m_param.other_param.bianshen_end_timestamp;
		if (force_end_bianshen || (bianshen_end_time > 0 && now_second >= bianshen_end_time))
		{
			bianshen_end_time = 0;
			m_param.other_param.cur_used_seq = -1;
			// ������������ñ�����ȴʱ��
			m_param.other_param.bianshen_cd = GREATE_SOLDIER_OTHER_CFG().bianshen_cd_s * 1000;

			m_skill.CheckActiveSkill();  // �����������⼼��
		}
	}

	// ������CD
	{
		unsigned int offline_time_s = now_second - last_logout_time;

		unsigned int cd = m_param.other_param.bianshen_cd;
		if (cd < offline_time_s * 1000)
		{
			cd = 0;
		}
		else
		{
			cd -= offline_time_s * 1000;
		}

		m_param.other_param.bianshen_cd = (int)cd;
	}

	this->CheckFuncOpen();
}


void GreateSoldier::OnRoleLogout()
{
	// ��������������״̬
	if (m_param.other_param.is_on_bianshen_trail)
	{
		this->SetBianshenTrail(0);
	}
}

void GreateSoldier::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		m_other_capability = 0;

		//int min_xinghun_level = 0;

		// ������������
		for (int i = 0; i < ARRAY_ITEM_COUNT(m_param.item_param.item_list); ++i)
		{
			GreateSoldierItemInfo* item_info = &m_param.item_param.item_list[i];
			const GreateSoldierLevelConfig::CfgItem* level_cfg = GREATE_SOLDIER_LEVEL_CFG(i);

			if (!item_info->HasActive() || !level_cfg)
			{
				continue;
			}

			// �ȼ�����
			{
				int per_attr = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierLevelAttrPer(item_info->level);
				double factor = (double)((double)per_attr * ROLE_ATTR_PER);
				level_cfg->attrs_cfg.AddAttrs(m_attrs_add, factor);
			}

			// Ǳ������
			{
				double rate = 1.0;
				/*for (int j = 0; j < GREATE_SOLDIER_SLOT_MAX_COUNT; j++)   // ���ڽ�λֻ������λ����ս�á�û�����Լӳ�
				{
					const auto &slot_item = m_param.slot_param.slot_item_list[j];
					if (slot_item.IsFree() || slot_item.item_seq != i)
					{
						continue;
					}

					auto slot_cfg = GREATE_SOLDIER_SLOT_CFG(j, slot_item.level);
					if (slot_cfg)
					{
						rate += slot_cfg->wash_attr_add_percent / 100.0;
						break;	// �ҵ���λ���˳�Ѱ��
					}
				}*/
				
				// ��֤Ǳ������Ϊ1������������
				static CharIntAttrs potential_attrs; potential_attrs.Reset();  // ��֤���ȣ���ʱ�Ȼ�������
				for (int level = 1; level <= item_info->potential_level; level++)
				{
					// ���ñ�ȼ���1��ʼ
					auto potential_cfg = GREATE_SOLDIER_POTENTIAL_CFG(i, level - 1);
					if (potential_cfg)
					{
						//potential_cfg->attrs_cfg.AddAttrs(m_attrs_add, rate);
						potential_cfg->attrs_cfg.AddAttrs(potential_attrs);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += potential_cfg->gongji_strength;
					}
				}
				for (int i = 0; i < CharIntAttrs::CHARINTATTR_TYPE_MAX; i++)
				{
					m_attrs_add.AddValue(i, static_cast<Attribute>(potential_attrs.m_attrs[i] * rate));  // ��Ǳ�����Լӽ���
				}
			}

			// ���ǣ��ǻ꣩���ԡ� �߻�˵ȡ���ˣ���ʱû��
			/*{
				int xinghun_level = m_param.xinghun_level_list[i];

				if (m_param.xinghun_level_list[i] >= 0)
				{
					min_xinghun_level += xinghun_level;

					//��Ф�ǻ�����
					const ChineseZodiacXingHunCfg *xinghun_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetChineseZodiacXingHunCfg(i, xinghun_level);
					if (NULL != xinghun_cfg)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += xinghun_cfg->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += xinghun_cfg->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += xinghun_cfg->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += xinghun_cfg->baoji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += xinghun_cfg->jianren;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += xinghun_cfg->mingzhong;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += xinghun_cfg->shanbi;
					}
				}
			}*/

			// װ������
			CharIntAttrs equip_base_attr;
			for (int equip_index = 0; equip_index < GREATE_SOLDIER_EQUIPMENT_SLOT_MAX_COUNT; equip_index++)
			{
				GreateSoldierEquipmentInfo &equip_info = item_info->equipment_list[equip_index];
				if (equip_info.item_id > 0)
				{
					const Equipment * equip = dynamic_cast<const Equipment*>(ITEMPOOL->GetItem(equip_info.item_id));
					if (nullptr == equip)
					{
						continue;
					}

					int equip_type = equip->GetEquipType() - Equipment::E_TYPE_GREATESOLDIER_MIN;
					if (equip_type != equip_index)
					{
						continue;
					}

					// ����װ����������
					equip_base_attr.Reset();
					equip->GetBaseAttr(equip_base_attr);
					m_attrs_add.AddAttrs(&equip_base_attr);
					
					// ����װ���ȼ�����
					auto cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierEquipLevelConfig(equip_index, equip->GetColor(), equip_info.strength_level);
					if (cfg)
					{
						cfg->attr_cfg.AddAttrs(m_attrs_add);
					}
				}
			}
		}

		// ��Ф�ǻ������ԡ� �߻�˵ȡ���ˣ���ʱû��
		/*const ChineseZodiacXingHunExtraAttrCfg *xinghun_attr_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetChineseZodiacXingHunAttrCfg(min_xinghun_level);
		if (NULL != xinghun_attr_cfg)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += xinghun_attr_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += xinghun_attr_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += xinghun_attr_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += xinghun_attr_cfg->baoji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += xinghun_attr_cfg->jianren;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += xinghun_attr_cfg->mingzhong;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += xinghun_attr_cfg->shanbi;
		}*/

		// ��λ���ԡ��߻�˵ȡ���ˣ���ʱû��
		/*{
			for (int j = 0; j < GREATE_SOLDIER_SLOT_MAX_COUNT; j++)
			{
				const auto &slot_item = m_param.slot_param.slot_item_list[j];
				if (!slot_item.IsFree())
				{
					auto slot_cfg = GREATE_SOLDIER_SLOT_CFG(j, slot_item.level);
					if (slot_cfg)
					{
						m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, slot_cfg->attr_cfg.gongji);
						m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, slot_cfg->attr_cfg.fangyu);
						m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, slot_cfg->attr_cfg.maxhp);
						m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, slot_cfg->attr_cfg.baoji);
					}
				}
			}
		}*/

		// ������ԡ� �߻�˵ȡ���ˣ���ʱû��
		/*for (int i = 0; i < GREATE_SOLDIER_COMBINE_MAX_COUNT; ++i)
		{
			if (!IS_BIT_SET(m_combine_flag, i)) continue;

			int level = this->GetCombineMinLevel(i);
			if (level == TWO_BILLION)
			{
				continue;
			}

			auto combine_attr_cfg = GREATE_SOLDIER_COMBINE_ATTR_CFG(i, level);
			if (combine_attr_cfg)
			{
				combine_attr_cfg->attrs_cfg.AddAttrs(m_attrs_add);
				if (combine_attr_cfg->all_percent > 0)
				{
					m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP]	+= (float)(combine_attr_cfg->all_percent * ROLE_ATTR_PER);
					m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (float)(combine_attr_cfg->all_percent * ROLE_ATTR_PER);
					m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (float)(combine_attr_cfg->all_percent * ROLE_ATTR_PER);
				}
				
			}
		}*/

		// �û����ԡ� �߻��������ﲻ�ټӣ��û�������Ŀǰ��ֱ�Ӽ���W3��СĿ������
		/*for (int i = 0; i < int(sizeof(m_param.other_param.huanhua_flag) * CHAR_BIT); i++)
		{
			if (IS_BIT_SET(m_param.other_param.huanhua_flag, i))
			{
				const GreateSoldierHuanhuaConfig* cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierHuanhuaConfig(i);
				if (cfg)
				{
					cfg->attr_cfg.AddAttrs(m_attrs_add);
				}
			}
		}*/

		// ��������ս��
		if (m_param.other_param.is_on_bianshen_trail)
		{
			m_other_capability += GREATE_SOLDIER_OTHER_CFG().bianshen_trail_capability;
		}

		// Ŀ�����Լӳɡ�����W3�Լ��Ĵ�СĿ�꣬�����СĿ����ʱû����
		/*{
			int add_per = 0;
			for (int goal_type = 0; goal_type < GreateSoldierGoalData::GOAL_TYPE_MAX; ++goal_type)
			{
				const GreateSoldierGoalConfig *cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierGoalConfig(goal_type);
				if (!cfg) continue;

				GreateSoldierGoalData::DataItem *data = &m_param.goal_data.data_list[goal_type];
				if (data->is_finish)
				{
					add_per += cfg->add_base_attr_per;
				}
			}

			if (add_per > 0)
			{
				for (int i = 0; i < CharIntAttrs::CHARINTATTR_TYPE_MAX; i++)
				{
					m_attrs_add.m_attrs[i] += static_cast<Attribute>(m_attrs_add.m_attrs[i] * 0.01f * add_per);
				}
			}
		}*/

		// ��СĿ��̶����Լӳ�
		m_role->GetRoleBigSmallGoal()->RoleBigSmallGoalAddSpecialAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_GREATE_SOLDIER, m_attrs_add);
		//��СĿ����ֱȼӳ�
		this->ReCalcAttrBigSmallGoalPer(m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_GREATE_SOLDIER, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_GREATE_SOLDIER, m_other_capability);
}

void GreateSoldier::ReCalcAttrBigSmallGoalPer(CharIntAttrs & attr_add)
{
	CharIntAttrs greate_soldier_attr;
	//����������ħ������
	for (int i = 0; i < ARRAY_ITEM_COUNT(m_param.item_param.item_list); ++i)
	{
		GreateSoldierItemInfo* item_info = &m_param.item_param.item_list[i];
		const GreateSoldierLevelConfig::CfgItem* level_cfg = GREATE_SOLDIER_LEVEL_CFG(i);

		if (!item_info->HasActive() || !level_cfg)
		{
			continue;
		}

		// �ȼ�����
		{
			int per_attr = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierLevelAttrPer(item_info->level);
			double factor = (double)((double)per_attr * ROLE_ATTR_PER);
			level_cfg->attrs_cfg.AddAttrs(greate_soldier_attr, factor);
		}
	}
	//��СĿ����ֱȼӳ�
	int add_value = m_role->GetRoleBigSmallGoal()->GetRoleBigSmallGoalAddAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_GREATE_SOLDIER);
	//�����ṩ������ = ǿ�������� * �ӳ�
	if (add_value > 0)
	{
		greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] = static_cast<Attribute>(greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (add_value * ROLE_ATTR_PER));
		greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] = static_cast<Attribute>(greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (add_value * ROLE_ATTR_PER));
		greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] = static_cast<Attribute>(greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (add_value * ROLE_ATTR_PER));
		greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] = static_cast<Attribute>(greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (add_value * ROLE_ATTR_PER));
		greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] = static_cast<Attribute>(greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (add_value * ROLE_ATTR_PER));
		greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] = static_cast<Attribute>(greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (add_value * ROLE_ATTR_PER));
		greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] = static_cast<Attribute>(greate_soldier_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (add_value * ROLE_ATTR_PER));
		
		//�ӵ�ϵͳ������
		attr_add.Add(greate_soldier_attr.m_attrs);
	}
}

void GreateSoldier::Update(time_t now_second, unsigned long interval)
{
	// ������CD
	int &bianshen_cd = m_param.other_param.bianshen_cd;
	if (bianshen_cd > 0)
	{
		bianshen_cd -= (int)interval;
		if (bianshen_cd < 0)
		{
			bianshen_cd = 0;
		}

		// CD����֪ͨ�ͻ��ˣ�����Ӱ��ͻ��˹һ���W3��ɧ����
		if (bianshen_cd <= 0)
		{
			this->SendOtherInfo(true);
		}
	}

	// ���������ʱ��
	UInt32 &bianshen_end_time = m_param.other_param.bianshen_end_timestamp;
	if (bianshen_end_time > 0 && now_second >= bianshen_end_time)
	{
		bianshen_end_time = 0;
		m_param.other_param.cur_used_seq = -1;
		m_param.other_param.bianshen_cd = GREATE_SOLDIER_OTHER_CFG().bianshen_cd_s * 1000;

		m_skill.CheckActiveSkill();  // �����������⼼��

		this->SendOtherInfo(false);
	}

	// ���鵽��
	{
		bool need_recalc = false;
		for (int seq = 0; seq < GREATE_SOLDIER_MAX_COUNT; ++seq)
		{
			UInt32 &unactive_timestamp = m_param.item_param.item_list[seq].unactive_timestamp;

			if (unactive_timestamp > 0 && now_second >= unactive_timestamp)
			{
				this->UnActive(seq);

				unactive_timestamp = 0;
				need_recalc = true;
			}
		}
		if (need_recalc)
		{
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);
		}
	}

	// �����߼�
	m_skill.Update(now_second, interval);
}

void GreateSoldier::OnDayChange(UInt32 old_dayid, UInt32 now_dayid)
{
	if (old_dayid == now_dayid)
	{
		return;
	}
	this->CheckFuncOpen();
	m_param.other_param.has_dailyfirst_draw_ten = 0;

	{
		this->CheckFetchLimitDrawReward();
	}

	memset(m_param.seq_exchange_counts, 0, sizeof(m_param.seq_exchange_counts));

	this->SendOtherInfo(false);
}

void GreateSoldier::OnRoleLevelChange(int level)
{
	this->CheckFuncOpen();
}

void GreateSoldier::OnCompleteTask(TaskID task_id)
{
	this->CheckFuncOpen();
}

const int GreateSoldier::GetSlotLevel(const int slot_seq)
{
	if (!GREATE_SOLDIER_CFG().IsValidSlotSeq(slot_seq))
	{
		return 0;
	}

	GreateSoldierSlotItemInfo* slot_info = &m_param.slot_param.slot_item_list[slot_seq];

	return slot_info->level;
}

const int GreateSoldier::GetPotentialLevelBySeq(const int seq)
{
	if (!GREATE_SOLDIER_CFG().IsValidSeq(seq))
	{
		return 0;
	}

	return m_param.item_param.item_list[seq].potential_level;
}

bool GreateSoldier::OnTiyanActive(int seq, int tiyan_time_sec)
{
	return false; // ���ι���
	
	if (!GREATE_SOLDIER_CFG().IsValidSeq(seq))
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return false;
	}

	if (tiyan_time_sec <= 0)
	{
		return false;
	}

	GreateSoldierItemInfo* item_info = &m_param.item_param.item_list[seq];
	// �����ѱ�����
	if (item_info->level > 0)
	{
		return false;
	}

	auto level_cfg = GREATE_SOLDIER_LEVEL_CFG(seq);
	if (!level_cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return false;
	}

	++item_info->level;
	item_info->unactive_timestamp = (UInt32)EngineAdapter::Instance().Time() + tiyan_time_sec;
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);

	this->SendItemInfo(seq);

	return true;
}

bool GreateSoldier::IsActive(int seq)
{
	if (!GREATE_SOLDIER_CFG().IsValidSeq(seq))
	{
		return false;
	}

	GreateSoldierItemInfo* item_info = &m_param.item_param.item_list[seq];

	return item_info->HasActive();
}

// Ŀǰֻ�����鵽�ڲŻ�ʧЧ�� ��ʽ���������������Ч
void GreateSoldier::UnActive(int seq)
{
	if (!GREATE_SOLDIER_CFG().IsValidSeq(seq))
	{
		return;
	}

	GreateSoldierItemInfo* item_info = &m_param.item_param.item_list[seq];
	item_info->level = 0;

	// ж�½�λ
	for (auto &slot : m_param.slot_param.slot_item_list)
	{
		if (slot.item_seq == seq)
		{
			slot.item_seq = -1;
			break;
		}
	}

	this->SendItemInfo(seq);
	this->SendSlotInfo();

	return;
}

void GreateSoldier::SetBianshenTrail(int is_on)
{
	return; // ���ι���

	{
		// ���ñ������
		m_role->GetSkillManager()->RemoveSkill(GREATE_SOLDIER_SKILL_1_ID, false);
		m_role->GetSkillManager()->RemoveSkill(GREATE_SOLDIER_SKILL_2_ID, false);
		m_role->GetSkillManager()->RemoveSkill(GREATE_SOLDIER_SKILL_3_ID, false);

		if (is_on)
		{
			int skill_level = 1;
			auto level_cfg = GREATE_SOLDIER_LEVEL_CFG(m_param.other_param.cur_used_seq);
			if (nullptr != level_cfg)
			{
				skill_level = level_cfg->active_skill_level;

				if (this->GetHuanhuaID() > 0)  // �û���ʹ�ûû��ļ��ܵȼ�
				{
					const GreateSoldierHuanhuaConfig* huanhua_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierHuanhuaConfig(this->GetHuanhuaID());
					if (huanhua_cfg)
					{
						skill_level = huanhua_cfg->active_skill_level;
					}
				}
			}

			m_role->GetSkillManager()->AddSkill(GREATE_SOLDIER_SKILL_1_ID, skill_level, nullptr, false, false);
			m_role->GetSkillManager()->AddSkill(GREATE_SOLDIER_SKILL_2_ID, skill_level, nullptr, false, false);
			m_role->GetSkillManager()->AddSkill(GREATE_SOLDIER_SKILL_3_ID, skill_level, nullptr, false, false);
		}

		m_role->GetSkillManager()->SendSkillListInfo();
	}

	char old_status = m_param.other_param.is_on_bianshen_trail;
	m_param.other_param.is_on_bianshen_trail = (char)is_on;

	if (old_status != m_param.other_param.is_on_bianshen_trail)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);
	}
}

void GreateSoldier::GmClearBianshenCD()
{
	m_param.other_param.bianshen_cd = 0;
	this->SendOtherInfo(true);
}

int GreateSoldier::GetAchieveXinghunlevelNum(int level)
{
	int ret = 0;
	for (int soldier_idx = 0; soldier_idx < GREATE_SOLDIER_MAX_COUNT; soldier_idx++)
	{
		if (m_param.xinghun_level_list[soldier_idx] >= level)
		{
			ret++;
		}
	}

	return ret;
}

int GreateSoldier::GetXinghunSumLevel()
{
	int ret = 0;
	for (int soldier_idx = 0; soldier_idx < GREATE_SOLDIER_MAX_COUNT; soldier_idx++)
	{
		ret += m_param.xinghun_level_list[soldier_idx];
	}

	return ret;
}

int GreateSoldier::GetActiveCount(int color)
{
	int count = 0;
	for (unsigned int soldier_idx = 0; soldier_idx < static_array_size(m_param.item_param.item_list); soldier_idx++)
	{
		auto level_cfg = GREATE_SOLDIER_LEVEL_CFG(soldier_idx);
		if (level_cfg)
		{
			if (this->IsActive(soldier_idx) && level_cfg->color >= color)
			{
				count++;
			}
		}
	}

	return count;
}

int GreateSoldier::GetLevelCount(int level)
{
	int count = 0;
	for (unsigned int soldier_idx = 0; soldier_idx < static_array_size(m_param.item_param.item_list); soldier_idx++)
	{
		auto level_cfg = GREATE_SOLDIER_LEVEL_CFG(soldier_idx);
		if (level_cfg)
		{
			if (this->IsActive(soldier_idx))
			{
				int cur_potential_level = m_param.item_param.item_list[soldier_idx].potential_level;
				if (cur_potential_level >= level)
				{
					count++;
				}
			}
		}
	}

	return count;
}

int GreateSoldier::OnUseHuanhuaCard(int id, bool is_notify /* = true */)
{
	// ��飺���������ã��Ƿ��Ѽ���
	if (id <= 0 || id >= int(sizeof(m_param.other_param.huanhua_flag) * CHAR_BIT))
	{
		if (is_notify) m_role->NoticeNum(errornum::EN_PARAMETER_ERROR);
		return -1;
	}

	const GreateSoldierHuanhuaConfig* cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierHuanhuaConfig(id);
	if (!cfg)
	{
		if (is_notify) m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return -2;
	}

	// ���ټ�����һ������Ʒ�ʵ���ħ
	if (this->GetActiveCount(0) < 1)
	{
		if (is_notify) m_role->NoticeNum(errornum::EN_GREATESOLDIER_NOT_ALIVE);
		return -3;
	}

	if (IS_BIT_SET(m_param.other_param.huanhua_flag, id))
	{
		if (is_notify) m_role->NoticeNum(errornum::EN_GREATE_SOLDIER_HUANHUA_CARD_ALREADY_ACTIVED);
		return -4;
	}

	// ���� ��

	// ���裬���ñ��
	SET_BIT(m_param.other_param.huanhua_flag, id);

	// ���裬ʹ������
	m_param.other_param.use_huanhua_id = id;

	// �����СĿ������
	m_role->GetRoleBigSmallGoal()->ActiveRoleBigSmallGoalSpecialAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_GREATE_SOLDIER, id);
	//m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);  // ��СĿ�������м������ԣ����ﲻ���ظ�����

	// ���� & Log
	this->SendOtherInfo(false);
	
	gamelog::g_log_greate_soldier.printf(LL_INFO, "GreateSoldier::OnUseHuanhuaCard Succ! user[%d %s], id: %d, huanhua_flag: %u\n",
		m_role->GetUID(), m_role->GetName(), id, m_param.other_param.huanhua_flag);

	return 0;
}

void GreateSoldier::OnUseHuanhuaID(int id)
{
	// ��飺�������Ƿ��Ѽ���
	// ���Է�0,0����ȡ��
	if (id < 0 || id >= int(sizeof(m_param.other_param.huanhua_flag) * CHAR_BIT))
	{
		m_role->NoticeNum(errornum::EN_PARAMETER_ERROR);
		return;
	}

	if (id != 0 && !IS_BIT_SET(m_param.other_param.huanhua_flag, id))
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return;
	}

	if (id == m_param.other_param.use_huanhua_id)
	{
		return;
	}

	// ���� ��

	// ���裬ʹ������
	m_param.other_param.use_huanhua_id = id;

	// ����
	this->SendOtherInfo(false);
}

bool GreateSoldier::OnLevelUp(int seq)
{
	/*
	 * ��������
	 * ��0 -> 1��Ϊ����
	 */

	const char *func_name = "GreateSoldier::OnLevelUp";

	if (!GREATE_SOLDIER_CFG().IsValidSeq(seq))
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return false;
	}

	GreateSoldierItemInfo* item_info = &m_param.item_param.item_list[seq];
	if (item_info->level >= GREATE_SOLDIER_OTHER_CFG().max_level)
	{
		m_role->NoticeNum(errornum::EN_LEVEL_LIMIT);
		return false;
	}

	auto level_cfg = GREATE_SOLDIER_LEVEL_CFG(seq);
	if (!level_cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return false;
	}

	// ����
	if (!m_role->GetKnapsack()->ConsumeItem(level_cfg->item_id, 1, func_name))
	{
		m_role->NoticeItemLack(level_cfg->item_id, 1);
		return false;
	}

	// ����
	short old_level = item_info->level;
	++item_info->level;

	if (0 == old_level)
	{
		this->CheckCombineFlag();

		m_goal.OnActive(seq);
		this->OnPuton(seq, 0);

		EventHandler::Instance().OnActiveGreateSoldier(m_role, seq);

		if (level_cfg->is_broadcast)
		{
			// dsds
			int sendlen = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "greate_soldier_active",
				m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), level_cfg->name.c_str());
			if (sendlen > 0)
			{
				SCENE_MGR->SystemMsgThread(gstr::GSTR_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_GREATE_SOLDIER, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_3, seq);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);

	item_info->unactive_timestamp = 0;

	this->SendItemInfo(seq);

	gamelog::g_log_greate_soldier.printf(LL_INFO, "%s, role_id:(%d, %s), greate_soldier level: %d -> %d", func_name, m_role->GetUID(), m_role->GetName(),
		old_level, item_info->level);

	return true;
}

void GreateSoldier::OnBianshen()
{
	/*
	 * ����
	 * ֻ�ܱ�������λ
	 * ��ǰ���ڱ����޷�����
	 * ������cd��cdδ�����ܱ���
	 * ������ʱ������
	 * �ڱ���ʱ���ڣ�ӵ�����������󣬿�ʹ���������������ܣ��ܴ��������ı�������
	 */

	const char *func_name = "GreateSoldier::OnBianshen";

	const GreateSoldierSlotItemInfo &slot_item = m_param.slot_param.slot_item_list[0];

	int seq = slot_item.item_seq;
	if (!GREATE_SOLDIER_CFG().IsValidSeq(seq))
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return;
	}

	auto level_cfg = GREATE_SOLDIER_LEVEL_CFG(seq);
	if (!level_cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	GreateSoldierItemInfo* item_info = &m_param.item_param.item_list[seq];
	if (!item_info->HasActive())
	{
		gstr::SendError(m_role, "greate_soldier_not_active", level_cfg->name.c_str());
		return;
	}

	if (this->IsBianshenStatus())  // �������ʾ�����룬�Ϳͻ��˹һ���Ӱ��
	{
		return;
	}

	if (m_role->GetEffectList()->GetBianshenEffect() || m_role->IsTaskAppearnStatus())
	{
		m_role->NoticeNum(errornum::EN_BIANSHEN_LIMIT);
		return;
	}

	auto real_cd = m_param.other_param.bianshen_cd - m_bianshen_cd_reduce_s * 1000;
	if (real_cd < 0)
	{
		real_cd = 0;
	}

	if (real_cd > 0)
	{
		gstr::SendError(m_role, "greate_soldier_bianshen_cd");
		return;
	}

	// ����
	m_param.other_param.cur_used_seq = seq;

	m_param.other_param.bianshen_end_timestamp = (UInt32)EngineAdapter::Instance().Time() + GREATE_SOLDIER_OTHER_CFG().bianshen_duration_s;

	Attribute old_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	m_skill.CheckActiveSkill();  // ����ʼ����⼼��
	Attribute new_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);

	// ���ϼӳɵ�Ѫ��
	if (new_hp > old_hp && m_role->IsAlive())
	{
		Attribute change_val = new_hp - old_hp;
		m_role->ChangeHp(&change_val, true);
	}

	{
		int image_id = level_cfg->image_id;
		if (m_param.other_param.use_huanhua_id > 0)
		{
			const GreateSoldierHuanhuaConfig* huanhua_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierHuanhuaConfig(m_param.other_param.use_huanhua_id);
			if (huanhua_cfg)
			{
				image_id = huanhua_cfg->res_id;
			}
		}

		UInt32 end_time = m_param.other_param.bianshen_end_timestamp;
		bool is_broadcast = true;
		m_role->GetSpecialAppearance()->SetGreateSoldierAppearance(image_id, end_time, is_broadcast);
	}

	this->ClearBianshenCDReduce(m_fabao_skill_id);

	// ��������������ص�
	//m_role->GetSkillManager()->OnGreateSoldierBianshen();

	this->SendOtherInfo(false);

	gamelog::g_log_greate_soldier.printf(LL_INFO, "%s, role_id: %d, role_name: %s, seq: %d", func_name, m_role->GetUID(), m_role->GetName(),
		seq);
}

void GreateSoldier::OnBianshenTrial(int seq)
{
	return; // ���ι���

	if (!GREATE_SOLDIER_CFG().IsValidSeq(seq))
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return;
	}

	if (this->IsBianshenStatus())  // �������ʾ�����룬�Ϳͻ��˹һ���Ӱ��
	{
		return;
	}

	if (m_role->GetEffectList()->GetBianshenEffect() || m_param.other_param.is_on_bianshen_trail || m_role->IsTaskAppearnStatus())
	{
		m_role->NoticeNum(errornum::EN_BIANSHEN_LIMIT);
		return;
	}

	this->SetBianshenTrail(1);

	{
		auto level_cfg = GREATE_SOLDIER_LEVEL_CFG(seq);
		if (level_cfg)
		{
			int image_id = level_cfg->image_id;
			UInt32 end_time = GREATE_SOLDIER_OTHER_CFG().bianshen_trail_s + (UInt32)EngineAdapter::Instance().Time();
			bool is_broadcast = false;
			m_role->GetSpecialAppearance()->SetGreateSoldierAppearance(image_id, end_time, is_broadcast);
		}
	}
}

void GreateSoldier::OnPuton(int item_seq, int slot_seq)
{
	/*
	 * װ�ϲ�λ
	 * ��Ҫ�ﵽһ���ȼ�
	 * ���������Ѿ�����
	 * ���װ�Ĳ�������λ��0������ô����Ҫ������λ�Ѿ�װ��������
	 * ����ò�λ�ǿգ����滻��
	 * ��λ���Լӳɵ���ɫ����
	 */

	 /*
	 * comment by 2019-03-01
	 * ����������Ȼ���ã����ǵ�ǰֻ��Ϊ��սʹ��
	 * �����ս��Ϊ����װ������λ�� Ŀǰ�ͻ���ֻ��slot_seq = 0
	 */

	const char *func_name = "GreateSoldier::OnPuton";

	if (!GREATE_SOLDIER_CFG().IsValidSeq(item_seq) || !GREATE_SOLDIER_CFG().IsValidSlotSeq(slot_seq))
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return;
	}

	auto slot_other_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSlotOtherCfg(slot_seq);
	if (!slot_other_cfg || slot_other_cfg->need_level > m_role->GetLevel())
	{
		gstr::SendError(m_role, "greate_soldier_slot_limit_2");
		return;
	}

	auto level_cfg = GREATE_SOLDIER_LEVEL_CFG(item_seq);
	if (!level_cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	GreateSoldierItemInfo* item_info = &m_param.item_param.item_list[item_seq];
	if (!item_info->HasActive())
	{
		gstr::SendError(m_role, "greate_soldier_not_active", level_cfg->name.c_str());
		return;
	}

	// ���Ҫװ����λ���Ѿ�����ͬ����������������
	for (int i = 0; i < GREATE_SOLDIER_SLOT_MAX_COUNT; ++i)
	{
		if (i == slot_seq && m_param.slot_param.slot_item_list[i].item_seq == item_seq)
		{
			return;
		}
	}

	GreateSoldierSlotItemInfo* main_slot_item = &m_param.slot_param.slot_item_list[0];

	if (0 != slot_seq && main_slot_item->IsFree())
	{
		gstr::SendError(m_role, "greate_soldier_slot_limit");
		return;
	}

	// ����ж������λ
	if (!main_slot_item->IsFree() && item_seq == main_slot_item->item_seq)
	{
		gstr::SendError(m_role, "greate_soldier_cant_putoff_mainslot");
		return;
	}

	// ��ǰ��װ����������ж��
	for (auto &slot : m_param.slot_param.slot_item_list)
	{
		if (slot.item_seq == item_seq)
		{
			slot.item_seq = -1;
			break;
		}
	}

	// ��ǰ��λ��װ��������ж��
	GreateSoldierSlotItemInfo* slot_info = &m_param.slot_param.slot_item_list[slot_seq];
	if (!slot_info->IsFree())
	{
		slot_info->item_seq = -1;
	}

	// װ��
	slot_info->item_seq = item_seq;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);

	this->SendSlotInfo();

	gamelog::g_log_greate_soldier.printf(LL_INFO, "%s, role_id: %d, role_name: %s, item_seq: %d, slot_seq: %d",
		func_name, m_role->GetUID(), m_role->GetName(), item_seq, slot_seq);
}

void GreateSoldier::OnPutoff(int slot_seq)
{
	/*
	 * ж������
	 * �ò�λ����������
	 * ����λ����ж��
	 */

	const char *func_name = "GreateSoldier::OnPutoff";

	if (!GREATE_SOLDIER_CFG().IsValidSlotSeq(slot_seq))
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	if (0 == slot_seq)
	{
		gstr::SendError(m_role, "greate_soldier_cant_putoff_mainslot");
		return;
	}

	GreateSoldierSlotItemInfo* slot_info = &m_param.slot_param.slot_item_list[slot_seq];
	if (slot_info->IsFree())
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return;
	}

	// ж��
	slot_info->item_seq = -1;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);

	this->SendSlotInfo();

	gamelog::g_log_greate_soldier.printf(LL_INFO, "%s, role_id: %d, role_name: %s, slot_seq: %d",
		func_name, m_role->GetUID(), m_role->GetName(), slot_seq);
}

bool GreateSoldier::OnSlotLevelUp(int slot_seq)
{
	return false;   // ���β�λ����
	/*
	 * ������λ
	 * ��λ�ȼ���1����ʼ
	 * ���ף��ֵ����
	 */

	const char *func_name = "GreateSoldier::OnSlotLevelUp";

	if (!GREATE_SOLDIER_CFG().IsValidSlotSeq(slot_seq))
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return false;
	}

	GreateSoldierSlotItemInfo* slot_info = &m_param.slot_param.slot_item_list[slot_seq];

	auto next_slot_cfg = GREATE_SOLDIER_SLOT_CFG(slot_seq, slot_info->level + 1);
	if (!next_slot_cfg)
	{
		gstr::SendError(m_role, "greate_soldier_slot_already_max_level");
		return false;
	}

	auto slot_cfg = GREATE_SOLDIER_SLOT_CFG(slot_seq, slot_info->level);
	if (!slot_cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return false;
	}

	// ����
	if (!m_role->GetKnapsack()->ConsumeItem(slot_cfg->item_id, slot_cfg->item_num, func_name))
	{
		//m_role->NoticeItemLack(slot_cfg->item_id, slot_cfg->item_num);
		return false;
	}

	// ��ף��ֵ and ����
	int old_level_val = slot_info->level_val;
	short old_level = slot_info->level;
	bool is_level_up = false;
	
	int add_val = slot_cfg->item_add_cfg.Random();
	slot_info->level_val += add_val;

	if (slot_info->level_val >= slot_cfg->need_val)
	{
		slot_info->level_val -= slot_cfg->need_val;
		if (slot_info->level_val >= next_slot_cfg->need_val)
		{
			slot_info->level_val = next_slot_cfg->need_val - 1; // ���ⱬ��������
		}

		is_level_up = true;
	}

	if (is_level_up)
	{
		++slot_info->level;
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);
		gstr::SendNotice(m_role, "greate_soldier_slot_uplevel_succ");
	}

	this->SendSlotInfo();

	gamelog::g_log_greate_soldier.printf(LL_INFO, "%s, role_id: %d, role_name: %s, slot_seq: %d, level_val: %d -> %d, level: %d -> %d",
		func_name, m_role->GetUID(), m_role->GetName(), slot_seq,
		old_level_val, slot_info->level_val, old_level, slot_info->level);

	return true;
}

void GreateSoldier::OnDraw(int draw_type, bool is_auto_buy)
{
	/*
	* �齱
	* ���Ե���10�顢50��
	* ����10�顢50�飬��һ��������齱����������Ʒ
	* �齱������Ʒ
	* �齱��������Ѱ���ֿ�
	*/

	// ��ǰ�����жϳ齱��Ƿ���
	/*if (!RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_GREATE_SOLDIER_DRAW))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}*/

	const char *func_name = "GreateSoldier::OnDraw";

	const ConsumeItemConfig *consume_item = nullptr;
	int give_num = 1;

	if (GREATE_SOLDIER_DRAW_TYPE_1_DRAW == draw_type)
	{
		consume_item = &GREATE_SOLDIER_OTHER_CFG().draw_1_consume_item;
	}
	else if (GREATE_SOLDIER_DRAW_TYPE_10_DRAW == draw_type)
	{
		if (m_param.other_param.has_dailyfirst_draw_ten > 0)
		{
			consume_item = &GREATE_SOLDIER_OTHER_CFG().draw_10_consume_item;
		}
		else
		{
			consume_item = &GREATE_SOLDIER_OTHER_CFG().dailyfirst_draw_10_consume_item;
		}

		give_num = 10;
	}
	else if (GREATE_SOLDIER_DRAW_TYPE_50_DRAW == draw_type)
	{
		consume_item = &GREATE_SOLDIER_OTHER_CFG().draw_50_consume_item;
		give_num = 50;
	}
	if (!consume_item)
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return;
	}

	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, give_num))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	/*if (m_role->GetKnapsack()->Count(consume_item->item_id) < consume_item->item_num)
	{
		m_role->NoticeItemLack(consume_item->item_id, consume_item->item_num);
		return;
	}*/

	// �齱
	std::vector<ItemConfigData> rewards;
	std::map<int, int> broadcast_map;
	for (int i = 0; i < give_num; ++i)
	{
		auto tmp_draw_type = draw_type;
		//if (i == 0)
		//{
		//	if (GREATE_SOLDIER_DRAW_TYPE_10_DRAW == draw_type)
		//	{
		//		tmp_draw_type = GREATE_SOLDIER_DRAW_TYPE_SPECIAL_10_DRAW;
		//	}
		//	else if (GREATE_SOLDIER_DRAW_TYPE_50_DRAW == draw_type)
		//	{
		//		tmp_draw_type = GREATE_SOLDIER_DRAW_TYPE_SPECIAL_50_DRAW;
		//	}
		//}

		auto rand_reward = GREATE_SOLDIER_CFG().GetRandomDrawReward(tmp_draw_type);
		if (rand_reward)
		{
			rewards.push_back(rand_reward->reward_item);
			if (rand_reward->broadcast_type > 0)
			{
				broadcast_map[rand_reward->reward_item.item_id] = rand_reward->broadcast_type;
			}
		}
	}
	if (rewards.size() == 0)
	{
		return; // impossible
	}

	// ����
	if (!m_role->GetKnapsack()->ConsumeItem(consume_item->item_id, consume_item->item_num, func_name))
	{
		m_role->NoticeItemLack(consume_item->item_id, consume_item->item_num);
		return;
	}

	if (GREATE_SOLDIER_DRAW_TYPE_10_DRAW == draw_type)
	{
		m_param.other_param.has_dailyfirst_draw_ten = 1;
	}

	switch (draw_type)
	{
		case GREATE_SOLDIER_DRAW_TYPE_1_DRAW:
		{
			m_param.draw_times += 1;
		}
		break;

		case GREATE_SOLDIER_DRAW_TYPE_10_DRAW:
		{
			m_param.draw_times += 10;
		}
		break;

		case GREATE_SOLDIER_DRAW_TYPE_50_DRAW:
		{
			m_param.draw_times += 50;
		}
		break;
	}
	

	// ����
	static ChestItemInfo reward_item_list[MODE_50_COUNT];
	for (int i = 0; i < MODE_50_COUNT; ++i)
	{
		reward_item_list[i].Reset();
	}

	int reward_count = 0;
	for (const auto &item : rewards)
	{
		reward_item_list[reward_count].item_id = item.item_id;
		reward_item_list[reward_count].num = (short)item.num;
		reward_item_list[reward_count].is_bind = item.is_bind;

		++reward_count;
		if (reward_count >= MODE_50_COUNT) break;
	}

	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, reward_count, reward_item_list, func_name);
	this->SendOtherInfo(false);
	this->SendFetchRewardInfo();
	gamecommon::SendDrawResult(m_role, rewards, Protocol::SCDrawResult::DRAW_REASON_GREATE_SOLDIER, give_num);

	{
		//����
		for (auto it : broadcast_map)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_greate_soldier_draw_content,
				m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), it.first);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, it.second);
			}
		}
	}

	EventHandler::Instance().OnGreateSoldierTenDraw(m_role);

	gamelog::g_log_greate_soldier.printf(LL_INFO, "GreateSoldier::OnDraw, role_id: %d, role_name: %s, draw_type: %d",
		m_role->GetUID(), m_role->GetName(), draw_type);
}

void GreateSoldier::OnUpgradePotential(int seq)
{
	const char *func_name = "GreateSoldier::OnUpgradePotential";

	if (!GREATE_SOLDIER_CFG().IsValidSeq(seq))
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return;
	}

	if (!this->IsActive(seq))
	{
		m_role->NoticeNum(errornum::EN_GREATESOLDIER_NOT_ALIVE);
		return;
	}
	
	GreateSoldierItemInfo* item_info = &m_param.item_param.item_list[seq];
	auto potential_cfg = GREATE_SOLDIER_POTENTIAL_CFG(seq, item_info->potential_level);
	if (!potential_cfg)
	{
		m_role->NoticeNum(errornum::EN_LEVEL_LIMIT);
		return;
	}

	// ����
	if (!m_role->GetKnapsack()->ConsumeItem(potential_cfg->upgrade_item.item_id, potential_cfg->upgrade_item.num, func_name))
	{
		m_role->NoticeItemLack(potential_cfg->upgrade_item.item_id, potential_cfg->upgrade_item.num);
		return;
	}

	// ����
	short old_level = item_info->potential_level;
	if (RandomNum(100) <= potential_cfg->succ_percent)
	{	
		++item_info->potential_level;
	}
	else
	{
		m_role->NoticeNum(errornum::EN_GREATESOLDIER_POTENTIAL_UP_FAIL);
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);

	this->SendItemInfo(seq);

	EventHandler::Instance().OnUpPotentialGreateSoldier(m_role, seq);

	gamelog::g_log_greate_soldier.printf(LL_INFO, "%s, role_id: %d, role_name: %s, potential_level: %d -> %d", func_name, m_role->GetUID(), m_role->GetName(),
		old_level, item_info->potential_level);
}

void GreateSoldier::SendAllInfo()
{
	this->SendItemInfo();
	this->SendOtherInfo(false);
	this->SendSlotInfo();
	this->SendFetchRewardInfo();
	m_goal.SendGoalInfo();
}

void GreateSoldier::AddBianshenCDReduce(int reduce_s, UInt16 skill_id)
{
	if (reduce_s > 0 && m_bianshen_cd_reduce_skillid.find(skill_id) == m_bianshen_cd_reduce_skillid.end())
	{
		m_bianshen_cd_reduce_s += reduce_s;
		m_bianshen_cd_reduce_skillid.insert({ skill_id, reduce_s });
	}
}

void GreateSoldier::ClearBianshenCDReduce(UInt16 skill_id)
{
	auto it = m_bianshen_cd_reduce_skillid.find(skill_id);
	if (it != m_bianshen_cd_reduce_skillid.end())
	{
		m_bianshen_cd_reduce_s -= it->second;
		if (m_bianshen_cd_reduce_s < 0)
		{
			m_bianshen_cd_reduce_s = 0;
		}

		m_bianshen_cd_reduce_skillid.erase(it);
	}
}

void GreateSoldier::ForceClearBianshenStatus()
{
	if (!this->IsBianshenStatus())
	{
		return;
	}

	// ǿ�Ʊ������
	{
		m_param.other_param.bianshen_end_timestamp = 0;
		m_param.other_param.cur_used_seq = -1;
		m_param.other_param.bianshen_cd = GREATE_SOLDIER_OTHER_CFG().bianshen_cd_s * 1000;

		m_skill.CheckActiveSkill();  // �����������⼼��

		// ������
		m_role->GetSpecialAppearance()->SetGreateSoldierAppearance(0, 0, true);

		this->SendOtherInfo(false);
	}
}

void GreateSoldier::SendItemInfo(int seq)
{
	Protocol::SCGreateSoldierItemInfo msg;

	if (-1 == seq)
	{
		for (int i = 0; i < GREATE_SOLDIER_MAX_COUNT; ++i)
		{
			if (GREATE_SOLDIER_CFG().IsValidSeq(i))
			{
				msg.seq = i;
				msg.item_info = m_param.item_param.item_list[i];
				SEND_TO_ROLE(m_role, msg);
			}
		}
	}
	else
	{
		if (GREATE_SOLDIER_CFG().IsValidSeq(seq))
		{
			msg.seq = seq;
			msg.item_info = m_param.item_param.item_list[seq];
			SEND_TO_ROLE(m_role, msg);
		}
	}
}

void GreateSoldier::SendOtherInfo(bool is_bianshen_cd_end)
{
	Protocol::SCGreateSoldierOtherInfo msg;
	msg.other_param = m_param.other_param;
	msg.bianshen_cd_reduce_s = m_bianshen_cd_reduce_s;
	msg.is_bianshen_cd_end = is_bianshen_cd_end ? 1 : 0;

	UNSTD_STATIC_CHECK(sizeof(msg.seq_exchange_counts) == sizeof(m_param.seq_exchange_counts))
	memcpy(msg.seq_exchange_counts, m_param.seq_exchange_counts, sizeof(msg.seq_exchange_counts));

	SEND_TO_ROLE(m_role, msg);
}

void GreateSoldier::SendSlotInfo()
{
	Protocol::SCGreateSoldierSlotInfo msg;
	msg.slot_param = m_param.slot_param;
	SEND_TO_ROLE(m_role, msg);
}

int GreateSoldier::GetCombineMinLevel(int seq)
{
	auto combine_cfg = GREATE_SOLDIER_COMBINE_CFG(seq);
	if (!combine_cfg || combine_cfg->greate_soldier_seq_list.Count() <= 1)
	{
		return 0;
	}

	int min_level = TWO_BILLION;
	for (int i = 0; i < combine_cfg->greate_soldier_seq_list.Count(); ++i)
	{
		int item_seq = *combine_cfg->greate_soldier_seq_list.Value(i);
		if (GREATE_SOLDIER_CFG().IsValidSeq(item_seq))
		{
			min_level = min_level >= m_param.item_param.item_list[item_seq].level ? m_param.item_param.item_list[item_seq].level : min_level;
		}
	}

	return min_level;
}

void GreateSoldier::CheckCombineFlag()
{
	for (int seq = 0; seq < GREATE_SOLDIER_COMBINE_MAX_COUNT; ++seq)
	{
		auto combine_cfg = GREATE_SOLDIER_COMBINE_CFG(seq);
		if (!combine_cfg || combine_cfg->greate_soldier_seq_list.Count() <= 1) continue;

		bool can_active = true;
		for (int i = 0; i < combine_cfg->greate_soldier_seq_list.Count(); ++i)
		{
			int item_seq = *combine_cfg->greate_soldier_seq_list.Value(i);
			if (GREATE_SOLDIER_CFG().IsValidSeq(item_seq))
			{
				if (!m_param.item_param.item_list[item_seq].HasActive())
				{
					can_active = false;
					break;
				}
			}
		}
		
		if (can_active)
		{
			SET_BIT(m_combine_flag, seq);
		}
	}
}

void GreateSoldier::OnFetchLimitDrawReward(int seq)
{
	return;
	/*
	ÿ���ɴγ齱������ȡһ�ν���
	*/

	const GreateSoldierLimitDrawConfig& reward_cfg = GREATE_SOLDIER_CFG().GetLimitDrawConfig();
	if (seq < 0 || seq >= int(reward_cfg.reward_vec.size()))
	{
		m_role->NoticeNum(errornum::EN_BEAUTY_FETCH_SEQ_ERROR);
		return;
	}

	if (IS_BIT_SET(m_param.reward_fetch_flag, seq))
	{
		m_role->NoticeNum(errornum::EN_BEAUTY_FETCH_HAS_FETCHED);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
		return;
	}


	const GreateSoldierLimitDrawConfig::CfgItem &cfg = reward_cfg.reward_vec[seq];
	if (m_param.draw_times >= cfg.draw_num)
	{
		SET_BIT(m_param.reward_fetch_flag, seq);
	}

	// ����
	m_role->GetKnapsack()->PutListOrMail(cfg.reward_list, PUT_REASON_GREATESOLDIER_FETCH_DRAW_REWARD);
	// Send and Log
	this->SendFetchRewardInfo();

	gamelog::g_log_greate_soldier.printf(LL_INFO, "GreateSoldier::OnFetchLimitDrawReward, role_id: %d, role_name: %s, draw_times: %d, reward_fetch_flag %u",
		m_role->GetUID(), m_role->GetName(), m_param.draw_times, m_param.reward_fetch_flag);
}

void GreateSoldier::OnExchangeSeq(int seq)
{
	/*
	*���ڶһ��û���Ʒ
	* ÿ���д�������
	* ��Ҫ��������
	* �Ļ�����һ����������Ʒ
	*/

	auto gs_cfg = GREATE_SOLDIER_LEVEL_CFG(seq);
	if (!gs_cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	if (seq < 0 || seq >= GREATE_SOLDIER_MAX_COUNT)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	short & exchange_count = m_param.seq_exchange_counts[seq];
	if (exchange_count >= gs_cfg->exchange_counts)
	{
		m_role->NoticeNum(errornum::EN_NO_TIMES);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// ����
	if (!m_role->GetKnapsack()->ConsumeItem(gs_cfg->exchange_item_id, gs_cfg->exchange_item_num, "GreateSoldier::OnExchangeSeq"))
	{
		m_role->NoticeItemLack(gs_cfg->exchange_item_id, gs_cfg->exchange_item_num);
		return;
	}

	int old_exchange_count = exchange_count;
	exchange_count += 1;

	// ����
	m_role->GetKnapsack()->Put(ItemConfigData(gs_cfg->item_id, true, 1), PUT_REASON_GREATESOLDIER);

	m_role->NoticeNum(noticenum::NT_GREATESOLDIER_EXCHANGE_SUCC);
	this->SendOtherInfo(false);
	gamelog::g_log_greate_soldier.printf(LL_INFO, "greatesoldier::exchange, role_id: %d, role_name: %s, seq: %d, exchange_count: %d -> %d",
		m_role->GetUID(), m_role->GetName(), seq, old_exchange_count, exchange_count);
}

void GreateSoldier::CheckFetchLimitDrawReward()
{
	//����
	return;
	const GreateSoldierLimitDrawConfig& cfg = GREATE_SOLDIER_CFG().GetLimitDrawConfig();

	for (int i = 0; i < int(cfg.reward_vec.size()); ++i)
	{
		if (!IS_BIT_SET(m_param.reward_fetch_flag, i) && m_param.draw_times >= cfg.reward_vec[i].draw_num)
		{
			static MailContentParam contentparam; contentparam.Reset();
			contentparam.AddItem(cfg.reward_vec[i].reward_list[0]);
			contentparam.AddItem(cfg.reward_vec[i].reward_list[1]);
			contentparam.AddItem(cfg.reward_vec[i].reward_list[2]);

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_greate_fetch_reward_content);
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}

	m_param.reward_fetch_flag = 0;
	m_param.draw_times = 0;

	this->SendFetchRewardInfo();
}

void GreateSoldier::SendFetchRewardInfo()
{
	return;
	Protocol::SCGreateSoldierFetchReward gsfr;

	gsfr.draw_times = m_param.draw_times;
	gsfr.fetch_flag = m_param.reward_fetch_flag;

	SEND_TO_ROLE(m_role, gsfr);
}

void GreateSoldier::CheckFuncOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (m_role->GetRoleBigSmallGoal()->IsFuncActive(ROLE_BIG_SMALL_GOAL_SYSTEM_GREATE_SOLDIER))
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_GREATE_SOLDIER);
	if (fun_open_cfg != nullptr && opengame_day >= fun_open_cfg->opengame_day)
	{
		if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
		{
			m_role->GetRoleBigSmallGoal()->OnOpenFunc(ROLE_BIG_SMALL_GOAL_SYSTEM_GREATE_SOLDIER);
		}
	}
}

// ��ħװ�����
//////////////////////////////////////////////////////////////////////////
int GreateSoldier::PutOnEquip(ItemDataWrapper &item_wrapper, int seq)
{
	if (!GREATE_SOLDIER_CFG().IsValidSeq(seq))
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return -1;
	}

	if (!this->IsActive(seq))
	{
		m_role->NoticeNum(errornum::EN_GREATESOLDIER_NOT_ALIVE);
		return -2;
	}

	//GreateSoldierItemInfo* item_info = &m_param.item_param.item_list[seq];

	const Equipment * equip = dynamic_cast<const Equipment*>(ITEMPOOL->GetItem(item_wrapper.item_id));
	if (nullptr == equip)
	{
		return -3;
	}

	int equip_slot_index = equip->GetEquipType() - Equipment::E_TYPE_GREATESOLDIER_MIN;
	if (equip_slot_index < 0 || equip_slot_index >= GREATE_SOLDIER_EQUIPMENT_SLOT_MAX_COUNT)
	{
		return -4;
	}

	auto level_cfg = GREATE_SOLDIER_LEVEL_CFG(seq);

	if (level_cfg->color < equip->GetColor()) // Ʒ�ʲ�����������װ��
	{
		m_role->NoticeNum(errornum::EN_GREATESOLDIER_QUALITY_NOT_ENOUGH);
		return -5;
	}

	GreateSoldierEquipmentInfo &equipment_item = m_param.item_param.item_list[seq].equipment_list[equip_slot_index];

	if (equipment_item.item_id > 0)
	{
		if (!this->TakeOffEquip(seq, equip_slot_index, true))
		{
			return -6;
		}
	}

	equipment_item.item_id = item_wrapper.item_id;
	equipment_item.strength_level = item_wrapper.param_data.strengthen_level;
	equipment_item.shuliandu = item_wrapper.param_data.param1;

	gamelog::g_log_greate_soldier.printf(LL_INFO, "GreateSoldier::PutOnEquip user[%d, %s] seq[%d] equipment_slot[%d, %d] ",
		m_role->GetUID(), m_role->GetName(), seq, equip_slot_index, equipment_item.item_id);

	this->SendItemInfo(seq);

	m_role->NoticeNum(noticenum::NT_GREATESOLDIER_PUTON_SUCC);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);

	return 0;
}

bool GreateSoldier::TakeOffEquip(int seq, int equip_slot_index, bool is_puton)
{
	if (!GREATE_SOLDIER_CFG().IsValidSeq(seq))
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return false;
	}

	if (!this->IsActive(seq))
	{
		m_role->NoticeNum(errornum::EN_GREATESOLDIER_NOT_ALIVE);
		return false;
	}

	if (equip_slot_index < 0 || equip_slot_index >= GREATE_SOLDIER_EQUIPMENT_SLOT_MAX_COUNT)
	{
		return false;
	}

	GreateSoldierEquipmentInfo &equipment_item = m_param.item_param.item_list[seq].equipment_list[equip_slot_index];
	if (equipment_item.item_id <= 0)
	{
		if (!is_puton)
		{
			m_role->NoticeNum(errornum::EN_GREATESOLDIER_NO_EQUIP);
		}
		return false;
	}

	const Equipment * equip = dynamic_cast<const Equipment*>(ITEMPOOL->GetItem(equipment_item.item_id));
	if (nullptr == equip)
	{
		return false;
	}

	int equip_type = equip->GetEquipType() - Equipment::E_TYPE_GREATESOLDIER_MIN;
	if (equip_type != equip_slot_index)
	{
		return false;
	}

	Knapsack* knapsack = m_role->GetKnapsack();
	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	ItemDataWrapper put_item;
	put_item.item_id = equipment_item.item_id;
	put_item.is_bind = true;	// 2019-04-25 ���¸ĳɰ󶨣���ֹ��������������󶨱��
	put_item.num = 1;
	put_item.has_param = 1;
	put_item.param_data.strengthen_level = equipment_item.strength_level;
	put_item.param_data.param1 = equipment_item.shuliandu;

	if (!knapsack->Put(put_item, PUT_REASON_NO_NOTICE))
	{
		return false;
	}

	equipment_item.Reset();

	if (!is_puton)
	{
		m_role->NoticeNum(noticenum::NT_GREATESOLDIER_TAKE_OFF_SUCC);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);

		this->SendItemInfo(seq);
	}

	gamelog::g_log_greate_soldier.printf(LL_INFO, "GreateSoldier::TakeOffEquip user[%d, %s] seq[%d] equipment_slot[%d, %d], level[%d], shuliandu[%d] ",
		m_role->GetUID(), m_role->GetName(), seq, equip_slot_index, put_item.item_id, put_item.param_data.strengthen_level, put_item.param_data.param1);
	
	return true;
}

void GreateSoldier::UpgradeEquip(int seq, int equip_slot_index, short destroy_num, short destroy_backpack_index_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM])
{
	if (!GREATE_SOLDIER_CFG().IsValidSeq(seq))
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return;
	}

	if (!this->IsActive(seq))
	{
		m_role->NoticeNum(errornum::EN_GREATESOLDIER_NOT_ALIVE);
		return;
	}

	if (equip_slot_index < 0 || equip_slot_index >= GREATE_SOLDIER_EQUIPMENT_SLOT_MAX_COUNT
		|| destroy_num <= 0 || destroy_num > ItemNamespace::MAX_KNAPSACK_GRID_NUM)
	{
		return;
	}

	int MAX_LEVEL = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierItemMaxLevel();  // ���õ�����ǿ���ȼ�

	GreateSoldierEquipmentInfo &equip_item = m_param.item_param.item_list[seq].equipment_list[equip_slot_index];

	if (equip_item.item_id <= 0)
	{
		m_role->NoticeNum(errornum::EN_GREATESOLDIER_NO_EQUIP);
		return;
	}

	if (equip_item.strength_level >= MAX_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_GREATESOLDIER_EQUIP_STRENGTH_LEVEL_MAX);
		return;
	}

	const Equipment * equip = dynamic_cast<const Equipment*>(ITEMPOOL->GetItem(equip_item.item_id));
	if (nullptr == equip || ((equip->GetEquipType() - Equipment::E_TYPE_GREATESOLDIER_MIN) != equip_slot_index))
	{
		return;
	}

	long long total_add_shuliandu = 0;	// ���������������Ʒ�õ�����������
	auto now_shuliandu_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierEquipLevelConfig(equip_slot_index, equip->GetColor(), equip_item.strength_level);
	if (nullptr == now_shuliandu_cfg)
	{
		return;
	}
	int now_contain_shulian = now_shuliandu_cfg->contain_shulian;	// �ϼ���ǿ������ʱ����������ֵcontain_shulian

	int shuliandu_max = LOGIC_CONFIG->GetGreateSoldierConfig().GetEquipShulianduMax(equip_slot_index, equip->GetColor());
	if (shuliandu_max < 0)
	{
		return;
	}

	Knapsack * knapasck = m_role->GetKnapsack();
	if (knapasck->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	// �ͻ��˷��͵ı������������ظ����ظ���������ݱ�ˢ������ֱ�Ӵ���set��ȡһ����Ч�ľ���
	std::set<short> destroy_backpack_index_set;
	for (int i = 0; i < destroy_num && i < ItemNamespace::MAX_KNAPSACK_GRID_NUM; ++i)
	{
		int backpack_idx = destroy_backpack_index_list[i];
		if (backpack_idx < 0 || backpack_idx >= ItemNamespace::MAX_KNAPSACK_GRID_NUM)
		{
			continue;
		}
		destroy_backpack_index_set.insert(backpack_idx);
	}


	bool is_overstep = false;
	std::map<int, int> consum_index;
	//for (int consume_num = 0; consume_num < destroy_num && consume_num < ItemNamespace::MAX_KNAPSACK_GRID_NUM; ++consume_num)
	for (auto &it : destroy_backpack_index_set)
	{
		//int backpack_index = destroy_backpack_index_list[consume_num];
		int backpack_index = it;
		if (backpack_index < 0 || backpack_index >= ItemNamespace::MAX_KNAPSACK_GRID_NUM)
		{
			continue;
		}

		ItemGridData * tmp_equip_item = knapasck->GetItemGridData(backpack_index);
		if (nullptr == tmp_equip_item) {
			continue;
		}

		EquipmentParam* tmp_equip_param = dynamic_cast<EquipmentParam*>(tmp_equip_item->param);
		if (nullptr == tmp_equip_param)
		{
			continue;
		}

		const Equipment * tmp_equip = dynamic_cast<const Equipment*>(ITEMPOOL->GetItem(tmp_equip_item->item_id));
		if (nullptr == tmp_equip || tmp_equip->GetEquipType() < Equipment::E_TYPE_GREATESOLDIER_MIN || tmp_equip->GetEquipType() >= Equipment::E_TYPE_GREATESOLDIER_MAX)
		{
			continue;
		}

		auto tmp_shuliandu_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierEquipLevelConfig(equip_slot_index, tmp_equip->GetColor(), tmp_equip_param->strengthen_level);
		if (nullptr != tmp_shuliandu_cfg)
		{
			total_add_shuliandu += tmp_shuliandu_cfg->contain_shulian;
		}

		total_add_shuliandu += tmp_equip_param->param1; // ��ǰ����������

		consum_index[backpack_index] = 0;
		// �ж�����������
		for (int i = 0; i < tmp_equip_item->num; ++i)
		{
			consum_index[backpack_index]++;

			if (total_add_shuliandu >= (shuliandu_max - (equip_item.shuliandu + now_contain_shulian)))
			{
				is_overstep = true;
				break;
			}
		}

		if (is_overstep)
		{
			break;
		}
	}

	if (total_add_shuliandu <= 0)
	{
		return;
	}

	long long left_shuliandu = total_add_shuliandu;

	GreateSoldierEquipmentInfo target_equip_item = equip_item; // ��ʱ����������

	while (left_shuliandu > 0)
	{
		auto cur_shuliandu_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierEquipLevelConfig(equip_slot_index, equip->GetColor(), target_equip_item.strength_level);
		auto next_shuliandu_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierEquipLevelConfig(equip_slot_index, equip->GetColor(), target_equip_item.strength_level + 1);
		if (NULL == cur_shuliandu_cfg || NULL == next_shuliandu_cfg) // ��û����һ�����ã�˵������߼���
		{
			break;
		}

		int upgrade_need_shuliandu = cur_shuliandu_cfg->upgrade_need_shulian - target_equip_item.shuliandu;
		if (left_shuliandu >= upgrade_need_shuliandu)
		{
			left_shuliandu -= upgrade_need_shuliandu;

			++target_equip_item.strength_level;
			target_equip_item.shuliandu = 0;

			if (target_equip_item.strength_level >= MAX_LEVEL)
			{
				target_equip_item.strength_level = MAX_LEVEL;
				break;
			}
		}
		else
		{
			target_equip_item.shuliandu += int(left_shuliandu);
			left_shuliandu = 0;
		}
	}

	// ������Ʒ
	for (auto it = consum_index.begin(); it != consum_index.end(); ++it)
	{
		knapasck->ConsumeItemByIndex(it->first, it->second, "GreateSoldier::UpgradeEquip");
	}

	// ���轱������
	equip_item = target_equip_item;

	this->SendItemInfo(seq);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);

	gamelog::g_log_greate_soldier.printf(LL_INFO, "GreateSoldier::UpgradeEquip user[%d, %s] seq[%d] equipment_slot[%d, %d], level[%d], shuliandu[%d] ",
		m_role->GetUID(), m_role->GetName(), seq, equip_slot_index, equip_item.item_id, equip_item.strength_level, equip_item.shuliandu);

}

void GreateSoldier::OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill)
{
	m_skill.OnRoleBeenAttack(attacker, skill_id, delta_hp, from_skill);
}

void GreateSoldier::GmClearAllInfo()
{
	m_param.Reset();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_INVALID);

	this->SendAllInfo();
}

