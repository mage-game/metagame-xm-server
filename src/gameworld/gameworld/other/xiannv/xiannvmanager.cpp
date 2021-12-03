#include "xiannvmanager.hpp"
#include "servercommon/configcommon.h"
#include "obj/character/role.h"
#include "xiannvconfig.hpp"
#include "protocal/msgxiannv.h"
#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "scene/scene.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/namefilter.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "other/event/eventhandler.hpp"
#include "gameworld/scene/scenemanager.h"
#include "servercommon/string/gameworldstr.h"
#include "gameworld/world.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "protocal/msgfight.h"
#include "other/shop/shop.hpp"
#include "other/halo/haloconfig.hpp"
#include "item/knapsack.h"
#include "other/vip/vip.hpp"
#include "other/capability/capability.hpp"
#include "other/sublock/sublock.hpp"
#include "other/halo/halomanager.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "config/joinlimitconfig.hpp"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "task/taskrecorder.h"
#include "effect/skilleffect/effecthpstore.hpp"
#include "common/bintohex.h"
#include "servercommon/serverconfig/crossconfig.hpp"

XiannvManager::XiannvManager() : m_role(NULL), m_shengwu_skill_capablity(0), m_shengwu_base_capapblity(0)
{
	memset(m_last_skill_time_list, 0, sizeof(m_last_skill_time_list));
}

XiannvManager::~XiannvManager()
{
}

void XiannvManager::Init(Role *role, const XiannvParam &xiannv_param)
{
	m_role = role;
	m_xiannv_param = xiannv_param;

	bool is_first_play = true;
	for (int i = 0; i < XIANNV_SHENGWU_MILING_FONT_COUNT; ++i)
	{
		if (m_xiannv_param.miling_list[i] >= 0)
		{
			is_first_play = false;
		}
	}
	if (is_first_play) // 第一次初始化女神圣物功能，免费刷新一次
	{
		this->ShengwuSystemChouLing();
	}

	this->RecalcSkillList();
}

void XiannvManager::GetInitParam(XiannvParam *xiannv_param)
{
	*xiannv_param = m_xiannv_param;
}

void XiannvManager::OnLogin()
{
	this->CheckFuncOpen();
}

void XiannvManager::Update(time_t now_second)
{
}

void XiannvManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	XiannvConfig &xn_cfg = LOGIC_CONFIG->GetXiannvConfig();

	if (is_recalc)
	{
		m_attrs_add.Reset();

		if (m_xiannv_param.IsXnActived())
		{
			// 仙女属性
			CharIntAttrs total_xn_level_attrs; // 仙女等级总属性
			for (int i = 0; i < MAX_XIANNV_COUNT; ++i)
			{
				if (m_xiannv_param.IsXiannvActive(i))
				{
					const XiannvItem &xn_item = m_xiannv_param.xn_item_list[i];

					// 等级属性
					const XiannvLevelCfg::ConfigItem *level_cfg_item = xn_cfg.GetLevelCfg(i, xn_item.xn_level);
					if (NULL != level_cfg_item)
					{
						total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg_item->maxhp;
						total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg_item->gongji;
						total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg_item->fangyu;
						total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += level_cfg_item->baoji;
						total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += level_cfg_item->jianren;
						total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg_item->mingzhong;
						total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg_item->shanbi;
					}

					// 资质属性
					const XiannvZiZhiCfg::ConfigItem *zizhi_cfg_item = xn_cfg.GetZiZhiCfg(i, xn_item.xn_zizhi);
					if (NULL != zizhi_cfg_item)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += zizhi_cfg_item->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += zizhi_cfg_item->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += zizhi_cfg_item->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += zizhi_cfg_item->baoji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += zizhi_cfg_item->jianren;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += zizhi_cfg_item->mingzhong;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += zizhi_cfg_item->shanbi;
					}
					
				}
			}

			// 等级总属性
			m_attrs_add.Add(total_xn_level_attrs.m_attrs);

			// 技能3对每个仙女提供的万分比属性加成
			ShengwuSkill &skill_item3 = m_shengwu_skill_list[XIANNV_SHENGWU_SKILL_TYPE_3];
			if (skill_item3.param_1 > 0)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (ROLE_ATTR_PER * skill_item3.param_1));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (ROLE_ATTR_PER * skill_item3.param_1));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (ROLE_ATTR_PER * skill_item3.param_1));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (ROLE_ATTR_PER * skill_item3.param_1));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (ROLE_ATTR_PER * skill_item3.param_1));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (ROLE_ATTR_PER * skill_item3.param_1));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (ROLE_ATTR_PER * skill_item3.param_1));
			}

			// 激活大目标
			int add_value = m_role->GetRoleBigSmallGoal()->GetRoleBigSmallGoalAddAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_XIANNV);
			if (add_value > 0)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(round(total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (add_value * ROLE_ATTR_PER)));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(round(total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (add_value * ROLE_ATTR_PER)));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(round(total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (add_value * ROLE_ATTR_PER)));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(round(total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (add_value * ROLE_ATTR_PER)));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(round(total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (add_value * ROLE_ATTR_PER)));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(round(total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (add_value * ROLE_ATTR_PER)));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(round(total_xn_level_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (add_value * ROLE_ATTR_PER)));
			}

			// 仙女幻化等级属性
			for (int i = 0; i < MAX_XIANNV_HUANHUA_COUNT; ++i)
			{
				if (m_xiannv_param.IsHuanHuaActive(i))
				{
					const int huanhua_level = m_xiannv_param.xiannv_huanhua_level[i];

					// 等级属性加成
					const XiannvHuanHuaLevelCfg::ConfigItem *level_cfg_item = xn_cfg.GetHuanHuaLevelCfg(i, huanhua_level);
					if (NULL != level_cfg_item)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg_item->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg_item->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg_item->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += level_cfg_item->baoji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += level_cfg_item->jianren;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg_item->mingzhong;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg_item->shanbi;
					}
					this->HuanHuaSpecialCapAdd(i, huanhua_level, m_attrs_add);
				}
			}

			// 仙女攻击
			if (m_xiannv_param.pos_list[0] >= 0)
			{
				XiannvConfig& xn_cfg = LOGIC_CONFIG->GetXiannvConfig();
				const XiannvOtherCfg* other_cfg = xn_cfg.GetOtherCfg();

				for (int i = 0; i < MAX_XIANNV_COUNT; i++)
				{
					if (m_xiannv_param.IsXiannvActive(i))
					{
						int percent = other_cfg->idle_percent;

						for (int k = 0; k < MAX_XIANNV_POS; k++)
						{
							if (m_xiannv_param.pos_list[k] == i)
							{
								if (0 == k)
								{
									percent = other_cfg->main_percent;
								}
								else
								{
									percent = other_cfg->aid_percent;
								}
								break;
							}
						}

						Attribute xiannv_gongji = 0;
						const XiannvLevelCfg::ConfigItem* level_cfg_item = xn_cfg.GetLevelCfg(i, m_xiannv_param.xn_item_list[i].xn_level);
						if (NULL != level_cfg_item)
						{
							xiannv_gongji += level_cfg_item->xiannv_gongji;
						}

						const XiannvZiZhiCfg::ConfigItem* zizhi_cfg_item = xn_cfg.GetZiZhiCfg(i, m_xiannv_param.xn_item_list[i].xn_zizhi);
						if (NULL != zizhi_cfg_item)
						{
							xiannv_gongji += zizhi_cfg_item->xiannv_gongji;
						}

						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] += (Attribute)(xiannv_gongji * (percent / 100.0));
					}
				}

				// 幻化攻击
				int huanhua_count = xn_cfg.GetHuanHuaCount();
				for (int i = 0; i < huanhua_count && i < MAX_XIANNV_HUANHUA_COUNT; i++)
				{
					if (m_xiannv_param.IsHuanHuaActive(i))
					{
						const XiannvHuanHuaLevelCfg::ConfigItem* cfg = xn_cfg.GetHuanHuaLevelCfg(i, m_xiannv_param.xiannv_huanhua_level[i]);
						if (NULL != cfg)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] += cfg->xiannv_gongji;
						}
					}
				}
			}
		}

		// 圣物系统战力计算
		{
			// 圣物系统单独计算，有些属性专门对圣物系统战力做加成的
			CharIntAttrs shengwu_attrs_add; shengwu_attrs_add.Reset();
			CharIntAttrs shengwu0_attrs_add; //其实没必要Reset，构造是的时候已经Reset过了
			m_shengwu_skill_capablity = 0;

			// 女神圣物基础属性
			for (int shengwu_id = 0; shengwu_id <= XIANNV_SHENGWU_MAX_ID; ++shengwu_id)
			{
				const NvshenShengwuInfoCfg::ShengwuItem *shengwu_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(shengwu_id, m_xiannv_param.all_shengwu_list[shengwu_id].level);
				if (NULL != shengwu_cfg)
				{
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += shengwu_cfg->maxhp;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += shengwu_cfg->gongji;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += shengwu_cfg->fangyu;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += shengwu_cfg->baoji;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += shengwu_cfg->jianren;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += shengwu_cfg->mingzhong;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += shengwu_cfg->shanbi;

					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] += shengwu_cfg->fujia;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += shengwu_cfg->mianshang;
					if (shengwu_id + 1 == XIANNV_SHENGWU_SKILL_TYPE_1)
					{
						shengwu0_attrs_add.Set(shengwu_attrs_add.m_attrs);	//获取第一个圣物的属性
					}
				}
			}
			ShengwuSkill &skill_item1 = m_shengwu_skill_list[XIANNV_SHENGWU_SKILL_TYPE_1];
			ShengwuSkill &skill_item2 = m_shengwu_skill_list[XIANNV_SHENGWU_SKILL_TYPE_2];
			CharIntAttrs shengwu_attrs_base_add;
			shengwu_attrs_base_add.Set(shengwu_attrs_add.m_attrs);//获取所有圣物的基础属性
			// 圣物技能1提供的属性加成
			{
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(shengwu0_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (skill_item1.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(shengwu0_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (skill_item1.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(shengwu0_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (skill_item1.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(shengwu0_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (skill_item1.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(shengwu0_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (skill_item1.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(shengwu0_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (skill_item1.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(shengwu0_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (skill_item1.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] += static_cast<Attribute>(shengwu0_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] * (skill_item1.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += static_cast<Attribute>(shengwu0_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] * (skill_item1.param_1 * ROLE_ATTR_PER));
			}
			// 圣物技能2提供的属性加成
			{
				//第二个圣物技能为所有圣物属性提供加成
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(shengwu_attrs_base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (skill_item2.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(shengwu_attrs_base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (skill_item2.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(shengwu_attrs_base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (skill_item2.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(shengwu_attrs_base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (skill_item2.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(shengwu_attrs_base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (skill_item2.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(shengwu_attrs_base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (skill_item2.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(shengwu_attrs_base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (skill_item2.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] += static_cast<Attribute>(shengwu_attrs_base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] * (skill_item2.param_1 * ROLE_ATTR_PER));
				shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += static_cast<Attribute>(shengwu_attrs_base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] * (skill_item2.param_1 * ROLE_ATTR_PER));
			}
			//获取基础属性战力
			m_shengwu_base_capapblity = m_role->GetCapability()->GetCapByAttr(shengwu_attrs_add);

			int attr_percent = 0;
			// 圣物共鸣，每个格子战力
			for (int shengwu_grid_id = 0; shengwu_grid_id <= XIANNV_SHENGWU_GONGMING_MAX_GRID_ID; ++shengwu_grid_id)
			{
				const NvshenShengwuGongmingCfg::ItemCfg *grid_cfg = LOGIC_CONFIG->GetXiannvConfig().GetGongmingGridCfg(shengwu_grid_id, m_xiannv_param.grid_level_list[shengwu_grid_id]);
				if (NULL != grid_cfg)
				{
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += grid_cfg->maxhp;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += grid_cfg->gongji;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += grid_cfg->fangyu;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += grid_cfg->baoji;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += grid_cfg->jianren;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += grid_cfg->mingzhong;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += grid_cfg->shanbi;

					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] += grid_cfg->fujia;
					shengwu_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += grid_cfg->mianshang;

					m_shengwu_skill_capablity += grid_cfg->capbility;
					attr_percent += grid_cfg->attr_percent;
				}
			}

			// 对圣物系统属性做加成
			for (int attr_type = 0; attr_type < CharIntAttrs::CHARINTATTR_TYPE_MAX; ++attr_type)
			{
//				shengwu_attrs_add.m_attrs[attr_type] += Attribute(shengwu_attrs_add.m_attrs[attr_type] * (ROLE_ATTR_PER * attr_percent));
				shengwu_attrs_add.AddValue(attr_type, Attribute(shengwu_attrs_add.m_attrs[attr_type] * (ROLE_ATTR_PER * attr_percent)));
			}

			m_attrs_add.Add(shengwu_attrs_add.m_attrs);
		}
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_XIANNV, m_attrs_add);
	this->RecalcSkillCapability(m_shengwu_skill_capablity);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_XIANNV, m_shengwu_skill_capablity);

	m_xiannv_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_XIANNV);

	base_add.Add(m_attrs_add.m_attrs, true);
}

void XiannvManager::OnAttack(ObjID attacker, Attribute delta_hp)
{
// 	unsigned long now_time = (unsigned int)EngineAdapter::Instance().GetGameTime();
// 	ShengwuSkill &skill_item = m_shengwu_skill_list[XIANNV_SHENGWU_SKILL_TYPE_4];
// 	if (now_time > skill_item.cd_end_perform_time)
// 	{
// 		if (RandomNum(ShengwuSkill::TRIGGER_RATE_NUM) < skill_item.trigger_rate)
// 		{
// 			// 反伤
// 			//EffectReboundHurt* effect_rebound = new EffectReboundHurt(m_role->GetId(), 0, skill_item.param_2, skill_item.param_1, 1);
// 			//effect_rebound->SetClientEffectType(EffectBase::ECT_SKILL_REBOUNDHURT);
// 
// 			// 护盾
// 			int hp_store = static_cast<int>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP) * (ROLE_ATTR_PER * skill_item.param_1));
// 			EffectHpStore* effect_rebound = new EffectHpStore(m_role->GetId(), 0, skill_item.param_2, hp_store, 1);
// 
// 			m_role->AddEffect(effect_rebound);
// 
// 			// 最终冷却时间 = 当前时间 + buff持续时间 + 技能冷却时间
// 			skill_item.cd_end_perform_time = now_time + skill_item.param_2 + skill_item.cool_down_ms;
// 		}
// 	}
}

void XiannvManager::OnHurtOther(Character *target, long long base_injure, long long &injure)
{
	if (NULL == target) return;

	//技能4 攻击时，有trigger_rate概率生成相当于攻击param_1万分比的护盾，持续param_2毫秒,冷却cool_down_ms毫秒
	unsigned long now_time = (unsigned long)EngineAdapter::Instance().GetGameTime();
	ShengwuSkill &skill_item = m_shengwu_skill_list[XIANNV_SHENGWU_SKILL_TYPE_4];
	if (now_time > skill_item.cd_end_perform_time)
	{
		if (RandomNum(ShengwuSkill::TRIGGER_RATE_NUM) < skill_item.trigger_rate)
		{
			// 护盾
			Attribute hp_store = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (ROLE_ATTR_PER * skill_item.param_1));
			EffectHpStore* effect_hps = new EffectHpStore(m_role->GetId(), 0, skill_item.param_2, hp_store, 1);
			effect_hps->SetReplacePercent(100);
			effect_hps->SetClientEffectType(EffectBase::ECT_SKILL_HPSTORE);
			m_role->AddEffect(effect_hps);

			// 最终冷却时间 = 当前时间 + buff持续时间 + 技能冷却时间
			skill_item.cd_end_perform_time = now_time + skill_item.param_2 + skill_item.cool_down_ms;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	return;	//前3个圣物技能现在改成了加属性

	//unsigned long now_time = (unsigned int)EngineAdapter::Instance().GetGameTime();

	{
		ShengwuSkill &skill_item = m_shengwu_skill_list[XIANNV_SHENGWU_SKILL_TYPE_1];
		if (now_time > skill_item.cd_end_perform_time)
		{
			if (RandomNum(ShengwuSkill::TRIGGER_RATE_NUM) < skill_item.trigger_rate)
			{
				// 一次性伤害
				EffectOnce *effect_once = new EffectOnce(m_role->GetId(), 0, -skill_item.param_1, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_XIANNV_SHENGWU_SKILL);
				target->AddEffect(effect_once);

				skill_item.cd_end_perform_time = now_time + skill_item.cool_down_ms;
			}
		}
	}

	{
		ShengwuSkill &skill_item = m_shengwu_skill_list[XIANNV_SHENGWU_SKILL_TYPE_2];
		if (now_time > skill_item.cd_end_perform_time)
		{
			if (RandomNum(ShengwuSkill::TRIGGER_RATE_NUM) < skill_item.trigger_rate)
			{
				// 回血
				Attribute delta_blood = skill_item.param_1;
				m_role->ChangeHp(&delta_blood, true);

				// 策划说回血特效只在仙女技能上展示，所以没有放到effectbase里统一处理
				m_role->NotifyFightSpecialEffect(Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_XIANNV_SHENGWU_RESTORE_HP, m_role->GetId(), Protocol::SCFightSpecialFloat::EFFECT_RESTORE_HP, delta_blood);

				skill_item.cd_end_perform_time = now_time + skill_item.cool_down_ms;
			}
		}
	}

	{
		ShengwuSkill &skill_item = m_shengwu_skill_list[XIANNV_SHENGWU_SKILL_TYPE_3];
		if (now_time > skill_item.cd_end_perform_time)
		{
			if (RandomNum(ShengwuSkill::TRIGGER_RATE_NUM) < skill_item.trigger_rate)
			{
				// 加防御
				EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), 1, skill_item.param_2, 1);
				effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				effect_buff->AddFangYuEffect(skill_item.param_1);
				effect_buff->SetClientEffectType(EffectBase::ECT_SKILL_ADD_FANGYU);
				m_role->AddEffect(effect_buff);

				skill_item.cd_end_perform_time = now_time + skill_item.param_2 + skill_item.cool_down_ms;
			}
		}
	}
}

void XiannvManager::OnDayChange(int old_dayid, int now_dayid)
{
	m_xiannv_param.day_free_miling_times = 0;
	m_xiannv_param.day_fetch_ling_time = 0;
	m_xiannv_param.cur_gold_miling_times = 0;

	this->CheckFuncOpen();
	//this->ShengwuSendChangeInfo(Protocol::SCXiannvShengwuChangeInfo::LINGYE, m_xiannv_param.day_free_miling_times, m_xiannv_param.cur_gold_miling_times, m_xiannv_param.day_fetch_ling_time, m_xiannv_param.shengwu_lingye);
}

bool XiannvManager::ActiveXiannv(int xiannv_id)
{
	if (xiannv_id < 0 || xiannv_id >= MAX_XIANNV_COUNT)
	{
		return false;
	}

	const XiannvBaseCfg::ConfigItem *base_cfg = LOGIC_CONFIG->GetXiannvConfig().GetBaseCfg(xiannv_id);
	if (NULL == base_cfg)
	{
		return false;
	}

	if (!m_xiannv_param.IsXiannvActive(xiannv_id))
	{
		m_xiannv_param.ActiveXiannv(xiannv_id);
		m_xiannv_param.xn_item_list[xiannv_id].xn_zizhi = 1;
		m_xiannv_param.xn_item_list[xiannv_id].xn_level = m_role->GetLevel();

		if (m_xiannv_param.pos_list[0] < 0)
		{
			m_xiannv_param.pos_list[0] = xiannv_id;
			m_role->GetSkillManager()->AddXiannvSkill(base_cfg->skill_id, 1);
			this->BroadcastXiannvInfo();
		}
		{//激活就出战
			char pos_list[MAX_XIANNV_POS];
			memset(pos_list, -1, sizeof(pos_list));
			pos_list[0] = xiannv_id;
			this->FightOut(pos_list);
		}
		m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_XIANNV, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_3, xiannv_id);
		m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_XIANNV, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_5, m_xiannv_param.xn_item_list[xiannv_id].xn_zizhi);

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANNV);
		this->SendAllXiannvInfo();
		m_role->NoticeNum(noticenum::NT_XIANNV_ACTIVE_SUCC);

		this->PrintXiannv("XiannvActive", xiannv_id);
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_XIANNV, xiannv_id, true);
// 		int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xiannv_active_notice, m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), 
// 			xiannv_id, (int)m_xiannv_param.xn_item_list[xiannv_id].xn_grade, m_xiannv_param.xn_item_list[xiannv_id].xn_zizhi,m_xiannv_param.xn_item_list[xiannv_id].xn_level);
// 		if (len > 0) 
// 		{
// 			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER);
// 		}

		// 激活传闻
		if (base_cfg->is_send_system_msg)
		{
			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xiannv_active_content, m_role->GetUID(), m_role->GetName(),
				(int)m_role->GetCamp(), xiannv_id);
			if (len > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
			}
		}

		EventHandler::Instance().OnActiveXiannv(m_role, base_cfg->active_item_id);

		return true;
	}

	return false;
}

void XiannvManager::Rename(short xianvn_id, const GameName new_name)
{
	if (xianvn_id < 0 || xianvn_id >= MAX_XIANNV_COUNT)
		return;

	const XiannvOtherCfg *other_cfg = LOGIC_CONFIG->GetXiannvConfig().GetOtherCfg();
	if (NULL == other_cfg)	return;

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg->rename_consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	GameName tmp_new_name;
	F_STRNCPY(tmp_new_name, new_name, sizeof(GameName));

	InvalidCharFilte(tmp_new_name);

	if (NameFilter::Instance().HasSensitiveWord(sizeof(GameName), tmp_new_name))
	{
		m_role->NoticeNum(errornum::EN_RENAME_NAME_INVALID);
		return;
	}

	F_STRNCPY(m_xiannv_param.xiannv_name[xianvn_id], tmp_new_name, sizeof(GameName));

	m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg->rename_consume_gold, "XiannvRename");

	this->SendAllXiannvInfo();
	this->BroadcastXiannvInfo();
}

void XiannvManager::SendAllXiannvInfo()
{
	Protocol::SCAllXiannvInfo axvi;
	axvi.active_xiannv_flag = m_xiannv_param.active_xiannv_flag;
	UNSTD_STATIC_CHECK(sizeof(axvi.active_huanhua_flag) == sizeof(m_xiannv_param.active_huanhua_flag));
	memcpy(axvi.active_huanhua_flag, m_xiannv_param.active_huanhua_flag, sizeof(axvi.active_huanhua_flag));
	axvi.huanhua_id = m_xiannv_param.huanhua_id;
	memcpy(axvi.xiannv_name, m_xiannv_param.xiannv_name, sizeof(axvi.xiannv_name));
	memcpy(axvi.xiannv_huanhua_level, m_xiannv_param.xiannv_huanhua_level, sizeof(axvi.xiannv_huanhua_level));
	memcpy(axvi.xn_item_list, m_xiannv_param.xn_item_list, sizeof(axvi.xn_item_list));
	memcpy(axvi.pos_list, m_xiannv_param.pos_list, sizeof(axvi.pos_list));

	axvi.shengwu_lingye = m_xiannv_param.shengwu_lingye;
	axvi.shengwu_chou_id = m_xiannv_param.shengwu_chou_id;
	memcpy(axvi.shengwu_chou_exp_list, m_xiannv_param.shengwu_chou_exp_list, sizeof(axvi.shengwu_chou_exp_list));
	memcpy(axvi.all_shengwu_list, m_xiannv_param.all_shengwu_list, sizeof(axvi.all_shengwu_list));
	memcpy(axvi.grid_level_list, m_xiannv_param.grid_level_list, sizeof(axvi.grid_level_list));
	axvi.day_free_miling_times = m_xiannv_param.day_free_miling_times;
	axvi.day_fetch_ling_time = m_xiannv_param.day_fetch_ling_time;
	axvi.cur_gold_miling_times = m_xiannv_param.cur_gold_miling_times;
	memcpy(axvi.miling_list, m_xiannv_param.miling_list, sizeof(axvi.miling_list));
	axvi.shengwu_essence = m_xiannv_param.shengwu_essence;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&axvi, sizeof(axvi));
}

void XiannvManager::SendXiannvInfo(int xiannv_id, int notify_reason)
{
	Protocol::SCXiannvInfo xvi;
	xvi.notify_reaon = notify_reason;
	xvi.xiannv_id = xiannv_id;
	if (xiannv_id >= 0 && xiannv_id < MAX_XIANNV_COUNT)
	{
		xvi.xn_item = m_xiannv_param.xn_item_list[xiannv_id];
	}
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&xvi, sizeof(xvi));
}

void XiannvManager::BroadcastXiannvInfo()
{
	if (NULL != m_role->GetScene())
	{
		Protocol::SCXiannvViewChange xvvc;
		xvvc.obj_id = m_role->GetId();
		xvvc.use_xiannv_id = m_xiannv_param.pos_list[0];
		xvvc.huanhua_id = m_xiannv_param.huanhua_id;
		F_STRNCPY(xvvc.xiannv_name, m_xiannv_param.xiannv_name[xvvc.use_xiannv_id], sizeof(xvvc.xiannv_name));
		m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void*)&xvvc, sizeof(xvvc));
	}
}

void XiannvManager::FightOut(const char pos_list[MAX_XIANNV_POS])
{
	for (int i = 0; i < MAX_XIANNV_POS; i++)
	{
		if (pos_list[i] >= 0 && !m_xiannv_param.IsXiannvActive(pos_list[i]))
		{
			return;
		}
	}

	int fight_out_count = 1;

	// 屏蔽旧的光环技能0
// 	HaloParam *halo_param = m_role->GetHalo()->GetHaloParam();
// 	if (NULL != halo_param)
// 	{
// 		const HaloSkillConifg *skill_cfg = LOGIC_CONFIG->GetHaloConfig().GetSkillCfg(0, halo_param->skill_level_list[0]);
// 		// 策划的恶心需求，没激活光环也要强制获取一级技能的助战数
// 		if (0 == halo_param->skill_level_list[0])
// 		{
// 			skill_cfg = LOGIC_CONFIG->GetHaloConfig().GetSkillCfg(0, 1);
// 		}
// 
// 		if (NULL != skill_cfg)
// 		{
// 			fight_out_count += skill_cfg->param_a;
// 		}
// 	}

	char old_xiannv_id = m_xiannv_param.pos_list[0];

	for(int j = 0; j < fight_out_count && j < MAX_XIANNV_POS; j++)
	{
		m_xiannv_param.pos_list[j] = pos_list[j];
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANNV);
	this->SendAllXiannvInfo();
	//出战时设置仙女技能
	int pos_0 = m_xiannv_param.pos_list[0];
	const XiannvBaseCfg::ConfigItem *base_cfg = LOGIC_CONFIG->GetXiannvConfig().GetBaseCfg(pos_0);
	if (NULL != base_cfg)
	{
		XiannvItem &xiannv_item = m_xiannv_param.xn_item_list[pos_0];
		m_role->GetSkillManager()->AddXiannvSkill(base_cfg->skill_id, xiannv_item.xn_zizhi);
	}

	if (old_xiannv_id != pos_0)
	{
		this->BroadcastXiannvInfo();
	}

	UNSTD_STATIC_CHECK(4 == MAX_XIANNV_POS);
	this->PrintXiannv("XiannvFightOut", pos_list[0], pos_list[1], pos_list[2], pos_list[3]);
}

bool XiannvManager::OnActiveReq(int xiannv_id, short item_index)
{
	if (xiannv_id < 0 || xiannv_id >= MAX_XIANNV_COUNT)
	{
		return false;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		return false;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	const XiannvBaseCfg::ConfigItem *base_cfg = LOGIC_CONFIG->GetXiannvConfig().GetBaseCfg(xiannv_id);
	if (NULL == base_cfg)
	{
		return false;
	}

	if (!m_xiannv_param.IsXiannvActive(xiannv_id))
	{
		// 检查消耗
		ItemGridData item_data;
		if (!m_role->GetKnapsack()->GetItemGridData(item_index, &item_data))
		{
			return false;
		}

		if (item_data.item_id != base_cfg->active_item_id)
		{
			return false;
		}

		// 消耗
		if (!m_role->GetKnapsack()->ConsumeItemByIndex(item_index, 1, "XiannvActive")) 
		{
			return false;
		}

		// 生效
		this->ActiveXiannv(xiannv_id);
	}

	return true;
}

void XiannvManager::OnRoleLevelChange(int level)
{
	for(int i = 0;i < MAX_XIANNV_COUNT;i++)
	{
		if (m_xiannv_param.IsXiannvActive(i))
		{
			m_xiannv_param.xn_item_list[i].xn_level = level;
			this->SendXiannvInfo(i);
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANNV);
		}
	}

	this->CheckFuncOpen();
}

bool XiannvManager::OnAddZizhiReq(int xiannv_id, bool is_auto_buy)
{
	if (xiannv_id < 0 || xiannv_id >= MAX_XIANNV_COUNT)
	{
		return false;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		return false;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_xiannv_param.IsXiannvActive(xiannv_id))
	{
		m_role->NoticeNum(errornum::EN_XIANNV_NOT_ACTIVE);
		return false;
	}

	XiannvItem &xiannv_item = m_xiannv_param.xn_item_list[xiannv_id];

	const XiannvZiZhiCfg::ConfigItem *curr_cfg = LOGIC_CONFIG->GetXiannvConfig().GetZiZhiCfg(xiannv_id, xiannv_item.xn_zizhi);
	if (NULL == curr_cfg)
	{
		return false;
	}

	const XiannvZiZhiCfg::ConfigItem *next_cfg = LOGIC_CONFIG->GetXiannvConfig().GetZiZhiCfg(xiannv_id, xiannv_item.xn_zizhi + 1);
	if (NULL == next_cfg)
	{
		m_role->NoticeNum(errornum::EN_XIANNV_ZIZHI_MAX);
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	{
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = curr_cfg->uplevel_stuff_id;
		stuff_list[0].num = curr_cfg->uplevel_stuff_num;
		stuff_list[0].buyable = is_auto_buy;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
		{
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "XiannvManager::OnAddZizhiReq"))
		{
			return false;
		}
	}

	xiannv_item.xn_zizhi++;
 	// 仙女技能等级跟随仙女资质提升，不直接出战这个技能
// 	const XiannvBaseCfg::ConfigItem *base_cfg = LOGIC_CONFIG->GetXiannvConfig().GetBaseCfg(xiannv_id);
// 	if (NULL != base_cfg)
// 	{
// 		m_role->GetSkillManager()->AddXiannvSkill(base_cfg->skill_id, xiannv_item.xn_zizhi);
// 	}

	this->SendXiannvInfo(xiannv_id);

	m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_XIANNV, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_2, xiannv_item.xn_zizhi);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANNV);

	//m_role->NoticeNum(noticenum::NT_XIANNV_ADD_ZIZHI_SUCC);

	this->PrintXiannv("XiannvUpZizhi", xiannv_id, xiannv_item.xn_zizhi);
	
	EventHandler::Instance().OnXiannvUplevelZizhi(m_role, xiannv_item.xn_zizhi);

	return true;
}

const char * XiannvManager::GetName(short xiannv_id) 
{ 
	if (xiannv_id < 0 || xiannv_id >= MAX_XIANNV_COUNT)
		return NULL;

	return (char *)(&m_xiannv_param.xiannv_name[xiannv_id]);
}

void XiannvManager::GetXiannvInfo(GameName &xiannv_name)
{
	short used_xiannv_id = m_xiannv_param.pos_list[0];
	
	if (0 > used_xiannv_id || used_xiannv_id >= MAX_XIANNV_COUNT)
	{
		memset(xiannv_name,0, sizeof(GameName));
		return;
	}

	F_STRNCPY(xiannv_name, m_xiannv_param.xiannv_name[used_xiannv_id], sizeof(GameName));
}

int XiannvManager::GetActiveXiannvCount()
{
	int count = 0;
	for (int i = 0; i < MAX_XIANNV_COUNT; i++)
	{
		if (m_xiannv_param.active_xiannv_flag & (1 << i))
		{
			count++;
		}
	}
	return count;
}

int XiannvManager::GetActiveXiannvCountFast()
{
	int active_xiannv_flag = m_xiannv_param.active_xiannv_flag;
	// 统计仙女数量,不算幻化的
	int count = 0;
	while (active_xiannv_flag)
	{
		++count;
		active_xiannv_flag = active_xiannv_flag & (active_xiannv_flag - 1);			// 每次去掉一个1
	}
	return count;
}

int XiannvManager::GetMaxXiannvZizhi()
{
	int max_zizhi_level = 0;
	for (int i = 0; i < MAX_XIANNV_COUNT; i++)
	{
		if (0 == (m_xiannv_param.active_xiannv_flag & (1 << i)))
		{
			continue;
		}

		if (max_zizhi_level < m_xiannv_param.xn_item_list[i].xn_zizhi)
		{
			max_zizhi_level = m_xiannv_param.xn_item_list[i].xn_zizhi;
		}
	}

	return max_zizhi_level;
}

bool XiannvManager::ActiveHuanHua(int huanhua_id)
{
	if (!m_xiannv_param.IsXnActived())
	{
		m_role->NoticeNum(errornum::EN_XIANNV_ACTIVE_FIRST);
		return false;
	}

	const XiannvHuanHuaCfg::ConfigItem *huanhua_cfg = LOGIC_CONFIG->GetXiannvConfig().GetHuanHuaCfg(huanhua_id);
	if (NULL == huanhua_cfg)
	{
		return false;
	}

	if (m_xiannv_param.IsHuanHuaActive(huanhua_id))
	{
		m_role->NoticeNum(errornum::EN_XIANNV_ALREADY_ACTIVE);
		return false;
	}

	m_xiannv_param.ActiveHuanHua(huanhua_id);
	m_xiannv_param.xiannv_huanhua_level[huanhua_id] = 1;
	m_xiannv_param.huanhua_id = huanhua_id;

	m_role->GetRoleBigSmallGoal()->ActiveRoleBigSmallGoalSpecialAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_XIANNV, huanhua_id);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANNV);
	this->SendAllXiannvInfo();
	m_role->NoticeNum(noticenum::NT_XIANNV_ACTIVEHUANHUA_SUCC);

	this->PrintXiannv("XiannvActiveHuanhua", huanhua_id);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_XIANNV,huanhua_id,true);
	return true;
}

bool XiannvManager::OnActiveHuanHuaReq(int huanhua_id, short item_index)
{
	if (!m_xiannv_param.IsXnActived())
	{
		m_role->NoticeNum(errornum::EN_XIANNV_ACTIVE_FIRST);
		return false;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		return false;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	const XiannvHuanHuaCfg::ConfigItem *huanhua_cfg = LOGIC_CONFIG->GetXiannvConfig().GetHuanHuaCfg(huanhua_id);
	if (NULL == huanhua_cfg)
	{
		return false;
	}

	if (!m_xiannv_param.IsHuanHuaActive(huanhua_id))
	{
		// 检查消耗
		ItemGridData item_data;
		if (!m_role->GetKnapsack()->GetItemGridData(item_index, &item_data))
		{
			return false;
		}

		if (item_data.item_id != huanhua_cfg->active_item_id)
		{
			return false;
		}

		// 消耗
		if (!m_role->GetKnapsack()->ConsumeItemByIndex(item_index, 1, "XiannvActive")) 
		{
			return false;
		}

		// 生效
		this->ActiveHuanHua(huanhua_id);
	}

	return true;
}

bool XiannvManager::OnHuanHuaReq(int huanhua_id)
{
	if (m_xiannv_param.IsHuanHuaActive(huanhua_id) || huanhua_id == -1)
	{
		m_xiannv_param.huanhua_id = huanhua_id;
		this->SendAllXiannvInfo();
		this->BroadcastXiannvInfo();
	}

	return true;
}

bool XiannvManager::OnHuanHuaUplevelReq(int huanhua_id, bool is_auto_buy)
{
	if (huanhua_id < 0 || huanhua_id >= MAX_XIANNV_HUANHUA_COUNT)
	{
		return false;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		return false;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_xiannv_param.IsHuanHuaActive(huanhua_id))
	{
		m_role->NoticeNum(errornum::EN_PHANTOM_IMG_NOT_ACTIVE);
		return false;
	}

	int &xiannv_huanhua_level = m_xiannv_param.xiannv_huanhua_level[huanhua_id];

	const XiannvHuanHuaLevelCfg::ConfigItem *curr_cfg = LOGIC_CONFIG->GetXiannvConfig().GetHuanHuaLevelCfg(huanhua_id, xiannv_huanhua_level);
	if (NULL == curr_cfg)
	{
		return false;
	}

	const XiannvHuanHuaLevelCfg::ConfigItem *next_cfg = LOGIC_CONFIG->GetXiannvConfig().GetHuanHuaLevelCfg(huanhua_id, xiannv_huanhua_level + 1);
	if (NULL == next_cfg)
	{
		m_role->NoticeNum(errornum::EN_XIANNV_HUANHUAN_LEVEL_MAX);
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	{
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = curr_cfg->uplevel_stuff_id;
		stuff_list[0].num = curr_cfg->uplevel_stuff_num;
		stuff_list[0].buyable = is_auto_buy ? true : false;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
		{
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "XiannvManager::OnHuanHuaUplevelReq"))
		{
			return false;
		}
	}

	xiannv_huanhua_level++;

	this->SendAllXiannvInfo();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANNV);

	this->PrintXiannv("XiannvHuanHuaUpLevel", huanhua_id, xiannv_huanhua_level);

	return true;
}

void XiannvManager::GetRankXnData(int *xn_id, long long *xn_zizhi)
{
	if (NULL == xn_id || NULL == xn_zizhi) return;

	*xn_id = -1;
	*xn_zizhi = 0;

	for (int i = 0; i < MAX_XIANNV_COUNT; i++)
	{
		if (m_xiannv_param.xn_item_list[i].xn_zizhi > *xn_zizhi)
		{
			*xn_id = i;
			*xn_zizhi = m_xiannv_param.xn_item_list[i].xn_zizhi;
		}
	}
}

bool XiannvManager::ShengwuAddLingye(int total_lingye, const char * reason_str)
{
	if (NULL == reason_str) return false;

	int old_ling = m_xiannv_param.shengwu_lingye;
	m_xiannv_param.shengwu_lingye += total_lingye;
	if (m_xiannv_param.shengwu_lingye < 0)
	{
		m_xiannv_param.shengwu_lingye = INT_MAX;
	}

	//this->ShengwuSendChangeInfo(Protocol::SCXiannvShengwuChangeInfo::LINGYE, m_xiannv_param.day_free_miling_times, m_xiannv_param.cur_gold_miling_times, m_xiannv_param.day_fetch_ling_time, m_xiannv_param.shengwu_lingye);

	gamelog::g_log_xiannv.printf(LL_INFO, "XiannvManager::ShengwuAddLingye reason[%s] role(%d,%s) ling[%d -> %d]", reason_str, m_role->GetUID(), m_role->GetName(), old_ling, m_xiannv_param.shengwu_lingye);

	return true;
}

void XiannvManager::OnShengwuReq(short req_type, short param1, int param2, int param3)
{
	switch (req_type)
	{
	case Protocol::CSXiannvShengwuReq::NORMAL_CHOU_EXP:
		{
			this->ShengwuChouExp(NvshenShengwuExpChouCfg::CHOU_TYPE_NORMAL, param1 > 0, param2 > 0, param3 > 0);
		}
		break;

	case Protocol::CSXiannvShengwuReq::PERFECT_CHOU_EXP:
		{
			this->ShengwuChouExp(NvshenShengwuExpChouCfg::CHOU_TYPE_PERFECT, param1 > 0, param2 > 0, param3 > 0);
		}
		break;

	case Protocol::CSXiannvShengwuReq::FETCH_EXP:
		{
			if (m_xiannv_param.shengwu_chou_id >= 0)
			{
				static Protocol::SCXiannvShengwuChouExpResult swcer;
				swcer.is_auto_fetch = 0;
				swcer.add_exp_count = 0;
				{
					static Protocol::SCXiannvShengwuChouExpResult::FetchItem &item = swcer.add_exp_list[swcer.add_exp_count];
					item.shengwu_id = m_xiannv_param.shengwu_chou_id;
					item.add_exp = 0;
					for (int i = 0; i < XIANNV_SHENGWU_CHOU_EXP_COUNT; ++i)
					{
						item.add_exp += m_xiannv_param.shengwu_chou_exp_list[i];
					}
					++swcer.add_exp_count;
				}
				EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&swcer, sizeof(swcer));
			}

			// 发完通知再获取经验，不然数据被重置了
			this->ShengwuFetchExp();
			this->SendShengwuChouExpList(0);
		}
		break;

	//case Protocol::CSXiannvShengwuReq::UPGRADE_GRID:
	//{
	//	this->ShengwuUpgradeGrid(param1);
	//}
	//break;

	//case Protocol::CSXiannvShengwuReq::CHOU_LING:
	//	{
	//		this->ShengwuChouLing();
	//	}
	//	break;

	//case Protocol::CSXiannvShengwuReq::FETCH_LING:
	//	{
	//		if (this->ShengwuFetchLing(param1 > 0))
	//		{
	//			// 领取完毕，系统帮玩家免费刷新一次
	//			this->ShengwuSystemChouLing();
	//		}
	//	}
	//	break;
	case Protocol::CSXiannvShengwuReq::UPGRADE_EXP:
		{
			this->ShengwuUpgrade(param1);
		}
		break;
	default: break;
	}
}

void XiannvManager::OnShengwuGm(int type, int param1, int param2, int param3)
{
	enum
	{
		TYPE_ADD_LINGYE = 0,		// gm xnshengwu:0 xxx 0 0		增加灵液数量xxx
		TYPE_RESET,					// gm xnshengwu:1 0 0 0			重置所有圣物数据
	};

	switch (type)
	{
	case TYPE_ADD_LINGYE:
		{
			m_xiannv_param.shengwu_lingye += param1;
			this->ShengwuSendChangeInfo(Protocol::SCXiannvShengwuChangeInfo::LINGYE, m_xiannv_param.day_free_miling_times, m_xiannv_param.cur_gold_miling_times, m_xiannv_param.day_fetch_ling_time, m_xiannv_param.shengwu_lingye);
		}
		break;

	case TYPE_RESET:
		{
			m_xiannv_param.shengwu_lingye = 0;
			m_xiannv_param.shengwu_chou_id = -1;
			memset(m_xiannv_param.shengwu_chou_exp_list, 0, sizeof(m_xiannv_param.shengwu_chou_exp_list));
			memset(m_xiannv_param.all_shengwu_list, 0, sizeof(m_xiannv_param.all_shengwu_list));
			memset(m_xiannv_param.grid_level_list, 0, sizeof(m_xiannv_param.grid_level_list));
			m_xiannv_param.day_free_miling_times = 0;
			m_xiannv_param.cur_gold_miling_times = 0;
			m_xiannv_param.day_fetch_ling_time = 0;
			m_xiannv_param.shengwu_essence = 0;

			this->SendAllXiannvInfo();
		}
		break;

	default: break;
	}
}

void XiannvManager::PrintXiannv(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	const int special_image_hex_len = MAX_XIANNV_ACTIVE_SPECIAL_IMAGE_BYTE * 2 + 1;
	char active_special_image_hex[special_image_hex_len] = { 0 };
	BinToHex((const char*)m_xiannv_param.active_huanhua_flag, sizeof(m_xiannv_param.active_huanhua_flag), active_special_image_hex);
	gamelog::g_log_xiannv.printf(LL_INFO, "Xiannv role(%d,%s) role_level[%d], %s(%d,%d,%d,%d,%d) "
		"active_xiannv_flag:%d active_huanhua_flag:%s huanhua_id:%d", 
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), reason, param1, param2, param3, param4, param5,
		(int)m_xiannv_param.active_xiannv_flag, active_special_image_hex, (int)m_xiannv_param.huanhua_id);
}

