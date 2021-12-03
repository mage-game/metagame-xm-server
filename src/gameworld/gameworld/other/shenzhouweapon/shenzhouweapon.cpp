#include "shenzhouweapon.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "item/itemextern.hpp"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "protocal/msgshenzhouweapon.h"
#include "servercommon/noticenum.h"
#include "obj/character/attribute.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "item/itemgriddata.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "other/route/mailroute.hpp"
#include "protocal/msgitem.h"
#include "protocal/msgrole.h"
#include "other/shenzhouweapon/shenzhouweaponconfig.hpp"
#include "item/knapsack.h"
#include "other/shop/roleshop.hpp"
#include "other/capability/capability.hpp"
#include "other/sublock/sublock.hpp"
#include "servercommon/activedegreedef.hpp"
#include "other/activedegree/activedegree.hpp"
#include "other/event/eventhandler.hpp"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "task/taskrecorder.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "effect/skilleffect/effectwudiprotect.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "other/lotterycount/lotterycount.hpp"
#include "other/chestshop/rolechestshop.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

ShenzhouWeapon::ShenzhouWeapon() : m_role(NULL), m_last_gather_time(0), m_capablity(0)
{
}

ShenzhouWeapon::~ShenzhouWeapon()
{

}

void ShenzhouWeapon::InitParam(Role *role, const ShenzhouWeaponParam &param)
{
	m_role = role;
	m_param = param;

	ShenzhouBoxCfg & box_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetBoxCfg();
	m_param.box_id = box_cfg.box_id;
}

void ShenzhouWeapon::GetInitParam(ShenzhouWeaponParam *param)
{
	*param = m_param;

	ShenzhouBoxCfg & box_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetBoxCfg();
	m_param.box_id = box_cfg.box_id;
}

void ShenzhouWeapon::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		this->ReCalcSuitCount();

		m_capablity = 0;

		m_attrs_add.Reset();
		CharIntAttrs identity_attrs_add; identity_attrs_add.Reset();
		CharIntAttrs slot_part_attrs_add; slot_part_attrs_add.Reset();
		CharIntAttrs suit_attrs_add; suit_attrs_add.Reset();
		int xilian_total_star[SHENZHOU_WEAPON_COUNT] = {0};

		// 鉴定等级加成
		const ShenzhouIdentifyLevelCfg::AttributeConfig *identigy_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetIdentifyAttributeCfg(m_param.identify_level, m_param.identify_star_level);
		if (NULL != identigy_cfg)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += identigy_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += identigy_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += identigy_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += identigy_cfg->mingzhong;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += identigy_cfg->shanbi;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += identigy_cfg->baoji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += identigy_cfg->jianren;

			identity_attrs_add.AddAttrs(&m_attrs_add);
		}

		// 每个器件，每个部位等级加成
		for (int hunqi_id = 0; hunqi_id < SHENZHOU_WEAPON_COUNT; hunqi_id++)
		{
			CharIntAttrs temp_attrs_add;
			temp_attrs_add.Reset();

			int extra_add_per = 0;

			// 魂器部件属性
			for(int slot = 0; slot < SHENZHOU_WEAPON_SLOT_COUNT; slot++)
			{
				const ShenzhouSlotAttrCfg::LevelItemConfig *slot_attr_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetSlotLevelAttrCfg(hunqi_id, slot, m_param.weapon_data[hunqi_id].weapon_slot_level[slot]);
				if (NULL == slot_attr_cfg) continue;
				
				temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += slot_attr_cfg->maxhp;
				temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += slot_attr_cfg->gongji;
				temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += slot_attr_cfg->fangyu;
				temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += slot_attr_cfg->baoji;
				temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += slot_attr_cfg->jianren;
				temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += slot_attr_cfg->mingzhong;
				temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += slot_attr_cfg->shanbi;
			}

			{// 人物大目标激活
				int add_value = m_role->GetRoleBigSmallGoal()->GetRoleBigSmallGoalAddAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENZHOU_WEAPON);
				if (add_value > 0)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (add_value * ROLE_ATTR_PER));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (add_value * ROLE_ATTR_PER));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (add_value * ROLE_ATTR_PER));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (add_value * ROLE_ATTR_PER));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (add_value * ROLE_ATTR_PER));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (add_value * ROLE_ATTR_PER));
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (add_value * ROLE_ATTR_PER));
				}
			}

			slot_part_attrs_add.AddAttrs(&temp_attrs_add);

			// 炼魂元素属性
			for(int element = 0; element < SHENZHOU_ELEMET_MAX_TYPE; element++)
			{
				const ShenzhouElementCfg::ShenzhouElementLevelCfg *element_attr_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetElemetLevelAttrCfg(hunqi_id, element, m_param.weapon_data[hunqi_id].element_level[element]);
				if (NULL != element_attr_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += element_attr_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += element_attr_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += element_attr_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += element_attr_cfg->baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += element_attr_cfg->jianren;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += element_attr_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += element_attr_cfg->shanbi;
					extra_add_per += element_attr_cfg->attr_add_per;
				}
			}

			// 炼魂元素万分比加成
			temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (extra_add_per * ROLE_ATTR_PER));
			temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (extra_add_per * ROLE_ATTR_PER));
			temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (extra_add_per * ROLE_ATTR_PER));
			temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (extra_add_per * ROLE_ATTR_PER));
			temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (extra_add_per * ROLE_ATTR_PER));
			temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (extra_add_per * ROLE_ATTR_PER));
			temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (extra_add_per * ROLE_ATTR_PER));

			m_attrs_add.Add(temp_attrs_add.m_attrs);

			// 魂印属性
			for (int hunyin_slot = 0; hunyin_slot < SHENZHOU_HUNYIN_MAX_SLOT; hunyin_slot++)
			{
				temp_attrs_add.Reset();

				// 魂印属性
				const ShenzhouHunYinCfg *hunyin_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetHunYinCfg(m_param.weapon_data[hunqi_id].hunyin_slot[hunyin_slot].hunyin_id);
				if (NULL != hunyin_cfg)
				{
					temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += hunyin_cfg->maxhp;
					temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += hunyin_cfg->gongji;
					temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += hunyin_cfg->fangyu;
					temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += hunyin_cfg->baoji;
					temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += hunyin_cfg->jianren;
					temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += hunyin_cfg->mingzhong;
					temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += hunyin_cfg->shanbi;

					// 魂印套装对当前魂石的加成
					//const ShenzhouHunyinSuitCfg *hunyin_suit = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetHunyinSuitCfg(hunqi_id, m_param.weapon_data[hunqi_id].hunyin_suit_level);
					//if (NULL != hunyin_suit)
					//{
					//	temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(hunyin_cfg->maxhp * hunyin_suit->per_hunshi * ROLE_ATTR_PER);
					//	temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(hunyin_cfg->gongji* hunyin_suit->per_hunshi * ROLE_ATTR_PER);
					//	temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(hunyin_cfg->fangyu* hunyin_suit->per_hunshi * ROLE_ATTR_PER);
					//	temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(hunyin_cfg->baoji* hunyin_suit->per_hunshi * ROLE_ATTR_PER);
					//	temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(hunyin_cfg->jianren* hunyin_suit->per_hunshi * ROLE_ATTR_PER);
					//	temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(hunyin_cfg->mingzhong* hunyin_suit->per_hunshi * ROLE_ATTR_PER);
					//	temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(hunyin_cfg->shanbi* hunyin_suit->per_hunshi * ROLE_ATTR_PER);
					//}

					//（新）魂印套装
					for (int i = 0; i < HUNYIN_COLOR_MAX; i++)
					{
						int hunyin_quality_count = this->GetHunyinSuitQualityCount(hunqi_id, i);
						const ShenzhouHunyinSuitCfg *hunyin_suit = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetSuitCfg(hunqi_id, i, hunyin_quality_count);
						if (hunyin_suit == NULL)
						{
							continue;
						}

						temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(hunyin_cfg->maxhp * (hunyin_suit->per_hunshi * ROLE_ATTR_PER));
						temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(hunyin_cfg->gongji* (hunyin_suit->per_hunshi * ROLE_ATTR_PER));
						temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(hunyin_cfg->fangyu* (hunyin_suit->per_hunshi * ROLE_ATTR_PER));
						temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(hunyin_cfg->baoji* (hunyin_suit->per_hunshi * ROLE_ATTR_PER));
						temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(hunyin_cfg->jianren* (hunyin_suit->per_hunshi * ROLE_ATTR_PER));
						temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(hunyin_cfg->mingzhong* (hunyin_suit->per_hunshi * ROLE_ATTR_PER));
						temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(hunyin_cfg->shanbi* (hunyin_suit->per_hunshi * ROLE_ATTR_PER));
					}
					
					m_attrs_add.Add(temp_attrs_add.m_attrs);
					suit_attrs_add.Add(temp_attrs_add.m_attrs);
				}

				// 灵枢属性
				const ShenzhouLingshuLevelCfg::LingshuLevelCfg *hunyin_slot_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetLingshuLevelAttrCfg(hunqi_id, hunyin_slot, m_param.weapon_data[hunqi_id].hunyin_slot[hunyin_slot].lingshu_level);
				if (NULL != hunyin_slot_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += hunyin_slot_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += hunyin_slot_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += hunyin_slot_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += hunyin_slot_cfg->baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += hunyin_slot_cfg->jianren;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += hunyin_slot_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += hunyin_slot_cfg->shanbi;

					suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += hunyin_slot_cfg->maxhp;
					suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += hunyin_slot_cfg->gongji;
					suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += hunyin_slot_cfg->fangyu;
					suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += hunyin_slot_cfg->baoji;
					suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += hunyin_slot_cfg->jianren;
					suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += hunyin_slot_cfg->mingzhong;
					suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += hunyin_slot_cfg->shanbi;

					if (hunyin_slot_cfg->add_per > 0)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));

						suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));
						suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));
						suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));
						suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));
						suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));
						suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));
						suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(temp_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (hunyin_slot_cfg->add_per * ROLE_ATTR_PER));
					}
				}
			}

			// 魂印套装属性
			//const ShenzhouHunyinSuitCfg *hunyin_suit = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetHunyinSuitCfg(hunqi_id, m_param.weapon_data[hunqi_id].hunyin_suit_level);
			//if (NULL != hunyin_suit)
			//{
			//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += hunyin_suit->maxhp;
			//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += hunyin_suit->gongji;
			//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += hunyin_suit->fangyu;
			//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += hunyin_suit->baoji;
			//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += hunyin_suit->jianren;
			//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += hunyin_suit->mingzhong;
			//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += hunyin_suit->shanbi;

			//	m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(hunyin_suit->per_gongji * ROLE_ATTR_PER);		//套装全身攻击加成
			//	m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(hunyin_suit->per_maxhp * ROLE_ATTR_PER);		//套装全身气血加成
			//}

			//(新)魂印套装属性
			for (int i = 0; i < HUNYIN_COLOR_MAX; i++)
			{
				int hunyin_quality_count = this->GetHunyinSuitQualityCount(hunqi_id, i);
				const ShenzhouHunyinSuitCfg *hunyin_suit = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetSuitCfg(hunqi_id, i, hunyin_quality_count);
				if (hunyin_suit == NULL)
				{
					continue;
				}
				
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += hunyin_suit->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += hunyin_suit->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += hunyin_suit->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += hunyin_suit->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += hunyin_suit->jianren;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += hunyin_suit->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += hunyin_suit->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI_PER] += hunyin_suit->shanbi_per;
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += hunyin_suit->per_mianshang;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER] += hunyin_suit->skill_jianshang_per;
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += hunyin_suit->per_pofang;

				// 下面特殊属性用乘法
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, hunyin_suit->per_mianshang);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, hunyin_suit->per_pofang);

				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(hunyin_suit->per_gongji * ROLE_ATTR_PER);		//套装全身攻击加成
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(hunyin_suit->per_maxhp * ROLE_ATTR_PER);		//套装全身气血加成
			}


			// 魂器洗炼属性
			for (int xilian_slot = 0; xilian_slot < SHENZHOU_HUNYIN_MAX_SLOT; xilian_slot++)
			{
				xilian_total_star[hunqi_id] += m_param.xilian_data[hunqi_id].xilian_shuxing_star[xilian_slot];
				int shuxing_type = m_param.xilian_data[hunqi_id].xilian_shuxing_type[xilian_slot];
				int shuxing_value = m_param.xilian_data[hunqi_id].xilian_shuxing_value[xilian_slot];
				int real_type = TransformXilianAttrType(shuxing_type);
				if (real_type < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP || real_type > CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN)
				{
					continue;
				}

				if (shuxing_type >= XL_SHUXING_TYPE_MAXHP && shuxing_type <= XL_SHUXING_TYPE_JIANREN)
				{
					m_attrs_add.m_attrs[real_type] += shuxing_value;
				}
				else if (shuxing_type >= XL_SHUXING_TYPE_HUNQI_LEVEL_MAXHP_PER && shuxing_type <= XL_SHUXING_TYPE_HUNQI_LEVEL_JIANREN_PER)
				{
					slot_part_attrs_add.m_percent_attrs[real_type] += (float)shuxing_value;  // 这里直接加数值，因为下面的计算，还会乘以 ROLE_ATTR_PER
				}
				else if (shuxing_type >= XL_SHUXING_TYPE_HUNQI_IDENTIFY_MAXHP_PER && shuxing_type <= XL_SHUXING_TYPE_HUNQI_IDENTIFY_JIANREN_PER)
				{
					identity_attrs_add.m_percent_attrs[real_type] += (float)shuxing_value;  // 这里直接加数值，因为下面的计算，还会乘以 ROLE_ATTR_PER
				}
			}

			// 洗炼套装属性
			const SZXilianSuitCfg *xilian_suit_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianSuitCfg(hunqi_id, xilian_total_star[hunqi_id]);
			if (NULL != xilian_suit_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += xilian_suit_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += xilian_suit_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += xilian_suit_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += xilian_suit_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += xilian_suit_cfg->jianren;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += xilian_suit_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += xilian_suit_cfg->shanbi;

				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (xilian_suit_cfg->add_per * ROLE_ATTR_PER));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (xilian_suit_cfg->add_per * ROLE_ATTR_PER));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (xilian_suit_cfg->add_per * ROLE_ATTR_PER));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (xilian_suit_cfg->add_per * ROLE_ATTR_PER));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (xilian_suit_cfg->add_per * ROLE_ATTR_PER));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (xilian_suit_cfg->add_per * ROLE_ATTR_PER));
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(suit_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (xilian_suit_cfg->add_per * ROLE_ATTR_PER));
			}
		}

		// 洗炼属性万分比加成
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(identity_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (identity_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * ROLE_ATTR_PER));
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(identity_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (identity_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * ROLE_ATTR_PER));
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(identity_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (identity_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * ROLE_ATTR_PER));
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(identity_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (identity_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * ROLE_ATTR_PER));
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(identity_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (identity_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * ROLE_ATTR_PER));
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(identity_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (identity_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * ROLE_ATTR_PER));
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(identity_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (identity_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * ROLE_ATTR_PER));

			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(slot_part_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (slot_part_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * ROLE_ATTR_PER));
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(slot_part_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (slot_part_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * ROLE_ATTR_PER));
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(slot_part_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (slot_part_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * ROLE_ATTR_PER));
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(slot_part_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (slot_part_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * ROLE_ATTR_PER));
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(slot_part_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (slot_part_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * ROLE_ATTR_PER));
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(slot_part_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (slot_part_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * ROLE_ATTR_PER));
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(slot_part_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (slot_part_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * ROLE_ATTR_PER));
		}

		// 人物大目标固定属性
		m_role->GetRoleBigSmallGoal()->RoleBigSmallGoalAddSpecialAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENZHOU_WEAPON, m_attrs_add);

		// 技能属性，目前有格挡减伤 会心攻击
		CharIntAttrs skill_attrs_add; skill_attrs_add.Reset();
		this->CheckSkillAttr(skill_attrs_add);
		m_attrs_add.Add(skill_attrs_add.m_attrs);
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SHENZHOU_WEAPON, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_SKILL, m_capablity);
	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

