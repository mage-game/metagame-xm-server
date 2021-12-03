#include "chinesezodiac.hpp"
#include "obj/character/role.h"
#include "obj/character/attribute.hpp"

#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/noticenum.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "world.h"


#include "config/logicconfigmanager.hpp"
#include "other/fb/dailyfbconfig.hpp"
#include "other/chinesezodiac/chinesezodiacconfig.hpp"

#include "item/itemextern.hpp"
#include "item/knapsack.h"
#include "other/event/eventhandler.hpp"
#include "other/capability/capability.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "gameworld/item/itempool.h"
#include "gameworld/equipment/equipment.h"
#include "other/lotterycount/lotterycount.hpp"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "config/joinlimitconfig.hpp"
#include "task/taskrecorder.h"
#include "servercommon/serverconfig/crossconfig.hpp"

#include <algorithm>

ChineseZodiac::ChineseZodiac() : m_role(NULL), m_other_capbility(0), m_miji_put_notice(0)
{
	memset(m_equip_quality_count, 0, sizeof(m_equip_quality_count));

	for (int i = 0; i < TIAN_XIANG_CURR_CHAPTER_NUM; i++)
	{
		for (int j = 0; j < TIAN_XIANG_ALL_CHAPTER_COMBINE; j++)
		{
			m_active_list[i][j] = 0;
		}
	}
}

ChineseZodiac::~ChineseZodiac()
{
}

void ChineseZodiac::InitParam(Role *role, const ChineseZodiacParam &param)
{
	m_role = role;
	m_param = param;

	for (int i = m_param.curr_chapter; i >= 0; i--)
	{
		this->CheckCombineAll(i, true);	
	}
}

void ChineseZodiac::GetInitParam(ChineseZodiacParam *param)
{
	*param = m_param;
}

void ChineseZodiac::OnRoleLogin()
{
	this->CheckFuncOpen();
	this->SendXinglingInfo();
}

int ChineseZodiac::GetTotalLevel()
{
	int total_level = 0;
	for (int zodiac_type = 0; zodiac_type < CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT; ++zodiac_type)
	{
		total_level += m_param.zodiac_level_list[zodiac_type];
	}

	return total_level;
}

int ChineseZodiac::GetUpLevelCount(int level)
{
	int count = 0;
	for (int zodiac_type = 0; zodiac_type < CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT; ++zodiac_type)
	{
		if (m_param.zodiac_level_list[zodiac_type] >= level)
		{
			count++;
		}
	}

	return count;
}

int ChineseZodiac::GetActiveCount(int color)
{
	int count = 0;
	for (unsigned int zodiac_type = 0; zodiac_type < static_array_size(m_param.zodiac_level_list); zodiac_type ++)
	{
		if (m_param.zodiac_level_list[zodiac_type] <= 0)
		{
			continue;
		}

		const ChineseZodiacSingleAttr::SingleType *single_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetSingleAttr(zodiac_type, m_param.zodiac_level_list[zodiac_type]);
		if (NULL != single_cfg)
		{
			if (single_cfg->color >= color)
			{
				count ++;
			}
		}
	}

	return count;
}

void ChineseZodiac::PromoteXingHun(int zodiac_type, bool is_auto_buy, bool is_use_promote_item)
{
	if (zodiac_type < 0 || zodiac_type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
	{
		return;
	}

	if (m_param.xinghun_level_list[zodiac_type] >= CHINESE_ZODIAC_XINGHUN_LEVEL_MAX)
	{
		m_role->NoticeNum(errornum::EN_CHINESE_ZODIAC_XINGHUN_LEVEL_LIMIT);
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	const ChineseZodiacXingHunCfg *xinghun_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetChineseZodiacXingHunCfg(zodiac_type, m_param.xinghun_level_list[zodiac_type]);
	if (NULL == xinghun_cfg)
	{
		return;
	}

	if (zodiac_type > 0 && m_param.xinghun_level_max_list[zodiac_type - 1] < xinghun_cfg->backwards_highest_level)
	{
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	{
		// 获取可消耗的背包材料列表
		short stuff_count = 0;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = xinghun_cfg->consume_stuff_id;
		stuff_list[stuff_count].num = xinghun_cfg->consume_stuff_num;
		stuff_list[stuff_count].buyable = is_auto_buy;
		++stuff_count;

		if (is_use_promote_item)
		{
			stuff_list[stuff_count].item_id = xinghun_cfg->protect_item_id;
			stuff_list[stuff_count].num = xinghun_cfg->protect_item_num;
			stuff_list[stuff_count].buyable = false;
			++stuff_count;
		}

		// 检查
		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "ChineseZodiac::PromoteXingHun"))
		{
			return;
		}
	}

	int old_mount_add_per = 0, old_wing_add_per = 0, old_halo_add_per = 0, old_shengong_add_per = 0, old_shenyi_add_per = 0, old_fight_mount_add_per = 0, old_foot_print_add_per = 0;
	this->GetXingHunTotalAddPer(&old_mount_add_per, &old_wing_add_per, &old_halo_add_per, &old_shengong_add_per, &old_shenyi_add_per, &old_fight_mount_add_per, &old_foot_print_add_per);

	int rand_value = RandomNum(100);
	if (rand_value < xinghun_cfg->succ_percent)
	{
		++m_param.xinghun_level_list[zodiac_type];

		if (m_param.xinghun_level_list[zodiac_type] >= CHINESE_ZODIAC_XINGHUN_LEVEL_MAX)
		{
			m_param.xinghun_baoji_value_list[zodiac_type] = 0;
		}
		else
		{
			if (static_cast<int>(m_param.xinghun_baoji_value_list[zodiac_type] >= xinghun_cfg->max_baoji_value))
			{
				++m_param.xinghun_level_list[zodiac_type];
				m_param.xinghun_baoji_value_list[zodiac_type] = 0;
			}
		}
	}
	else
	{
		if (0 == xinghun_cfg->is_protect_level && 0 == is_use_promote_item)
		{
			if (m_param.xinghun_level_list[zodiac_type] >= 1)
			{
				--m_param.xinghun_level_list[zodiac_type];
			}
		}

		const ChineseZodiacXingHunCfg *fail_level_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetChineseZodiacXingHunCfg(zodiac_type, m_param.xinghun_level_list[zodiac_type]);
		if (NULL != fail_level_cfg)
		{
			m_param.xinghun_baoji_value_list[zodiac_type] += xinghun_cfg->faild_add_baoji_value;
			if (m_param.xinghun_baoji_value_list[zodiac_type] > fail_level_cfg->max_baoji_value)
			{
				m_param.xinghun_baoji_value_list[zodiac_type] = fail_level_cfg->max_baoji_value;
			}
		}
		
		m_role->NoticeNum(errornum::EN_CHINESE_ZODIAC_XINGHUN_LEVEL_UPGRADE_FAIL);
	}

	if (m_param.xinghun_level_list[zodiac_type] > m_param.xinghun_level_max_list[zodiac_type])
	{
		m_param.xinghun_level_max_list[zodiac_type] = m_param.xinghun_level_list[zodiac_type];
	}

	this->SendChineseZodiacAllInfo();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHINESE_ZODIAC);

	int mount_add_per = 0, wing_add_per = 0, halo_add_per = 0, shengong_add_per = 0, shenyi_add_per = 0, fight_mount_add_per = 0, foot_print_add_per = 0;
	this->GetXingHunTotalAddPer(&mount_add_per, &wing_add_per, &halo_add_per, &shengong_add_per, &shenyi_add_per, &fight_mount_add_per, &foot_print_add_per);

	if (mount_add_per != old_mount_add_per)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);
	}
	if (wing_add_per != old_wing_add_per)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);
	}
	if (halo_add_per != old_halo_add_per)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
	}
	if (shengong_add_per != old_shengong_add_per)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGONG);
	}
	if (shenyi_add_per != old_shenyi_add_per)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);
	}
	if (fight_mount_add_per != old_fight_mount_add_per)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
	}
	if (foot_print_add_per != old_foot_print_add_per)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);
	}

	gamelog::g_log_chinese_zodiac.printf(LL_INFO, "ChineseZodiac::PromoteXingHun user[%d %s] zodiac_type[%d] zodiac_level[%d] xinghun_level[%d], consume_gold:[%d] is_use_promote_item[%d]",
		m_role->GetUID(), m_role->GetName(), zodiac_type, m_param.zodiac_level_list[zodiac_type], m_param.xinghun_level_list[zodiac_type], consume_list.need_gold_num, (int)is_use_promote_item);
}

void ChineseZodiac::GetXingHunTotalAddPer(int *mount_per, int *wing_per, int *halo_per, int *shengong_per, int *shenyi_per, int *fightmount_per, int *footprint_per)
{
	for (int zodiac_type = 0; zodiac_type < CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT; ++zodiac_type)
	{
		int xinghun_level = m_param.xinghun_level_list[zodiac_type];
		if (m_param.xinghun_level_list[zodiac_type] >= 0)
		{
			const ChineseZodiacXingHunCfg *xinghun_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetChineseZodiacXingHunCfg(zodiac_type, xinghun_level);
			if (NULL != xinghun_cfg)
			{
				if (NULL != mount_per) *mount_per += xinghun_cfg->mount_add;
				if (NULL != wing_per) *wing_per += xinghun_cfg->wing_add;
				if (NULL != halo_per) *halo_per += xinghun_cfg->halo_add;
				if (NULL != shengong_per) *shengong_per += xinghun_cfg->shengong_add;
				if (NULL != shenyi_per) *shenyi_per += xinghun_cfg->shenyi_add;
				if (NULL != fightmount_per) *fightmount_per += xinghun_cfg->fightmount_add;
				if (NULL != footprint_per) *footprint_per += xinghun_cfg->footprint_add;
			}
		}
	}
}

