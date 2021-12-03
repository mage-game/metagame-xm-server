#include <algorithm>
#include "shengesystem.hpp"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "config/logicconfigmanager.hpp"
#include "other/shengesystem/shengesystemconfig.hpp"
#include "protocal/msgshengesystem.h"
#include "engineadapter.h"
#include "obj/character/monster.h"
#include "gamelog.h"
#include "effect/skilleffect/effectonce.hpp"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "item/itemextern.hpp"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"
#include "other/event/eventhandler.hpp"
#include "config/joinlimitconfig.hpp"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "task/taskrecorder.h"
#include "other/lotterycount/lotterycount.hpp"
#include "config/rolebigsmallgoalconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

ShengeSystem::ShengeSystem(): m_role(NULL), m_empty_grids(0), m_drop_cd_tiemstamp(0), m_can_recover(true), m_time_interval(0), m_other_capbility(0), m_up_zhangkong_level_notice(0)
{
	memset(m_combine_seq_list, 0, sizeof(m_combine_seq_list));
}

ShengeSystem::~ShengeSystem()
{
}

void ShengeSystem::InitParam(Role *role, const ShengeSystemParam &param)
{
	m_role = role;
	m_param = param;
	this->SortVirtrulBag();
	this->ReCalShengeEffct(true);
	this->ReCalcSuitCount();		//重算套装数据
}

void ShengeSystem::GetInitParam(ShengeSystemParam *param) const
{
	*param = m_param;
}

void ShengeSystem::OnDayChange(int old_dayid, int now_dayid)
{
	m_param.free_choujiang_times = 0;
	this->CheckFuncOpen();
}

void ShengeSystem::Update(unsigned long interval, unsigned int now_second)
{
	m_time_interval += interval;

	if (m_can_recover && m_time_interval >= (SHENGE_SYSTEM_RECOVER_TIME_INTERVAL * 1000))
	{
		m_time_interval = 0;
		this->CheckShengesystemRecover();
	}
}

void ShengeSystem::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		this->ReCalcSuitCount();	//重算套装数据

		m_attrs_add.Reset();
		static CharIntAttrs page_attrs_add_list[SHENGE_SYSTEM_CUR_SHENGE_PAGE];
		static CharIntAttrs zhangkong_attrs_add; zhangkong_attrs_add.Reset();
		
		for (int i = 0; i < SHENGE_SYSTEM_CUR_SHENGE_PAGE; i++)
		{
			page_attrs_add_list[i].Reset();
		}
		
		m_other_capbility = 0;
		
		// 神格页基础属性加成
		const ShengeSystemOtherCfg &other_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetOterCfg();
		for (int page_index = 0; page_index < SHENGE_SYSTEM_CUR_SHENGE_PAGE; ++page_index)
		{
			static CharIntAttrs page_attrs_add;
			page_attrs_add.Reset();
			double add_percent = other_cfg.unactive_attr_add_percen / 100.0;
			if (page_index == m_param.using_shenge_page_index)
			{
				add_percent = other_cfg.active_attr_add_percen / 100.0;
			}

			for (int grid_index = 0; grid_index < SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID; ++grid_index)
			{
				ShengeParamData &shenge = m_param.shenge_page_list[page_index].shenge_grid[grid_index];
				if (shenge.IsEmpty())
				{
					continue;
				}

				const ShengeSystemAttrLevelCfg *shenge_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetShengeSystemCfgByQualityTypeLevel(shenge.quality, shenge.type, shenge.level);
				if (NULL == shenge_cfg)
				{
					continue;
				}

				for (int attr_index = 0; attr_index < ShengeSystemAttrLevelCfg::MAX_ATTR_COUNTS; ++attr_index)
				{
					const ShengeSystemAttrSingelCfg &attr_cfg = shenge_cfg->base_attr[attr_index];

					int real_type;
					if (this->TransformAttrType(attr_cfg.attr_type, real_type))
					{
//						page_attrs_add.m_attrs[real_type] += static_cast<Attribute>(attr_cfg.attr_value * add_percent);
						page_attrs_add.AddValue(real_type, static_cast<Attribute>(attr_cfg.attr_value * add_percent));
						
						//聚星加成规则（旧）
						//int quyu = shenge_cfg->quyu - 1;
						//if (quyu >= 0 && quyu < SHENGE_SYSTEM_QUALITY_MAX_VALUE  && -1 != m_param.zhangkong_level_list[quyu].level)
						//{
						//	ZhangkongLevelCfg::ConfigItem * zhangkong_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetZhangkongCfgByGridAndLevel(quyu, m_param.zhangkong_level_list[quyu].level);
						//	if (NULL != zhangkong_cfg)
						//	{
						//		zhangkong_attrs_add.m_attrs[real_type] += static_cast<Attribute>(attr_cfg.attr_value * add_percent * zhangkong_cfg->shenge_pro * 0.0001);
						//	}
						//}

						{
							//聚星加成规则（新）
							for (int grid_index = 0; grid_index < SHENGE_SYSTEM_QUALITY_MAX_VALUE; grid_index++)
							{
								const UplevelGridCfg * grid_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetUplevelGridCfg(grid_index);
								if (grid_cfg == NULL)
								{
									continue;
								}

								//if ((shenge.quality+1) <= grid_cfg->quality_range)
								if ((shenge.quality + 1) >= grid_cfg->quality_range && (shenge.quality + 1) <= grid_cfg->quality_range_2)
								{
									ZhangkongLevelCfg::ConfigItem * zhangkong_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetZhangkongCfgByGridAndLevel(grid_index, m_param.zhangkong_level_list[grid_index].level);
									if (zhangkong_cfg == NULL)
									{
										continue;
									}

//									zhangkong_attrs_add.m_attrs[real_type] += static_cast<Attribute>(attr_cfg.attr_value * add_percent * zhangkong_cfg->shenge_pro * 0.0001);
									zhangkong_attrs_add.AddValue(real_type, static_cast<Attribute>(attr_cfg.attr_value * add_percent * zhangkong_cfg->shenge_pro * 0.0001));
								}
							}
						}
					}
				}
				m_other_capbility += static_cast<int>(shenge_cfg->capbility * add_percent);
			}

			page_attrs_add_list[page_index] = page_attrs_add;

			{ // 激活大目标
				int add_value = m_role->GetRoleBigSmallGoal()->GetRoleBigSmallGoalAddAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGE);
				if (add_value > 0)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(page_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (add_value * ROLE_ATTR_PER));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(page_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (add_value * ROLE_ATTR_PER));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(page_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (add_value * ROLE_ATTR_PER));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(page_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (add_value * ROLE_ATTR_PER));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(page_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (add_value * ROLE_ATTR_PER));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(page_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (add_value * ROLE_ATTR_PER));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(page_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (add_value * ROLE_ATTR_PER));
				}
			}

			m_attrs_add.Add(page_attrs_add.m_attrs);
		}

		// 神格组合加成
		for (int seq = 0; seq < SHENGE_SYSTEM_COMBINATION_MAX_CFG_COUNTS; seq++)
		{
			if (0 == m_combine_seq_list[seq])
			{
				continue;
			}

			const ShengeSystemCombinationCfg *combination_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetCombinationCfgBySeq(seq);
			if (NULL != combination_cfg)
			{
				for (int page_index = 0; page_index < SHENGE_SYSTEM_CUR_SHENGE_PAGE; ++page_index)
				{
					for (int i = 0; i < SHENGE_SYSTEM_COMBINATION_PERCEN_CFG; ++i)
					{
						int real_type;
						const ShengeSystemCombinationCfg::AddPerCenCfg &percen_cfg = combination_cfg->add_percen_cfg_list[i];
						double add_percen = percen_cfg.attr_value / 100.0;
						if (!this->TransformAttrType(percen_cfg.attr_tyep, real_type)) continue;
//						m_attrs_add.m_attrs[real_type] += static_cast<Attribute>(page_attrs_add_list[page_index].m_attrs[real_type] * add_percen);
						m_attrs_add.AddValue(real_type, static_cast<Attribute>(page_attrs_add_list[page_index].m_attrs[real_type] * add_percen));
					}
				}
			}
		}

		// 神格组合特殊效果加成
		if (this->CheckIsActiveSpecialEffect(SHENGE_SPECIAL_EFFECT_TYPE_ADD_SHENGE_ATTR))
		{
			double add_percen = m_special_buff_list[SHENGE_SPECIAL_EFFECT_TYPE_ADD_SHENGE_ATTR];

			for (int page_index = 0; page_index < SHENGE_SYSTEM_CUR_SHENGE_PAGE; ++page_index)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(page_attrs_add_list[page_index].m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * add_percen);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(page_attrs_add_list[page_index].m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * add_percen);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(page_attrs_add_list[page_index].m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * add_percen);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(page_attrs_add_list[page_index].m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * add_percen);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(page_attrs_add_list[page_index].m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * add_percen);
				// 不加移速
				//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED] += static_cast<Attribute>(page_attrs_add_list[page_index].m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED] * add_percen);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(page_attrs_add_list[page_index].m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * add_percen);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(page_attrs_add_list[page_index].m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * add_percen);
			}
		}

		// 掌控基础加成
		for (int grid_index = 0; grid_index < SHENGE_SYSTEM_QUALITY_MAX_VALUE; ++grid_index)
		{
			ZhangkongLevelCfg::ConfigItem * zhangkong_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetZhangkongCfgByGridAndLevel(grid_index, m_param.zhangkong_level_list[grid_index].level);
			if (NULL == zhangkong_cfg) continue;

			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += zhangkong_cfg->maxhp_pro;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += zhangkong_cfg->gongji_pro;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += zhangkong_cfg->fangyu_pro;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += zhangkong_cfg->baoji_pro;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += zhangkong_cfg->kangbao_pro;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += zhangkong_cfg->mingzhong_pro;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += zhangkong_cfg->shanbi_pro;
		}

		// 神格神躯
		static CharIntAttrs shenqu_attr_list;
		shenqu_attr_list.Reset();

		for (int shenqu_id = 0; shenqu_id < SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM; ++ shenqu_id)
		{
			for (int point_type = 0; point_type < SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM; ++point_type)
			{
				for (int grid_index = 0; grid_index < SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM; ++grid_index)
				{
					const AttrInfo &grid_cfg = m_param.shenqu_list[shenqu_id].shenqu_attr_list[point_type].attr_info[grid_index];

					int real_type = this->GetRealType(grid_cfg.attr_point);
					if (real_type < 0) continue;

// 					m_attrs_add.m_attrs[real_type] += grid_cfg.attr_value;
// 					shenqu_attr_list.m_attrs[real_type] += grid_cfg.attr_value;

					m_attrs_add.AddValue(real_type, grid_cfg.attr_value);
					shenqu_attr_list.AddValue(real_type, grid_cfg.attr_value);
				}
			}

			int count = this->GetShenquPerfectAttrNum(shenqu_id);

			if (count > 0)
			{
				const ShengeSystemShenquCfg *shenqu_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetShenquCfg(shenqu_id);
				if (NULL != shenqu_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (shenqu_cfg->value_percent * 0.01 * count));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (shenqu_cfg->value_percent * 0.01 * count));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (shenqu_cfg->value_percent * 0.01 * count));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (shenqu_cfg->value_percent * 0.01 * count));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (shenqu_cfg->value_percent * 0.01 * count));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (shenqu_cfg->value_percent * 0.01 * count));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (shenqu_cfg->value_percent * 0.01 * count));

					{
						// 特殊加成后当前神躯的战力
						shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (shenqu_cfg->value_percent * 0.01 * count));
						shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (shenqu_cfg->value_percent * 0.01 * count));
						shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (shenqu_cfg->value_percent * 0.01 * count));
						shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (shenqu_cfg->value_percent * 0.01 * count));
						shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (shenqu_cfg->value_percent * 0.01 * count));
						shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (shenqu_cfg->value_percent * 0.01 * count));
						shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(shenqu_attr_list.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (shenqu_cfg->value_percent * 0.01 * count));
					}
				}
			}

			int cur_shenqu_cap = Capability::GetCapByAttr(shenqu_attr_list);
			if (cur_shenqu_cap > m_param.shenqu_history_cap_list[shenqu_id])
			{
				m_param.shenqu_history_cap_list[shenqu_id] = cur_shenqu_cap;
			}
			shenqu_attr_list.Reset();
		}

		//（新）套装属性
		{
			for (int i = 0; i < SHENGE_QUALITY_COUNT; i++)
			{
				//int suit_count = 0;
				//suit_count = this->GetSatisfyMagicalPreciousCount(i);
				const ShengeSystemNewSuitAttrCfg *suit_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetSuitCfg(i, m_quality_equip_count[i]);
				if (suit_cfg != NULL)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += suit_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += suit_cfg->baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += suit_cfg->jianren;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += suit_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += suit_cfg->shanbi;
//					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += suit_cfg->per_mianshang;

					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, suit_cfg->per_mianshang);

					m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(suit_cfg->per_gongji * ROLE_ATTR_PER);	//套装全身攻击加成
					m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(suit_cfg->per_maxhp * ROLE_ATTR_PER);		//套装全身气血加成
					m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<float>(suit_cfg->per_fangyu * ROLE_ATTR_PER);	//套装全身防御加成
				}
			}
		}

		// 神格区域的加成，要放最后面
		m_attrs_add.Add(zhangkong_attrs_add.m_attrs);
		// 人物大目标固定属性
		m_role->GetRoleBigSmallGoal()->RoleBigSmallGoalAddSpecialAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGE, m_attrs_add);
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SHENGE_SYSTEM, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_SHENGE_SYSTEM, m_other_capbility);

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