void ShenzhouWeapon::Update(unsigned long interval, time_t now_second)
{
	for (int i = 0; i < SHENZHOU_WEAPON_COUNT; ++i)
	{
		ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[i];
		if (skill_data.skill_effect_time_ms >= 0)
		{
			skill_data.skill_effect_time_ms -= interval;
		}
		
		if (skill_data.skill_effect_time_ms < 0)
		{
			skill_data.skill_effect_time_ms = 0;
			skill_data.skill_stack_count = 0;
		}
	}
	Attribute delta_hp = 0;
	OnAttackSkill07(0, 0, delta_hp, false);	// 生命值低于一定值时恢复最大血量百分比，参数暂时没有用到
}

void ShenzhouWeapon::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid)
	{
		return;
	}

	m_param.today_gather_times = 0;
	m_param.today_buy_gather_times = 0;
	m_param.today_exchange_identify_exp_times = 0;
	m_param.today_open_box_free_num = 0;
	m_param.last_free_open_box_time = 0;

	m_param.normal_item_num = 0;
	m_param.rare_item_num = 0;
	m_param.unique_item_num = 0;
	m_param.today_help_box_num = 0;
	m_param.day_free_xilian_times = 0;

	this->SendBaseInfo();
	this->CheckFuncOpen();
}

//////////////////////////////////////////////////////////////////////////
void ShenzhouWeapon::OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	OnAttackSkill01(attacker, skill_id, delta_hp, from_skill);
	OnAttackSkill03(attacker, skill_id, delta_hp, from_skill);
	//OnAttackSkill06(attacker, skill_id, delta_hp, from_skill);
	OnAttackSkill08(attacker, skill_id, delta_hp, from_skill);
	OnAttackSkill10(attacker, skill_id, delta_hp, from_skill);
}

void ShenzhouWeapon::OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag)
{
	OnHurtOtherSkill02(target, skill_id, base_injure, injure);
	//OnHurtOtherSkill03(target, skill_id, base_injure, injure);
	//OnHurtOtherSkill04(target, skill_id, base_injure, injure);
	//OnHurtOtherSkill05(target, skill_id, base_injure, injure);
	OnHurtOtherSkill06(target, skill_id, base_injure, injure);
	OnHurtOtherSkill09(target, skill_id, base_injure, injure);
	OnHurtOtherSkill11(target, skill_id, base_injure, injure);
}

void ShenzhouWeapon::OnKillOtherRole(Role * target_role)
{
	//OnKillOtherRole07(target_role);
}

long long ShenzhouWeapon::GetShenzhouSkillFixInjure(Character *target, long long base_injure, int skill_id)
{
	long long injure = 0;

// 	for (int hunqi_id = 0; hunqi_id < SHENZHOU_WEAPON_COUNT; hunqi_id++)
// 	{
// 		ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[hunqi_id];
// 		if (weapon_data.weapon_level <= 0) continue;
// 
// 		ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(hunqi_id, weapon_data.weapon_level);
// 		if (NULL == skill_cfg) continue;
// 
// 		ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[hunqi_id];
// 
// 		if (WEAPON_SKILL_ADD_HARM_ON_KILL_ROLE_07 == skill_cfg->skill_type)
// 		{
// 			if (skill_data.skill_effect_time_ms > 0 && skill_data.skill_stack_count > 0)
// 			{
// 				injure += static_cast<long long>(base_injure * skill_cfg->param_2 * skill_data.skill_stack_count * ROLE_ATTR_PER);
// 			}
// 		}
// 	}

	return injure;
}

void ShenzhouWeapon::CheckSkillAttr(CharIntAttrs &skill_attrs_add)
{
	for (int hunqi_id = 0; hunqi_id < SHENZHOU_WEAPON_COUNT; hunqi_id++)
	{
		ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[hunqi_id];
		if (weapon_data.weapon_level <= 0) continue;

		ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(hunqi_id, weapon_data.weapon_level);
		if (NULL == skill_cfg) continue;

		//ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[hunqi_id];

		if (WEAPON_SKILL_CAUSE_MORE_HARM_04 == skill_cfg->skill_type)  // 格挡减伤增加
		{
			skill_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER] += skill_cfg->param_2;
		}
		else if (WEAPON_SKILL_ADD_HARM_ON_DEBUFF_ROLE_05 == skill_cfg->skill_type)  // 会心攻击增加
		{
			skill_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER] += skill_cfg->param_2;
		}

		m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_SKILL_CAP_PER] += skill_cfg->skill_cap_per;  
		m_capablity += skill_cfg->capbility;
	}
}

// 受到攻击有param_2概率抵挡param_3点伤害，冷却时间param_1秒
void ShenzhouWeapon::OnAttackSkill01(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
	{
		ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[i];
		if (weapon_data.weapon_level <= 0) continue;
		ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(i, weapon_data.weapon_level);
		if (NULL == skill_cfg || WEAPON_SKILL_ABSORB_HARM_01 != skill_cfg->skill_type) continue;

		ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[i];

		if (0 != skill_cfg->param_1 && now_sec < skill_data.perform_skill_last_time + skill_cfg->param_1) continue;

		if (delta_hp < 0)
		{
			int rand_num = RandomNum(10000);
			if (rand_num < skill_cfg->param_2)
			{
				// 抵挡伤害
				delta_hp += skill_cfg->param_3;

				skill_data.perform_skill_last_time = now_sec;
				return;
			}
		}
	}
}

// 受到伤害有param_2几率获得无敌效果，持续param_3毫秒, 冷却param_1秒
void ShenzhouWeapon::OnAttackSkill08(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	Obj* obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == obj || Obj::OBJ_TYPE_ROLE != obj->GetObjType())
	{
		return;
	}

	Attribute base_delta_hp = delta_hp;
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
	{
		ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[i];
		if (weapon_data.weapon_level <= 0) continue;

		ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(i, weapon_data.weapon_level);
		if (NULL == skill_cfg || WEAPON_SKILL_DEC_HARM_LESS_THAN_HP_08 != skill_cfg->skill_type) continue;

		ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[i];

		if (0 != skill_cfg->param_1 && now_sec < skill_data.perform_skill_last_time + skill_cfg->param_1) continue;

		if (base_delta_hp < 0)
		{
			int rand_num = RandomNum(10000);
			if (rand_num < skill_cfg->param_2)
			{
				// 无敌
				EffectWudiProtect *wudi_effect = new EffectWudiProtect(m_role->GetId(), 0, skill_cfg->param_3, skill_cfg->level);
				wudi_effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				wudi_effect->SetClientEffectType(EffectBase::ECT_SKILL_WUDI);
				m_role->AddEffect(wudi_effect);

				skill_data.perform_skill_last_time = now_sec;
				return;
			}
		}
	}
}

// 受到攻击时, param_2%的概率触发，将承受的伤害的param_3%反弹给攻击的玩家，冷却param_1秒（PVP有效）
void ShenzhouWeapon::OnAttackSkill10(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	Obj* obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == obj || Obj::OBJ_TYPE_ROLE != obj->GetObjType())
	{
		return;
	}

	Role *attack_role = (Role*)obj;
	Attribute base_delta_hp = delta_hp;
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
	{
		ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[i];
		if (weapon_data.weapon_level <= 0) continue;

		ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(i, weapon_data.weapon_level);
		if (NULL == skill_cfg || WEAPON_SKILL_REBOUND_HARM_10 != skill_cfg->skill_type) continue;

		ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[i];

		if (0 != skill_cfg->param_1 && now_sec < skill_data.perform_skill_last_time + skill_cfg->param_1) continue;

		if (base_delta_hp < 0)
		{
			int rand_num = RandomNum(10000);
			if (rand_num < skill_cfg->param_2 && from_skill)
			{
				Attribute rebound_hp = static_cast<Attribute>(base_delta_hp * (skill_cfg->param_3 * ROLE_ATTR_PER));
				EffectOnce* effect_once = new EffectOnce(m_role->GetId(), 0, rebound_hp, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_REBOUNDHURT);
				attack_role->AddEffect(effect_once);

				skill_data.perform_skill_last_time = now_sec;
				return;
			}
		}
	}
}



// 攻击时有param_2概率造成param_3点额外伤害,冷却param_1
void ShenzhouWeapon::OnHurtOtherSkill02(Character *target, int skill_id, long long base_injure, long long &injure)
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
	{
		ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[i];
		if (weapon_data.weapon_level <= 0) continue;

		ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(i, weapon_data.weapon_level);
		if (NULL == skill_cfg || WEAPON_SKILL_ADD_HARM_ON_MONSTER_02 != skill_cfg->skill_type) continue;

		ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[i];
		if (0 != skill_cfg->param_1 && now_sec < skill_data.perform_skill_last_time + skill_cfg->param_1) continue;

		int rand_num = RandomNum(10000);
		if (rand_num < skill_cfg->param_2)
		{
			injure += skill_cfg->param_3;

			skill_data.perform_skill_last_time = now_sec;
			return;
		}
	}
}

// 受到攻击有param_2概率恢复param_3点生命值，冷却param_1秒
void ShenzhouWeapon::OnAttackSkill03(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
	{
		ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[i];
		if (weapon_data.weapon_level <= 0) continue;

		ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(i, weapon_data.weapon_level);
		if (NULL == skill_cfg || WEAPON_SKILL_ADD_HARM_ON_ROLE_03 != skill_cfg->skill_type) continue;

		ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[i];

		if (0 != skill_cfg->param_1 && now_sec < skill_data.perform_skill_last_time + skill_cfg->param_1) continue;

		int rand_num = RandomNum(10000);
		if (rand_num < skill_cfg->param_2)
		{
			Attribute recover_delta_hp = skill_cfg->param_3;
			m_role->ChangeHp(&recover_delta_hp, true);

			skill_data.perform_skill_last_time = now_sec;
			return;
		}
	}
}

// 攻击的时候，param_2%概率提升param_3%伤害，冷却时间 param_1 秒
void ShenzhouWeapon::OnHurtOtherSkill04(Character *target, int skill_id, long long base_injure, long long &injure)
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
	{
		ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[i];
		if (weapon_data.weapon_level <= 0) continue;

		ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(i, weapon_data.weapon_level);
		if (NULL == skill_cfg || WEAPON_SKILL_CAUSE_MORE_HARM_04 != skill_cfg->skill_type) continue;

		ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[i];

		if (0 != skill_cfg->param_1 && now_sec < skill_data.perform_skill_last_time + skill_cfg->param_1) continue;

		if (base_injure > 0)
		{
			int rand_num = RandomNum(10000);
			if (rand_num < skill_cfg->param_2)
			{
				Attribute change_hp = static_cast<Attribute>(base_injure * ((skill_cfg->param_3 - 10000) * ROLE_ATTR_PER));
				injure += change_hp;

				skill_data.perform_skill_last_time = now_sec;
				return;
			}
		}
	}
}

// 攻击对眩晕、减速的玩家额外造成 param_2% 的伤害
void ShenzhouWeapon::OnHurtOtherSkill05(Character *target, int skill_id, long long base_injure, long long &injure)
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
	{
		ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[i];
		if (weapon_data.weapon_level <= 0) continue;

		ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(i, weapon_data.weapon_level);
		if (NULL == skill_cfg || WEAPON_SKILL_ADD_HARM_ON_DEBUFF_ROLE_05 != skill_cfg->skill_type) continue;

		ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[i];

		if (0 != skill_cfg->param_1 && now_sec < skill_data.perform_skill_last_time + skill_cfg->param_1) continue;

		if (NULL != target && Obj::OBJ_TYPE_ROLE == target->GetObjType())
		{
			if (base_injure > 0)
			{
				if (target->IsXuanYun() || target->IsChiHuan())
				{
					Attribute change_hp = static_cast<Attribute>(base_injure * (skill_cfg->param_2 * ROLE_ATTR_PER));
					injure += change_hp;

					skill_data.perform_skill_last_time = now_sec;
					return;
				}
			}
		}
	}
}

// 攻击时有param_2概率使目标灼烧，每秒造成param_3%攻击的伤害，持续param_4毫秒，冷却时间param_1秒
void ShenzhouWeapon::OnHurtOtherSkill06(Character *target, int skill_id, long long base_injure, long long &injure)
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
	{
		ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[i];
		if (weapon_data.weapon_level <= 0) continue;

		ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(i, weapon_data.weapon_level);
		if (NULL == skill_cfg || WEAPON_SKILL_ZHUOSHAO_06 != skill_cfg->skill_type) continue;

		ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[i];

		if (0 != skill_cfg->param_1 && now_sec < skill_data.perform_skill_last_time + skill_cfg->param_1) continue;

		if (NULL != target && target->IsCharactor(target->GetObjType()))
		{
			if (base_injure > 0)
			{
				int rand_num = RandomNum(10000);
				if (rand_num < skill_cfg->param_2)
				{
					Attribute dec_hp = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (skill_cfg->param_3 * ROLE_ATTR_PER));
					int count = skill_cfg->param_4 / 1000;
					EffectInterval *effect_interval = new EffectInterval(m_role->GetId(), skill_id, EBT_DROP_BLOOD, 1, EffectBase::PUODUCT_METHOD_SHENZHOU_WEAPON_SKILL);
					effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
					effect_interval->SetParam(1000, count <= 0 ? 1 : count, -dec_hp);
					effect_interval->SetSave(true);
					effect_interval->SetClientEffectType(EffectBase::ECT_IMG_SHENZHOU_WEAPON_ZHUOSHAO);
					target->AddEffect(effect_interval);

					skill_data.perform_skill_last_time = now_sec;
					return;
				}
			}
		}
	}
}

