//#include "stdafx.h"

#include "shengqi.hpp"
#include "other/shengqi/shengqiconfig.hpp"
#include "obj/character/role.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "gameworld/gameworld/gstr.h"
#include "gamelog.h"
#include "config/logicconfigmanager.hpp"
#include "other/capability/capability.hpp"
#include "item/knapsack.h"
#include "other/event/eventhandler.hpp"
#include "scene/scene.h"
#include "world.h"
#include "item/itempool.h"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "config/joinlimitconfig.hpp"
#include "task/taskrecorder.h"
#include "servercommon/serverconfig/crossconfig.hpp"

Shengqi::Shengqi() : m_role(NULL)
{
}

Shengqi::~Shengqi()
{
}

void Shengqi::Init(Role *role, const ShengqiParam &param)
{
	m_role = role;
	m_param = param;
}

void Shengqi::GetInitParam(ShengqiParam *param)
{
	*param = m_param;
}

void Shengqi::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		for (int shengqi_index = 0; shengqi_index < SHENGQI_MAX_NUM; ++shengqi_index)
		{
			int level = m_param.item_list[shengqi_index].level;
			// 强化属性
			const ShengqiStrengthCfg *strength_cfg = LOGIC_CONFIG->GetShengqiConfig().GetStrengthCfg(shengqi_index, level);
			if (NULL == strength_cfg)
			{
				continue;
			}
			
			if (m_param.IsActivateShengqi(shengqi_index))// 开启的圣器
			{
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += strength_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += strength_cfg->gongji;
					//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_GONGJI] += strength_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += strength_cfg->fangyu;
					//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_FANGYU] += strength_cfg->fa_fangyu;
					//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_POJIA] += strength_cfg->pojia;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += strength_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += strength_cfg->shanbi;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += strength_cfg->baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += strength_cfg->jianren;

					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI] += strength_cfg->per_baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += strength_cfg->per_jingzhuan;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO] += strength_cfg->per_kangbao;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER] += strength_cfg->skill_jianshang_per;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI_PER] += strength_cfg->shanbi_per;
