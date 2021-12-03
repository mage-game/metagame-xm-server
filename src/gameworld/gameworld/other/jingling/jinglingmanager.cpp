#include "gamelog.h"
#include "servercommon/servercommon.h"
#include "item/knapsack.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "obj/character/attribute.hpp"
#include "item/itempool.h"
#include "other/event/eventhandler.hpp"
#include "jinglingmanager.h"
#include "equipment/equipment.h"
#include "item/itembase.h"
#include "item/itemgriddata.h"
#include "protocal/msgjingling.h"
#include "protocal/msgfight.h"
#include "scene/scene.h"
#include "skill/skillbase.hpp"
#include "skill/skillmanager.hpp"
#include "gameworld/equipment/equipmentfunction.h"

#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "globalconfig/globalconfig.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/namefilter.hpp"

#include "config/logicconfigmanager.hpp"
#include "other/jingling/jinglingconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "global/globalrole/jinglinghome/jinglinghome.hpp"
#include "other/capability/capability.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/achieve/title/titleconfig.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "config/joinlimitconfig.hpp"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "task/taskrecorder.h"
#include "effect/skilleffect/effectonce.hpp"
#include "gameworld/protocal/msgrole.h"
#include "config/rolebigsmallgoalconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

JingLingManager::JingLingManager() : m_role(NULL), m_group_skill_id(0), m_group_num(0), m_use_jingling_index(-1), 
	m_use_jingling_id(0), m_grade(0), m_grade_bless(0), m_active_image_flag(0), m_used_imageid(0),
	m_phantom_imgageid(-1), m_soul_taozhuang_level(0), m_halo_active_image_flag(0),
	m_halo_level(0), m_use_halo_img(0), m_halo_bless_val(0), m_xianzhen_level(0), m_xianzhen_exp(0), m_xianzhen_up_count(0),
	m_skill_lei_ting_id(0), m_skill_xi_xue_id(0), m_skill_zhi_ming_id(0), m_skill_fan_tan_id(0),
	m_history_max_capability(0), m_explore_maxhp(0), m_explore_hp(0), m_explore_mode(0), m_explore_buy_buff_count(0),
	m_history_max_chengzhang(0), m_history_max_wuxing(0),
	m_skill_last_perform_time_lei_ting(0), m_skill_last_perform_time_xixue(0), m_skill_last_perform_time_zhi_ming(0), m_skill_last_perform_time_fan_tan(0)
{
	memset(m_phantom_image_flag, 0, sizeof(m_phantom_image_flag));
	memset(m_groupid_list, 0, sizeof(m_groupid_list));
	memset(m_jingling_name, 0, sizeof(m_jingling_name));
	memset(m_shuxingdan_list, 0, sizeof(m_shuxingdan_list));
	memset(m_equip_strength_list, 0, sizeof(m_equip_strength_list));
	memset(m_phantom_level_list, 0, sizeof(m_phantom_level_list));
	memset(m_soul_level_list, 0, sizeof(m_soul_level_list));
	memset(m_hunyu_level_list, 0, sizeof(m_hunyu_level_list));
}

JingLingManager::~JingLingManager() 
{
 
}

void JingLingManager::Init(Role *role, const JingLingParam &jingling_param)
{
	m_role = role;
	
	for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; ++ i)
	{
		ItemGridData *grid_data = &m_jingling_list[i];
		grid_data->item_id = jingling_param.jingling_list[i].item_id;
		grid_data->is_bind = (jingling_param.jingling_list[i].is_bind == 1);
		EquipmentParam *equip_param = new EquipmentParam();
		equip_param->strengthen_level = jingling_param.jingling_list[i].level;
		equip_param->param1 = jingling_param.jingling_list[i].wuxing_level;
		equip_param->param2 = jingling_param.jingling_list[i].max_wuxing_level;
		equip_param->param3 = jingling_param.jingling_list[i].ling_jing;
		memcpy(equip_param->xianpin_type_list, jingling_param.jingling_list[i].talent_list, sizeof(equip_param->xianpin_type_list));
		memcpy(equip_param->jing_ling_skill_list, jingling_param.jingling_list[i].jing_ling_skill_list, sizeof(equip_param->jing_ling_skill_list));
		grid_data->param = equip_param;
	}

	m_use_jingling_id = jingling_param.use_jingling_id;
	m_use_jingling_index = this->GetJingLingIndexByItemId(m_use_jingling_id);

	UNSTD_STATIC_CHECK(sizeof(m_card_list) == sizeof(jingling_param.card_list));
	memcpy(m_card_list, jingling_param.card_list, sizeof(m_card_list));

	F_STRNCPY(m_jingling_name, jingling_param.jingling_name, sizeof(GameName)); 

	UNSTD_STATIC_CHECK(sizeof(m_shuxingdan_list) == sizeof(jingling_param.shuxingdan_list));
	memcpy(m_shuxingdan_list, jingling_param.shuxingdan_list, sizeof(m_shuxingdan_list));

	m_grade = jingling_param.jingling_grade;
	m_grade_bless = jingling_param.grade_bless;

	m_active_image_flag = jingling_param.active_image_flag;
	m_used_imageid = jingling_param.used_imageid;

	UNSTD_STATIC_CHECK(sizeof(m_equip_strength_list) == sizeof(jingling_param.equip_strength_list));
	memcpy(m_equip_strength_list, jingling_param.equip_strength_list, sizeof(m_equip_strength_list));

	UNSTD_STATIC_CHECK(sizeof(m_phantom_level_list) == sizeof(jingling_param.phantom_level_list));
	memcpy(m_phantom_level_list, jingling_param.phantom_level_list, sizeof(m_phantom_level_list));

	m_phantom_imgageid = jingling_param.phantom_imageid;
	UNSTD_STATIC_CHECK(sizeof(m_phantom_image_flag) == sizeof(jingling_param.phantom_image_flag));
	memcpy(m_phantom_image_flag, jingling_param.phantom_image_flag, sizeof(m_phantom_image_flag));

	UNSTD_STATIC_CHECK(sizeof(m_soul_level_list) ==  sizeof(jingling_param.soul_level_list));
	memcpy(m_soul_level_list, jingling_param.soul_level_list, sizeof(m_soul_level_list));

	m_halo_active_image_flag = jingling_param.halo_active_image_flag;
	m_halo_level = jingling_param.halo_level;
	m_use_halo_img = jingling_param.use_halo_img;
	m_halo_bless_val = jingling_param.halo_bless_val;

	m_xianzhen_level = jingling_param.xianzhen_level;
	m_xianzhen_exp = jingling_param.xianzhen_exp;
	m_xianzhen_up_count = jingling_param.xianzhen_up_count;
	memcpy(m_hunyu_level_list, jingling_param.hunyu_level_list, sizeof(m_hunyu_level_list));
	memcpy(m_skill_storage_list, jingling_param.skill_storage_list, sizeof(m_skill_storage_list));
	memcpy(m_skill_refresh_item_list, jingling_param.skill_refresh_item_list, sizeof(m_skill_refresh_item_list));

	m_history_max_capability = jingling_param.history_max_capability;
	m_explore_maxhp = jingling_param.explore_maxhp;
	m_explore_hp = jingling_param.explore_hp;
	m_explore_mode = jingling_param.explore_mode;
	m_explore_buy_buff_count = jingling_param.explore_buy_buff_count;
	memcpy(m_explore_info_list, jingling_param.explore_info_list, sizeof(m_explore_info_list));

	m_history_max_chengzhang = jingling_param.history_max_chengzhang;
	m_history_max_wuxing = jingling_param.history_max_wuxing;

	m_skill_last_perform_time_lei_ting = jingling_param.skill_last_perform_time_lei_ting;
	m_skill_last_perform_time_xixue = jingling_param.skill_last_perform_time_xixue;
	m_skill_last_perform_time_zhi_ming = jingling_param.skill_last_perform_time_zhi_ming;
	m_skill_last_perform_time_fan_tan = jingling_param.skill_last_perform_time_fan_tan;
	
	this->ReCalcGroup();

	this->UpdateSoulTaoZhuangLevel();
}

void JingLingManager::GetInitParam(JingLingParam *jingling_param)
{
	jingling_param->Reset();

	for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; ++ i)
	{
		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_jingling_list[i].item_id);
		if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() || Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
		{
			continue;
		}

		JingLingParamItem *item = &jingling_param->jingling_list[i];
		item->item_id = m_jingling_list[i].item_id;
		item->is_bind = m_jingling_list[i].is_bind ? 1 : 0;

		EquipmentParam *equip_param = (EquipmentParam *)m_jingling_list[i].param;
		item->level = equip_param->strengthen_level;
		item->wuxing_level = equip_param->param1;
		item->max_wuxing_level = equip_param->param2;
		item->ling_jing = equip_param->param3;
		memcpy(item->talent_list, equip_param->xianpin_type_list, sizeof(item->talent_list));
		memcpy(item->jing_ling_skill_list, equip_param->jing_ling_skill_list, sizeof(item->jing_ling_skill_list));
	}

	jingling_param->use_jingling_id = m_use_jingling_id;

	UNSTD_STATIC_CHECK(sizeof(m_card_list) == sizeof(jingling_param->card_list));
	memcpy(jingling_param->card_list, m_card_list, sizeof(jingling_param->card_list));

	F_STRNCPY(jingling_param->jingling_name, m_jingling_name, sizeof(GameName));

	UNSTD_STATIC_CHECK(sizeof(jingling_param->shuxingdan_list) == sizeof(m_shuxingdan_list));
	memcpy(jingling_param->shuxingdan_list, m_shuxingdan_list, sizeof(jingling_param->shuxingdan_list));

	jingling_param->jingling_grade = m_grade;
	jingling_param->grade_bless = m_grade_bless;
	jingling_param->active_image_flag = m_active_image_flag;
	jingling_param->used_imageid = m_used_imageid;

	UNSTD_STATIC_CHECK(sizeof(m_equip_strength_list) == sizeof(jingling_param->equip_strength_list));
	memcpy(jingling_param->equip_strength_list, m_equip_strength_list, sizeof(jingling_param->equip_strength_list));

	memcpy(jingling_param->phantom_level_list, m_phantom_level_list, sizeof(jingling_param->phantom_level_list));
	jingling_param->phantom_imageid = m_phantom_imgageid;
	UNSTD_STATIC_CHECK(sizeof(m_phantom_image_flag) == sizeof(jingling_param->phantom_image_flag));
	memcpy(jingling_param->phantom_image_flag, m_phantom_image_flag, sizeof(jingling_param->phantom_image_flag));

	UNSTD_STATIC_CHECK(sizeof(m_soul_level_list) ==  sizeof(jingling_param->soul_level_list));
	memcpy(jingling_param->soul_level_list, m_soul_level_list, sizeof(jingling_param->soul_level_list));

	jingling_param->halo_active_image_flag = m_halo_active_image_flag;
	jingling_param->halo_level = m_halo_level;
	jingling_param->use_halo_img = m_use_halo_img;
	jingling_param->halo_bless_val = m_halo_bless_val;

	jingling_param->xianzhen_level = m_xianzhen_level;
	jingling_param->xianzhen_exp = m_xianzhen_exp;
	jingling_param->xianzhen_up_count = m_xianzhen_up_count;
	memcpy(jingling_param->hunyu_level_list, m_hunyu_level_list, sizeof(jingling_param->hunyu_level_list));
	memcpy(jingling_param->skill_storage_list, m_skill_storage_list, sizeof(jingling_param->skill_storage_list));
	memcpy(jingling_param->skill_refresh_item_list, m_skill_refresh_item_list, sizeof(jingling_param->skill_refresh_item_list));

	jingling_param->history_max_capability = m_history_max_capability;
	jingling_param->explore_maxhp = m_explore_maxhp;
	jingling_param->explore_hp = m_explore_hp;
	jingling_param->explore_mode = m_explore_mode;
	jingling_param->explore_buy_buff_count = m_explore_buy_buff_count;
	memcpy(jingling_param->explore_info_list, m_explore_info_list, sizeof(jingling_param->explore_info_list));

	jingling_param->history_max_chengzhang = m_history_max_chengzhang;
	jingling_param->history_max_wuxing = m_history_max_wuxing;

	jingling_param->skill_last_perform_time_lei_ting = m_skill_last_perform_time_lei_ting;
	jingling_param->skill_last_perform_time_xixue = m_skill_last_perform_time_xixue;
	jingling_param->skill_last_perform_time_zhi_ming = m_skill_last_perform_time_zhi_ming;
	jingling_param->skill_last_perform_time_fan_tan = m_skill_last_perform_time_fan_tan;
}

void JingLingManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		m_other_capability = 0;
		this->ReCalcFightOutJingLingAttr(m_other_capability);
		this->ReCalcNoFightOutJingLingAttr(m_other_capability);
		// 大目标的战力计算只对上面几个函数有加成，所以要先保证先执行上面几个函数
		this->ReCalcSpecialJinglingAttr();

		this->ReCalcGroupAttr();
		this->ReCalcCardAttr();
		this->ReCalcShuXinDanAttr();
		this->ReCalcGradeAttr();
		this->ReCalcEquipAttr();
		this->ReCalcPhantomAttr();
		this->ReCalcSoulAttr();
		this->ReCalcSoulTaoZhuangAttr();

//		this->ReCalcJingLingHaloAttr();

		JingLingHome *jing_ling_home = GLOBAL_ROLE_MGR.GeJingLingHome(m_role->GetUID());
		if (NULL != jing_ling_home)
		{
			jing_ling_home->OnJingLingCapabilityChange(m_role);
		}
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_JINGLING, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_JINGLING, m_other_capability);
	int capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING);
	if (capability > m_history_max_capability)
	{
		m_history_max_capability = capability;
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

void JingLingManager::ReCalcFightOutJingLingAttr(int &other_capability)
{
	if (m_use_jingling_index >= 0 && m_use_jingling_index < JINGLING_MAX_TAKEON_NUM && m_use_jingling_id > 0)
	{
		const ItemGridData *grid_data = &m_jingling_list[m_use_jingling_index];
		if (!grid_data->Invalid() && grid_data->item_id == m_use_jingling_id)
		{
			const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
			if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() || Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
			{
				return;
			}

			EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
			const JingLingItemCfg *item_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingItemCfg(grid_data->item_id, equip_param->strengthen_level);
			if (NULL == item_cfg)
			{
				return;
			}

			int maxhp_zizhi = 0, gongji_zizhi = 0, fangyu_zizhi = 0;

			const JingLingTalentAttrItemCfg *talent_cfg = LOGIC_CONFIG->GetJingLingCfg().GetTalentAttrCfg(grid_data->item_id);
			if (NULL != talent_cfg)
			{
				maxhp_zizhi += talent_cfg->maxhp_zizhi;
				gongji_zizhi += talent_cfg->gongji_zizhi;
				fangyu_zizhi += talent_cfg->fangyu_zizhi;
			}

			// 单个精灵增加的属性集
			CharIntAttrs single_jingling_attr_add;

			const JingLingWuXingCfg *wuxing_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingWuXingCfg(equip->GetColor(), equip_param->param1);
			if (NULL != wuxing_cfg)
			{
				// 精灵悟性加成 -- 对全身属性进行加成
				single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(wuxing_cfg->maxhp_zizhi * ROLE_ATTR_PER);
				single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(wuxing_cfg->gongji_zizhi * ROLE_ATTR_PER);
				single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<float>(wuxing_cfg->fangyu_zizhi * ROLE_ATTR_PER);

				single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += wuxing_cfg->maxhp;
				single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += wuxing_cfg->gongji;
				single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += wuxing_cfg->fangyu;
			}

			//base
			single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(item_cfg->maxhp + item_cfg->maxhp * (maxhp_zizhi * ROLE_ATTR_PER));
			single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(item_cfg->gongji + item_cfg->gongji * (gongji_zizhi * ROLE_ATTR_PER));
			single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(item_cfg->fangyu + item_cfg->fangyu * (fangyu_zizhi * ROLE_ATTR_PER));

			//talent
			for (int k = 0; k < ItemNamespace::MAX_XIANPIN_NUM; ++ k)
			{
				int talent_type = equip_param->xianpin_type_list[k];
				int talent_value = LOGIC_CONFIG->GetJingLingCfg().GetTalentValue(grid_data->item_id, talent_type);

				if (talent_value > 0)
				{
					switch (talent_type)
					{
					case JL_TALENT_GONGJI:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_FANGYU:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_MAXHP:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_MINGZHONG:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_SHANGBI:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_BAOJI:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_JIANREN:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_JINGZHUN_PER:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_BAOJI_PER:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_POFANG_PER:
//						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += talent_value * EQUIP_ATTR_PER;
						single_jingling_attr_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, talent_value);
						break;

					case JL_TALENT_MIANSHANG_PER:
//						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += talent_value * EQUIP_ATTR_PER;
						single_jingling_attr_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, talent_value);
						break;
					}
				}
			}

			// 技能
			Attribute maxhp_per = 0, gongji_per = 0, fangyu_per = 0;
			m_skill_lei_ting_id = 0;
			m_skill_xi_xue_id = 0;
			m_skill_zhi_ming_id = 0;
			m_skill_fan_tan_id = 0;

			for (int k = 0; k < JING_LING_SKILL_COUNT_MAX; k++)
			{
				const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(equip_param->jing_ling_skill_list[k].skill_id);
				if (NULL != skill_cfg)
				{
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += skill_cfg->maxhp;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += skill_cfg->gongji;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += skill_cfg->fangyu;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += skill_cfg->mingzhong;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += skill_cfg->shanbi;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += skill_cfg->baoji;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += skill_cfg->kangbao;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += skill_cfg->constant_zengshang;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += skill_cfg->constant_mianshang;

					maxhp_per += skill_cfg->maxhp_per;
					gongji_per += skill_cfg->gongji_per;
					fangyu_per += skill_cfg->fangyu_per;

					other_capability += skill_cfg->capability;

					if (SKILL_LEI_TING_TYPE == skill_cfg->skill_type)
					{
						m_skill_lei_ting_id = skill_cfg->skill_id;
					}
					else if (SKILL_XI_XUE_TYPE == skill_cfg->skill_type)
					{
						m_skill_xi_xue_id = skill_cfg->skill_id;
					}
					else if (SKILL_ZHI_MING_TYPE == skill_cfg->skill_type)
					{
						m_skill_zhi_ming_id = skill_cfg->skill_id;
					}
					else if (SKILL_FAN_TAN_TYPE == skill_cfg->skill_type)
					{
						m_skill_fan_tan_id = skill_cfg->skill_id;
					}
				}
			}

			single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (maxhp_per * ROLE_ATTR_PER));
			single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (gongji_per * ROLE_ATTR_PER));
			single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (fangyu_per * ROLE_ATTR_PER));
		
			// 将该精灵的属性加入到玩家属性
			m_attrs_add.Add(single_jingling_attr_add.m_attrs);
			m_attrs_add.AddPercent(single_jingling_attr_add.m_percent_attrs);
		}
	}
}