bool XiannvManager::ShengwuChouExpOnce(int chou_type, bool is_auto_buy)
{
	int not_full_level_shengwu_id_count = 0;
	int not_full_level_shengwu_id_list[XIANNV_SHENGWU_MAX_ID + 1];

	int temp_not_full_level_shengwu_id_count = 0;
	int temp_not_full_level_shengwu_id_list[XIANNV_SHENGWU_MAX_ID + 1];

	int min_shengwu_level = 0;

	// 遍历所有圣物，找出所有未满级的圣物ID
	for (int shengwu_id = 0; shengwu_id <= XIANNV_SHENGWU_MAX_ID && not_full_level_shengwu_id_count <= XIANNV_SHENGWU_MAX_ID; ++shengwu_id)
	{
		NvshenShengwuItem &shengwu_item = m_xiannv_param.all_shengwu_list[shengwu_id];
		const NvshenShengwuInfoCfg::ShengwuItem *shengwu_next_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(shengwu_id, shengwu_item.level + 1);
		if (NULL != shengwu_next_level_cfg)
		{
			temp_not_full_level_shengwu_id_list[temp_not_full_level_shengwu_id_count++] = shengwu_id;
		}

		if (0 == shengwu_id || min_shengwu_level > shengwu_item.level)
		{
			min_shengwu_level = shengwu_item.level;
		}
	}

	const NvshenShengwuChouConsumeCfg* consuem_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuChouConsumeCfg(min_shengwu_level);
	if (NULL == consuem_cfg)
	{
		return false;
	}

	for (int valid_shengwu = 0; valid_shengwu < temp_not_full_level_shengwu_id_count && valid_shengwu <= XIANNV_SHENGWU_MAX_ID; ++valid_shengwu)
	{
		int temp_shengwu_id = temp_not_full_level_shengwu_id_list[valid_shengwu];
		if (temp_shengwu_id >= 0 && temp_shengwu_id <= XIANNV_SHENGWU_MAX_ID)
		{
			NvshenShengwuItem &shengwu_item = m_xiannv_param.all_shengwu_list[temp_shengwu_id];
			if (shengwu_item.level <= consuem_cfg->max_level)
			{
				not_full_level_shengwu_id_list[not_full_level_shengwu_id_count++] = temp_shengwu_id;
			}
		}
	}

	if (not_full_level_shengwu_id_count <= 0)
	{
		m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_LEVEL_HAS_FULL);
		return false;
	}
	
	int target_shengwu_index = RandomNum(not_full_level_shengwu_id_count);
	int target_shengwu_id = not_full_level_shengwu_id_list[target_shengwu_index];

	const NvshenShengwuExpChouCfg *chou_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuExpChouCfg(chou_type);
	if (NULL == chou_cfg)
	{
		return false;
	}

	short add_exp_list[XIANNV_SHENGWU_CHOU_EXP_COUNT]; memset(add_exp_list, 0, sizeof(add_exp_list));
	for (int j = 0; j < XIANNV_SHENGWU_CHOU_EXP_COUNT; ++j)
	{
		int random_num = RandomNum(100);
		if (random_num < 20)
		{
			add_exp_list[j] += chou_cfg->rate1_exp;
		}
		else if (random_num < 40)
		{
			add_exp_list[j] += chou_cfg->rate2_exp;
		}
		else
		{
			add_exp_list[j] += chou_cfg->rate3_exp;
		}
	}

	// 消耗
	if (NvshenShengwuExpChouCfg::CHOU_TYPE_PERFECT == chou_type) // 完美抽直接消耗元宝
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(consuem_cfg->chou_exp_gold, "XiannvManager::ShengwuChouExp"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return false;
		}
	}
	else if (NvshenShengwuExpChouCfg::CHOU_TYPE_NORMAL == chou_type) // 普通抽消耗物品
	{
		if (!m_role->GetKnapsack()->ConsumeItem(consuem_cfg->chou_exp_stuff, 1, "XiannvManager::ShengwuChouExp"))
		{
			if (is_auto_buy)
			{
				int gold_price = 0;
				int bind_gold_price = 0;
				if (!LOGIC_CONFIG->GetShop().GetShopItemGoldPrice(consuem_cfg->chou_exp_stuff, &gold_price, &bind_gold_price) || gold_price <= 0)
				{
					return false;
				}

				if (!m_role->GetKnapsack()->GetMoney()->UseGold(gold_price, "XiannvManager::ShengwuChouExpOnce"))
				{
					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return false;
				}

				if (!m_role->GetKnapsack()->GetMoney()->UseGoldBind(bind_gold_price, "XiannvManager::ShengwuChouExpOnce"))
				{
					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return false;
				}
			}
			else
			{
				m_role->NoticeNum(errornum::EN_ITEM_NUM_NOT_ENOUGH);
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	// 给予
	m_xiannv_param.shengwu_chou_id = target_shengwu_id;
	memcpy(m_xiannv_param.shengwu_chou_exp_list, add_exp_list, sizeof(m_xiannv_param.shengwu_chou_exp_list));

	gamelog::g_log_xiannv.printf(LL_INFO, "XiannvManager::ShengwuChouExp role(%d,%s) shengwu_chou_id(%d), add_exp_list[%d, %d, %d, %d, %d, %d]",
		m_role->GetUID(), m_role->GetName(), target_shengwu_id, add_exp_list[0], add_exp_list[1], add_exp_list[2], add_exp_list[3], add_exp_list[4], add_exp_list[5]);

	return true;
}

void XiannvManager::ShengwuChouExp(int chou_type, bool is_auto_buy, bool is_auto_fetch, bool is_multi_chou)
{
	if (m_xiannv_param.shengwu_chou_exp_list[0] > 0) // 只检查一个就够了，6个格子肯定要么都有经验，要么都是0
	{
		m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_EXP_HAS_NOT_FETCH);
		return;
	}

	const XiannvOtherCfg *other_cfg = LOGIC_CONFIG->GetXiannvConfig().GetOtherCfg();
	if (NULL == other_cfg) return;
	
	int chou_times = is_multi_chou ? MAX_EXP_CHOU_TIME : 1;

	bool vip_can_auto_buy = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_XIANNV_SHENGWU_AUTO_FETCH) > 0;
	if (is_auto_fetch && !vip_can_auto_buy) // 指定vip允许自动获取奖励
	{
		return;
	}
	bool vip_can_multi_chou = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_XIANNV_SHENGWU_AUTO_FETCH) > 0;
	if (is_multi_chou && !vip_can_multi_chou) // 指定vip允许十连抽
	{
		return;
	}

	if (chou_times > 1) // 如果是多次抽取，则强制设置为自动获取奖励
	{
		is_auto_fetch = true;
	}

	if (!is_auto_buy)
	{
		int min_shengwu_level = 0;
		// 遍历所有圣物，找出圣物最低等级
		for (int shengwu_id = 0; shengwu_id <= XIANNV_SHENGWU_MAX_ID; ++shengwu_id)
		{
			NvshenShengwuItem &shengwu_item = m_xiannv_param.all_shengwu_list[shengwu_id];
			if (0 == shengwu_id || min_shengwu_level > shengwu_item.level)
			{
				min_shengwu_level = shengwu_item.level;
			}
		}

		const NvshenShengwuChouConsumeCfg* consuem_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuChouConsumeCfg(min_shengwu_level);
		if (NULL == consuem_cfg)
		{
			return;
		}

		if (NvshenShengwuExpChouCfg::CHOU_TYPE_NORMAL == chou_type && (m_role->GetKnapsack()->Count(consuem_cfg->chou_exp_stuff) < chou_times))
		{
			chou_times = m_role->GetKnapsack()->Count(consuem_cfg->chou_exp_stuff);
		}

		if (NvshenShengwuExpChouCfg::CHOU_TYPE_NORMAL == chou_type && m_role->GetKnapsack()->Count(consuem_cfg->chou_exp_stuff) < chou_times)
		{
			m_role->NoticeNum(errornum::EN_ITEM_NUM_NOT_ENOUGH);
			return;
		}
		else if (NvshenShengwuExpChouCfg::CHOU_TYPE_PERFECT == chou_type && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(consuem_cfg->chou_exp_gold * chou_times))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}
	
	int shengwu_exp_list[XIANNV_SHENGWU_MAX_ID + 1]; memset(shengwu_exp_list, 0, sizeof(shengwu_exp_list));

	short add_exp_list[XIANNV_SHENGWU_CHOU_EXP_COUNT]; memset(add_exp_list, 0, sizeof(add_exp_list));
	int total_add_exp = 0;
	for (int i = 0; i < chou_times; ++i)
	{
		if (!this->ShengwuChouExpOnce(chou_type, is_auto_buy))
		{
			return;
		}

		for (int j = 0; j < XIANNV_SHENGWU_CHOU_EXP_COUNT; ++j)
		{
			shengwu_exp_list[m_xiannv_param.shengwu_chou_id] += m_xiannv_param.shengwu_chou_exp_list[j];
		}

		if (is_auto_fetch)
		{
			this->ShengwuFetchExp();
		}
	}

	this->SendShengwuChouExpList(chou_type);

	if (is_auto_fetch)
	{
		static Protocol::SCXiannvShengwuChouExpResult swcer;
		swcer.is_auto_fetch = is_auto_fetch ? 1 : 0;
		swcer.add_exp_count = 0;
		for (int shengwu_id = 0; shengwu_id <= XIANNV_SHENGWU_MAX_ID; ++shengwu_id)
		{
			if (shengwu_exp_list[shengwu_id] > 0)
			{
				swcer.add_exp_list[swcer.add_exp_count].shengwu_id = shengwu_id;
				swcer.add_exp_list[swcer.add_exp_count].add_exp = shengwu_exp_list[shengwu_id];
				++swcer.add_exp_count;
			}
		}
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&swcer, sizeof(swcer));
	}

	gamelog::g_log_xiannv.printf(LL_INFO, "XiannvManager::ShengwuChouExp role(%d,%s) chou_times[%d] total_add_exp[%d]",
		m_role->GetUID(), m_role->GetName(), chou_times, total_add_exp);
}