void ShengeSystem::ReCalShengeEffct(bool is_init, bool is_broadcast_combine)
{
	m_special_buff_list.clear();
	int old_combine_seq_list[SHENGE_SYSTEM_COMBINATION_MAX_CFG_COUNTS];
	memcpy(old_combine_seq_list, m_combine_seq_list, sizeof(old_combine_seq_list));
	memset(m_combine_seq_list, 0, sizeof(m_combine_seq_list));
	// 神格页基础效果加成
	const ShengeSystemOtherCfg &other_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetOterCfg();
	for (int page_index = 0; page_index < SHENGE_SYSTEM_CUR_SHENGE_PAGE; ++page_index)
	{
		double add_percen = other_cfg.unactive_attr_add_percen / 100.0;
		if (page_index == m_param.using_shenge_page_index)
		{
			add_percen = other_cfg.active_attr_add_percen / 100.0;
		}

		for (int grid_index = 0; grid_index < SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID; ++grid_index)
		{
			ShengeParamData &shenge = m_param.shenge_page_list[page_index].shenge_grid[grid_index];
			if (shenge.IsEmpty())
			{
				continue;
			}

			const ShengeSystemAttrLevelCfg *shenge_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetShengeSystemCfgByQualityTypeLevel(shenge.quality, shenge.type, shenge.level);
			if (NULL == shenge_cfg)
			{
				continue;
			}

			for (int attr_index = 0; attr_index < ShengeSystemAttrLevelCfg::MAX_ATTR_COUNTS; ++attr_index)
			{
				const ShengeSystemAttrSingelCfg &attr_cfg = shenge_cfg->base_attr[attr_index];
				this->ActiveSpecialEffectByShengeType(shenge.type, attr_cfg.attr_value * add_percen);
			}
		}
	}

	// 检查组合
	int combine_count = LOGIC_CONFIG->GetShengesystemCfg().GetCombinationCfgCount();
	for (int seq = 0; seq < combine_count && seq < SHENGE_SYSTEM_COMBINATION_MAX_CFG_COUNTS; ++seq)
	{
		if (!this->CheckCanActiveCombinationBySeq(seq))
		{
			continue;
		}

		const ShengeSystemCombinationCfg *combination_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetCombinationCfgBySeq(seq);
		if (NULL != combination_cfg)
		{
			for (int i = 0; i < SHENGE_SYSTEM_COMBINATION_SPECIAL_EFFECT_CFG; ++i)
			{
				const ShengeSystemCombinationCfg::SpecialEffectCfg &effect_cfg = combination_cfg->special_cfg_list[i];
				this->ActiveSpecialEffect(effect_cfg.ecffect_type, effect_cfg.percen_value);
			}
		}

		m_combine_seq_list[seq] = 1;

		{
			// 传闻
			if (is_broadcast_combine)
			{
				if (1 == m_combine_seq_list[seq] && m_combine_seq_list[seq] != old_combine_seq_list[seq])
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenge_active_combine,
						m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), seq);
					if (length > 0) 
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
					}
				}
			}
		}
	}

	if (!is_init)
	{
		this->SendActiveCombinationInfo();
	}
}

void ShengeSystem::OperaationReq(const Protocol::CSShengeSystemReq *req)
{
	if (req->index_count > SHENGE_SYSTEM_BAG_MAX_GRIDS) return;

	switch (req->info_type)
	{
	case Protocol::SHENGE_SYSTEM_REQ_TYPE_ALL_INFO:
		{
			this->SendAllInfo();
		}
		break;

	case Protocol::SHENGE_SYSTEM_REQ_TYPE_DECOMPOSE:
		{
			this->ReqDecomposeShenge(req);
		}
		break;

	case Protocol::SHENGE_SYSTEM_REQ_TYPE_COMPOSE:
		{
			this->ReqComposeShenge(req->param1, req->param2, req->param3);
		}
		break;

	case Protocol::SHENGE_SYSTEM_REQ_TYPE_SET_RUAN:
		{
			this->ReqSetShenge(req->param1, req->param2, req->param3);
		}
		break;

	case Protocol::SHENGE_SYSTEM_REQ_TYPE_CHANGE_RUNA_PAGE:
		{
			this->ReqChangeShengePage(req->param1);
		}
		break;

	case Protocol::SHENGE_SYSTEM_REQ_TYPE_CHOUJIANG:
		{
			this->ReqChoujiang(req->param1);
		}
		break;

	case Protocol::SHENGE_SYSTEM_REQ_TYPE_UPLEVEL:
		{
			if (0 == req->param1)
			{
				this->ReqUpShengeLevelInVirtualBag(req->param2);
			}
			else if (1 == req->param1)	//升级一次
			{
				this->ReqUpShengeLevelInShengePage(req->param2, req->param3, false);
			}
			else if (2 == req->param1)	//自动升级
			{
				this->ReqUpShengeLevelInShengePage(req->param2, req->param3, true);
			}
		}
		break;

	case Protocol::SHENGE_SYSTEM_REQ_TYPE_SORT_BAG:
		{
			this->SortVirtrulBag();
		}
		break;

	case Protocol::SHENGE_SYSTEM_REQ_TYPE_UNLOAD_SHENGE:
		{
			this->ReqUnloadShenge(req->param1, req->param2);
		}
		break;

	case Protocol::SHENGE_STYTEM_REQ_TYPE_CLEAR_PAGE:
		{
			this->ReqClearShengePage(req->param1);
		}
		break;

	case Protocol::SHENGE_STYTEM_REQ_TYPE_UPLEVEL_ZHANGKONG:
		{
			//this->ReqUpLevelZhangkong(req->param1);
			//m_role->MsgReply(noticenum::NT_REPLY_TYPE_SHENGE, 1);

			this->ReqUpLevelGrid(req->param1, req->param2);
			m_role->MsgReply(noticenum::NT_REPLY_TYPE_SHENGE, 1);
		}
		break;

	case Protocol::SHENGE_STYTEM_REQ_TYPE_RECLAC_ATTR:
		{
			this->UpZhangkongLevelRet();
			this->SendZhangkongInfo();
		}
		break;

	case Protocol::SHENGE_SYSTEM_REQ_TYPE_XILIAN:
		{
			bool is_auto = 0 != req->param3;
			this->Strengthen(req->param1, req->param2, is_auto, req->param_4);
			m_role->MsgReply(noticenum::NT_REPLY_TYPE_SHENGE, 0);
		}
		break;

	default:
		break;
	}
}

UInt16 ShengeSystem::GetDropId()
{
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (!this->CheckIsActiveSpecialEffect(SHENGE_SPECIAL_EFFECT_TYPE_ADD_ITEM_DROP) || now_second < m_drop_cd_tiemstamp)
	{
		return 0;
	}

	// 消耗
	const ShengeSystemOtherCfg &other_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetOterCfg();
	m_drop_cd_tiemstamp = other_cfg.drop_item_cd_s + now_second;

	// 检查是否给予物品
	double randnum = RandomNum(SPECIAL_EFFECT_ADD_PERCENT) * 1.0 / SPECIAL_EFFECT_ADD_PERCENT;
	double effect_value = this->GetSpecialEffectValuPercentByType(SHENGE_SPECIAL_EFFECT_TYPE_ADD_ITEM_DROP);
	if (randnum > effect_value)
	{
		return 0;
	}

	// 随机给予物品
	int drop_id = 0;
	int rand_weight = RandomNum(LOGIC_CONFIG->GetShengesystemCfg().GetDropIdCfgTotalWeight());
	int drop_count = LOGIC_CONFIG->GetShengesystemCfg().GetDropIdCfgCount();
	for (int seq = 0; seq < drop_count; ++seq)
	{
		const ShengeSystemDropIdCfg *reward = LOGIC_CONFIG->GetShengesystemCfg().GetDropIdCfgBySeq(seq);
		if (NULL == reward)
		{
			continue;
		}

		if (rand_weight < reward->weight)
		{
			drop_id = reward->drop_id;
			break;
		}

		rand_weight -= reward->weight;
	}

	return UInt16(drop_id);
}

double ShengeSystem::GetSpecialEffectValuPercentByType(int effect_type)
{
	std::map<short, double>::iterator it_map = m_special_buff_list.find(effect_type);
	if (it_map != m_special_buff_list.end())
	{
		return it_map->second;
	}

	return 0;
}

void ShengeSystem::ReqUpShengeLevelInVirtualBag(int virtual_index)
{
	if (this->IsInvalidVirtualIndex(virtual_index))
	{
		return;
	}

	ShengeParamData &shenge = m_param.virtual_bag_list[virtual_index];
	if (!this->ReqUpShengeLevel(shenge))
	{
		return;
	}

	this->SendVirtualBagSingleInfo(virtual_index);
}

void ShengeSystem::ReqUpShengeLevelInShengePage(int page_index, int grid_index, bool is_auto)
{
	if (this->IsInvalidPageIndex(page_index) || this->IsInvalidGridIndex(grid_index))
	{
		return;
	}

	ShengeParamData &shenge = m_param.shenge_page_list[page_index].shenge_grid[grid_index];

	if (is_auto)
	{
		while (this->ReqUpShengeLevel(shenge))
		{
			this->SendShengePageGridSingleInfo(page_index, grid_index);
		}
	}
	else
	{
		if (!this->ReqUpShengeLevel(shenge))
		{
			return;
		}

		this->SendShengePageGridSingleInfo(page_index, grid_index);
	}
}