void ChineseZodiac::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		this->ReCalcSuitCount();

		m_attrs_add.Reset();
		m_special_buff_list.clear();
		m_other_capbility = 0;

		int min_xinghun_level = 0;

		for (int zodiac_type = 0; zodiac_type < CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT; ++ zodiac_type)
		{
			{
				int zodiac_level = m_param.zodiac_level_list[zodiac_type];

				if (zodiac_level < 0)
				{
					continue;
				}
				
				const ChineseZodiacSingleAttr::SingleType *single_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetSingleAttr(zodiac_type, zodiac_level);
				if (NULL != single_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += single_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += single_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += single_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += single_cfg->baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += single_cfg->jianren;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += single_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += single_cfg->shanbi;
				}
			}

			{
				int suit_color = ItemBase::I_COLOR_PINK;
				for(int equip_type = 0; equip_type < CHINESE_ZODIAC_CUR_EQUIP_SLOT_MAX_LIMIT; equip_type++)
				{
					//装备
					int equip_item = m_param.chinesezodiac_equip_list[zodiac_type].equip_Item_list[equip_type];
					
					if (equip_item > 0)
					{
						//int ChineseZodiacColor = LOGIC_CONFIG->GetChineseZodiacCfg().GetColorCfg(zodiac_type);
						//const ChineseZodiacEquipAttr *equip_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetEquigpAttr(equip_type, equip_level);
						//const ChineseZodiacEquipAttr *equip_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetEquigpAttr(ChineseZodiacColor, equip_type, equip_level);
						const Equipment* equip = dynamic_cast<const Equipment*>(ITEMPOOL->GetItem(equip_item));
						if (NULL != equip)
						{
							CharIntAttrs get_attr;
							equip->GetBaseAttr(get_attr);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += get_attr.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += get_attr.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += get_attr.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += get_attr.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += get_attr.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += get_attr.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += get_attr.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI);
							if (suit_color > equip->GetColor())
							{
								suit_color = equip->GetColor();
							}
						}
						else
						{
							suit_color = 0;
						}
					}
					else
					{
						suit_color = 0;
					}
				}

				//if (suit_color > 0)
				//{
				//	const ChineseZodiacEquipSuitAttr * suit_attr = LOGIC_CONFIG->GetChineseZodiacCfg().GetEquipSuitAttr(zodiac_type, suit_color);
				//	if(nullptr != suit_attr)
				//	{
				//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_attr->maxhp;
				//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_attr->gongji;
				//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += suit_attr->fangyu;
				//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += suit_attr->baoji;
				//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += suit_attr->jianren;
				//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += suit_attr->mingzhong;
				//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += suit_attr->shanbi;
				//	}
				//}
			}

			//（新）套装
			{
				for (int i = 0; i < ItemBase::I_COLOR_MAX; i++)
				{
					const ChineseZodiacEquipSuitAttr * suit_attr = LOGIC_CONFIG->GetChineseZodiacCfg().GetNewSuitCfg(zodiac_type, i, m_equip_quality_count[zodiac_type][i]);
					if (suit_attr != NULL)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_attr->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_attr->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += suit_attr->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += suit_attr->baoji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += suit_attr->jianren;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += suit_attr->mingzhong;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += suit_attr->shanbi;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO] += suit_attr->per_kangbao;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += suit_attr->per_baoji_hurt;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI] += suit_attr->per_baoji;
					}
				}
			}

			//{
			//	// 秘籍
			//	for(int kong = 0; kong < MIJI_KONG_NUM; kong++)
			//	{
			//		int miji_index = m_param.miji_list[zodiac_type].single_miji_list[kong];
			//		if (miji_index >= 0)
			//		{
			//			const ChineseZodiacMiji * kong_miji_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiCfgByIndex(miji_index);
			//			if (NULL != kong_miji_cfg)
			//			{
			//				int real_type;
			//				if (this->TransformAttrType(kong_miji_cfg->type, real_type))
			//				{
			//					m_attrs_add.m_attrs[real_type] += kong_miji_cfg->value;
			//					m_other_capbility += kong_miji_cfg->capacity;
			//				}
			//				else
			//				{
			//					this->ActiveSpecialEffect(kong_miji_cfg->type, kong_miji_cfg->value);
			//					m_other_capbility += kong_miji_cfg->capacity;
			//				}
			//			}
			//		}
			//	}
			//}

			{
				int xinghun_level = m_param.xinghun_level_list[zodiac_type];

				if (m_param.xinghun_level_list[zodiac_type] >= 0)
				{
					min_xinghun_level += xinghun_level;
					
					//生肖星魂属性
					const ChineseZodiacXingHunCfg *xinghun_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetChineseZodiacXingHunCfg(zodiac_type, xinghun_level);
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
			}
		}

		// 生肖星魂套属性
		const ChineseZodiacXingHunExtraAttrCfg *xinghun_attr_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetChineseZodiacXingHunAttrCfg(min_xinghun_level);
		if (NULL != xinghun_attr_cfg)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += xinghun_attr_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += xinghun_attr_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += xinghun_attr_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += xinghun_attr_cfg->baoji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += xinghun_attr_cfg->jianren;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += xinghun_attr_cfg->mingzhong;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += xinghun_attr_cfg->shanbi;
		}

		{
			// 星座总等级属性
			const ChineseZodiacSuitAttr *suit_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetSuitAttr(this->GetZoiacTotalLevel());
			if (NULL != suit_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += suit_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += suit_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += suit_cfg->jianren;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += suit_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += suit_cfg->shanbi;
			}
		}
			
		{
			// 星图基础属性
			if (m_param.curr_chapter >= TIAN_XIANG_CHAPTER_NUM) return;
			for (int i = 0; i <= m_param.curr_chapter; i++)
			{
				for (int y = 0; y < TIAN_XIANG_TABEL_ROW_COUNT; ++y)
				{
					for (int x = 0; x < TIAN_XIANG_TABEL_MIDDLE_GRIDS; ++x)
					{
						const TianXiangTableGridParam &tmp_bead = m_param.chapter_lsit[i].table_lsit[x][y];
						if (tmp_bead.IsEmpty() || !this->IsValidPosi(x, y))
						{
							continue;
						}

						const TianXiangBeadCfg *bead_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetBeadCfg(tmp_bead.type);
						if (NULL != bead_cfg)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += bead_cfg->attr.maxhp;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += bead_cfg->attr.gongji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += bead_cfg->attr.fangyu;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += bead_cfg->attr.mingzhong;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += bead_cfg->attr.shanbi;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += bead_cfg->attr.baoji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += bead_cfg->attr.jianren;
						}
					}
				}
			}

			// 星图组合、章节加成
			for (int chapter = 0; chapter <= m_param.curr_chapter && chapter < TIAN_XIANG_CURR_CHAPTER_NUM; chapter++)
			{
				double curr_chapger_add_attr_per = 0;
				Attribute curr_chapter_jingzhun = 0;
				for (int i = 0; i < TIAN_XIANG_ALL_CHAPTER_COMBINE; i++)
				{
					if (0 == m_active_list[chapter][i])
					{
						break;
					}

					if (i == TIAN_XIANG_ALL_CHAPTER_COMBINE - 1)
					{
						const ChapterAttributesConfig * chapter_attr_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetChapterAttr(chapter);
						if (NULL == chapter_attr_cfg)
						{
							break;
						}
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += chapter_attr_cfg->attr.maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += chapter_attr_cfg->attr.gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += chapter_attr_cfg->attr.fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += chapter_attr_cfg->attr.mingzhong;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += chapter_attr_cfg->attr.shanbi;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += chapter_attr_cfg->attr.baoji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += chapter_attr_cfg->attr.jianren;

						curr_chapger_add_attr_per = chapter_attr_cfg->per_attr * ROLE_ATTR_PER;
						curr_chapter_jingzhun += chapter_attr_cfg->per_jingzhun;
					}
				}

				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += curr_chapter_jingzhun;
				
				XingLingParam &xingling_param = m_param.xingling_lsit[chapter];
				const XingLingCfg::ConfigItem * curr_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetXinglingCfg(chapter, xingling_param.level);
				if (NULL != curr_cfg)
				{
					curr_chapger_add_attr_per += curr_cfg->xingtu_add_prob * 0.0001;
				}

				for (int i = 0; i < TIAN_XIANG_ALL_CHAPTER_COMBINE; i++)
				{
					if (1 == m_active_list[chapter][i])
					{
						int real_seq = chapter * TIAN_XIANG_ALL_CHAPTER_COMBINE + i;
						const TianXiangCombineAttrCfg *combine_attr_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetCombineAttrCfg(real_seq);
						if (NULL != combine_attr_cfg)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += combine_attr_cfg->attr.maxhp + static_cast<Attribute>(combine_attr_cfg->attr.maxhp * curr_chapger_add_attr_per);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += combine_attr_cfg->attr.gongji + static_cast<Attribute>(combine_attr_cfg->attr.gongji * curr_chapger_add_attr_per);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += combine_attr_cfg->attr.fangyu + static_cast<Attribute>(combine_attr_cfg->attr.fangyu * curr_chapger_add_attr_per);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += combine_attr_cfg->attr.mingzhong + static_cast<Attribute>(combine_attr_cfg->attr.mingzhong * curr_chapger_add_attr_per);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += combine_attr_cfg->attr.shanbi + static_cast<Attribute>(combine_attr_cfg->attr.shanbi * curr_chapger_add_attr_per);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += combine_attr_cfg->attr.baoji + static_cast<Attribute>(combine_attr_cfg->attr.baoji * curr_chapger_add_attr_per);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += combine_attr_cfg->attr.jianren + static_cast<Attribute>(combine_attr_cfg->attr.jianren * curr_chapger_add_attr_per);
						}
					}
				}
			}