void XiannvManager::ShengwuFetchExp()
{
	if (m_xiannv_param.shengwu_chou_exp_list[0] <= 0 || m_xiannv_param.shengwu_chou_id < 0 || m_xiannv_param.shengwu_chou_id > XIANNV_SHENGWU_MAX_ID)
	{
		return;
	}

	NvshenShengwuItem &shengwu_item = m_xiannv_param.all_shengwu_list[m_xiannv_param.shengwu_chou_id];
	
	const NvshenShengwuInfoCfg::ShengwuItem *shengwu_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(m_xiannv_param.shengwu_chou_id, shengwu_item.level);
	if (NULL == shengwu_level_cfg)
	{
		return;
	}
	const NvshenShengwuInfoCfg::ShengwuItem *shengwu_next_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(m_xiannv_param.shengwu_chou_id, shengwu_item.level + 1);
	if (NULL == shengwu_next_level_cfg)
	{
		m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_LEVEL_HAS_FULL);
		return;
	}

	// 给予
	int old_exp = shengwu_item.exp;
	for (int i = 0; i < XIANNV_SHENGWU_CHOU_EXP_COUNT; ++i)
	{
		shengwu_item.exp += m_xiannv_param.shengwu_chou_exp_list[i];
	}
	if (shengwu_item.exp < 0)
	{
		shengwu_item.exp = INT_MAX;
	}

	int old_level = shengwu_item.level;
	int MAX_LOOP_TIMES = 100;
	for (int i = 0; i < MAX_LOOP_TIMES && shengwu_item.exp >= shengwu_level_cfg->upgrade_need_exp; ++i)
	{
		shengwu_item.exp -= shengwu_level_cfg->upgrade_need_exp;
		++shengwu_item.level;
		
		int length = 0;
		if (NvshenShengwuInfoCfg::BROADCAST_TYPE_1 == shengwu_next_level_cfg->broadcast_type)
		{
			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xiannv_shengwu_upgrade,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), int(m_xiannv_param.shengwu_chou_id), int(shengwu_item.level));
		}
		else if (NvshenShengwuInfoCfg::BROADCAST_TYPE_2 == shengwu_next_level_cfg->broadcast_type)
		{
			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xiannv_shengwu_skill_upgrade,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), shengwu_next_level_cfg->skill_id, shengwu_next_level_cfg->skill_level);
		}
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}



		this->RecalcSkillList();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANNV);

		m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_XIANNV_SHENWU_CAP); //嘉年华

		shengwu_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(m_xiannv_param.shengwu_chou_id, shengwu_item.level);
		shengwu_next_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(m_xiannv_param.shengwu_chou_id, shengwu_item.level + 1);
		if (NULL == shengwu_level_cfg || NULL == shengwu_next_level_cfg) break;
	}
	
	// 消耗
	short cur_shengwu_id = m_xiannv_param.shengwu_chou_id;
	m_xiannv_param.shengwu_chou_id = -1;
	memset(m_xiannv_param.shengwu_chou_exp_list, 0, sizeof(m_xiannv_param.shengwu_chou_exp_list));

	this->ShengwuSendChangeInfo(Protocol::SCXiannvShengwuChangeInfo::SHENGWU_INFO, cur_shengwu_id, shengwu_item.level, 0, shengwu_item.exp);

	gamelog::g_log_xiannv.printf(LL_INFO, "XiannvManager::ShengwuFetchExp role(%d,%s) shengwu_chou_id[%d] level[%d -> %d] exp[%d -> %d]",
		m_role->GetUID(), m_role->GetName(), m_xiannv_param.shengwu_chou_id, old_level, shengwu_item.level, old_exp, shengwu_item.exp);
}