// 攻击时，自身血量高于param_2%时，伤害提升param_3%（仅pvp有效）
void ShenzhouWeapon::OnHurtOtherSkill09(Character *target, int skill_id, long long base_injure, long long &injure)
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
	{
		ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[i];
		if (weapon_data.weapon_level <= 0) continue;

		ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(i, weapon_data.weapon_level);
		if (NULL == skill_cfg || WEAPON_SKILL_ADD_HARM_MORE_THAN_HP_09 != skill_cfg->skill_type) continue;

		ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[i];

		if (0 != skill_cfg->param_1 && now_sec < skill_data.perform_skill_last_time + skill_cfg->param_1) continue;

		if (NULL != target && Obj::OBJ_TYPE_ROLE == target->GetObjType())
		{
			if (base_injure > 0)
			{
				Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				Attribute cur_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				Attribute trigger_hp = static_cast<Attribute>(max_hp * (skill_cfg->param_2 * ROLE_ATTR_PER));

				if (cur_hp > trigger_hp)
				{
					Attribute change_hp = static_cast<Attribute>(base_injure * (skill_cfg->param_3 * ROLE_ATTR_PER));
					injure += change_hp;

					skill_data.perform_skill_last_time = now_sec;
					return;
				}
			}
		}
	}
}

// 攻击时,param_2%的概率触发，对自身范围里主目标及附近param_3名玩家造成param_4%的伤害并眩晕，持续param_5秒，冷却param_1秒（仅pvp有效）
void ShenzhouWeapon::OnHurtOtherSkill11(Character *target, int skill_id, long long base_injure, long long &injure)
{
	Scene *scene = m_role->GetScene();
	if (NULL == scene) 
		return;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
	{
		ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[i];
		if (weapon_data.weapon_level <= 0) continue;

		ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(i, weapon_data.weapon_level);
		if (NULL == skill_cfg || WEAPON_SKILL_XUANYUN_ON_RANGE_TARGET_11 != skill_cfg->skill_type) continue;

		ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[i];

		if (0 != skill_cfg->param_1 && now_sec < skill_data.perform_skill_last_time + skill_cfg->param_1) continue;

		if (NULL != target && Obj::OBJ_TYPE_ROLE == target->GetObjType())
		{
			int rand_num = RandomNum(10000);
			if (base_injure > 0 && rand_num < skill_cfg->param_2)
			{
				static const int RANGE = 10;
				static const int MAX_COUNT_OBJ = 32;
				static ObjID targer_list[MAX_COUNT_OBJ] = { INVALID_OBJ_ID };
				int count = scene->GetZoneMatrix()->GetObjByArea(target->GetPos(), RANGE, RANGE, targer_list, sizeof(targer_list) / sizeof(targer_list[0]));

				// 主目标眩晕并保持之前的伤害
				if (target && m_role->IsEnemy(target))
				{
					EffectSpecialState *effect_yun = new EffectSpecialState(m_role->GetId(), 0, skill_cfg->param_5 * 1000,
						EBT_XUANYUN, skill_cfg->level, EffectBase::PUODUCT_METHOD_SHENZHOU_WEAPON_SKILL);
					effect_yun->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
					effect_yun->SetClientEffectType(EffectBase::ECT_SKILL_XUANYUN);
					effect_yun->SetClearOnDie(true);
					target->AddEffect(effect_yun);
				}


				// 其他目标眩晕并伤害百分比
				int taret_num = 0;
				for (int i = 0; i < count && i < MAX_COUNT_OBJ && taret_num < skill_cfg->param_3; i++)
				{
					Obj *tmp_target_obj = scene->GetObj(targer_list[i]);
					if (NULL == tmp_target_obj || Obj::OBJ_TYPE_ROLE != tmp_target_obj->GetObjType()) continue;
					if (!m_role->IsEnemy(tmp_target_obj) || tmp_target_obj == target) continue;

					Character *tmp_target = (Character*)tmp_target_obj;
					Attribute hurt_hp = static_cast<Attribute>(base_injure * skill_cfg->param_4 * ROLE_ATTR_PER);
					EffectOnce *effect = new EffectOnce(m_role->GetId(), 0, -hurt_hp,  EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PUODUCT_METHOD_SHENZHOU_WEAPON_SKILL);
					tmp_target->AddEffect(effect);

					EffectSpecialState *effect_yun = new EffectSpecialState(m_role->GetId(), 0, skill_cfg->param_5 * 1000,
						EBT_XUANYUN, skill_cfg->level, EffectBase::PUODUCT_METHOD_SHENZHOU_WEAPON_SKILL);
					effect_yun->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
					effect_yun->SetClientEffectType(EffectBase::ECT_SKILL_XUANYUN);
					effect_yun->SetClearOnDie(true);
					tmp_target->AddEffect(effect_yun);

					++ taret_num;
				}

				skill_data.perform_skill_last_time = now_sec;
				return;
			}
		}
	}
}


// 生命值万分比低于param_2时恢复自身param_3%最大生命值，冷却param_1秒
void ShenzhouWeapon::OnAttackSkill07(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
	{
		ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[i];
		if (weapon_data.weapon_level <= 0) continue;

		ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(i, weapon_data.weapon_level);
		if (NULL == skill_cfg || WEAPON_SKILL_ADD_HARM_ON_KILL_ROLE_07 != skill_cfg->skill_type) continue;

		ShenzhouWeaponParam::WeaponSkillData &skill_data = m_param.skill_data[i];

		if (0 != skill_cfg->param_1 && now_sec < skill_data.perform_skill_last_time + skill_cfg->param_1) continue;

		Attribute maxHp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		Attribute currentHp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		int hp_per = static_cast<int>(maxHp > 0 ? currentHp * 10000 / maxHp : 0);	//血量万分比
		if (hp_per < skill_cfg->param_2)
		{
			Attribute recover_hp = static_cast<Attribute>(maxHp * (skill_cfg->param_3 * ROLE_ATTR_PER));
			m_role->ChangeHp(&recover_hp, true);
			
			skill_data.perform_skill_last_time = now_sec;
		}
		return;
	}
}

//////////////////////////////////////////////////////////////////////////

void ShenzhouWeapon::GMAddIdentifyExp(int exp)
{
	m_param.identify_exp += exp;
	if (m_param.identify_exp < 0)
	{
		m_param.identify_exp = 0;
	}

	this->SendBaseInfo();
}

void ShenzhouWeapon::GMAddShenzhouExp(int exp)
{
	m_param.hunqi_jinghua += exp;
	m_param.lingshu_exp += exp;
	if (m_param.hunqi_jinghua < 0)
	{
		m_param.hunqi_jinghua = 0;
	}

	if (m_param.lingshu_exp < 0)
	{
		m_param.lingshu_exp = 0;
	}

	this->SendBaseInfo();
}