// 			// 星途完成的章节组合加成
// 			for (int i = 0; i <  m_param.curr_chapter; i++)
// 			{
// 				const ChapterAttributesConfig * chapter_attr_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetChapterAttr(i);
// 				if (NULL == chapter_attr_cfg)
// 				{
// 					continue;
// 				}
// 				
// 				double add_attr_per = chapter_attr_cfg->per_attr * 0.01;
// 
// 				XingLingParam &xingling_param = m_param.xingling_lsit[i];
// 				const XingLingCfg::ConfigItem * curr_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetXinglingCfg(i, xingling_param.level);
// 				if (NULL != curr_cfg)
// 				{
// 					add_attr_per += curr_cfg->xingtu_add_prob * 0.0001;
// 				}
// 
// 				for (int j = 0; j <  TIAN_XIANG_ALL_CHAPTER_COMBINE; j++)
// 				{
// 					int real_seq = i * TIAN_XIANG_ALL_CHAPTER_COMBINE + j;
// 
// 					const TianXiangCombineAttrCfg *combine_attr_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetCombineAttrCfg(real_seq);
// 					if (NULL != combine_attr_cfg)
// 					{
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += combine_attr_cfg->attr.maxhp + static_cast<int>(combine_attr_cfg->attr.maxhp * add_attr_per);
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += combine_attr_cfg->attr.gongji + static_cast<int>(combine_attr_cfg->attr.gongji * add_attr_per);
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += combine_attr_cfg->attr.fangyu + static_cast<int>(combine_attr_cfg->attr.fangyu * add_attr_per);
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += combine_attr_cfg->attr.mingzhong + static_cast<int>(combine_attr_cfg->attr.mingzhong * add_attr_per);
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += combine_attr_cfg->attr.shanbi + static_cast<int>(combine_attr_cfg->attr.shanbi * add_attr_per);
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += combine_attr_cfg->attr.baoji + static_cast<int>(combine_attr_cfg->attr.baoji * add_attr_per);
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += combine_attr_cfg->attr.jianren + static_cast<int>(combine_attr_cfg->attr.jianren * add_attr_per);
// 					}
// 				}
// 			}
		}

		{
			// 星灵基础属性
			for (int i = 0; i <  TIAN_XIANG_CURR_CHAPTER_NUM; i++)
			{
				XingLingParam &xingling_param = m_param.xingling_lsit[i];
				const XingLingCfg::ConfigItem * curr_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetXinglingCfg(i, xingling_param.level);
				if (NULL == curr_cfg)
				{
					continue;
				}

				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += curr_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += curr_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += curr_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += curr_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += curr_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += curr_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += curr_cfg->kangbao;
			}
		}
		// 大小目标固定属性加成
		m_role->GetRoleBigSmallGoal()->RoleBigSmallGoalAddSpecialAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_CHINESE_ZODIAC, m_attrs_add);
		this->ReCalcAttrBigSmallGoalAddPer(m_attrs_add);
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_CHINESE_ZODIAC, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_CHINESE_ZODIAC, m_other_capbility);

	base_add.Add(m_attrs_add.m_attrs, true);
}

void ChineseZodiac::ReCalcAttrBigSmallGoalAddPer(CharIntAttrs & attr_add)
{
	CharIntAttrs zodiac_attr;
	//计算所有生肖精魄的属性
	for (int zodiac_type = 0; zodiac_type < CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT; ++zodiac_type)
	{
		{
			int zodiac_level = m_param.zodiac_level_list[zodiac_type];

			if (zodiac_level < 0)
			{
				continue;
			}

			const ChineseZodiacSingleAttr::SingleType *single_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetSingleAttr(zodiac_type, zodiac_level);
			if (NULL != single_cfg)
			{
				zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += single_cfg->maxhp;
				zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += single_cfg->gongji;
				zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += single_cfg->fangyu;
				zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += single_cfg->baoji;
				zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += single_cfg->jianren;
				zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += single_cfg->mingzhong;
				zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += single_cfg->shanbi;
			}
		}
	}
	//大小目标万分比加成
	int add_value = m_role->GetRoleBigSmallGoal()->GetRoleBigSmallGoalAddAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_CHINESE_ZODIAC);
	//最终提供的属性 = 强化总属性 * 加成
	if (add_value > 0)
	{
		zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] = static_cast<Attribute>(zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (add_value * ROLE_ATTR_PER));
		zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] = static_cast<Attribute>(zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (add_value * ROLE_ATTR_PER));
		zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] = static_cast<Attribute>(zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (add_value * ROLE_ATTR_PER));
		zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] = static_cast<Attribute>(zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (add_value * ROLE_ATTR_PER));
		zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] = static_cast<Attribute>(zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (add_value * ROLE_ATTR_PER));
		zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] = static_cast<Attribute>(zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (add_value * ROLE_ATTR_PER));
		zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] = static_cast<Attribute>(zodiac_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (add_value * ROLE_ATTR_PER));
		
		//加到系统属性上
		attr_add.Add(zodiac_attr.m_attrs);
	}
}

bool ChineseZodiac::TransformAttrType(int type, int &real_type)
{
	switch (type)
	{
	case MIJI_ARRT_MAXHP:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP;
		}
		break;

	case MIJI_ARRT_GONGJI:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI;
		}
		break;

	case MIJI_ARRT_FANGYU:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU;
		}
		break;

	case MIJI_ARRT_BAOJI:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI;
		}
		break;

	case MIJI_ARRT_KANGBAO:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN;
		}
		break;

	case MIJI_ARRT_MINGZHONG:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG;
		}
		break;
	case MIJI_ARRT_SHANBI:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI;
		}
		break;
	case MIJI_ARRT_NVSHENGONGJI:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI;
		}
		break;

	case MIJI_ARRT_GD_ZENGSHANG:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG;
		}
		break;

	case MIJI_ARRT_GD_MIANFANG:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG;
		}
		break;

	default:
		return false;
	}

	return true;
}

void ChineseZodiac::ActiveSpecialEffect(int effect_type, int value)
{
	if (effect_type < MIJI_SPECIAL_TYPE_MIN || effect_type >= MIJI_SPECIAL_TYPE_MAX || value <= 0)
	{
		return;
	}

	m_special_buff_list[static_cast<short>(effect_type)] += value;
}

int ChineseZodiac::GetHuiXuePercent()
{
	return static_cast<int>(this->GetSpecialEffectValuPercentByType(MIJI_ARRT_HUIXUE_PER));
}

int ChineseZodiac::GetSkillCoolDownShuoduan()
{
	return static_cast<int>(this->GetSpecialEffectValuPercentByType(MIJI_ARRT_JINENG_COOLCOWN_SUODUAN_PER));
}

int ChineseZodiac::GetSkillXishuUp()
{
	return static_cast<int>(this->GetSpecialEffectValuPercentByType(MIJI_ARRT_JINENG_XISHU_PER));
}

int ChineseZodiac::GetSpecialEffectValuPercentByType(int effect_type)
{
	std::map<short, int>::iterator it_map = m_special_buff_list.find(effect_type);
	if (it_map != m_special_buff_list.end())
	{
		return it_map->second;
	}

	return 0;
}

void ChineseZodiac::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;
	m_param.today_ggl_free_times = 0;
	m_param.last_free_ggl_time = 0;
	this->CheckFuncOpen();
}

int ChineseZodiac::GetZoiacTotalLevel()
{
	int total_level = 0;
	for (int i = 0; i < CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT; i++)
	{
		total_level += m_param.zodiac_level_list[i];
	}

	return total_level;
}

bool ChineseZodiac::PromoteZodiac(int zodiac_type, short is_auto_buy)
{
	if (zodiac_type < 0 || zodiac_type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
	{
		return false;
	}
	
	const ChineseZodiacSingleAttr::SingleType *single_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetSingleAttr(zodiac_type, m_param.zodiac_level_list[zodiac_type] + 1);
	if (NULL == single_cfg)
	{
		return false;
	}

	if (zodiac_type != 0)
	{
		const ChineseZodiacSingleAttr::SingleType *last_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetSingleAttr(zodiac_type - 1, 1);
		if (NULL == last_cfg)
		{
			return false;
		}

		if (m_param.zodiac_level_list[zodiac_type - 1] < single_cfg->level_limit)
		{
			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xingzuo_update_limit, last_cfg->name, single_cfg->level_limit);

			if (len > 0) 
			{
				int sendlen = 0;
				static Protocol::SCSystemMsg sm;

				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_PERSONAL_NOTICE))
				{
					EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sm, sizeof(sm));
				}
			}
			
			return false;
		}
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	{
		static ItemExtern::ItemConsumeList consume_list; 
		consume_list.Reset();

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = single_cfg->item_id;
		stuff_list[0].num = single_cfg->expend;
		stuff_list[0].buyable = 1 == is_auto_buy ? true : false;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
		{
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "ChineseZodiac::PromoteZodiac"))
		{
			return false;
		}
	}

	if (m_param.zodiac_level_list[zodiac_type] >= CHINESE_ZODIAC_LEVEL_MAX_LIMIT)
	{
		m_role->NoticeNum(errornum::EN_CHINESE_ZODIAC_LEVEL_LIMIT);
		return false;
	}

// 	int old_suit_level = 0;
// 
// 	const ChineseZodiacSuitAttr *suit_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetSuitAttr(this->GetZoiacTotalLevel());
// 	if (NULL != suit_cfg)
// 	{
// 		old_suit_level = suit_cfg->level;
// 	}

	++ m_param.zodiac_level_list[zodiac_type];

	{
		// 传闻
// 		suit_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetSuitAttr(this->GetZoiacTotalLevel());
// 		if (NULL != suit_cfg && suit_cfg->level > old_suit_level)
// 		{
// 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xingzuo_taozhuang,
// 				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), suit_cfg->level, suit_cfg->level);
// 			if (length > 0) 
// 			{
// 				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
// 			}
// 		}
	}

	m_role->NoticeNum(noticenum::NT_CHINESE_ZODIAC_UP_LEVEL_SUCC);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHINESE_ZODIAC);
	m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_CHINESE_ZODIAC, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_3, zodiac_type);
	
	EventHandler::Instance().OnChineseZodiacUpLevel(m_role);

	this->SendChineseZodiacAllInfo(zodiac_type);

	gamelog::g_log_chinese_zodiac.printf(LL_INFO, "ChineseZodiac::OnUseSoul user[%d %s] zodiac_type[%d] level[%d]",
		m_role->GetUID(), m_role->GetName(), zodiac_type, m_param.zodiac_level_list[zodiac_type]);

	return true;
}

int ChineseZodiac::PutOnEquip(ItemDataWrapper & item_wrapper,int zodiac_type)
{
	if (zodiac_type < 0 || zodiac_type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
	{
		return -1;
	}

	if (m_param.zodiac_level_list[zodiac_type] < 1)					//未激活的生肖不能穿装备
	{
		m_role->NoticeNum(errornum::EN_CHINESE_ZODIAC_LIMIT_LEVEL_LIMIT);
		return -100;
	}

	const Equipment * equip = dynamic_cast<const Equipment*>(ITEMPOOL->GetItem(item_wrapper.item_id));
	if (equip == nullptr) return -2;

	int equip_type = equip->GetEquipType() - Equipment::E_TYPE_CHINESE_ZODIAC_MIN;

	if (equip_type <0 || equip_type >= CHINESE_ZODIAC_CUR_EQUIP_SLOT_MAX_LIMIT)
	{
		m_role->NoticeNum(errornum::EN_CHINESE_ZOIDAC_NOT_EQUIP);
		return -3;
	}

	int last_item_id = m_param.chinesezodiac_equip_list[zodiac_type].equip_Item_list[equip_type];
	if (last_item_id < 0)
	{
		return -4 ;
	}

	if (last_item_id > 0)
	{
		int ret = this->TakeOffEquip(zodiac_type, equip_type,true);
		if (0 != ret)
		{
			return -5;
		}
	}

	m_param.chinesezodiac_equip_list[zodiac_type].equip_Item_list[equip_type] = item_wrapper.item_id;

	this->SendChineseZodiacEquipInfo(zodiac_type, equip_type);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHINESE_ZODIAC);

	gamelog::g_log_chinese_zodiac.printf(LL_INFO, "ChineseZodiac::PutOnEquip user[%d %s] zodiac_type[%d] equip_type[%d] item_id[%d] num[%d]",
		m_role->GetUID(), m_role->GetName(), zodiac_type, equip_type, item_wrapper.item_id , 1);
	return 0;
}