//					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += strength_cfg->per_mianshang;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG_PER] += strength_cfg->mingzhong_per;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_PER] += strength_cfg->gedang_per;
//					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += strength_cfg->per_pofang;

					// 下面特殊属性用乘法
					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, strength_cfg->per_mianshang);
					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, strength_cfg->per_pofang);
				}
			}

			for (int spirit_index = 0; spirit_index < SHENGQI_SPIRIT_MAX_NUM; ++spirit_index)
			{
				if (m_param.item_list[shengqi_index].IsSpiritOpen(spirit_index))// 开启的附灵
				{
					{
						// 附灵属性
						const ShengqiSpiritCfg *spirit_cfg = LOGIC_CONFIG->GetShengqiConfig().GetSpiritCfg(shengqi_index);
						if (NULL == spirit_cfg)
						{
							continue;
						}

						if (SHENGQI_ATTR_TYPE_MAXHP == spirit_cfg->attr_type[spirit_index])
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						}
						else if (SHENGQI_ATTR_TYPE_GONGJI == spirit_cfg->attr_type[spirit_index])
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
							//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_GONGJI] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						}
						else if (SHENGQI_ATTR_TYPE_FANGYU == spirit_cfg->attr_type[spirit_index])
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						}
						else if (SHENGQI_ATTR_TYPE_FA_FANGYU == spirit_cfg->attr_type[spirit_index])
						{
							//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_FANGYU] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						}
						else if (SHENGQI_ATTR_TYPE_BAOJI == spirit_cfg->attr_type[spirit_index])
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						}
						else if (SHENGQI_ATTR_TYPE_JIANREN == spirit_cfg->attr_type[spirit_index])
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						}
						else if (SHENGQI_ATTR_TYPE_MINGZHONG == spirit_cfg->attr_type[spirit_index])
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						}
						else if (SHENGQI_ATTR_TYPE_SHANBI == spirit_cfg->attr_type[spirit_index])
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						}
						else if (SHENGQI_ATTR_TYPE_FUJIA_SHANGHAI == spirit_cfg->attr_type[spirit_index])
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						}
						else if (SHENGQI_ATTR_TYPE_DIKANG_SHANGHAI == spirit_cfg->attr_type[spirit_index])
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						}
						//else if (SHENGQI_ATTR_TYPE_WUXING_GONGJI == spirit_cfg->attr_type[spirit_index])
						//{
						//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_WUXING_GONGJI] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						//}
						//else if (SHENGQI_ATTR_TYPE_WUXING_FANGYU == spirit_cfg->attr_type[spirit_index])
						//{
						//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_WUXING_FANGYU] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						//}
						else if (SHENGQI_ATTR_TYPE_PER_BAOJI == spirit_cfg->attr_type[spirit_index])
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						}
						else if (SHENGQI_ATTR_TYPE_PER_KANGBAO == spirit_cfg->attr_type[spirit_index])
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						}
						else if (SHENGQI_ATTR_TYPE_PER_POFANG == spirit_cfg->attr_type[spirit_index])
						{
							Attribute per_pofang = static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);

							// 下面特殊属性用乘法
							m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, per_pofang);
						}
						else if (SHENGQI_ATTR_TYPE_PER_MIANSHANG == spirit_cfg->attr_type[spirit_index])
						{
							Attribute per_mianshang = static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
							m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, per_mianshang);
						}
						else if (SHENGQI_ATTR_PVP_JIANSHANG_PER == spirit_cfg->attr_type[spirit_index])
						{
							Attribute pvp_jianshang_per = static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
							m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, pvp_jianshang_per);
						}
						else if (SHENGQI_ATTR_PVP_ZENGSHANG_PER == spirit_cfg->attr_type[spirit_index])
						{
							Attribute pvp_zengshang_per = static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
							m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, pvp_zengshang_per);
						}
						else if (SHENGQI_ATTR_PVE_JIANSHANG_PER == spirit_cfg->attr_type[spirit_index])
						{
							Attribute pve_jianshang_per = static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
							m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, pve_jianshang_per);
						}
						else if (SHENGQI_ATTR_PVE_ZENGSHANG_PER == spirit_cfg->attr_type[spirit_index])
						{
							Attribute pve_zengshang_per = static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
							m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, pve_zengshang_per);
						}
						//else if (SHENGQI_ATTR_SHANGHAI_JIACHENG_PER == spirit_cfg->attr_type[spirit_index])
						//{
						//	m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_SHANGHAI_JIACHENG_PER] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						//}
						//else if (SHENGQI_ATTR_SHANGHAI_JIANSHAO_PER == spirit_cfg->attr_type[spirit_index])
						//{
						//	m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_SHANGHAI_JIANSHAO_PER] += static_cast<Attribute>(m_param.item_list[shengqi_index].spirit_value[spirit_index]);
						//}
						else if (SHENGQI_ATTR_BASE_JIACHENG_PER == spirit_cfg->attr_type[spirit_index])
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(static_cast<long long>(strength_cfg->maxhp) * (m_param.item_list[shengqi_index].spirit_value[spirit_index] / 10000.0));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(static_cast<long long>(strength_cfg->gongji) * (m_param.item_list[shengqi_index].spirit_value[spirit_index] / 10000.0));
							//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_GONGJI] += static_cast<Attribute>(strength_cfg->gongji * m_param.item_list[shengqi_index].spirit_value[spirit_index] / 10000.0);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(static_cast<long long>(strength_cfg->fangyu) * (m_param.item_list[shengqi_index].spirit_value[spirit_index] / 10000.0));
							//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_FANGYU] += static_cast<Attribute>(strength_cfg->fa_fangyu * m_param.item_list[shengqi_index].spirit_value[spirit_index] / 10000.0);
							//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_POJIA] += static_cast<Attribute>(strength_cfg->pojia * m_param.item_list[shengqi_index].spirit_value[spirit_index] / 10000.0);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(static_cast<long long>(strength_cfg->mingzhong) * (m_param.item_list[shengqi_index].spirit_value[spirit_index] / 10000.0));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(static_cast<long long>(strength_cfg->shanbi) * (m_param.item_list[shengqi_index].spirit_value[spirit_index] / 10000.0));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(static_cast<long long>(strength_cfg->baoji) * (m_param.item_list[shengqi_index].spirit_value[spirit_index] / 10000.0));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(static_cast<long long>(strength_cfg->jianren) * (m_param.item_list[shengqi_index].spirit_value[spirit_index] / 10000.0));
						}
					}
				}
			}
		}
		// 大小目标固定属性加成
		m_role->GetRoleBigSmallGoal()->RoleBigSmallGoalAddSpecialAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGQI, m_attrs_add);
		//大小目标万分比加成
		this->ReCalcAttrBigSmallGoalAddPer(m_attrs_add);

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SHENGQI, m_attrs_add);
	}
	base_add.Add(m_attrs_add.m_attrs, true);
}