//void JingLingManager::ReCalcNoFightOutJingLingAttr()
//{
//	Attribute add_maxhp = 0, add_gongji = 0, add_fangyu = 0;
//	Attribute skill_maxhp = 0, skill_gongji = 0, skill_fangyu = 0;
//	Attribute maxhp_per = 0, gongji_per = 0, fangyu_per = 0;
//
//	for (int jing_ling_index = 0; jing_ling_index < JINGLING_MAX_TAKEON_NUM; jing_ling_index++)
//	{
//		if (jing_ling_index == m_use_jingling_index) continue;
//
//		const ItemGridData *grid_data = &m_jingling_list[jing_ling_index];
//		if (!grid_data->Invalid())
//		{
//			const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
//			if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() || Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
//			{
//				continue;
//			}
//
//			EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
//			const JingLingItemCfg *item_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingItemCfg(grid_data->item_id, equip_param->strengthen_level);
//			if (NULL == item_cfg)
//			{
//				continue;
//			}
//
//			int maxhp_zizhi = 0, gongji_zizhi = 0, fangyu_zizhi = 0;
//
//			const JingLingTalentAttrItemCfg *talent_cfg = LOGIC_CONFIG->GetJingLingCfg().GetTalentAttrCfg(grid_data->item_id);
//			if (NULL != talent_cfg)
//			{
//				maxhp_zizhi += talent_cfg->maxhp_zizhi;
//				gongji_zizhi += talent_cfg->gongji_zizhi;
//				fangyu_zizhi += talent_cfg->fangyu_zizhi;
//			}
//
//			const JingLingWuXingCfg *wuxing_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingWuXingCfg(equip_param->param1);
//			if (NULL != wuxing_cfg)
//			{
//				maxhp_zizhi += wuxing_cfg->maxhp_zizhi;
//				gongji_zizhi += wuxing_cfg->gongji_zizhi;
//				fangyu_zizhi += wuxing_cfg->fangyu_zizhi;
//
//				add_maxhp += wuxing_cfg->maxhp;
//				add_gongji += wuxing_cfg->gongji;
//				add_fangyu += wuxing_cfg->fangyu;
//			}
//
//			//base
//			add_maxhp += (Attribute)(item_cfg->maxhp + item_cfg->maxhp * (maxhp_zizhi * ROLE_ATTR_PER));
//			add_gongji += (Attribute)(item_cfg->gongji + item_cfg->gongji * (gongji_zizhi * ROLE_ATTR_PER));
//			add_fangyu += (Attribute)(item_cfg->fangyu + item_cfg->fangyu * (fangyu_zizhi * ROLE_ATTR_PER));
//
//			// 技能
//			for (int k = 0; k < JING_LING_SKILL_COUNT_MAX; k++)
//			{
//				const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(equip_param->jing_ling_skill_list[k].skill_id);
//				if (NULL != skill_cfg)
//				{
//					skill_maxhp += skill_cfg->maxhp;
//					skill_gongji += skill_cfg->gongji;
//					skill_fangyu += skill_cfg->fangyu;
//					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += skill_cfg->constant_zengshang;
//					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += skill_cfg->constant_mianshang;
//
//					maxhp_per += skill_cfg->maxhp_per;
//					gongji_per += skill_cfg->gongji_per;
//					fangyu_per += skill_cfg->fangyu_per;
//				}
//			}
//		}
//	}
//
//	add_maxhp += (Attribute)(skill_maxhp + add_maxhp * (maxhp_per * EQUIP_ATTR_PER));
//	add_gongji += (Attribute)(skill_gongji + add_gongji * (gongji_per * EQUIP_ATTR_PER));
//	add_fangyu += (Attribute)(skill_fangyu + add_fangyu * (fangyu_per * EQUIP_ATTR_PER));
//
//	int convert_rate_maxhp = 0, convert_rate_gongji = 0, convert_rate_fangyu = 0;
//	Attribute xianzhen_maxhp = 0, xianzhen_gongji = 0, xianzhen_fangyu = 0;
//
//	// 仙阵
//	const JingLingXianZhenCfg *xianzhen_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingXianZhenCfg(m_xianzhen_level);
//	if (NULL != xianzhen_cfg)
//	{
//		xianzhen_maxhp = xianzhen_cfg->maxhp;
//		xianzhen_gongji = xianzhen_cfg->gongji;
//		xianzhen_fangyu = xianzhen_cfg->fangyu;
//
//		convert_rate_maxhp = convert_rate_gongji = convert_rate_fangyu = xianzhen_cfg->convert_rate;
//	}
//
//	// 魂玉
//	for (int i = 0; i < XIAN_ZHEN_HUN_YU_TYPE_MAX; i++)
//	{
//		const JingLingXianZhenHunYuCfg *hunyu_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingXianZhenHunYuCfg(i, m_hunyu_level_list[i]);
//		if (NULL != hunyu_cfg)
//		{
//			if (XIAN_ZHEN_HUN_YU_TYPE_MAXHP == i) convert_rate_maxhp += hunyu_cfg->convert_rate;
//			else if (XIAN_ZHEN_HUN_YU_TYPE_GONGJI == i) convert_rate_gongji += hunyu_cfg->convert_rate;
//			else if (XIAN_ZHEN_HUN_YU_TYPE_FANGYU == i) convert_rate_fangyu += hunyu_cfg->convert_rate;
//		}
//	}
//
//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += xianzhen_maxhp + (Attribute)(add_maxhp * (convert_rate_maxhp * EQUIP_ATTR_PER));
//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += xianzhen_gongji + (Attribute)(add_gongji * (convert_rate_gongji * EQUIP_ATTR_PER));
//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += xianzhen_fangyu + (Attribute)(add_fangyu * (convert_rate_fangyu * EQUIP_ATTR_PER));
//}

//void JingLingManager::ReCalcNoFightOutJingLingAttr()
//{
//	Attribute add_maxhp = 0, add_gongji = 0, add_fangyu = 0;
//	Attribute skill_maxhp = 0, skill_gongji = 0, skill_fangyu = 0;
//	Attribute maxhp_per = 0, gongji_per = 0, fangyu_per = 0;
//
//	for (int jing_ling_index = 0; jing_ling_index < JINGLING_MAX_TAKEON_NUM; jing_ling_index++)		//遍历未出战仙宠
//	{
//		if (jing_ling_index == m_use_jingling_index) continue;		//仙宠已使用
//
//		const ItemGridData *grid_data = &m_jingling_list[jing_ling_index];		
//
//		if (!grid_data->Invalid())
//		{
//			const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
//			if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() || Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())		
//			{
//				continue;
//			}
//
//			EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
//			const JingLingItemCfg *item_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingItemCfg(grid_data->item_id, equip_param->strengthen_level);		
//			if (NULL == item_cfg)
//			{
//				continue;
//			}
//
//			//int maxhp_zizhi = 0, gongji_zizhi = 0, fangyu_zizhi = 0;		//临时增加天赋资质值，最后通过转换率统一计算到已使用的仙宠上
//			float maxhp_zizhi = 0, gongji_zizhi = 0, fangyu_zizhi = 0;
//
//																			//天赋的资质属性加成
//			const JingLingTalentAttrItemCfg *talent_cfg = LOGIC_CONFIG->GetJingLingCfg().GetTalentAttrCfg(grid_data->item_id);		//根据仙宠id取得对应加成的资质值
//			if (NULL != talent_cfg)
//			{
//				maxhp_zizhi += (talent_cfg->maxhp_zizhi) * Jingling_ATTR_PER;
//				gongji_zizhi += (talent_cfg->gongji_zizhi) * Jingling_ATTR_PER;
//				fangyu_zizhi += (talent_cfg->fangyu_zizhi) * Jingling_ATTR_PER;
//			}
//
//			//talent	天赋的其他属性加成
//			for (int k = 0; k < ItemNamespace::MAX_XIANPIN_NUM; ++k)
//			{
//				int talent_type = equip_param->xianpin_type_list[k];
//				int talent_value = LOGIC_CONFIG->GetJingLingCfg().GetTalentValue(grid_data->item_id, talent_type);
//
//				if (talent_value > 0)
//				{
//					switch (talent_type)
//					{
//					case JL_TALENT_GONGJI:
//						m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (talent_value * EQUIP_ATTR_PER) * Jingling_ATTR_PER;
//						break;
//
//					case JL_TALENT_FANGYU:
//						m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (talent_value * EQUIP_ATTR_PER) * Jingling_ATTR_PER;
//						break;
//
//					case JL_TALENT_MAXHP:
//						m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (talent_value * EQUIP_ATTR_PER) * Jingling_ATTR_PER;
//						break;
//
//					case JL_TALENT_MINGZHONG:
//						m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (talent_value * EQUIP_ATTR_PER) * Jingling_ATTR_PER;
//						break;
//
//					case JL_TALENT_SHANGBI:
//						m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (talent_value * EQUIP_ATTR_PER) * Jingling_ATTR_PER;
//						break;
//
//					case JL_TALENT_BAOJI:
//						m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (talent_value * EQUIP_ATTR_PER) * Jingling_ATTR_PER;
//						break;
//
//					case JL_TALENT_JIANREN:
//						m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (talent_value * EQUIP_ATTR_PER) * Jingling_ATTR_PER;
//						break;
//
//					case JL_TALENT_JINGZHUN_PER:
//						m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += (talent_value * EQUIP_ATTR_PER) * Jingling_ATTR_PER;
//						break;
//
//					case JL_TALENT_BAOJI_PER:
//						m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += (talent_value * EQUIP_ATTR_PER) * Jingling_ATTR_PER;
//						break;
//
//					case JL_TALENT_POFANG_PER:
//						m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += (talent_value * EQUIP_ATTR_PER) * Jingling_ATTR_PER;
//						break;
//
//					case JL_TALENT_MIANSHANG_PER:
//						m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += (talent_value * EQUIP_ATTR_PER) * Jingling_ATTR_PER;
//						break;
//					}
//				}
//			}
//
//			//悟性加成
//			const JingLingWuXingCfg *wuxing_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingWuXingCfg(equip->GetColor(), equip_param->param1);
//			if (NULL != wuxing_cfg)
//			{
//				// 精灵悟性加成 -- 对全身属性进行加成 千分比
//				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(wuxing_cfg->maxhp_zizhi * ROLE_ATTR_PER);
//				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(wuxing_cfg->gongji_zizhi * ROLE_ATTR_PER);
//				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<float>(wuxing_cfg->fangyu_zizhi * ROLE_ATTR_PER);
//
//				add_maxhp += (Attribute)((wuxing_cfg->maxhp) * Jingling_ATTR_PER);
//				add_gongji += (Attribute)((wuxing_cfg->gongji) * Jingling_ATTR_PER);
//				add_fangyu += (Attribute)((wuxing_cfg->fangyu) * Jingling_ATTR_PER);
//			}
//
//			//base  ，仙宠基础属性
//			add_maxhp += (Attribute)((item_cfg->maxhp + item_cfg->maxhp * (maxhp_zizhi * ROLE_ATTR_PER)) * Jingling_ATTR_PER);
//			add_gongji += (Attribute)((item_cfg->gongji + item_cfg->gongji * (gongji_zizhi * ROLE_ATTR_PER)) * Jingling_ATTR_PER);
//			add_fangyu += (Attribute)((item_cfg->fangyu + item_cfg->fangyu * (fangyu_zizhi * ROLE_ATTR_PER)) * Jingling_ATTR_PER);
//
//
//
//			// 技能
//			for (int k = 0; k < JING_LING_SKILL_COUNT_MAX; k++)
//			{
//				const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(equip_param->jing_ling_skill_list[k].skill_id);
//				if (NULL != skill_cfg)
//				{
//					skill_maxhp += skill_cfg->maxhp;
//					skill_gongji += skill_cfg->gongji;
//					skill_fangyu += skill_cfg->fangyu;
//					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += skill_cfg->mingzhong;
//					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += skill_cfg->shanbi;
//					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += skill_cfg->baoji;
//					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += skill_cfg->kangbao;
//					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += skill_cfg->constant_zengshang;
//					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += skill_cfg->constant_mianshang;
//
//					maxhp_per += skill_cfg->maxhp_per;		//最后按比率计算实际增加的属性
//					gongji_per += skill_cfg->gongji_per;
//					fangyu_per += skill_cfg->fangyu_per;
//
//				}
//			}
//		}
//	}
//
//
//	add_maxhp += (Attribute)(skill_maxhp + (add_maxhp * (maxhp_per * EQUIP_ATTR_PER))* Jingling_ATTR_PER);			//根据一定比例计算出未出战仙宠总加成值
//	add_gongji += (Attribute)(skill_gongji + (add_gongji * (gongji_per * EQUIP_ATTR_PER))* Jingling_ATTR_PER);
//	add_fangyu += (Attribute)(skill_fangyu + (add_fangyu * (fangyu_per * EQUIP_ATTR_PER))* Jingling_ATTR_PER);
//																								
//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(add_maxhp);
//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(add_gongji);
//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(add_fangyu);
//
//}

