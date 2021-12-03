#include "yaoshifunction.hpp"
#include "yaoshimanager.hpp"
#include "yaoshiconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "other/upgradeskill/upgradeskillconfig.hpp"
#include "obj/character/role.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/errornum.h"
#include "servercommon/struct/yaoshiparam.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/commondata.hpp"
#include "protocal/msgyaoshi.hpp"
#include "world.h"
#include "gstr.h"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "other/sublock/sublock.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "servercommon/activitydef.hpp"
#include "other/upgradesyscrit/upgradesyscrit.hpp"

static const int CLEAR_BLESS_VAL_TIME_HOUR = 5;

bool YaoShiFunction::Upgrade(Role *role, bool is_auto_buy, int repeat_times)
{
	if (nullptr == role)
	{
		return false;
	}

	Knapsack *knapsack = role->GetKnapsack();
	if (nullptr == knapsack)
	{
		return false;
	}

	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	YaoShiManager *yaoshi_mng = role->GetYaoShiManager();
	if(nullptr == yaoshi_mng)
	{
		return false;
	}

	YaoShiParam *yaoshi = yaoshi_mng->GetYaoShiParam();
	if (nullptr == yaoshi)
	{
		return false;
	}

	if (!yaoshi->IsYaoShiActived())
	{
		role->NoticeNum(errornum::EN_YAOSHI_NOT_EXIST);
		return false;
	}

	short old_grade = yaoshi->grade;
	short oldest_bless_val = yaoshi->grade_bless_val;
	bool succeeded = false;

	const YaoShiGradeConfig *curr_grade_cfg = YAOSHI_CFG().GetGradeConfig(old_grade);
	if (nullptr == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_YAOSHI_CANNOT_UPGRADE);
		return false;
	}

	const YaoShiGradeConfig *target_grade_cfg = YAOSHI_CFG().GetGradeConfig(old_grade + 1);
	if (nullptr == target_grade_cfg)
	{
		role->NoticeNum(errornum::EN_YAOSHI_CANNOT_UPGRADE);
		return false;
	}

	for (int i = 0; i < repeat_times; ++i)
	{
		short old_bless_val = yaoshi->grade_bless_val;
		const YaoShiUpGradeProbConfig *upgrade_prob_cfg = YAOSHI_CFG().GetUpGradeProbConfig(old_grade, old_bless_val);
		if (nullptr == upgrade_prob_cfg)
		{
			role->NoticeNum(errornum::EN_YAOSHI_CANNOT_UPGRADE);
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

		if (consume_list.count <= 0 && consume_list.need_gold_num <= 0 && consume_list.need_bind_gold_num <= 0)
		{
			//return false;
			succeeded = true;
			break;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(role, consume_list, "YaoShiFunction::Upgrade"))
		{
			succeeded = true;
			break;
		}

		{
			// 生效
			int rand_val = RandomNum(10000);
			
			if (rand_val < upgrade_prob_cfg->succ_rate || yaoshi->grade_bless_val >= curr_grade_cfg->bless_val_limit)
			{
				succeeded = true;
			}

			if (!succeeded)
			{
				// 记录进阶第一次增加祝福值的时间，用于判断清0祝福值,大于5阶时使用
				if (1 == curr_grade_cfg->is_clear_bless && 0 == yaoshi->clear_upgrade_timestamp)
				{
					unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					yaoshi->clear_upgrade_timestamp = now_sec + EngineAdapter::Instance().NextDayInterval(CLEAR_BLESS_VAL_TIME_HOUR, 0, 0);
				}
				int crit_value = UpgradeSysCrit::Instance().CalculateCritValue(role, UPGRADE_SYS_CRIT_TYPE_YAOSHI);
				yaoshi->grade_bless_val += (upgrade_prob_cfg->RandAddBlessVal() * crit_value);

				if (yaoshi->grade_bless_val > curr_grade_cfg->bless_val_limit)
				{
					yaoshi->grade_bless_val = curr_grade_cfg->bless_val_limit;
				}

				//role->ReCalcAttr(CharIntAttrs::RECALC_REASON_YAOSHI);
			}
			else
			{
				yaoshi->grade++;
				yaoshi->grade_bless_val = 0;
				yaoshi->clear_upgrade_timestamp = 0;

				const YaoShiSkillConifg *skill_cfg = YAOSHI_CFG().GetSkillCfg(0, yaoshi->skill_level_list[0] + 1);
				if (nullptr != skill_cfg && yaoshi->grade >= skill_cfg->yaoshi_grade)
				{
					yaoshi->skill_level_list[0]++;
				}

				role->GetYaoShiManager()->ActiveImage(target_grade_cfg->active_image_id, false);
				role->GetYaoShiManager()->UseImage(target_grade_cfg->active_image_id);

				role->ReCalcAttr(CharIntAttrs::RECALC_REASON_YAOSHI);

				if (target_grade_cfg->is_broadcast)
				{
					int sendlen = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "yaoshi_upgrade", UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade);
					if (sendlen > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
					}
				}
				//role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_YAOSHI, yaoshi->grade);   // 进阶丹返还活动
				break;
			}
		}
	}
	role->GetYaoShiManager()->SendYaoShiInfo();
	role->GetYaoShiManager()->PrintYaoShi("Upgrade", old_grade, oldest_bless_val);

	EventHandler::Instance().OnYaoShiUpgrade(role, old_grade, yaoshi->grade);

	return !succeeded;
}

