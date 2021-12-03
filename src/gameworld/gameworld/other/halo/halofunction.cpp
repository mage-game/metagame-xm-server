#include "halofunction.hpp"
#include "halomanager.hpp"
#include "haloconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "other/upgradeskill/upgradeskillconfig.hpp"
#include "obj/character/role.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/errornum.h"
#include "servercommon/halodef.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/commondata.hpp"
#include "protocal/msghalo.hpp"
#include "world.h"
#include "gamelog.h"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "other/sublock/sublock.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "other/upgraderush/upgraderushconfig.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "servercommon/activitydef.hpp"
#include "other/upgradesyscrit/upgradesyscrit.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/crossconfig.hpp"

bool HaloFunction::Upgrade(Role *role, bool is_auto_buy, int repeat_times, int *old_level, int *coin_cost, int *gold_cost)
{
	if (NULL == role) return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	HaloManager *halo_mng = role->GetHalo();
	if (NULL == halo_mng)
	{
		return false;
	}

	HaloParam *halo = halo_mng->GetHaloParam();
	if (NULL == halo)
	{
		return false;
	}

	if (!halo->IsHaloActived())
	{
		role->NoticeNum(errornum::EN_HALO_NOT_EXIST);
		return false;
	}

	short old_grade = halo->grade;
	short oldest_bless_val = halo->grade_bless_val;
	bool succeeded = false;

	if (NULL != old_level)
	{
		*old_level = old_grade;
	}

	const HaloGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_HALO_CANNOT_UPGRADE);
		return false;
	}

	const HaloGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		role->NoticeNum(errornum::EN_HALO_CANNOT_UPGRADE);
		return false;
	}

	for (int i = 0; i < repeat_times; ++i)
	{
		short old_bless_val = halo->grade_bless_val;
		const HaloUpGradeProbConfig * upgrade_prob_cfg = LOGIC_CONFIG->GetHaloConfig().GetUpGradeProbConfig(old_grade, old_bless_val);
		if (NULL == upgrade_prob_cfg)
		{
			role->NoticeNum(errornum::EN_HALO_CANNOT_UPGRADE);
			//return false;
			succeeded = true;
			break;
		}

		static ItemExtern::ItemConsumeList consume_list;
		consume_list.Reset();

		{
			// 进阶符
			int stuff_count = 0;
			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

			short upgrade_stuff_num = role->GetKnapsack()->Count(curr_grade_cfg->upgrade_stuff2_id);
			// 如果有限时进阶丹, 则优先使用
			if (upgrade_stuff_num > 0)
			{
				stuff_list[stuff_count].item_id = curr_grade_cfg->upgrade_stuff2_id;
				stuff_list[stuff_count].num = upgrade_stuff_num >= curr_grade_cfg->upgrade_stuff_count ? curr_grade_cfg->upgrade_stuff_count : upgrade_stuff_num;
				stuff_list[stuff_count].buyable = is_auto_buy;

				++stuff_count;
			}

			// 如果限时进阶丹用完了，还不够一次批量使用，就使用普通进阶丹
			if (upgrade_stuff_num < curr_grade_cfg->upgrade_stuff_count && stuff_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
			{
				stuff_list[stuff_count].item_id = curr_grade_cfg->upgrade_stuff_id;
				stuff_list[stuff_count].num = curr_grade_cfg->upgrade_stuff_count - upgrade_stuff_num;
				stuff_list[stuff_count].buyable = is_auto_buy;

				++stuff_count;
			}

			if (!ItemExtern::GetConsumeListByOrder(role, false, stuff_count, stuff_list, &consume_list, false))
			{
				//return false;
				succeeded = true;
				break;
			}
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(role, consume_list, "HaloFunction::Upgrade"))
		{
			succeeded = true;
			break;
		}

		{
			// 生效
			int rand_val = RandomNum(10000);
			
			if (rand_val < upgrade_prob_cfg->succ_rate || halo->grade_bless_val >= curr_grade_cfg->bless_val_limit)
			{
				succeeded = true;
			}

			if (!succeeded)
			{
				// 			// 记录进阶第一次增加祝福值的时间，用于判断清0祝福值,大于5阶时使用
				// 			if (1 == curr_grade_cfg->is_clear_bless && 0 == halo->clear_upgrade_time)
				// 			{
				// 				halo->clear_upgrade_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 24 * 3600;
				// 			}
							// 记录进阶第一次增加祝福值的时间
				if (0 == halo->clear_upgrade_time)
				{
					halo->clear_upgrade_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + EngineAdapter::Instance().NextDayInterval(5, 0, 0);
				}

				int crit_value = UpgradeSysCrit::Instance().CalculateCritValue(role, UPGRADE_SYS_CRIT_TYPE_HALO);
				halo->grade_bless_val += (upgrade_prob_cfg->RandAddBlessVal() * crit_value);
				//role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);

				if (halo->grade_bless_val > curr_grade_cfg->bless_val_limit) halo->grade_bless_val = curr_grade_cfg->bless_val_limit;

			}
			else
			{
				halo->grade++;
				halo->grade_bless_val = 0;

				const HaloGradeConfig *grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(halo->grade);
				if (NULL != grade_cfg)
				{
					role->GetHalo()->ActiveImage(grade_cfg->active_image_id, false);
					role->GetHalo()->UseImage(grade_cfg->active_image_id);
					if (grade_cfg->is_broadcast)
					{
						int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_halo_upgrade,
							UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), grade_cfg->client_grade, grade_cfg->grade, role->GetHalo()->GetMedalSkillLevel());
						if (sendlen > 0)
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
						}
					}
				}

				const HaloSkillConifg *skill_cfg = LOGIC_CONFIG->GetHaloConfig().GetSkillCfg(0, halo->skill_level_list[0] + 1);
				if (NULL != skill_cfg && halo->grade >= skill_cfg->halo_grade)
				{
					halo->skill_level_list[0]++;
				}

				role->GetHalo()->ActiveImage(target_grade_cfg->active_image_id, false);
				role->GetHalo()->UseImage(target_grade_cfg->active_image_id);

				role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
				role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);			//形象赋灵增加百分比进阶属性
				//role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_GUANGHUAN, halo->grade);   // 进阶丹返还活动
				break;
			}
		}
	}

	role->GetHalo()->SendHaloInfo();
	role->GetHalo()->PrintHalo("HaloUpgrade", old_grade, oldest_bless_val);

	EventHandler::Instance().OnHaloUpgrade(role, old_grade, halo->grade);

	return !succeeded;
}

