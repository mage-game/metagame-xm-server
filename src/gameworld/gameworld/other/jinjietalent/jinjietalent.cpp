#include "other/jinjietalent/jinjietalent.hpp"
#include "other/jinjietalent/jinjietalentconfig.hpp"

#include "obj/character/roleshadow.hpp"
#include "obj/character/role.h"
#include "obj/character/roleclone.hpp"
#include "servercommon/errornum.h"

#include "config/logicconfigmanager.hpp"
#include "servercommon/configcommon.h"
#include "other/capability/capability.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/footprint/footprint.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/mount/mountopconfig.hpp"
#include "other/wing/wingconfig.hpp"
#include "other/halo/haloconfig.hpp"
#include "other/footprint/footprintconfig.hpp"
#include "other/shengong/shengongconfig.hpp"
#include "other/fightmount/fightmountconfig.hpp"
#include "other/shenyi/shenyiconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "skill/skillbase.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/shizhuang/shizhuangconfig.h"

#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectbianshen.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "other/specialappearance/specialappearance.hpp"

#include "gameworld/item/itemextern.hpp"
#include "scene/scene.h"
#include "protocal/msgfight.h"

#include "item/knapsack.h"

#include "protocal/msgjinjietalent.hpp"

#include "engineadapter.h"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "scene/map.h"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "gamecommon.h"
#include "other/fabao/fabaomanager.hpp"
#include "effect/skilleffect/effectinterval.hpp"

JinJieTalent::JinJieTalent()
	: m_role(NULL)
	, m_capability(0)
{
}

JinJieTalent::~JinJieTalent()
{
}

void JinJieTalent::InitParam(Role * role, const JinJieTalentParam & talent_param)
{
	m_role = role;
	m_param = talent_param;
	for (int i = 0; i < JINJIE_TALENT_TYPE_MAX; i++)
	{
		int grade = GetTalentGrade(i);
		OnCheckOpenTalentGrid(i, grade);
		//初始化终极技能
		const SCJinJieTalentGrid& grid_skill = m_param.skill_info_list[i].grid_array[JINJIE_TALENT_SKILL_GRID_MAX_NUM - 1];
		if (grid_skill.skill_id > 0)
		{
			m_skill_list[grid_skill.skill_id] = SkillParam(0, grid_skill.skill_star);
		}
	}
}

void JinJieTalent::GetInitParam(JinJieTalentParam * talent_param)
{
	*talent_param = m_param;
}

void JinJieTalent::OndayChange(int olddayid, int nowdayid)
{
	if (nowdayid != olddayid)
	{
		m_param.free_chou_count = 0;
		this->SendTalentChoujiangPage();
	}
}

void JinJieTalent::OnLogin()
{
	SendAttentionSkillList();
}