void ShenzhouWeapon::OpenXilianSlot(int hunqi, int slot)
{
	if (hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT) return;

	if (slot < 0 || slot >= SHENZHOU_HUNYIN_MAX_SLOT) return;

	const SZXilianOpenCfg *open_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianOpenCfg(hunqi, slot);
	if (NULL == open_cfg) return;

	if (slot != 0 && open_cfg->gold_cost <= 0)
	{
		return;
	}

	ShenzhouWeaponParam::XilianData &xilian_data = m_param.xilian_data[hunqi];
	if (0 != (xilian_data.xilian_slot_open_falg & (1 << slot)))
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_XILIAN_OPEN_SLOT_LIMIT);
		return;
	}

	ShenzhouWeaponParam::WeaponData &weapon_data = m_param.weapon_data[hunqi];
	if (weapon_data.weapon_level < open_cfg->lingshu_level_limit)
	{
		return;
	}

	if (open_cfg->gold_cost > 0 && !m_role->GetKnapsack()->GetMoney()->UseAllGold(open_cfg->gold_cost, "SZOpenXilianSlot"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	xilian_data.xilian_slot_open_falg |= (1 << slot);
	this->XilianOneReq(hunqi, slot, XL_COUMME_COLOR_FREE);
}

// void ShenzhouWeapon::XilianReq(int hunqi, int lock_flag, int xilian_comsume_color, bool is_auto_buy, bool is_free)
// {
// 	if (hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT) return;
// 
// 	if (xilian_comsume_color <= XL_COUMME_COLOR_INVALID || xilian_comsume_color >= XL_COUMME_COLOR_MAX) return;
// 
// 	if (m_param.xilian_data[hunqi].xilian_slot_open_falg == 0)
// 	{
// 		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_XILIAN_NOT_OPEN);
// 		return;
// 	}
// 
// 	if (is_free)
// 	{
// 		const ShenzhouOtherCfg &other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();
// 		if (m_param.day_free_xilian_times >= other_cfg.free_xilian_times)
// 		{
// 			m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_XILIAN_FREE_TIMES_LIMIT);
// 			return;
// 		}
// 
// 		xilian_comsume_color = XL_COUMME_COLOR_FREE;
// 	}
// 
// 	// 计算锁定属性条数
// 	int lock_num = 0;
// 	for (int i = 0; i < SHENZHOU_HUNYIN_MAX_SLOT; i++)
// 	{
// 		if (0 != (lock_flag & (1 << i)))
// 		{
// 			++lock_num;
// 		}
// 	}
// 
// 	const SZXilianLockCfg *lock_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianLockCfg(lock_num);
// 	if (NULL == lock_cfg) return;
// 
// 	const int star_count_total_weight = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianConsuemeTotalWeight(xilian_comsume_color);
// 	if (star_count_total_weight <= 0) return;
// 
// 	const SZXilianComsumeCfg *comsume_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianConsumeCfg(xilian_comsume_color);
// 	if (NULL == comsume_cfg) return;
// 
// 	const int shuxing_total_weight = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianShuxingTotalWeight(hunqi);
// 	if (shuxing_total_weight <= 0) return;
// 
// 	std::map<int, std::map<int, SZXilianShuxingCfg> > &shuxing_map = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianShuxingMap();
// 	if (shuxing_map.find(hunqi) == shuxing_map.end()) return;
// 
// 	std::map<int, SZXilianShuxingCfg> &hunqi_shuxing_map = shuxing_map[hunqi];
// 
// 	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
// 	if (!is_free)
// 	{
// 		// 获取可消耗的背包材料列表
// 		short stuff_count = 0;
// 		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
// 		stuff_list[stuff_count].item_id = comsume_cfg->consume_item.item_id;
// 		stuff_list[stuff_count].num = comsume_cfg->consume_item.num;
// 		stuff_list[stuff_count].buyable = is_auto_buy;
// 		++stuff_count;
// 
// 		if (lock_num > 0)
// 		{
// 			//const SZXilianLockCfg *lock_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianLockCfg(xilian_comsume_color, lock_num);
// 			//if (NULL == lock_cfg) return;
// 			//stuff_list[stuff_count].num += lock_cfg->lock_comsume_item.num;
// 
// 			stuff_list[stuff_count].item_id = lock_cfg->lock_comsume_item.item_id;
// 			stuff_list[stuff_count].num = lock_cfg->lock_comsume_item.num;
// 			stuff_list[stuff_count].buyable = is_auto_buy;
// 			++stuff_count;
// 		}
// 
// 		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
// 		{
// 			return;
// 		}
// 	}
// 
// 	int notic_num = 0;
// 	int notic_star_list[SHENZHOU_HUNYIN_MAX_SLOT] = { 0 };
// 	static ShenzhouWeaponParam::XilianData temp_xilian_data;
// 	temp_xilian_data.Reset();
// 	temp_xilian_data.xilian_slot_open_falg = m_param.xilian_data[hunqi].xilian_slot_open_falg;
// 
// 	for (int xilian_times = 0; xilian_times < SHENZHOU_HUNYIN_MAX_SLOT; xilian_times++)
// 	{
// 		if (0 == (temp_xilian_data.xilian_slot_open_falg & (1 << xilian_times)))
// 		{
// 			continue;
// 		}
// 
// 		if (0 != (lock_flag & (1 << xilian_times)))
// 		{
// 			temp_xilian_data.xilian_shuxing_type[xilian_times] = m_param.xilian_data[hunqi].xilian_shuxing_type[xilian_times];
// 			temp_xilian_data.xilian_shuxing_star[xilian_times] = m_param.xilian_data[hunqi].xilian_shuxing_star[xilian_times];
// 			temp_xilian_data.xilian_shuxing_value[xilian_times] = m_param.xilian_data[hunqi].xilian_shuxing_value[xilian_times];
// 		}
// 		else
// 		{
// 			// 随机一种类型属性
// 			std::map<int, SZXilianShuxingCfg>::iterator iter = hunqi_shuxing_map.begin();
// 			int rand_shuxing_weight_value = RandomNum(shuxing_total_weight);
// 			int rand_shuxing_type = XL_SHUXING_TYPE_INVALID;
// 			for (; iter != hunqi_shuxing_map.end(); iter++)
// 			{
// 				if (rand_shuxing_weight_value < iter->second.type_weight)
// 				{
// 					rand_shuxing_type = iter->second.shuxing_type;
// 					break;
// 				}
// 				else
// 				{
// 					rand_shuxing_weight_value -= iter->second.type_weight;
// 				}
// 			}
// 
// 			const SZXilianShuxingCfg *shuxing_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianShuxingCfg(hunqi, rand_shuxing_type);
// 			if (NULL == shuxing_cfg) return;
// 
// 			// 随机属性的星数
// 			int rand_star_value = RandomNum(star_count_total_weight);
// 			int rand_star = 0;
// 			for (int j = 0; j < SHENZHOU_XILIAN_MAX_STAR_COUNT; j++)
// 			{
// 				if (rand_star_value < comsume_cfg->star_weight[j])
// 				{
// 					rand_star = j + 1;
// 					break;
// 				}
// 				else
// 				{
// 					rand_star_value -= comsume_cfg->star_weight[j];
// 				}
// 			}
// 
// 			if (rand_star <= 0 || rand_star > SHENZHOU_XILIAN_MAX_STAR_COUNT) return;
// 
// 			int rand_shuxing_value = RandomNum(shuxing_cfg->star_min[rand_star - 1], shuxing_cfg->star_max[rand_star - 1] + 1);
// 			if (rand_shuxing_value <= 0) return;
// 
// 			temp_xilian_data.xilian_shuxing_type[xilian_times] = rand_shuxing_type;
// 			temp_xilian_data.xilian_shuxing_star[xilian_times] = rand_star;
// 			temp_xilian_data.xilian_shuxing_value[xilian_times] = rand_shuxing_value;
// 
// 			if (rand_star >= 7 && notic_num >= 0 && notic_num < SHENZHOU_HUNYIN_MAX_SLOT)
// 			{
// 				notic_star_list[notic_num++] = rand_star;
// 			}
// 		}
// 	}
// 
// 	if (!is_free)
// 	{
// 		// 消耗
// 		if (consume_list.need_gold_num > 0)
// 		{
// 			long long consume_gold = 0, consume_gold_bind = 0;
// 			if (xilian_comsume_color > XL_COUMME_COLOR_BLUE)
// 			{
// 				if (!m_role->GetKnapsack()->GetMoney()->UseGold(consume_list.need_gold_num, "PromoteXingHun", true))
// 				{
// 					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 					return;
// 				}
// 			}
// 			else
// 			{
// 				if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(consume_list.need_gold_num, "PromoteXingHun", true))
// 				{
// 					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 					return;
// 				}
// 			}
// 
// 			EventHandler::Instance().OnShopBuyItem(m_role, consume_list.buy_count, consume_list.buy_list, consume_gold, consume_gold_bind);
// 		}
// 
// 		if (consume_list.count > 0)
// 		{
// 			if (!m_role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "ShenzhouWeapon::XilianReq"))
// 			{
// 				return;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		++m_param.day_free_xilian_times;
// 	}
// 
// 	memcpy(m_param.xilian_data[hunqi].xilian_shuxing_type, temp_xilian_data.xilian_shuxing_type, sizeof(m_param.xilian_data[hunqi].xilian_shuxing_type));
// 	memcpy(m_param.xilian_data[hunqi].xilian_shuxing_star, temp_xilian_data.xilian_shuxing_star, sizeof(m_param.xilian_data[hunqi].xilian_shuxing_star));
// 	memcpy(m_param.xilian_data[hunqi].xilian_shuxing_value, temp_xilian_data.xilian_shuxing_value, sizeof(m_param.xilian_data[hunqi].xilian_shuxing_value));
// 
// 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENZHOU_WEAPON);
// 	this->SendBaseInfo();
// 
// 	for (int i = 0; i < notic_num && i < SHENZHOU_HUNYIN_MAX_SLOT; i++)
// 	{
// 		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenzhou_xilian, m_role->GetUID(),
// 			m_role->GetName(), (int)m_role->GetCamp(), notic_star_list[i]);
// 		if (length > 0)
// 		{
// 			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
// 		}
// 	}
// }

// 新的洗练规则，根据消耗的颜色获取该消耗颜色的权重配置，产生一条保底星数。其他的星数都根据免费消耗的权重随机
void ShenzhouWeapon::XilianReq(int hunqi, int lock_flag, int xilian_comsume_color, bool is_auto_buy, bool is_free)
{
	if (hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT) return;

	if (xilian_comsume_color <= XL_COUMME_COLOR_INVALID || xilian_comsume_color >= XL_COUMME_COLOR_MAX) return;

	if (m_param.xilian_data[hunqi].xilian_slot_open_falg == 0)
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_XILIAN_NOT_OPEN);
		return;
	}

	if (is_free)
	{
		const ShenzhouOtherCfg &other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();
		if (m_param.day_free_xilian_times >= other_cfg.free_xilian_times)
		{
			m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_XILIAN_FREE_TIMES_LIMIT);
			return;
		}

		xilian_comsume_color = XL_COUMME_COLOR_FREE;
	}

	// 计算锁定属性条数
	int lock_num = 0;
	for (int i = 0; i < SHENZHOU_HUNYIN_MAX_SLOT; i++)
	{
		if (0 != (lock_flag & (1 << i)))
		{
			++lock_num;
		}
	}

	const SZXilianLockCfg *lock_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianLockCfg(lock_num);
	if (NULL == lock_cfg) return;

	const int star_count_total_weight = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianConsuemeTotalWeight(xilian_comsume_color);
	if (star_count_total_weight <= 0) return;

	const int default_star_total_weight = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianConsuemeTotalWeight(XL_COUMME_COLOR_FREE);
	if (default_star_total_weight <= 0) return;

	const SZXilianComsumeCfg *comsume_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianConsumeCfg(xilian_comsume_color);
	if (NULL == comsume_cfg) return;

	const SZXilianComsumeCfg *default_comsume_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianConsumeCfg(XL_COUMME_COLOR_FREE);
	if (NULL == default_comsume_cfg) return;

	const int shuxing_total_weight = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianShuxingTotalWeight(hunqi);
	if (shuxing_total_weight <= 0) return;

	std::map<int, std::map<int, SZXilianShuxingCfg> > &shuxing_map = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianShuxingMap();
	if (shuxing_map.find(hunqi) == shuxing_map.end()) return;

	std::map<int, SZXilianShuxingCfg> &hunqi_shuxing_map = shuxing_map[hunqi];

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	if (!is_free)
	{
		// 获取可消耗的背包材料列表
		short stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = comsume_cfg->consume_item.item_id;
		stuff_list[stuff_count].num = comsume_cfg->consume_item.num;
		stuff_list[stuff_count].buyable = is_auto_buy;
		++stuff_count;

		if (lock_num > 0)
		{
			//const SZXilianLockCfg *lock_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianLockCfg(xilian_comsume_color, lock_num);
			//if (NULL == lock_cfg) return;
			//stuff_list[stuff_count].num += lock_cfg->lock_comsume_item.num;

			stuff_list[stuff_count].item_id = lock_cfg->lock_comsume_item.item_id;
			stuff_list[stuff_count].num = lock_cfg->lock_comsume_item.num;
			stuff_list[stuff_count].buyable = is_auto_buy;
			++stuff_count;
		}

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			return;
		}
	}

	int notic_num = 0;
	int notic_star_list[SHENZHOU_HUNYIN_MAX_SLOT] = { 0 };
	static ShenzhouWeaponParam::XilianData temp_xilian_data;
	temp_xilian_data.Reset();
	temp_xilian_data.xilian_slot_open_falg = m_param.xilian_data[hunqi].xilian_slot_open_falg;

	// 保底星数的slot_idx
	int rand_baodi_slot_idx = -1;

	{
		int xilian_slot_idx_num = 0;
		int xilian_slot_idx_list[SHENZHOU_HUNYIN_MAX_SLOT] = { 0 };

		for (int i = 0; i < SHENZHOU_HUNYIN_MAX_SLOT; ++i)
		{
			if (0 != (temp_xilian_data.xilian_slot_open_falg & (1 << i)))
			{
				if (0 == (lock_flag & (1 << i)))
				{
					xilian_slot_idx_list[xilian_slot_idx_num] = i;
					++xilian_slot_idx_num;
				}
			}
		}
		// 随机一个保底slot_idx
		if (xilian_slot_idx_num > 0)
		{
			int index = RandomNum(xilian_slot_idx_num);
			rand_baodi_slot_idx = xilian_slot_idx_list[index];
		}
	}

	for (int xilian_times = 0; xilian_times < SHENZHOU_HUNYIN_MAX_SLOT; xilian_times++)
	{
		if (0 == (temp_xilian_data.xilian_slot_open_falg & (1 << xilian_times)))
		{
			continue;
		}

		if (0 != (lock_flag & (1 << xilian_times)))
		{
			temp_xilian_data.xilian_shuxing_type[xilian_times] = m_param.xilian_data[hunqi].xilian_shuxing_type[xilian_times];
			temp_xilian_data.xilian_shuxing_star[xilian_times] = m_param.xilian_data[hunqi].xilian_shuxing_star[xilian_times];
			temp_xilian_data.xilian_shuxing_value[xilian_times] = m_param.xilian_data[hunqi].xilian_shuxing_value[xilian_times];
		}
		else
		{
			// 随机一种类型属性
			std::map<int, SZXilianShuxingCfg>::iterator iter = hunqi_shuxing_map.begin();
			int rand_shuxing_weight_value = RandomNum(shuxing_total_weight);
			int rand_shuxing_type = XL_SHUXING_TYPE_INVALID;
			for (; iter != hunqi_shuxing_map.end(); iter++)
			{
				if (rand_shuxing_weight_value < iter->second.type_weight)
				{
					rand_shuxing_type = iter->second.shuxing_type;
					break;
				}
				else
				{
					rand_shuxing_weight_value -= iter->second.type_weight;
				}
			}

			const SZXilianShuxingCfg *shuxing_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianShuxingCfg(hunqi, rand_shuxing_type);
			if (NULL == shuxing_cfg) return;

			// 随机属性的星数
			int rand_star = 0;

			// 保底的用消耗材料颜色的权重配置去随机星数，其他的用默认的
			if (rand_baodi_slot_idx == xilian_times)
			{
				int rand_star_value = RandomNum(star_count_total_weight);

				for (int j = 0; j < SHENZHOU_XILIAN_MAX_STAR_COUNT; j++)
				{
					if (rand_star_value < comsume_cfg->star_weight[j])
					{
						rand_star = j + 1;
						break;
					}
					else
					{
						rand_star_value -= comsume_cfg->star_weight[j];
					}
				}
			}
			else
			{
				int rand_star_value = RandomNum(default_star_total_weight);

				for (int j = 0; j < SHENZHOU_XILIAN_MAX_STAR_COUNT; j++)
				{
					if (rand_star_value < default_comsume_cfg->star_weight[j])
					{
						rand_star = j + 1;
						break;
					}
					else
					{
						rand_star_value -= default_comsume_cfg->star_weight[j];
					}
				}
			}

			if (rand_star <= 0 || rand_star > SHENZHOU_XILIAN_MAX_STAR_COUNT) return;

			int rand_shuxing_value = RandomNum(shuxing_cfg->star_min[rand_star - 1], shuxing_cfg->star_max[rand_star - 1] + 1);
			if (rand_shuxing_value <= 0) return;

			temp_xilian_data.xilian_shuxing_type[xilian_times] = rand_shuxing_type;
			temp_xilian_data.xilian_shuxing_star[xilian_times] = rand_star;
			temp_xilian_data.xilian_shuxing_value[xilian_times] = rand_shuxing_value;

			if (rand_star >= 7 && notic_num >= 0 && notic_num < SHENZHOU_HUNYIN_MAX_SLOT)
			{
				notic_star_list[notic_num++] = rand_star;
			}
		}
	}

	if (!is_free)
	{
		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "ShenzhouWeapon::XilianReq"))
		{
			return;
		}
	}
	else
	{
		++m_param.day_free_xilian_times;
	}

	memcpy(m_param.xilian_data[hunqi].xilian_shuxing_type, temp_xilian_data.xilian_shuxing_type, sizeof(m_param.xilian_data[hunqi].xilian_shuxing_type));
	memcpy(m_param.xilian_data[hunqi].xilian_shuxing_star, temp_xilian_data.xilian_shuxing_star, sizeof(m_param.xilian_data[hunqi].xilian_shuxing_star));
	memcpy(m_param.xilian_data[hunqi].xilian_shuxing_value, temp_xilian_data.xilian_shuxing_value, sizeof(m_param.xilian_data[hunqi].xilian_shuxing_value));

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENZHOU_WEAPON);
	this->SendBaseInfo();

	for (int i = 0; i < notic_num && i < SHENZHOU_HUNYIN_MAX_SLOT; i++)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenzhou_xilian, m_role->GetUID(),
			m_role->GetName(), (int)m_role->GetCamp(), notic_star_list[i]);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}
}

int ShenzhouWeapon::TransformXilianAttrType(int type)
{
	switch (type)
	{
	case XL_SHUXING_TYPE_MAXHP:
	case XL_SHUXING_TYPE_HUNQI_LEVEL_MAXHP_PER:
	case XL_SHUXING_TYPE_HUNQI_IDENTIFY_MAXHP_PER:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP;
	}
	break;

	case XL_SHUXING_TYPE_GONGJI:
	case XL_SHUXING_TYPE_HUNQI_LEVEL_GONGJI_PER:
	case XL_SHUXING_TYPE_HUNQI_IDENTIFY_GONGJI_PER:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI;
	}
	break;

	case XL_SHUXING_TYPE_FANGYU:
	case XL_SHUXING_TYPE_HUNQI_LEVEL_FANGYU_PER:
	case XL_SHUXING_TYPE_HUNQI_IDENTIFY_FANGYU_PER:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU;
	}
	break;

	case XL_SHUXING_TYPE_MINGZHONG:
	case XL_SHUXING_TYPE_HUNQI_LEVEL_MINGZHONG_PER:
	case XL_SHUXING_TYPE_HUNQI_IDENTIFY_MINGZHONG_PER:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG;
	}
	break;

	case XL_SHUXING_TYPE_SHANBI:
	case XL_SHUXING_TYPE_HUNQI_LEVEL_SHANBI_PER:
	case XL_SHUXING_TYPE_HUNQI_IDENTIFY_SHANBI_PER:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI;
	}
	break;

	case XL_SHUXING_TYPE_BAOJI:
	case XL_SHUXING_TYPE_HUNQI_LEVEL_BAOJI_PER:
	case XL_SHUXING_TYPE_HUNQI_IDENTIFY_BAOJI_PER:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI;
	}
	break;

	case XL_SHUXING_TYPE_JIANREN:
	case XL_SHUXING_TYPE_HUNQI_LEVEL_JIANREN_PER:
	case XL_SHUXING_TYPE_HUNQI_IDENTIFY_JIANREN_PER:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN;
	}
	break;
	}

	return CharIntAttrs::CHARINTATTR_TYPE_INVALID;
}

void ShenzhouWeapon::XilianOneReq(int hunqi, int slot, int xilian_comsume_color, bool is_recal)
{
	if (hunqi < 0 || hunqi >= SHENZHOU_WEAPON_COUNT) return;

	if (xilian_comsume_color <= XL_COUMME_COLOR_INVALID || xilian_comsume_color >= XL_COUMME_COLOR_MAX) return;

	if (slot < 0 || slot >= SHENZHOU_HUNYIN_MAX_SLOT) return;

	if (0 == (m_param.xilian_data[hunqi].xilian_slot_open_falg & (1 << slot)))
	{
		return;
	}

	const int star_count_total_weight = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianConsuemeTotalWeight(xilian_comsume_color);
	if (star_count_total_weight <= 0) return;

	const SZXilianComsumeCfg *comsume_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianConsumeCfg(xilian_comsume_color);
	if (NULL == comsume_cfg) return;

	const int shuxing_total_weight = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianShuxingTotalWeight(hunqi);
	if (shuxing_total_weight <= 0) return;

	std::map<int, std::map<int, SZXilianShuxingCfg> > &shuxing_map = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianShuxingMap();
	if (shuxing_map.find(hunqi) == shuxing_map.end()) return;

	std::map<int, SZXilianShuxingCfg> &hunqi_shuxing_map = shuxing_map[hunqi];

	// 随机一种类型属性
	std::map<int, SZXilianShuxingCfg>::iterator iter = hunqi_shuxing_map.begin();
	int rand_shuxing_weight_value = RandomNum(shuxing_total_weight);
	int rand_shuxing_type = XL_SHUXING_TYPE_INVALID;
	for (; iter != hunqi_shuxing_map.end(); iter++)
	{
		if (rand_shuxing_weight_value < iter->second.type_weight)
		{
			rand_shuxing_type = iter->second.shuxing_type;
			break;
		}
		else
		{
			rand_shuxing_weight_value -= iter->second.type_weight;
		}
	}

	const SZXilianShuxingCfg *shuxing_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianShuxingCfg(hunqi, rand_shuxing_type);
	if (NULL == shuxing_cfg) return;

	// 随机属性的星数
	int rand_star_value = RandomNum(star_count_total_weight);
	int rand_star = 0;
	for (int j = 0; j < SHENZHOU_XILIAN_MAX_STAR_COUNT; j++)
	{
		if (rand_star_value < comsume_cfg->star_weight[j])
		{
			rand_star = j + 1;
			break;
		}
		else
		{
			rand_star_value -= comsume_cfg->star_weight[j];
		}
	}

	if (rand_star <= 0 || rand_star > SHENZHOU_XILIAN_MAX_STAR_COUNT) return;

	int rand_shuxing_value = RandomNum(shuxing_cfg->star_min[rand_star - 1], shuxing_cfg->star_max[rand_star - 1] + 1);
	if (rand_shuxing_value <= 0) return;

	m_param.xilian_data[hunqi].xilian_shuxing_type[slot] = rand_shuxing_type;
	m_param.xilian_data[hunqi].xilian_shuxing_star[slot] = rand_star;
	m_param.xilian_data[hunqi].xilian_shuxing_value[slot] = rand_shuxing_value;

	if (is_recal)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENZHOU_WEAPON);
	}

	this->SendBaseInfo();
}