bool YaoShiFunction::UpOneGrade(Role *role, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num)
{
	if (nullptr == role)
	{
		return false;
	}

	YaoShiManager *yaoshi_mng = role->GetYaoShiManager();
	if (nullptr == yaoshi_mng)
	{
		return false;
	}

	YaoShiParam *yaoshi = yaoshi_mng->GetYaoShiParam();
	if (nullptr == yaoshi)
	{
		return false;
	}

	if (!yaoshi->IsYaoShiActived())
	{
		role->NoticeNum(errornum::EN_YAOSHI_NOT_EXIST);
		return false;
	}

	if (yaoshi->grade < valid_min_grade)
	{
		role->NoticeNum(errornum::EN_YAOSHI_UP_ONE_GRADE_LIMIT);
		return false;
	}
	else if (yaoshi->grade > valid_max_grade)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(return_item_id);
		if (nullptr != item_base)
		{
			for (int loop_time = 0; return_item_num > 0 && loop_time < 1000; ++loop_time)
			{
				int put_num = return_item_num > item_base->GetPileLimit() ? item_base->GetPileLimit() : return_item_num;
				return_item_num -= put_num;

				ItemConfigData return_item(return_item_id, true, put_num);
				role->GetKnapsack()->PutOrMail(return_item, PUT_REASON_YAOSHI_UPGRADE_ONE_RETURN);
			}

			return true;
		}

		return false;
	}

	int old_grade = yaoshi->grade;
	int old_bless_val = yaoshi->grade_bless_val;

	const YaoShiGradeConfig *curr_grade_cfg = YAOSHI_CFG().GetGradeConfig(old_grade);
	if (nullptr == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_YAOSHI_CANNOT_UPGRADE);
		return false;
	}

	const YaoShiGradeConfig * target_grade_cfg = YAOSHI_CFG().GetGradeConfig(old_grade + 1);
	if (nullptr == target_grade_cfg)
	{
		role->NoticeNum(errornum::EN_YAOSHI_CANNOT_UPGRADE);
		return false;
	}

	{
		yaoshi->grade++;
		yaoshi->grade_bless_val = 0;
		yaoshi->clear_upgrade_timestamp = 0;

		const YaoShiSkillConifg *skill_cfg = YAOSHI_CFG().GetSkillCfg(0, yaoshi->skill_level_list[0] + 1);
		if (nullptr != skill_cfg && yaoshi->grade >= skill_cfg->yaoshi_grade)
		{
			yaoshi->skill_level_list[0]++;
		}

		role->GetYaoShiManager()->ActiveImage(target_grade_cfg->active_image_id, false);
		role->GetYaoShiManager()->UseImage(target_grade_cfg->active_image_id);

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_YAOSHI);

		if (target_grade_cfg->is_broadcast)
		{
			int sendlen = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "yaoshi_upgrade", UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade);
			if (sendlen > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	role->GetYaoShiManager()->SendYaoShiInfo();
	role->GetYaoShiManager()->PrintYaoShi(__FUNCTION__, old_grade, old_bless_val);

	EventHandler::Instance().OnYaoShiUpgrade(role, old_grade, yaoshi->grade);

	return true;
}

bool YaoShiFunction::GMResetGrade(Role *role)
{
	if (nullptr == role) return false;

	YaoShiParam *yaoshi = role->GetYaoShiManager()->GetYaoShiParam();
	if (nullptr == yaoshi)
	{
		return false;
	}

	yaoshi->grade = 1;
	yaoshi->grade_bless_val = 0;

	role->GetYaoShiManager()->SendYaoShiInfo();

	return true;
}