void JingLingManager::ReCalcNoFightOutJingLingAttr(int &other_capability)
{
	//Attribute add_maxhp = 0, add_gongji = 0, add_fangyu = 0;
	//Attribute skill_maxhp = 0, skill_gongji = 0, skill_fangyu = 0;
	//Attribute maxhp_per = 0, gongji_per = 0, fangyu_per = 0;

	for (int jing_ling_index = 0; jing_ling_index < JINGLING_MAX_TAKEON_NUM; jing_ling_index++)		//遍历未出战仙宠
	{
		if (jing_ling_index == m_use_jingling_index) continue;		//仙宠已使用

		const ItemGridData *grid_data = &m_jingling_list[jing_ling_index];

		if (!grid_data->Invalid())
		{
			const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
			if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() || Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
			{
				continue;
			}

			EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
			const JingLingItemCfg *item_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingItemCfg(grid_data->item_id, equip_param->strengthen_level);
			if (NULL == item_cfg)
			{
				continue;
			}

			int maxhp_zizhi = 0, gongji_zizhi = 0, fangyu_zizhi = 0;

			const JingLingTalentAttrItemCfg *talent_cfg = LOGIC_CONFIG->GetJingLingCfg().GetTalentAttrCfg(grid_data->item_id);
			if (NULL != talent_cfg)
			{
				maxhp_zizhi += talent_cfg->maxhp_zizhi;
				gongji_zizhi += talent_cfg->gongji_zizhi;
				fangyu_zizhi += talent_cfg->fangyu_zizhi;
			}

			// 单个精灵增加的属性集
			static CharIntAttrs single_jingling_attr_add; single_jingling_attr_add.Reset();

			const JingLingWuXingCfg *wuxing_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingWuXingCfg(equip->GetColor(), equip_param->param1);
			if (NULL != wuxing_cfg)
			{
				// 精灵悟性加成 -- 对全身属性进行加成
				single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(wuxing_cfg->maxhp_zizhi * ROLE_ATTR_PER);
				single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(wuxing_cfg->gongji_zizhi * ROLE_ATTR_PER);
				single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<float>(wuxing_cfg->fangyu_zizhi * ROLE_ATTR_PER);

				single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += wuxing_cfg->maxhp;
				single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += wuxing_cfg->gongji;
				single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += wuxing_cfg->fangyu;
			}

			//base
			single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(item_cfg->maxhp + item_cfg->maxhp * (maxhp_zizhi * ROLE_ATTR_PER));
			single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(item_cfg->gongji + item_cfg->gongji * (gongji_zizhi * ROLE_ATTR_PER));
			single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(item_cfg->fangyu + item_cfg->fangyu * (fangyu_zizhi * ROLE_ATTR_PER));

			//talent
			for (int k = 0; k < ItemNamespace::MAX_XIANPIN_NUM; ++k)
			{
				int talent_type = equip_param->xianpin_type_list[k];
				int talent_value = LOGIC_CONFIG->GetJingLingCfg().GetTalentValue(grid_data->item_id, talent_type);

				if (talent_value > 0)
				{
					switch (talent_type)
					{
					case JL_TALENT_GONGJI:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_FANGYU:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_MAXHP:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_MINGZHONG:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_SHANGBI:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_BAOJI:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_JIANREN:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_JINGZHUN_PER:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_BAOJI_PER:
						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += talent_value * EQUIP_ATTR_PER;
						break;

					case JL_TALENT_POFANG_PER:
//						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += talent_value * EQUIP_ATTR_PER;
						single_jingling_attr_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, talent_value);
						break;

					case JL_TALENT_MIANSHANG_PER:
//						single_jingling_attr_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += talent_value * EQUIP_ATTR_PER;
						single_jingling_attr_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, talent_value);
						break;
					}
				}
			}

			// 技能
			Attribute maxhp_per = 0, gongji_per = 0, fangyu_per = 0;
			m_skill_lei_ting_id = 0;
			m_skill_xi_xue_id = 0;
			m_skill_zhi_ming_id = 0;
			m_skill_fan_tan_id = 0;

			for (int k = 0; k < JING_LING_SKILL_COUNT_MAX; k++)
			{
				const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(equip_param->jing_ling_skill_list[k].skill_id);
				if (NULL != skill_cfg)
				{
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += skill_cfg->maxhp;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += skill_cfg->gongji;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += skill_cfg->fangyu;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += skill_cfg->mingzhong;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += skill_cfg->shanbi;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += skill_cfg->baoji;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += skill_cfg->kangbao;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += skill_cfg->constant_zengshang;
					single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += skill_cfg->constant_mianshang;

					maxhp_per += skill_cfg->maxhp_per;
					gongji_per += skill_cfg->gongji_per;
					fangyu_per += skill_cfg->fangyu_per;

					//other_capability += skill_cfg->capability;		// 出战宠物才有额外战力加成

					if (SKILL_LEI_TING_TYPE == skill_cfg->skill_type)
					{
						m_skill_lei_ting_id = skill_cfg->skill_id;
					}
					else if (SKILL_XI_XUE_TYPE == skill_cfg->skill_type)
					{
						m_skill_xi_xue_id = skill_cfg->skill_id;
					}
					else if (SKILL_ZHI_MING_TYPE == skill_cfg->skill_type)
					{
						m_skill_zhi_ming_id = skill_cfg->skill_id;
					}
					else if (SKILL_FAN_TAN_TYPE == skill_cfg->skill_type)
					{
						m_skill_fan_tan_id = skill_cfg->skill_id;
					}
				}
			}

			single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (maxhp_per * ROLE_ATTR_PER));
			single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (gongji_per * ROLE_ATTR_PER));
			single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(single_jingling_attr_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (fangyu_per * ROLE_ATTR_PER));
		
			// 将该精灵的属性加入到玩家属性
			m_attrs_add.Add(single_jingling_attr_add.m_attrs);
			m_attrs_add.AddPercent(single_jingling_attr_add.m_percent_attrs);
		}
	}

	//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (maxhp_per * EQUIP_ATTR_PER));
	//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (gongji_per * EQUIP_ATTR_PER));
	//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (fangyu_per * EQUIP_ATTR_PER));
}

void JingLingManager::ReCalcGroupAttr()
{
	for (int i = 0; i < m_group_num && i < JINGLING_MAX_GROUP_IN_ONECE; ++ i)
	{
		if (m_groupid_list[i] > 0)
		{
			const JingLingGroupCfg *group_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingGroupCfg(m_groupid_list[i]);
			if (NULL != group_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += group_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += group_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += group_cfg->maxhp;
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += group_cfg->mianshang_per;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += group_cfg->jingzhun_per;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += group_cfg->baoji_per;

				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, group_cfg->mianshang_per);

				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += group_cfg->gongji_per * SKILL_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += group_cfg->maxhp_per * SKILL_ATTR_PER;
			}
		}
	}
}

void JingLingManager::ReCalcCardAttr()
{
	static CharIntAttrs jingling_card_attrs_add;
	jingling_card_attrs_add.Reset();

	for (int i = 0; i < JINGLING_CARD_MAX_TYPE; ++ i)
	{
		JingLingCardParamItem &card_item = m_card_list[i];
		
		const JingLingCardCfg *card_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingCardCfg(i, card_item.level);
		if (NULL == card_cfg)
		{
			continue;
		}

		for (int k = 0; k < JINGLING_CARD_ATTR_COUNT; ++ k)
		{
			JingLingCardCfg::AttrItem attr_item = card_cfg->attr_list[k];
			
			if (CharIntAttrs::IsRoleBaseAttr(attr_item.attr_type) && attr_item.value > 0)
			{
//				jingling_card_attrs_add.m_attrs[attr_item.attr_type] += GETPROFVALUE(m_role->GetProf(), attr_item.attr_type, attr_item.value);
				jingling_card_attrs_add.AddValue(attr_item.attr_type, GETPROFVALUE(m_role->GetProf(), attr_item.attr_type, attr_item.value));
			}
		}
	}

	m_attrs_add.Add(jingling_card_attrs_add.m_attrs);
	m_attrs_add.AddPercent(jingling_card_attrs_add.m_percent_attrs);
}

void JingLingManager::ReCalcSpecialJinglingAttr()
{
	int add_value = m_role->GetRoleBigSmallGoal()->GetRoleBigSmallGoalAddAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_XIANCHONG);
	if (add_value > 0)
	{
		for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++i)
		{
			if (CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED == i) continue;

			//m_attrs_add.m_attrs[i] += static_cast<Attribute>(m_attrs_add.m_attrs[i] * (ROLE_ATTR_PER * add_value));
			m_attrs_add.AddValue(i, static_cast<Attribute>(m_attrs_add.m_attrs[i] * (ROLE_ATTR_PER * add_value)));
		}
	}
}

void JingLingManager::ReCalcShuXinDanAttr()
{
	for (int i = 0; i < JINGLING_SHUXINGDAN_MAX_TYPE; ++ i)
	{
		const int num = m_shuxingdan_list[i];
		if (num > 0)
		{
			const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_JINGLING, i);

			if (NULL != cfg_item)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp * num;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji * num;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu * num;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong * num;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi * num;
			}
		}
	}
}

void JingLingManager::ReCalcGradeAttr()
{
	const JingLingUpGradeCfg *upgrade_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingUpGradeCfg(m_grade);
	
	if (NULL != upgrade_cfg)
	{
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, upgrade_cfg->maxhp);
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, upgrade_cfg->gongji);
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, upgrade_cfg->fangyu);
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, upgrade_cfg->mingzhong);
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, upgrade_cfg->shanbi);
	}
}

void JingLingManager::ReCalcEquipAttr()
{
	for (int i = 0; i < JINGLING_EQUIP_MAX_PART; ++ i)
	{
		const int level = m_equip_strength_list[i];
		const JingLingEquipStrengthCfg *cfg_item = LOGIC_CONFIG->GetJingLingCfg().GetJingLingEquipStrengthCfg(i, level);

		if (NULL != cfg_item)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, cfg_item->maxhp);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, cfg_item->gongji);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, cfg_item->fangyu);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, cfg_item->mingzhong);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, cfg_item->shanbi);
		}
	}
}

void JingLingManager::ReCalcPhantomAttr()
{
	for (int i = 0; i < JINGLING_PTHANTOM_MAX_TYPE; ++ i)
	{
		int level = m_phantom_level_list[i];

		const JingLingPhantomCfg *phantom_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingPhantomCfg(i, level);
		if (NULL == phantom_cfg)
		{
			continue;
		}
		
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, phantom_cfg->maxhp);
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, phantom_cfg->gongji);
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, phantom_cfg->fangyu);
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, phantom_cfg->mingzhong);
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, phantom_cfg->shanbi);
		this->HuanHuaSpecialCapAdd(i, level, m_attrs_add);
	}
}

void JingLingManager::ReCalcSoulAttr()
{
	for (int i = 0; i < JINGLING_CARD_MAX_TYPE; ++ i)
	{
		const int level = m_soul_level_list[i];

		const JingLingSoulCfg *soul_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingSoulcfg(i, level);
		if (NULL == soul_cfg)
		{
			continue;
		}

		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, soul_cfg->maxhp);
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, soul_cfg->gongji);
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, soul_cfg->fangyu);
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, soul_cfg->mingzhong);
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, soul_cfg->shanbi);
	}
}

void JingLingManager::ReCalcSoulTaoZhuangAttr()
{
	if (m_soul_taozhuang_level > 0)
	{
		const JingLingSoulTaoZhuangCfg *taozhuang_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSoulTaoZhuangCfg(m_soul_taozhuang_level);
		if (NULL != taozhuang_cfg && taozhuang_cfg->need_min_level == m_soul_taozhuang_level)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, taozhuang_cfg->maxhp);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, taozhuang_cfg->gongji);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, taozhuang_cfg->fangyu);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, taozhuang_cfg->mingzhong);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, taozhuang_cfg->shanbi);
		}
	}
}

void JingLingManager::ReCalcJingLingHaloAttr()
{
	{
		// 光环
		const JingLingHaloUplevelCfg *cfg_item = LOGIC_CONFIG->GetJingLingCfg().GetHaloUplevelCfg(m_halo_level);

		if (NULL != cfg_item)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi;
		}
	}
}

const ItemGridData *JingLingManager::GetJingLingData(short jingling_index)
{
	if (jingling_index < 0 || jingling_index >= JINGLING_MAX_TAKEON_NUM
		|| m_jingling_list[jingling_index].Invalid())
	{
		return NULL;
	}

	return &m_jingling_list[jingling_index];
}

int JingLingManager::CalcJingLingCapability(const ItemGridData *grid_data)
{
	if (grid_data == nullptr || grid_data->Invalid())
	{
		return 0;
	}
	
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() || Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
	{
		return 0;
	}

	EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
	const JingLingItemCfg *item_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingItemCfg(grid_data->item_id, equip_param->strengthen_level);
	if (NULL == item_cfg)
	{
		return 0;
	}

	int maxhp_zizhi = 0, gongji_zizhi = 0, fangyu_zizhi = 0;

	const JingLingTalentAttrItemCfg *talent_cfg = LOGIC_CONFIG->GetJingLingCfg().GetTalentAttrCfg(grid_data->item_id);
	if (NULL != talent_cfg)
	{
		maxhp_zizhi += talent_cfg->maxhp_zizhi;
		gongji_zizhi += talent_cfg->gongji_zizhi;
		fangyu_zizhi += talent_cfg->fangyu_zizhi;
	}

	CharIntAttrs attrs_add;

	const JingLingWuXingCfg *wuxing_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingWuXingCfg(equip->GetColor(), equip_param->param1);
	if (NULL != wuxing_cfg)
	{
		// 精灵悟性加成 -- 对全身属性进行加成 千分比
		attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(wuxing_cfg->maxhp_zizhi * ROLE_ATTR_PER);
		attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(wuxing_cfg->gongji_zizhi * ROLE_ATTR_PER);
		attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<float>(wuxing_cfg->fangyu_zizhi * ROLE_ATTR_PER);

		attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += wuxing_cfg->maxhp;
		attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += wuxing_cfg->gongji;
		attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += wuxing_cfg->fangyu;
	}

	//base
	attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(item_cfg->maxhp + item_cfg->maxhp * (maxhp_zizhi * ROLE_ATTR_PER));
	attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(item_cfg->gongji + item_cfg->gongji * (gongji_zizhi * ROLE_ATTR_PER));
	attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(item_cfg->fangyu + item_cfg->fangyu * (fangyu_zizhi * ROLE_ATTR_PER));

	// 技能
	Attribute maxhp_per = 0, gongji_per = 0, fangyu_per = 0;
	int other_capability = 0;

	for (int i = 0; i < JING_LING_SKILL_COUNT_MAX; i++)
	{
		const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(equip_param->jing_ling_skill_list[i].skill_id);
		if (NULL != skill_cfg)
		{
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += skill_cfg->maxhp;
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += skill_cfg->gongji;
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += skill_cfg->fangyu;
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += skill_cfg->mingzhong;
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += skill_cfg->shanbi;
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += skill_cfg->baoji;
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += skill_cfg->kangbao;
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += skill_cfg->constant_zengshang;
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += skill_cfg->constant_mianshang;

			maxhp_per += skill_cfg->maxhp_per;
			gongji_per += skill_cfg->gongji_per;
			fangyu_per += skill_cfg->fangyu_per;
			other_capability += skill_cfg->capability;
		}
	}

	attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (maxhp_per * ROLE_ATTR_PER));
	attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (gongji_per * ROLE_ATTR_PER));
	attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (fangyu_per * ROLE_ATTR_PER));

	return Capability::GetCapByAttr(attrs_add) + other_capability;
}

void JingLingManager::OnDayChange(int old_dayid, int now_dayid)
{
	if (0 != m_explore_maxhp)
	{
		m_explore_maxhp = 0;
		m_explore_hp = 0;
		m_explore_mode = 0;
		m_explore_buy_buff_count = 0;
		memset(m_explore_info_list, 0, sizeof(m_explore_info_list));
		this->SendExploreInfo(Protocol::JL_EXPLORE_INFO_REASON_RESET);
	}

	this->CheckFuncOpen();
}