bool HaloFunction::UpOneGrade(Role *role, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num)
{
	HaloManager *halo_mng = role->GetHalo();
	if (NULL == halo_mng)
	{
		return false;
	}

	HaloParam *halo = halo_mng->GetHaloParam();
	if (NULL == halo)
	{
		return false;
	}

	if (!halo->IsHaloActived())
	{
		role->NoticeNum(errornum::EN_HALO_NOT_EXIST);
		return false;
	}

	if (halo->grade < valid_min_grade)
	{
		role->NoticeNum(errornum::EN_HALO_UP_ONE_GRADE_LIMIT);
		return false;
	}
	else if (halo->grade > valid_max_grade)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(return_item_id);
		if (item_base != nullptr)
		{
			for (int loop_time = 0; return_item_num > 0 && loop_time < 1000; ++loop_time)
			{
				int put_num = return_item_num > item_base->GetPileLimit() ? item_base->GetPileLimit() : return_item_num;
				return_item_num -= put_num;

				ItemConfigData return_item(return_item_id, true, put_num);
				role->GetKnapsack()->PutOrMail(return_item, PUT_REASON_HALO_UPGRADE_ONE_RETURN);
			}

			return true;
		}
		return false;
	}

	int old_grade = halo->grade;
	int old_bless_val = halo->grade_bless_val;

	const HaloGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_HALO_CANNOT_UPGRADE);
		return false;
	}

	const HaloGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		role->NoticeNum(errornum::EN_HALO_CANNOT_UPGRADE);
		return false;
	}

	{
		halo->grade++;
		halo->grade_bless_val = 0;
		halo->star_level = (halo->grade -1) * 10;

		// 			// 进阶成功清空时间
		// 			mount->clear_upgrade_time = 0;

		const HaloSkillConifg *skill_cfg = LOGIC_CONFIG->GetHaloConfig().GetSkillCfg(0, halo->skill_level_list[0] + 1);
		if (NULL != skill_cfg && halo->grade >= skill_cfg->halo_grade)
		{
			halo->skill_level_list[0]++;
		}

		role->GetHalo()->ActiveImage(target_grade_cfg->active_image_id, false);
		role->GetHalo()->UseImage(target_grade_cfg->active_image_id);

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);

		if (target_grade_cfg->is_broadcast)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_halo_upgrade, 
				UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade, role->GetHalo()->GetMedalSkillLevel());
			if (sendlen > 0) 
			{
				if (target_grade_cfg->client_grade >= 5)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
				}
				else
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}

	role->GetHalo()->PrintHalo("HaloUpgrade", old_grade, old_bless_val);

	EventHandler::Instance().OnHaloUpgrade(role, old_grade, halo->grade);

	return true;
}