void Shengqi::ReCalcAttrBigSmallGoalAddPer(CharIntAttrs & attrs_add)
{
	CharIntAttrs strength_attrs_add;
	//先计算强化总属性
	for (int shengqi_index = 0; shengqi_index < SHENGQI_MAX_NUM; ++shengqi_index)
	{
		int level = m_param.item_list[shengqi_index].level;
		// 强化属性
		const ShengqiStrengthCfg *strength_cfg = LOGIC_CONFIG->GetShengqiConfig().GetStrengthCfg(shengqi_index, level);
		if (NULL == strength_cfg)
		{
			continue;
		}

		if (m_param.IsActivateShengqi(shengqi_index))// 开启的圣器
		{
			{
				strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += strength_cfg->maxhp;
				strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += strength_cfg->gongji;
				strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += strength_cfg->fangyu;
				strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += strength_cfg->mingzhong;
				strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += strength_cfg->shanbi;
				strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += strength_cfg->baoji;
				strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += strength_cfg->jianren;
			}
		}
	}
	//大小目标万分比加成
	int add_value = m_role->GetRoleBigSmallGoal()->GetRoleBigSmallGoalAddAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGQI);
	//最终提供的属性 = 强化总属性 * 加成
	if (add_value > 0)
	{
		strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] = static_cast<Attribute>(strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (add_value * ROLE_ATTR_PER));
		strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] = static_cast<Attribute>(strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (add_value * ROLE_ATTR_PER));
		strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] = static_cast<Attribute>(strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (add_value * ROLE_ATTR_PER));
		strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] = static_cast<Attribute>(strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (add_value * ROLE_ATTR_PER));
		strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] = static_cast<Attribute>(strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (add_value * ROLE_ATTR_PER));
		strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] = static_cast<Attribute>(strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (add_value * ROLE_ATTR_PER));
		strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] = static_cast<Attribute>(strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (add_value * ROLE_ATTR_PER));
		
		//加到系统属性上
		attrs_add.Add(strength_attrs_add.m_attrs);
	}
}

void Shengqi::ShengqiOpera(int opera_type, int param1, int param2)
{
	int open_level = LOGIC_CONFIG->GetShengqiConfig().GetOtherCfg().open_level;
	if (m_role->GetLevel() < open_level || opera_type < 0 || opera_type >= Protocol::SHENGQI_OPERA_TYPE_MAX)
	{
		m_role->NoticeNum(errornum::EN_SHENGQI_OPERATION_FAIL);
		return;
	}

	switch (opera_type)
	{
	case Protocol::SHENGQI_OPERA_TYPE_ALL_INFO:
	{
		this->SendShengqiInfo();
	}
	break;

	case Protocol::SHENGQI_OPERA_TYPE_ACTIVATE:
	{
		this->OnActivateShengqi(param1, false);
	}
	break;

	case Protocol::SHENGQI_OPERA_TYPE_OPEN_SPIRIT:
	{
		this->OnOpenSpiritAttribute(param1, param2);
	}
	break;

	case Protocol::SHENGQI_OPERA_TYPE_SPIRIT:
	{
		this->ShengqiSpirit(param1, param2);
	}
	break;

	case Protocol::SHENGQI_OPERA_TYPE_STRENGTH:
	{
		this->ShengqiStrength(param1, param2);
	}
	break;

	case Protocol::SHENGQI_DECOMPOSE:
	{
		this->Decompose(param1, param2);
	}
	break;
	}
}