void ShenzhouWeapon::SendBaseInfo()
{
	static Protocol::SCShenzhouWeapondAllInfo all_info;
	all_info.today_gather_times = m_param.today_gather_times;
	all_info.today_buy_gather_times = m_param.today_buy_gather_times;
	all_info.today_exchange_identify_exp_times = m_param.today_exchange_identify_exp_times;
	all_info.identify_level = m_param.identify_level;
	all_info.identify_star_level = m_param.identify_star_level;
	all_info.identify_exp = m_param.identify_exp;
	all_info.hunqi_jinghua = m_param.hunqi_jinghua;
	all_info.lingshu_exp = m_param.lingshu_exp;
	all_info.day_free_xilian_times = m_param.day_free_xilian_times;

	all_info.hunqi_count = SHENZHOU_WEAPON_COUNT;

	for (int i = 0; i < all_info.hunqi_count && i < SHENZHOU_WEAPON_MAX_SAVE_COUNT; ++i)
	{
		all_info.weapon_data[i] = m_param.weapon_data[i];
		all_info.xilian_data[i] = m_param.xilian_data[i];
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&all_info, sizeof(all_info));
}

void ShenzhouWeapon::SendGatherInfo()
{
	static Protocol::SCShenzhouWeapondGatherInfo gather_info;
	gather_info.today_gather_times = m_param.today_gather_times;
	gather_info.today_buy_gather_times = m_param.today_buy_gather_times;
	gather_info.scene_leave_num = ActivityShadow::Instance().GetShenzhouGatherNum();
	gather_info.normal_item_num = m_param.normal_item_num;
	gather_info.rare_item_num = m_param.rare_item_num;
	gather_info.unique_item_num = m_param.unique_item_num;
	gather_info.next_refresh_time = ActivityShadow::Instance().GetShenzhouGatherNextRefreshTime();

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&gather_info, sizeof(gather_info));
}

void ShenzhouWeapon::SlotUpLevel(int hunqi_id, int slot)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT) return;

	if (slot < 0 || slot >= SHENZHOU_WEAPON_SLOT_COUNT) return;

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}
	
	ShenzhouWeaponParam::WeaponData &hunqi = m_param.weapon_data[hunqi_id];

	const ShenzhouSlotAttrCfg::LevelItemConfig *cur_level_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetSlotLevelAttrCfg(hunqi_id, slot, hunqi.weapon_slot_level[slot]);
	if (NULL == cur_level_cfg)
	{
		return;
	}

	const ShenzhouSlotAttrCfg::LevelItemConfig *next_level_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetSlotLevelAttrCfg(hunqi_id, slot, hunqi.weapon_slot_level[slot] + 1);
	if (NULL == next_level_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_SLOT_LEVEL_MAX);
		return;
	}

	if (m_param.hunqi_jinghua < cur_level_cfg->up_level_jinghua)
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_JINGHUA_NOT_ENOUGH);
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	if (cur_level_cfg->up_level_item.item_id > 0)
	{
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = cur_level_cfg->up_level_item.item_id;
		stuff_list[0].num = cur_level_cfg->up_level_item.num;
		stuff_list[0].buyable = false;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
		{
			return;
		}
	}

	// 消耗
	m_param.hunqi_jinghua -= cur_level_cfg->up_level_jinghua;

	if (!ItemExtern::ConsumeItem(m_role, consume_list, "ShenzhouWeapon::SlotUpLevel"))
	{
		return;
	}

	// 给予
	int old_slot_level = hunqi.weapon_slot_level[slot];
	++ hunqi.weapon_slot_level[slot];
	m_role->SendOperateResult(Protocol::SCOperateResult::OP_SHENZHOU_WEAPON, 1);

	this->RefreshWeaponLevel(hunqi_id);
	this->SendBaseInfo();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENZHOU_WEAPON);

	gamelog::g_log_shenzhou_weapon.printf(LL_INFO, "ShenzhouWeapon::SlotUpLevel user %d %s, hunqi %d, slot %d,old_level %d , new_level %d",
		m_role->GetUID(), m_role->GetName(), hunqi_id, slot, old_slot_level, m_param.weapon_data[hunqi_id].weapon_slot_level[slot]);
}

void ShenzhouWeapon::RefreshWeaponLevel(int hunqi_id)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT) return;

	ShenzhouWeaponParam::WeaponData &hunqi = m_param.weapon_data[hunqi_id];

	int old_level = hunqi.weapon_level;
	int slot_min_level = hunqi.weapon_slot_level[0];
	for(int i = 1; i < SHENZHOU_WEAPON_SLOT_COUNT; i++)
	{
		if (hunqi.weapon_slot_level[i] < slot_min_level)
		{
			slot_min_level = hunqi.weapon_slot_level[i];
		}
	}

	if (slot_min_level > hunqi.weapon_level)
	{
		hunqi.weapon_level = slot_min_level;
	}

	if (hunqi.weapon_level > 0)
	{
		if (0 == old_level && 1 == hunqi.weapon_level)
		{
			ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(hunqi_id, 1);
			if (NULL != skill_cfg)
			{
				if (0 != skill_cfg->is_notice)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenzhou_weapon_active,
						UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), hunqi_id, hunqi_id);
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			} 
			m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENZHOU_WEAPON, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_3, hunqi_id);

			m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_ACTIVE_SHENZHOU_WEAPON);
		}
		else if (old_level != hunqi.weapon_level)
		{
			ShenzhouSkillCfg *skill_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetWeaponSkillCfg(hunqi_id, hunqi.weapon_level);
			if (NULL != skill_cfg)
			{
				if (0 != skill_cfg->is_notice)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenzhou_skill_up_level,
						UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), hunqi_id, hunqi.weapon_level, hunqi_id);
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}

	if (0 == (m_param.xilian_data[hunqi_id].xilian_slot_open_falg & (1 << 0)))
	{
		const SZXilianOpenCfg *xilian_open_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianOpenCfg(hunqi_id, 0);
		if (NULL != xilian_open_cfg)
		{
			if (hunqi.weapon_level >= xilian_open_cfg->lingshu_level_limit)
			{
				m_param.xilian_data[hunqi_id].xilian_slot_open_falg |= (1 << 0);
				this->XilianOneReq(hunqi_id, 0, XL_COUMME_COLOR_FREE, false);
			}
		}
	}
}

void ShenzhouWeapon::ElementUpLevel(int hunqi_id, int element_type)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT) return;

	if (element_type < 0 || element_type >= SHENZHOU_ELEMET_MAX_TYPE) return;

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	ShenzhouWeaponParam::WeaponData &hunqi = m_param.weapon_data[hunqi_id];

	const ShenzhouElementCfg::ShenzhouElementLevelCfg *cur_level_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetElemetLevelAttrCfg(hunqi_id, element_type, hunqi.element_level[element_type]);
	if (NULL == cur_level_cfg)
	{
		return;
	}

	const ShenzhouElementCfg::ShenzhouElementLevelCfg *next_level_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetElemetLevelAttrCfg(hunqi_id, element_type, hunqi.element_level[element_type] + 1);
	if (NULL == next_level_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_ELEMENT_LEVEL_MAX);
		return;
	}

	if (hunqi.weapon_level < cur_level_cfg->huqi_level_limit)
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_HUNQI_LEVEL_LIMIT);
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	if (cur_level_cfg->up_level_item.item_id > 0)
	{
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = cur_level_cfg->up_level_item.item_id;
		stuff_list[0].num = cur_level_cfg->up_level_item.num;
		stuff_list[0].buyable = false;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
		{
			return;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "ShenzhouWeapon::ElementUpLevel"))
	{
		return;
	}

	// 给予
	int old_element_level = hunqi.element_level[element_type];
	++ hunqi.element_level[element_type];

	this->SendBaseInfo();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENZHOU_WEAPON);

	if (1 == hunqi.element_level[element_type])
	{
		bool is_notice_active = true;

		for(int i = 0; i < SHENZHOU_ELEMET_MAX_TYPE; i++)
		{
			if (1 != hunqi.element_level[i])
			{
				is_notice_active = false;
				break;
			}
		}

		if (is_notice_active)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenzhou_lianhun_active, UidToInt(m_role->GetUserId()),
				m_role->GetName(), (int)m_role->GetCamp(), hunqi_id);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	if (0 != cur_level_cfg->is_notice)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenzhou_lianhun_level_up, UidToInt(m_role->GetUserId()),
			m_role->GetName(), (int)m_role->GetCamp(), hunqi_id, element_type, hunqi.element_level[element_type]);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	gamelog::g_log_shenzhou_weapon.printf(LL_INFO, "ShenzhouWeapon::ElementUpLevel user %d %s, hunqi %d, element_type %d, old_level %d , new_level %d",
		m_role->GetUID(), m_role->GetName(), hunqi_id, element_type, old_element_level, hunqi.element_level[element_type]);
}

void ShenzhouWeapon::IdentifyBatchItem(int index, int num)
{
	if (index < 0 || index >= ItemNamespace::MAX_KNAPSACK_GRID_NUM || num <= 0 || num > MAX_IDENTIFY_BATCH_ITEM_COUNT)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->CheckEmptyGridNoLessThan(MAX_ATTACHMENT_ITEM_NUM))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	ItemGridData item_data;
	if (!knapsack->GetItemGridData(index, &item_data))
	{
		return;
	}

	if (item_data.num < num)
	{
		return;
	}

	ShenzhouIdentifyItemCfg *identify_item_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetIdentifyItemById(item_data.item_id);
	if (NULL == identify_item_cfg) return;

	int identify_reward_item_total_pro = 0;
	std::vector<ShenzhouIdentifyRewardCfg> temp_identify_reward;
	for(std::map<int, ShenzhouIdentifyRewardCfg>::iterator it = identify_item_cfg->reward_pool_map.begin(); it != identify_item_cfg->reward_pool_map.end(); it++)
	{
		ShenzhouIdentifyRewardCfg &temp_cfg = it->second;
		if (temp_cfg.identify_level <= m_param.identify_level)
		{
			temp_identify_reward.push_back(temp_cfg);
			identify_reward_item_total_pro += temp_cfg.weight;
		}
	}

	int index_count = 0;
	static ItemConfigData item_cfg_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	memset(item_cfg_list, 0, sizeof(item_cfg_list));

	int consume_num = 0; 
	int add_total_exp = 0;
	int add_hunqi_jinghua = 0;
	for (; consume_num < num && consume_num < MAX_IDENTIFY_BATCH_ITEM_COUNT; consume_num++)
	{
		if (!knapsack->CheckEmptyGridNoLessThan(MAX_ATTACHMENT_ITEM_NUM))
		{
			break;
		}

		int rand_num = RandomNum(10000);

		if (rand_num < identify_item_cfg->identify_suc_pro && static_cast<int>(temp_identify_reward.size()) > 0 && identify_reward_item_total_pro > 0)
		{
			int rand_item_pro = RandomNum(identify_reward_item_total_pro);
			for(std::vector<ShenzhouIdentifyRewardCfg>::iterator reward_item_it = temp_identify_reward.begin(); rand_item_pro > 0 && reward_item_it != temp_identify_reward.end(); reward_item_it++)
			{
				if (rand_item_pro < reward_item_it->weight)
				{
					knapsack->PutListOrMail(reward_item_it->reward_item, PUT_REASON_SHENZHOU_WEAPON_IDENTIFY);
					add_total_exp += identify_item_cfg->identify_exp;

					for (int reward_num = 0; reward_num < MAX_ATTACHMENT_ITEM_NUM; reward_num++)
					{
						ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, item_cfg_list, &index_count, 
							reward_item_it->reward_item[reward_num].item_id, reward_item_it->reward_item[reward_num].num, reward_item_it->reward_item[reward_num].is_bind);
					}

					gamelog::g_log_shenzhou_weapon.printf(LL_INFO, "ShenzhouWeapon::IdentityItem user %d %s, identity_item_id %d, get_item_1 %s, get_itme_2 %s, get_item_3 %s",
						m_role->GetUID(), m_role->GetName(), item_data.item_id, ItemConfigDataLog(&reward_item_it->reward_item[0], NULL), 
						ItemConfigDataLog(&reward_item_it->reward_item[1], NULL), ItemConfigDataLog(&reward_item_it->reward_item[2], NULL));
					break;
				}
				else
				{
					rand_item_pro -= reward_item_it->weight;
				}
			}
		}
		else
		{
			add_hunqi_jinghua += identify_item_cfg->fail_jinghua;
			add_total_exp += identify_item_cfg->identify_exp;
		}
	}
	
	if (consume_num > 0)
	{
		// 消耗物品
		knapsack->ConsumeItemByIndex(index, consume_num, "shenzhouidentify");

		if (add_total_exp > 0)
		{
			this->AddIdentifyExp(add_total_exp, false);
		}

		if (add_hunqi_jinghua > 0)
		{
			m_role->GetRoleShop()->AddChestShopMojing(add_hunqi_jinghua, "ShenzhouIdentifyFail");
		}
		
		this->SendBaseInfo();

		gamelog::g_log_shenzhou_weapon.printf(LL_INFO, "ShenzhouWeapon::IdentifyItem user %d %s, add_identify_exp %d, add_hunqi_jinhua %d",
			m_role->GetUID(), m_role->GetName(), add_total_exp, add_hunqi_jinghua);
	}

	if (index_count > 0 || add_hunqi_jinghua > 0)
	{
		ItemExtern::SendRewardListInfo(m_role, item_cfg_list, index_count, Protocol::SCRewardListInfo::NOTICE_TYPE_SHENZHOU_WEAPON, add_hunqi_jinghua);
	}
}