bool ShengeSystem::AddShengeByItemId(ItemID item_id)
{
	if (!this->CheckHasSpace())
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_BAG_NO_ENOUGH_SPACE);
		return false;
	}

	const ShengeSystemItemIdShengeCfg *shenge_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetItemIdShengeCfgByItemId(item_id);
	if (NULL == shenge_cfg)
	{
		return false;
	}

	ShengeParamData shenge;
	shenge.Reset();
	shenge.quality = static_cast<char>(shenge_cfg->quality);
	shenge.type = static_cast<char>(shenge_cfg->type);
	shenge.level = 1;

	this->PutShengeToVirtualBag(shenge, "ShengeSystem::AddShengeByItemId");
	return true;
}

bool ShengeSystem::AddShengeByItemIdByColor(int color, ItemID item_id, int num)
{
	if (color <= ItemBase::I_COLOR_PURPLE && m_role->GetShengeSystem()->CheckHasSpace(num))
	{
		for (int i = 0; i < num; i++)
		{
			m_role->GetShengeSystem()->AddShengeByItemId(item_id);
		}
		return true;
	}
	return false;
}

void ShengeSystem::SendAllInfo()
{
	this->SendVirtualBagAllInfo();
	this->SendShengePageGridAllInfo();
	this->SendMarrowScoreInfo();
	this->SendChoujiangInfo();
	this->SendActiveCombinationInfo();
	this->SendZhangkongInfo();
	this->SendXilianAllInfo();
}

void ShengeSystem::GmOpera(int type, int p1, int p2, int p3)
{
	switch (type)
	{
	case 0:
		{
			ShengeParamData shenge;
			shenge.quality = p1;
			shenge.type = p2;
			shenge.level = p3;

			this->PutShengeToVirtualBag(shenge, "Gm");
		}
		break;

	case 1:
		{
			this->ReqSetShenge(p1, p2, p3);
		}
		break;

	default:
		break;
	}
}

void ShengeSystem::RecalcShanbi(Character *target, int &shanbi)
{
	if (NULL == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	shanbi += static_cast<int>(shanbi * this->GetSpecialEffectValuPercentByType(SHENGE_SPECIAL_EFFECT_TYPE_ADD_SHANBI));
}

int ShengeSystem::GetDecHurtPercent()
{
	// 转化为百分比 分子数值
	return static_cast<int>(this->GetSpecialEffectValuPercentByType(SHENGE_SPECIAL_EFFECT_TYPE_DIKANG_SHANGHAI) * 100);
}

int ShengeSystem::GetAddExpPercent()
{
	// 转化为百分比 分子数值
	return static_cast<int>(this->GetSpecialEffectValuPercentByType(SHENGE_SPECIAL_EFFECT_TYPE_ADD_EXP) * 100);
}

int ShengeSystem::GetAddItemDropPercent()
{
	// 转化为百分比 分子数值
	return static_cast<int>(this->GetSpecialEffectValuPercentByType(SHENGE_SPECIAL_EFFECT_TYPE_ADD_ITEM_DROP) * 100);
}

void ShengeSystem::OnHurtOther(Character *target, long long base_injure, long long &injure)
{
	if (NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_MONSTER == target->GetObjType())
	{
		injure += static_cast<long long>(base_injure * this->GetSpecialEffectValuPercentByType(SHENGE_SPECIAL_EFFECT_TYPE_ADD_BOSS_DAMAGE));
	}

	if (Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		injure += static_cast<long long>(base_injure * this->GetSpecialEffectValuPercentByType(SHENGE_SPECIAL_EFFECT_TYPE_ADD_ROLE_DAMAGE));
	}
	
	injure += static_cast<long long>(base_injure * this->GetSpecialEffectValuPercentByType(SHENGE_SPECIAL_EFFECT_TYPE_ADD_DEC_FENGYU));
}

void ShengeSystem::OnStartFight()
{
	if (!this->CheckIsActiveSpecialEffect(SHENGE_SPECIAL_EFFECT_TYPE_ADD_RECOVER))
	{
		return;
	}

	m_can_recover = false;
}

void ShengeSystem::OnStopFight()
{
	if (!this->CheckIsActiveSpecialEffect(SHENGE_SPECIAL_EFFECT_TYPE_ADD_RECOVER))
	{
		return;
	}

	m_can_recover = true;
}

void ShengeSystem::CheckShengesystemRecover()
{
	if (!this->CheckIsActiveSpecialEffect(SHENGE_SPECIAL_EFFECT_TYPE_ADD_RECOVER))
	{
		return;
	}

	// 治疗
	const Attribute FIGHT_HP = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	const Attribute FIGHT_MAX_HP = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);

	double add_hp_rate = this->GetSpecialEffectValuPercentByType(SHENGE_SPECIAL_EFFECT_TYPE_ADD_RECOVER);
	Attribute add_hp = 0;
	if (FIGHT_HP > 0 && FIGHT_HP < FIGHT_MAX_HP)
	{
		add_hp = static_cast<Attribute>(add_hp_rate * FIGHT_MAX_HP);

		UInt64 test_final_hp = (UInt64)(add_hp + FIGHT_HP);
		if (test_final_hp > (UInt64)FIGHT_MAX_HP)
		{
			add_hp = FIGHT_MAX_HP - FIGHT_HP;
		}

		if (add_hp <= 0)
		{
			return;
		}

		m_role->ChangeHp(&add_hp, true);
	}
}

void ShengeSystem::ReqDecomposeShenge(const Protocol::CSShengeSystemReq *req)
{
	int all_return_score = 0;
	for (int i = 0; i < req->index_count; i++)
	{
		if (this->IsInvalidVirtualIndex(req->virtual_index_list[i]))
		{
			return;
		}

		ShengeParamData &shenge = m_param.virtual_bag_list[req->virtual_index_list[i]];
		const ShengeSystemAttrLevelCfg *shenge_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetShengeSystemCfgByQualityTypeLevel(shenge.quality, shenge.type, shenge.level);
		if (NULL == shenge_cfg)
		{
			m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_CAN_NOR_DECOMPOSE);
			return;
		}

		shenge.Reset();
		++m_empty_grids;
		all_return_score += shenge_cfg->return_score;
		gamelog::g_log_shenge_system.printf(LL_INFO, "ShengeSystem::ReqDecomposeShenge user[%d %s], decompose_shenge[quality:%d, type:%d, level:%d] add_score[%d]",
			m_role->GetUID(), m_role->GetName(), shenge.quality, shenge.type, shenge.level, shenge_cfg->return_score);
	}

	this->SendVirtualBagAllInfo();
	this->AddMarrowScore(all_return_score, "ShengeSystem::ReqDecomposeShenge");

	m_role->NoticeNum(noticenum::NT_SHENGE_SYSTEM_DECOMPOSE_SUCC);
}

void ShengeSystem::ReqComposeShenge(int virtual_index_1, int virtual_index_2, int virtual_index_3)
{
	/*if (!this->CheckHasSpace())
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_BAG_NO_ENOUGH_SPACE);
		return;
	}*/

	if (this->IsInvalidVirtualIndex(virtual_index_1) || this->IsInvalidVirtualIndex(virtual_index_2) || this->IsInvalidVirtualIndex(virtual_index_3))
	{
		return;
	}

	if (virtual_index_1 == virtual_index_2 || virtual_index_1 == virtual_index_3 || virtual_index_2 == virtual_index_3)
	{
		return;
	}

	ShengeParamData &shenge_1 = m_param.virtual_bag_list[virtual_index_1];
	ShengeParamData &shenge_2 = m_param.virtual_bag_list[virtual_index_2];
	ShengeParamData &shenge_3 = m_param.virtual_bag_list[virtual_index_3];

	if (shenge_1.IsEmpty() || shenge_1.quality != shenge_2.quality || shenge_2.quality != shenge_3.quality)
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_CAN_NOR_COMPOSE_DIFF_QUALITY);
		return;
	}
	int cur_quality = shenge_1.quality;
	int next_quality = cur_quality + 1;

	const ShengeSystemAttrLevelCfg *shenge1_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetShengeSystemCfgByQualityTypeLevel(cur_quality, shenge_1.type, shenge_1.level);
	const ShengeSystemAttrLevelCfg *shenge2_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetShengeSystemCfgByQualityTypeLevel(cur_quality, shenge_2.type, shenge_2.level);
	const ShengeSystemAttrLevelCfg *shenge3_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetShengeSystemCfgByQualityTypeLevel(cur_quality, shenge_3.type, shenge_3.level);
	if (NULL == shenge1_cfg || NULL == shenge2_cfg || NULL == shenge3_cfg)
	{
		return;
	}

	//不同种类不能合成
	if (shenge1_cfg->kind != shenge2_cfg->kind || shenge2_cfg->kind != shenge3_cfg->kind || shenge1_cfg->kind != shenge3_cfg->kind)
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_CAN_NOR_COMPOSE_DIFF_KIND);
		return;
	}
	int kind = shenge1_cfg->kind;

	// 从三个合成物品中选择一个类型
	std::vector<int> type_list;
	type_list.push_back(shenge_1.type);
	type_list.push_back(shenge_2.type);
	type_list.push_back(shenge_3.type);
	int type_index = RandomNum(static_cast<int>(type_list.size()));
	int type = type_list[type_index];

	// 获取合成配置
	const ShengeSystemAttrLevelCfg *shenge_next_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetShengeSystemCfgByQualityTypeLevel(next_quality, type, 1);
	if (NULL == shenge_next_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_MAX_COMPOSE_QUALITY);
		return;
	}

	const ShengeSystemDecomposeQualityCfg *compose_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetcomposeCfgByKindAndQuality(kind, shenge_1.quality);
	if (NULL == compose_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_MAX_QUALITY);
		return;
	}

	//消耗
	shenge_1.Reset();
	shenge_2.Reset();
	shenge_3.Reset();
	m_empty_grids += 3;
	this->SendVirtualBagSingleInfo(virtual_index_1);
	this->SendVirtualBagSingleInfo(virtual_index_2);
	this->SendVirtualBagSingleInfo(virtual_index_3);

	int is_succ = 0;
	ShengeParamData shenge;
	shenge.Reset();
	//检查合成成功率 百分比
	int weight = RandomNum(100);
	if (weight >= compose_cfg->composite_prob)
	{
		//合成失败
		if (SHENGE_SYSTEM_SHENGE_KIND_NORMAL == kind)
		{
			shenge.quality = cur_quality;
			shenge.type = type;

			if (SHENGE_TYPE_SHANBI == shenge.type || SHENGE_TYPE_DIKANG_SHANGHAI == shenge.type)	//	这两种类型不产出
			{
				shenge.type += 1;
			}

			shenge.level = 1;
			m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_COMPOSE_FAILE);
		}
		else if (SHENGE_SYSTEM_SHENGE_KIND_UNLIMIT == kind)
		{
			shenge.quality = cur_quality;
			shenge.type = type;
			shenge.level = 1;
			m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_COMPOSE_FAILE);
		}
	}
	else
	{
		//合成成功
		shenge.quality = next_quality;
		shenge.type = type;
		shenge.level = 1;
		m_role->NoticeNum(noticenum::NT_SHENGE_SYSTEM_COMPOSE_SUCC);
		is_succ = 1;

		const ShengeSystemChoujiagnCfg *reward_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgByQualityAndType(next_quality, shenge.type);
		if (NULL == reward_cfg)
		{
			return;
		}

		if (1 == reward_cfg->is_msg)
		{
			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenge_combine_msg, 
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), reward_cfg->seq);
			if (len > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	//给予奖励
	this->PutShengeToVirtualBag(shenge, "ShengeSystem::ReqComposeShenge");
	this->SendComposeInfo(shenge, is_succ);
	gamelog::g_log_shenge_system.printf(LL_INFO, "ShengeSystem::ReqComposeShenge user[%d %s],   compose_shenge1[quality:%d, type:%d, level:%d] compose_shenge2[quality:%d, type:%d, level:%d]\
												compose_shenge3[quality:%d, type:%d, level:%d] compose_shenge[quality:%d, type:%d, level:%d]",
												m_role->GetUID(), m_role->GetName(), shenge_1.quality, shenge_1.type, shenge_1.level, shenge_2.quality, shenge_2.type, shenge_2.level, shenge_3.quality, shenge_3.type, shenge_3.level, shenge.quality, shenge.type, shenge.level);
}