bool YaoShiFunction::ActiveYaoShi(Role *role, bool is_recal)
{
	if (nullptr == role) return false;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return false;
	}

	// 检查等级
	YaoShiManager *yaoshi_mng = role->GetYaoShiManager();

	YaoShiParam *yaoshi = yaoshi_mng->GetYaoShiParam();
	if (nullptr == yaoshi)
	{
		return false;
	}
	if (!yaoshi->IsYaoShiActived())
	{
		yaoshi->level = role->GetLevel();
		yaoshi->grade = 1;
		yaoshi->grade_bless_val = 0;
		yaoshi->star_level = 0;

		const YaoShiGradeConfig *grade_cfg = YAOSHI_CFG().GetGradeConfig(1);
		if (nullptr != grade_cfg)
		{
			yaoshi_mng->ActiveImage(grade_cfg->active_image_id, false);
			yaoshi_mng->UseImage(grade_cfg->active_image_id);
		}

		const YaoShiSkillConifg *skill_cfg = YAOSHI_CFG().GetSkillCfg(0, 1);
		if (nullptr != skill_cfg && yaoshi->grade >= skill_cfg->yaoshi_grade && (0 == yaoshi->skill_level_list[0]))
		{
			yaoshi->skill_level_list[0] = 1;
		}
		
		if (is_recal)
		{
			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_YAOSHI);
		}

		yaoshi_mng->SendYaoShiInfo();

		yaoshi_mng->PrintYaoShi("ActiveYaoShi");
		JinjiesysFuncHelper::Instance().OnActiveJinjieSys(role, JINJIE_TYPE_YAOSHI);

		//role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_YAOSHI, 0, 0);
	}

	return true;
}

bool YaoShiFunction::YaoShiSpecialImgUpgrade(Role *role, short special_img_id)
{
	if (nullptr == role)
	{
		return false;
	}

	if (special_img_id <= 0 || special_img_id >= YaoShiParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return false;
	}

	Knapsack *knapsack = role->GetKnapsack();
	if (nullptr == knapsack)
	{
		return false;
	}
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	YaoShiManager *yaoshi_mng = role->GetYaoShiManager();
	if (nullptr == yaoshi_mng)
	{
		return false;
	}

	YaoShiParam *yaoshi = yaoshi_mng->GetYaoShiParam();
	if (nullptr == yaoshi)
	{
		return false;
	}

	if (!yaoshi->IsSpecialImageActive(special_img_id))
	{
		role->NoticeNum(errornum::EN_YAOSHI_IMG_NOT_ACTIVE);
		return false;
	}

	const int old_grade = yaoshi->special_img_grade_list[special_img_id];
	if (old_grade >= YaoShiParam::MAX_UPGRADE_LIMIT)
	{
		role->NoticeNum(errornum::EN_YAOSHI_SPECIAL_IMG_GRADE_LIMIT);
		return false;
	}

	const YaoShiSpecialImgUpgradeCfg *upgrade_cfg = YAOSHI_CFG().GetSpecialImgUpgradeCfg(special_img_id, old_grade + 1);
	if (nullptr == upgrade_cfg)
	{
		role->NoticeNum(errornum::EN_YAOSHI_SPECIAL_IMG_GRADE_LIMIT);
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
	if (!ItemExtern::ConsumeItem(role, consume_list, "YaoShiFunction::YaoShiSpecialImgUpgrade"))
	{
		return false;
	}

	// 生效
	++ yaoshi->special_img_grade_list[special_img_id];

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_YAOSHI);

	role->GetYaoShiManager()->SendYaoShiInfo();

	role->GetYaoShiManager()->PrintYaoShi("YaoShiSpecialImgUpgrade", old_grade, yaoshi->special_img_grade_list[special_img_id]);

	return true;
}