bool HaloFunction::GMResetGrade(Role *role)
{
	if (NULL == role) return false;

	HaloParam *halo = role->GetHalo()->GetHaloParam();
	if (NULL == halo)
	{
		return false;
	}

	halo->grade = 1;
	halo->grade_bless_val = 0;

	role->GetHalo()->SendHaloInfo();

	return true;
}

bool HaloFunction::ActiveHalo(Role *role, bool is_recal)
{
	if (NULL == role) return false;

	// 检查等级
	HaloManager *halo_mng = role->GetHalo();
	if (NULL == halo_mng)
	{
		return false;
	}

	HaloParam *halo = halo_mng->GetHaloParam();
	if (NULL == halo)
	{
		return false;
	}

	if (!halo->IsHaloActived())
	{
		halo->star_level = 0;
		halo->level = role->GetLevel();
		halo->grade = 1;
		halo->grade_bless_val = 0;

		const HaloGradeConfig *grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(1);
		if (NULL != grade_cfg)
		{
			halo_mng->ActiveImage(grade_cfg->active_image_id, false);
			halo_mng->UseImage(grade_cfg->active_image_id);
		}

		const HaloSkillConifg *skill_cfg = LOGIC_CONFIG->GetHaloConfig().GetSkillCfg(0, 1);
		if (NULL != skill_cfg && halo->grade >= skill_cfg->halo_grade)
		{
			halo->skill_level_list[0] = 1;
		}

		if (is_recal)
		{
			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
		}

		halo_mng->SendHaloInfo();

		halo_mng->PrintHalo("ActiveHalo");

		role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_HALO, 0, 0);
		JinjiesysFuncHelper::Instance().OnActiveJinjieSys(role, JINJIE_TYPE_HALO);
	}

	return true;
}

bool HaloFunction::HaloSpecialImgUpgrade(Role *role, short special_img_id)
{
	if (NULL == role)
	{
		return false;
	}

	if (special_img_id <= 0 || special_img_id >= HaloParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return false;
	}

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	HaloManager *halo_mng = role->GetHalo();
	if (NULL == halo_mng)
	{
		return false;
	}

	HaloParam *halo = halo_mng->GetHaloParam();
	if (NULL == halo)
	{
		return false;
	}

	if (!halo->IsSpecialImageActive(special_img_id))
	{
		role->NoticeNum(errornum::EN_HALO_IMG_NOT_ACTIVE);
		return false;
	}

	const int old_grade = halo->special_img_grade_list[special_img_id];
	if (old_grade >= HaloParam::MAX_UPGRADE_LIMIT)
	{
		role->NoticeNum(errornum::EN_HALO_SPECIAL_IMG_GRADE_LIMIT);
		return false;
	}

	const HaloSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetHaloConfig().GetSpecialImgUpgradeCfg(special_img_id, old_grade + 1);
	if (NULL == upgrade_cfg)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	{
		// 进阶符
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = upgrade_cfg->stuff_id;
		stuff_list[0].num = upgrade_cfg->stuff_num;
		stuff_list[0].buyable = false;

		if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
		{
			role->NoticeNum(errornum::EN_ITEM_FUNC_ITEM_COMPOSE_NOT_ENOUGH);
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(role, consume_list, "HaloFunction::HaloSpecialImgUpgrade"))
	{
		return false;
	}

	// 生效
	++ halo->special_img_grade_list[special_img_id];

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);

	role->GetHalo()->SendHaloInfo();

	role->GetHalo()->PrintHalo("HaloSpecialImgUpgrade", old_grade, halo->special_img_grade_list[special_img_id]);

	return true;
}