void ShengeSystem::ReqSetShenge(int virtual_index, int page_index, int grid_index)
{
	if (this->IsInvalidVirtualIndex(virtual_index) || this->IsInvalidPageIndex(page_index) || this->IsInvalidGridIndex(grid_index))
	{
		return;
	}

	ShengeParamData &bag_shenge = m_param.virtual_bag_list[virtual_index];
	ShengeParamData &grid_shenge = m_param.shenge_page_list[page_index].shenge_grid[grid_index];

	if (bag_shenge.IsEmpty())
	{
		return;
	}

	// 终极神格不能装备 到 普通格子
	if (bag_shenge.type >= SHENGE_TYPE_NORMAL_COUNT && grid_index >= SHENGE_SYSTEM_PER_SHENGE_PAGE_MAX_ZHONGJI_SHENGE_COUNT)
	{
		return;
	}

	// 普通神格不能装备 到 终极格子
	if (bag_shenge.type < SHENGE_TYPE_NORMAL_COUNT && grid_index < SHENGE_SYSTEM_PER_SHENGE_PAGE_MAX_ZHONGJI_SHENGE_COUNT)
	{
		return;
	}

	if (grid_shenge.type != bag_shenge.type && this->CheckIsMaxTyptCount(bag_shenge.type, page_index))
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_KIND_MAX_COUNT);
		return;
	}

	ShengeParamData tmp_shenge;
	tmp_shenge.Reset();
	tmp_shenge = grid_shenge;
	grid_shenge = bag_shenge;
	bag_shenge = tmp_shenge;

	if (bag_shenge.IsEmpty())
	{
		++m_empty_grids;
	}

	this->CheckChangeTotalLevel();
	this->SendVirtualBagSingleInfo(virtual_index);
	this->SendShengePageGridSingleInfo(page_index, grid_index);
	this->ReCalShengeEffct(false, true);
	this->ReCalcSuitCount();
	for (int i = 0; i < ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX; ++i)	//检查是否完成大小目标
	{
		const RoleBSGoalItemConfig *cfg = LOGIC_CONFIG->GetRoleBigSmallGoalConfig().GetBSGoalItemConfig(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGE, i);
		if (cfg != nullptr)
		{
			int need_quality = cfg->param_1;						//品质
			int count = this->GetSatisfyMagicalPreciousCount(need_quality);						//满足品质的数量
			m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGE, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_1, grid_shenge.quality, count);
		}
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGE_SYSTEM);
	m_role->NoticeNum(noticenum::NT_SHENGE_SYSTEM_SET_SHENGE_SUCC);

	gamelog::g_log_shenge_system.printf(LL_INFO, " ShengeSystem::ReqSetShenge user[%d %s],  bag_shenge[quality:%d, type:%d, level:%d]  , grid_shenge[quality:%d, type:%d, level:%d]",
		m_role->GetUID(), m_role->GetName(), bag_shenge.quality, bag_shenge.type, bag_shenge.level, grid_shenge.quality, grid_shenge.type, grid_shenge.level);
}

bool ShengeSystem::ReqUpShengeLevel(ShengeParamData &shenge)
{
	const ShengeSystemAttrLevelCfg *shenge_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetShengeSystemCfgByQualityTypeLevel(shenge.quality, shenge.type, shenge.level);
	if (NULL == shenge_cfg)
	{
		return false;
	}

	const ShengeSystemAttrLevelCfg *shenge_next_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetShengeSystemCfgByQualityTypeLevel(shenge.quality, shenge.type, shenge.level + 1);
	if (NULL == shenge_next_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_MAX_LEVEL);
		return false;
	}

	//检查消耗
	int need_score = shenge_cfg->next_level_need_marrow_score;
	if (need_score <= 0 || need_score > this->GetMarrowScore())
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_NOR_ENOUGH_SCORE);
		return false;
	}

	this->AddMarrowScore(-need_score, "ShengeSystem::ReqUpShengeLevel");
	++shenge.level;
	this->ReCalShengeEffct();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGE_SYSTEM);
	m_role->NoticeNum(noticenum::NT_SHENGE_SYSTEM_SHENGE_UP_LEVEL_SUCC);
	this->CheckChangeTotalLevel();

	gamelog::g_log_shenge_system.printf(LL_INFO, "ShengeSystem::ReqUpShengeLevel user[%d %s],  shenge[quality:%d, type:%d, level:%d], dec_score[%d]",
		m_role->GetUID(), m_role->GetName(), shenge.quality, shenge.type, shenge.level, need_score);

	return true;
}

void ShengeSystem::ReqChangeShengePage(int page_index)
{
	if (this->IsInvalidPageIndex(page_index))
	{
		return;
	}

	m_param.using_shenge_page_index = page_index;
	this->ReCalShengeEffct();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGE_SYSTEM);

	this->SendUsingIndex();
}

void ShengeSystem::ReqUnloadShenge(int page_index, int grid_index)
{
	if (this->IsInvalidPageIndex(page_index) || this->IsInvalidGridIndex(grid_index))
	{
		return;
	}

	if (!this->CheckHasSpace())
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_BAG_NO_ENOUGH_SPACE);
		return;
	}

	//拆卸神格
	ShengeParamData shenge = m_param.shenge_page_list[page_index].shenge_grid[grid_index];
	m_param.shenge_page_list[page_index].shenge_grid[grid_index].Reset();

	this->SendShengePageGridSingleInfo(page_index, grid_index);
	this->PutShengeToVirtualBag(shenge, "ShengeSystem::ReqUnloadShenge");

	this->ReCalShengeEffct();
	this->ReCalcSuitCount();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGE_SYSTEM);
	m_role->NoticeNum(noticenum::NT_SHENGE_SYSTEM_SHENGE_UNLOAD_SHENGE_SUCC);
}

void ShengeSystem::ReqClearShengePage(int page_index)
{
	if (this->IsInvalidPageIndex(page_index))
	{
		return;
	}

	int need_empty_count = 0;
	static ShengeParamData shenge_list[SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID];

	for (int i = 0; i < SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID; ++i)
	{
		if (!m_param.shenge_page_list[page_index].shenge_grid[i].IsEmpty())
		{
			shenge_list[need_empty_count++] = m_param.shenge_page_list[page_index].shenge_grid[i];
		}
	}

	if (!this->CheckHasSpace(need_empty_count))
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_BAG_NO_ENOUGH_SPACE);
		return;
	}

	this->PutShengeListToVirtualBag(shenge_list, need_empty_count, "ReqClearShengePage");
	m_param.shenge_page_list[page_index].Reset();

	this->ReCalcSuitCount();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGE_SYSTEM);
	m_role->NoticeNum(noticenum::NT_SHENGE_SYSTEM_SHENGE_PAGE_CLEAR_SUCC);

	this->SendShengePageInfo(page_index);
}

void ShengeSystem::PutShengeListToVirtualBag(const ShengeParamData shenge[], int size, const char *reason)
{
	if (NULL == shenge)
	{
		return;
	}

	gamelog::g_log_shenge_system.buff_printf("reason[%s] user[%d %s] shenge_list(", reason, m_role->GetUID(), m_role->GetName());

	static Protocol::SCShengeSystemBagInfo rsbi;
	rsbi.info_type = Protocol::SHENGE_SYSTEM_INFO_TYPE_SIGLE_CHANGE;
	rsbi.param1 = 0;
	rsbi.count = 0;
	rsbi.param3 = 0;

	int list_count = 0;
	for (int index = 0; index < SHENGE_SYSTEM_BAG_MAX_GRIDS && list_count < size; ++ index)
	{
		ShengeParamData &bag_shenge = m_param.virtual_bag_list[index];
		if (bag_shenge.IsEmpty())
		{
			bag_shenge = shenge[list_count];
			++list_count;
			--m_empty_grids;

			rsbi.bag_list[rsbi.count].quanlity = bag_shenge.quality;
			rsbi.bag_list[rsbi.count].type = bag_shenge.type;
			rsbi.bag_list[rsbi.count].level = static_cast<unsigned char>(bag_shenge.level);
			rsbi.bag_list[rsbi.count].index = index;
			++rsbi.count;

			gamelog::g_log_shenge_system.buff_printf("[quality:%d, type:%d, level:%d],", bag_shenge.quality, bag_shenge.type, bag_shenge.level);
			//m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGE, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_4, bag_shenge.quality, 1);
		}
	}

	gamelog::g_log_shenge_system.buff_printf("),  chou_count[%d]", list_count);
	gamelog::g_log_shenge_system.commit_buff(LL_INFO);

	int length = sizeof(rsbi) - sizeof(rsbi.bag_list[0]) * (SHENGE_SYSTEM_BAG_MAX_GRIDS - rsbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsbi, length);
}

void ShengeSystem::PutShengeToVirtualBag(const ShengeParamData &shenge, const char *reason)
{
	this->PutShengeListToVirtualBag(&shenge, 1, reason);
}