void Shengqi::SendShengqiInfo()
{
	static Protocol::SCShengqiInfo sqi;

	sqi.activate_flag = m_param.activate_flag;
	for (int shengqi_index = 0; shengqi_index < SHENGQI_MAX_NUM; ++shengqi_index)
	{
		sqi.shengqi_item[shengqi_index].level = m_param.item_list[shengqi_index].level;
		sqi.shengqi_item[shengqi_index].spirit_flag = m_param.item_list[shengqi_index].spirit_flag;
		for (int spirit_index = 0; spirit_index < SHENGQI_SPIRIT_MAX_NUM; ++spirit_index)
		{
			sqi.shengqi_item[shengqi_index].spirit_value[spirit_index] = m_param.item_list[shengqi_index].spirit_value[spirit_index];
			sqi.shengqi_item[shengqi_index].per_spirit_value[spirit_index] = m_param.item_list[shengqi_index].per_spirit_value[spirit_index];
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&sqi, sizeof(sqi));
}

void Shengqi::OnActivateShengqi(ItemID item_id, bool open_system)
{
	int index = LOGIC_CONFIG->GetShengqiConfig().GetShengqiIndexByItem(item_id);
	if (index < 0 || index >= SHENGQI_MAX_NUM)
	{
		m_role->NoticeNum(errornum::EN_SHENGQI_OPERATION_FAIL);
		return;
	}

	// 判断圣器是否已经激活
	bool spirit_flag = m_param.IsActivateShengqi(index);
	if (spirit_flag)
	{
		gstr::SendNotice(m_role, "shengqi_already_activate");
		return;
	}

	// 激活圣器
	const ShengqiCfg *shengqi_cfg = LOGIC_CONFIG->GetShengqiConfig().GetShengqiCfg(index);
	const ShengqiOpenConditionCfg *open_cfg = LOGIC_CONFIG->GetShengqiConfig().GetOpenConditionCfg(index);
	if (NULL == shengqi_cfg || NULL == open_cfg)
	{
		return;
	}

	if (!open_system && index > 0)	// 激活第一个圣器无需判断材料
	{
		if (shengqi_cfg->series <= 0)
		{
			int open_level = open_cfg->open_level;
			if (open_level > m_param.item_list[index - 1].level)
			{
				gstr::SendNotice(m_role, "shengqi_can_not_activate");
				return;
			}
		}
		else
		{
			if (!m_role->GetKnapsack()->ConsumeItem(open_cfg->stuff_id, open_cfg->stuff_num, "OnActivateShengqi"))
			{
				m_role->NoticeNum(errornum::EN_ITEM_NOT_ENOUGH);
				return;
			}
		}
	}

	// 激活开启默认属性
	int per_spirit_default = LOGIC_CONFIG->GetShengqiConfig().GetOtherCfg().per_spirit_default;
	m_param.item_list[index].spirit_flag = 1;
	m_param.item_list[index].per_spirit_value[0] = per_spirit_default * 100;

	// 开启圣器系统时，将所有圣器的附灵属性都按默认属性百分比计算出来
	if (open_system)
	{
		for (int shengqi_index = 0; shengqi_index < SHENGQI_MAX_NUM; ++shengqi_index)
		{
			const ShengqiSpiritCfg *spirit_cfg = LOGIC_CONFIG->GetShengqiConfig().GetSpiritCfg(shengqi_index);
			if (NULL == spirit_cfg)
			{
				continue;
			}
			m_param.item_list[shengqi_index].level = 1;

			for (int spirit_index = 0; spirit_index < SHENGQI_SPIRIT_MAX_NUM; ++spirit_index)
			{
				int spirit_value = static_cast<int>(spirit_cfg->attr_total_value[spirit_index] * (per_spirit_default / 100.0f));
				m_param.item_list[shengqi_index].spirit_value[spirit_index] = spirit_value;
			}
		}
	}

	m_param.ActivateShengqi(index);
	m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGQI, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_3, index);
	if (!open_system)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGQI);
	}

	gstr::SendNotice(m_role, "shengqi_activate_succ");

	this->SendShengqiInfo();
}