void JingLingManager::OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag)
{
	if (NULL == target)
	{
		return;
	}

	Scene *scene = m_role->GetScene();
	if (NULL == scene)
		return;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	// 有param_1概率召唤雷电，对目标及周围共param_2个目标造成伤害。低级固定值param_3，高级万分比param_4+固定值param_3
	if (m_skill_lei_ting_id > 0)
	{
		const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(m_skill_lei_ting_id);
		if (NULL != skill_cfg)
		{
			if (now_sec >= m_skill_last_perform_time_lei_ting + skill_cfg->cooldown)
			{
				if (skill_cfg->param_1 > 0 && RandomNum(10000) < skill_cfg->param_1)
				{
					m_skill_last_perform_time_lei_ting = now_sec;

					static const int RANGE = 10;
					static const int MAX_COUNT_OBJ = 32;
					static ObjID targer_list[MAX_COUNT_OBJ] = { INVALID_OBJ_ID };
					int count = scene->GetZoneMatrix()->GetObjByArea(target->GetPos(), RANGE, RANGE, targer_list, sizeof(targer_list) / sizeof(targer_list[0]));

					Attribute hurt_injure = (Attribute)skill_cfg->param_3;
					if (skill_cfg->param_4 > 0)
					{
						hurt_injure += (Attribute)(hurt_injure * (skill_cfg->param_4 * SKILL_ATTR_PER));
					}

					// 伤害
					int taret_num = 0;
					for (int i = 0; i < count && i < MAX_COUNT_OBJ && taret_num < skill_cfg->param_2; i++)
					{
						Obj *tmp_target_obj = scene->GetObj(targer_list[i]);
						if (NULL == tmp_target_obj || Obj::OBJ_TYPE_ROLE != tmp_target_obj->GetObjType()) continue;
						if (!m_role->IsEnemy(tmp_target_obj)) continue;

						Character *tmp_target = (Character*)tmp_target_obj;
						EffectOnce *effect = new EffectOnce(m_role->GetId(), 0, -hurt_injure, EffectBase::FIGHT_TYPE_NORMAL);
						tmp_target->AddEffect(effect);

						++taret_num;
					}
				}
			}
		}
	}

	// 有param_1概率使主人获得吸血效果，攻击时恢复自身生命值，持续param_2秒。低级恢复固定值param_3，高级恢复造成伤害的万分比param_4+固定值param_3
	if (m_skill_xi_xue_id > 0 && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		bool can_effect = false;
		const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(m_skill_xi_xue_id);
		if (NULL != skill_cfg)
		{
			if (now_sec >= m_skill_last_perform_time_xixue + skill_cfg->cooldown)
			{
				if (skill_cfg->param_1 > 0 && RandomNum(10000) < skill_cfg->param_1)
				{
					can_effect = true;
					m_skill_last_perform_time_xixue = now_sec;
				}
			}
			else
			{
				// 冷却中，判断是否在持续时间
				if (now_sec <= m_skill_last_perform_time_xixue + skill_cfg->param_2)
				{
					can_effect = true;
				}
			}

			if (can_effect)  // 生效
			{
				passive_flag |= (1 << EffectBase::PASSIVE_FLAG_JING_LING_XI_XUE);

				Attribute add_hp = (Attribute)skill_cfg->param_3;
				if (skill_cfg->param_4 > 0)
				{
					add_hp += (Attribute)(base_injure * (skill_cfg->param_4 * SKILL_ATTR_PER));
				}

				if (add_hp > 0)
				{
					m_role->ChangeHp(&add_hp, true);

					static Protocol::SCFightSpecialFloat pro;
					pro.obj_id = m_role->GetId();
					pro.deliver_obj_id = m_role->GetId();
					pro.float_type = Protocol::SCFightSpecialFloat::EFFECT_RESTORE_HP;
					pro.float_value = add_hp;
					pro.skill_special_effect = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_NON;
					m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), &pro, sizeof(pro));
				}
			}
		}
	}
}

void JingLingManager::OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	if (delta_hp >= 0)
	{
		return;
	}

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	// 有param_1概率使主人获得反弹效果，被攻击时反弹伤害给攻击者，持续param_2秒。低级反弹固定值param_3，高级反弹受到伤害的万分比param_4+固定值param_3
	if (m_skill_fan_tan_id > 0)
	{
		bool can_effect = false;
		const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(m_skill_fan_tan_id);
		if (NULL != skill_cfg)
		{
			if (now_sec >= m_skill_last_perform_time_fan_tan + skill_cfg->cooldown)
			{
				if (skill_cfg->param_1 > 0 && RandomNum(10000) < skill_cfg->param_1)
				{
					can_effect = true;
					m_skill_last_perform_time_fan_tan = now_sec;
				}
			}
			else
			{
				// 冷却中，判断是否在持续时间
				if (now_sec <= m_skill_last_perform_time_fan_tan + skill_cfg->param_2)
				{
					can_effect = true;
				}
			}

			if (can_effect)  // 生效
			{
				Attribute hurt_hp = (Attribute)(delta_hp * (skill_cfg->param_4 * SKILL_ATTR_PER));
				hurt_hp -= (Attribute)skill_cfg->param_3;

				if (hurt_hp < 0)
				{
					Character *atk_cha = (Character *)m_role->GetScene()->GetObj(attacker);
					if (NULL != atk_cha && Obj::IsCharactor(atk_cha->GetObjType()) && from_skill)
					{
						EffectOnce *effect = new EffectOnce(m_role->GetId(), 0, hurt_hp, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_REBOUNDHURT);
						atk_cha->AddEffect(effect);

						static Protocol::SCFightSpecialFloat pro;
						pro.obj_id = attacker;
						pro.deliver_obj_id = m_role->GetId();
						pro.float_type = Protocol::SCFightSpecialFloat::EFFECT_REBOUNDHURT;
						pro.float_value = -hurt_hp;
						pro.skill_special_effect = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_JINGLING_REBOUNDHURT;
						atk_cha->GetScene()->GetZoneMatrix()->NotifyAreaMsg(atk_cha->GetObserHandle(), &pro, sizeof(pro));
					}
				}
			}
		}
	}

	// 有param_1概率使主人获得减伤效果，受到的伤害降低，持续param_2秒。低级固定值param_3，高级万分比param_4+固定值param_3
	if (m_skill_zhi_ming_id > 0)
	{
		bool can_effect = false;
		const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(m_skill_zhi_ming_id);
		if (NULL != skill_cfg)
		{
			if (now_sec >= m_skill_last_perform_time_zhi_ming + skill_cfg->cooldown)
			{
				if (skill_cfg->param_1 > 0 && RandomNum(10000) < skill_cfg->param_1)
				{
					can_effect = true;
					m_skill_last_perform_time_zhi_ming = now_sec;
				}
			}
			else
			{
				// 冷却中，判断是否在持续时间
				if (now_sec <= m_skill_last_perform_time_zhi_ming + skill_cfg->param_2)
				{
					can_effect = true;
				}
			}

			if (can_effect)  // 生效
			{
				Attribute hurt_hp = (Attribute)(delta_hp * (skill_cfg->param_4 * SKILL_ATTR_PER));
				hurt_hp -= (Attribute)skill_cfg->param_3;

				if (hurt_hp < 0)
				{
					delta_hp -= hurt_hp;

					//Character *atk_cha = (Character *)m_role->GetScene()->GetObj(attacker);
					//if (NULL != atk_cha && Obj::IsCharactor(atk_cha->GetObjType()))
					//{
						//static Protocol::SCFightSpecialFloat pro;
						//pro.obj_id = attacker;
						//pro.deliver_obj_id = m_role->GetId();
						//pro.float_type = Protocol::SCFightSpecialFloat::EFFECT_REBOUNDHURT;
						//pro.float_value = -hurt_hp;
						//pro.skill_special_effect = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_JINGLING_REBOUNDHURT;
						//atk_cha->GetScene()->GetZoneMatrix()->NotifyAreaMsg(atk_cha->GetObserHandle(), &pro, sizeof(pro));
					//}
				}
			}
		}
	}
}

bool JingLingManager::IsBaoJi()
{
	//if (m_skill_zhi_ming_id > 0)
	//{
	//	const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(m_skill_zhi_ming_id);
	//	return (NULL != skill_cfg && skill_cfg->param_1 > 0 && RandomNum(10000) < skill_cfg->param_1);
	//}

	return false;
}

int JingLingManager::GetJingLingMaxLevel()
{
	int max_strengthen_level = 0;
	for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; ++ i)
	{
		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_jingling_list[i].item_id);
		if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() || Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
		{
			continue;
		}
		
		EquipmentParam *equip_param = (EquipmentParam *)m_jingling_list[i].param;
		max_strengthen_level = equip_param->strengthen_level > max_strengthen_level ? equip_param->strengthen_level : max_strengthen_level;
	}
	
	return max_strengthen_level;
}

void JingLingManager::GetUsedJingLingInfo(ItemID *jingling_id, short *level, GameName jingling_name, char *use_imageid, short *phantom_imgageid)
{
	if (m_use_jingling_id <= 0 || m_use_jingling_index < 0 || m_use_jingling_index >= JINGLING_MAX_TAKEON_NUM)
	{
		if (NULL != jingling_id) *jingling_id = 0;
		if (NULL != level) *level = 0;
		if (NULL != use_imageid) *use_imageid = 0;
		if (NULL != phantom_imgageid)*phantom_imgageid = 0;
		return;
	}

	if (NULL != jingling_id) *jingling_id = m_use_jingling_id;
	if (NULL != level) *level = this->GetJingLingLevelByIndex(m_use_jingling_index);
	if (NULL != jingling_name)
	{
		F_STRNCPY(jingling_name, m_jingling_name, sizeof(GameName));
	}
	if (NULL != use_imageid) *use_imageid = static_cast<char>(m_used_imageid);
	if (NULL != phantom_imgageid) *phantom_imgageid = m_phantom_imgageid;
}

int JingLingManager::GetJingLingLevelByIndex(short index)
{
	if (index < 0 || index >= JINGLING_MAX_TAKEON_NUM)
	{
		return 0;
	}

	ItemGridData *grid_data = &m_jingling_list[index];
	if (grid_data->Invalid())
	{
		return 0;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() || Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
	{
		return 0;
	}

	EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;

	return equip_param->strengthen_level;
}

short JingLingManager::GetJingLingIndexByItemId(ItemID item_id)
{
	for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; ++ i)
	{
		if (!m_jingling_list[i].Invalid() && item_id == m_jingling_list[i].item_id)
		{
			return i;
		}
	}

	return -1;
}

short JingLingManager::GetFirstEmptyGridIndex()
{
	for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; ++ i)
	{
		if (m_jingling_list[i].Invalid())
		{
			return i;
		}
	}
	
	return -1;
}

bool JingLingManager::HasSameJingLing(ItemID item_id)
{
	if (item_id <= 0)
	{
		return false;
	}

	for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; ++ i)
	{
		if (!m_jingling_list[i].Invalid() && item_id == m_jingling_list[i].item_id)
		{
			return true;
		}
	}

	return false;
}

int JingLingManager::PutOn(const ItemDataWrapper &item_wrapper, short equip_index)
{
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_wrapper.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()) return -1;

	if (0 == item_wrapper.has_param) return -2;

	if (Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType()) return -3;
	

	//if (this->HasSameJingLing(item_wrapper.item_id))
	//{
	//	m_role->NoticeNum(errornum::EN_JINGLING_SAME_TYPE);
	//	return -5;
	//}

	if (equip_index < 0 || equip_index >= JINGLING_MAX_TAKEON_NUM)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_NOT_EMPTY_GRID);
		return -4;
	}

	for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; i++)
	{
		if (m_jingling_list[i].Invalid())
		{
			continue;
		}
		if (i == equip_index)
		{
			continue;
		}
		if (m_jingling_list[i].item_id == item_wrapper.item_id)
		{
			m_role->NoticeNum(errornum::EN_JINGLING_SAME_TYPE);
			return -5;
		}
	}

	if (!m_jingling_list[equip_index].Invalid())
	{
		if (TakeOff(equip_index, false) != 0)
		{
			m_role->NoticeNum(errornum::EN_JINGLING_NOT_EMPTY_GRID);
			return -5;
		}
	}


	//short equip_index = this->GetFirstEmptyGridIndex();
	//if (equip_index < 0 || equip_index >= JINGLING_MAX_TAKEON_NUM)
	//{
	//	m_role->NoticeNum(errornum::EN_JINGLING_NOT_EMPTY_GRID);
	//	return -4;
	//}

	int old_jingling_count = this->GetPutOnJinglingCount(); 

	EquipmentParam *equip_param = equip->CreateParamFromStruct(&item_wrapper.param_data);
	if (NULL == equip_param) return -6;
	
	if (equip_param->strengthen_level <= 0)
	{
		equip_param->strengthen_level = 1; //用强化等级认为是精灵等级
	}

	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (NULL == other_cfg) return -7;
	for (int i = 0; i < other_cfg->skill_solt_free_count && i < JING_LING_SKILL_COUNT_MAX; ++i)   // 默认开启x个技能槽
	{
		equip_param->jing_ling_skill_list[i].is_open = 1;
	}
	
	m_jingling_list[equip_index].item_id = item_wrapper.item_id;
	m_jingling_list[equip_index].param = equip_param;
	m_jingling_list[equip_index].num = 1;
	m_jingling_list[equip_index].invalid_time = item_wrapper.invalid_time;
	//m_jingling_list[equip_index].is_bind = (item_wrapper.is_bind == 1);
	m_jingling_list[equip_index].is_bind = true;	//策划需求，装备后变为绑定物品
	// 如果替换掉的旧宠物的出战状态清掉了，则让新宠物出战
	if (0 == m_use_jingling_id)
	{
		this->FightOutJingLing(equip_index);
	}

	//屏蔽掉精灵王称号，策划说不要了，需要再开回来
	//m_role->GetTitleManager()->RecalJinglingTitle();

	this->ReCalcGroup();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);

	this->SyncJingLingInfo();

	const short color = equip->GetColor();						//品质
	const int count = this->GetPutOnJinglingCount(color);						//满足品质的数量
	m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_XIANCHONG, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_1, color, count);	//大小目标

	int new_jingling_count = this->GetPutOnJinglingCount();
	EventHandler::Instance().OnPutOnJinglingCount(m_role, old_jingling_count, new_jingling_count);
		
	EventHandler::Instance().OnJingLingPuton(m_role, equip_param->strengthen_level);

	return 0;
}

int JingLingManager::TakeOff(short equip_index, bool notice_client)
{
	if (equip_index < 0 || equip_index >= JINGLING_MAX_TAKEON_NUM) return -1;

	if (m_jingling_list[equip_index].Invalid()) return -2;

	EquipmentParam *equip_param = (EquipmentParam *)m_jingling_list[equip_index].param;
	if (NULL == equip_param) return -3;

	const Equipment *equip = (const Equipment*)ITEMPOOL->GetItem(m_jingling_list[equip_index].item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT || equip->GetEquipType() != Equipment::E_TYPE_SPECIAL_JINGLING)
	{
		return -4;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return -5;
	}

	ItemDataWrapper item_wrapper(m_jingling_list[equip_index].item_id, 1, (short)m_jingling_list[equip_index].is_bind, 
		m_jingling_list[equip_index].param, m_jingling_list[equip_index].invalid_time, 0);

	int put_index = -1;
	if (!knapsack->Put(item_wrapper, PUT_REASON_NO_NOTICE, &put_index))
	{
		return -6;
	}
	
	if (m_use_jingling_index == equip_index)	// 先召回现在的
	{
		this->CallBackJingLing(notice_client);
	}

	m_jingling_list[equip_index].Clear();		// 指针已无用 删除之


	if (notice_client)
	{
		this->ReCalcGroup();

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);

		this->SyncJingLingInfo();
	}

	JingLingHome *jing_ling_home = GLOBAL_ROLE_MGR.GeJingLingHome(m_role->GetUID());
	if (NULL != jing_ling_home)
	{
		jing_ling_home->OnTakeOffJingLing(m_role, item_wrapper.item_id);
	}

	return 0;
}

void JingLingManager::ReCalcGroup()
{
	m_group_num = 0;
	memset(m_groupid_list, 0, sizeof(m_groupid_list));
	
	LOGIC_CONFIG->GetJingLingCfg().GetJingLingGroupCfg(m_jingling_list[0].item_id, m_jingling_list[1].item_id, m_jingling_list[2].item_id, m_jingling_list[3].item_id,
		m_jingling_list[4].item_id, m_groupid_list, &m_group_num);

	//计算组合出的技能
	{
		int old_skill_id = m_group_skill_id;

		m_group_skill_id = 0;
		for (int i = 0; i < m_group_num && i < JINGLING_MAX_GROUP_IN_ONECE; ++ i)
		{
			const JingLingGroupCfg *group_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingGroupCfg(m_groupid_list[i]);
			if (NULL != group_cfg && group_cfg->skill_id > 0)
			{
				m_group_skill_id = group_cfg->skill_id;
			}
		}

		if (old_skill_id != m_group_skill_id)
		{
			if (old_skill_id != 0)
			{
				m_role->GetSkillManager()->RemoveSkill(old_skill_id);
			}
			if (m_group_skill_id != 0)
			{
				m_role->GetSkillManager()->AddJingLingSkill(m_group_skill_id, 1);
			}
		}
	}
}