void ShengeSystem::ReqChoujiang(int times)
{
	if (!this->CheckHasSpace(times))
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_BAG_NO_ENOUGH_SPACE);
		return;
	}

	const ShengeSystemOtherCfg &other_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetOterCfg();

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	int need_gold = 0;
	bool is_free_chou = false;
	int total_weight = 0;
	switch (times)
	{
	case SHENGE_SYSTEM_CHOUJIANG_ONE_TIMES:
		{
			need_gold = other_cfg.one_chou_need_gold;
			if (m_param.free_choujiang_times < other_cfg.free_choujiang_times && now_second > m_param.next_free_chou_timestamp)
			{
				++m_param.free_choujiang_times;
				m_param.next_free_chou_timestamp = other_cfg.left_free_chou_cd_s + now_second;
				is_free_chou = true;
			}

			total_weight = LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangTotalWeight();
		}
		break;

	case SHENGE_SYSTEM_CHOUJIANG_TEN_TIMES:
		{
			need_gold = other_cfg.ten_chou_need_gold;
			total_weight = LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangTotalTenWeight();
		}
		break;

	default:
		return;
	}

	if (total_weight <= 0)
	{
		return;
	}

	//检查消耗
	//if (!is_free_chou && (need_gold <= 0 || !m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "ShengeSystem::ReqChoujiang")))
	//{
	//	m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
	//	return;
	//}

	if (!is_free_chou)
	{
		if (other_cfg.can_use_item)
		{
			if (times == SHENGE_SYSTEM_CHOUJIANG_ONE_TIMES)
			{
				if (!m_role->GetKnapsack()->ConsumeItem(other_cfg.replacement_id, other_cfg.consume_replacement, "ShengeSystem::ReqOneChoujiang"))
				{
					if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "ShengeSystem::ReqOneChoujiang"))
					{
						m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
						return;
					}
				}
			}
			else if (times == SHENGE_SYSTEM_CHOUJIANG_TEN_TIMES)
			{
				if (!m_role->GetKnapsack()->ConsumeItem(other_cfg.ten_chou_use_itemid, other_cfg.ten_chou_use_item_num, "ShengeSystem::ReqTenChoujiang"))
				{
					if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "ShengeSystem::ReqTenChoujiang"))
					{
						m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
						return;
					}
				}
			}
		}
		else
		{
			if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "ShengeSystem::ReqChoujiang"))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
		}
	}

	//抽奖
	int reward_count = 0;
	ShengeParamData reward_shenge_list[SHENGE_SYSTEM_CHOUJIANG_TEN_TIMES];
	memset(reward_shenge_list, 0, sizeof(reward_shenge_list));
	for (int chou_times = 0; chou_times < times && reward_count < SHENGE_SYSTEM_CHOUJIANG_TEN_TIMES; ++chou_times)
	{
		if (SHENGE_SYSTEM_CHOUJIANG_TEN_TIMES == times && 0 == chou_times)
		{
			int special_total_weight = LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangTotalTenSpecialWeight();
			if (special_total_weight > 0)
			{
				int specail_weitht = RandomNum(special_total_weight);
				for (int special_seq = 0; special_seq < LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgCount() && special_seq < SHENGE_SYSTEM_CHOUJIANG_MAX_CFG_COUNTS; ++special_seq)
				{
					const ShengeSystemChoujiagnCfg *special_reward_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgBySeq(special_seq);
					if (NULL == special_reward_cfg || SHENGE_SYSTEM_TEN_CHOU_MUST_GET_QUALITY != special_reward_cfg->reward_shenge.quality)
					{
						continue;
					}

					if (specail_weitht < special_reward_cfg->ten_weight)
					{
						LotterySequence replace_special_seq = static_cast<LotterySequence>(special_seq);
						m_role->GetLotteryCount()->ReplaceLotterySequence(LOTTERY_REPLACE_TYPE_SHENGESYS, replace_special_seq);
						const ShengeSystemChoujiagnCfg *check_reward_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgBySeq(replace_special_seq);
						if (NULL == check_reward_cfg || replace_special_seq >= LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgCount() || 
							replace_special_seq >= SHENGE_SYSTEM_CHOUJIANG_MAX_CFG_COUNTS ||
							SHENGE_SYSTEM_TEN_CHOU_MUST_GET_QUALITY != check_reward_cfg->reward_shenge.quality)
						{
							replace_special_seq = special_seq;
						}
						const ShengeSystemChoujiagnCfg *special_reward_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgBySeq(replace_special_seq);
						if (NULL == special_reward_cfg || replace_special_seq >= LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgCount() || 
							replace_special_seq >= SHENGE_SYSTEM_CHOUJIANG_MAX_CFG_COUNTS ||
							SHENGE_SYSTEM_TEN_CHOU_MUST_GET_QUALITY != special_reward_cfg->reward_shenge.quality)
						{
							continue;
						}
						reward_shenge_list[reward_count].quality = static_cast<char>(special_reward_cfg->reward_shenge.quality);
						reward_shenge_list[reward_count].type = static_cast<char>(special_reward_cfg->reward_shenge.type);
						reward_shenge_list[reward_count].level = 1;
						++reward_count;

						if (1 == special_reward_cfg->is_msg)
						{
							int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_chou_jiang_msg, 
								m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), special_reward_cfg->seq);
							if (len > 0) 
							{
								World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE);
							}
						}

						break;
					}
					else
					{
						specail_weitht -= special_reward_cfg->ten_weight;
					}
				}

				continue;
			}
		}

		int weight = RandomNum(total_weight);
		for (int seq = 0; seq < LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgCount() && seq < SHENGE_SYSTEM_CHOUJIANG_MAX_CFG_COUNTS; ++seq)
		{
			const ShengeSystemChoujiagnCfg *reward_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgBySeq(seq);
			if (NULL == reward_cfg)
			{
				continue;
			}

			if (SHENGE_SYSTEM_CHOUJIANG_ONE_TIMES == times)
			{
				if (weight < reward_cfg->weight)
				{
					LotterySequence replace_special_seq = static_cast<LotterySequence>(seq);
					m_role->GetLotteryCount()->ReplaceLotterySequence(LOTTERY_REPLACE_TYPE_SHENGESYS, replace_special_seq);
					const ShengeSystemChoujiagnCfg *check_reward_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgBySeq(replace_special_seq);
					if (NULL == check_reward_cfg ||	replace_special_seq >= LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgCount() || replace_special_seq >= SHENGE_SYSTEM_CHOUJIANG_MAX_CFG_COUNTS)
					{
						replace_special_seq = seq;
					}
					const ShengeSystemChoujiagnCfg *reward_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgBySeq(replace_special_seq);
					if (NULL == reward_cfg || replace_special_seq >= LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgCount() || replace_special_seq >= SHENGE_SYSTEM_CHOUJIANG_MAX_CFG_COUNTS)
					{
						continue;
					}
					reward_shenge_list[reward_count].quality = static_cast<char>(reward_cfg->reward_shenge.quality);
					reward_shenge_list[reward_count].type = static_cast<char>(reward_cfg->reward_shenge.type);
					reward_shenge_list[reward_count].level = 1;
					++reward_count;

					if (1 == reward_cfg->is_msg)
					{
						int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_chou_jiang_msg, 
							m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), reward_cfg->seq);
						if (len > 0) 
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE);
						}
					}

					break;
				}
				else
				{
					weight -= reward_cfg->weight;
				}
			}
			else if (SHENGE_SYSTEM_CHOUJIANG_TEN_TIMES == times)
			{
				if (weight < reward_cfg->ten_weight)
				{
					LotterySequence replace_special_seq = static_cast<LotterySequence>(seq);
					m_role->GetLotteryCount()->ReplaceLotterySequence(LOTTERY_REPLACE_TYPE_SHENGESYS, replace_special_seq);
					const ShengeSystemChoujiagnCfg *check_reward_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgBySeq(replace_special_seq);
					if (NULL == check_reward_cfg || replace_special_seq >= LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgCount() || replace_special_seq >= SHENGE_SYSTEM_CHOUJIANG_MAX_CFG_COUNTS)
					{
						replace_special_seq = seq;
					}
					const ShengeSystemChoujiagnCfg *reward_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgBySeq(replace_special_seq);
					if (NULL == reward_cfg || replace_special_seq >= LOGIC_CONFIG->GetShengesystemCfg().GetChoujiangCfgCount() || replace_special_seq >= SHENGE_SYSTEM_CHOUJIANG_MAX_CFG_COUNTS)
					{
						continue;
					}
					reward_shenge_list[reward_count].quality = static_cast<char>(reward_cfg->reward_shenge.quality);
					reward_shenge_list[reward_count].type = static_cast<char>(reward_cfg->reward_shenge.type);
					reward_shenge_list[reward_count].level = 1;
					++reward_count;

					if (1 == reward_cfg->is_msg)
					{
						int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_chou_jiang_msg, 
							m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), reward_cfg->seq);
						if (len > 0) 
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE);
						}
					}

					break;
				}
				else
				{
					weight -= reward_cfg->ten_weight;
				}
			}
		}
	}

	//检查第一次十抽
	if (SHENGE_SYSTEM_CHOUJIANG_TEN_TIMES == times && 0 == m_param.is_can_get_fisrt_ten_chou)
	{
		m_param.is_can_get_fisrt_ten_chou = 1;
		this->GiveFisrTenCHou(reward_shenge_list, reward_count);
	}

	static Protocol::SCShengeSystemBagInfo rsbi;
	rsbi.info_type = Protocol::SHENGE_SYSTEM_INFO_TYPE_ALL_CHOUJIANG_INFO;
	rsbi.param1 = m_param.free_choujiang_times;
	rsbi.param3 = m_param.next_free_chou_timestamp;
	rsbi.count = 0;

	//给予
	this->PutShengeListToVirtualBag(reward_shenge_list, reward_count, "ShengeSystem::ReqChoujiang");

	for (int count = 0; count < reward_count; ++count)
	{
		rsbi.bag_list[rsbi.count].quanlity = reward_shenge_list[count].quality;
		rsbi.bag_list[rsbi.count].type = reward_shenge_list[count].type;
		rsbi.bag_list[rsbi.count].level = static_cast<unsigned char>(reward_shenge_list[count].level);
		++rsbi.count;
	}

	int length = sizeof(rsbi) - sizeof(rsbi.bag_list[0]) * (SHENGE_SYSTEM_BAG_MAX_GRIDS - rsbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsbi, length);

	this->SendChoujiangInfo();
}

void ShengeSystem::GiveFisrTenCHou(ShengeParamData reward_list[], int size) const
{
	for (int i = 0; i < size; i++)
	{
		if (SHENGE_QUALITY_RED == reward_list[i].quality)
		{
			return;
		}
	}

	ShengeParamData &tmp_shenge = reward_list[RandomNum(size)];
	int weight = RandomNum(LOGIC_CONFIG->GetShengesystemCfg().GetFirstTenChouJiangTotalWeight());
	for (int seq = 0; seq < LOGIC_CONFIG->GetShengesystemCfg().GetFirstTenChouJiangCfgCount(); ++seq)
	{
		const ShengeSystemChoujiagnCfg *reward_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetFirstTenChouJiangCfgBySeq(seq);
		if (NULL == reward_cfg)
		{
			continue;
		}

		if (weight < reward_cfg->weight)
		{
			tmp_shenge.quality = static_cast<char>(reward_cfg->reward_shenge.quality);
			tmp_shenge.type = static_cast<char>(reward_cfg->reward_shenge.type);
			tmp_shenge.level = 1;
			break;
		}

		weight -= reward_cfg->weight;
	}
}

int CompareShengeStruc(const void *arg1, const void *arg2)
{
	ShengeParamData *s1 = (ShengeParamData*)arg1;
	ShengeParamData *s2 = (ShengeParamData*)arg2;

	if (s1->quality > s2->quality)
	{
		return -1;
	}
	else if (s1->quality < s2->quality)
	{
		return 1;
	}
	else if (s1->type < s2->type)
	{
		return -1;
	}
	else if (s1->type > s2->type)
	{
		return 1;
	}
	return s1->level < s2->level;
}

void ShengeSystem::SortVirtrulBag()
{
	m_empty_grids = 0;
	qsort(m_param.virtual_bag_list, SHENGE_SYSTEM_BAG_MAX_GRIDS, sizeof(ShengeParamData), CompareShengeStruc);

	for (int index = SHENGE_SYSTEM_BAG_MAX_GRIDS - 1; index >= 0; --index)
	{
		ShengeParamData &shenge = m_param.virtual_bag_list[index];
		if (!shenge.IsEmpty())
		{
			break;
		}

		++m_empty_grids;
	}
	this->SendVirtualBagAllInfo();
}

bool ShengeSystem::AddMarrowScore(int score, const char *reason)
{
	long long tmp_score = m_param.marrow_score;
	tmp_score += score;
	if (tmp_score > INT_MAX)
	{
		tmp_score = INT_MAX;
	}

	m_param.marrow_score = static_cast<int>(tmp_score);

	this->SendMarrowScoreInfo();

	gamelog::g_log_shenge_system.printf(LL_INFO, "ShengeSystem::AddMarrowScore user[%d %s], reason[%s], score[%d]",
		m_role->GetUID(), m_role->GetName(), reason, score);

	return true;
}