void Shengqi::OnOpenSpiritAttribute(ItemID item_id, int slot_idx)
{
	int index = LOGIC_CONFIG->GetShengqiConfig().GetShengqiIndexByItem(item_id);
	if (index < 0 || index >= SHENGQI_MAX_NUM || slot_idx < 0 || slot_idx >= SHENGQI_SPIRIT_MAX_NUM)
	{
		m_role->NoticeNum(errornum::EN_SHENGQI_OPERATION_FAIL);
		return;
	}

	// 判断附灵是否已经开启
	bool spirit_flag = m_param.item_list[index].IsSpiritOpen(slot_idx);
	if (spirit_flag)
	{
		return;
	}

	// 开启附灵
	int open_gold = LOGIC_CONFIG->GetShengqiConfig().GetOtherCfg().open_gold;
	if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(open_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(open_gold, "Shengqi::OnOpenSpiritAttribute"))
	{
		return;
	}
	int per_spirit_default = LOGIC_CONFIG->GetShengqiConfig().GetOtherCfg().per_spirit_default;
	m_param.item_list[index].per_spirit_value[slot_idx] = per_spirit_default * 100;

	m_param.item_list[index].OpenSpirit(slot_idx);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGQI);
	gstr::SendNotice(m_role, "shengqi_spirit_open_succ");

	this->SendShengqiInfo();
}

void Shengqi::ShengqiSpirit(ItemID item_id, ItemID stuff_id)
{
	int index = LOGIC_CONFIG->GetShengqiConfig().GetShengqiIndexByItem(item_id);
	if (index < 0 || index >= SHENGQI_MAX_NUM)
	{
		m_role->NoticeNum(errornum::EN_SHENGQI_OPERATION_FAIL);
		return;
	}

	// 获取满级附灵数量
	int max_num = 0;
	bool max_level = true;
	for (int spirit_index = 0; spirit_index < SHENGQI_SPIRIT_MAX_NUM; ++spirit_index)
	{
		if (!m_param.item_list[index].IsSpiritOpen(spirit_index))	// 未开启附灵
		{
			continue;
		}

		if (!m_param.item_list[index].IsSpiritMax(spirit_index))	// 未满级的附灵
		{
			max_level = false;
			continue;
		}

		max_num++;
	}

	// 附灵全部满级则返回
	if (max_level)
	{
		m_role->NoticeNum(errornum::EN_SHENGQI_SPIRIT_MAX);
		return;
	}

	// 消耗
	const ShengqiSpiritCfg *spirit_cfg = LOGIC_CONFIG->GetShengqiConfig().GetSpiritCfg(index);
	if (NULL == spirit_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHENGQI_OPERATION_FAIL);
		return;
	}

	if (!m_role->GetKnapsack()->ConsumeItem(stuff_id, spirit_cfg->spirit_stuff_num, "ShengqiSpirit"))
	{
		m_role->NoticeNum(errornum::EN_ITEM_NOT_ENOUGH);
		return;
	}

	int spirit_max = LOGIC_CONFIG->GetShengqiConfig().GetOtherCfg().spirit_max;
	int per_one_spirit_strength = LOGIC_CONFIG->GetShengqiConfig().GetOtherCfg().per_one_spirit_strength;
	int per_two_spirit_strength = LOGIC_CONFIG->GetShengqiConfig().GetOtherCfg().per_two_spirit_strength;
	int per_three_spirit_strength = LOGIC_CONFIG->GetShengqiConfig().GetOtherCfg().per_three_spirit_strength;

	for (int spirit_index = 0; spirit_index < SHENGQI_SPIRIT_MAX_NUM; ++spirit_index)
	{

		if (!m_param.item_list[index].IsSpiritOpen(spirit_index))	// 未开启的附灵
		{
			continue;
		}

		if (m_param.item_list[index].IsSpiritMax(spirit_index))	// 满级的附灵
		{
			continue;
		}

		int per_spirit_value = m_param.item_list[index].per_spirit_value[spirit_index];
		//if (per_spirit_value < 0 && per_spirit_value >= spirit_max)
		if (per_spirit_value < 0 || per_spirit_value >= spirit_max * 100)
		{
			continue;
		}

		int per_strength_min = spirit_cfg->per_strength_min;
		int per_strength_max = spirit_cfg->per_strength_max;

		// 附灵满值增幅
		if (1 == max_num)
		{
			per_strength_min = static_cast<int>(per_strength_min * per_one_spirit_strength / 100.0f);
			per_strength_max = static_cast<int>(per_strength_max * per_one_spirit_strength / 100.0f);
		}
		if (2 == max_num)
		{
			per_strength_min = static_cast<int>(per_strength_min * per_two_spirit_strength / 100.0f);
			per_strength_max = static_cast<int>(per_strength_max * per_two_spirit_strength / 100.0f);
		}
		if (3 <= max_num)
		{
			per_strength_min = static_cast<int>(per_strength_min * per_three_spirit_strength / 100.0f);
			per_strength_max = static_cast<int>(per_strength_max * per_three_spirit_strength / 100.0f);
		}
		int rand_value = RandomNum(per_strength_min, per_strength_max);
		per_spirit_value += rand_value;
		int spirit_value = static_cast<int>(spirit_cfg->attr_total_value[spirit_index] * (per_spirit_value / 10000.0f));

		if (per_spirit_value >= spirit_max * 100)
		{
			per_spirit_value = spirit_max * 100;
			//spirit_value = spirit_cfg->attr_total_value[spirit_index];
			spirit_value = static_cast<int>(spirit_cfg->attr_total_value[spirit_index] * (per_spirit_value / 10000.0f));	//策划需求，附灵上限可以突破100%
			m_param.item_list[index].MaxSpirit(spirit_index);
		}
		m_param.item_list[index].per_spirit_value[spirit_index] = per_spirit_value;
		m_param.item_list[index].spirit_value[spirit_index] = spirit_value;

		gamelog::g_log_shengqi.printf(LL_INFO, "Shengqi::ShengqiSpirit role(%d,%s) index:%d level:%d spirit_value:%d per_spirit_value:%d",
			m_role->GetUID(), m_role->GetName(),
			index, m_param.item_list[index].level,
			m_param.item_list[index].spirit_value[spirit_index], m_param.item_list[index].per_spirit_value[spirit_index]
			);
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGQI);
	gstr::SendNotice(m_role, "shengqi_spirit_succ");

	this->SendShengqiInfo();
}