void ShenzhouWeapon::OneKeyIdentifyItem()
{
	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
	}

	// 这堆变量是发送奖励列表用的
	int total_add_jinghua = 0;
	int index_count = 0;
	static ItemConfigData item_cfg_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	memset(item_cfg_list, 0, sizeof(item_cfg_list));

	int cur_valid_gird_num = m_role->GetKnapsack()->GetCurrKnapsackValidNum();
	for(int cur_index = 0; cur_index < cur_valid_gird_num && cur_index < ItemNamespace::MAX_KNAPSACK_GRID_NUM; cur_index++)
	{
		if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(MAX_ATTACHMENT_ITEM_NUM))
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		}

		ItemGridData item_data;
		if (!m_role->GetKnapsack()->GetItemGridData(cur_index, &item_data))
		{
			continue;
		}

		ShenzhouIdentifyItemCfg *identify_item_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetIdentifyItemById(item_data.item_id);
		if (NULL == identify_item_cfg) continue;

		int identify_reward_item_total_pro = 0;
		std::vector<ShenzhouIdentifyRewardCfg> temp_identify_reward_pool;
		for(std::map<int, ShenzhouIdentifyRewardCfg>::iterator it = identify_item_cfg->reward_pool_map.begin(); it != identify_item_cfg->reward_pool_map.end(); it++)
		{
			ShenzhouIdentifyRewardCfg &temp_cfg = it->second;
			if (temp_cfg.identify_level <= m_param.identify_level)
			{
				temp_identify_reward_pool.push_back(temp_cfg);
				identify_reward_item_total_pro += temp_cfg.weight;
			}
		}

		int index_num = item_data.num;
		int consume_num = 0; 
		int add_total_exp = 0;
		int add_hunqi_jinghua = 0;

		for (; consume_num < index_num && consume_num < MAX_IDENTIFY_BATCH_ITEM_COUNT; consume_num++)
		{
			if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(MAX_ATTACHMENT_ITEM_NUM))
			{
				break;
			}

			int rand_num = RandomNum(10000);
			if (rand_num < identify_item_cfg->identify_suc_pro && static_cast<int>(temp_identify_reward_pool.size()) > 0 && identify_reward_item_total_pro > 0)
			{
				int rand_item_pro = RandomNum(identify_reward_item_total_pro);

				for(std::vector<ShenzhouIdentifyRewardCfg>::iterator reward_item_it = temp_identify_reward_pool.begin(); reward_item_it != temp_identify_reward_pool.end(); reward_item_it++)
				{
					if (rand_item_pro < reward_item_it->weight)
					{
						m_role->GetKnapsack()->PutListOrMail(reward_item_it->reward_item, PUT_REASON_SHENZHOU_WEAPON_IDENTIFY);
						add_total_exp += identify_item_cfg->identify_exp;

						for (int reward_num = 0; reward_num < MAX_ATTACHMENT_ITEM_NUM; reward_num++)
						{
							ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, item_cfg_list, &index_count, 
								reward_item_it->reward_item[reward_num].item_id, reward_item_it->reward_item[reward_num].num, reward_item_it->reward_item[reward_num].is_bind);
						}

						gamelog::g_log_shenzhou_weapon.printf(LL_INFO, "ShenzhouWeapon::IdentityItem user %d %s, identity_item_id %d, get_item_1 %s, get_itme_2 %s, get_item_3 %s",
							m_role->GetUID(), m_role->GetName(), item_data.item_id, ItemConfigDataLog(&reward_item_it->reward_item[0], NULL), 
							ItemConfigDataLog(&reward_item_it->reward_item[1], NULL), ItemConfigDataLog(&reward_item_it->reward_item[2], NULL));
						break;
					}
					else
					{
						rand_item_pro -= reward_item_it->weight;
					}
				}
			}
			else
			{
				add_hunqi_jinghua += identify_item_cfg->fail_jinghua;
				total_add_jinghua += identify_item_cfg->fail_jinghua;
				add_total_exp += identify_item_cfg->identify_exp;
			}
		}

		if (consume_num > 0)
		{
			// 消耗物品
			m_role->GetKnapsack()->ConsumeItemByIndex(cur_index, consume_num, "shenzhouidentify");

			if (add_total_exp > 0)
			{
				this->AddIdentifyExp(add_total_exp, false);
			}

			if (add_hunqi_jinghua > 0)
			{
				m_role->GetRoleShop()->AddChestShopMojing(add_hunqi_jinghua, "ShenzhouIdentifyFail");
			}

			gamelog::g_log_shenzhou_weapon.printf(LL_INFO, "ShenzhouWeapon::IdentifyItem user %d %s, add_identify_exp %d, add_hunqi_jinhua %d",
				m_role->GetUID(), m_role->GetName(), add_total_exp, add_hunqi_jinghua);
		}
	}

	if (index_count > 0 || total_add_jinghua > 0)
	{
		this->SendBaseInfo();

		ItemExtern::SendRewardListInfo(m_role, item_cfg_list, index_count, Protocol::SCRewardListInfo::NOTICE_TYPE_SHENZHOU_WEAPON, total_add_jinghua);
	}
}

void ShenzhouWeapon::ExchangeIdentifyExp()
{
	if (m_param.identify_level == LOGIC_CONFIG->GetShenzhouWeaponCfg().GetMaxIdentifyLevel() && m_param.identify_star_level == SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT)
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_IDENTIFY_LEVEL_ALEADY_MAX);
		return;
	}

	const ShenzhouExchangeIdentifyExpCfg *exp_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetExchangeIdentifyExpCfg(m_param.today_exchange_identify_exp_times);
	if (NULL == exp_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_EXCHANGE_TIME_MAX);
		return;
	}

	if (exp_cfg->consume_gold <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(exp_cfg->consume_gold, "ShenzhouWeapon::ExchangeIdentifyExp"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}
	
	int old_exchange_times = m_param.today_exchange_identify_exp_times;
	++ m_param.today_exchange_identify_exp_times;

	// 给予
	this->AddIdentifyExp(exp_cfg->reward_exp);

	gamelog::g_log_shenzhou_weapon.printf(LL_INFO, "ShenzhouWeapon::ExchangeIdentifyExp user %d %s, old_exchange_times %d, consume_gold %d reward_exp %d, current_identify_exp %d",
		m_role->GetUID(), m_role->GetName(), old_exchange_times, exp_cfg->consume_gold, exp_cfg->reward_exp, m_param.identify_exp);
}

void ShenzhouWeapon::AddIdentifyExp(int add_identify_exp, bool is_notice_client)
{
	if (add_identify_exp <= 0) return;

	if (m_param.identify_level >= LOGIC_CONFIG->GetShenzhouWeaponCfg().GetMaxIdentifyLevel() && m_param.identify_star_level >= SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT)
	{
		if (m_param.identify_level > LOGIC_CONFIG->GetShenzhouWeaponCfg().GetMaxIdentifyLevel())
		{
			m_param.identify_level = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetMaxIdentifyLevel();
		}

		if (m_param.identify_star_level > SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT)
		{
			m_param.identify_star_level = SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT;
		}

		return;
	}

	const ShenzhouIdentifyLevelCfg::AttributeConfig *level_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetIdentifyAttributeCfg(m_param.identify_level, m_param.identify_star_level);
	if (NULL == level_cfg) return;
	

	const ShenzhouIdentifyLevelCfg::AttributeConfig *next_level_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetIdentifyNextAttributeCfg(m_param.identify_level, m_param.identify_star_level);
	if (NULL == next_level_cfg) return;
	

	int old_exp = m_param.identify_exp;
	m_param.identify_exp += add_identify_exp;

	// 升级
	if (level_cfg->upgrade_need_exp > 0 && m_param.identify_exp >= level_cfg->upgrade_need_exp)
	{
		for(int i = 0; i < SHENZHOU_IDENTIFY_LEVEL_CFG_COUNT * SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT; i++)
		{
			if (m_param.identify_exp < level_cfg->upgrade_need_exp)
			{
				break;
			}

			m_param.identify_exp -= level_cfg->upgrade_need_exp;

			++ m_param.identify_star_level;

			// 给予
			if (m_param.identify_star_level > SHENZHOU_IDENTIFY_STAR_LEVEL_COUNT)
			{
				++ m_param.identify_level;
				m_param.identify_star_level = 0;

				if (0 != next_level_cfg->is_notice)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenzhou_identify_up_level,
						UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), m_param.identify_level, m_param.identify_star_level);
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}

			level_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetIdentifyAttributeCfg(m_param.identify_level, m_param.identify_star_level);
			if (NULL == level_cfg) break;;


			next_level_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetIdentifyNextAttributeCfg(m_param.identify_level, m_param.identify_star_level);
			if (NULL == next_level_cfg) break;
		}
	}
	
	if (is_notice_client)
	{
		this->SendBaseInfo();
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENZHOU_WEAPON);
	m_role->NoticeNum(noticenum::NT_SHENZHOU_WEAPON_UPGRADE_IDENTIFY_SUCC);

	gamelog::g_log_shenzhou_weapon.printf(LL_INFO, "ShenzhouWeapon::AddIdentifyExp user %d %s, old_exp %d, new_exp %d, new_level %d, new_star_level %d",
		m_role->GetUID(), m_role->GetName(), old_exp, m_param.identify_exp, m_param.identify_level, m_param.identify_star_level);
}

bool ShenzhouWeapon::CanGather(int gather_id)
{
	const ShenzhouOtherCfg &other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();
	if (other_cfg.gather_id == gather_id && m_param.today_gather_times >= other_cfg.role_day_gather_num + m_param.today_buy_gather_times)
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_TODAY_GATHER_TIME_MAX);
		return false;
	}

	return true;
}

void ShenzhouWeapon::BuyGatherTimes()
{
	const ShenzhouOtherCfg &other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();
	if (m_param.today_buy_gather_times >= other_cfg.buy_day_gather_num_limit)
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_BUY_TODAY_GATHER_TIME_LIMIT);
		return;
	}

	if (other_cfg.buy_day_gather_num_cost <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.buy_day_gather_num_cost, "BuyShenzhouWeaponGatherTimes"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_param.today_buy_gather_times++;
	this->SendBaseInfo();
	this->SendGatherInfo();
}

void ShenzhouWeapon::OnStartGather(int gather_id)
{
	const ShenzhouOtherCfg &other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();
	if (other_cfg.gather_id != gather_id)
	{
		return;
	}

	time_t now = EngineAdapter::Instance().Time();
	if (now > (m_last_gather_time + SHENZHOU_WEAPON_GATHER_BROADCAST_INTERVAL))
	{
		m_last_gather_time = now;

		// 发传闻
		int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenzhou_weapon_gather_content,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
		if (len > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
		}
	}
}

void ShenzhouWeapon::OnGather(int gather_id)
{
	const ShenzhouOtherCfg &other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();
	if (other_cfg.gather_id != gather_id)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (m_param.today_gather_times >= other_cfg.role_day_gather_num + m_param.today_buy_gather_times)
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_TODAY_GATHER_TIME_MAX);
		return;
	}

	ShenzhouGatherRewardCfg *gather_reward_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetGatherRandReward();
	if (NULL == gather_reward_cfg) return;

	if(NORMAL_GAHTER_REWARD == gather_reward_cfg->reward_type)
	{
		m_param.normal_item_num++;
	}
	else if (RARE_GATHER_REWARD == gather_reward_cfg->reward_type)
	{
		m_param.rare_item_num++;
	}
	else if (UNIQUE_GATHER_REWARD == gather_reward_cfg->reward_type)
	{
		m_param.unique_item_num++;

		// 发传闻
		int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenzhou_gather_top_class_stone,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
		if (len > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	++m_param.today_gather_times;
	m_role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_ANCIENT_YIJI_GATHER);
	m_role->GetKnapsack()->PutListOrMail(gather_reward_cfg->gather_reward, PUT_REASON_SHENZHOU_WEAPON_GATHER);
	this->SendBaseInfo();

	ActivityShadow::Instance().SynceShenzhouGatherInfoToAll();

	gamelog::g_log_shenzhou_weapon.printf(LL_INFO, "ShenzhouWeapon::OnGather user %d %s, gather_id %d, today_gather_times %d, get_item_1 %s, get_itme_2 %s, get_item_3 %s",
		m_role->GetUID(), m_role->GetName(), gather_id, m_param.today_gather_times, ItemConfigDataLog(&gather_reward_cfg->gather_reward[0], NULL), 
		ItemConfigDataLog(&gather_reward_cfg->gather_reward[1], NULL), ItemConfigDataLog(&gather_reward_cfg->gather_reward[2], NULL));
}

void ShenzhouWeapon::SkipGather()
{
	const ShenzhouOtherCfg &other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();
	if (m_role->GetLevel() < other_cfg.skip_gather_level_limit)
	{
		return;
	}

	int times = other_cfg.role_day_gather_num + m_param.today_buy_gather_times - m_param.today_gather_times;
	if (times > 0)
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.skip_gather_consume * times, "Shenzhouweapon skip gather"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		for (int i = 0; i < times; ++i)
		{
			this->OnGather(other_cfg.gather_id);
		}
	}
}

void ShenzhouWeapon::InviteHelpOtherBox()
{
	int help_num = 0;
	for (int i = 0; i < SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT; i++)
	{
		if (m_param.box_help_uid_list[i] != 0)
		{
			help_num++;
		}
	}

	if (SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT == help_num)
	{
		return;
	}

	static int rand_role_list[SCENE_MANAGER_RANDOM_GET_ROLE_MAX_COUNT]; memset(rand_role_list, 0, sizeof(rand_role_list));
	int rand_role_count = World::GetInstWorld()->GetSceneManager()->RandomGetRoleList(rand_role_list);

	if (0 == rand_role_count)
	{
		//m_role->NoticeNum(errornum::EN_SHEN_ZHOU_INVITE_HELP_BOX_FAIL);
		return;
	}

	int uid = 0;
	for (int i = 0; i < rand_role_count && i < SCENE_MANAGER_RANDOM_GET_ROLE_MAX_COUNT; i++)
	{
		for (int j = 0; j < SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT; j++)
		{
			if (rand_role_list[i] == m_param.box_help_uid_list[j] || rand_role_list[i] == m_role->GetUID())
			{
				break;
			}

			if (j == SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT - 1)
			{
				uid = rand_role_list[i];
				break;
			}
		}

		if (i == rand_role_count - 1 && 0 == uid)
		{
			//m_role->NoticeNum(errornum::EN_SHEN_ZHOU_INVITE_HELP_BOX_FAIL);
			return;
		}
	}

	for (int i = 0; i < SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT; i++)
	{
		if (0 == m_param.box_help_uid_list[i])
		{
			m_param.box_help_uid_list[i] = uid;
			//m_role->NoticeNum(errornum::EN_SHEN_ZHOU_INVITE_HELP_BOX_SUCC);
			break;
		}
	}

	this->SendBoxInfo();
}