bool ShengeSystem::CheckCanActiveCombinationBySeq(int seq)
{
	const ShengeSystemCombinationCfg *combination_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetCombinationCfgBySeq(seq);
	if (NULL == combination_cfg)
	{
		return false;
	}

	int type_list[SHENGE_TYPE_COUNT];
	memcpy(type_list, combination_cfg->type, sizeof(type_list));

	int using_page = static_cast<int>(m_param.using_shenge_page_index);
	if (using_page < 0 || using_page > SHENGE_SYSTEM_MAX_SHENGE_PAGE)
	{
		return false;
	}

	//检查神格类型数量
	for (int page = 0; page < SHENGE_SYSTEM_CUR_SHENGE_PAGE; page++)
	{
		for (int index = 0; index < SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID; ++ index)
		{
			ShengeParamData &shenge = m_param.shenge_page_list[page].shenge_grid[index];
			if (shenge.type >= 0 && shenge.type < SHENGE_TYPE_COUNT && shenge.quality >= combination_cfg->quality[static_cast<int>(shenge.type)])
			{
				--type_list[static_cast<int>(shenge.type)];
			}
		}
	}

	//检查是否满足数量
	for (int type = SHENGE_TYPE_HP; type < SHENGE_TYPE_COUNT; ++type)
	{
		if (type_list[type] > 0)
		{
			return false;
		}
	}

	return true;
}

bool ShengeSystem::TransformAttrType(int type, int &real_type)
{
	switch (type)
	{
	case SHENGE_SYSTEM_ATTR_TYPE_MAXHP:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP;
		}
		break;

	case SHENGE_SYSTEM_ATTR_TYPE_GONGJI:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI;
		}
		break;

	case SHENGE_SYSTEM_ATTR_TYPE_FANGYU:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU;
		}
		break;

	case SHENGE_SYSTEM_ATTR_TYPE_ADD_SHANGHAI:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI;
		}
		break;

	case SHENGE_SYSTEM_ATTR_TYPE_DIKANG_SHANGHAI:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN;
		}
		break;

	case SHENGE_SYSTEM_ATTR_TYPE_SPEED:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED;
		}
		break;

	case SHENGE_SYSTEM_ATTR_TYPE_BAOJI_FUJIA_SHANGHAI:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG;
		}
		break;

	case SHENGE_SYSTEM_ATTR_TYPE_SHANBI:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool ShengeSystem::CheckIsMaxTyptCount(int type, int page_index)
{
	if (this->IsInvalidPageIndex(page_index) || type < 0 || type >= SHENGE_TYPE_COUNT)
	{
		return true;
	}

	const ShengeSystemOtherCfg &other_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetOterCfg();
	int max_count = other_cfg.normal_shenge_type_max_count;
	if (type >= SHENGE_TYPE_NORMAL_COUNT)
	{
		max_count = other_cfg.ultimate_shenge_type_max_count;
	}

	for (int index = 0; index < SHENGE_SYSTEM_MAX_SHENGE_GRID; ++index)
	{
		ShengeParamData &shenge = m_param.shenge_page_list[static_cast<int>(m_param.using_shenge_page_index)].shenge_grid[index];

		if (shenge.type == type)
		{
			--max_count;
		}
	}

	return max_count <= 0;
}

bool ShengeSystem::CheckIsActiveSpecialEffect(int effect_type)
{
	std::map<short, double>::iterator it_map = m_special_buff_list.find(effect_type);
	return it_map != m_special_buff_list.end();
}

void ShengeSystem::ActiveSpecialEffectByShengeType(int type, double value)
{
	switch (type)
	{

	case SHENGE_SYSTEM_ATTR_TYPE_ADD_EXP:
		{
			this->ActiveSpecialEffect(SHENGE_SPECIAL_EFFECT_TYPE_ADD_EXP, value);
		}
		break;

	case SHENGE_SYSTEM_ATTR_TYPE_ADD_ITEM_DROP:
		{
			this->ActiveSpecialEffect(SHENGE_SPECIAL_EFFECT_TYPE_ADD_ITEM_DROP, value);
		}
		break;

	default:
		break;
	}
}

void ShengeSystem::ActiveSpecialEffect(int effect_type, double value)
{
	if (effect_type < SHENGE_SPECIAL_EFFECT_TYPE_ADD_EXP || effect_type > SHENGE_SPECIAL_EFFECT_TYPE_COUNT || value <= 0)
	{
		return;
	}

	m_special_buff_list[static_cast<short>(effect_type)] += value / SPECIAL_EFFECT_ADD_PERCENT;
}

void ShengeSystem::NotActiveSpecialEffect(int effect_type)
{
	std::map<short, double>::iterator it_map = m_special_buff_list.find(effect_type);
	if (it_map != m_special_buff_list.end())
	{
		m_special_buff_list.erase(it_map);
	}
}

void ShengeSystem::SendVirtualBagSingleInfo(int virtual_index)
{
	if (this->IsInvalidVirtualIndex(virtual_index))
	{
		return;
	}

	static Protocol::SCShengeSystemBagInfo rsbi;
	rsbi.info_type = Protocol::SHENGE_SYSTEM_INFO_TYPE_SIGLE_CHANGE;
	rsbi.param1 = 0;
	rsbi.count = 1;
	rsbi.param3 = 0;

	ShengeParamData &shenge = m_param.virtual_bag_list[virtual_index];
	rsbi.bag_list[0].quanlity = shenge.quality;
	rsbi.bag_list[0].type = shenge.type;
	rsbi.bag_list[0].level = static_cast<unsigned char>(shenge.level);
	rsbi.bag_list[0].index = virtual_index;

	int length = sizeof(rsbi) - sizeof(rsbi.bag_list[0]) * (SHENGE_SYSTEM_BAG_MAX_GRIDS - rsbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsbi, length);
}

void ShengeSystem::SendShengePageGridSingleInfo(int page_index, int grid_index)
{
	if (this->IsInvalidPageIndex(page_index) , this->IsInvalidGridIndex(grid_index))
	{
		return;
	}

	static Protocol::SCShengeSystemBagInfo rsbi;
	rsbi.info_type = Protocol::SHENGE_SYSTEM_INFO_TYPE_SHENGE_INFO;
	rsbi.param1 = page_index;
	rsbi.count = 1;
	rsbi.param3 = m_param.history_set_total_level[page_index];


	ShengeParamData &shenge = m_param.shenge_page_list[page_index].shenge_grid[grid_index];
	rsbi.bag_list[0].quanlity = shenge.quality;
	rsbi.bag_list[0].type = shenge.type;
	rsbi.bag_list[0].index = grid_index;
	rsbi.bag_list[0].level = static_cast<unsigned char>(shenge.level);

	int length = sizeof(rsbi) - sizeof(rsbi.bag_list[0]) * (SHENGE_SYSTEM_BAG_MAX_GRIDS - rsbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsbi, length);
}

void ShengeSystem::SendVirtualBagAllInfo()
{
	static Protocol::SCShengeSystemBagInfo rsbi;
	rsbi.info_type = Protocol::SHENGE_SYSTEM_INFO_TYPE_ALL_BAG_INFO;
	rsbi.param1 = m_param.using_shenge_page_index;
	rsbi.count = 0;
	rsbi.param3 = 0;

	for (int index = 0; index < SHENGE_SYSTEM_BAG_MAX_GRIDS; ++ index)
	{
		ShengeParamData &shenge = m_param.virtual_bag_list[index];
		rsbi.bag_list[rsbi.count].quanlity = shenge.quality;
		rsbi.bag_list[rsbi.count].type = shenge.type;
		rsbi.bag_list[rsbi.count].level = static_cast<unsigned char>(shenge.level);
		rsbi.bag_list[rsbi.count].index = static_cast<unsigned char>(index);
		++rsbi.count;
	}

	int length = sizeof(rsbi) - sizeof(rsbi.bag_list[0]) * (SHENGE_SYSTEM_BAG_MAX_GRIDS - rsbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsbi, length);
}

void ShengeSystem::SendShengePageInfo(int page_index)
{
	if (this->IsInvalidPageIndex(page_index))
	{
		return;
	}

	static Protocol::SCShengeSystemBagInfo rsbi;
	rsbi.info_type = Protocol::SHENGE_SYSTEM_INFO_TYPE_ALL_SHENGE_INFO;
	rsbi.param1 = page_index;
	rsbi.count = 0;
	rsbi.param3 = m_param.history_set_total_level[page_index];

	for (int index = 0; index < SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID; ++index)
	{
		ShengeParamData &shenge = m_param.shenge_page_list[page_index].shenge_grid[index];
		rsbi.bag_list[rsbi.count].quanlity = shenge.quality;
		rsbi.bag_list[rsbi.count].type = shenge.type;
		rsbi.bag_list[rsbi.count].level = static_cast<unsigned char>(shenge.level);
		rsbi.bag_list[rsbi.count].index = static_cast<unsigned char>(index);
		++rsbi.count;
	}

	int length = sizeof(rsbi) - sizeof(rsbi.bag_list[0]) * (SHENGE_SYSTEM_BAG_MAX_GRIDS - rsbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsbi, length);
}

void ShengeSystem::SendShengePageGridAllInfo()
{
	for (int index = 0; index < SHENGE_SYSTEM_CUR_SHENGE_PAGE; ++ index)
	{
		this->SendShengePageInfo(index);
	}
}

void ShengeSystem::SendMarrowScoreInfo() const
{
	static Protocol::SCShengeSystemBagInfo rsbi;
	rsbi.info_type = Protocol::SHENGE_SYSTEM_INFO_TYPE_ALL_MARROW_SCORE_INFO;
	rsbi.param1 = 0;
	rsbi.count = 0;
	rsbi.param3 = m_param.marrow_score;

	int length = sizeof(rsbi) - sizeof(rsbi.bag_list[0]) * (SHENGE_SYSTEM_BAG_MAX_GRIDS - rsbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsbi, length);
}

void ShengeSystem::SendUsingIndex() const
{
	static Protocol::SCShengeSystemBagInfo rsbi;
	rsbi.info_type = Protocol::SHENGE_SYSTEM_INFO_TYPE_USING_PAGE_INDEX;
	rsbi.param1 = m_param.using_shenge_page_index;
	rsbi.count = 0;
	rsbi.param3 = 0;

	int length = sizeof(rsbi) - sizeof(rsbi.bag_list[0]) * (SHENGE_SYSTEM_BAG_MAX_GRIDS - rsbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsbi, length);
}

void ShengeSystem::SendComposeInfo(ShengeParamData &shenge, int is_succ) const
{
	static Protocol::SCShengeSystemBagInfo rsbi;
	rsbi.info_type = Protocol::SHENGE_SYSTEM_INFO_TYPE_COMPOSE_SHENGE_INFO;
	rsbi.param1 = is_succ;
	rsbi.count = 1;
	rsbi.param3 = 0;

	rsbi.bag_list[0].quanlity = shenge.quality;
	rsbi.bag_list[0].type = shenge.type;
	rsbi.bag_list[0].level = static_cast<unsigned char>(shenge.level);

	int length = sizeof(rsbi) - sizeof(rsbi.bag_list[0]) * (SHENGE_SYSTEM_BAG_MAX_GRIDS - rsbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsbi, length);
}

void ShengeSystem::SendActiveCombinationInfo() const
{
	static Protocol::SCShengeSystemBagInfo rsbi;
	rsbi.info_type = Protocol::SHENGE_SYSTEM_INFO_TYPE_ACTIVE_COMBINE_INFO;

	rsbi.param1 = 0;
	for (int seq = 0; seq < SHENGE_SYSTEM_COMBINATION_MAX_CFG_COUNTS; seq++)
	{
		if (1 == m_combine_seq_list[seq])
		{
			rsbi.param1 |= (1 << seq);
		}
	}

	rsbi.count = 0;
	rsbi.param3 = 0;

	int length = sizeof(rsbi) - sizeof(rsbi.bag_list[0]) * (SHENGE_SYSTEM_BAG_MAX_GRIDS - rsbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsbi, length);
}