bool YaoShiFunction::UplevelEquip(Role *role, int equip_idx)
{
	//if (nullptr == role || equip_idx < 0 || equip_idx >= YaoShiParam::EQUIP_COUNT)
	//{
	//	return false;
	//}

	//if (role->GetSublock()->CheckHasLock())
	//{
	//	return false;
	//}

	//if (role->GetKnapsack()->IsLock())
	//{
	//	role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
	//	return false;
	//}

	//YaoShiManager *yaoshi_mng = role->GetYaoShiManager();
	//if (nullptr == yaoshi_mng)
	//{
	//	return false;
	//}

	//YaoShiParam *yaoshi = yaoshi_mng->GetYaoShiParam();
	//if (nullptr == yaoshi)
	//{
	//	return false;
	//}

	//if (!yaoshi->IsYaoShiActived())
	//{
	//	role->NoticeNum(errornum::EN_YAOSHI_NOT_ACTIVE);
	//	return false;
	//}

	//const YaoShiEquipConifg *equip_cfg = YAOSHI_CFG().GetEquipCfg(equip_idx);
	//if (nullptr == equip_cfg)
	//{
	//	return false;
	//}

	//short level = yaoshi->equip_level_list[equip_idx];
	//if (level >= equip_cfg->max_level - 1)
	//{
	//	role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
	//	return false;
	//}

	//if (level < 0)
	//{
	//	return false;
	//}

	//if (yaoshi->grade < YAOSHI_CFG().GetActiveEquipGrade())
	//{
	//	return false;
	//}

	//short old_level = level;

	//// 消耗
	//if (!role->GetKnapsack()->ConsumeItem(equip_cfg->level_cfg[level].item.item_id, equip_cfg->level_cfg[level].item.num, __FUNCTION__))
	//{
	//	return false;
	//}

	//// 结算

	//yaoshi->equip_level_list[equip_idx]++;

	//bool is_active_upgrade_skill = false;
	//int min_equip_level = yaoshi_mng->RefreshYaoShiMinEquipLevel();
	//if (YAOSHI_CFG().GetActiveEquipSkillLevel() <= min_equip_level)
	//{
	//	int equip_skill_level = YAOSHI_CFG().GetEquipSkillLevel(min_equip_level);
	//	if (equip_skill_level > 0 && equip_skill_level > yaoshi->equip_skill_level)
	//	{
	//		yaoshi->equip_skill_level = equip_skill_level;
	//		is_active_upgrade_skill = true;
	//	}
	//}

	//role->ReCalcAttr(CharIntAttrs::RECALC_REASON_YAOSHI);
	//role->GetYaoShiManager()->SendYaoShiInfo();

	//if (is_active_upgrade_skill)
	//{
	//	CommonDataParam *common_data = role->GetCommonDataParam();
	//	if (nullptr != common_data)
	//	{
	//		//if (0 == (common_data->upgrade_skill_skill_active_flag & (1 << UpGradeSkillConfig::SKILL_TYPE_YAOSHI)))
	//		//{
	//		//	common_data->upgrade_skill_skill_active_flag |= (1 << UpGradeSkillConfig::SKILL_TYPE_YAOSHI);
	//		//	if (-1 == common_data->upgrade_skill_next_skill_index)
	//		//	{
	//		//		common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_YAOSHI;
	//		//	}
	//		//	
	//		//	role->SendGradeSkillInfo(role);
	//		//}
	//	}
	//}

	//yaoshi_mng->PrintYaoShi("UplevelEquip", old_level);

	//return true;

	return false;
}

bool YaoShiFunction::UplevelSkill(Role *role, int skill_idx, bool is_auto_buy)
{
	if (nullptr == role || skill_idx < 0 || skill_idx >= YaoShiParam::SKILL_COUNT)
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

	YaoShiManager *yaoshi_mng = role->GetYaoShiManager();
	if (nullptr == yaoshi_mng)
	{
		return false;
	}

	YaoShiParam *yaoshi = yaoshi_mng->GetYaoShiParam();
	if (nullptr == yaoshi)
	{
		return false;
	}

	if (!yaoshi->IsYaoShiActived())
	{
		role->NoticeNum(errornum::EN_YAOSHI_NOT_EXIST);
		return false;
	}

	const YaoShiSkillConifg *target_skill_cfg = YAOSHI_CFG().GetSkillCfg(skill_idx, yaoshi->skill_level_list[skill_idx] + 1);
	if (nullptr == target_skill_cfg)
	{
		return false;
	}

	if (0 == target_skill_cfg->skill_type)
	{
		return false;
	}

	if (target_skill_cfg->yaoshi_grade > yaoshi->grade)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();
	
	{
		// 升级材料
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = target_skill_cfg->uplevel_stuff_id;
		stuff_list[0].num = target_skill_cfg->uplevel_stuff_num;
		stuff_list[0].buyable = is_auto_buy ? true : false;

		if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
		{
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(role, consume_list, "YaoShiFunction::UplevelSkill"))
		{
			return false;
		}
	}

 	short skill_level = yaoshi->skill_level_list[skill_idx];
	yaoshi->skill_level_list[skill_idx]++;

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_YAOSHI);
	role->GetYaoShiManager()->SendYaoShiInfo();

	yaoshi_mng->PrintYaoShi("UplevelSkill", skill_idx, skill_level);

	return true;
}