bool HaloFunction::UplevelEquip(Role *role, int equip_idx)
{	
	if (NULL == role || equip_idx < 0 || equip_idx >= HaloParam::EQUIP_COUNT)
	{
		return false;
	}

	if (role->GetSublock()->CheckHasLock())
	{
		return false;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	HaloManager *halo_mng = role->GetHalo();

	HaloParam *halo = halo_mng->GetHaloParam();
	if (NULL == halo)
	{
		return false;
	}

	if (!halo->IsHaloActived())
	{
		return false;
	}

	const HaloEquipConifg *equip_cfg = LOGIC_CONFIG->GetHaloConfig().GetEquipCfg(equip_idx);
	if (NULL == equip_cfg)
	{
		return false;
	}

	short level = halo->equip_level_list[equip_idx];
	//if (level >= equip_cfg->max_level - 1)
	//{
	//	role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
	//	return false;
	//}

	if (level < 0)
	{
		return false;
	}

	//策划需求，装备升级级数由光环阶数决定
	const HaloGradeConfig * cur_grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(halo->grade);
	if (cur_grade_cfg == NULL)
	{
		return false;
	}

	if (level >= cur_grade_cfg->equip_level_toplimit)
	{
		role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
		return false;
	}

	if (halo->grade < LOGIC_CONFIG->GetHaloConfig().GetActiveEquipGrade())
	{
		return false;
	}

	short old_level = level;

	// 消耗
	if (!role->GetKnapsack()->ConsumeItem(equip_cfg->level_cfg[level].item.item_id, equip_cfg->level_cfg[level].item.num, "HaloEquipUplevel"))
	{
		return false;
	}

	// 结算

	halo->equip_level_list[equip_idx]++;

	bool is_active_upgrade_skill = false;
	int min_equip_level = halo_mng->RefreshHaloMinEquipLevel();
	if (LOGIC_CONFIG->GetHaloConfig().GetActiveEquipSkillLevel() <= min_equip_level)
	{
		int equip_skill_level = LOGIC_CONFIG->GetHaloConfig().GetEquipSkillLevel(min_equip_level);
		if (equip_skill_level > 0)
		{
			halo->equip_skill_level = equip_skill_level;
			is_active_upgrade_skill = true;
		}
	}

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
	role->GetHalo()->SendHaloInfo();

	if (is_active_upgrade_skill)
	{
		CommonDataParam *common_data = role->GetCommonDataParam();
		if (NULL != common_data)
		{
			if (0 == (common_data->upgrade_skill_skill_active_flag & (1 << UpGradeSkillConfig::SKILL_TYPE_HALO)))
			{
				common_data->upgrade_skill_skill_active_flag |= (1 << UpGradeSkillConfig::SKILL_TYPE_HALO);
				if (-1 == common_data->upgrade_skill_next_skill_index)
				{
					common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_HALO;
				}

				role->SendGradeSkillInfo(role);
			}
		}
	}

	halo_mng->PrintHalo("UplevelEquip", old_level);

	return true;
}

bool HaloFunction::UplevelSkill(Role *role, int skill_idx, bool is_auto_buy)
{
	if (NULL == role || skill_idx < 0 || skill_idx >= HaloParam::SKILL_COUNT)
	{
		return false;
	}

	if (role->GetSublock()->CheckHasLock())
	{
		return false;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	HaloManager *halo_mng = role->GetHalo();

	HaloParam *halo = halo_mng->GetHaloParam();
	if (NULL == halo)
	{
		return false;
	}

	if (!halo->IsHaloActived())
	{
		role->NoticeNum(errornum::EN_HALO_NOT_EXIST);
		return false;
	}

	const HaloSkillConifg *target_skill_cfg = LOGIC_CONFIG->GetHaloConfig().GetSkillCfg(skill_idx, halo->skill_level_list[skill_idx] + 1);
	if (NULL == target_skill_cfg)
	{
		return false;
	}

	if (0 == target_skill_cfg->skill_type)
	{
		return false;
	}

	if (target_skill_cfg->halo_grade > halo->grade)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	{
		// 升级材料
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = target_skill_cfg->uplevel_stuff_id;
		stuff_list[0].num = target_skill_cfg->uplevel_stuff_num;
		stuff_list[0].buyable = is_auto_buy;

		if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(role, consume_list, "HaloFunction::UplevelSkill"))
	{
		return false;
	}

	short skill_level = halo->skill_level_list[skill_idx];
	halo->skill_level_list[skill_idx]++;

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
	role->GetHalo()->SendHaloInfo();

	halo_mng->PrintHalo("UplevelSkill", skill_idx, skill_level);

	return true;
}

bool HaloFunction::UpStarLevel(Role *role,Protocol::CSHaloUpStarLevel *msg)
{
	if (NULL == msg || NULL == role) return false;

	if (msg->stuff_index < 0 || msg->stuff_index >= HaloParam::MAX_UP_STAR_ITEM_COUNT)
		return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (role->GetSublock()->CheckHasLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	HaloManager *halo_mng = role->GetHalo();
	if (NULL == halo_mng)
	{
		return false;
	}

	HaloParam *halo = halo_mng->GetHaloParam();
	if (NULL == halo)
	{
		return false;
	}

	if (!halo->IsHaloActived())
	{
		role->NoticeNum(errornum::EN_HALO_NOT_EXIST);
		return false;
	}

	short old_star_level = halo->star_level;
	short old_grade = halo->grade;
	int old_bless_val = halo->grade_bless_val;

	const HaloUpStarExpCfg * curr_star_level_cfg = LOGIC_CONFIG->GetHaloConfig().GetUpStarExpCfg(old_star_level);
	if (NULL == curr_star_level_cfg)
	{
		return false;
	}

	const HaloUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetHaloConfig().GetUpStarExpCfg(old_star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return false;
	}

	const HaloUpStarStuffCfg * up_star_stuff_cfg = LOGIC_CONFIG->GetHaloConfig().GetUpStarStuffCfg(msg->stuff_index);
	if (NULL == up_star_stuff_cfg)
	{
		return false;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(up_star_stuff_cfg->up_star_item_id);
	if (NULL == item_base)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[0].item_id = up_star_stuff_cfg->up_star_item_id;
	stuff_list[0].num = 1;
	stuff_list[0].buyable = 1 == msg->is_auto_buy ? true : false;

	if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
	{
		return false;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(role, consume_list, "HaloFunction::UpStarLevel"))
	{
		return false;
	}

	// 结算
	{
		int rand_value = RandomNum(10000);
		if (rand_value < up_star_stuff_cfg->baoji_per)
		{
			halo->grade_bless_val += up_star_stuff_cfg->star_exp * up_star_stuff_cfg->baoji_mul;
		}
		else
		{
			halo->grade_bless_val += up_star_stuff_cfg->star_exp;
		}

		int up_star_need_exp = curr_star_level_cfg->up_star_level_exp;
		while(halo->grade_bless_val >= up_star_need_exp)
		{	
			const HaloUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetHaloConfig().GetUpStarExpCfg(halo->star_level + 1);
			if (NULL == next_star_level_cfg || up_star_need_exp <= 0)
			{
				halo->grade_bless_val = 0;
				break;
			}

			halo->grade_bless_val -= up_star_need_exp;
			halo->star_level++;

			if ((halo->grade - 1) != (halo->star_level / 10) && (halo->grade - 1)  < (halo->star_level / 10))
			{
				halo->grade = (halo->star_level / 10) + 1;

				const HaloGradeConfig * grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(halo->grade);
				if (NULL != grade_cfg)
				{
					role->GetHalo()->ActiveImage(grade_cfg->active_image_id, false);
					role->GetHalo()->UseImage(grade_cfg->active_image_id);
					if (grade_cfg->is_broadcast)
					{
						int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_halo_upgrade, 
							UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), grade_cfg->client_grade, grade_cfg->grade, role->GetHalo()->GetMedalSkillLevel());
						if (sendlen > 0) 
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
						}
					}
				}
				
				const HaloSkillConifg *skill_cfg = LOGIC_CONFIG->GetHaloConfig().GetSkillCfg(0, halo->skill_level_list[0] + 1);
				if (NULL != skill_cfg && halo->grade >= skill_cfg->halo_grade)
				{
					halo->skill_level_list[0]++;
				}		
			}

			up_star_need_exp = next_star_level_cfg->up_star_level_exp;
		}

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);			//形象赋灵增加百分比进阶属性
	}

	role->GetHalo()->SendHaloInfo();

	role->GetHalo()->PrintHalo("HaloUpgrade", old_star_level, old_bless_val);

	EventHandler::Instance().OnHaloUpgrade(role, old_grade, halo->grade);
	EventHandler::Instance().OnHaloUpStar(role, old_star_level, halo->star_level);

	return true;

}