void XiannvManager::ShengwuFetchUpgradeExp()
{
	short shengwu_id = m_xiannv_param.upgrade_shengwu_id;
	if (shengwu_id < 0 || shengwu_id > XIANNV_SHENGWU_MAX_ID)
	{
		return;
	}

	NvshenShengwuItem &shengwu_item = m_xiannv_param.all_shengwu_list[m_xiannv_param.upgrade_shengwu_id];

	const NvshenShengwuInfoCfg::ShengwuItem *shengwu_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(shengwu_id, shengwu_item.level);
	if (NULL == shengwu_level_cfg)
	{
		return;
	}
	const NvshenShengwuInfoCfg::ShengwuItem *shengwu_next_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(shengwu_id, shengwu_item.level + 1);
	if (NULL == shengwu_next_level_cfg)
	{
		m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_LEVEL_HAS_FULL);
		return;
	}

	// 给予
	int old_exp = shengwu_item.exp;
	shengwu_item.exp += m_xiannv_param.upgrade_exp_list[shengwu_id];
	if (shengwu_item.exp < 0)
	{
		shengwu_item.exp = INT_MAX;
	}

	int old_level = shengwu_item.level;
	int MAX_LOOP_TIMES = 100;
	for (int i = 0; i < MAX_LOOP_TIMES && shengwu_item.exp >= shengwu_level_cfg->upgrade_need_exp; ++i)
	{
		shengwu_item.exp -= shengwu_level_cfg->upgrade_need_exp;
		++shengwu_item.level;

		int length = 0;
		if (NvshenShengwuInfoCfg::BROADCAST_TYPE_1 == shengwu_next_level_cfg->broadcast_type)
		{
			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xiannv_shengwu_upgrade,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), int(shengwu_id), int(shengwu_item.level));
		}
		else if (NvshenShengwuInfoCfg::BROADCAST_TYPE_2 == shengwu_next_level_cfg->broadcast_type)
		{
			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xiannv_shengwu_skill_upgrade,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), shengwu_next_level_cfg->skill_id, shengwu_next_level_cfg->skill_level);
		}
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}

		this->RecalcSkillList();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANNV);

		m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_XIANNV_SHENWU_CAP); //嘉年华

		shengwu_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(shengwu_id, shengwu_item.level);
		shengwu_next_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(shengwu_id, shengwu_item.level + 1);
		if (NULL == shengwu_level_cfg || NULL == shengwu_next_level_cfg) break;
	}

	// 消耗
	m_xiannv_param.shengwu_chou_id = -1;
	memset(m_xiannv_param.upgrade_exp_list, 0, sizeof(m_xiannv_param.upgrade_exp_list));

	this->ShengwuSendChangeInfo(Protocol::SCXiannvShengwuChangeInfo::SHENGWU_INFO, shengwu_id, shengwu_item.level, 0, shengwu_item.exp);

	gamelog::g_log_xiannv.printf(LL_INFO, "XiannvManager::ShengwuFetchUpgradeExp role(%d,%s) upgrade_shengwu_id[%d] level[%d -> %d] exp[%d -> %d]",
		m_role->GetUID(), m_role->GetName(), shengwu_id, old_level, shengwu_item.level, old_exp, shengwu_item.exp);
}