void ShengeSystem::SendChoujiangInfo() const
{
	static Protocol::SCShengeSystemBagInfo rsbi;
	rsbi.info_type = Protocol::SHENGE_SYSTEM_INFO_TYPE_CHOUJIANG_INFO;
	rsbi.param1 = m_param.free_choujiang_times;
	rsbi.count = 0;
	rsbi.param3 = m_param.next_free_chou_timestamp;

	int length = sizeof(rsbi) - sizeof(rsbi.bag_list[0]) * (SHENGE_SYSTEM_BAG_MAX_GRIDS - rsbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsbi, length);
}

void ShengeSystem::SendSingleXilianInfo(int shenqu_id)
{
	if (shenqu_id < 0 || shenqu_id > SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM) return;

	static Protocol::SCShengeShenquInfo ssi;

	ssi.shenqu_id = shenqu_id;
	for (int attr_type = 0; attr_type < SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM; ++ attr_type)
	{
		for (int slot_index = 0; slot_index < SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM; ++ slot_index)
		{
			AttrInfo &attr_info = m_param.shenqu_list[shenqu_id].shenqu_attr_list[attr_type].attr_info[slot_index];
			ssi.shenqu_attr_list[attr_type].attr_info[slot_index].attr_point = attr_info.attr_point;
			ssi.shenqu_attr_list[attr_type].attr_info[slot_index].attr_value = attr_info.attr_value;
			//ssi.shenqu_attr_list[attr_type].attr_info[slot_index].reserve_sh = attr_info.reserve_sh;
			ssi.shenqu_attr_list[attr_type].attr_info[slot_index].qianghua_times = attr_info.qianghua_times;
		}
	}
	ssi.shenqu_history_max_cap = m_param.shenqu_history_cap_list[shenqu_id];

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ssi, sizeof(ssi));
}

void ShengeSystem::SendXilianAllInfo()
{
	static Protocol::SCShengeShenquAllInfo ssai;

	UNSTD_STATIC_CHECK(sizeof(ssai.shenqu_list) == sizeof(m_param.shenqu_list));
	memcpy(ssai.shenqu_list, m_param.shenqu_list, sizeof(ssai.shenqu_list));

	for (int shenqu_id = 0; shenqu_id < SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM; ++ shenqu_id)
	{
		ssai.shenqu_history_max_cap[shenqu_id] = m_param.shenqu_history_cap_list[shenqu_id];
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ssai, sizeof(ssai));
}

void ShengeSystem::CheckChangeTotalLevel()
{
	int old_total_page_level = 0;
	int total_page_level = 0;

	for (int page = 0; page < SHENGE_SYSTEM_MAX_SHENGE_PAGE; page ++)
	{
		old_total_page_level += m_param.history_set_total_level[page];

		int cur_total_level = 0;
		for (int index = 0; index < SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID; ++index)
		{
			ShengeParamData &shenge = m_param.shenge_page_list[page].shenge_grid[index];
			cur_total_level += shenge.level;
		}

		if (cur_total_level > m_param.history_set_total_level[page])
		{
			m_param.history_set_total_level[page] = cur_total_level;
		}
		
		total_page_level += m_param.history_set_total_level[page];
	}

	{
		// 传闻
		if (old_total_page_level < total_page_level)
		{
			int count = LOGIC_CONFIG->GetShengesystemCfg().IsBroadcast(total_page_level);
			if (count > 0)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenge_total_level,
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), total_page_level);
				if (length > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}

			const ShengeSystemPageCfg page_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetShengePageCfg(total_page_level);
			if (page_cfg.is_broadcast)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenge_active_new_page,
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), total_page_level, page_cfg.page + 1);
				if (length > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}

	this->SendShengePageGridAllInfo();
}

int ShengeSystem::GetZhangkongRandGrid()
{
	int max_weight = 0;//权重和
	std::vector<std::pair<int, int>> gird_rand_weight_list;//可以升级的id， 和权重
	for (int i = 0; i < static_cast<int>(static_array_size(m_param.zhangkong_level_list)); ++i)
	{
		ZhangkongLevelCfg::ConfigItem * zhangkong_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetZhangkongCfgByGridAndLevel(i, m_param.zhangkong_level_list[i].level + 1);
		if (NULL == zhangkong_cfg) continue;
		int weight = LOGIC_CONFIG->GetShengesystemCfg().GetZhangkongGridRandWeight(i);
		if (weight <= 0)
		{
			continue;
		}
		gird_rand_weight_list.push_back(std::make_pair(i, weight));
		max_weight += weight;
	}

	if (max_weight <= 0)
	{
		return -1;
	}

	int rand_value = RandomNum(max_weight);
	//根据权重随机取ID
	auto it = std::find_if(gird_rand_weight_list.begin(), gird_rand_weight_list.end(),
		[&](decltype(gird_rand_weight_list)::value_type& value)->bool {
		if (rand_value < value.second)
		{
			return true;
		}
		rand_value -= value.second;
		return false;
	});

	if (it == gird_rand_weight_list.end())
	{//it 一定不等于 end 
		return -1;
	}

	//返回id;
	return it->first;
}

void ShengeSystem::ReqUpLevelZhangkong(int type)
{
	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	int times = 1;
	if (1 == type)
	{
		times = 10;
	}

	int gird = -1;
	int exp = 0;
	for (int i = 0; i < times; i++)
	{
		exp = LOGIC_CONFIG->GetShengesystemCfg().GetZhangkongRandExp();

		/*for (int r = 0; r < 1000; r++)
		{	
			gird = LOGIC_CONFIG->GetShengesystemCfg().GetZhangkongRandGrid();

			if (gird < 0 || gird >= SHENGE_SYSTEM_QUALITY_MAX_VALUE) return;
			if (m_param.zhangkong_level_list[gird].level < SHENGE_SYSTEM_ZHANGKONG_MAX_LEVEL)
			{
				break;
			}
		}*/

		gird = GetZhangkongRandGrid();
		
		if (gird < 0 || gird >= SHENGE_SYSTEM_QUALITY_MAX_VALUE)
		{
			m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_ZHANGKONG_FULL_LEVEL);
			return;	
		}

		ZhangkongLevelCfg::ConfigItem * zhangkong_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetZhangkongCfgByGridAndLevel(gird, m_param.zhangkong_level_list[gird].level + 1);
		if (NULL == zhangkong_cfg) continue;
		
		const ShengeSystemOtherCfg &other_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetOterCfg();
		static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

		{
			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
			stuff_list[0].item_id = other_cfg.uplevel_zhangkong_itemid;
			stuff_list[0].num = 1;
			stuff_list[0].buyable = false;

			if (ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, false))
			{
				// 消耗物品
				if (consume_list.count > 0 && !m_role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "ReqUpLevelZhangkong"))
				{
					return;
				}
			}
			else
			{
				// 消耗元宝
				if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.uplevel_zhangkong_gold, "ReqUpLevelZhangkong"))
				{
					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}
			}
		}

		m_param.zhangkong_level_list[gird].exp += exp;
		for (int loop_time = 0; loop_time < 100 && NULL != zhangkong_cfg; ++loop_time)
		{
			if (m_param.zhangkong_level_list[gird].exp >= zhangkong_cfg->exp)
			{
				m_param.zhangkong_level_list[gird].exp -= zhangkong_cfg->exp;
				m_param.zhangkong_level_list[gird].level++;
				m_up_zhangkong_level_notice = noticenum::NT_SHENGE_SYSTEM_ZHANGKONG_UPLEVEL_SUCC;
				if (1 == zhangkong_cfg->is_broadcast)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenge_zhangkong_system_msg,
						m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), gird, zhangkong_cfg->grade + 1);
					if (length > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
					}
				}

				zhangkong_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetZhangkongCfgByGridAndLevel(gird, m_param.zhangkong_level_list[gird].level + 1);
			}
			else
			{
				break;
			}
		}
	}
	if (gird < 0 || gird >= SHENGE_SYSTEM_QUALITY_MAX_VALUE || exp <= 0) return;
	this->SendZhangkongSignInfo(gird, m_param.zhangkong_level_list[gird].level, m_param.zhangkong_level_list[gird].exp, exp);
	if (type == 1)
	{
		UpZhangkongLevelRet();
		SendZhangkongInfo();
	}
}

void ShengeSystem::UpZhangkongLevelRet()
{
	if (m_up_zhangkong_level_notice != 0)
	{
		m_role->NoticeNum(m_up_zhangkong_level_notice);
		m_up_zhangkong_level_notice = 0;
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGE_SYSTEM);
}