int HaloFunction::UplevelAndSaveBless(Role *role, int add_star)
{
	HaloManager *halo_mng = role->GetHalo();
	if (NULL == halo_mng)
	{
		return -1;
	}

	HaloParam *halo = halo_mng->GetHaloParam();
	if (NULL == halo)
	{
		return -2;
	}

	if (!halo->IsHaloActived())
	{
		return -3;
	}

	short old_star_level = halo->star_level;
	short old_grade = halo->grade;
	int old_bless_val = halo->grade_bless_val;

	const HaloUpStarExpCfg * curr_star_level_cfg = LOGIC_CONFIG->GetHaloConfig().GetUpStarExpCfg(old_star_level);
	if (NULL == curr_star_level_cfg)
	{
		return -4;
	}

	const HaloUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetHaloConfig().GetUpStarExpCfg(old_star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return -5;
	}

	{
		halo->star_level += add_star;
		halo->grade_bless_val = old_bless_val;		// 升星助力保留祝福值

		if ((halo->grade - 1) != (halo->star_level / 10) && (halo->grade - 1) < (halo->star_level / 10))
		{
			halo->grade = (halo->star_level / 10) + 1;

			const HaloGradeConfig * grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(halo->grade);
			if (NULL != grade_cfg)
			{
				role->GetHalo()->ActiveImage(grade_cfg->active_image_id, false);
				role->GetHalo()->UseImage(grade_cfg->active_image_id);
				if (grade_cfg->is_broadcast)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_halo_upgrade,
						UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), grade_cfg->client_grade, grade_cfg->grade, role->GetHalo()->GetMedalSkillLevel());
					if (sendlen > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
					}
				}
			}

			const HaloSkillConifg *skill_cfg = LOGIC_CONFIG->GetHaloConfig().GetSkillCfg(0, halo->skill_level_list[0] + 1);
			if (NULL != skill_cfg && halo->grade >= skill_cfg->halo_grade)
			{
				halo->skill_level_list[0]++;
			}
		}

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);			//形象赋灵增加百分比进阶属性
	}

	role->GetHalo()->SendHaloInfo();

	role->GetHalo()->PrintHalo("HaloUpgrade", old_star_level, old_bless_val);

	EventHandler::Instance().OnHaloUpgrade(role, old_grade, halo->grade);
	EventHandler::Instance().OnHaloUpStar(role, old_star_level, halo->star_level);

	return halo->star_level;
}

bool HaloFunction::CheckCanUpGrade(Role* role)
{
	if (NULL == role)
	{
		return false;
	}

	HaloParam* halo_param = role->GetHalo()->GetHaloParam();
	if (NULL == halo_param)
	{
		return false;
	}

	if (!halo_param->IsHaloActived())
	{
		return false;
	}

	const HaloUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetHaloConfig().GetUpStarExpCfg(halo_param->star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return false;
	}

	return true;
}