void JingLingManager::UpLevel(short index)
{
	if (index < 0 || index >= JINGLING_MAX_TAKEON_NUM || m_role->GetLingJing() <= 0)
	{
		return;
	}
	
	ItemGridData *grid_data = &m_jingling_list[index];
	if (grid_data->Invalid())
	{
		return;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() || Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
	{
		return;
	}
	
	EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
	if (equip_param->strengthen_level >= JINGLING_MAX_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_MAX_LEVEL);
		return;
	}

	const JingLingItemCfg *jingling_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingItemCfg(grid_data->item_id, equip_param->strengthen_level);
	if (NULL == jingling_cfg)
	{
		return;
	}

	const JingLingItemCfg *next_jingling_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingItemCfg(grid_data->item_id, equip_param->strengthen_level + 1);
	if (NULL == next_jingling_cfg)
	{
		return;
	}

	const short old_strength_level = equip_param->strengthen_level;

	int cost_lingjing = 0;
	if (m_role->GetLingJing() + equip_param->param3 < jingling_cfg->cost_lingjing)
	{
		cost_lingjing = m_role->GetLingJing();
		equip_param->param3 += m_role->GetLingJing();
	}
	else
	{
		cost_lingjing = jingling_cfg->cost_lingjing - equip_param->param3;
		equip_param->param3 = 0;
		++equip_param->strengthen_level;
		if (equip_param->strengthen_level > m_history_max_chengzhang)
		{
			m_history_max_chengzhang = equip_param->strengthen_level;
		}
	}

	m_role->AddLingJing(-cost_lingjing, "CostByJinglingUplevel");
	grid_data->is_bind = true;

	if (equip_param->strengthen_level > old_strength_level)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);
		EventHandler::Instance().OnJingLingUpLevel(m_role, jingling_cfg->item_id, equip_param->strengthen_level);

		//传闻
		{
			const JingLingItemCfg *jingling_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingItemCfg(grid_data->item_id, equip_param->strengthen_level);
			if (NULL != jingling_cfg && jingling_cfg->is_broadcast > 0)
			{
				const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(m_role->GetUserId()), equip->GetItemId(), true, grid_data->invalid_time, equip_param, NULL);
				if (NULL != equip_param_string)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_link_jingling_uplevel,
						UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), jingling_cfg->item_id, equip_param_string, equip_param->strengthen_level);
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}

	this->SyncJingLingInfo();

	gamelog::g_log_jingling.printf(LL_INFO, "UpLevel user %d %s, cost_lingjing %d, old_level %d, new_level %d",
		m_role->GetUID(), m_role->GetName(), (int)cost_lingjing, (int)old_strength_level, (int)equip_param->strengthen_level);
}

void JingLingManager::CallBackJingLing(bool notice_client)
{
	if (0 == m_use_jingling_id || -1 == m_use_jingling_index)
	{
		return;
	}

	m_use_jingling_index = -1;
	m_use_jingling_id = 0;


	if (notice_client)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);
		this->SyncJingLingInfo();
	}
	this->BroadcastJingLingInfo();
}

void JingLingManager::FightOutJingLing(short index)
{
	if (index < 0 || index >= JINGLING_MAX_TAKEON_NUM)
	{
		return;
	}

	ItemGridData *grid_data = &m_jingling_list[index];
	if (NULL == grid_data || grid_data->Invalid())
	{
		return;
	}

	if (m_use_jingling_index == index)
	{
		return;
	}

	m_use_jingling_index = index;
	m_use_jingling_id = grid_data->item_id;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);
	this->SyncJingLingInfo();
	this->BroadcastJingLingInfo();
}

void JingLingManager::UpLevelCard(short card_type)
{
	if (card_type < 0 || card_type >= JINGLING_CARD_MAX_TYPE)
	{
		return;
	}
	
	JingLingCardParamItem *card_item = &m_card_list[card_type];
	const JingLingCardCfg *card_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingCardCfg(card_type, card_item->level + 1);
	if (NULL == card_cfg)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_CARD_MAX_LEVEL);
		return;
	}

	UNSTD_STATIC_CHECK(JINGLING_CARD_STUFF_ADD_EXP > 0);
	int need_num_uplevel = (card_cfg->exp - card_item->exp) / JINGLING_CARD_STUFF_ADD_EXP;

	int knaspack_has_num = m_role->GetKnapsack()->GetItemNum(card_cfg->stuff_id);
	need_num_uplevel = knaspack_has_num > need_num_uplevel ? need_num_uplevel : knaspack_has_num;

	g_consume_list.Reset();
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[0].item_id = card_cfg->stuff_id;
	stuff_list[0].num = need_num_uplevel;
	stuff_list[0].buyable = false;

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &g_consume_list, true))
	{
		return;
	}

	{
		for (int i = 0; i < g_consume_list.count && i < ItemNamespace::MAX_KNAPSACK_GRID_NUM; i++)
		{
			int knapsack_index = g_consume_list.index_list[i];

			static ItemGridData item_data;
			item_data.Reset();

			if (!m_role->GetKnapsack()->GetItemGridData(knapsack_index, &item_data))
			{
				continue;
			}

			if (item_data.Invalid())
			{
				continue;
			}

			const ItemBase *item_base = ITEMPOOL->GetItem(item_data.item_id);
			if (NULL == item_base) continue;

			if (ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType()) continue;

			int add_lingjing = 0;
			ItemConfigData stuff;
			const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_data.item_id);
			if (Equipment::E_TYPE_SPECIAL_JINGLING == equip->GetEquipType())
			{
				EquipmentParam *equip_param = (EquipmentParam *)item_data.param;
				add_lingjing = static_cast<int>(LOGIC_CONFIG->GetJingLingCfg().GetRecycleLingjing(item_data.item_id, equip_param->strengthen_level) /* * 0.8f */);

				const JingLingWuXingCfg *wuxing_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingWuXingCfg(item_base->GetColor(), equip_param->param1);
				if (NULL != wuxing_cfg && equip_param->param1 > 0)
				{
					stuff.item_id = wuxing_cfg->stuff_id;
					stuff.num = wuxing_cfg->recycle_stuff_num;
					stuff.is_bind = true;
				}

				add_lingjing += item_base->GetRecyclGet(item_data.param);
				m_role->AddLingJing(add_lingjing, "Discard item");

				if (0 != stuff.item_id && stuff.num > 0)
				{
					m_role->GetKnapsack()->PutOrMail(stuff, PUT_REASON_DISCARD_JINGLING);
				}
			}
		}
	}

	int add_exp = 0;
	if (need_num_uplevel > 0 && m_role->GetKnapsack()->ConsumeItemByIndexList(g_consume_list.count, g_consume_list.index_list, g_consume_list.num_list, "JinglingCardUplevel"))
	{
		add_exp += need_num_uplevel * JINGLING_CARD_STUFF_ADD_EXP;
	}

	if (add_exp <= 0)
	{
		return;
	}

	int old_level = card_item->level;

	this->AddExp(card_type, card_item, add_exp);
	this->SyncJingLingInfo();
	
	if (card_item->level > old_level)
	{
		//m_role->GetTitleManager()->RecalJinglingCardTitle();        // 精灵称号卡屏蔽
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);
	}

	gamelog::g_log_jingling.printf(LL_INFO, "UpLevelCard user %d %s, card_type %d, cost_stuff_id %d, cost_stuff_count %d, add_exp %d,"
		"old level %d, new_level %d",
		m_role->GetUID(), m_role->GetName(), (int)card_type, (int)card_cfg->stuff_id, need_num_uplevel, add_exp, old_level, card_item->level);
}

void JingLingManager::AddExp(int card_idx, JingLingCardParamItem *card_item, int add_exp)
{
	if (NULL == card_item || add_exp <= 0 || card_item->level >= JINGLING_CARD_MAX_LEVEL) return;

	int round_count = 0; 
	while (add_exp > 0 && round_count ++ < 100)
	{
		if (card_item->level >= JINGLING_CARD_MAX_LEVEL)
		{
			break;
		}

		const JingLingCardCfg *item_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingCardCfg(card_idx, card_item->level + 1);
		if (NULL == item_cfg)
		{
			break;
		}

		int need_exp = item_cfg->exp;

		if (card_item->exp + add_exp >= need_exp)
		{
			add_exp -= (need_exp - card_item->exp);

			card_item->level += 1;
			card_item->exp = 0;
		}
		else
		{
			card_item->exp += add_exp;
			break;
		}
	}
}

int JingLingManager::GetCardMinLevel()
{
	int min_level = JINGLING_CARD_MAX_LEVEL;

	for (int i = 0; i < JINGLING_CARD_MAX_TYPE; ++ i)
	{
		min_level = m_card_list[i].level < min_level ? m_card_list[i].level : min_level;
	}

	return min_level;
}

int JingLingManager::GetCardTotalLevel()
{
	int total_level = 0;

	for (int i = 0; i < JINGLING_CARD_MAX_TYPE; ++i)
	{
		total_level += m_card_list[i].level;
	}

	return total_level;
}

void JingLingManager::Rename(const GameName new_name)
{
	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (other_cfg == nullptr)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg->rename_cost))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	GameName tmp_new_name; memset(tmp_new_name, 0, sizeof(tmp_new_name));

	memcpy(tmp_new_name, new_name, sizeof(tmp_new_name));
	tmp_new_name[sizeof(GameName) - 1] = 0;

	InvalidCharFilte(tmp_new_name);

	bool NAMEFILTER_OPEN_FLAG = ServerConfigPool::Instance().common_config.namefilter_open_flag;
	if (NAMEFILTER_OPEN_FLAG && NameFilter::Instance().HasSensitiveWord(sizeof(GameName), tmp_new_name)) 
	{
		m_role->NoticeNum(errornum::EN_RENAME_NAME_INVALID);
		return;
	} 

	F_STRNCPY(m_jingling_name, tmp_new_name, sizeof(GameName)); 

	m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg->rename_cost, "JinglingRename");

	if (NULL != m_role->GetScene())
	{
		this->SyncJingLingInfo();
		this->BroadcastJingLingInfo();
	}
}

void JingLingManager::SyncJingLingInfo()
{
	static Protocol::SCJingLingInfo cmd;
	
	UNSTD_STATIC_CHECK(sizeof(cmd.card_list) == sizeof(m_card_list));
	memcpy(cmd.card_list, m_card_list, sizeof(cmd.card_list));

	memset(cmd.shuxingdan_list, 0, sizeof(cmd.shuxingdan_list));
	UNSTD_STATIC_CHECK(sizeof(cmd.shuxingdan_list) == sizeof(m_shuxingdan_list));
	memcpy(cmd.shuxingdan_list, m_shuxingdan_list, sizeof(cmd.shuxingdan_list));

	UNSTD_STATIC_CHECK(sizeof(cmd.equip_strength_list) == sizeof(m_equip_strength_list));
	memcpy(cmd.equip_strength_list, m_equip_strength_list, sizeof(cmd.equip_strength_list));

	UNSTD_STATIC_CHECK(sizeof(cmd.phantom_level_list) == sizeof(m_phantom_level_list));
	memcpy(cmd.phantom_level_list, m_phantom_level_list, sizeof(cmd.phantom_level_list));

	UNSTD_STATIC_CHECK(sizeof(cmd.soul_level_list) == sizeof(m_soul_level_list));
	memcpy(cmd.soul_level_list, m_soul_level_list, sizeof(cmd.soul_level_list));

	memset(cmd.jingling_name, 0, sizeof(GameName));
	F_STRNCPY(cmd.jingling_name, m_jingling_name, sizeof(GameName));

	cmd.use_jingling_id = m_use_jingling_id;
	cmd.use_imageid = m_used_imageid;
	cmd.m_active_image_flag = m_active_image_flag;
	cmd.grade = m_grade;
	cmd.grade_bless = m_grade_bless;
	UNSTD_STATIC_CHECK(sizeof(cmd.phantom_image_flag) == sizeof(m_phantom_image_flag));
	memcpy(cmd.phantom_image_flag, m_phantom_image_flag, sizeof(cmd.phantom_image_flag));
	cmd.phantom_imageid = m_phantom_imgageid;
	cmd.taozhuang_level = m_soul_taozhuang_level;

	cmd.halo_active_image_flag = m_halo_active_image_flag;
	cmd.halo_level = m_halo_level;
	cmd.use_halo_img = m_use_halo_img;
	cmd.halo_bless_val = m_halo_bless_val;

	cmd.xianzhen_level = m_xianzhen_level;
	cmd.xianzhen_exp = m_xianzhen_exp;
	cmd.xianzhen_up_count = m_xianzhen_up_count;
	memcpy(cmd.hunyu_level_list, m_hunyu_level_list, sizeof(cmd.hunyu_level_list));
	memcpy(cmd.skill_storage_list, m_skill_storage_list, sizeof(cmd.skill_storage_list));
	memcpy(cmd.skill_refresh_item_list, m_skill_refresh_item_list, sizeof(cmd.skill_refresh_item_list));

	int count = 0;
	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (other_cfg == nullptr)
	{
		return;
	}

	for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; ++ i)
	{
		if (m_jingling_list[i].Invalid() || NULL == m_jingling_list[i].param) continue;
		if (count < 0 || count >= JINGLING_MAX_TAKEON_NUM) continue;

		//处理一下老号已经有仙宠装备并且有技能的情况，默认开两个技能槽，有装备技能的号也开启技能槽
		{
			EquipmentParam *equip_param = (EquipmentParam *)(m_jingling_list[i].param);
			for (int slot_index = 0; slot_index < JING_LING_SKILL_COUNT_MAX; ++slot_index)
			{
				if (0 != equip_param->jing_ling_skill_list[slot_index].is_open)   // 已经开启不做处理
				{
					continue;
				}

				if (slot_index < other_cfg->skill_solt_free_count)    // 默认开启
				{
					equip_param->jing_ling_skill_list[slot_index].is_open = 1;
					continue;
				}

				if (0 != equip_param->jing_ling_skill_list[slot_index].skill_id) //有技能
				{
					equip_param->jing_ling_skill_list[slot_index].is_open = 1;
					continue;
				}
			}
		}
		
		cmd.jingling_list[count].index = i;
		cmd.jingling_list[count].item_wrapper.Reset();
		m_jingling_list[i].WrapTo(&(cmd.jingling_list[count].item_wrapper));

		++ count;
	}
	
	cmd.count = count;

	unsigned int sendlen = sizeof(cmd) - sizeof(cmd.jingling_list[0]) * (JINGLING_MAX_TAKEON_NUM - cmd.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sendlen);
}

void JingLingManager::BroadcastJingLingInfo()
{
 	if (NULL != m_role->GetScene())
 	{
 		static Protocol::SCJingLingViewChange cmd;
 		cmd.obj_id = m_role->GetId();
		cmd.jingling_id = m_use_jingling_id;
		cmd.level = 0;
		cmd.use_imageid = m_used_imageid;
		cmd.jingling_phantom_img = m_phantom_imgageid;
		if (m_use_jingling_index >= 0 && m_use_jingling_index < JINGLING_MAX_TAKEON_NUM && m_use_jingling_id != 0)
		{
			const ItemGridData *grid_data = &m_jingling_list[m_use_jingling_index];
			const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
			if (NULL != equip && ItemBase::I_TYPE_EQUIPMENT == equip->GetItemType() && Equipment::E_TYPE_SPECIAL_JINGLING == equip->GetEquipType())
			{
				EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
				cmd.level = equip_param->strengthen_level;
			}
		}

		memset(cmd.jingling_name, 0, sizeof(GameName));
		F_STRNCPY(cmd.jingling_name, m_jingling_name, sizeof(GameName));

		cmd.jingling_cur_use_halo_img = m_use_halo_img;

 		m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
 	}
}

int JingLingManager::GetPutOnJinglingCount(int color)
{
	int jingling_count = 0;
	for (int i = 0; i <= JINGLING_MAX_TAKEON_NUM; i++)
	{
		if (m_jingling_list[i].Invalid())
		{
			continue;
		}
			
		if (color > 0)
		{
			const ItemGridData *grid_data = &m_jingling_list[i];
			const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
			if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() || Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
			{
				continue;
			}

			if (equip->GetColor() < color)
			{
				continue;
			}
		}

		jingling_count++;
	}

	return jingling_count;
}