void XiannvManager::ShengwuUpgradeExp(int shengwu_id)
{
	if (shengwu_id < 0 || shengwu_id > XIANNV_SHENGWU_MAX_ID)
	{
		return;
	}
	if (m_xiannv_param.upgrade_exp_list[shengwu_id] > 0) 
	{
		m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_EXP_HAS_NOT_FETCH);
		return;
	}
	NvshenShengwuItem &shengwu_item = m_xiannv_param.all_shengwu_list[shengwu_id];
	const NvshenShengwuUpGradeCfg* consuem_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuUpgradeCfg(shengwu_id);
	if (NULL == consuem_cfg)
	{
		return;
	}
	const NvshenShengwuInfoCfg::ShengwuItem *shengwu_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(shengwu_id, shengwu_item.level);
	if (NULL == shengwu_level_cfg)
	{
		m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_LEVEL_HAS_FULL);
		return;
	}
	const NvshenShengwuInfoCfg::ShengwuItem *shengwu_next_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(shengwu_id, shengwu_item.level + 1);
	if (NULL == shengwu_next_level_cfg)
	{
		m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_LEVEL_HAS_FULL);
		return;
	}
	//判断圣物是否已经满级
	if (shengwu_item.level >= consuem_cfg->max_upgrade_level)
	{
		m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_LEVEL_HAS_FULL);
		return;
	}
	
	switch (consuem_cfg->consume_type)
	{
	case NvshenShengwuUpGradeCfg::CONSUME_TYPE_ESSENCE:
	{
		if (!this->AddShengwuExp(shengwu_id))
		{
			m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_ESSENCE_NOT_ENOUGH);
			return;
		}
	}
	break;
	case NvshenShengwuUpGradeCfg::CONSUME_TYPE_ITEM:
	{
		if (!m_role->GetKnapsack()->ConsumeItem(consuem_cfg->upgrade_stuff_id, shengwu_level_cfg->upgrade_need_stuff_number, "XiannvManager::ShengwuUpgradeExp"))
		{
			m_role->NoticeNum(errornum::EN_ITEM_NUM_NOT_ENOUGH);
			return;
		}
	}
	break;
	case NvshenShengwuUpGradeCfg::CONSUME_TYPE_ESSENCE_AND_ITEM:
	{
		if (!this->AddShengwuExp(shengwu_id))
		{
			m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_ESSENCE_NOT_ENOUGH);
			return;
		}
		if (!m_role->GetKnapsack()->ConsumeItem(consuem_cfg->upgrade_stuff_id, shengwu_level_cfg->upgrade_need_stuff_number, "XiannvManager::ShengwuUpgradeExp"))
		{
			m_role->NoticeNum(errornum::EN_ITEM_NUM_NOT_ENOUGH);
			return;
		}
	}
	break;
	default: return;
	}
	m_xiannv_param.upgrade_shengwu_id = shengwu_id;
	m_xiannv_param.upgrade_exp_list[shengwu_id] += consuem_cfg->exp_reward * shengwu_level_cfg->upgrade_need_stuff_number;
	this->ShengwuFetchUpgradeExp();
}