void JinJieTalent::ReCalcAttr(CharIntAttrs & base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_capability = 0;
		m_attrs_add.Reset();
		TelentAttrConfig telent_attr_array[JINJIE_TALENT_TYPE_MAX];
		TelentExtendAttrMp extend_attr;
		for (int tl_type = 0; tl_type < JINJIE_TALENT_TYPE_MAX; ++tl_type)
		{
			std::vector<TelentExtendAttr> extend_attr_vec;
			for (int grid_id = 0; grid_id < JINJIE_TALENT_SKILL_GRID_MAX_NUM; grid_id++)
			{
				const SCJinJieTalentGrid& grid_skill = m_param.skill_info_list[tl_type].grid_array[grid_id];
				if (0 == grid_skill.skill_id)
				{
					continue;
				}
				const JinJieTalenSkillParamCfg* skill_cfg = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(grid_skill.skill_id, grid_skill.skill_star);
				if (NULL == skill_cfg)
				{
					continue;
				}
				m_capability += skill_cfg->capability;
				switch (skill_cfg->skill_type)
				{
				case JINJIE_TALENT_SKILL_TYPE_0:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += skill_cfg->value;
					telent_attr_array[tl_type].maxhp += skill_cfg->value;
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_1:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += skill_cfg->value;
					telent_attr_array[tl_type].gongji += skill_cfg->value;
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_2:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += skill_cfg->value;
					telent_attr_array[tl_type].fangyu += skill_cfg->value;
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_3:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += skill_cfg->value;
					telent_attr_array[tl_type].mingzhong += skill_cfg->value;
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_4:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += skill_cfg->value;
					telent_attr_array[tl_type].shanbi += skill_cfg->value;
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_5:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += skill_cfg->value;
					telent_attr_array[tl_type].baoji += skill_cfg->value;
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_6:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += skill_cfg->value;
					telent_attr_array[tl_type].jianren += skill_cfg->value;
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_7:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += skill_cfg->value;
					//telent_attr_array[tl_type].constant_zengshang += skill_cfg->value;
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_8:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += skill_cfg->value;
					//telent_attr_array[tl_type].constant_mianshang += skill_cfg->value;
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_9:
				{
					switch (tl_type)
					{
					case JINJIE_TALENT_MOUNT:
					{
						int level = m_role->GetMountManager()->GetMountParam()->star_level;
						const MountUpStarExpCfg * start_config = LOGIC_CONFIG->GetMountOpConfig().GetUpStarExpCfg(level);
						if (start_config)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(start_config->maxhp*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(start_config->gongji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(start_config->fangyu*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(start_config->mingzhong*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(start_config->shanbi*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(start_config->baoji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(start_config->jianren*(skill_cfg->per * SKILL_ATTR_PER));
						}
					}
					break;
					case JINJIE_TALENT_WING:
					{
						int level = m_role->GetWing()->GetWingParam()->star_level;
						const WingUpStarExpCfg * start_config = LOGIC_CONFIG->GetWingConfig().GetUpStarExpCfg(level);
						if (start_config)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(start_config->maxhp*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(start_config->gongji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(start_config->fangyu*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(start_config->mingzhong*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(start_config->shanbi*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(start_config->baoji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(start_config->jianren*(skill_cfg->per * SKILL_ATTR_PER));
						}
					}
					break;
					case JINJIE_TALENT_HALO:
					{
						int level = m_role->GetHalo()->GetHaloParam()->star_level;
						const HaloUpStarExpCfg * start_config = LOGIC_CONFIG->GetHaloConfig().GetUpStarExpCfg(level);
						if (start_config)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(start_config->maxhp*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(start_config->gongji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(start_config->fangyu*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(start_config->mingzhong*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(start_config->shanbi*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(start_config->baoji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(start_config->jianren*(skill_cfg->per * SKILL_ATTR_PER));
						}
					}
					break;
					case JINJIE_TALENT_FIGHTMOUNT:
					{
						int level = m_role->GetFightMountManager()->GetFightMountParam()->star_level;
						const FightMountUpStarExpCfg * start_config = LOGIC_CONFIG->GetFightMountOpConfig().GetUpStarExpCfg(level);
						if (start_config)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(start_config->maxhp*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(start_config->gongji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(start_config->fangyu*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(start_config->mingzhong*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(start_config->shanbi*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(start_config->baoji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(start_config->jianren*(skill_cfg->per * SKILL_ATTR_PER));
						}
					}
					break;
					/*case JINJIE_TALENT_SHENGGONG:
					{
						int level = m_role->GetShengong()->GetShengongParam()->star_level;
						const ShengongUpStarExpCfg * start_config = LOGIC_CONFIG->GetShengongConfig().GetUpStarExpCfg(level);
						if (start_config)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(start_config->maxhp*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(start_config->gongji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(start_config->fangyu*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(start_config->mingzhong*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(start_config->shanbi*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(start_config->baoji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(start_config->jianren*(skill_cfg->per * SKILL_ATTR_PER));
						}
					}
					break;
					case JINJIE_TALENT_SHENYI:
					{
						int level = m_role->GetShenyi()->GetShenyiParam()->star_level;
						const ShenyiUpStarExpCfg * start_config = LOGIC_CONFIG->GetShenyiConfig().GetUpStarExpCfg(level);
						if (start_config)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(start_config->maxhp*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(start_config->gongji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(start_config->fangyu*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(start_config->mingzhong*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(start_config->shanbi*(skill_cfg->per* SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(start_config->baoji*(skill_cfg->per* SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(start_config->jianren*(skill_cfg->per * SKILL_ATTR_PER));
						}
					}
					break;*/
					case JINJIE_TALENT_SHIZHUANG:
					{
						int level = m_role->GetShizhuang()->GetShiZhuangParam()->item_list[SHIZHUANG_TYPE_BODY].grade;
						auto start_config = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangGradeConfig(level);
						if (start_config)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(start_config->maxhp*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(start_config->gongji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(start_config->fangyu*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(start_config->mingzhong*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(start_config->shanbi*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(start_config->baoji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(start_config->jianren*(skill_cfg->per * SKILL_ATTR_PER));
						}
					}
					break;
					case JINJIE_TALENT_SHENBING:
					{
						int level = m_role->GetShizhuang()->GetShiZhuangParam()->item_list[SHIZHUANG_TYPE_WUQI].grade;
						auto start_config = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponGradeConfig(level);
						if (start_config)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(start_config->maxhp*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(start_config->gongji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(start_config->fangyu*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(start_config->mingzhong*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(start_config->shanbi*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(start_config->baoji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(start_config->jianren*(skill_cfg->per * SKILL_ATTR_PER));
						}
					}
					break;
					case JINJIE_TALENT_FOOTPRINT:
					{
						int level = m_role->GetFootPrint()->GetFootprintParam()->star_level;
						const FootprintUpStarExpCfg * start_config = LOGIC_CONFIG->GetFootPrintConfig().GetUpStarExpCfg(level);
						if (start_config)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(start_config->maxhp*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(start_config->gongji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(start_config->fangyu*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(start_config->mingzhong*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(start_config->shanbi*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(start_config->baoji*(skill_cfg->per * SKILL_ATTR_PER));
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(start_config->jianren*(skill_cfg->per * SKILL_ATTR_PER));
						}
					}
					break;
					default:
						break;
					}
				}
				case JINJIE_TALENT_SKILL_TYPE_10:
				case JINJIE_TALENT_SKILL_TYPE_11:
				case JINJIE_TALENT_SKILL_TYPE_12:
				case JINJIE_TALENT_SKILL_TYPE_13:
				case JINJIE_TALENT_SKILL_TYPE_14:
				case JINJIE_TALENT_SKILL_TYPE_15:
				case JINJIE_TALENT_SKILL_TYPE_16:
				case JINJIE_TALENT_SKILL_TYPE_17:
				case JINJIE_TALENT_SKILL_TYPE_18:
				{
					extend_attr_vec.push_back(TelentExtendAttr(skill_cfg->skill_type, skill_cfg->value, skill_cfg->per));
				}
				break;
				default:
					break;
				}
			}
			extend_attr[tl_type] = extend_attr_vec;
		}

		for (TelentExtendAttrMp::iterator it = extend_attr.begin(); it != extend_attr.end(); it++)
		{
			for (std::vector<TelentExtendAttr>::iterator itt = it->second.begin(); itt != it->second.end(); itt++)
			{
				switch (itt->skill_type)
				{
				case JINJIE_TALENT_SKILL_TYPE_10:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += itt->value + static_cast<Attribute>((telent_attr_array[it->first].maxhp + itt->value) * (itt->per * SKILL_ATTR_PER));
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_11:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += itt->value + static_cast<Attribute>((telent_attr_array[it->first].gongji + itt->value) * (itt->per * SKILL_ATTR_PER));
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_12:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += itt->value + static_cast<Attribute>((telent_attr_array[it->first].fangyu + itt->value) * (itt->per * SKILL_ATTR_PER));
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_13:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += itt->value + static_cast<Attribute>((telent_attr_array[it->first].mingzhong + itt->value) * (itt->per * SKILL_ATTR_PER));
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_14:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += itt->value + static_cast<Attribute>((telent_attr_array[it->first].shanbi + itt->value) * (itt->per * SKILL_ATTR_PER));
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_15:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += itt->value + static_cast<Attribute>((telent_attr_array[it->first].baoji + itt->value) * (itt->per * SKILL_ATTR_PER));
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_16:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += itt->value + static_cast<Attribute>((telent_attr_array[it->first].jianren + itt->value) * (itt->per * SKILL_ATTR_PER));
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_17:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += itt->value + static_cast<Attribute>((telent_attr_array[it->first].constant_zengshang + itt->value) * (itt->per * SKILL_ATTR_PER));
				}
				break;
				case JINJIE_TALENT_SKILL_TYPE_18:
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += itt->value + static_cast<Attribute>((telent_attr_array[it->first].constant_mianshang + itt->value) * (itt->per * SKILL_ATTR_PER));
				}
				break;
				default:
					break;
				}
			}
		}
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_JINJIE_TALENT, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_JINJIE_TALENT, m_capability);
	base_add.Add(m_attrs_add.m_attrs, true);
}

void JinJieTalent::OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill)
{
	Obj* obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == obj || Obj::OBJ_TYPE_ROLE != obj->GetObjType())
	{
		return;
	}
	Role *attacker_role = (Role*)obj;

	unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();
	for (std::map<int, SkillParam>::iterator it = m_skill_list.begin(); it != m_skill_list.end(); it++)
	{
		const JinJieTalenSkillParamCfg* skill_param = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(it->first, it->second.star);
		if (NULL == skill_param)
		{
			continue;
		}
		if (now_second - it->second.last_time <= (unsigned int)skill_param->cd) // CD时间未到
		{
			continue;
		}

		if (JINJIE_TALENT_SKILL_TYPE_19 == skill_param->skill_type)
		{
			if (RandomNum(10000) < skill_param->param1) // N% 的概率触发
			{
				it->second.last_time = now_second;

				EffectDecHurt *effect_dec_hurt = new EffectDecHurt(m_role->GetId(), it->first, skill_param->param3, it->second.star, EffectBase::PRODUCT_METHOD_SKILL, EBT_TALENT_MOUNT_SKILL);
				effect_dec_hurt->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
				effect_dec_hurt->SetDecPercent(skill_param->param2); // 注意，这个参数最终被当做万份比来用
				effect_dec_hurt->SetClientEffectType(EffectBase::ECT_OTHER_CALL_MONSTER_DEC_HURT);
				m_role->AddEffect(effect_dec_hurt);
			}
		}
		else if (JINJIE_TALENT_SKILL_TYPE_21 == skill_param->skill_type)
		{
			Attribute cur_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			if (cur_hp < max_hp * (skill_param->param2 * SKILL_ATTR_PER) && RandomNum(10000) < skill_param->param1) // 血量低于 N %（万份比），有 M %几率触发
			{
				Posi deltaP = attacker_role->GetPos() - m_role->GetPos();
				Scalar distance = (Scalar)skill_param->param3;
				Dir dir = gamecommon::GetMoveDir(deltaP, NULL);
				if (dir == Dir_S)
				{
					continue;
				}

				Posi diff_pos((Axis)(distance * cos(dir)), (Axis)(distance * sin(dir)));
				Posi dest_pos = attacker_role->GetPos() + diff_pos;

				Map *map = m_role->GetScene()->GetMap();
				if (NULL != map)
				{
					if (map->Validate(Obj::OBJ_TYPE_ROLE, dest_pos.x, dest_pos.y))
					{
						it->second.last_time = now_second;
						attacker_role->SkillResetPos(dest_pos, Character::SKILL_RESET_POS_TYPE_CAPTURE, it->first, true);
					}
				}
			}
		}
		else if (JINJIE_TALENT_SKILL_TYPE_24 == skill_param->skill_type)
		{
			//受到攻击时有概率将自身变为敌方形象，反弹受到的伤害的50%，持续xx秒。Cdxx秒。 param2百分比 param3持续时间
			if (RandomNum(10000) < skill_param->param1) // N% 的概率触发
			{
				it->second.last_time = now_second;
				//目标变身
				EffectBianshen *effect_bianshen = new EffectBianshen(m_role->GetId(), it->first, skill_param->param3, 1, EffectBase::PRODUCT_METHOD_SKILL);
				effect_bianshen->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
				effect_bianshen->SetImage(APPEARANCE_ROLE);
				effect_bianshen->SetObjIdDes(attacker);
				effect_bianshen->SetClientEffectType(EffectBase::ECT_SKILL_BIANSHEN);
				m_role->AddEffect(effect_bianshen);

				// 查找BUFF是否添加成功
				EffectBase *add_buff = m_role->GetEffectList()->FindEffect(EffectBase::EFFECT_TYPE_BIANSHEN, EBT_BIANXING_FOOL, m_role->GetId());
				if (nullptr != add_buff)
				{
					// 广播形象
					static Protocol::SCBianShenView cmd;
					cmd.obj_id = m_role->GetId();
					cmd.bianshen_image = APPEARANCE_ROLE;
					cmd.obj_id_des = attacker;
					m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
				}

				EffectReboundHurt *effect_rebound = new EffectReboundHurt(obj->GetId(), it->first, skill_param->param3, skill_param->param2, 1, 0);
				m_role->AddEffect(effect_rebound);
			}
		}
	}
}

void JinJieTalent::OnHurtOther(Character * target, long long injure)
{
	if (target->GetObjType() != Obj::OBJ_TYPE_ROLE)
	{
		return;
	}
	unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();
	for (std::map<int, SkillParam>::iterator it = m_skill_list.begin(); it != m_skill_list.end(); it++)
	{
		const JinJieTalenSkillParamCfg* skill_param = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(it->first, it->second.star);
		if (NULL == skill_param)
		{
			continue;
		}
		if (now_second - it->second.last_time <= (unsigned int)skill_param->cd) // CD时间未到
		{
			continue;
		}
		if (JINJIE_TALENT_SKILL_TYPE_20 == skill_param->skill_type) // 攻击时有xx%概率发出冲击波，对目标玩家造成混乱，类似恐惧效果，乱跑。持续xx秒。Cdxx秒
		{
			if (RandomNum(10000) < skill_param->param1) // N% 的概率触发
			{
				it->second.last_time = now_second;

				EffectSpecialState *effect = new EffectSpecialState(m_role->GetId(), it->first, skill_param->param3, EBT_TALENT_WING_SKILL, 1);
				effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
				target->AddEffect(effect);
			}
		}
		else if (JINJIE_TALENT_SKILL_TYPE_22 == skill_param->skill_type) //攻击时有概率去掉敌方所有特效，形象变为最初始的状态，防御降低xx%。持续xx秒，cdxx秒 param1:概率 param2:百分比 param3:持续时间
		{
			if (RandomNum(10000) < skill_param->param1) // N% 的概率触发
			{
				it->second.last_time = now_second;
				target->ClearAllDeBuffEffect();
				// 减防御
				EffectBuff *effect_buff = new EffectBuff(target->GetId(), it->first, skill_param->param3, 1);
				effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				int value = (int)(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU) * (skill_param->param2 * ROLE_ATTR_PER));
				effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, -value, -skill_param->param2);
				effect_buff->SetClientEffectType(EffectBase::ECT_SKILL_DEC_FANGYU);
				target->AddEffect(effect_buff);

				//目标变身
				EffectBianshen *effect_bianshen = new EffectBianshen(target->GetId(), it->first, skill_param->param3, 1, EffectBase::PRODUCT_METHOD_SKILL);
				effect_bianshen->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
				effect_bianshen->SetImage(APPEARANCE_INITAL_STATUS);
				effect_bianshen->SetClientEffectType(EffectBase::ECT_SKILL_BIANSHEN);
				target->AddEffect(effect_bianshen);

				// 查找BUFF是否添加成功
				EffectBase *add_buff = target->GetEffectList()->FindEffect(EffectBase::EFFECT_TYPE_BIANSHEN, EBT_BIANXING_FOOL, target->GetId());
				if (nullptr != add_buff)
				{
					// 广播形象
					static Protocol::SCBianShenView cmd;
					cmd.obj_id = target->GetId();
					cmd.bianshen_image = APPEARANCE_INITAL_STATUS;
					target->GetScene()->GetZoneMatrix()->NotifyAreaMsg(target->GetObserHandle(), (void*)&cmd, sizeof(cmd));
				}
			}
		}
		else if (JINJIE_TALENT_SKILL_TYPE_23 == skill_param->skill_type) //攻击时有30%概率把敌人打下坐骑和战斗坐骑。Cdx秒 param1:概率
		{
			if (RandomNum(10000) < skill_param->param1) // N% 的概率触发
			{
				it->second.last_time = now_second;
				Role* role = dynamic_cast<Role*>(target);
				role->GetMountManager()->GoonMount(false);
				role->GetFightMountManager()->GoonFightMount(false);
			}
		}
		else if (JINJIE_TALENT_SKILL_TYPE_25 == skill_param->skill_type) //攻击时有概率召唤一个分身，继承自己50%的属性，协同作战。持续xx秒。Cdxx秒。
		{
			if (RandomNum(10000) < skill_param->param1) // N% 的概率触发
			{
				it->second.last_time = now_second;
				RoleClone *role_clone = new RoleClone(ROLE_SHADOW_TYPE_CLONE_ROLE);
				role_clone->Init(m_role, static_cast<float>(skill_param->param2 * ROLE_ATTR_PER), skill_param->param3, target->GetId());
				role_clone->SetPos(m_role->GetPos());
				m_role->GetScene()->AddObj(role_clone);
			}
		}
		else if (JINJIE_TALENT_SKILL_TYPE_25 == skill_param->skill_type)
		{
			if (RandomNum(10000) < skill_param->param1) // N% 的概率触发
			{
				it->second.last_time = now_second;

				EffectInterval *effect_interval = new EffectInterval(m_role->GetId(), skill_param->skill_id, EBT_DROP_BLOOD, 1, EffectBase::PRODUCT_METHOD_SKILL);
				effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
				effect_interval->SetParam(1000, skill_param->param3, static_cast<Attribute>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_param->param2 * 0.01)));
				effect_interval->SetSave(false);
				target->AddEffect(effect_interval);
			}
		}
	}
}

void JinJieTalent::OnCheckOpenTalentGrid(int tl_type, int grade)
{
	SCJinJieTalentSingle& single_talent = m_param.skill_info_list[tl_type];
	for (int i = 0; i < JINJIE_TALENT_SKILL_GRID_MAX_NUM; i++)
	{
		char is_open = single_talent.grid_array[i].is_open;
		if (1 == is_open)
		{
			continue;
		}
		const JinJieActivateTalentGridCondCfg* activate_cfg = LOGIC_CONFIG->GetJinJieTalentConfig().GetActivateGridCondCfg(i);
		if (NULL == activate_cfg || activate_cfg->need_grade > grade)
		{
			continue;
		}
		if (-1 == activate_cfg->pre_index1 && -1 == activate_cfg->pre_index2 && -1 == activate_cfg->pre_index3)
		{
			is_open = 1;
		}
		else if (-1 != activate_cfg->pre_index1 && -1 == activate_cfg->pre_index2 && -1 == activate_cfg->pre_index3)
		{
			if (0 == single_talent.grid_array[activate_cfg->pre_index1].is_open)
			{
				continue;
			}
			if (activate_cfg->per_gird_quality > 0)
			{
				SCJinJieTalentGrid& talent_grid = single_talent.grid_array[activate_cfg->pre_index1];
				if (talent_grid.skill_id == 0)
				{
					continue;
				}
				const JinJieTalenSkillParamCfg* skill_param = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(talent_grid.skill_id, talent_grid.skill_star);
				if (NULL == skill_param)
				{
					continue;
				}
				if (activate_cfg->per_gird_quality <= skill_param->skill_quality)
				{
					is_open = 1;
				}
			}
			else
			{
				is_open = 1;
			}
		}
		else if (-1 != activate_cfg->pre_index1 && -1 != activate_cfg->pre_index2 && -1 == activate_cfg->pre_index3)
		{
			if (0 == single_talent.grid_array[activate_cfg->pre_index1].is_open || 0 == single_talent.grid_array[activate_cfg->pre_index2].is_open)
			{
				continue;
			}
			if (activate_cfg->per_gird_quality > 0)
			{
				SCJinJieTalentGrid& talent_grid1 = single_talent.grid_array[activate_cfg->pre_index1];
				SCJinJieTalentGrid& talent_grid2 = single_talent.grid_array[activate_cfg->pre_index2];
				if (0 == talent_grid1.skill_id || 0 == talent_grid2.skill_id)
				{
					continue;
				}
				const JinJieTalenSkillParamCfg* skill_param1 = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(talent_grid1.skill_id, talent_grid1.skill_star);
				const JinJieTalenSkillParamCfg* skill_param2 = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(talent_grid2.skill_id, talent_grid2.skill_star);
				if (NULL == skill_param1 || NULL == skill_param2)
				{
					continue;
				}
				if (activate_cfg->per_gird_quality <= skill_param1->skill_quality && activate_cfg->per_gird_quality <= skill_param2->skill_quality)
				{
					is_open = 1;
				}
			}
			else
			{
				is_open = 1;
			}
		}
		else
		{
			if (0 == single_talent.grid_array[activate_cfg->pre_index1].is_open || single_talent.grid_array[activate_cfg->pre_index2].is_open || single_talent.grid_array[activate_cfg->pre_index3].is_open)
			{
				if (activate_cfg->per_gird_quality > 0)
				{
					SCJinJieTalentGrid& talent_grid1 = single_talent.grid_array[activate_cfg->pre_index1];
					SCJinJieTalentGrid& talent_grid2 = single_talent.grid_array[activate_cfg->pre_index2];
					SCJinJieTalentGrid& talent_grid3 = single_talent.grid_array[activate_cfg->pre_index3];
					if (0 == talent_grid1.skill_id || 0 == talent_grid2.skill_id || 0 == talent_grid3.skill_id)
					{
						continue;
					}
					const JinJieTalenSkillParamCfg* skill_param1 = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(talent_grid1.skill_id, talent_grid1.skill_star);
					const JinJieTalenSkillParamCfg* skill_param2 = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(talent_grid2.skill_id, talent_grid2.skill_star);
					const JinJieTalenSkillParamCfg* skill_param3 = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(talent_grid3.skill_id, talent_grid3.skill_star);
					if (NULL == skill_param1 || NULL == skill_param2|| NULL == skill_param3)
					{
						continue;
					}
					if (activate_cfg->per_gird_quality <= skill_param1->skill_quality && activate_cfg->per_gird_quality <= skill_param2->skill_quality && activate_cfg->per_gird_quality <= skill_param3->skill_quality)
					{
						is_open = 1;
					}
				}
				else
				{
					is_open = 1;
				}
			}
		}
		if (is_open != single_talent.grid_array[i].is_open)
		{
			single_talent.grid_array[i].is_open = 1;
			UpdateTalentSingleGrid(tl_type, i);
		}
	}
}

void JinJieTalent::OnPutOn(int tl_type, int grid_index, int knapsack_index)
{
	if (tl_type < 0 || tl_type >= JINJIE_TALENT_TYPE_MAX)
	{
		m_role->NoticeNum(errornum::EN_TALENT_NOT_FOUNT_TALENT);
		return;
	}
	if ( grid_index < 0 || grid_index >= JINJIE_TALENT_SKILL_GRID_MAX_NUM)
	{
		m_role->NoticeNum(errornum::EN_TALENT_TALENT_GRID_ERROR);
		return;
	}
	SCJinJieTalentGrid& grid_skill = m_param.skill_info_list[tl_type].grid_array[grid_index];
	if (0 == grid_skill.is_open)
	{
		m_role->NoticeNum(errornum::EN_TALENT_GRID_NOT_ACTIVATE);
		return;
	}
	if (0 < grid_skill.skill_id)
	{
		m_role->NoticeNum(errornum::EN_TALENT_GRID_SKILL_EXISTS);
		return;
	}
	const JinJieTalenSkillParamCfg* skill_param = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(grid_skill.skill_id, grid_skill.skill_star);
	if (skill_param && skill_param->capability > 0)
	{
		m_role->NoticeNum(errornum::EN_TALENT_NEED_LEARN_JUNIOR_SKILL);
		return;
	}

	ItemGridData grid_data;
	if (!m_role->GetKnapsack()->GetItemGridData(knapsack_index, &grid_data) || grid_data.Invalid())
	{
		m_role->NoticeNum(errornum::EN_TALENT_SKILL_ITEM_NOT_ENOUTH);
		return;
	}
	int skill_id = LOGIC_CONFIG->GetJinJieTalentConfig().GetSkillByBookId(grid_data.item_id);
	skill_param = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(skill_id, 0);
	if (NULL == skill_param)
	{
		m_role->NoticeNum(errornum::EN_TALENT_NOT_SKILL_BOOK);
		return;
	}
	if (LOGIC_CONFIG->GetJinJieTalentConfig().IsFinalSkill(skill_param->skill_type))
	{
		std::pair<int, int> final_skill = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalentFinalSkill(tl_type);
		if (skill_param->skill_type != final_skill.first)
		{
			m_role->NoticeNum(errornum::EN_TALENT_FINAL_SKILL_TYPE_ERROR);
			return;
		}
		if (grid_index != final_skill.second)
		{
			m_role->NoticeNum(errornum::EN_TALENT_FINAL_SKILL_GRID_ERROR);
			return;
		}
	}
	for (int i = 0; i < JINJIE_TALENT_SKILL_GRID_MAX_NUM; i++)
	{
		SCJinJieTalentGrid& grid_skill_t = m_param.skill_info_list[tl_type].grid_array[i];
		if (grid_skill_t.skill_id > 0)
		{
			const JinJieTalenSkillParamCfg* skill_param_t = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(grid_skill_t.skill_id, grid_skill_t.skill_star);
			if (skill_param_t && skill_param_t->skill_type == skill_param->skill_type)
			{
				m_role->NoticeNum(errornum::EN_TALENT_GRID_SKILL_TYPE_EXISTS);
				return;
			}
		}
	}
	if (!m_role->GetKnapsack()->ConsumeItemByIndex(knapsack_index, 1, "TalentPutOn"))
	{
		m_role->NoticeNum(errornum::EN_TALENT_SKILL_ITEM_NOT_ENOUTH);
		return;
	}
	grid_skill.skill_id = skill_id;
	grid_skill.skill_star = 0;
	UpdateTalentSingleGrid(tl_type, grid_index);
	//技能品质有更新 需要检测激活
	int grade = GetTalentGrade(tl_type);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINJIE_TALENT);
	OnCheckOpenTalentGrid(tl_type, grade);
	if (JINJIE_TALENT_SKILL_GRID_MAX_NUM - 1 == grid_index)
	{
		m_skill_list[grid_skill.skill_id] = SkillParam(0, grid_skill.skill_star);
	}
	// 日志
	gamelog::g_log_huashen.printf(LL_INFO, "Talent::OnPutOn user[%d, %s], talent_type[%d], grid_index[%d], item_id[%d],item_num[%d], star[%d]",
		m_role->GetUID(), m_role->GetName(), tl_type, grid_index, grid_data.item_id, 1, 0);
}

void JinJieTalent::OnPutOff(int tl_type, int grid_index)
{
	if (tl_type < 0 || tl_type >= JINJIE_TALENT_TYPE_MAX)
	{
		m_role->NoticeNum(errornum::EN_TALENT_NOT_FOUNT_TALENT);
		return;
	}
	if (grid_index < 0 || grid_index >= JINJIE_TALENT_SKILL_GRID_MAX_NUM)
	{
		m_role->NoticeNum(errornum::EN_TALENT_TALENT_GRID_ERROR);
		return;
	}
	SCJinJieTalentGrid& grid_skill = m_param.skill_info_list[tl_type].grid_array[grid_index];
	if (0 == grid_skill.is_open)
	{
		m_role->NoticeNum(errornum::EN_TALENT_GRID_NOT_ACTIVATE);
		return;
	}
	if (0 == grid_skill.skill_id)
	{
		m_role->NoticeNum(errornum::EN_TALENT_GRID_SKILL_NOT_EXISTS);
		return;
	}
	const JinJieTalenSkillParamCfg* skill_param = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(grid_skill.skill_id, 0);	//返回0星时候的材料
	if (!skill_param)
	{
		return;
	}
	Money* money = m_role->GetKnapsack()->GetMoney();
	if (NULL == money)
	{
		return;
	}
	if (!money->GoldMoreThan(skill_param->forget_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}
	// 返还升级材料
	ItemConfigData stuff_item_list[21];
	auto stuff_count = LOGIC_CONFIG->GetJinJieTalentConfig().GetSkillStarAllStuff(grid_skill.skill_id, grid_skill.skill_star, stuff_item_list, static_array_size(stuff_item_list));

	for (int i = 0; i < stuff_count; i++)
	{
		m_role->GetKnapsack()->PutOrMail(stuff_item_list[i], PUT_REASON_TALENT_GIRD_PUTOFF);
		gamelog::g_log_huashen.printf(LL_INFO, "Talent::OnPutOff user[%d, %s], talent_type[%d], grid_index[%d], item_id[%d], item_num[%d], star[%d]",
			m_role->GetUID(), m_role->GetName(), tl_type, grid_index, stuff_item_list[i].item_id, stuff_item_list[i].num, 0);
	}
	
	// 返还升级材料

	money->UseGold(skill_param->forget_gold, "ForgetTalentSkill");
	short tmp_skill_id = grid_skill.skill_id;
	grid_skill.is_open = true;
	grid_skill.skill_id = 0;
	grid_skill.skill_star = 0;
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINJIE_TALENT);
	UpdateTalentSingleGrid(tl_type, grid_index);
	if (JINJIE_TALENT_SKILL_GRID_MAX_NUM - 1 == grid_index)
	{
		std::map<int, SkillParam>::iterator it = m_skill_list.find(tmp_skill_id);
		if (it != m_skill_list.end())
		{
			m_skill_list.erase(it);
		}
	}
	// 日志
//	gamelog::g_log_huashen.printf(LL_INFO, "Talent::OnPutOff user[%d, %s], talent_type[%d], grid_index[%d], item_id[%d], star[%d]",
//		m_role->GetUID(), m_role->GetName(), tl_type, grid_index, item_data.item_id, 0);
}

void JinJieTalent::OnChoujiangFlush(int count)
{
	count = count ? 9 : 1;
	int gold = LOGIC_CONFIG->GetJinJieTalentConfig().GetFlushGold(count);
	if (gold <= 0)
	{
		m_role->NoticeNum(errornum::EN_TALENT_FLUSH_NOT_CONFIG);
		return;
	}

	const JinJieTalentOtherCfg *other_cfg = LOGIC_CONFIG->GetJinJieTalentConfig().GetOtherCfg();
	if (NULL == other_cfg)
	{
		return;
	}

	bool is_free_chou = false;
	if (1 == count && m_param.free_chou_count < other_cfg->free_count)
	{
		is_free_chou = true;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(1))
	{
		m_role->NoticeNum(errornum::EN_TALENT_KNAPSACK_GRID_NO_LESS);
		return;
	}

	Money* money = m_role->GetKnapsack()->GetMoney();
	if (NULL == money)
	{
		return;
	}

	if (!is_free_chou)
	{
		if (!money->GoldMoreThan(gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
		if (!money->UseGold(gold, "FlushTalentChoujiang"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}
	else
	{
		++ m_param.free_chou_count;
	}

	int stage = LOGIC_CONFIG->GetJinJieTalentConfig().GetStage(m_param.flush_count);
	std::vector<int> skill_array = LOGIC_CONFIG->GetJinJieTalentConfig().RandomSkill(stage, count);
	
	for (size_t i = 0; i < skill_array.size() && i < JINJIE_TALENT_CHOUJIANG_GRID_MAX_NUM; i++)
	{
		m_param.choujiang_grid_skill[i] = skill_array.at(i);
	}
	m_param.flush_count += count;
	SendTalentChoujiangPage();
}

void JinJieTalent::OnAwakeSkill(int grid_index)
{
	if (grid_index < 0 || grid_index >= JINJIE_TALENT_SKILL_GRID_MAX_NUM)
	{
		m_role->NoticeNum(errornum::EN_TALENT_TALENT_GRID_ERROR);
		return;
	}
	if (grid_index >= JINJIE_TALENT_CHOUJIANG_GRID_MAX_NUM)
	{
		m_role->NoticeNum(errornum::EN_TALENT_NOT_CHOUJIANG_GRID);
		return;
	}
	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(1))
	{
		m_role->NoticeNum(errornum::EN_TALENT_KNAPSACK_GRID_NO_LESS);
		return;
	}
	if (0 == m_param.choujiang_grid_skill[grid_index])
	{
		m_role->NoticeNum(errornum::EN_TALENT_NOT_CHOUJIANG_SKILL);
		return;
	}
	const JinJieTalenSkillParamCfg* skill_param = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(m_param.choujiang_grid_skill[grid_index], 0);	//返回0星时候的材料
	ItemConfigData item_data;
	item_data.item_id = skill_param->book_id;
	item_data.num = 1;
	item_data.is_bind = true;
	if (!m_role->GetKnapsack()->Put(item_data, PUT_REASON_TALENT_GIRD_PUTOFF))
	{
		return;
	}
	m_param.ResetChoujiang();
	SendTalentChoujiangPage();
	// 日志
	gamelog::g_log_huashen.printf(LL_INFO, "Talent::AwakeSkill user[%d, %s], grid_index[%d], item_id[%d], star[%d]",
		m_role->GetUID(), m_role->GetName(),grid_index, item_data.item_id, 0);
}

void JinJieTalent::OnUpgrageSkillLevel(int tl_type, int grid_index, bool auto_buy)
{
	if (tl_type < 0 || tl_type >= JINJIE_TALENT_TYPE_MAX)
	{
		m_role->NoticeNum(errornum::EN_TALENT_NOT_FOUNT_TALENT);
		return;
	}
	if (grid_index < 0 || grid_index >= JINJIE_TALENT_SKILL_GRID_MAX_NUM)
	{
		m_role->NoticeNum(errornum::EN_TALENT_TALENT_GRID_ERROR);
		return;
	}
	SCJinJieTalentGrid& grid_skill = m_param.skill_info_list[tl_type].grid_array[grid_index];
	if (grid_skill.skill_id == 0)
	{
		m_role->NoticeNum(errornum::EN_TALENT_GRID_SKILL_NOT_EXISTS);
		return;
	}
	const JinJieTalenSkillParamCfg* skill_param = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillInfoCfg(grid_skill.skill_id, grid_skill.skill_star);
	if (skill_param == nullptr) return;

	if (skill_param->skill_quality >= 3 && skill_param->skill_star >= 3)
	{
		m_role->NoticeNum(errornum::EN_TALENT_FINAL_NOT_CAN_UPGRADE);
		return;
	}
	ItemConfigData item_data;
	item_data.item_id = skill_param->need_item_id;
	item_data.num = skill_param->need_item_count;

	int skill_type = skill_param->skill_type;
	int skill_star = skill_param->skill_star;
	int skill_quality = skill_param->skill_quality;

	if (skill_param->skill_star == 3)
	{
		if (skill_param->skill_quality == 3)
		{
			m_role->NoticeNum(errornum::EN_TALENT_FINAL_NOT_CAN_UPGRADE);
			return;
		}
		else
		{
			skill_star = 0;
			skill_quality += 1;
		}
	}
	else
	{
		if (LOGIC_CONFIG->GetJinJieTalentConfig().IsFinalSkill(skill_param->skill_type))	//终极技能没有星 直接升品质
		{
			skill_quality += 1;
		}
		else
		{
			skill_star += 1;
		}
	}
	int skill_id = LOGIC_CONFIG->GetJinJieTalentConfig().GetTalenSkillId(skill_type, skill_quality, skill_star);
	if (0 == skill_id)
	{
		m_role->NoticeNum(errornum::EN_TALENT_FINAL_NOT_CAN_UPGRADE);
		return;
	}
	short stuff_count = 0;
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[stuff_count].item_id = skill_param->need_item_id;
	stuff_list[stuff_count].num = skill_param->need_item_count;
	stuff_list[stuff_count].buyable = auto_buy;
	++stuff_count;

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
	{
		return;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "JinJieTalent::OnUpgrageSkillLevel"))
	{
		return;
	}

	if (JINJIE_TALENT_SKILL_GRID_MAX_NUM - 1 == grid_index)
	{
		std::map<int, SkillParam>::iterator it = m_skill_list.find(grid_skill.skill_id);
		if (it != m_skill_list.end())
		{
			if ((grid_skill.skill_id != skill_id))
			{
				m_skill_list.erase(it);
				m_skill_list[skill_id] = SkillParam(0, skill_star);
			}
			else
			{
				it->second.star = skill_star;
			}
		}
		else
		{
			m_skill_list[skill_id] = SkillParam(0, skill_star);
		}
	}
	grid_skill.skill_id = skill_id;
	grid_skill.skill_star = skill_star;
	UpdateTalentSingleGrid(tl_type, grid_index);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINJIE_TALENT);
	//技能品质有更新 需要检测激活
	int grade = GetTalentGrade(tl_type);
	OnCheckOpenTalentGrid(tl_type, grade);
}

void JinJieTalent::OnTestOpenAllGrid()
{
	for (int i = 0; i < JINJIE_TALENT_TYPE_MAX; i++)
	{
		for (int grid_id = 0; grid_id < JINJIE_TALENT_SKILL_GRID_MAX_NUM; grid_id++)
		{
			m_param.skill_info_list[i].grid_array[grid_id].is_open = 1;
			UpdateTalentSingleGrid(i, grid_id);
		}
	}
}

void JinJieTalent::SendTalentAllInfo()
{
	static Protocol::SCJinJieTalentAllInfo tl_all_info;
	memcpy(tl_all_info.talent_array, m_param.skill_info_list, sizeof(m_param.skill_info_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&tl_all_info, sizeof(tl_all_info));
}

void JinJieTalent::UpdateTalentSingleGrid(int tl_type, int number)
{
	if (tl_type < 0 || tl_type >= JINJIE_TALENT_TYPE_MAX || number < 0 || number >= JINJIE_TALENT_SKILL_GRID_MAX_NUM)
	{
		return;
	}

	static Protocol::SCJinJieTalentUpdateSingleGrid single_grid;
	memcpy(&single_grid.single_info, &m_param.skill_info_list[tl_type].grid_array[number], sizeof(single_grid.single_info));
	single_grid.talent_type = tl_type;
	single_grid.talent_index = number;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&single_grid, sizeof(single_grid));
}

void JinJieTalent::SendTalentChoujiangPage()
{
	static Protocol::SCJinJieTalentChoujiangPage choujiang_page;
	choujiang_page.free_chou_count = m_param.free_chou_count;
	choujiang_page.cur_count = m_param.flush_count;
	memcpy(choujiang_page.choujiang_grid_skill_array, m_param.choujiang_grid_skill, sizeof(m_param.choujiang_grid_skill));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&choujiang_page, sizeof(choujiang_page));
}

int JinJieTalent::GetTalentGrade(int tl_type)
{
	int grade = 0;
	switch (tl_type)
	{
	case JINJIE_TALENT_MOUNT:
	{
		grade = m_role->GetMountManager()->GetMountParam()->grade;
	}
	break;
	case JINJIE_TALENT_WING:
	{
		grade = m_role->GetWing()->GetWingParam()->grade;
	}
	break;
	case JINJIE_TALENT_HALO:
	{
		grade = m_role->GetHalo()->GetHaloParam()->grade;
	}
	break;
	case JINJIE_TALENT_FIGHTMOUNT:
	{
		grade = m_role->GetFightMountManager()->GetFightMountParam()->grade;
	}
	break;
/*	case JINJIE_TALENT_SHENGGONG:
	{
		grade = m_role->GetShengong()->GetShengongParam()->grade;
	}
	break;
	case JINJIE_TALENT_SHENYI:
	{
		grade = m_role->GetShenyi()->GetShenyiParam()->grade;
	}
	break*/;
	case JINJIE_TALENT_SHIZHUANG:
		grade = m_role->GetShizhuang()->GetShiZhuangParam()->item_list[SHIZHUANG_TYPE_BODY].grade;
		break;
	case JINJIE_TALENT_SHENBING:
		grade = m_role->GetShizhuang()->GetShiZhuangParam()->item_list[SHIZHUANG_TYPE_WUQI].grade;
		break;
	case JINJIE_TALENT_FOOTPRINT:
	{
		grade = m_role->GetFootPrint()->GetFootprintParam()->grade;
	}
		break;
	case JINJIE_TALENT_FABAO:
		grade = m_role->GetFabaoManager()->GetParam()->grade;
		break;
	default:
		break;
	}
	return grade;
}

void JinJieTalent::OnSaveAttentionSkill(short skill_id)
{
	if (skill_id <= 0)
	{
		return;
	}

	for (int idx = 0; idx < TALENT_ATTENTION_SKILL_MAX_SAVE_NUM; ++idx)
	{
		if (skill_id == m_param.attention_skill_id[idx])
		{
			m_role->NoticeNum(errornum::EN_TALENT_SKILL_ALREADY_ATTENTION);
			return;
		}
	}

	int empty_idx = -1;
	for (int idx = 0; idx < TALENT_ATTENTION_SKILL_MAX_SAVE_NUM; ++idx)
	{
		if (0 == m_param.attention_skill_id[idx])
		{
			empty_idx = idx;
			break;
		}
	}

	if (!IsValidAttentionSkillIdx(empty_idx))
	{
		m_role->NoticeNum(errornum::EN_TALENT_SKILL_ATTENTION_TOO_MUCH);
		return;
	}
	m_param.attention_skill_id[empty_idx] = skill_id;

	this->SendAttentionSkillList();
	return;
}

void JinJieTalent::OnCancelAttentionSkill(short skill_id)
{
	if (skill_id <= 0)
	{
		return;
	}

	for (int idx = 0; idx < TALENT_ATTENTION_SKILL_MAX_SAVE_NUM; ++idx)
	{
		if (skill_id == m_param.attention_skill_id[idx])
		{
			m_param.attention_skill_id[idx] = 0;

			this->SendAttentionSkillList();
			return;
		}
	}

	m_role->NoticeNum(errornum::EN_TALENT_SKILL_NOT_ATTENTION);
	return;

}

void JinJieTalent::SendAttentionSkillList()
{
	static Protocol::SCTalentAttentionSkillID msg;
	msg.count = 0;
	memset(msg.save_skill_id, 0, sizeof(msg.save_skill_id));
	for (auto skill_id : m_param.attention_skill_id)
	{
		if (skill_id > 0)
		{
			msg.save_skill_id[msg.count++] = skill_id;
		}
	}
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&(msg), sizeof(msg));
}

bool JinJieTalent::IsValidAttentionSkillIdx(int idx)
{
	return idx >= 0 && idx < TALENT_ATTENTION_SKILL_MAX_SAVE_NUM;
}