void ShenzhouWeapon::HelpOtherBox(int uid)
{
// 	const ShenzhouOtherCfg &other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();
// 	if (m_param.today_help_box_num >= other_cfg.box_help_num_limit)
// 	{
// 		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_HELP_BOX_LIMIT);
// 		return;
// 	}
// 
// 	Role *other_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
// 	if (NULL == other_role)
// 	{
// 		m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
// 		return;
// 	}
// 
// 	ShenzhouWeapon * other_shenzhou = other_role->GetShenzhouWeapon();
// 	if (NULL == other_shenzhou) return;
// 
// 	ShenzhouBoxCfg & box_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetBoxCfg();
// 	if (box_cfg.box_id != other_shenzhou->GetShenZhouParam().box_id)
// 	{
// 		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_OTHER_BOX_YET_OPEN);
// 		return;
// 	}
// 
// 	if (m_role->GetUID() == uid)
// 	{
// 		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_CANT_HELP_SELF_BOX);
// 		return;
// 	}
// 	
// 	for (int i = 0; i < SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT; i++)
// 	{
// 		if (m_role->GetUID() == other_shenzhou->GetShenZhouParam().box_help_uid_list[i])
// 		{
// 			m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_CANT_BOUBLE_HELP_SELF_BOX);
// 			return;
// 		}
// 	}
// 	
// 	bool is_help_succ = false;
// 	for (int i = 0; i < SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT; i++)
// 	{
// 		if (0 == other_shenzhou->GetShenZhouParam().box_help_uid_list[i])
// 		{
// 			other_shenzhou->GetShenZhouParam().box_help_uid_list[i] = m_role->GetUID();
// 			is_help_succ = true;
// 			break;
// 		}
// 	}
// 
// 	Knapsack *knapsack = m_role->GetKnapsack();
// 	if (knapsack->IsLock())
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
// 		return;
// 	}
// 
// 	if (is_help_succ)
// 	{
// 		knapsack->PutOrMail(box_cfg.help_reward_item, PUT_REASON_SZLQ_HELP_BOX_REWARD);
// 
// 		m_param.today_help_box_num++;
// 		m_role->NoticeNum(noticenum::NT_SHEN_ZHOU_HELP_BOX_SUCCESS);
// 	}
// 	else
// 	{
// 		m_role->NoticeNum(errornum::EN_SHEN_ZHOU_HELP_BOX_FAIL);
// 	}
// 
// 	other_role->GetShenzhouWeapon()->SendBoxInfo();
// 	this->SendBoxInfo();
}

void ShenzhouWeapon::PutBox()
{
	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}
	
	ShenzhouBoxCfg & box_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetBoxCfg();
	if (!knapsack->ConsumeItem(box_cfg.box_id, 1, "ChineseZodiac::PromoteEquip"))
	{
		return;
	}
	
	m_param.box_id = box_cfg.box_id;
	this->SendBoxInfo();
}

void ShenzhouWeapon::OpenBox(int itmes)
{	
	if (itmes > MODE_MAX_COUNT) return;

	if (1 != itmes && 10 != itmes)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	ShenzhouBoxCfg & box_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetBoxCfg();
	if (box_cfg.box_id != m_param.box_id)
	{
		return;
	}

	int help_num = 0;
	for (int i = 0; i < SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT; i++)
	{
		if (m_param.box_help_uid_list[i] != 0)
		{
			help_num++;
		}
	}

	int item_num = 0;
	ItemConfigData item_list[SHENZHOU_HUNYIN_MAX_BOX_REWARD_COUNT];

	const ShenzhouOtherCfg & shenzhou_other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();

	//元宝不足，不给连续抽奖十次
	//if (itmes == 10)
	//{
	//	if (knapsack->GetMoney()->GetGold() < (box_cfg.cousume_diamond) * 10 && knapsack->GetItemNum(shenzhou_other_cfg.open_box_10_use_itemid) < shenzhou_other_cfg.open_box_10_use_item_num)
	//	{
	//		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
	//		return;
	//	}
	//}

	//元宝不足，不给连续抽奖十次
	if (itmes == 10)
	{
		if (shenzhou_other_cfg.can_use_item)
		{
			if (!m_role->GetKnapsack()->ConsumeItem(shenzhou_other_cfg.open_box_10_use_itemid, shenzhou_other_cfg.open_box_10_use_item_num, "ShenzhouWeaponOpenTenBox"))
			{
				if (!knapsack->GetMoney()->UseGold(box_cfg.cousume_diamond10, "ShenzhouWeaponOpenTenBox"))
				{
					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}
			}
		}
		else
		{
			if (!knapsack->GetMoney()->UseGold(box_cfg.cousume_diamond10, "ShenzhouWeaponOpenTenBox"))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
		}
	}

	if (itmes == 1)		//抽1次才有免费时间
	{
		if (EngineAdapter::Instance().Time() > m_param.last_free_open_box_time + shenzhou_other_cfg.box_free_times_cd &&  m_param.today_open_box_free_num < shenzhou_other_cfg.box_free_times)
		{
			m_param.last_free_open_box_time = (unsigned int)(EngineAdapter::Instance().Time());
			m_param.today_open_box_free_num++;
		}
		else
		{
			if (shenzhou_other_cfg.can_use_item)
			{
				if (!m_role->GetKnapsack()->ConsumeItem(shenzhou_other_cfg.replacement_id, shenzhou_other_cfg.consume_replacement, "ShenzhouWeaponOpenBox"))
				{
					if (!knapsack->GetMoney()->UseGold(box_cfg.cousume_diamond, "ShenZhouOpenBox"))
					{
						m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
						return;
					}
				}
			}
			else
			{
				if (!knapsack->GetMoney()->UseGold(box_cfg.cousume_diamond, "ShenZhouOpenBox"))
				{
					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}
			}
		}
	}

	int list_num = 0;
	ChestItemInfo xunbao_item_list[MODE_MAX_COUNT];
	for (int i = 0; i < MODE_MAX_COUNT; ++i)
	{
		xunbao_item_list[i].Reset();
	}

	for (int t = 0; t < itmes; t++)
	{
		//if (EngineAdapter::Instance().Time() > m_param.last_free_open_box_time + shenzhou_other_cfg.box_free_times_cd &&  m_param.today_open_box_free_num < shenzhou_other_cfg.box_free_times)
		//{
		//	m_param.last_free_open_box_time = (unsigned int)(EngineAdapter::Instance().Time());
		//	m_param.today_open_box_free_num++;
		//}
		//else
		//{
		//	if (shenzhou_other_cfg.can_use_item)
		//	{
		//		if (!m_role->GetKnapsack()->ConsumeItem(shenzhou_other_cfg.replacement_id, shenzhou_other_cfg.consume_replacement, "ShenzhouWeaponOpenBox"))
		//		{
		//			if (!knapsack->GetMoney()->UseGold(box_cfg.cousume_diamond, "ShenZhouOpenBox"))
		//			{
		//				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		//				return;
		//			}
		//		}
		//	}
		//	else
		//	{
		//		if (!knapsack->GetMoney()->UseGold(box_cfg.cousume_diamond, "ShenZhouOpenBox"))
		//		{
		//			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		//			return;
		//		}
		//	}
		//}

		int all_weight = 0;
		std::vector<ShenzhouBoxRewardCfg> box_reward_vec;
		LOGIC_CONFIG->GetShenzhouWeaponCfg().GetBoxRandRewardByLevel(m_role->GetLevel(), box_reward_vec, all_weight);
		int reward_num = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetBoxRewardCountByLevel(m_role->GetLevel(), help_num);
		for (int i = 0; i < reward_num; i++)
		{	
			if (all_weight <= 0) return;
			int rand_item_pro = RandomNum(all_weight);

			for (int j = 0; j < static_cast<int>(box_reward_vec.size()); j++)
			{
				if (rand_item_pro < box_reward_vec[j].weight && item_num < SHENZHOU_HUNYIN_MAX_BOX_REWARD_COUNT)
				{
					LotterySequence replace_j = static_cast<LotterySequence>(j);
					m_role->GetLotteryCount()->ReplaceLotterySequence(LOTTERY_REPLACE_TYPE_SHENZHOUWEAPON, replace_j);
					const std::vector<ShenzhouBoxRewardCfg> &box_reward_vec_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetBoxRewardCfg();
					if (replace_j < 0 || replace_j >= box_reward_vec_cfg.size())
					{
						replace_j = j;
					}
					const std::vector<ShenzhouBoxRewardCfg> &box_reward_vec_ref = (j == replace_j) ? box_reward_vec : box_reward_vec_cfg;	//根据是否替换过,来获取现配置还是原配置
					//knapsack->PutOrMail(box_reward_vec[replace_j].reward_item, PUT_REASON_SZLQ_OPEN_BOX_REWARD);
					//改为放进寻宝仓库
					if (list_num < MODE_MAX_COUNT)
					{
						xunbao_item_list[list_num].item_id = box_reward_vec_ref[replace_j].reward_item.item_id;
						xunbao_item_list[list_num].num = box_reward_vec_ref[replace_j].reward_item.num;
						xunbao_item_list[list_num].is_bind = box_reward_vec_ref[replace_j].reward_item.is_bind;
						++list_num;
					}

					//同步客户端显示
					item_list[item_num] = box_reward_vec_ref[replace_j].reward_item;
					item_num++;

					//珍稀传闻
					if (box_reward_vec_ref[replace_j].is_broadcast)
					{
						int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenzhou_treasure,
							m_role->GetUID(), (const char*)m_role->GetName(), (int)m_role->GetCamp(), (int)box_reward_vec_ref[replace_j].reward_item.item_id);

						if (sendlen > 0)
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE);
						}
					}

					break;
				}

				rand_item_pro -= box_reward_vec[j].weight;
			}
		}

		int temp_score = 0;
		temp_score = RandomNum(shenzhou_other_cfg.min_shenzhouweapon_score, shenzhou_other_cfg.max_shenzhouweapon_score);
		m_role->GetRoleShop()->AddChestShopShenZhouWeaponCredit(temp_score, "ShenZhouWeapon OpenBox");
	}

	if (item_num > 0)
	{
		m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_SHENZHOUWEAPON, list_num, xunbao_item_list, "SHENZHOUWEAPON OPEN BOX");
		ItemExtern::SendRewardListInfo(m_role, item_list, item_num, Protocol::SCRewardListInfo::NOTICE_TYPE_SHENZHOU_WEAPON_OPEN_BOX);
	}

	this->SendBoxInfo();
}

void ShenzhouWeapon::RemoveBoxHelp()
{
	memset(m_param.box_help_uid_list, 0, sizeof(m_param.box_help_uid_list));

	this->SendBoxInfo();
}

void ShenzhouWeapon::SendBoxInfo()
{
	static Protocol::SCShenzhouBoxInfo box_info;
	box_info.box_id = m_param.box_id;
	box_info.today_open_box_free_num = m_param.today_open_box_free_num;
	box_info.last_free_ggl_time = m_param.last_free_open_box_time;
	box_info.today_help_box_num = m_param.today_help_box_num;
	memcpy(box_info.box_help_uid_list, m_param.box_help_uid_list, sizeof(box_info.box_help_uid_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&box_info, sizeof(box_info));
}

void ShenzhouWeapon::HunyinInlay(int hunqi_id, int hunyin_slot, int index_in_bag)
{
	if (m_role->GetKnapsack()->IsLock() || m_role->GetSublock()->CheckHasLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT) return;

	if (hunyin_slot < 0 || hunyin_slot >= SHENZHOU_HUNYIN_MAX_SLOT) return;

	const ShenzhouHunyinSoltOpenCfg *slot_open_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetHunyinSlotOpenCfg(hunqi_id, hunyin_slot);
	if (NULL == slot_open_cfg) return;

	if (m_param.weapon_data[hunqi_id].weapon_level < slot_open_cfg->open_hunqi_level)
	{
		return;
	}

	ItemGridData grid_data;
	if (!m_role->GetKnapsack()->GetItemGridData(index_in_bag, &grid_data) || grid_data.Invalid()) return;

	const ShenzhouHunYinCfg *hunyin_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetHunYinCfg(grid_data.item_id);
	if (NULL == hunyin_cfg) return;

	if (hunyin_cfg->inlay_slot != hunyin_slot && hunyin_cfg->inlay_slot != 8)
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_HUNYIN_TYPE_LIMIT);
		return;
	}

	ShenzhouHunyinSlot &hunyin_item = m_param.weapon_data[hunqi_id].hunyin_slot[hunyin_slot];

	ItemConfigData old_hunyin;
	old_hunyin.item_id = hunyin_item.hunyin_id;
	old_hunyin.is_bind = hunyin_item.is_bind > 0 ? true : false;
	old_hunyin.num = 1;

	if (!m_role->GetKnapsack()->ConsumeItemByIndex(index_in_bag, 1, "HunyinInlay")) return;

	hunyin_item.hunyin_id = grid_data.item_id;
	hunyin_item.is_bind = grid_data.is_bind ? 1 : 0;

	this->RefreshHunyinSuitColor(hunqi_id);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENZHOU_WEAPON);

	this->SendBaseInfo();

	// 返还被替换的魂印
	if (old_hunyin.item_id > 0 && NULL != ITEMPOOL->GetItem(old_hunyin.item_id))
	{
		m_role->GetKnapsack()->PutOrMail(old_hunyin, PUT_REASON_SHENZHOU_HUNYIN_INLAY);
	}

	EventHandler::Instance().OnHunyinInlay(m_role, hunqi_id, hunyin_slot);
	
	gamelog::g_log_shenzhou_weapon.printf(LL_INFO, "HunyinInlay role(%d,%s) inlay_hunyin(%d), replace_hunyin(%d)",
		m_role->GetUID(), m_role->GetName(), grid_data.item_id, old_hunyin.item_id);
}

void ShenzhouWeapon::RefreshHunyinSuitColor(int hunqi_id)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT) return;

	ShenzhouWeaponParam::WeaponData &hunqi = m_param.weapon_data[hunqi_id];
	const ShenzhouHunYinCfg *hunyin_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetHunYinCfg(hunqi.hunyin_slot[0].hunyin_id);
	if (NULL == hunyin_cfg)
	{
		hunqi.hunyin_suit_level = HUNYIN_COLOR_COLOR_INVALID;
		return;
	}

	hunqi.hunyin_suit_level = hunyin_cfg->hunyin_color;

	for (int i = 1; i < SHENZHOU_HUNYIN_MAX_SLOT; i++)
	{
		const ShenzhouHunYinCfg *temp_hunyin_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetHunYinCfg(hunqi.hunyin_slot[i].hunyin_id);
		if (NULL == temp_hunyin_cfg)
		{
			hunqi.hunyin_suit_level = HUNYIN_COLOR_COLOR_INVALID;
			return;
		}

		if (temp_hunyin_cfg->hunyin_color < hunqi.hunyin_suit_level)
		{
			hunqi.hunyin_suit_level = temp_hunyin_cfg->hunyin_color;
		}
	}

	//this->ReCalcSuitCount();
}