int JingLingManager::GetPutOnSkillCount(int level)
{
	int count = 0;
	for (unsigned int jingling_index = 0; jingling_index < static_array_size(m_jingling_list); jingling_index++)
	{
		ItemGridData *grid_data = &m_jingling_list[jingling_index];
		if (grid_data->Invalid())
		{
			continue;
		}

		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
		if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()
			|| Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
		{
			continue;
		}

		EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
		for (unsigned int skill_index = 0; skill_index < static_array_size(equip_param->jing_ling_skill_list); skill_index ++)
		{
			if (equip_param->jing_ling_skill_list[skill_index].skill_id <= 0)
			{
				continue;
			}

			const JingLingSkillCfg *temp_skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(equip_param->jing_ling_skill_list[skill_index].skill_id);
			if (NULL != temp_skill_cfg)
			{
				if (temp_skill_cfg->skill_level >= level)
				{
					count++;
				}
			}
		}
	}

	return count;
}

bool JingLingManager::OnUseShuxingdan(int slot_idx, int use_count)
{
	if (slot_idx < 0 || slot_idx >= JINGLING_SHUXINGDAN_MAX_TYPE || use_count <= 0) 
	{
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_JINGLING, slot_idx);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_shuxingdan_list[slot_idx] + use_count > cfg_item->use_limit_num)
	{
		m_role->NoticeNum(errornum::EN_SHUXINGDAN_USE_LIMIT_NUM);
		return false;
	}

	m_shuxingdan_list[slot_idx] += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);
	this->SyncJingLingInfo();

	return true;
}

bool JingLingManager::OnUpGrade(int is_auto_buy)
{
	if (m_grade >= JINGLING_MAX_GRADE)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_MAX_GRADE);

		return false;
	}

	const JingLingUpGradeCfg *cur_upgrade_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingUpGradeCfg(m_grade);
	if (NULL == cur_upgrade_cfg)
	{
		return false;
	}

	const JingLingUpGradeCfg *target_grade_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingUpGradeCfg(m_grade + 1);
	if (NULL == target_grade_cfg)
	{
		return false;
	}
	
	const JingLingUpGradeProbCfg *upgrade_prob_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingUpGradeProbCfg(m_grade, m_grade_bless);
	if (NULL == upgrade_prob_cfg)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	// check
	{
		short stuff_count = 0;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = cur_upgrade_cfg->stuff_id;
		stuff_list[stuff_count].num = cur_upgrade_cfg->stuff_count;
		stuff_list[stuff_count].buyable = is_auto_buy ? true : false;

		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "JingLingManager::OnUpGrade"))
	{
		return false;
	}

	bool succeeded = false;

	const int old_grage = m_grade;
	const int old_bless = m_grade_bless;
	{
		// 生效

		int rand_val = RandomNum(10000);
		if (rand_val < upgrade_prob_cfg->succ_rate || m_grade_bless >= cur_upgrade_cfg->bless_val_limit)
		{
			succeeded = true;
		}

		if (false == succeeded)
		{
			short new_bless_val = m_grade_bless + upgrade_prob_cfg->RandAddBlessVal();

			if (new_bless_val > cur_upgrade_cfg->bless_val_limit) new_bless_val = cur_upgrade_cfg->bless_val_limit;

			m_grade_bless = new_bless_val;
		}
		else
		{
			m_grade += 1;
			m_grade_bless = 0;
			
			this->ActiveImage(target_grade_cfg->active_image_id);
			this->UseImage(target_grade_cfg->active_image_id);
		}
	}

	EventHandler::Instance().OnJingLingFeiSheng(m_role);
	this->SyncJingLingInfo();

	if (succeeded)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);

		this->BroadcastJingLingInfo();

		if (1 == target_grade_cfg->broadcast)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_link_jingling_upgrade, 
				UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), target_grade_cfg->grade, target_grade_cfg->active_image_id);
			if (sendlen > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	{
		gamelog::g_log_jingling.printf(LL_INFO, "OnUpGrade user [%d %s], old_grade[%d] grade [%d] old_bless[%d] new_bless[%d]",
			m_role->GetUID(), m_role->GetName(), old_grage, m_grade, old_bless, m_grade_bless);
	}

	return true;
}

bool JingLingManager::ActiveImage(int imageid)
{
	if (imageid <= 0 || imageid >= JINGLING_MAX_IMAGE_ID)
	{
		return false;
	}

	m_active_image_flag |= (1 << imageid);

	this->SyncJingLingInfo();

	return true;
}

bool JingLingManager::UseImage(int imageid)
{
	if (0 == (m_active_image_flag & (1 << imageid)))
	{
		m_role->NoticeNum(errornum::EN_JINGLING_IMG_NOT_ACTIVE);
		return false;
	}

	m_used_imageid = imageid;

	this->BroadcastJingLingInfo();
	this->SyncJingLingInfo();

	m_role->NoticeNum(noticenum::NT_SELECT_SUCC);

	return true;
}

bool JingLingManager::ActivePhantomImage(int type)
{
	if (type < 0 || type >= JINGLING_PTHANTOM_MAX_TYPE)
	{
		return false;
	}

	if (IsActivedPhantomImg(type))
	{
		m_role->NoticeNum(errornum::EN_JINGLING_ALREADY_ACTIVE);
		return false;
	}

	m_phantom_level_list[type] = 1;

	this->ActivePhantomImageFlag(type);
	this->UsePhantomImg(type);

	m_role->GetRoleBigSmallGoal()->ActiveRoleBigSmallGoalSpecialAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_XIANCHONG, type);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);
	this->SyncJingLingInfo();

	m_role->GetTitleManager()->CheckAllTitleCondition(TITILE_TYPE_PHANTOM);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_JINGLING,type,true);
	return true;
}

bool JingLingManager::ActivePhantomImageFlag(int phantom_img_id)
{
	if (phantom_img_id < 0 || phantom_img_id >= JINGLING_PTHANTOM_MAX_TYPE)
	{
		return false;
	}
	return BitmapFunction::SetBit(m_phantom_image_flag, sizeof(m_phantom_image_flag), phantom_img_id);
}

bool JingLingManager::UsePhantomImg(int type)
{
	static const int CANCEL_USE_PHANTOM_IMG = -1;
	if (CANCEL_USE_PHANTOM_IMG == type)
	{
		m_phantom_imgageid = CANCEL_USE_PHANTOM_IMG;
	}
	else
	{
		if (type < 0 || type >= JINGLING_PTHANTOM_MAX_TYPE)
		{
			return false;
		}

		int phantom_grade = m_phantom_level_list[type];

		if (phantom_grade > 0 && this->IsActivedPhantomImg(type))
		{
			m_phantom_imgageid = type;

			m_role->NoticeNum(noticenum::NT_SELECT_SUCC);
		}
		else
		{
			m_role->NoticeNum(errornum::EN_PHANTOM_IMG_NOT_ACTIVE);
		}
	}

	this->SyncJingLingInfo();
	this->BroadcastJingLingInfo();

	return true;
}


bool JingLingManager::IsActivedPhantomImg(int phantom_img_id)
{
	if (phantom_img_id < 0 || phantom_img_id >= JINGLING_PTHANTOM_MAX_TYPE)
	{
		return false;
	}
	return BitmapFunction::IsBitSet(m_phantom_image_flag, sizeof(m_phantom_image_flag), phantom_img_id);
}

void JingLingManager::OnStrength(short index)
{
	if (index < 0 || index >= JINGLING_EQUIP_MAX_PART)
	{
		return;
	}

	const int old_stength_level = m_equip_strength_list[index];

	const JingLingEquipStrengthCfg *strength_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingEquipStrengthCfg(index, old_stength_level + 1);
	if (NULL == strength_cfg)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_EQUIP_MAX_STRENGTH);

		return;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		short stuff_count = 0;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = strength_cfg->stuff_id;
		stuff_list[stuff_count].num = strength_cfg->stuff_count;
		stuff_list[stuff_count].buyable = false;

		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			return;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "JingLingManager::OnStrength"))
	{
		return;
	}

	++ m_equip_strength_list[index];

	m_role->NoticeNum(noticenum::NT_STRENGTH_SUCC);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);
	EventHandler::Instance().OnJingLingEquipmentStrength(m_role);
	this->SyncJingLingInfo();

	if (1 == strength_cfg->broadcast)
	{	
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_link_jingling_strength, UidToInt(m_role->GetUserId()),
			m_role->GetName(), (int)m_role->GetCamp(), strength_cfg->index, strength_cfg->strength_level);
		if (length > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	gamelog::g_log_jingling.printf(LL_INFO, "OnStrength user %d %s, index %d, old_level %d, new_level %d",
		m_role->GetUID(), m_role->GetName(), index, (int)old_stength_level, m_equip_strength_list[index]);
}

void JingLingManager::OnUpLevelPhantom(const int type)
{
	if (type < 0 || type >= JINGLING_PTHANTOM_MAX_TYPE)
	{
		return;
	}

	int old_level = m_phantom_level_list[type];

	if (old_level >= JINGLING_PTHANTOM_MAX_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_PHANTOM_MAX_LEVEL);
		return;
	}

	const JingLingPhantomCfg *cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingPhantomCfg(type, old_level + 1);
	if (NULL == cfg)
	{
		return;
	}

	if (cfg->level != old_level + 1)
	{
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		short stuff_count = 0;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = cfg->stuff_id;
		stuff_list[stuff_count].num = cfg->stuff_num;
		stuff_list[stuff_count].buyable = false;

		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			m_role->NoticeNum(errornum::EN_PHANTOM_CARD_NOT_ENOUGH);
			return;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "JingLingManager::OnUpLevelPhantom"))
		{
			return;
		}
	}	

	int new_level = ++m_phantom_level_list[type];

	if (0 == old_level)
	{
		ActivePhantomImage(type);
	}

	if (cfg->is_broadcast)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_link_jingling_phantom, UidToInt(m_role->GetUserId()),
			m_role->GetName(), (int)m_role->GetCamp(), new_level);
		if (length > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);

	this->SyncJingLingInfo();

	gamelog::g_log_jingling.printf(LL_INFO, "OnUpLevelPhantom user %d %s, phantom_type %d, old_level %d, new_level %d",
		m_role->GetUID(), m_role->GetName(), type, (int)old_level, new_level);
}

void JingLingManager::OnUpGradeSoul(const int type, int is_protected, int is_auto_buy)
{
	if (type < 0 || type  >= JINGLING_CARD_MAX_TYPE)
	{
		return;
	}

	if (m_soul_level_list[type] >= JINGLING_SOUL_MAX_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_SOUL_MAX_LEVEL);
		return;
	}
	
	const int old_level = m_soul_level_list[type];

	const JingLingSoulCfg *cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingSoulcfg(type, old_level + 1);
	if (NULL == cfg)
	{
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		short stuff_count = 0;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = cfg->stuff1_id;
		stuff_list[stuff_count].num = cfg->stuff1_num;
		stuff_list[stuff_count].buyable = (1 == is_auto_buy ? true : false);

		++ stuff_count;

		if (1 == is_protected && cfg->stuff2_num > 0)
		{
			stuff_list[stuff_count].item_id = cfg->stuff2_id;
			stuff_list[stuff_count].num = cfg->stuff2_num;
			stuff_list[stuff_count].buyable = false;

			++ stuff_count;	
		}

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			m_role->NoticeNum(errornum::EN_JINGLING_SOUL_NOT_ENOUGH);
			return;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "JingLingManager::OnUpGradeSoul"))
		{
			return;
		}
	}	
	
	{
		int rate = RandomNum(100);
		if (rate < cfg->succ_rate)
		{
			++ m_soul_level_list[type];
		}
		else
		{
			if (0 == is_protected && 1 == cfg->fail_down_level)
			{
				-- m_soul_level_list[type];
			}
		}
	}

	this->UpdateSoulTaoZhuangLevel();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);

	this->SyncJingLingInfo();

	if (cfg->is_broadcast)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_link_jingling_soul, UidToInt(m_role->GetUserId()),
			m_role->GetName(), (int)m_role->GetCamp(), m_soul_level_list[type]);
		if (length > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	gamelog::g_log_jingling.printf(LL_INFO, "OnUpGradeSoul user %d %s, soul_type %d, old_level %d, new_level %d",
		m_role->GetUID(), m_role->GetName(), type, (int)old_level, m_soul_level_list[type]);
}

void JingLingManager::UpdateSoulTaoZhuangLevel()
{
	const int MAX_LEVEL = 999;
	int temp_level = MAX_LEVEL;

	for (int i = 0; i < JINGLING_CARD_MAX_TYPE; ++ i)
	{
		if (m_soul_level_list[i] == 0)
		{
			temp_level = 0;
			break;
		}

		if (m_soul_level_list[i] < temp_level)
		{
			temp_level = m_soul_level_list[i];
		}
	}

	if (temp_level != MAX_LEVEL && temp_level <= JINGLING_SOUL_MAX_LEVEL)
	{
		m_soul_taozhuang_level = temp_level;
	}
}

int JingLingManager::GetJingLingPhantomCount()
{
	int count = 0;
	for (int i = 0; i < JINGLING_PTHANTOM_MAX_TYPE; ++ i)
	{
		if (this->IsActivedPhantomImg(i))
		{
			++ count;
		}
	}

	return count;
}

bool JingLingManager::ActiveHaloImage(int imageid, bool recalc)
{
	if (imageid >= 0 && imageid <= HALO_MAX_IMAGEID)
	{
		m_halo_active_image_flag |= (1 << imageid); 
	}

	if(recalc)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);
	}

	return true;
}

bool JingLingManager::CheckActiveHalo(bool recalc)
{
	if (m_halo_level < 1)
	{
		m_halo_level = 1;

		m_halo_active_image_flag |= (1 << 1); 

		this->SyncJingLingInfo();

		if (recalc)
		{
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);
		}

		gamelog::g_log_jingling.printf(LL_INFO, "CheckActiveHalo user %d %s ActiveHaloImage"  ,
			m_role->GetUID(), m_role->GetName());
	}

	return true;
}

bool JingLingManager::UseHaloImage(int imageid)
{
	if (imageid < 0 || imageid > HALO_MAX_IMAGEID)
	{
		return false;
	}

	if (0 == (m_halo_active_image_flag & (1 << imageid)))
	{
		return false;
	}

	m_use_halo_img = imageid;

	m_role->NoticeNum(noticenum::NT_SELECT_APPERANCE_SUCC);

	this->BroadcastJingLingInfo();
	return true;
}

bool JingLingManager::HaloUpgrade(bool is_auto_buy)
{
	Knapsack *knapsack = m_role->GetKnapsack();
	if (NULL == knapsack)
	{
		return false;
	}

	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	int halo_max_level = LOGIC_CONFIG->GetJingLingCfg().GetHaloMaxLevel();
	if (m_halo_level >= halo_max_level)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_HALO_LEVEL_LIMIT);
		return false;
	}

	const JingLingHaloUplevelCfg *curr_level_cfg = LOGIC_CONFIG->GetJingLingCfg().GetHaloUplevelCfg(m_halo_level);
	if (NULL == curr_level_cfg)
	{
		return false;
	}

	int old_halo_bless_val = m_halo_bless_val;
	int old_halo_level = m_halo_level;

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		// 获取可消耗的背包材料列表

		short stuff_count = 0;

		// 进阶符
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg()->halo_upgrade_item_id;
		stuff_list[stuff_count].num = curr_level_cfg->uplevel_need_stuff_count;
		stuff_list[stuff_count].buyable = is_auto_buy ? true : false;

		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "JingLingManager::HaloUpGrade"))
	{
		return false;
	}

	{
		// 生效
		int rand_val = RandomNum(100);
		int add_bless = 0;

		const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
		if (NULL == other_cfg)
		{
			return false;
		}

		if (rand_val < other_cfg->halo_baoji_rate)
		{
			add_bless = other_cfg->halo_baoji_exp;
		}
		else
		{
			add_bless = other_cfg->halo_normal_exp;
		}

		bool has_uplevel = false;

		int loop_count = 0; 
		while (add_bless > 0 && loop_count ++ < 100)
		{
			if (m_halo_level >= halo_max_level)
			{
				break;
			}

			const JingLingHaloUplevelCfg *pre_using_cfg = LOGIC_CONFIG->GetJingLingCfg().GetHaloUplevelCfg(m_halo_level);
			if (NULL == pre_using_cfg)
			{
				break;
			}

			if (m_halo_bless_val + add_bless >= pre_using_cfg->bless_val_limit)
			{
				has_uplevel = true;

				int detal_bless = pre_using_cfg->bless_val_limit - m_halo_bless_val;

				add_bless -= detal_bless;
				m_halo_bless_val = 0;
				++ m_halo_level;

				const JingLingHaloUplevelCfg *using_cfg = LOGIC_CONFIG->GetJingLingCfg().GetHaloUplevelCfg(m_halo_level);
				if (NULL != using_cfg)
				{
					// 传闻
					if (using_cfg->is_broadcast) 
					{
						int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_jingling_halo_upgrade_notice, 
							UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), using_cfg->big_grade);
						if (sendlen > 0) 
						{
							if (using_cfg->level >= 31)
							{
								World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
							}
							else
							{
								World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
							}
						}
					}

					// 激活
					int active_image_id = LOGIC_CONFIG->GetJingLingCfg().GetHaloActiveIdWithLevel(m_halo_level);
					if (active_image_id >= 0 && active_image_id <= HALO_MAX_IMAGEID)
					{
						if(0 == (m_halo_active_image_flag & (1<<active_image_id)))
						{
							ActiveHaloImage(active_image_id, false);
							UseHaloImage(active_image_id);
						}
					}
				}
			}
			else 
			{
				m_halo_bless_val += add_bless;
				add_bless = 0;
				break;
			}
		}

		if (has_uplevel)
		{
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);
			m_role->NoticeNum(noticenum::NT_JINGLING_HALO_UPGRADE_SUCC);
		}
	}

	gamelog::g_log_jingling.printf(LL_INFO, "HaloUpgrade user %d %s, old_level %d, new_level %d, old_bless_value[%d], now_bless_value[%d]",
		m_role->GetUID(), m_role->GetName(), (int)old_halo_level, m_halo_level, old_halo_bless_val, m_halo_bless_val);

	this->SyncJingLingInfo();

	return true;
}