int ChineseZodiac::TakeOffEquip(int zodiac_type, int equip_type, bool is_put_on)
{
	if (zodiac_type < 0 || zodiac_type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT ||
		equip_type < 0 || equip_type >= CHINESE_ZODIAC_CUR_EQUIP_SLOT_MAX_LIMIT) {
		return -1;
	}
	
	ItemID off_item = m_param.chinesezodiac_equip_list[zodiac_type].equip_Item_list[equip_type];
	if (off_item <= 0)
	{
		return -2;
	}
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(off_item);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() ||
		equip->GetEquipType() < Equipment::E_TYPE_CHINESE_ZODIAC_1 || equip->GetEquipType() >= Equipment::E_TYPE_CHINESE_ZODIAC_MAX)
	{
		m_role->NoticeNum(errornum::EN_CHINESE_ZODIAC_EQUIP_ERROR);
		return -3;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return -5;
	}
	ItemConfigData item_data;
	item_data.item_id = equip->GetItemId();
	item_data.is_bind = equip->IsBind();
	item_data.num = 1;
	if (!knapsack->Put(item_data, PUT_REASON_NO_NOTICE))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_EQUIP_TAKEOFF_ERROR);
		return -6;
	}

	m_param.chinesezodiac_equip_list[zodiac_type].equip_Item_list[equip_type] = 0;

	if (!is_put_on)
	{
		this->SendChineseZodiacEquipInfo(zodiac_type, equip_type);

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHINESE_ZODIAC);
	}
	gamelog::g_log_chinese_zodiac.printf(LL_INFO, "ChineseZodiac::TakeOffEquip user[%d %s] zodiac_type[%d] equip_type[%d] item_id[%d] num[%d]",
		m_role->GetUID(), m_role->GetName(), zodiac_type, equip_type, equip->GetItemId(), 1);
	return 0;
}

void ChineseZodiac::Recycle(Protocol::CSChineseZodiacRecycle * rcy_msg)
{
	if (nullptr == rcy_msg) return;
	Knapsack * knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	ItemID id_get = LOGIC_CONFIG->GetChineseZodiacCfg().GetOtherCfg().equiment_recycle;
	const ItemBase * item_get = ITEMPOOL->GetItem(id_get);
	if (nullptr == item_get)
	{
		return;
	}

	//const int pile_max_num = item_get->GetPileLimit();

	std::vector<ItemConfigData> item_vec;
	for (unsigned int i = 0; i < (unsigned int)rcy_msg->index_count && i < static_array_size(rcy_msg->index_list); i ++)
	{
		gamelog::g_log_seal.buff_printf("Seal::Recycle user[%d, %s] hunlist[", m_role->GetUID(), m_role->GetName());

		int backpack_index = rcy_msg->index_list[i];
		if (backpack_index < 0 || backpack_index >= ItemNamespace::MAX_KNAPSACK_GRID_NUM)
		{
			continue;
		}

		ItemGridData* item_grid = knapsack->GetItemGridData(backpack_index);
		if (item_grid == nullptr)
		{
			continue;
		}

		const Equipment * equip = dynamic_cast<const Equipment*>(ITEMPOOL->GetItem(item_grid->item_id));
		if (nullptr == equip || equip->GetEquipType() < Equipment::E_TYPE_CHINESE_ZODIAC_1 || equip->GetEquipType() >= Equipment::E_TYPE_CHINESE_ZODIAC_MAX)
		{
			continue;
		}

		int get_num = item_grid->num * equip->GetRecyclGet(nullptr);

		ItemConfigData item_put;
		item_put.item_id = id_get;
		item_put.num = get_num;
		item_put.is_bind = item_grid->is_bind;

		if (!knapsack->Discard(backpack_index, item_grid->num, DISCARD_METHOD_RECYCL, false))
		{
			continue;
		}

		auto it = std::find_if(item_vec.begin(), item_vec.end(), [item_put](const ItemConfigData &tmp_item) {
			return item_put.item_id == tmp_item.item_id && item_put.is_bind == tmp_item.is_bind;
		});
		if (item_vec.end() != it)
		{
			(*it).num += get_num;
		}
		else
		{
			item_vec.push_back(item_put);
		}

		gamelog::g_log_chinese_zodiac.buff_printf("rcyid:%d,num:%d,getid:%d,num:%d", item_grid->item_id, item_grid->num, id_get, get_num);
		gamelog::g_log_seal.commit_buff(LL_INFO);
	}


	// 放物品进背包
	{
		for (auto &it : item_vec)
		{
			ItemConfigData item_data(it.item_id, it.is_bind, it.num);
			m_role->GetKnapsack()->PutOrMail(item_data, PUT_REASON_NO_NOTICE);
		}
	
		m_role->GetKnapsack()->SendKnapsackInfoToRole();
	}

	return;
}

bool ChineseZodiac::IsChineseZodiacActivity(int zodiac_type)
{
	if (zodiac_type < 0 || zodiac_type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
	{
		return false;
	}

	return m_param.zodiac_level_list[zodiac_type] > 0;
}

void ChineseZodiac::SendChineseZodiacAllInfo(int upgrade_zodiac)
{
	static Protocol::SCChineseZodiacAllInfo czai;
	memcpy(czai.zodiac_level_list, m_param.zodiac_level_list, sizeof(czai.zodiac_level_list));
	memcpy(czai.xinghun_level_list, m_param.xinghun_level_list, sizeof(czai.xinghun_level_list));
	memcpy(czai.xinghun_level_max_list, m_param.xinghun_level_max_list, sizeof(czai.xinghun_level_max_list));
	memcpy(czai.xinghun_baoji_value_list, m_param.xinghun_baoji_value_list, sizeof(czai.xinghun_baoji_value_list));
	memcpy(czai.chinesezodiac_equip_list, m_param.chinesezodiac_equip_list, sizeof(czai.chinesezodiac_equip_list));

	for (int i = 0; i < CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT; i++)
	{
		for (int j = 0; j < MIJI_KONG_NUM; j++)
		{
			czai.miji_list[i].single_miji_list[j] = m_param.miji_list[i].single_miji_list[j];
		}
	}

	czai.zodiac_progress = m_param.zodiac_progress;
	czai.upgrade_zodiac = upgrade_zodiac;
	czai.xinghun_progress = m_param.xinghun_progress;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&czai, sizeof(czai));

	// 滚滚乐信息
	static Protocol::SCGunGunLeInfo pro;
	pro.count = 0;
	memset(pro.combine_type_list, 0, sizeof(pro.combine_type_list));
	pro.today_ggl_free_times = m_param.today_ggl_free_times;
	pro.last_free_ggl_time = m_param.last_free_ggl_time;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&pro, sizeof(pro));
}

void ChineseZodiac::SendChineseZodiacEquipInfo(int zodiac_type, int equip_type)
{
	if (zodiac_type < 0 || zodiac_type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
	{
		return;
	}

	if (equip_type < 0 || equip_type >= CHINESE_ZODIAC_CUR_EQUIP_SLOT_MAX_LIMIT)
	{
		return;
	}

	static Protocol::SCChineseZodiacEquipInfo czei;
	czei.zodiac_type = zodiac_type;
	czei.equip_type = equip_type;
	czei.equip_item = m_param.chinesezodiac_equip_list[zodiac_type].equip_Item_list[equip_type];

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&czei, sizeof(czei));
}

void ChineseZodiac::ReqOpera(int info_type, int param1, int param2, int param3, int param4, int param5)
{
	const ChineseZodiacOther &other_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetOtherCfg();
	if (m_role->GetLevel() < other_cfg.limit_level)
	{
		return;
	}

	switch (info_type)
	{
	case Protocol::CS_TIAN_XIANG_TYPE_OLD_ALL_INFO:
		{
			this->SendAllInfo();
		}
		break;

	case Protocol::CS_TIAN_XIANG_TYPE_OLD_CHANGE_BEAD:
		{
			this->ChangeGrid(param1, param2, param3, param4, param5);
		}
		break;

	case Protocol::CS_TIAN_XIANG_TYPE_OLD_XIE_BEAD:
		{
			this->UnloadBead(param1, param2, param3);
		}
		break;

	case Protocol::CS_TIAN_XIANG_TYPE_OLD_GUNGUN_LE_REQ:
		{
			this->GunGunLeChou(param1);
		}
		break;

	case Protocol::CS_TIAN_XIANG_TYPE_OLD_UNLOCK:
		{
			this->ZoiacUnlock();
		}
		break;

	//case Protocol::CS_TIAN_XIANG_TYPE_OLD_PUT_MIJI:
	//	{
	//		this->PutMiji(param1, param2);
	//	}
	//	break;
	//	
	//case Protocol::CS_TIAN_XIANG_TYPE_OLD_CALC_CAPACITY:
	//	{
	//		this->PutMijiRet();
	//	}
	//	break;

	//case Protocol::CS_TIAN_XIANG_TYPE_OLD_MIJI_COMPOUND:
	//	{
	//		this->MijiCompound(param1);
	//	}
	//	break;

	case Protocol::CS_TIAN_XIANG_TYPE_OLD_PUT_BEAD:
		{
			this->ReqActivebead(param1, param2);
		}
		break;
	case Protocol::CS_TIAN_XIANG_TYPE_OLD_UPGRADE_XINGLING:
		{
			this->UpgradeXingling(param1, param2);
		}
		break;
	case Protocol::CS_TIAN_XIANG_TYPE_OLD_UPLEVEL_XINGHUN:
	{
		this->PromoteXingHun(param1, 0 != param2, 0 != param3);
	}
	break;

	case Protocol::CS_TIAN_XIANG_TYPE_OLD_XINGHUN_UNLOCK:
	{
		this->XinhunUnlock();
	}
	break;

	default:
		break;
	}
}

void ChineseZodiac::GmOpera(int type, int p1, int p2, int p3, int p4)
{
#ifndef _DEBUG
	return;
#endif

	switch (type)
	{
	case 0:
		{
			
		}
		break;

	case 1:
		{
			this->TrantsItem(p1);
		}
		break;

	case 2:
		{
			//this->ChangeGrid(p1, p2, p3, p4);
		}
		break;

	default:
		break;
	}
}