void ShenzhouWeapon::HunyinResolve(Protocol::CSShenzhouHunyinResolveReq *cmd)
{
	if (NULL == cmd) return;

	for (int i = 0; i < cmd->index_count && i < Protocol::CSShenzhouHunyinResolveReq::HUNYIN_RESOLVE_MAX_INDEX_NUM; i++)
	{
		ItemGridData grid_data;
		if (!m_role->GetKnapsack()->GetItemGridData(cmd->index_in_bag_list[i], &grid_data) || grid_data.Invalid()) return;

		const ShenzhouHunYinCfg *hunyin_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetHunYinCfg(grid_data.item_id);
		if (NULL == hunyin_cfg) return;

		if (!m_role->GetKnapsack()->ConsumeItemByIndex(cmd->index_in_bag_list[i], grid_data.num, "HunyinResolve")) return;

		m_param.lingshu_exp += hunyin_cfg->discard_exp * grid_data.num;
		m_role->GetRoleShop()->AddChestShopHunyinLingzhi(hunyin_cfg->blue_lingzhi * grid_data.num, hunyin_cfg->purple_lingzhi * grid_data.num, hunyin_cfg->orange_lingzhi * grid_data.num, "HunyinResolve");

		gamelog::g_log_shenzhou_weapon.printf(LL_INFO, "HunyinResolve role(%d,%s) inlay_hunyin(%d), add_exp(%d)",
			m_role->GetUID(), m_role->GetName(),  grid_data.item_id, hunyin_cfg->discard_exp * grid_data.num);
	}

	this->SendBaseInfo();
}

void ShenzhouWeapon::LingshuUplevel(int hunqi_id, int hunyin_slot, bool is_notice_client)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT) return;

	if (hunyin_slot < 0 || hunyin_slot >= SHENZHOU_HUNYIN_MAX_SLOT) return;

	if (m_role->GetKnapsack()->IsLock() || m_role->GetSublock()->CheckHasLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	ShenzhouWeaponParam::WeaponData &hunqi = m_param.weapon_data[hunqi_id];

	const ShenzhouLingshuLevelCfg::LingshuLevelCfg *cur_level_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetLingshuLevelAttrCfg(hunqi_id, hunyin_slot, hunqi.hunyin_slot[hunyin_slot].lingshu_level);
	if (NULL == cur_level_cfg) return;
	

	const ShenzhouLingshuLevelCfg::LingshuLevelCfg *next_level_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetLingshuLevelAttrCfg(hunqi_id, hunyin_slot, hunqi.hunyin_slot[hunyin_slot].lingshu_level + 1);
	if (NULL == next_level_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_LINGSHU_LEVEL_MAX);
		return;
	}

	if (0 >= hunqi.hunyin_slot[hunyin_slot].hunyin_id)
	{
		switch (hunyin_slot)
		{
			case 0:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_WOLF_NOT_INLAY_LIMIT);
			}
			break;

			case 1:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_LEOPARD_NOT_INLAY_LIMIT);
			}
			break;

			case 2:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_BEAR_NOT_INLAY_LIMIT);
			}
			break;

			case 3:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_LION_NOT_INLAY_LIMIT);
			}
			break;

			case 4:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_TIGER_NOT_INLAY_LIMIT);
			}
			break;

			case 5:
			{	
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_KYLIN_NOT_INLAY_LIMIT);
			}
			break;

			case 6:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_PHOENIX_NOT_INLAY_LIMIT);
			}
			break;

			case 7:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_DRAGON_NOT_INLAY_LIMIT);
			}
			break;
		}

		return;
	}

	const ShenzhouHunYinCfg *hunyin_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetHunYinCfg(hunqi.hunyin_slot[hunyin_slot].hunyin_id);
	if (NULL == hunyin_cfg) return;

	if (next_level_cfg->lingshu_color > hunyin_cfg->hunyin_color)
	{
		switch (hunyin_slot)
		{
			case 0:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_WOLF_COLOR_LIMIT);
			}
			break;

			case 1:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_LEOPARD_COLOR_LIMIT);
			}
			break;

			case 2:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_BEAR_COLOR_LIMIT);
			}
			break;

			case 3:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_LION_COLOR_LIMIT);
			}
			break;

			case 4:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_TIGER_COLOR_LIMIT);
			}
			break;

			case 5:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_KYLIN_COLOR_LIMIT);
			}
			break;

			case 6:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_PHOENIX_COLOR_LIMIT);
			}
			break;

			case 7:
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_DRAGON_COLOR_LIMIT);
			}
			break;
		}
		//m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_LINGSHU_COLOR_LIMIT);
		return;
	}

	if (m_param.lingshu_exp < cur_level_cfg->up_level_exp)
	{
		if (is_notice_client)
		{
			m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_LINGSHU_EXP_NOT_ENOUGH);
		}
		return;
	}

	m_param.lingshu_exp -= cur_level_cfg->up_level_exp;

	// 给予
	int old_lingshu_level = hunqi.hunyin_slot[hunyin_slot].lingshu_level;
	++hunqi.hunyin_slot[hunyin_slot].lingshu_level;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENZHOU_WEAPON);

	this->SendBaseInfo();

	gamelog::g_log_shenzhou_weapon.printf(LL_INFO, "ShenzhouWeapon::LingshuUpLevel user %d %s, hunqi %d, hunyin_slot %d, old_level %d , new_level %d",
		m_role->GetUID(), m_role->GetName(), hunqi_id, hunyin_slot, old_lingshu_level, hunqi.hunyin_slot[hunyin_slot].lingshu_level);
}

void ShenzhouWeapon::AutoLingshuUplevel(int hunqi_id)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT) return;

	std::set<int> valid_hunyin_slot_set;

	for (int hunyin_slot = 0; hunyin_slot < SHENZHOU_HUNYIN_MAX_SLOT; hunyin_slot++)
	{
		ShenzhouWeaponParam::WeaponData &hunqi = m_param.weapon_data[hunqi_id];
		if (0 >= hunqi.hunyin_slot[hunyin_slot].hunyin_id)
		{
			continue;
		}

		const ShenzhouLingshuLevelCfg::LingshuLevelCfg *next_level_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetLingshuLevelAttrCfg(hunqi_id, hunyin_slot, hunqi.hunyin_slot[hunyin_slot].lingshu_level + 1);
		if (NULL == next_level_cfg)
		{
			continue;
		}

		valid_hunyin_slot_set.insert(hunyin_slot);
	}

	int count = 0;
	for (auto it : valid_hunyin_slot_set)
	{
		if (static_cast<size_t>(++count) == valid_hunyin_slot_set.size())
		{
			this->LingshuUplevel(hunqi_id, it, true);
		}
		else
		{
			this->LingshuUplevel(hunqi_id, it, false);
		}
	}
}

void ShenzhouWeapon::OnCompleteTask(TaskID task_id)
{
	this->CheckFuncOpen();
}

int ShenzhouWeapon::GetActiveCount()
{
	int count = 0;
	for (const auto &data : m_param.weapon_data)
	{
		if (data.weapon_level > 0)
		{
			++count;
		}
	}

	return count;
}

int ShenzhouWeapon::GetHunyinInlayCount(int hunyin_color)
{
	int count = 0;
	for (const auto &data : m_param.weapon_data)
	{
		for (unsigned int slot_index = 0; slot_index < SHENZHOU_HUNYIN_MAX_SLOT && slot_index < static_array_size(data.hunyin_slot); slot_index ++)
		{
			const ShenzhouHunYinCfg *hunyin_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetHunYinCfg(data.hunyin_slot[slot_index].hunyin_id);
			if (NULL == hunyin_cfg)
			{
				continue;
			}

			if (hunyin_cfg->hunyin_color >= hunyin_color)
			{
				++count;
			}
		}
	}

	return count;
}

void ShenzhouWeapon::OpenHunYinSlot(int hunqi_id, int hunyin_slot, bool is_notice, bool is_check_open)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT) return;

	if (hunyin_slot < 0 || hunyin_slot >= SHENZHOU_HUNYIN_MAX_SLOT) return;

	ShenzhouHunyinSlot &hunyin_item = m_param.weapon_data[hunqi_id].hunyin_slot[hunyin_slot];

	if (hunyin_item.reserve1 > 0)
	{
		if (is_notice)
		{
			m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_XILIAN_OPEN_SLOT_LIMIT);
		}
		return;
	}

	const ShenzhouHunyinSoltOpenCfg *slot_open_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetHunyinSlotOpenCfg(hunqi_id, hunyin_slot);
	if (NULL == slot_open_cfg) return;

	if (slot_open_cfg->gold == 0) //元宝为0，魂印槽默认开启
	{
		hunyin_item.reserve1 = 1;
		return;
	}

	if (slot_open_cfg->gold > 0 && is_check_open)		//只是登录检查开启0元宝的魂印槽，不能自动使用元宝开启
	{
		return;
	}

	if (slot_open_cfg->limit_1 < 0 || slot_open_cfg->limit_1 >= SHENZHOU_WEAPON_COUNT) return;
	if (slot_open_cfg->limit_2 < 0 || slot_open_cfg->limit_2 >= SHENZHOU_HUNYIN_MAX_SLOT) return;

	if (slot_open_cfg->limit_1 != hunqi_id)		//如果前置魂器等于当前魂器，则认为limit_1自动失效
	{
		if (slot_open_cfg->limit_1 >= 1)
		{
			if (m_param.weapon_data[slot_open_cfg->limit_1].weapon_level < 1)				//前一个魂器未激活
			{
				m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_HUNYIN_OPEN_LIMIT_1);
				return;
			}
		}
	}

	if (m_param.weapon_data[hunqi_id].hunyin_slot[slot_open_cfg->limit_2].reserve1 < 0)		//前一个魂印槽未开启
	{
		m_role->NoticeNum(errornum::EN_SHENZHOU_WEAPON_HUNYIN_OPEN_LIMIT_2);
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->GetMoney()->UseAllGold(slot_open_cfg->gold, "ShenZhouOpenHunYinSlot"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	hunyin_item.reserve1 = 1;

	this->SendBaseInfo();
}

void ShenzhouWeapon::OnRoleLogin()
{
	this->CheckFuncOpen();
	for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
	{
		for (int j = 0; j < SHENZHOU_HUNYIN_MAX_SLOT; j++)
		{
			this->OpenHunYinSlot(i, j, false, true);		//开启免费的魂印槽
		}
	}

	//this->ReCalcSuitCount();
}

void ShenzhouWeapon::GMAddHunqiLevel(int hunqi_id)
{
#ifndef _DEBUG
	return;
#endif
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT)
	{
		return;
	}
	ShenzhouWeaponParam::WeaponData &hunqi = m_param.weapon_data[hunqi_id];
	for (int i = 0; i < SHENZHOU_WEAPON_SLOT_COUNT; ++i)
	{
		++hunqi.weapon_slot_level[i];
	}
	
	m_role->SendOperateResult(Protocol::SCOperateResult::OP_SHENZHOU_WEAPON, 1);

	this->RefreshWeaponLevel(hunqi_id);
	this->SendBaseInfo();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENZHOU_WEAPON);
}

void ShenzhouWeapon::OnRoleLevelChange(int level)
{
	this->CheckFuncOpen();
}

void ShenzhouWeapon::CheckFuncOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (m_role->GetRoleBigSmallGoal()->IsFuncActive(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENZHOU_WEAPON))
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENZHOU_WEAPON);
	if (fun_open_cfg != nullptr && opengame_day >= fun_open_cfg->opengame_day)
	{
		if (m_role->GetLevel() >= fun_open_cfg->role_level || m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
		{
			m_role->GetRoleBigSmallGoal()->OnOpenFunc(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENZHOU_WEAPON);

			//开启异火系统，同时激活默认异火
			for (int i = 0; i < SHENZHOU_WEAPON_COUNT; i++)
			{
				if (0 == (m_param.xilian_data[i].xilian_slot_open_falg & (1 << 0)))
				{
					const SZXilianOpenCfg *xilian_open_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetXilianOpenCfg(i, 0);
					if (NULL != xilian_open_cfg)
					{
						if (m_param.weapon_data[i].weapon_level >= xilian_open_cfg->lingshu_level_limit)
						{
							m_param.xilian_data[i].xilian_slot_open_falg |= (1 << 0);
							this->XilianOneReq(i, 0, XL_COUMME_COLOR_FREE, false);
						}
					}
				}
			}
		}
	}
}

void ShenzhouWeapon::ReCalcSuitCount()
{
	memset(m_hunyin_quality_count, 0, sizeof(m_hunyin_quality_count));
	// 每个器件，每个部位
	for (int hunqi_id = 0; hunqi_id < SHENZHOU_WEAPON_COUNT; hunqi_id++)
	{
		for (int hunyin_slot = 0; hunyin_slot < SHENZHOU_HUNYIN_MAX_SLOT; hunyin_slot++)
		{
			// 魂印
			const ShenzhouHunYinCfg *hunyin_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetHunYinCfg(m_param.weapon_data[hunqi_id].hunyin_slot[hunyin_slot].hunyin_id);
			if (NULL == hunyin_cfg)
			{
				continue;
			}

			if (hunyin_cfg->hunyin_color > 0 && hunyin_cfg->hunyin_color < HUNYIN_COLOR_MAX)
			{
				++m_hunyin_quality_count[hunqi_id][hunyin_cfg->hunyin_color];
			}
		}
	}
}

int ShenzhouWeapon::GetHunyinQualityCount(int hunqi_id, int color)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT || color < 0 || color >= HUNYIN_COLOR_MAX)
	{
		return 0;
	}
	return m_hunyin_quality_count[hunqi_id][color];
}

int ShenzhouWeapon::GetHunyinSuitQualityCount(int hunqi_id, int color)
{
	if (hunqi_id < 0 || hunqi_id >= SHENZHOU_WEAPON_COUNT || color < 0 || color >= HUNYIN_COLOR_MAX)
	{
		return 0;
	}
	UNSTD_STATIC_CHECK(HUNYIN_COLOR_PINK < HUNYIN_COLOR_MAX);
	int hunyin_quality_count = 0;
	if (color == HUNYIN_COLOR_RED)
	{
		hunyin_quality_count = m_hunyin_quality_count[hunqi_id][color] + m_hunyin_quality_count[hunqi_id][HUNYIN_COLOR_PINK];
	}
	else
	{
		hunyin_quality_count = m_hunyin_quality_count[hunqi_id][color];
	}
	return hunyin_quality_count;
}