void ShengeSystem::SendZhangkongInfo()
{
	static Protocol::SCShengeZhangkongInfo pro;

	for (int i = 0; i < SHENGE_SYSTEM_QUALITY_MAX_VALUE; i++)
	{
		pro.zhangkong_list[i].exp = m_param.zhangkong_level_list[i].exp;
		pro.zhangkong_list[i].level = m_param.zhangkong_level_list[i].level;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void ShengeSystem::SendZhangkongSignInfo(int grid, int level, int exp, int add_exp)
{
	static Protocol::SCZhangkongSingleChange pro;

	pro.grid = grid;
	pro.level = level;
	pro.exp = exp;
	pro.add_exp = add_exp;
	
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

int ShengeSystem::GetRealType(int vutual_type)
{
	int real_attr_type = -1;
	switch (vutual_type)
	{
	case SHENGE_SYSTEM_XILIAN_ATTR_TYPE_MAXHP:
	{
		real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP;
	}
	break;

	case SHENGE_SYSTEM_XILIAN_ATTR_TYPE_GONGJI:
	{
		real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI;
	}
	break;

	case SHENGE_SYSTEM_XILIAN_ATTR_TYPE_FANGYU:
	{
		real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU;
	}
	break;

	case SHENGE_SYSTEM_XILIAN_ATTR_TYPE_BAOJI_FUJIA_SHANGHAI:
	{
		real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG;
	}
	break;

	case SHENGE_SYSTEM_XILIAN_ATTR_TYPE_SHANBI:
	{
		real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI;
	}
	break;

	case SHENGE_SYSTEM_XILIAN_ATTR_TYPE_ADD_SHANGHAI:
	{
		real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI;
	}
	break;

	case SHENGE_SYSTEM_XILIAN_ATTR_TYPE_DIKANG_SHANGHAI:
	{
		real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN;
	}
	break;

	default:
		break;
	}

	return real_attr_type;
}

int ShengeSystem::GetXilianStuffNum(int shenqu_id, int select_slot_count)
{
	if (shenqu_id < 0 || shenqu_id >= SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM) return -1;

	if (select_slot_count <= 0 || select_slot_count > SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM) return -1;

	int stuff_num = 0;
	const ShengeSystemShenquCfg *shenqu_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetShenquCfg(shenqu_id);
	if (NULL != shenqu_cfg)
	{
		stuff_num = shenqu_cfg->stuff_num_list[select_slot_count - 1].stuff_num;
	}

	return stuff_num;
}

int ShengeSystem::GetShenquPerfectAttrNum(int shenqu_id)
{
	if (shenqu_id < 0 || shenqu_id >= SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM) return -1;

	int shenqu_perfect_attr_num = 0;
	for (int attr_point = 0; attr_point < SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM; ++ attr_point)
	{
		int perfect_attr_num = 0;
		for (int slot_index = 0; slot_index < SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM; ++ slot_index)
		{
			AttrInfo &attr_info = m_param.shenqu_list[shenqu_id].shenqu_attr_list[attr_point].attr_info[slot_index];
			if (attr_info.attr_point == attr_point)
			{
				++ perfect_attr_num;
			}
		}

		if (perfect_attr_num == SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM)
		{
			++ shenqu_perfect_attr_num;
		}
	}

	return shenqu_perfect_attr_num;
}

void ShengeSystem::CheckPerfectAttr(int shenqu_id, int point_type)
{
	if (shenqu_id < 0 || shenqu_id >= SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM) return;

	if (point_type < 0 || point_type >= SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM) return;

	int perfect_attr_num = 0;
	for(int slot_index = 0; slot_index < SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM; ++ slot_index)
	{
		AttrInfo &attr_info = m_param.shenqu_list[shenqu_id].shenqu_attr_list[point_type].attr_info[slot_index];
		if (attr_info.attr_point == point_type)
		{
			++ perfect_attr_num;
		}
	}

	if (perfect_attr_num == SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenge_xilian_system_msg,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}
}

void ShengeSystem::Strengthen(int shenqu_id, int point_type, bool is_auto, unsigned int select_slot)
{
	if (shenqu_id < 0 || shenqu_id >= SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM) return;

	if (point_type < 0 || point_type >= SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM) return;

	if (select_slot < 0) return;

	int select_slot_count = 0;
	for (int i = 0; i < SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM; ++ i)
	{
		if (0 != (select_slot & (1 << i)))
		{
			++ select_slot_count;
		}
	}

	int stuff_num = this->GetXilianStuffNum(shenqu_id, select_slot_count);
	if (stuff_num <= 0)
	{
		return;
	}

	const ShengeSystemShenquCfg *shenqu_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetShenquCfg(shenqu_id);
	if (NULL == shenqu_cfg) return;

	if (m_role->GetLevel() < shenqu_cfg->role_level)
	{
		m_role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
		return;
	}

	if (shenqu_id >= 1 && shenqu_id < SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM)
	{
		if (m_param.shenqu_history_cap_list[shenqu_id - 1] < shenqu_cfg->fighting_capacity)
		{
			m_role->NoticeNum(errornum::EN_SHENQU_XILIAN_CAP_NOT_ENOUGH);
			return;
		}
	}

	const ShengeSystemXilianAttrCfg *strengthen_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetXilianCfg(shenqu_id, point_type);
	if (NULL == strengthen_cfg)	return;

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	{
		// 升级材料
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = shenqu_cfg->stuff_id;
		stuff_list[0].num = stuff_num;
		stuff_list[0].buyable = is_auto;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
		{
			return;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "ShengeSystem::Strengthen"))
	{
		return;
	}

	for (int slot_index = 0; slot_index < SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM; ++ slot_index)
	{
		if (0 == (select_slot & (1 << slot_index)))
		{
			continue;
		}
		AttrInfo &attr_info = m_param.shenqu_list[shenqu_id].shenqu_attr_list[point_type].attr_info[slot_index];

		int rand_num = RandomNum(strengthen_cfg->total_weight);
		int value_type = 0;
		for (; value_type < SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM; ++value_type)
		{
			if (rand_num < strengthen_cfg->attr_list[value_type].weight)	break;
			rand_num -= strengthen_cfg->attr_list[value_type].weight;
		}

		++attr_info.qianghua_times;
		attr_info.attr_point = value_type;

		const ShengeSystemXilianAttrCfg::AttrInfo &strengthen_value = strengthen_cfg->attr_list[value_type];
		//修改规则，如果类型相同直接满属性
		if (point_type == value_type)
		{
			attr_info.attr_value = strengthen_value.value_max;
		}
		else
		{
			int qianghua_count = attr_info.qianghua_times;
			int baodi_index = -1;
			if (qianghua_count > 0 && qianghua_count > strengthen_cfg->baodi_count_cfg[SHENGE_SYSTEM_XILIAN_BAODI_MAX_COUNT - 1].baodi_count)
			{
				baodi_index = SHENGE_SYSTEM_XILIAN_BAODI_MAX_COUNT - 1;
			}
			else
			{
				for (int i = 0; i < SHENGE_SYSTEM_XILIAN_BAODI_MAX_COUNT; ++i)
				{
					if (qianghua_count <= strengthen_cfg->baodi_count_cfg[i].baodi_count)
					{
						baodi_index = i;
						break;
					}
				}
			}

			if (baodi_index >= 0 && baodi_index < SHENGE_SYSTEM_XILIAN_BAODI_MAX_COUNT)
			{
				int add_val = static_cast<int>((strengthen_value.value_max - strengthen_value.value_min) * strengthen_cfg->baodi_count_cfg[baodi_index].criti_per / 100.0f);
				int value_max = add_val + strengthen_value.value_min;
				attr_info.attr_value = RandomNum(strengthen_value.value_min, value_max);
			}
		}
	}

	//this->CheckPerfectAttr(shenqu_id, point_type);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGE_SYSTEM);
	this->SendSingleXilianInfo(shenqu_id);
}

void ShengeSystem::ReqUpLevelGrid(int index, short is_auto)
{
	if (index < 0 || index >= SHENGE_SYSTEM_QUALITY_MAX_VALUE)
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_ZHANGKONG_FULL_LEVEL);
		return;
	}

	const UplevelGridCfg * item_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetUplevelGridCfg(index);
	if (NULL == item_cfg)
	{
		return;
	}

	ZhangkongLevelCfg::ConfigItem * zhangkong_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetZhangkongCfgByGridAndLevel(index, m_param.zhangkong_level_list[index].level + 1);
	if (NULL == zhangkong_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_ZHANGKONG_FULL_LEVEL);
		return;
	}

	int add_exp = 0;

	if (!is_auto)
	{
		if (!m_role->GetKnapsack()->ConsumeItem(item_cfg->stuff_id, 1, "UplevelGrid"))
		{
			m_role->NoticeNum(errornum::EN_ELEMENT_HEART_ITEM_NOT_ENOGH);
			return;
		}
		
		add_exp += 1;
	}
	else
	{
		int consume_num = 0;

		if (!m_role->GetKnapsack()->HasItem(item_cfg->stuff_id))
		{
			m_role->NoticeNum(errornum::EN_ELEMENT_HEART_ITEM_NOT_ENOGH);
			return;
		}

		int num = m_role->GetKnapsack()->GetItemNum(item_cfg->stuff_id);

		if (m_param.zhangkong_level_list[index].exp < zhangkong_cfg->exp)
		{
			consume_num = zhangkong_cfg->exp - m_param.zhangkong_level_list[index].exp;
		}

		if (num < consume_num)
		{
			consume_num = num;
		}

		if (!m_role->GetKnapsack()->ConsumeItem(item_cfg->stuff_id, consume_num, "Auto_UplevelGrid"))
		{
			m_role->NoticeNum(errornum::EN_ELEMENT_HEART_ITEM_NOT_ENOGH);
			return;
		}

		add_exp += consume_num;
	}

	//if (add_exp > 0)
	//{
	//	m_param.zhangkong_level_list[index].exp += add_exp;

	//	for (int loop_time = 0; loop_time < 100 && NULL != zhangkong_cfg; ++loop_time)
	//	{
	//		if (m_param.zhangkong_level_list[index].exp >= zhangkong_cfg->exp)
	//		{
	//			m_param.zhangkong_level_list[index].exp -= zhangkong_cfg->exp;
	//			m_param.zhangkong_level_list[index].level++;
	//			m_up_zhangkong_level_notice = noticenum::NT_SHENGE_SYSTEM_ZHANGKONG_UPLEVEL_SUCC;
	//			if (1 == zhangkong_cfg->is_broadcast)
	//			{
	//				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenge_zhangkong_system_msg,
	//					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), index, zhangkong_cfg->grade + 1);
	//				if (length > 0)
	//				{
	//					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
	//				}
	//			}

	//			zhangkong_cfg = LOGIC_CONFIG->GetShengesystemCfg().GetZhangkongCfgByGridAndLevel(index, m_param.zhangkong_level_list[index].level + 1);

	//			if (zhangkong_cfg == NULL)
	//			{
	//				m_role->NoticeNum(errornum::EN_SHENGE_SYSTEM_ZHANGKONG_FULL_LEVEL);
	//			}
	//		}
	//		else
	//		{
	//			break;
	//		}
	//	}
	//}

	if (add_exp > 0)
	{
		m_param.zhangkong_level_list[index].exp += add_exp;
		if (m_param.zhangkong_level_list[index].exp >= zhangkong_cfg->exp)
		{
			m_param.zhangkong_level_list[index].exp -= zhangkong_cfg->exp;
			m_param.zhangkong_level_list[index].level++;
			m_up_zhangkong_level_notice = noticenum::NT_SHENGE_SYSTEM_ZHANGKONG_UPLEVEL_SUCC;
			if (1 == zhangkong_cfg->is_broadcast)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenge_zhangkong_system_msg,
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), index, zhangkong_cfg->grade + 1);
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}

	if (index < 0 || index >= SHENGE_SYSTEM_QUALITY_MAX_VALUE || add_exp <= 0) return;
	this->SendZhangkongSignInfo(index, m_param.zhangkong_level_list[index].level, m_param.zhangkong_level_list[index].exp, add_exp);

	this->UpZhangkongLevelRet();
	this->SendZhangkongInfo();
}

void ShengeSystem::OnRoleLevelChange(int level)
{
	this->CheckFuncOpen();
}

void ShengeSystem::OnCompleteTask(TaskID task_id)
{
	this->CheckFuncOpen();
}

int ShengeSystem::GetSatisfyMagicalPreciousCount(int order)
{
	int suit_count = 0;

	for (int page_index = 0; page_index < SHENGE_SYSTEM_CUR_SHENGE_PAGE; ++page_index)
	{
		for (int grid_index = 0; grid_index < SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID; ++grid_index)
		{
			const ShengeParamData &shenge = m_param.shenge_page_list[page_index].shenge_grid[grid_index];
			if (shenge.quality >= order)
			{
				++suit_count;
			}
		}
	}

	return suit_count;
}

void ShengeSystem::OnRoleLogin()
{
	//this->SortVirtrulBag();
	//this->ReCalShengeEffct(true);

	this->CheckFuncOpen();

}

void ShengeSystem::CheckFuncOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (m_role->GetRoleBigSmallGoal()->IsFuncActive(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGE))
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENGE);
	if (fun_open_cfg != nullptr && opengame_day >= fun_open_cfg->opengame_day)
	{
		if (m_role->GetLevel() >= fun_open_cfg->role_level || m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
		{
			m_role->GetRoleBigSmallGoal()->OnOpenFunc(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGE);
		}
	}
}

void ShengeSystem::ReCalcSuitCount()
{
	memset(m_quality_equip_count, 0, sizeof(m_quality_equip_count));

	for (int page_index = 0; page_index < SHENGE_SYSTEM_CUR_SHENGE_PAGE; ++page_index)
	{
		for (int grid_index = 0; grid_index < SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID; ++grid_index)
		{
			const ShengeParamData &shenge = m_param.shenge_page_list[page_index].shenge_grid[grid_index];
			if (shenge.IsEmpty())
			{
				continue;
			}

			size_t quality = shenge.quality;
			if (quality > 0 && quality < SHENGE_QUALITY_COUNT)
			{
				//++m_quality_equip_count[shenge.quality];

				if (quality == SHENGE_QUALITY_PING)
				{
					++m_quality_equip_count[SHENGE_QUALITY_GOLD];		//策划需求，最高品质装备可替代次一品质装备计算套装
				}
				else
				{
					++m_quality_equip_count[quality];
				}
			}
		}
	}

	this->SendSuitInfo();
}

void ShengeSystem::SendSuitInfo()
{
	static Protocol::SCShengeSuitInfo suit_info;
	memset(suit_info.shenge_quality_info, 0, sizeof(suit_info.shenge_quality_info));

	memcpy(suit_info.shenge_quality_info, m_quality_equip_count, sizeof(suit_info.shenge_quality_info));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&suit_info, sizeof(suit_info));
}