void ChineseZodiac::UnloadBead(int x, int y, int chapter)
{
// 	if (1 == m_param.is_finish_all_chapter)
// 	{
// 		return;
// 	}

	if (!this->IsValidPosi(x, y))
	{
		return;
	}
	
	if (chapter < 0 || chapter >= TIAN_XIANG_CURR_CHAPTER_NUM) return;
	TianXiangTableGridParam &grid = m_param.chapter_lsit[chapter].table_lsit[x][y];
	if (grid.IsEmpty())
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

	const TianXiangBeadCfg *bead_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetBeadCfg(grid.type);
	if (NULL == bead_cfg)
	{
		return;
	}
	
	if (!knapsack->Put(ItemConfigData(bead_cfg->item_id, true, 1), PUT_REASON_ZODIAC_BEAD_UNLOAD))
	{
		return;
	}

	grid.type = 0;
	
	this->SendSingleInfo(x, y, chapter);
	this->CheckCombineAll(chapter);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHINESE_ZODIAC);
}

bool ChineseZodiac::TrantsItem(int type)
{
// 	if (1 == m_param.is_finish_all_chapter)
// 	{
// 		return false;
// 	}

	if (type < TIAN_XIANG_BEAD_TYPE_FIRE || type >= TIAN_XIANG_BEAD_TYPE_MAX)
	{
		return false;
	}

	for (int y = 0; y < TIAN_XIANG_TABEL_ROW_COUNT; ++y)
	{
		for (int x = 0; x < TIAN_XIANG_TABEL_MIDDLE_GRIDS; ++x)
		{
			if (!this->IsValidPosi(x, y))
			{
				continue;
			}

			if (m_param.curr_chapter >= TIAN_XIANG_CURR_CHAPTER_NUM) return false;
			for (int i = m_param.curr_chapter; i >= 0; i--)
			{
				if (m_param.chapter_lsit[i].table_lsit[x][y].IsEmpty())
				{
					return this->Activebead(x, y, type, i);
				}
			}
		}
	}

	return false;
}

void ChineseZodiac::ReqActivebead(short type, int chapter)
{
	if (chapter < 0 || chapter >= TIAN_XIANG_CURR_CHAPTER_NUM) return;

	if (type < TIAN_XIANG_BEAD_TYPE_FIRE || type >= TIAN_XIANG_BEAD_TYPE_MAX)
	{
		return;
	}

	const TianXiangBeadCfg *bead_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetBeadCfg(type);
	if (NULL == bead_cfg)
	{
		return;
	}

	for (int y = 0; y < TIAN_XIANG_TABEL_ROW_COUNT; ++y)
	{
		for (int x = 0; x < TIAN_XIANG_TABEL_MIDDLE_GRIDS; ++x)
		{
			if (!this->IsValidPosi(x, y))
			{
				continue;
			}

			if (m_param.chapter_lsit[chapter].table_lsit[x][y].IsEmpty())
			{
				static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
				ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
				stuff_list[0].item_id = bead_cfg->item_id;
				stuff_list[0].num = 1;
				stuff_list[0].buyable = false;

				if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
				{
					m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
					return;
				}

				// 消耗
				if (!ItemExtern::ConsumeItem(m_role, consume_list, "Xintu_ReqActivebead"))
				{
					return;
				}

				this->Activebead(x, y, type, chapter);
				return;
			}

			if (y == TIAN_XIANG_TABEL_ROW_COUNT - 1 && x == TIAN_XIANG_TABEL_MIDDLE_GRIDS - 1)
			{
				m_role->NoticeNum(errornum::EN_CHINESE_XINGTU_ACTIVITE_FAIL);
				return;
			}
		}
	}
}

bool ChineseZodiac::Activebead(int x, int y, short type, int chapter)
{
	if (chapter < 0 || chapter >= TIAN_XIANG_CURR_CHAPTER_NUM) return false;

	if (type < TIAN_XIANG_BEAD_TYPE_FIRE || type >= TIAN_XIANG_BEAD_TYPE_MAX)
	{
		return false;
	}

	if (!this->IsValidPosi(x, y))
	{
		return false;
	}
	
	TianXiangTableGridParam &grid = m_param.chapter_lsit[chapter].table_lsit[x][y];
	if (!grid.IsEmpty())
	{
		return false;
	}

	grid.type = type;

	this->SendSingleInfo(x, y, chapter);
	this->CheckCombineAll(chapter);

	m_role->NoticeNum(noticenum::NT_TIAN_XIANG_ACTIVE_BEAD_SUCC);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHINESE_ZODIAC);
	return true;
}

void ChineseZodiac::ChangeGrid(int src_x, int src_y, int target_x, int target_y, int chapter)
{
// 	if (1 == m_param.is_finish_all_chapter)
// 	{
// 		return;
// 	}

	if (chapter < 0 || chapter >= TIAN_XIANG_CURR_CHAPTER_NUM) return;

	if (!this->IsValidPosi(src_x, src_y) || !this->IsValidPosi(target_x, target_y))
	{
		return;
	}
	
	TianXiangTableGridParam &src_grid = m_param.chapter_lsit[chapter].table_lsit[src_x][src_y];
	TianXiangTableGridParam &target_grid = m_param.chapter_lsit[chapter].table_lsit[target_x][target_y];
	if (target_grid.type == src_grid.type)
	{
		return;
	}

	//替换
	TianXiangTableGridParam tmp_grid = src_grid;
	src_grid = target_grid;
	target_grid = tmp_grid;

	this->SendSingleInfo(src_x, src_y, chapter);
	this->SendSingleInfo(target_x, target_y, chapter);
	this->CheckCombineAll(chapter);

	m_role->NoticeNum(noticenum::NT_TIAN_XIANG_MOVE_BEAD_SUCC);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHINESE_ZODIAC);
}

void ChineseZodiac::UpdateChapter(int chapter)
{
	if (chapter < 0 || chapter >= TIAN_XIANG_CURR_CHAPTER_NUM) return;

	bool is_update = true;
	for (int i = 0; i < TIAN_XIANG_ALL_CHAPTER_COMBINE; i++)
	{
		if (0 == m_active_list[chapter][i])
		{
			is_update = false;
		}
	}

	if (is_update && chapter == m_param.curr_chapter && 0 == m_param.is_finish_all_chapter)
	{
		int finish_chapter_index = -1;
		if (m_param.curr_chapter + 1 < TIAN_XIANG_CURR_CHAPTER_NUM)
		{
			finish_chapter_index = m_param.curr_chapter;
			m_param.curr_chapter++;
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHINESE_ZODIAC);
			//this->CheckCombineAll();
			this->SendAllInfo();
		}
		else
		{
			if (0 == m_param.is_finish_all_chapter)
			{
				m_param.is_finish_all_chapter = 1;
				finish_chapter_index = m_param.curr_chapter;
				this->SendAllInfo();
			}
		}

		if (finish_chapter_index != -1)
		{
			const ChapterAttributesConfig *chapter_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetChapterAttr(finish_chapter_index);
			if (NULL != chapter_cfg)
			{
				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xintu_finish_chapter, 
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), chapter_cfg->name);
				if (len > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}
}

void ChineseZodiac::CheckCombineAll(int chapter, bool is_init)
{
	if (chapter < 0 || chapter >= TIAN_XIANG_CURR_CHAPTER_NUM) return;

	for (int i = 0; i < TIAN_XIANG_ALL_CHAPTER_COMBINE; ++i)
	{
		for (int j = 0; j < TIAN_XIANG_COMBINE_MEX_BEAD_NUM; j++)
		{
			m_param.chapter_lsit[chapter].bead_by_combine_lsit[i][j].Reset();
		}
	}

	for (int j = 0; j < TIAN_XIANG_ALL_CHAPTER_COMBINE; j++)
	{
		m_active_list[chapter][j] = 0;
	}

	for (int y = 0; y < TIAN_XIANG_TABEL_ROW_COUNT; ++y)
	{
		for (int x = 0; x < TIAN_XIANG_TABEL_MIDDLE_GRIDS; ++x)
		{
			if (!this->IsValidPosi(x, y))
			{
				continue;
			}
			
			TianXiangTableGridParam &grid_bead = m_param.chapter_lsit[chapter].table_lsit[x][y];
			if (grid_bead.IsEmpty())
			{
				continue;
			}

			BeadPosi combine_posi(x, y);
			for (int i = 0; i < TIAN_XIANG_ALL_CHAPTER_COMBINE; ++i)
			{
				// 检查是否已激活, 不能重复激活；
				if (1 == m_active_list[chapter][i])
				{
					continue;
				}

				// 是否可以激活
				if (!this->CanActiveCombine(i, grid_bead.type, combine_posi, chapter))
				{
					continue;
				}

				// 激活组合
				m_active_list[chapter][i] = 1;
			}
		}
	}

	if (!is_init)
	{
		this->UpdateChapter(chapter);
		this->SendCombineInfo(chapter);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHINESE_ZODIAC);
	}
}