void JingLingManager::OnUpLevelWuXing(short jingling_index, bool is_safe, bool is_auto_buy)
{
	if (jingling_index < 0 || jingling_index >= JINGLING_MAX_TAKEON_NUM)
	{
		return;
	}

	ItemGridData *grid_data = &m_jingling_list[jingling_index];
	if (grid_data->Invalid())
	{
		return;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
	const ItemBase *equip_base = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()
		|| Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
	{
		return;
	}

	if (equip_base == NULL)
	{
		return;
	}

	EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;

	if (equip_param->param1 > JING_LING_WU_XING_LEVEL_MAX)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_WUXING_LEVEL_LIMIT);
		return;
	}

	const JingLingWuXingCfg *wuxing_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingWuXingCfg(equip->GetColor(), equip_param->param1);
	const JingLingWuXingCfg *next_wuxing_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingWuXingCfg(equip->GetColor(), equip_param->param1 + 1);
	if (NULL == wuxing_cfg || wuxing_cfg->succ_rate <= 0 || NULL == next_wuxing_cfg)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_WUXING_LEVEL_LIMIT);
		return;
	}


	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	g_consume_list.Reset();

	{
		int stuff_count = 0;
		// 材料1
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = wuxing_cfg->stuff_id;
		//stuff_list[stuff_count].item_id = stuff_cfg->stuff_id;
		stuff_list[stuff_count].num = wuxing_cfg->stuff_num;
		stuff_list[stuff_count].buyable = is_auto_buy ? true : false;
		++ stuff_count;

		// 保护符
		if (is_safe && wuxing_cfg->succ_rate < 100 && wuxing_cfg->safe_id > 0)
		{
			stuff_list[stuff_count].item_id = wuxing_cfg->safe_id;
			stuff_list[stuff_count].num = wuxing_cfg->safe_num;
			stuff_list[stuff_count].buyable = false;
			++ stuff_count;
		}

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &g_consume_list, true))
		{
			return;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, g_consume_list, "JingLingManager::OnUpLevelWuXing"))
	{
		return;
	}

	if (RandomNum(100) < wuxing_cfg->succ_rate)
	{
		equip_param->param1++;
		if (equip_param->param1 > equip_param->param2) equip_param->param2 = equip_param->param1;

		if (equip_param->param1 > m_history_max_wuxing)
		{
			m_history_max_wuxing = equip_param->param1;
		}

		int sendlen = 0;
		if (1 == next_wuxing_cfg->broadcast_type)
		{
			sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_jingling_wuxing_notice1,
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), grid_data->item_id, equip_param->param1);
		}
		else if (2 == next_wuxing_cfg->broadcast_type)
		{
			sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_jingling_wuxing_notice2,
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), grid_data->item_id, equip_param->param1);
		}
		if (sendlen > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
		}
	}
	else
	{
		if (wuxing_cfg->is_down && !is_safe && equip_param->param1 > 0)
		{
			equip_param->param1--;
		}

		m_role->NoticeNum(errornum::EN_JINGLING_WUXING_UP_FAIL);
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);

	this->SyncJingLingInfo();

	EventHandler::Instance().OnJingLingWuxingUpLevel(m_role, equip_param->param1);
}

void JingLingManager::OnUpLevelXianZhen(bool is_auto_buy)
{
	if (m_xianzhen_level > JING_LING_XIAN_ZHEN_LEVEL_MAX)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_XIANZHEN_LEVEL_LIMIT);
		return;
	}

	const JingLingXianZhenCfg *xianzhen_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingXianZhenCfg(m_xianzhen_level);
	const JingLingXianZhenCfg *next_xianzhen_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingXianZhenCfg(m_xianzhen_level + 1);
	if (NULL == xianzhen_cfg || NULL == next_xianzhen_cfg)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_XIANZHEN_LEVEL_LIMIT);
		return;
	}

	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	g_consume_list.Reset();

	{
		int stuff_count = 0;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = other_cfg->xianzhen_stuff_id;
		stuff_list[stuff_count].num = xianzhen_cfg->stuff_num;
		stuff_list[stuff_count].buyable = is_auto_buy;
		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &g_consume_list, true))
		{
			return;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, g_consume_list, "JingLingManager::OnUpLevelXianZhen"))
		{
			return;
		}
	}

	int add_exp = other_cfg->xianzhen_add_exp;
	if (m_xianzhen_up_count >= other_cfg->xianzhen_force_big_num)
	{
		add_exp = other_cfg->xianzhen_big_exp;
		m_xianzhen_up_count = 0;
	}
	else
	{
		int rand_num = RandomNum(100);
		if (rand_num < other_cfg->xianzhen_big_rate)
		{
			add_exp = other_cfg->xianzhen_big_exp;
		}
		else if (rand_num < other_cfg->xianzhen_small_rate)
		{
			add_exp = other_cfg->xianzhen_small_exp;
		}

		m_xianzhen_up_count++;
	}

	add_exp *= xianzhen_cfg->stuff_num;
	m_xianzhen_exp += add_exp;

	if (m_xianzhen_exp >= xianzhen_cfg->need_exp)
	{
		m_xianzhen_level++;
		m_xianzhen_exp -= xianzhen_cfg->need_exp;

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);
	}

	this->SyncJingLingInfo();
}

void JingLingManager::OnUpLevelHunYu(int hunyu_type)
{
	if (hunyu_type < 0 || hunyu_type >= XIAN_ZHEN_HUN_YU_TYPE_MAX)
	{
		return;
	}

	if (m_hunyu_level_list[hunyu_type] > JING_LING_XIAN_ZHEN_HUN_YU_LEVEL_MAX)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_HUNYU_LEVEL_LIMIT);
		return;
	}

	const JingLingXianZhenHunYuCfg *hunyu_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingXianZhenHunYuCfg(hunyu_type, m_hunyu_level_list[hunyu_type]);
	const JingLingXianZhenHunYuCfg *next_hunyu_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingXianZhenHunYuCfg(hunyu_type, m_hunyu_level_list[hunyu_type] + 1);
	if (NULL == hunyu_cfg || NULL == next_hunyu_cfg)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_HUNYU_LEVEL_LIMIT);
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	g_consume_list.Reset();

	{
		int stuff_count = 0;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = hunyu_cfg->stuff_id;
		stuff_list[stuff_count].num = hunyu_cfg->stuff_num;
		stuff_list[stuff_count].buyable = false;
		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &g_consume_list, true))
		{
			return;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, g_consume_list, "JingLingManager::OnUpLevelHunYu"))
		{
			return;
		}
	}

	m_hunyu_level_list[hunyu_type]++;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);

	this->SyncJingLingInfo();
}

void JingLingManager::OnSkillRemove(short jingling_index, short skill_index, bool is_auto_buy)
{
	if (jingling_index < 0 || jingling_index >= JINGLING_MAX_TAKEON_NUM
		|| skill_index < 0 || skill_index >= JING_LING_SKILL_COUNT_MAX)
	{
		return;
	}

	ItemGridData *grid_data = &m_jingling_list[jingling_index];
	if (grid_data->Invalid())
	{
		return;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()
		|| Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
	{
		return;
	}

	EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
	JingLingSkillInfo &skill_info = equip_param->jing_ling_skill_list[skill_index];
	if (0 == skill_info.skill_id)
	{
		return;
	}

	const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(skill_info.skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	{
		g_consume_list.Reset();
		int stuff_count = 0;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = skill_cfg->remove_stuff_id;
		stuff_list[stuff_count].num = 1;
		stuff_list[stuff_count].buyable = is_auto_buy;
		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &g_consume_list, true))
		{
			return;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, g_consume_list, "JingLingManager::OnSkillRemove"))
		{
			return;
		}
	}

	skill_info.Reset();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);

	this->SyncJingLingInfo();
}

void JingLingManager::OnSkillChangeMove(short jingling_index, short skill_index, bool is_auto_buy)
{
	if (jingling_index < 0 || jingling_index >= JINGLING_MAX_TAKEON_NUM
		|| skill_index < 0 || skill_index >= JING_LING_SKILL_COUNT_MAX)
	{
		return;
	}

	ItemGridData *grid_data = &m_jingling_list[jingling_index];
	if (grid_data->Invalid())
	{
		return;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()
		|| Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
	{
		return;
	}

	EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
	JingLingSkillInfo &skill_info = equip_param->jing_ling_skill_list[skill_index];
	if (0 == skill_info.skill_id || 0 != skill_info.can_move)
	{
		return;
	}

	const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(skill_info.skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	int storage_index = -1;
	for (int i = 0; i < JING_LING_SKILL_STORAGE_MAX; i++)
	{
		if (0 == m_skill_storage_list[i].skill_id)
		{
			storage_index = i;
			break;
		}
	}

	if (storage_index < 0)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_SKILL_STORAGE_FULL);
		return;
	}

	{
		g_consume_list.Reset();
		int stuff_count = 0;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = skill_cfg->move_stuff_id;
		stuff_list[stuff_count].num = 1;
		stuff_list[stuff_count].buyable = is_auto_buy;
		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &g_consume_list, true))
		{
			return;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, g_consume_list, "JingLingManager::OnSkillChangeMove"))
		{
			return;
		}
	}

	m_skill_storage_list[storage_index] = skill_info;
	m_skill_storage_list[storage_index].can_move = 1;
	skill_info.Reset();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);

	this->SyncJingLingInfo();
}

void JingLingManager::OnSkillPutOn(short jingling_index, short skill_index, short storage_index)
{
	if (jingling_index < 0 || jingling_index >= JINGLING_MAX_TAKEON_NUM 
		|| skill_index < 0 || skill_index >= JING_LING_SKILL_COUNT_MAX 
		|| storage_index < 0 || storage_index >= JING_LING_SKILL_STORAGE_MAX)
	{
		return;
	}

	ItemGridData *grid_data = &m_jingling_list[jingling_index];
	if (grid_data->Invalid())
	{
		return;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()
		|| Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
	{
		return;
	}

	EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
	if (0 != equip_param->jing_ling_skill_list[skill_index].skill_id
		|| 0 == m_skill_storage_list[storage_index].skill_id)
	{
		return;
	}

	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (NULL == other_cfg) return;

	if (skill_index >= other_cfg->skill_solt_free_count && 0 == equip_param->jing_ling_skill_list[skill_index].is_open) // 技能槽位未开启
	{
		m_role->NoticeNum(errornum::EN_JINGLING_SKILL_GRID_NOT_OPEN);
		return;
	}

	const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(m_skill_storage_list[storage_index].skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	for (int i = 0; i < JING_LING_SKILL_COUNT_MAX; i++)
	{
		const JingLingSkillCfg *temp_skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(equip_param->jing_ling_skill_list[i].skill_id);
		if (NULL != temp_skill_cfg && temp_skill_cfg->skill_type == skill_cfg->skill_type)
		{
			m_role->NoticeNum(errornum::EN_JINGLING_SKILL_TYPE_LIMIT);
			return;
		}
	}

	equip_param->jing_ling_skill_list[skill_index] = m_skill_storage_list[storage_index];
	m_skill_storage_list[storage_index].Reset();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);
	grid_data->is_bind = true;
	this->SyncJingLingInfo();

	EventHandler::Instance().OnJingLingSkillChange(m_role);
}

void JingLingManager::OnSkillTakeOff(short jingling_index, short skill_index, short storage_index)
{
	if (jingling_index < 0 || jingling_index >= JINGLING_MAX_TAKEON_NUM 
		|| skill_index < 0 || skill_index >= JING_LING_SKILL_COUNT_MAX 
		|| storage_index < 0 || storage_index >= JING_LING_SKILL_STORAGE_MAX)
	{
		return;
	}

	ItemGridData *grid_data = &m_jingling_list[jingling_index];
	if (grid_data->Invalid())
	{
		return;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()
		|| Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
	{
		return;
	}

	EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
	if (0 == equip_param->jing_ling_skill_list[skill_index].skill_id
		|| 0 == equip_param->jing_ling_skill_list[skill_index].can_move
		|| 0 != m_skill_storage_list[storage_index].skill_id)
	{
		return;
	}

	m_skill_storage_list[storage_index] = equip_param->jing_ling_skill_list[skill_index];
	equip_param->jing_ling_skill_list[skill_index].Reset();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);

	this->SyncJingLingInfo();
}

void JingLingManager::OnSkillLearn(short jingling_index, short skill_index, short item_index)
{
	if (jingling_index < 0 || jingling_index >= JINGLING_MAX_TAKEON_NUM
		|| skill_index < 0 || skill_index >= JING_LING_SKILL_COUNT_MAX)
	{
		return;
	}

	ItemGridData *grid_data = &m_jingling_list[jingling_index];
	if (grid_data->Invalid())
	{
		return;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()
		|| Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
	{
		return;
	}

	ItemGridData item_grid_data;
	if (!m_role->GetKnapsack()->GetItemGridData(item_index, &item_grid_data))
	{
		m_role->NoticeNum(errornum::EN_FUNC_ITEM_LOAD);
		return;
	}

	const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfgByItem(item_grid_data.item_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
	if (0 == skill_cfg->pre_skill)
	{
		for (int i = 0; i < JING_LING_SKILL_COUNT_MAX; i++)
		{
			const JingLingSkillCfg *temp_skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(equip_param->jing_ling_skill_list[i].skill_id);
			if (NULL != temp_skill_cfg && temp_skill_cfg->skill_type == skill_cfg->skill_type)
			{
				m_role->NoticeNum(errornum::EN_JINGLING_SKILL_TYPE_LIMIT);
				return;
			}
		}
	}
	else
	{
		if (skill_cfg->pre_skill != equip_param->jing_ling_skill_list[skill_index].skill_id)
		{
			m_role->NoticeNum(errornum::EN_JINGLING_SKILL_PRE);
			return;
		}
	}
	
	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (NULL == other_cfg) return;

	if (skill_index >= other_cfg->skill_solt_free_count && 0 == equip_param->jing_ling_skill_list[skill_index].is_open) // 技能槽位未开启
	{
		m_role->NoticeNum(errornum::EN_JINGLING_SKILL_GRID_NOT_OPEN);
		return;
	}

	m_role->GetKnapsack()->ConsumeItemByIndex(item_index, 1, "JingLingSkillLearn");

	equip_param->jing_ling_skill_list[skill_index].skill_id = skill_cfg->skill_id;
	equip_param->jing_ling_skill_list[skill_index].can_move = 0;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING);
	grid_data->is_bind = true;
	this->SyncJingLingInfo();

	EventHandler::Instance().OnJingLingSkillChange(m_role);
}

void JingLingManager::OnSkillRefreshActive(short refresh_index)
{
	if (refresh_index < 0 || refresh_index >= JING_LING_SKILL_REFRESH_ITEM_MAX)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	JingLingRefreshItem &refresh_item = m_skill_refresh_item_list[refresh_index];
	if (0 != refresh_item.is_active)
	{
		return;
	}

	{
		g_consume_list.Reset();
		int stuff_count = 0;
		// 材料1
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg()->get_skill_item;
		stuff_list[stuff_count].num = 1;
		stuff_list[stuff_count].buyable = false;
		++stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &g_consume_list, true))
		{
			return;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, g_consume_list, "JingLingManager::OnSkillRefreshActive"))
	{
		return;
	}

	refresh_item.is_active = 1;

	const JingLingSkillRefreshCfg *refresh_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingSkillRefreshCfg(0);
	if (NULL != refresh_cfg)
	{
		refresh_item.skill_list[0] = this->RandJingLingSkill(refresh_cfg);
	}

	this->SyncJingLingInfo();
}