void Shengqi::ShengqiStrength(ItemID item_id, ItemID stuff_id)
{
	int index = LOGIC_CONFIG->GetShengqiConfig().GetShengqiIndexByItem(item_id);
	if (index < 0 || index >= SHENGQI_MAX_NUM)
	{
		m_role->NoticeNum(errornum::EN_SHENGQI_OPERATION_FAIL);
		return;
	}

	int strength_max = LOGIC_CONFIG->GetShengqiConfig().GetOtherCfg().strength_max;
	if (m_param.item_list[index].level <= 0 || m_param.item_list[index].level >= strength_max)
	{
		m_role->NoticeNum(errornum::EN_SHENGQI_OPERATION_FAIL);
		return;
	}

	// 消耗
	const ShengqiStrengthCfg *next_strength_cfg = LOGIC_CONFIG->GetShengqiConfig().GetStrengthCfg(index, m_param.item_list[index].level + 1);
	const ShengqiStrengthCfg *strength_cfg = LOGIC_CONFIG->GetShengqiConfig().GetStrengthCfg(index, m_param.item_list[index].level);
	if (NULL == next_strength_cfg || NULL == strength_cfg)
	{
		gstr::SendNotice(m_role, "shengqi_can_not_strength");
		return;
	}

	if (!m_role->GetKnapsack()->ConsumeItem(stuff_id, strength_cfg->strength_stuff_num, "ShengqiSpirit"))
	{
		m_role->NoticeNum(errornum::EN_ITEM_NOT_ENOUGH);
		return;
	}

	++m_param.item_list[index].level;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGQI);
	gstr::SendNotice(m_role, "shengqi_strength_succ");

	EventHandler::Instance().OnShengqiStrengthLevel(m_role);

	this->SendShengqiInfo();

	gamelog::g_log_shengqi.printf(LL_INFO, "Shengqi::ShengqiStrength role(%d,%s) index:%d level:%d",
		m_role->GetUID(), m_role->GetName(),
		index, m_param.item_list[index].level
		);
}