bool ChineseZodiac::CanActiveCombine(int seq, short src_type, const BeadPosi &src_posi, int chapter)
{
	if (chapter < 0 || chapter >= TIAN_XIANG_CURR_CHAPTER_NUM) return false;

	int real_seq = chapter * TIAN_XIANG_ALL_CHAPTER_COMBINE + seq;

	// 到这里不存在第一个珠子为无效类型的组合配置
	const TianXiangCombineSeqCfg *Combine_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetCombineSeqCfg(real_seq);
	if (NULL == Combine_cfg)
	{
		return false;
	}

	// 对比第一个格子 第一个格子就不符合的，直接跳过；
	const TianXiangCombineItem &firt_bead = Combine_cfg->bead_list[0];
	if (firt_bead.type != TIAN_XIANG_BEAD_TYPE_SAME && firt_bead.type != src_type)
	{
		return false;
	}
	
	// 先插入第一个格子坐标
	m_tmp_posi_set.clear();
	m_tmp_posi_set.insert(src_posi);
	BeadPosi posi;
	// 对比后面的格子
	for (int index = 1; index < TIAN_XIANG_COMBINE_MEX_BEAD_NUM; ++index)
	{
		const TianXiangCombineItem &bead_cfg = Combine_cfg->bead_list[index];

		//只能是组合的必要珠子都合格了，才会到这里, 配置正确的话；
		if (bead_cfg.type == TIAN_XIANG_BEAD_TYPE_INVALID)
		{
			break;
		}

		// 寻找格子
		posi.Set(src_posi.x + bead_cfg.x, src_posi.y + bead_cfg.y);
		if (!this->IsValidPosi(posi.x, posi.y))
		{
			return false;
		}
		
		const TianXiangTableGridParam &tmp_bead = m_param.chapter_lsit[chapter].table_lsit[posi.x][posi.y];

		// 检查类型
		if (bead_cfg.type == TIAN_XIANG_BEAD_TYPE_SAME && tmp_bead.type != src_type)
		{
			return false;
		}

		if (bead_cfg.type != TIAN_XIANG_BEAD_TYPE_SAME && bead_cfg.type != tmp_bead.type)
		{
			return false;
		}

		m_tmp_posi_set.insert(posi);
	}
	
	if (m_tmp_posi_set.size() > 1)
	{	
		int index = 0;
		for (std::set<BeadPosi>::iterator it_set = m_tmp_posi_set.begin(); it_set != m_tmp_posi_set.end(); ++ it_set)
		{
			m_param.chapter_lsit[chapter].bead_by_combine_lsit[seq][index].x = it_set->x;
			m_param.chapter_lsit[chapter].bead_by_combine_lsit[seq][index].y = it_set->y;
			index++;
		}

		if (0 == (m_param.combine_system_msg_flag & (1 << real_seq)))
		{
			m_param.combine_system_msg_flag |= (1 << real_seq); 

			const TianXiangCombineAttrCfg *combine_attr_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetCombineAttrCfg(real_seq);
			if (NULL != combine_attr_cfg)
			{
				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xintu_finish_combine, 
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), combine_attr_cfg->name);
				if (len > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}
	m_tmp_posi_set.clear();

	return true;
}

bool ChineseZodiac::IsValidPosi(const int x, const int y)
{
	if (x < 0 || x >= TIAN_XIANG_TABEL_MIDDLE_GRIDS || y < 0 || y >= TIAN_XIANG_TABEL_ROW_COUNT)
	{
		return false;
	}

	if (y < TIAN_XIANG_TABEL_MIDDLE_ROW)
	{
		int difference = TIAN_XIANG_TABEL_MIDDLE_ROW - y;
		if (x >= TIAN_XIANG_TABEL_MIDDLE_GRIDS - difference)
		{
			return false;
		}
	}
	else
	{
		int difference = y - TIAN_XIANG_TABEL_MIDDLE_ROW;
		if (x < difference)
		{
			return false;
		}
	}

	return true;
}

void ChineseZodiac::SendAllInfo() const
{
	static Protocol::SCTianXiangAllInfo txai;
	txai.curr_chapter = m_param.curr_chapter;
	txai.is_finish_all_chapter = m_param.is_finish_all_chapter;

	for (int i = 0; i < TIAN_XIANG_CURR_CHAPTER_NUM; i++)
	{
		for (int j = 0; j < TIAN_XIANG_ALL_CHAPTER_COMBINE; j++)
		{
			txai.active_list[i][j] = m_active_list[i][j];
		}
	}
	
	for (int chapter = 0; chapter < TIAN_XIANG_CURR_CHAPTER_NUM; chapter++)
	{
		for (int combine = 0; combine < TIAN_XIANG_ALL_CHAPTER_COMBINE; combine++)
		{
			for (int bead = 0; bead < TIAN_XIANG_COMBINE_MEX_BEAD_NUM; bead++)
			{
				txai.chapter_list[chapter].bead_by_combine_list[combine][bead].Reset();
			}
		}
	}

	for (int chapter = 0; chapter <= txai.curr_chapter; chapter++)
	{
		int grid_index = 0;
		for (int y = 0; y < TIAN_XIANG_TABEL_ROW_COUNT; ++y)
		{
			for (int x = 0; x < TIAN_XIANG_TABEL_MIDDLE_GRIDS; ++x)
			{
				const TianXiangTableGridParam &tmp_grid = m_param.chapter_lsit[chapter].table_lsit[x][y];
				txai.chapter_list[chapter].grid_list[grid_index] = tmp_grid.type;
				grid_index++;
			}
		}
		
		for (int combine = 0; combine < TIAN_XIANG_ALL_CHAPTER_COMBINE; combine++)
		{
			for (int bead = 0; bead < TIAN_XIANG_COMBINE_MEX_BEAD_NUM; bead++)
			{
				txai.chapter_list[chapter].bead_by_combine_list[combine][bead].x = m_param.chapter_lsit[chapter].bead_by_combine_lsit[combine][bead].x;
				txai.chapter_list[chapter].bead_by_combine_list[combine][bead].y = m_param.chapter_lsit[chapter].bead_by_combine_lsit[combine][bead].y;
			}
		}
	}
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&txai, sizeof(txai));
}

void ChineseZodiac::SendSingleInfo(int x, int y, int chapter) const
{
	if (!this->IsValidPosi(x, y))
	{
		return;
	}

	if (chapter < 0 || chapter >= TIAN_XIANG_CURR_CHAPTER_NUM) return;
	const TianXiangTableGridParam &tmp_grid = m_param.chapter_lsit[chapter].table_lsit[x][y];
	static Protocol::SCTianXiangSignBead pro;
	pro.chapter = chapter;
	pro.x = (char)x;
	pro.y = (char)y;
	pro.type = tmp_grid.type;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&pro, sizeof(pro));
}

void ChineseZodiac::SendCombineInfo(int chapter) const
{
	if (chapter < 0 || chapter >= TIAN_XIANG_CURR_CHAPTER_NUM) return;

	static Protocol::SCTianXiangCombind pro;

	for (int i = 0; i < TIAN_XIANG_ALL_CHAPTER_COMBINE; i++)
	{
		pro.active_list[i] = m_active_list[chapter][i];
	}
	
	pro.chapter = chapter;
	for (int combine = 0; combine < TIAN_XIANG_ALL_CHAPTER_COMBINE; combine++)
	{
		for (int bead = 0; bead < TIAN_XIANG_COMBINE_MEX_BEAD_NUM; bead++)
		{
			pro.bead_by_combine_list[combine][bead].x = m_param.chapter_lsit[chapter].bead_by_combine_lsit[combine][bead].x;
			pro.bead_by_combine_list[combine][bead].y = m_param.chapter_lsit[chapter].bead_by_combine_lsit[combine][bead].y;
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&pro, sizeof(pro));
}

void ChineseZodiac::SendSingleMiji(int zodiac_type, int kong_index, int miji_index)
{
	static Protocol::SCMijiSingleChange pro;

	pro.zodiac_type = zodiac_type;
	pro.kong_index = kong_index;
	pro.miji_index = miji_index;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&pro, sizeof(pro));
}

void ChineseZodiac::SendXinglingInfo()
{
	static Protocol::SCXinglingInfo pro;

	for (int i = 0; i < TIAN_XIANG_CHAPTER_NUM; i++)
	{
		pro.xingling_lsit[i].level = m_param.xingling_lsit[i].level;
		pro.xingling_lsit[i].bless = m_param.xingling_lsit[i].bless;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&pro, sizeof(pro));
}