void XiannvManager::ShengwuActive(int shengwu_id)
{
	if (shengwu_id < 0 || shengwu_id > XIANNV_SHENGWU_MAX_ID)
	{
		return;
	}
	
	const NvshenShengwuUpGradeCfg* cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuUpgradeCfg(shengwu_id);
	if (NULL == cfg)
	{
		return;
	}
	// 统计仙女数量,不算幻化的
	int xiannv_count = this->GetActiveXiannvCountFast();
	if (xiannv_count < cfg->active_need_fairy)
	{
		m_role->NoticeNum(errornum::EN_XIANNV_COUNT_NOT_ENOUGH);
		return;
	}
	m_xiannv_param.ActiveShengwu(shengwu_id);
	this->RecalcSkillList();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANNV);

	NvshenShengwuItem &shengwu_item = m_xiannv_param.all_shengwu_list[shengwu_id];
	this->ShengwuSendChangeInfo(Protocol::SCXiannvShengwuChangeInfo::SHENGWU_INFO, shengwu_id, shengwu_item.level, 0, shengwu_item.exp);
}

void XiannvManager::ShengwuUpgrade(int shengwu_id)
{
	if (m_xiannv_param.IsShengwuActive(shengwu_id))
	{
		this->ShengwuUpgradeExp(shengwu_id);
	}
	else
	{
		this->ShengwuActive(shengwu_id);
	}
}

void XiannvManager::GMSetShengwuLevel(int shengwu_id, int level)
{
#ifndef _DEBUG
	return;
#endif
	if (shengwu_id < 0 || shengwu_id > XIANNV_SHENGWU_MAX_ID || level < 0)
	{
		return;
	}

	NvshenShengwuItem &shengwu_item = m_xiannv_param.all_shengwu_list[shengwu_id];
	shengwu_item.level = level;
	this->ShengwuSendChangeInfo(Protocol::SCXiannvShengwuChangeInfo::SHENGWU_INFO, shengwu_id, shengwu_item.level, 0, shengwu_item.exp);
}

void XiannvManager::ShengwuUpgradeGrid(short grid_id)
{
	if (grid_id < 0 || grid_id > XIANNV_SHENGWU_GONGMING_MAX_GRID_ID)
	{
		return;
	}

	const NvshenShengwuGongmingCfg::ItemCfg *grid_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetGongmingGridCfg(grid_id, m_xiannv_param.grid_level_list[grid_id]);
	if (NULL == grid_level_cfg)
	{
		return;
	}
	const NvshenShengwuGongmingCfg::ItemCfg *grid_next_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetGongmingGridCfg(grid_id, m_xiannv_param.grid_level_list[grid_id] + 1);
	if (NULL == grid_next_level_cfg)
	{
		m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_LEVEL_HAS_FULL);
		return;
	}

	if (m_xiannv_param.shengwu_lingye < grid_level_cfg->upgrade_need_ling)
	{
		m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_GRID_LINGYE_NOT_ENOUGH);
		return;
	}

	const std::set<int> *depend_grid_set = LOGIC_CONFIG->GetXiannvConfig().GetGridDependCfg(grid_id);
	if (NULL != depend_grid_set) // 有依赖关系才做检测
	{
		for (std::set<int>::const_iterator grid_it = depend_grid_set->begin(); grid_it != depend_grid_set->end(); ++grid_it)
		{
			int tmp_grid_id = *grid_it;
			if (tmp_grid_id < 0 || tmp_grid_id > XIANNV_SHENGWU_GONGMING_MAX_GRID_ID)
			{
				continue;
			}

			if (m_xiannv_param.grid_level_list[grid_id] >= m_xiannv_param.grid_level_list[tmp_grid_id])
			{
				m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_GRID_DEPEND_LEVEL_LIMIT);
				return;
			}
		}
	}

	// 消耗
	int old_lingye = m_xiannv_param.shengwu_lingye;
	m_xiannv_param.shengwu_lingye -= grid_level_cfg->upgrade_need_ling;

	// 给予
	int old_level = m_xiannv_param.grid_level_list[grid_id];
	++m_xiannv_param.grid_level_list[grid_id];


	if (grid_next_level_cfg->is_broadcast)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xiannv_gongming_upgrade,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), int(m_xiannv_param.grid_level_list[grid_id]));
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	//this->ShengwuSendChangeInfo(Protocol::SCXiannvShengwuChangeInfo::LINGYE, m_xiannv_param.day_free_miling_times, m_xiannv_param.cur_gold_miling_times, m_xiannv_param.day_fetch_ling_time, m_xiannv_param.shengwu_lingye);
	this->ShengwuSendChangeInfo(Protocol::SCXiannvShengwuChangeInfo::GRID_INFO, grid_id, m_xiannv_param.grid_level_list[grid_id]);

	this->RecalcSkillList();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANNV);

	gamelog::g_log_xiannv.printf(LL_INFO, "XiannvManager::ShengwuUpgradeGrid role(%d,%s) grid_id[%d] level[%d -> %d] lingye[%d -> %d]",
		m_role->GetUID(), m_role->GetName(), grid_id, old_level, m_xiannv_param.grid_level_list[grid_id], old_lingye, m_xiannv_param.shengwu_lingye);
}

void XiannvManager::ShengwuSystemChouLing()
{
	const static int SYSTEM_CHOU_LING_RATE = 50; // 系统抽灵的概率

	bool next_can_be_ling = true;
	int cur_ling_count = 0;
	for (int ling_index = 0; ling_index < XIANNV_SHENGWU_MILING_FONT_COUNT; ++ling_index)
	{
		if (next_can_be_ling)
		{
			const NvshenShengwuChouLingCfg *chou_ling_cfg = LOGIC_CONFIG->GetXiannvConfig().GetChouLingCfg(cur_ling_count);
			if (NULL == chou_ling_cfg)
			{
				continue;
			}

			if (RandomNum(100) < SYSTEM_CHOU_LING_RATE)
			{
				m_xiannv_param.miling_list[ling_index] = 0;
			}
			else
			{
				m_xiannv_param.miling_list[ling_index] = RandomNum(XIANNV_SHENGWU_MILING_TYPE_COUNT - 1) + 1; // 随机抽0以外的值
				next_can_be_ling = false;
			}
		}
		else
		{
			m_xiannv_param.miling_list[ling_index] = RandomNum(XIANNV_SHENGWU_MILING_TYPE_COUNT - 1) + 1; // 随机抽0以外的值
				next_can_be_ling = false;
		}

		if (0 == m_xiannv_param.miling_list[ling_index])
		{
			++cur_ling_count;
			if (cur_ling_count >= SYSTEM_CHOU_MAX_LING_COUNT)
			{
				next_can_be_ling = false;
			}
		}
	}

	this->SendMilingList();

	gamelog::g_log_xiannv.printf(LL_INFO, "XiannvManager::ShengwuSystemChouLing role(%d,%s) new_ling_list[%d,%d,%d,%d,%d,%d]", m_role->GetUID(), m_role->GetName(),
		m_xiannv_param.miling_list[0], m_xiannv_param.miling_list[1], m_xiannv_param.miling_list[2], m_xiannv_param.miling_list[3], m_xiannv_param.miling_list[4], m_xiannv_param.miling_list[5]);
}

void XiannvManager::ShengwuChouLing()
{
	char tmp_miling_list[XIANNV_SHENGWU_MILING_FONT_COUNT];
	memcpy(tmp_miling_list, m_xiannv_param.miling_list, sizeof(tmp_miling_list));

	bool use_free_chou = m_xiannv_param.day_free_miling_times < LOGIC_CONFIG->GetXiannvConfig().GetOtherCfg()->chou_ling_free_times;

	int cur_ling_count = 0;
	int cur_new_ling_count = 0;		// 当前次抽取，额外获取到的0（灵）个数（排除已有的灵）
	bool next_can_be_ling = true;	// 下一个有没有几率抽到0（灵）
	for (int ling_index = 0; ling_index < XIANNV_SHENGWU_MILING_FONT_COUNT; ++ling_index)
	{
		// 已经是0则不会重新随机
		if (0 == tmp_miling_list[ling_index])
		{
			++cur_ling_count;
			continue;
		}
		
		if (next_can_be_ling)
		{
			const NvshenShengwuChouLingCfg *chou_ling_cfg = LOGIC_CONFIG->GetXiannvConfig().GetChouLingCfg(cur_ling_count);
			if (NULL == chou_ling_cfg)
			{
				continue;
			}

			int rate = chou_ling_cfg->free_rate;
			if (!use_free_chou)
			{
				rate = chou_ling_cfg->gold_rate;
			}

			if (RandomNum(100) < rate) // 抽到灵
			{
				tmp_miling_list[ling_index] = 0;
			}
			else
			{
				tmp_miling_list[ling_index] = RandomNum(XIANNV_SHENGWU_MILING_TYPE_COUNT - 1) + 1; // 随机抽0以外的值
			}
		}
		else
		{
			tmp_miling_list[ling_index] = RandomNum(XIANNV_SHENGWU_MILING_TYPE_COUNT - 1) + 1; // 随机抽0以外的值
		}
		
		if (0 == tmp_miling_list[ling_index])
		{
			++cur_ling_count;
			++cur_new_ling_count;
			if (cur_new_ling_count >= CHOU_MAX_LING_COUNT)
			{
				next_can_be_ling = false;
			}
		}
		else // 抽到一次非0，则下一次不再有机会抽到了
		{
			next_can_be_ling = false;
		}
	}

	// 消耗
	if (use_free_chou)
	{
		++m_xiannv_param.day_free_miling_times;
	}
	else
	{
		int consume_gold = LOGIC_CONFIG->GetXiannvConfig().GetChouLingConsumeGold(m_xiannv_param.cur_gold_miling_times);
		if (consume_gold <= 0 || !m_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "XiannvManager::ShengwuChouLing"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		++m_xiannv_param.cur_gold_miling_times;
	}

	gamelog::g_log_xiannv.printf(LL_INFO, "XiannvManager::ShengwuChouLing role(%d,%s) old_ling_list[%d,%d,%d,%d,%d,%d] new_ling_list[%d,%d,%d,%d,%d,%d]", m_role->GetUID(), m_role->GetName(),
		m_xiannv_param.miling_list[0], m_xiannv_param.miling_list[1], m_xiannv_param.miling_list[2], m_xiannv_param.miling_list[3], m_xiannv_param.miling_list[4], m_xiannv_param.miling_list[5],
		tmp_miling_list[0], tmp_miling_list[1], tmp_miling_list[2], tmp_miling_list[3], tmp_miling_list[4], tmp_miling_list[5]);

	// 给予
	memcpy(m_xiannv_param.miling_list, tmp_miling_list, sizeof(m_xiannv_param.miling_list));

	this->SendMilingList();
	//this->ShengwuSendChangeInfo(Protocol::SCXiannvShengwuChangeInfo::LINGYE, m_xiannv_param.day_free_miling_times, m_xiannv_param.cur_gold_miling_times, m_xiannv_param.day_fetch_ling_time, m_xiannv_param.shengwu_lingye);
}

bool XiannvManager::ShengwuFetchLing(bool is_double_fetch)
{
	if (m_xiannv_param.day_fetch_ling_time >= LOGIC_CONFIG->GetXiannvConfig().GetOtherCfg()->fetch_ling_time)
	{
		m_role->NoticeNum(errornum::EN_XIANNV_SHENGWU_DAY_FETCH_LING_TIMES_LIMIT);
		return false;
	}

	int total_lingye = 0;
	int total_ling_count = 0;
	for (int ling_index = 0; ling_index < XIANNV_SHENGWU_MILING_FONT_COUNT; ++ling_index)
	{
		if (0 == m_xiannv_param.miling_list[ling_index])
		{
			++total_ling_count;
		}
	}

	const NvshenShengwuChouLingCfg *chou_ling_cfg = LOGIC_CONFIG->GetXiannvConfig().GetChouLingCfg(total_ling_count);
	if (NULL == chou_ling_cfg)
	{
		return false;
	}

	total_lingye = chou_ling_cfg->ling_value;

	if (is_double_fetch)
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(LOGIC_CONFIG->GetXiannvConfig().GetOtherCfg()->double_ling_gold, "XiannvManager::ShengwuFetchLing"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return false;
		}

		total_lingye *= 2;
	}

	++m_xiannv_param.day_fetch_ling_time;
	this->ShengwuAddLingye(total_lingye, "ShengwuChouLing");

	// 抽到全部灵，传闻
	if (total_ling_count >= XIANNV_SHENGWU_MILING_FONT_COUNT)
	{
		int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xiannv_shengwu_miling, m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), total_lingye);
		if (len > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
		}
	}
	
	return true;
}