void JingLingManager::OnSkillRefresh(short refresh_index, bool is_ten, bool is_auto_buy)
{
	if (refresh_index < 0 || refresh_index >= JING_LING_SKILL_REFRESH_ITEM_MAX)
	{
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_SKILL_REFRESH, 0);
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	JingLingRefreshItem &refresh_item = m_skill_refresh_item_list[refresh_index];
	const JingLingSkillRefreshCfg *refresh_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingSkillRefreshCfg(refresh_item.refresh_count);
	if (NULL == refresh_cfg)
	{
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_SKILL_REFRESH, 0);
		return;
	}

	{
		bool is_free = false;
		const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
		if (NULL == other_cfg)
		{
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_SKILL_REFRESH, 0);
			return;
		}

		if (!is_ten && m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_JINGLING_SKILL_COUNT, other_cfg->skill_free_refresh_count))
		{
			m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_JINGLING_SKILL_COUNT);
			is_free = true;
		}

		if (!is_free)
		{
			static ItemExtern::ItemConsumeList consume_list;
			consume_list.Reset();

			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
			stuff_list[0].item_id = other_cfg->skill_refresh_consume_id;
			stuff_list[0].num = other_cfg->refresh_one_consume_count;
			if (is_ten)
			{
				stuff_list[0].num = other_cfg->refresh_ten_consume_count;
			}
			stuff_list[0].buyable = is_auto_buy;

			if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list))
			{
				m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
				m_role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_SKILL_REFRESH, 0);
				return;
			}

			// 消耗
			if (!ItemExtern::ConsumeItem(m_role, consume_list, "JingLingManager::OnSkillRefresh"))
			{
				m_role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_SKILL_REFRESH, 0);
				return;
			}
		}	
	}

	if (is_ten)
	{
		for (int i = 1; i < JING_LING_SKILL_REFRESH_SKILL_MAX; i++)
		{
			if (refresh_item.refresh_count > refresh_cfg->max_count)
			{
				refresh_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingSkillRefreshCfg(refresh_item.refresh_count);
				if (NULL == refresh_cfg) break;
			}

			refresh_item.skill_list[i] = this->RandJingLingSkill(refresh_cfg);
			refresh_item.refresh_count++;
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_SKILL_REFRESH, 1);
		}
	}
	else
	{
		refresh_item.skill_list[1] = this->RandJingLingSkill(refresh_cfg);
		refresh_item.refresh_count++;
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_SKILL_REFRESH, 1);
	}

	this->SyncJingLingInfo();

	EventHandler::Instance().OnJingLingSkillChange(m_role);
}

void JingLingManager::OnSkillGet(short refresh_index, short skill_index)
{
	if (refresh_index < 0 || refresh_index >= JING_LING_SKILL_REFRESH_ITEM_MAX
		|| skill_index < 0 || skill_index >= JING_LING_SKILL_REFRESH_SKILL_MAX)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	JingLingRefreshItem &refresh_item = m_skill_refresh_item_list[refresh_index];
	//if (0 == refresh_item.is_active)
	//{
	//	return;
	//}

	if (0 == refresh_item.skill_list[skill_index])
	{
		return;
	}

	const JingLingSkillCfg *skill_cfg = LOGIC_CONFIG->GetJingLingCfg().GetSkillCfg(refresh_item.skill_list[skill_index]);
	if (NULL == skill_cfg)
	{
		return;
	}

	memset(&refresh_item, 0, sizeof(refresh_item));

	if (!knapsack->Put(ItemConfigData(skill_cfg->book_id, true, 1), PUT_REASON_JING_LING_SKILL))
	{
		return;
	}

	this->SyncJingLingInfo();
}

void JingLingManager::SendExploreInfo(int reason)
{
	Protocol::SCJinglIngExploreInfo pro;
	pro.reason = reason;
	pro.explore_mode = m_explore_mode;
	pro.explore_maxhp = m_explore_maxhp;
	pro.explore_hp = m_explore_hp;
	pro.buy_buff_count = m_explore_buy_buff_count;
	memcpy(pro.explore_info_list, m_explore_info_list, sizeof(pro.explore_info_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void JingLingManager::OnExploreSelectMode(short mode)
{
	if (mode < 0 || mode >= JING_LING_EXPLORE_MODE_COUNT)
	{
		return;
	}

	if (0 != m_explore_maxhp || m_history_max_capability <= 0)
	{
		return;
	}

	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (0 != m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_JING_LING_EXPLORE_CHALLENGE))
	{
		return;
	}

	int self_capability = m_history_max_capability + (int)(m_history_max_capability * (m_explore_buy_buff_count * other_cfg->explore_buff_add_per * 0.01));
	m_explore_maxhp = (int)(self_capability * (other_cfg->explore_hp_percent * 0.01));
	m_explore_hp = m_explore_maxhp;
	m_explore_mode = mode;
	for (int i = 0; i < JING_LING_EXPLORE_LEVEL_COUNT; i++)
	{
		const JingLingExploreCfg *explore_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingExploreCfg(mode, i);
		if (NULL != explore_cfg && explore_cfg->min_capability > 0 && explore_cfg->max_capability >= explore_cfg->min_capability)
		{
			int rand_num = explore_cfg->min_capability + RandomNum(explore_cfg->max_capability - explore_cfg->min_capability + 1);
			m_explore_info_list[i].capability = (int)(m_history_max_capability * (rand_num * 0.01));
			m_explore_info_list[i].hp = (int)(m_explore_info_list[i].capability * (other_cfg->explore_hp_percent * 0.01));
			m_explore_info_list[i].jingling_id = LOGIC_CONFIG->GetJingLingCfg().RandJingLingId();
			if (other_cfg->explore_name_count > 0)
			{
				m_explore_info_list[i].name_id = RandomNum(1, other_cfg->explore_name_count + 1);
			}
			m_explore_info_list[i].reward_times = 0;
		}
	}

	this->SendExploreInfo(Protocol::JL_EXPLORE_INFO_REASON_SELECT);
}

void JingLingManager::OnExploreChallenge()
{
	if (0 == m_explore_maxhp)
	{
		return;
	}

	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (!m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_JING_LING_EXPLORE_CHALLENGE, other_cfg->explore_times))
	{
		m_role->NoticeNum(errornum::EN_JINGLING_EXPLORE_TIMES_NOT_ENOUGH);
		return;
	}

	int level = -1;
	for (int i = 0; i < JING_LING_EXPLORE_LEVEL_COUNT; i++)
	{
		if (0 != m_explore_info_list[i].hp)
		{
			level = i;
			break;
		}
	}

	int self_capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING);
	self_capability += self_capability * m_explore_buy_buff_count * other_cfg->explore_buff_add_per / 100;

	if (level < 0 || level >= JING_LING_EXPLORE_LEVEL_COUNT || self_capability <= 0)
	{
		return;
	}

	m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_JING_LING_EXPLORE_CHALLENGE);

	int reason = Protocol::JL_EXPLORE_INFO_REASON_CHALLENGE_SUCC;

	double ratio = m_explore_info_list[level].capability * 1.0 / self_capability;
	if (ratio >= 1.0)
	{
		ratio += (RandomNum(31) * 1.0) / 100;
	}
	else
	{
		ratio -= (RandomNum(31) * 1.0) / 100;
		if (ratio < 0.0)
		{
			ratio = 0.0;
		}
	}

	int enemy_hp = (int)(ratio * m_explore_info_list[level].hp);
	if (m_explore_hp >= enemy_hp)
	{
		m_explore_info_list[level].hp = 0;
		m_explore_hp = (m_explore_hp - enemy_hp) + m_explore_hp * 5 / 100;
		if (m_explore_hp > m_explore_maxhp) m_explore_hp = m_explore_maxhp;
		if (m_explore_hp <= 0) m_explore_hp = 1;
	}
	else
	{
		reason = Protocol::JL_EXPLORE_INFO_REASON_CHALLENGE_FAIL;

		m_explore_info_list[level].hp = (int)(m_explore_info_list[level].hp - m_explore_hp / (ratio > 0.0 ? ratio : 1.0));
		if (m_explore_info_list[level].hp <= 0) m_explore_info_list[level].hp = 1;

		m_explore_maxhp = (int)(self_capability * (other_cfg->explore_hp_percent * 0.01));
		m_explore_hp = m_explore_maxhp;
	}

	this->SendExploreInfo(reason);
}

void JingLingManager::OnExploreFetchReward(short level, bool is_buy)
{
	if (level < 0 || level >= JING_LING_EXPLORE_LEVEL_COUNT)
	{
		return;
	}

	const JingLingExploreCfg *explore_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingExploreCfg(m_explore_mode, level);
	if (NULL == explore_cfg)
	{
		return;
	}

	if (m_explore_maxhp <= 0 || m_explore_info_list[level].hp > 0 || m_explore_info_list[level].capability <= 0)
	{
		m_role->NoticeNum(errornum::EN_JINGLING_EXPLORE_NOT_PASS_LEVEL);
		return;
	}

	const JingLingExploreCfg::RewardCfg *reward_cfg = explore_cfg->GetReward(m_role->GetLevel());
	if (NULL == reward_cfg)
	{
		return;
	}

	int cost_gold = 0;
	if (m_explore_info_list[level].reward_times >= explore_cfg->free_times)
	{
		if (!is_buy)
		{
			return;
		}

		int index = m_explore_info_list[level].reward_times - explore_cfg->free_times;
		if (index < 0 || index >= JING_LING_EXPLORE_GOLE_FETCH_COUNT)
		{
			return;
		}

		cost_gold = explore_cfg->fetch_gold_list[index];
	}

	if (cost_gold > 0)
	{
		if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(cost_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(reward_cfg->item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (cost_gold > 0)
	{
		m_role->GetKnapsack()->GetMoney()->UseGold(cost_gold, "JingLingExplore");
	}

	m_explore_info_list[level].reward_times++;

	m_role->GetKnapsack()->PutList(reward_cfg->item_count, reward_cfg->reward_item_list, PUT_REASON_JING_LING_EXPLORE);

	this->SendExploreInfo(Protocol::JL_EXPLORE_INFO_REASON_FETCH);
}

void JingLingManager::OnExploreReset()
{
	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (!m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_JING_LING_EXPLORE_RESET, other_cfg->explore_other_buy))
	{
		return;
	}

	if (other_cfg->explore_other_buy_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg->explore_other_buy_gold, "JingLingExploreReset"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_JING_LING_EXPLORE_RESET);
	m_role->GetDayCounter()->DayCountReset(DayCounter::DAYCOUNT_ID_JING_LING_EXPLORE_CHALLENGE);

	m_explore_maxhp = 0;
	m_explore_hp = 0;
	m_explore_mode = 0;
	memset(m_explore_info_list, 0, sizeof(m_explore_info_list));

	this->SendExploreInfo(Protocol::JL_EXPLORE_INFO_REASON_RESET);
}

void JingLingManager::OnExploreBuyBuff()
{
	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (m_explore_buy_buff_count >= other_cfg->explore_buff_max_count)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg->explore_buff_buy_gold, "JingLingExploreBuyBuff"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_explore_buy_buff_count++;

	this->SendExploreInfo(Protocol::JL_EXPLORE_INFO_REASON_BUY_BUFF);
}

short JingLingManager::RandJingLingSkill(const JingLingSkillRefreshCfg *refresh_cfg)
{
	if (refresh_cfg->total_weight <= 0)
	{
		return 0;
	}

	int rand_num = RandomNum(refresh_cfg->total_weight);
	for (int i = 0; i < refresh_cfg->cfg_count && i < JING_LING_SKILL_REFRESH_RATE_MAX; i++)
	{
		if (rand_num < refresh_cfg->cfg_list[i].weight)
		{
			return (short)refresh_cfg->cfg_list[i].skill_id;
		}

		rand_num -= refresh_cfg->cfg_list[i].weight;
	}

	return 0;
}

int JingLingManager::GetSkillNum(ItemID jingling_id, int jingling_level, int wuxing_level, int color)
{
	int skill_num = 0;
	const JingLingTalentAttrItemCfg *talent_cfg = LOGIC_CONFIG->GetJingLingCfg().GetTalentAttrCfg(jingling_id);
	if (NULL != talent_cfg)
	{
		skill_num += talent_cfg->skill_num;
	}

	const JingLingItemCfg *item_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingItemCfg(jingling_id, jingling_level);
	if (NULL != item_cfg)
	{
		skill_num += item_cfg->skill_num;
	}

	const JingLingWuXingCfg *wuxing_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingWuXingCfg(color, wuxing_level);
	if (NULL != wuxing_cfg)
	{
		skill_num += wuxing_cfg->skill_num;
	}

	return skill_num;
}

void JingLingManager::OnLogin()
{
	/*for (int i = 0; i < 4; i++)
	{
		JingLingRefreshItem &refresh_item = m_skill_refresh_item_list[i];
		const JingLingSkillRefreshCfg *refresh_cfg = LOGIC_CONFIG->GetJingLingCfg().GetJingLingSkillRefreshCfg(refresh_item.refresh_count);
		if (NULL == refresh_cfg)
		{
			return;
		}

		if (m_skill_refresh_item_list[i].skill_list[0] == 0)
		{
			refresh_item.skill_list[0] = this->RandJingLingSkill(refresh_cfg);
		}
	}*/
	this->CheckActiveHalo(false);
	this->CheckFuncOpen();
	this->SyncJingLingInfo();
}

void JingLingManager::OnRoleLevelChange(int level)
{
	this->CheckFuncOpen();
}

void JingLingManager::OnCompleteTask(TaskID task_id)
{
	this->CheckFuncOpen();
}

void JingLingManager::CheckFuncOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (m_role->GetRoleBigSmallGoal()->IsFuncActive(ROLE_BIG_SMALL_GOAL_SYSTEM_XIANCHONG))
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_XIANCHONG);
	if (fun_open_cfg != nullptr && opengame_day >= fun_open_cfg->opengame_day)
	{
		if (m_role->GetLevel() >= fun_open_cfg->role_level || m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
		{
			m_role->GetRoleBigSmallGoal()->OnOpenFunc(ROLE_BIG_SMALL_GOAL_SYSTEM_XIANCHONG);
		}
	}
}

// 幻化特殊战力加成
void JingLingManager::HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
{
	if (!LOGIC_CONFIG->GetJingLingCfg().IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = LOGIC_CONFIG->GetJingLingCfg().GetHuanHuaSpecialCapAddCfg(huanhua_id);
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

void JingLingManager::OnOpenSkillSlot(int jingling_index, int slot_index)
{
	if (jingling_index < 0 || jingling_index >= JINGLING_MAX_TAKEON_NUM
		|| slot_index < 0 || slot_index >= JING_LING_SKILL_COUNT_MAX)
	{
		return;
	}

	ItemGridData *grid_data = &m_jingling_list[jingling_index];
	if (grid_data->Invalid())
	{
		return;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()
		|| Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
	{
		return;
	}

	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (NULL == other_cfg) return;

	EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;

	if (0 != equip_param->jing_ling_skill_list[slot_index].is_open) // 已开启
	{
		return;
	}

	const int gold = LOGIC_CONFIG->GetJingLingCfg().GetSkillSlotOpenConsume(slot_index);
	if (slot_index >= other_cfg->skill_solt_free_count && gold <= 0)
	{
		return;
	}

	if (slot_index >= other_cfg->skill_solt_free_count && !m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (slot_index >= other_cfg->skill_solt_free_count && !m_role->GetKnapsack()->GetMoney()->UseAllGold(gold, "JingLingManager::OnOpenSkillSlot"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	equip_param->jing_ling_skill_list[slot_index].is_open = 1;
	grid_data->is_bind = true;
	this->SyncJingLingInfo();
}