void Shengqi::CheckActivateShengqiSystem()
{
	if (m_param.IsActivateShengqi())
	{
		return;
	}

	if (m_role->GetLevel() >= LOGIC_CONFIG->GetShengqiConfig().GetOtherCfg().open_level)
	{
		const ShengqiCfg *shengqi_cfg = LOGIC_CONFIG->GetShengqiConfig().GetShengqiCfg(0);
		if (NULL == shengqi_cfg)
		{
			return;
		}

		this->OnActivateShengqi(shengqi_cfg->id, true);
	}
}


void Shengqi::OnRoleLogin()
{
	this->CheckActivateShengqiSystem();
	this->CheckFuncOpen();

	//登录时重新计算未开启附灵的属性
	int per_spirit_default = LOGIC_CONFIG->GetShengqiConfig().GetOtherCfg().per_spirit_default;
	{
		//附灵属性
		for (int shengqi_index = 0; shengqi_index < SHENGQI_MAX_NUM; ++shengqi_index)
		{
			const ShengqiSpiritCfg *spirit_cfg = LOGIC_CONFIG->GetShengqiConfig().GetSpiritCfg(shengqi_index);
			if (NULL == spirit_cfg)
			{
				continue;
			}

			for (int spirit_index = 0; spirit_index < SHENGQI_SPIRIT_MAX_NUM; ++spirit_index)
			{
				// 判断附灵是否已经开启
				bool spirit_flag = m_param.item_list[shengqi_index].IsSpiritOpen(spirit_index);
				if (spirit_flag)		//附灵已开启的不能在此重算属性
				{
					continue;
				}

				int spirit_value = static_cast<int>(spirit_cfg->attr_total_value[spirit_index] * (per_spirit_default / 100.0f));
				m_param.item_list[shengqi_index].spirit_value[spirit_index] = spirit_value;
			}
		}
	}

	this->SendShengqiInfo();
}

void Shengqi::Decompose(ItemID stuff_id, int num)
{
	const ShengqiDecomposeCfg *decompose_cfg = LOGIC_CONFIG->GetShengqiConfig().GetDecomposeCfg(stuff_id);
	if (decompose_cfg == NULL)
	{
		m_role->NoticeNum(errornum::EN_SHENGQI_OPERATION_FAIL);
		return;
	}

	ItemConfigData item = decompose_cfg->return_item;
	item.num *= num;

	const ItemBase *itembase = ITEMPOOL->GetItem(item.item_id);
	if (itembase == nullptr) return;

	item.is_bind = itembase->IsBind();
	int stack_num = itembase->GetPileLimit();
	int need_grid = (stack_num + item.num) / stack_num;

	// 检查背包空间是否足够
	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_grid))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->ConsumeItem(stuff_id, num, "ShengqiSpirit"))
	{
		m_role->NoticeNum(errornum::EN_ITEM_NOT_ENOUGH);
		return;
	}

	if (m_role->GetKnapsack()->Put(item, PUT_REASON_SHENGQI_STUFF_DECOMPOSE))
	{
		m_role->NoticeNum(noticenum::NT_SHENGQI_STUFF_DECOMPOSE_SUCC);
	}
}

int Shengqi::GetTotalStrengthLevel()
{
	int total_level = 0;

	for (int index = 0; index < SHENGQI_MAX_NUM; index ++)
	{
		total_level += m_param.item_list[index].level;
	}

	return total_level;
}

int Shengqi::GetStrengthLevelCount(int level)
{
	int count = 0;

	for (int index = 0; index < SHENGQI_MAX_NUM; index++)
	{
		if (m_param.item_list[index].level >= level)
		{
			count++;
		}
	}

	return count;
}

void Shengqi::OnDayChange(UInt32 old_dayid, UInt32 now_dayid)
{
	if (old_dayid == now_dayid)
	{
		return;
	}
	this->CheckFuncOpen();
}

void Shengqi::OnRoleLevelChange(int level)
{
	this->CheckFuncOpen();
}

void Shengqi::OnCompleteTask(TaskID task_id)
{
	this->CheckFuncOpen();
}

void Shengqi::CheckFuncOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (m_role->GetRoleBigSmallGoal()->IsFuncActive(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGQI))
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENGQI);
	if (fun_open_cfg != nullptr && opengame_day >= fun_open_cfg->opengame_day)
	{
		if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
		{
			m_role->GetRoleBigSmallGoal()->OnOpenFunc(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGQI);
		}
	}
}