void XiannvManager::ShengwuSendChangeInfo(short notify_type, short param1, short param2, short param3, int param4) const
{
	static Protocol::SCXiannvShengwuChangeInfo xnswci;
	xnswci.notify_type = notify_type;
	xnswci.param1 = param1;
	xnswci.param2 = param2;
	xnswci.param3 = param3;
	xnswci.param4 = param4;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&xnswci, sizeof(xnswci));
}

void XiannvManager::SendMilingList() const
{
	static Protocol::SCXiannvShengwuMilingList swml;
	memcpy(swml.miling_list, m_xiannv_param.miling_list, sizeof(swml.miling_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&swml, sizeof(swml));
}

void XiannvManager::SendShengwuChouExpList(char chou_type) const
{
	static Protocol::SCXiannvShengwuChouExpList s_msg;
	s_msg.chou_type = chou_type;
	s_msg.shengwu_chou_id = m_xiannv_param.shengwu_chou_id;
	memcpy(s_msg.shengwu_chou_exp_list, m_xiannv_param.shengwu_chou_exp_list, sizeof(s_msg.shengwu_chou_exp_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&s_msg, sizeof(s_msg));
}

void XiannvManager::RecalcSkillList()
{
	memset(m_shengwu_skill_list, 0, sizeof(m_shengwu_skill_list));

	for (int shengwu_id = 0; shengwu_id <= XIANNV_SHENGWU_MAX_ID; ++shengwu_id)
	{
		const NvshenShengwuInfoCfg::ShengwuItem *shengwu_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(shengwu_id, m_xiannv_param.all_shengwu_list[shengwu_id].level);
		if (NULL == shengwu_cfg) continue;

		const NvshenShengwuSkillCfg::SkillItem *skill_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuSkillCfg(shengwu_cfg->skill_id, shengwu_cfg->skill_level);
		if (NULL == skill_cfg) continue;

		ShengwuSkill &skill_item = m_shengwu_skill_list[shengwu_cfg->skill_id];
		skill_item.trigger_rate = skill_cfg->trigger_rate;
		skill_item.cool_down_ms = skill_cfg->cool_down_ms;
		skill_item.param_1 = skill_cfg->param_1;
		skill_item.param_2 = skill_cfg->param_2;
	}

	// 格子对技能加成效果
	for (int shengwu_grid_id = 0; shengwu_grid_id <= XIANNV_SHENGWU_GONGMING_MAX_GRID_ID; ++shengwu_grid_id)
	{
		const NvshenShengwuGongmingCfg::ItemCfg *grid_cfg = LOGIC_CONFIG->GetXiannvConfig().GetGongmingGridCfg(shengwu_grid_id, m_xiannv_param.grid_level_list[shengwu_grid_id]);
		if (NULL != grid_cfg && grid_cfg->skill_id > 0 && grid_cfg->skill_id < XIANNV_SHENGWU_SKILL_TYPE_COUNT)
		{
			ShengwuSkill &skill_item = m_shengwu_skill_list[grid_cfg->skill_id];
			skill_item.trigger_rate += grid_cfg->trigger_rate;
			skill_item.cool_down_ms += grid_cfg->cool_down_ms;
			skill_item.param_1 += grid_cfg->param_1;
			skill_item.param_2 += grid_cfg->param_2;
		}
	}
}

void XiannvManager::RecalcSkillCapability(int & capability)
{
	int xiannv_capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_XIANNV);
	// 先计算每个技能的万分比加成,再算固定加成
	for (int i = XIANNV_SHENGWU_SKILL_TYPE_1; i < XIANNV_SHENGWU_SKILL_TYPE_COUNT; ++i)
	{
		capability += static_cast<int>(xiannv_capability * (ROLE_ATTR_PER * m_shengwu_skill_list[i].capability_per));
		capability += m_shengwu_skill_list[i].capability;
	}
}

void XiannvManager::OnCompleteTask(TaskID task_id)
{
	this->CheckFuncOpen();
}

int XiannvManager::GetGongMingTotalLevel()
{
	int total_level = 0;
	for (short level : m_xiannv_param.grid_level_list)
	{
		total_level += level;
	}

	return total_level;
}

int XiannvManager::GetShengwuTotalLevel()
{
	int total_level = 0;
	for (const auto &shengwu : m_xiannv_param.all_shengwu_list)
	{
		total_level += shengwu.level;
	}

	return total_level;
}

int XiannvManager::GetActiveXiannvZizhiNum(int zizhi_level)
{
	int active_num = 0;

	for (int xiannv_id = 0; xiannv_id < MAX_XIANNV_COUNT; xiannv_id ++)
	{
		int xn_zizhi = m_xiannv_param.xn_item_list[xiannv_id].xn_zizhi;

		if (xn_zizhi >= zizhi_level)
		{
			active_num++;
		}
	}

	return active_num;
}

bool XiannvManager::AddShengwuEssence(int add_number, const char* type)
{
	if (add_number <= 0 || add_number + m_xiannv_param.shengwu_essence < m_xiannv_param.shengwu_essence || nullptr == type)
	{
		return false;
	}
	m_xiannv_param.shengwu_essence += add_number;
	this->ShengwuSendChangeInfo(Protocol::SCXiannvShengwuChangeInfo::ESSENCE, 0, 0, add_number, m_xiannv_param.shengwu_essence);
	EventHandler::Instance().OnAddShengwuEssence(m_role, add_number);
	gamelog::g_log_xiannv.printf(LL_INFO, "[XiannvManager::AddShengwuEssence Succ] [user[%s %d] [level:%d] type:%s use_essence:%u remain_essence:%u]",
		m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)add_number, (unsigned int)m_xiannv_param.shengwu_essence);
	return true;
}

bool XiannvManager::UseShengwuEssence(int consume_number, const char *type, bool is_consume_sys)
{
	if (consume_number <= 0 || nullptr == type)
	{
		return false;
	}
	if (!this->IsShengwuEssenceEnough(consume_number))
	{
		return false;
	}
	m_xiannv_param.shengwu_essence -= consume_number;
	this->ShengwuSendChangeInfo(Protocol::SCXiannvShengwuChangeInfo::ESSENCE, 0, 0, 0, m_xiannv_param.shengwu_essence);

	if (is_consume_sys)
	{
		EventHandler::Instance().OnDecShengwuEssence(m_role, consume_number);
	}
	gamelog::g_log_xiannv.printf(LL_INFO, "[XiannvManager::UseShengwuEssence Succ] [user[%s %d] [level:%d] type:%s use_essence:%u remain_essence:%u]",
		m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)consume_number, (unsigned int)m_xiannv_param.shengwu_essence);
	return true;
}

bool XiannvManager::AddShengwuExp(int shengwu_id)
{
	if (shengwu_id < 0 || shengwu_id > XIANNV_SHENGWU_MAX_ID)
	{
		return false;
	}
	NvshenShengwuItem &shengwu_item = m_xiannv_param.all_shengwu_list[shengwu_id];
	const NvshenShengwuInfoCfg::ShengwuItem *shengwu_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(shengwu_id, shengwu_item.level);
	if (NULL == shengwu_level_cfg)
	{
		return false;
	}
	const NvshenShengwuInfoCfg::ShengwuItem *shengwu_next_level_cfg = LOGIC_CONFIG->GetXiannvConfig().GetShengwuCfg(shengwu_id, shengwu_item.level + 1);
	if (NULL == shengwu_next_level_cfg)
	{
		return false;
	}
	int consume_number = shengwu_level_cfg->upgrade_need_shengwu_essence - shengwu_item.exp;
	if (m_xiannv_param.shengwu_essence < consume_number)
	{
		consume_number = m_xiannv_param.shengwu_essence;
	}
	this->UseShengwuEssence(consume_number, " XiannvManager::AddShengwuExp");
	m_xiannv_param.upgrade_exp_list[shengwu_id] += consume_number;
	return true;
}

bool XiannvManager::IsShengwuEssenceEnough(int number)
{
	if (number < 0 || m_xiannv_param.shengwu_essence < number)
	{
		return false;
	}
	return true;
}

void XiannvManager::CheckFuncOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (m_role->GetRoleBigSmallGoal()->IsFuncActive(ROLE_BIG_SMALL_GOAL_SYSTEM_XIANNV))
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_XIANNV);
	if (fun_open_cfg != nullptr && opengame_day >= fun_open_cfg->opengame_day)
	{
		if (m_role->GetLevel() >= fun_open_cfg->role_level || m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
		{
			m_role->GetRoleBigSmallGoal()->OnOpenFunc(ROLE_BIG_SMALL_GOAL_SYSTEM_XIANNV);
		}
	}
}

// 幻化特殊战力加成
void XiannvManager::HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
{
	if (!LOGIC_CONFIG->GetXiannvConfig().IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = LOGIC_CONFIG->GetXiannvConfig().GetHuanHuaSpecialCapAddCfg(huanhua_id);
	if (nullptr == cfg_item)
	{
		return;
	}

	if (huanhua_level >= cfg_item->huanhua_level)
	{
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp;
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji;
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu;
	}
}

int XiannvManager::GetSpecialImageLevelById(int special_image_id)
{
	if (special_image_id < 0 || special_image_id >= MAX_XIANNV_HUANHUA_COUNT)
	{
		return 0;
	}
	if (!m_xiannv_param.IsHuanHuaActive(special_image_id))
	{
		return 0;
	}
	return m_xiannv_param.xiannv_huanhua_level[special_image_id];
}