void ChineseZodiac::GunGunLeChou(int type)
{
	const ChineseZodiacOther & other_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetOtherCfg();
	
	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	int chou_num = (type == 1) ? 10 : 1;

	if (!knapsack->CheckEmptyGridNoLessThan(chou_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if(chou_num == 10)
	{
		if (other_cfg.can_use_item)
		{
			if (!knapsack->ConsumeItem(other_cfg.ten_chou_use_itemid, other_cfg.ten_chou_use_item_num, "chinesezodiacGunGunLeChou"))
			{
				if (!knapsack->GetMoney()->UseGold(other_cfg.ggl_ten_consume_gold, "chinesezodiacGunGunLeChou"))
				{
					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}
			}
		}
		else
		{
			if (!knapsack->GetMoney()->UseGold(other_cfg.ggl_ten_consume_gold, "chinesezodiacGunGunLeChou"))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
		}
	}
	else if (chou_num == 1)
	{
		if (EngineAdapter::Instance().Time() > m_param.last_free_ggl_time + other_cfg.ggl_free_times_cd &&  m_param.today_ggl_free_times < other_cfg.ggl_free_times)
		{
			m_param.last_free_ggl_time = (unsigned int)(EngineAdapter::Instance().Time());
			m_param.today_ggl_free_times++;
		}
		else if (other_cfg.can_use_item)
		{
			if (!knapsack->ConsumeItem(other_cfg.replacement_id, other_cfg.consume_replacement, "chinesezodiacGunGunLeChou"))
			{
				if (!knapsack->GetMoney()->UseGold(other_cfg.ggl_consume_gold, "chinesezodiacGunGunLeChou"))
				{
					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}
			}
		}
		else if (!knapsack->GetMoney()->UseGold(other_cfg.ggl_consume_gold, "chinesezodiacGunGunLeChou"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}
	else
	{
		return;
	}

	static Protocol::SCGunGunLeInfo pro;
	pro.count = 0;
	memset(pro.combine_type_list, 0, sizeof(pro.combine_type_list));

	pro.today_ggl_free_times = m_param.today_ggl_free_times;
	pro.last_free_ggl_time = m_param.last_free_ggl_time;

	int reward_index_list[GGL_COMBINE_TYPE_MAX_NUM];
	memset(reward_index_list, 0, sizeof(reward_index_list));
	
	for (int i = 0; i < chou_num; i++)
	{	
		int index = 0;
		int res_num = 0;

		LOGIC_CONFIG->GetChineseZodiacCfg().GetGglResult(index, res_num);
		LotterySequence replace_index = static_cast<LotterySequence>(index);
		m_role->GetLotteryCount()->ReplaceLotterySequence(LOTTERY_REPLACE_TYPE_CHINESE_ZODIAC, replace_index);
		const GglCombineType *ggl_reward_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetGglRewardCfg(replace_index);
		if (NULL == ggl_reward_cfg)
		{
			replace_index = index;
		}
		if (replace_index != -1)
		{
			pro.combine_type_list[i] = replace_index;
			reward_index_list[i] = replace_index;
			pro.count++;
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&pro, sizeof(pro));
	
	for (int i = 0; i < pro.count; i++)
	{
		const GglCombineType *ggl_reward_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetGglRewardCfg(reward_index_list[i]);
		if (NULL != ggl_reward_cfg)
		{
			m_role->GetKnapsack()->PutListOrMail(ggl_reward_cfg->reward_item_list, PUT_REASON_ZODIAC_GGL_REWARD);
		}
	}
}

void ChineseZodiac::ZoiacUnlock()
{
	if (m_param.zodiac_progress < 1 || m_param.zodiac_progress >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
	{
		return;
	}

	int zodiac_level = m_param.zodiac_level_list[m_param.zodiac_progress - 1];
	
	const ChineseZodiacSingleAttr::SingleType *single_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetSingleAttr(m_param.zodiac_progress, 1);
	if (NULL == single_cfg)
	{
		return;
	}

	if (zodiac_level < single_cfg->level_limit)
	{
		return;
	}

	m_param.zodiac_progress++;
	this->SendChineseZodiacAllInfo();
}

void ChineseZodiac::XinhunUnlock()
{
	if (m_param.xinghun_progress < 1 || m_param.xinghun_progress >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
	{
		return;
	}

	int xinghun_max_level = m_param.xinghun_level_max_list[m_param.xinghun_progress - 1];
	const ChineseZodiacXingHunCfg *xinghun_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetChineseZodiacXingHunCfg(m_param.xinghun_progress, 0);
	if (NULL == xinghun_cfg)
	{
		return;
	}

	if (xinghun_max_level < xinghun_cfg->backwards_highest_level)
	{
		return;
	}

	m_param.xinghun_progress++;
	this->SendChineseZodiacAllInfo();
}

//void ChineseZodiac::PutMijiRet()
//{
//	if (m_miji_put_notice != 0)
//	{
//		m_role->NoticeNum(m_miji_put_notice);
//		m_miji_put_notice = 0;
//	}
//	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHINESE_ZODIAC);
//}

//void ChineseZodiac::PutMiji(int zodiac_type, int miji_index)
//{
//	const ChineseZodiacMijiOpenLimit * open_limit = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiOpenLimit(zodiac_type);
//	if (zodiac_type < 0 || zodiac_type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT || NULL == open_limit) return;
//
//	if (zodiac_type > 0)
//	{
//		int last_zodiac_miji_num = 0;
//		for (int i = 0; i < MIJI_KONG_NUM; i++)
//		{
//			if (m_param.miji_list[zodiac_type - 1].single_miji_list[i] != -1)
//			{
//				last_zodiac_miji_num++;
//			}
//		}
//
//		if (last_zodiac_miji_num < open_limit->last_zodiac_miji_num_limit)
//		{
//			m_role->NoticeNum(errornum::EN_CHINESE_LAST_ZODIAC_MIJI_NUM_LIMIT);
//			return;
//		}
//	}
//
//	const ChineseZodiacMiji * put_miji_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiCfgByIndex(miji_index);
//	if (NULL == put_miji_cfg) return;
//
//	for (int i = 0; i < MIJI_KONG_NUM; i++)
//	{
//		if (m_param.miji_list[zodiac_type].single_miji_list[i] != -1)
//		{
//			const ChineseZodiacMiji * miji_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiCfgByIndex(m_param.miji_list[zodiac_type].single_miji_list[i]);
//			if (NULL == miji_cfg) continue;
//
//			if (put_miji_cfg->type == miji_cfg->type)
//			{
//				m_role->NoticeNum(errornum::EN_CHINESE_ZODIAC_MIJI_NOT_SAME_TYPE);
//				return;
//			}
//		}
//	}
//
//	int total_num = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiKongOpenNumByZodiacLevel(m_param.zodiac_level_list[zodiac_type]);
//	if (0 == total_num)
//	{
//		m_role->NoticeNum(errornum::EN_CHINESE_ZODIAC_LIMIT_LEVEL_LIMIT);
//		return;
//	}
//
//	Knapsack *knapsack = m_role->GetKnapsack();
//	if (knapsack->IsLock())
//	{
//		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
//		return;
//	}
//
//	{
//		// 消耗
//		static ItemExtern::ItemConsumeList consume_list; 
//		consume_list.Reset();
//
//		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
//		stuff_list[0].item_id = put_miji_cfg->item_id;
//		stuff_list[0].num = 1;
//		stuff_list[0].buyable = false;
//
//		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, false))
//		{
//			m_role->NoticeNum(errornum::EN_PUBLICSALE_BUY_ITEM_NOT_EXIST);
//			return;
//		}
//
//		if (consume_list.count <= 0)
//		{
//			m_role->NoticeNum(errornum::EN_WUSHANG_EQUIP_NOT_ENOUGH);
//			return;
//		}
//		
//		if (consume_list.count > 0 && !knapsack->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "ChineseZodiac::PutMiji"))
//		{
//			m_role->NoticeNum(errornum::EN_WUSHANG_EQUIP_NOT_ENOUGH);
//			return;
//		}
//	}
//
//	int rand_kong = RandomNum(total_num);
//	
//	int rand_kong_miji = m_param.miji_list[zodiac_type].single_miji_list[rand_kong];
//	if (rand_kong_miji != -1)
//	{
//		const ChineseZodiacMiji * rand_kong_miji_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiCfgByIndex(rand_kong_miji);
//		if (NULL == rand_kong_miji_cfg) return;
//
//		if (rand_kong_miji_cfg->level > put_miji_cfg->level)
//		{
//			m_miji_put_notice = errornum::EN_CHINESE_ZODIAC_MIJI_PUT_FAIL;
//			zodiac_type = -1;
//		}
//		else
//		{
//			m_param.miji_list[zodiac_type].single_miji_list[rand_kong] = miji_index;
//			m_miji_put_notice = noticenum::NT_ZODIAC_MIJI_CHANGE_SUCC;
//		}
//	}
//	else
//	{
//		m_param.miji_list[zodiac_type].single_miji_list[rand_kong] = miji_index;
//		m_miji_put_notice = noticenum::NT_ZODIAC_MIJI_PUT_SUCC;
//	}
//
//	this->SendSingleMiji(zodiac_type, rand_kong, miji_index);
//}
//
//void ChineseZodiac::MijiCompound(int index1)
//{
//	Knapsack *knapsack = m_role->GetKnapsack();
//	if (knapsack->IsLock())
//	{
//		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
//		return;
//	}
//
//	if (!knapsack->HaveSpace())
//	{
//		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
//		return;
//	}
//	
//	bool is_bind = false;
//
//	int grid_index_list[1] = {index1};
//	
//	int miji_itemid_list[1];
//
//	for (int i = 0; i < 1; i++)
//	{
//		ItemGridData item_grid_data;
//		if (m_role->GetKnapsack()->GetItemGridData(grid_index_list[i], &item_grid_data))
//		{
//			miji_itemid_list[i] = item_grid_data.item_id;
//			if (item_grid_data.is_bind)
//			{
//				is_bind = true;
//			}
//		}
//		else
//		{
//			m_role->NoticeNum(errornum::EN_PUBLICSALE_BUY_ITEM_NOT_EXIST);
//			return;
//		}
//	}
//
//	// 检查
//	int ori_level = -1;
//	int ori_type = -1;
//	for (int i = 0; i < 1; i++)
//	{
//		const ChineseZodiacMiji * miji_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiCfgByItemid(miji_itemid_list[i]);
//		if (NULL == miji_cfg)
//		{
//			m_role->NoticeNum(errornum::EN_CHINESE_ZODIAC_ITEM_NOT_MIJI);
//			return;
//		}
//	
//		if (ori_level != -1 && miji_cfg->level != ori_level)
//		{
//			m_role->NoticeNum(errornum::EN_CHINESE_ZODIAC_MIJI_COMBINE_LEVEL_NOT_SAME);
//			return;
//		}
//		
//		ori_level = miji_cfg->level;
//		ori_type = miji_cfg->type;
//	}
//	
//	// 消耗
//	{
//		for (int i = 0; i < 1; i++)
//		{
//			if (!knapsack->ConsumeItem(miji_itemid_list[i], 1, "ChineseZodiac::MijiCompound"))
//			{
//				return;
//			}
//		}
//
//		const ChineseZodiacOther &other_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetOtherCfg();
//
//		int consume_gold = other_cfg.miji_compound_consume_gold3;
//		if (0 == ori_level)
//		{
//			consume_gold = other_cfg.miji_compound_consume_gold1;
//		}
//		else if (1 == ori_level)
//		{
//			consume_gold = other_cfg.miji_compound_consume_gold2;
//		}
//		
//		if (!knapsack->GetMoney()->UseGold(consume_gold, "ChineseZodiac::MijiCompound"))
//		{
//			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
//			return;
//		}
//	}
//
//	int miji_type_count = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiTypeCount();
//	
//	int rand_type = -1;
//	for (int i = 0; i < 20000; i++)
//	{
//		rand_type = RandomNum(miji_type_count);
//		if (rand_type != ori_type)
//		{
//			break;
//		}
//	}
//
//	if (-1 == rand_type)
//	{
//		return;
//	}
//	
//	const ChineseZodiacMiji * put_miji_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiCfgByTypeAndLevel(rand_type, ori_level);
//	if (put_miji_cfg != NULL)
//	{	
//		ItemConfigData item;
//		item.item_id = put_miji_cfg->item_id;
//		item.num = 1;
//		item.is_bind = is_bind;
//		m_role->GetKnapsack()->Put(item, PUT_REASON_XINGZUO_MIJI_COMPOUND);
//		m_role->NoticeNum(noticenum::NT_ZODIAC_MIJI_COMPOUND_SUCC);
//
//		static Protocol::SCMijiCombineSucc pro;
//		pro.miji_index = put_miji_cfg->index;
//
//		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&pro, sizeof(pro));
//	}
//}
//
//	if (zodiac_type > 0)
//	{
//		int last_zodiac_miji_num = 0;
//		for (int i = 0; i < MIJI_KONG_NUM; i++)
//		{
//			if (m_param.miji_list[zodiac_type - 1].single_miji_list[i] != -1)
//			{
//				last_zodiac_miji_num++;
//			}
//		}
//
//		if (last_zodiac_miji_num < open_limit->last_zodiac_miji_num_limit)
//		{
//			m_role->NoticeNum(errornum::EN_CHINESE_LAST_ZODIAC_MIJI_NUM_LIMIT);
//			return;
//		}
//	}
//
//	const ChineseZodiacMiji * put_miji_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiCfgByIndex(miji_index);
//	if (NULL == put_miji_cfg) return;
//
//	for (int i = 0; i < MIJI_KONG_NUM; i++)
//	{
//		if (m_param.miji_list[zodiac_type].single_miji_list[i] != -1)
//		{
//			const ChineseZodiacMiji * miji_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiCfgByIndex(m_param.miji_list[zodiac_type].single_miji_list[i]);
//			if (NULL == miji_cfg) continue;
//
//			if (put_miji_cfg->type == miji_cfg->type)
//			{
//				m_role->NoticeNum(errornum::EN_CHINESE_ZODIAC_MIJI_NOT_SAME_TYPE);
//				return;
//			}
//		}
//	}
//
//	int total_num = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiKongOpenNumByZodiacLevel(m_param.zodiac_level_list[zodiac_type]);
//	if (0 == total_num)
//	{
//		m_role->NoticeNum(errornum::EN_CHINESE_ZODIAC_LIMIT_LEVEL_LIMIT);
//		return;
//	}
//
//	Knapsack *knapsack = m_role->GetKnapsack();
//	if (knapsack->IsLock())
//	{
//		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
//		return;
//	}
//
//	{
//		// 消耗
//		static ItemExtern::ItemConsumeList consume_list; 
//		consume_list.Reset();
//
//		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
//		stuff_list[0].item_id = put_miji_cfg->item_id;
//		stuff_list[0].num = 1;
//		stuff_list[0].buyable = false;
//
//		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, false))
//		{
//			m_role->NoticeNum(errornum::EN_PUBLICSALE_BUY_ITEM_NOT_EXIST);
//			return;
//		}
//
//		if (consume_list.count <= 0)
//		{
//			m_role->NoticeNum(errornum::EN_WUSHANG_EQUIP_NOT_ENOUGH);
//			return;
//		}
//		
//		if (consume_list.count > 0 && !knapsack->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "ChineseZodiac::PutMiji"))
//		{
//			m_role->NoticeNum(errornum::EN_WUSHANG_EQUIP_NOT_ENOUGH);
//			return;
//		}
//	}
//
//	int rand_kong = RandomNum(total_num);
//	
//	int rand_kong_miji = m_param.miji_list[zodiac_type].single_miji_list[rand_kong];
//	if (rand_kong_miji != -1)
//	{
//		const ChineseZodiacMiji * rand_kong_miji_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiCfgByIndex(rand_kong_miji);
//		if (NULL == rand_kong_miji_cfg) return;
//
//		if (rand_kong_miji_cfg->level > put_miji_cfg->level)
//		{
//			m_miji_put_notice = errornum::EN_CHINESE_ZODIAC_MIJI_PUT_FAIL;
//			zodiac_type = -1;
//		}
//		else
//		{
//			m_param.miji_list[zodiac_type].single_miji_list[rand_kong] = miji_index;
//			m_miji_put_notice = noticenum::NT_ZODIAC_MIJI_CHANGE_SUCC;
//		}
//	}
//	else
//	{
//		m_param.miji_list[zodiac_type].single_miji_list[rand_kong] = miji_index;
//		m_miji_put_notice = noticenum::NT_ZODIAC_MIJI_PUT_SUCC;
//	}
//
//	this->SendSingleMiji(zodiac_type, rand_kong, miji_index);
//}
//
//void ChineseZodiac::MijiCompound(int index1)
//{
//	Knapsack *knapsack = m_role->GetKnapsack();
//	if (knapsack->IsLock())
//	{
//		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
//		return;
//	}
//
//	if (!knapsack->HaveSpace())
//	{
//		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
//		return;
//	}
//	
//	bool is_bind = false;
//
//	int grid_index_list[1] = {index1};
//	
//	int miji_itemid_list[1];
//
//	for (int i = 0; i < 1; i++)
//	{
//		ItemGridData item_grid_data;
//		if (m_role->GetKnapsack()->GetItemGridData(grid_index_list[i], &item_grid_data))
//		{
//			miji_itemid_list[i] = item_grid_data.item_id;
//			if (item_grid_data.is_bind)
//			{
//				is_bind = true;
//			}
//		}
//		else
//		{
//			m_role->NoticeNum(errornum::EN_PUBLICSALE_BUY_ITEM_NOT_EXIST);
//			return;
//		}
//	}
//
//	// 检查
//	int ori_level = -1;
//	int ori_type = -1;
//	for (int i = 0; i < 1; i++)
//	{
//		const ChineseZodiacMiji * miji_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiCfgByItemid(miji_itemid_list[i]);
//		if (NULL == miji_cfg)
//		{
//			m_role->NoticeNum(errornum::EN_CHINESE_ZODIAC_ITEM_NOT_MIJI);
//			return;
//		}
//	
//		if (ori_level != -1 && miji_cfg->level != ori_level)
//		{
//			m_role->NoticeNum(errornum::EN_CHINESE_ZODIAC_MIJI_COMBINE_LEVEL_NOT_SAME);
//			return;
//		}
//		
//		ori_level = miji_cfg->level;
//		ori_type = miji_cfg->type;
//	}
//	
//	// 消耗
//	{
//		for (int i = 0; i < 1; i++)
//		{
//			if (!knapsack->ConsumeItem(miji_itemid_list[i], 1, "ChineseZodiac::MijiCompound"))
//			{
//				return;
//			}
//		}
//
//		const ChineseZodiacOther &other_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetOtherCfg();
//
//		int consume_gold = other_cfg.miji_compound_consume_gold3;
//		if (0 == ori_level)
//		{
//			consume_gold = other_cfg.miji_compound_consume_gold1;
//		}
//		else if (1 == ori_level)
//		{
//			consume_gold = other_cfg.miji_compound_consume_gold2;
//		}
//		
//		if (!knapsack->GetMoney()->UseGold(consume_gold, "ChineseZodiac::MijiCompound"))
//		{
//			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
//			return;
//		}
//	}
//
//	int miji_type_count = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiTypeCount();
//
//	if (miji_type_count <= 0)
//	{
//		return;
//	}
//	
//	int rand_type = -1;
//	for (int i = 0; i < 20000; i++)
//	{
//		rand_type = RandomNum(miji_type_count);
//		if (rand_type != ori_type)
//		{
//			break;
//		}
//	}
//
//	if (-1 == rand_type)
//	{
//		return;
//	}
//	
//	const ChineseZodiacMiji * put_miji_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetMijiCfgByTypeAndLevel(rand_type, ori_level);
//	if (put_miji_cfg != NULL)
//	{	
//		ItemConfigData item;
//		item.item_id = put_miji_cfg->item_id;
//		item.num = 1;
//		item.is_bind = is_bind;
//		m_role->GetKnapsack()->Put(item, PUT_REASON_XINGZUO_MIJI_COMPOUND);
//		m_role->NoticeNum(noticenum::NT_ZODIAC_MIJI_COMPOUND_SUCC);
//
//		static Protocol::SCMijiCombineSucc pro;
//		pro.miji_index = put_miji_cfg->index;
//
//		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&pro, sizeof(pro));
//	}
//}

void ChineseZodiac::UpgradeXingling(int xingling_id, int is_auto_buy)
{
	if (xingling_id < 0 || xingling_id > m_param.curr_chapter || xingling_id >= TIAN_XIANG_CHAPTER_NUM)
	{
		m_role->NoticeNum(errornum::EN_CHINESE_XINGLING_CHAPTER_LIMIT);
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	XingLingParam &xingling_param = m_param.xingling_lsit[xingling_id];

	if (xingling_param.level + 1 >= XING_LING_MAX_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_CHINESE_XINGLING_LEVEL_LIMIT);
		return;
	}

	const XingLingCfg::ConfigItem * curr_cfg = LOGIC_CONFIG->GetChineseZodiacCfg().GetXinglingCfg(xingling_id, xingling_param.level + 1);
	if (NULL == curr_cfg)
	{
		return;
	}
	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	{
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = curr_cfg->uplevel_stuff_id;
		stuff_list[0].num = curr_cfg->uplevel_stuff_num;
		stuff_list[0].buyable = (1 == is_auto_buy);

		if (!ItemExtern::GetConsumeListByOrder(m_role, true, 1, stuff_list, &consume_list, true))
		{
			return;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "ChineseZodiac::UpgradeXingling"))
		{
			return;
		}
	}

	xingling_param.bless++;

	int succ_rate = xingling_param.bless * curr_cfg->add_prob + curr_cfg->init_prob;
	{
		int rand_val = RandomNum(10000);
		if (rand_val < succ_rate || xingling_param.bless >= curr_cfg->bless_val_limit)
		{
			xingling_param.bless = 0;
			xingling_param.level++;

			// 升级成功
			m_role->NoticeNum(noticenum::NT_CHINESE_XINGLING_UP_LEVEL_SUCC);
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHINESE_ZODIAC);
		}
		else
		{
			m_role->NoticeNum(errornum::EN_CHINESE_XINGLING_UPLEVEL_FAIL);
		}
	}

	this->SendXinglingInfo();
}

void ChineseZodiac::ReCalcSuitCount()
{
	memset(m_equip_quality_count, 0, sizeof(m_equip_quality_count));
	for (int zodiac_type = 0; zodiac_type < CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT; ++zodiac_type)
	{
		for (int equip_type = 0; equip_type < CHINESE_ZODIAC_CUR_EQUIP_SLOT_MAX_LIMIT; equip_type++)
		{
			int equip_item = m_param.chinesezodiac_equip_list[zodiac_type].equip_Item_list[equip_type];

			if (equip_item > 0)
			{
				const Equipment* equip = dynamic_cast<const Equipment*>(ITEMPOOL->GetItem(equip_item));
				if (NULL != equip)
				{
					int equip_color = equip->GetColor();
					if (equip_color <= ItemBase::I_COLOR_INVALID || equip_color >= ItemBase::I_COLOR_MAX) return;

					//++m_equip_quality_count[zodiac_type][equip_color];

					if (equip_color == ItemBase::I_COLOR_PINK)
					{
						++m_equip_quality_count[zodiac_type][ItemBase::I_COLOR_RED];	//策划需求，套装向下兼容，粉色可代替红色计算套装
					}
					else
					{
						++m_equip_quality_count[zodiac_type][equip_color];
					}
				}
			}
		}
	}
}

void ChineseZodiac::OnRoleLevelChange(int level)
{
	this->CheckFuncOpen();
}

void ChineseZodiac::OnCompleteTask(TaskID task_id)
{
	this->CheckFuncOpen();
}

void ChineseZodiac::CheckFuncOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (m_role->GetRoleBigSmallGoal()->IsFuncActive(ROLE_BIG_SMALL_GOAL_SYSTEM_CHINESE_ZODIAC))
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_CHINESE_ZODIAC);
	if (fun_open_cfg != nullptr && opengame_day >= fun_open_cfg->opengame_day)
	{
		if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
		{
			m_role->GetRoleBigSmallGoal()->OnOpenFunc(ROLE_BIG_SMALL_GOAL_SYSTEM_CHINESE_ZODIAC);
		}
	}
}