#include "shenyifunction.hpp"
#include "shenyimanager.hpp"
#include "shenyiconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "other/upgradeskill/upgradeskillconfig.hpp"
#include "obj/character/role.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/errornum.h"
#include "servercommon/shenyidef.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/commondata.hpp"
#include "world.h"
#include "gamelog.h"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "other/sublock/sublock.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/upgradesyscrit/upgradesyscrit.hpp"

bool ShenyiFunction::Upgrade(Role *role, bool is_auto_buy, int repeat_times, int *old_level, int *coin_cost, int *gold_cost)
{
 	if (NULL == role) return false;
 
 	Knapsack *knapsack = role->GetKnapsack();
 	if (knapsack->IsLock())
 	{
 		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
 		return false;
 	}
 
 	ShenyiManager *shenyi_mng = role->GetShenyi();
 
 	ShenyiParam *shenyi = shenyi_mng->GetShenyiParam();
 	if (NULL == shenyi)
 	{
 		role->NoticeNum(errornum::EN_SHENYI_NOT_EXIST);
 		return false;
 	}
 
 	if (!shenyi->IsShenyiActived())
 	{
 		return false;
 	}
 
 	short old_grade = shenyi->grade;
 	short oldest_bless_val = shenyi->grade_bless_val;
	bool succeeded = false;
 
 	if (NULL != old_level)
 	{
 		*old_level = old_grade;
 	}
 
 	const ShenyiGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(old_grade);
 	if (NULL == curr_grade_cfg)
 	{
 		role->NoticeNum(errornum::EN_SHENYI_CANNOT_UPGRADE);
 		return false;
 	}
 
 	const ShenyiGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(old_grade + 1);
 	if (NULL == target_grade_cfg)
 	{
 		role->NoticeNum(errornum::EN_SHENYI_CANNOT_UPGRADE);
 		return false;
 	}
 
	for (int i = 0; i < repeat_times; ++i)
	{
		short old_bless_val = shenyi->grade_bless_val;
		const ShenyiUpGradeProbConfig * upgrade_prob_cfg = LOGIC_CONFIG->GetShenyiConfig().GetUpGradeProbConfig(old_grade, old_bless_val);
		if (NULL == upgrade_prob_cfg)
		{
			role->NoticeNum(errornum::EN_SHENYI_CANNOT_UPGRADE);
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
				stuff_list[stuff_count].buyable = is_auto_buy ? true : false;

				++stuff_count;
			}
			// 如果限时进阶丹用完了，还不够一次批量使用，就使用普通进阶丹
			if (upgrade_stuff_num < curr_grade_cfg->upgrade_stuff_count && stuff_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
			{
				stuff_list[stuff_count].item_id = curr_grade_cfg->upgrade_stuff_id;
				stuff_list[stuff_count].num = curr_grade_cfg->upgrade_stuff_count - upgrade_stuff_num;
				stuff_list[stuff_count].buyable = is_auto_buy ? true : false;

				++stuff_count;
			}

			if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
			{
				//return false;
				succeeded = true;
				break;
			}
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(role, consume_list, "ShenyiFunction::Upgrade"))
		{
			succeeded = true;
			break;
		}

		int use_coin = curr_grade_cfg->coin;

		{
			// 检查金币和元宝
			Money *money = knapsack->GetMoney();
			if (!money->AllCoinMoreThan(use_coin))
			{
				role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
				//return false;
				succeeded = true;
				break;
			}
		}

		{
			// 铜钱
			if (use_coin > 0)
			{
				if (!knapsack->GetMoney()->UseAllCoin(use_coin, "ShenyiUpgrade"))
				{
					//return false;
					succeeded = true;
					break;
				}
			}

			if (NULL != coin_cost) *coin_cost = use_coin;
		}

		{
			// 生效
			int rand_val = RandomNum(10000);
			
			if (rand_val < upgrade_prob_cfg->succ_rate || shenyi->grade_bless_val >= curr_grade_cfg->bless_val_limit)
			{
				succeeded = true;
			}

			if (!succeeded)
			{
				// 记录进阶第一次增加祝福值的时间，用于判断清0祝福值,大于5阶时使用
				if (1 == curr_grade_cfg->is_clear_bless && 0 == shenyi->clear_upgrade_time)
				{
					const int CLEAR_BLESS_VAL_TIME_HOUR = 5;
					unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					shenyi->clear_upgrade_time = now_sec + EngineAdapter::Instance().NextDayInterval(CLEAR_BLESS_VAL_TIME_HOUR, 0, 0);
				}

				int crit_value = UpgradeSysCrit::Instance().CalculateCritValue(role, UPGRADE_SYS_CRIT_TYPE_SHENYI);
			
				shenyi->grade_bless_val += (upgrade_prob_cfg->RandAddBlessVal()*crit_value);

				if (shenyi->grade_bless_val > curr_grade_cfg->bless_val_limit) shenyi->grade_bless_val = curr_grade_cfg->bless_val_limit;

				//role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);
			}
			else
			{
				shenyi->grade++;
				shenyi->grade_bless_val = 0;
				shenyi->clear_upgrade_time = 0;

				const ShenyiSkillConifg *skill_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSkillCfg(0, shenyi->skill_level_list[0] + 1);
				if (NULL != skill_cfg && shenyi->grade >= skill_cfg->shenyi_grade)
				{
					shenyi->skill_level_list[0]++;
				}

				role->GetShenyi()->ActiveImage(target_grade_cfg->active_image_id, false);
				role->GetShenyi()->UseImage(target_grade_cfg->active_image_id);

				role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);

				if (target_grade_cfg->is_broadcast)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenyi_upgrade,
						UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade);
					if (sendlen > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
					}
				}
				break;
			}
		}
	}
	role->GetShenyi()->SendShenyiInfo();
 
 	role->GetShenyi()->PrintShenyi("ShenyiUpgrade", old_grade, oldest_bless_val);
 
 	EventHandler::Instance().OnShenyiUpgrade(role, old_grade, shenyi->grade);
 
 	return !succeeded;
}

bool ShenyiFunction::UpOneGrade(Role *role, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num)
{
	ShenyiManager *shenyi_mng = role->GetShenyi();
	if (NULL == shenyi_mng)
	{
		return false;
	}

	ShenyiParam *shenyi_param = shenyi_mng->GetShenyiParam();
	if (NULL == shenyi_param)
	{
		return false;
	}

	if (!shenyi_param->IsShenyiActived())
	{
		role->NoticeNum(errornum::EN_SHENYI_NOT_EXIST);
		return false;
	}

	if (shenyi_param->grade < valid_min_grade)
	{
		role->NoticeNum(errornum::EN_SHENYI_UP_ONE_GRADE_LIMIT);
		return false;
	}
	else if (shenyi_param->grade > valid_max_grade)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(return_item_id);
		if (item_base != nullptr)
		{
			for (int loop_time = 0; return_item_num > 0 && loop_time < 1000; ++loop_time)
			{
				int put_num = return_item_num > item_base->GetPileLimit() ? item_base->GetPileLimit() : return_item_num;
				return_item_num -= put_num;

				ItemConfigData return_item(return_item_id, true, put_num);
				role->GetKnapsack()->PutOrMail(return_item, PUT_REASON_SHENYI_UPGRADE_ONE_RETURN);
			}

			return true;
		}

		return false;
	}

	int old_grade = shenyi_param->grade;
	int old_bless_val = shenyi_param->grade_bless_val;

	const ShenyiGradeConfig *curr_grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_SHENYI_CANNOT_UPGRADE);
		return false;
	}

	const ShenyiGradeConfig *target_grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		role->NoticeNum(errornum::EN_SHENYI_CANNOT_UPGRADE);
		return false;
	}

	{
		shenyi_param->grade++;
		shenyi_param->grade_bless_val = 0;
		shenyi_param->clear_upgrade_time = 0;

		const ShenyiSkillConifg *skill_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSkillCfg(0, shenyi_param->skill_level_list[0] + 1);
		if (NULL != skill_cfg && shenyi_param->grade >= skill_cfg->shenyi_grade)
		{
			shenyi_param->skill_level_list[0]++;
		}

		role->GetShenyi()->ActiveImage(target_grade_cfg->active_image_id, false);
		role->GetShenyi()->UseImage(target_grade_cfg->active_image_id);

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);

		if (target_grade_cfg->is_broadcast)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenyi_upgrade, 
				UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade);
			if (sendlen > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	role->GetShenyi()->SendShenyiInfo();
	role->GetShenyi()->PrintShenyi("ShenyiUpgrade", old_grade, old_bless_val);

	EventHandler::Instance().OnShenyiUpgrade(role, old_grade, shenyi_param->grade);

	return true;
}

bool ShenyiFunction::GMResetGrade(Role *role)
{
	if (NULL == role) return false;

	ShenyiParam *shenyi = role->GetShenyi()->GetShenyiParam();
	if (NULL == shenyi)
	{
		return false;
	}

	shenyi->grade = 1;
	shenyi->grade_bless_val = 0;

	role->GetShenyi()->SendShenyiInfo();

	return true;
}

bool ShenyiFunction::ActiveShenyi(Role *role, bool is_recal)
{
	if (NULL == role) return false;

	// 检查等级
	ShenyiManager *shenyi_mng = role->GetShenyi();
	if (NULL == shenyi_mng)
	{
		return false;
	}

	ShenyiParam *shenyi = shenyi_mng->GetShenyiParam();
	if (NULL == shenyi)
	{
		return false;
	}

	if (!shenyi->IsShenyiActived())
	{
		shenyi->star_level = 0;
		shenyi->level = role->GetLevel();
		shenyi->grade = 1;
		shenyi->grade_bless_val = 0;

		const ShenyiGradeConfig *grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(1);
		if (NULL != grade_cfg)
		{
			shenyi_mng->ActiveImage(grade_cfg->active_image_id, false);
			shenyi_mng->UseImage(grade_cfg->active_image_id);
		}

		const ShenyiSkillConifg *skill_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSkillCfg(0, 1);
		if (NULL != skill_cfg && shenyi->grade >= skill_cfg->shenyi_grade && (0 == shenyi->skill_level_list[0]))
		{
			shenyi->skill_level_list[0] = 1;
		}

		if (is_recal)
		{
			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);
		}
	
		shenyi_mng->SendShenyiInfo();

		shenyi_mng->PrintShenyi("ActiveShenyi");

		role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_SHENYI, 0, 0);
		JinjiesysFuncHelper::Instance().OnActiveJinjieSys(role, JINJIE_TYPE_SHENYI);
	}

	return true;
}

bool ShenyiFunction::ShenyiSpecialImgUpgrade(Role *role, short special_img_id)
{
	if (NULL == role)
	{
		return false;
	}

	if (special_img_id <= 0 || special_img_id >= ShenyiParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return false;
	}

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	ShenyiManager *shenyi_mng = role->GetShenyi();
	if (NULL == shenyi_mng)
	{
		return false;
	}

	ShenyiParam *shenyi = shenyi_mng->GetShenyiParam();
	if (NULL == shenyi)
	{
		return false;
	}

	if (!shenyi->IsSpecialImageActive(special_img_id))
	{
		role->NoticeNum(errornum::EN_SHENYI_IMG_NOT_ACTIVE);
		return false;
	}

	const int old_grade = shenyi->special_img_grade_list[special_img_id];
	if (old_grade >= ShenyiParam::MAX_UPGRADE_LIMIT)
	{
		role->NoticeNum(errornum::EN_SHENYI_SPECIAL_IMG_GRADE_LIMIT);
		return false;
	}

	const ShenyiSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSpecialImgUpgradeCfg(special_img_id, old_grade + 1);
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
	if (!ItemExtern::ConsumeItem(role, consume_list, "ShenyiFunction::ShenyiSpecialImgUpgrade"))
	{
		return false;
	}

	// 生效
	++ shenyi->special_img_grade_list[special_img_id];

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);

	role->GetShenyi()->SendShenyiInfo();

	role->GetShenyi()->PrintShenyi("ShenyiSpecialImgUpgrade", old_grade, shenyi->special_img_grade_list[special_img_id]);

	return true;
}

bool ShenyiFunction::UplevelEquip(Role *role, int equip_idx)
{
 	if (NULL == role || equip_idx < 0 || equip_idx >= ShenyiParam::EQUIP_COUNT)
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
 
 	ShenyiManager *shenyi_mng = role->GetShenyi();
 
 	ShenyiParam *shenyi = shenyi_mng->GetShenyiParam();

 	if (NULL == shenyi)
 	{
 		role->NoticeNum(errornum::EN_SHENYI_NOT_EXIST);
 		return false;
 	}
 
 	if (!shenyi->IsShenyiActived())
 	{
 		return false;
 	}

	if (shenyi->grade < LOGIC_CONFIG->GetShenyiConfig().GetActiveEquipGrade())
	{
		return false;
	}

 	const ShenyiEquipConifg *equip_cfg = LOGIC_CONFIG->GetShenyiConfig().GetEquipCfg(equip_idx);
 	if (NULL == equip_cfg)
 	{
 		return false;
 	}
	
	short &equip_level = shenyi->equip_level_list[equip_idx];
	//if (equip_level >= equip_cfg->max_level - 1)
	//{
	//	role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
	//	return false;
	//}

	if (equip_level < 0)
	{
		return false;
	}

	//策划需求，装备升级级数由神翼阶数决定
	const ShenyiGradeConfig * cur_grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(shenyi->grade);
	if (cur_grade_cfg == NULL)
	{
		return false;
	}

	if (equip_level >= cur_grade_cfg->equip_level_toplimit)
	{
		role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
		return false;
	}

	{
		// 消耗
		ItemConfigData item = equip_cfg->equip_level_cfg_list[equip_level].item;
		if (!role->GetKnapsack()->ConsumeItem(item.item_id, item.num, "ShenyiEquipUpLevel"))
		{
			return false;
		}
	}

	short old_equip_level = equip_level;

	// 结算
	equip_level++;

	bool is_active_upgrade_skill = false;
	int min_equip_level = shenyi_mng->RefreshShenyiMinEquipLevel();
	if (LOGIC_CONFIG->GetShenyiConfig().GetActiveEquipSkillLevel() <= min_equip_level)
	{
		int equip_skill_level = LOGIC_CONFIG->GetShenyiConfig().GetEquipSkillLevel(min_equip_level);
		if (equip_skill_level > 0 && equip_skill_level > shenyi->equip_skill_level)
		{
			shenyi->equip_skill_level = equip_skill_level;
			is_active_upgrade_skill = true;
		}
	}

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);
	role->GetShenyi()->SendShenyiInfo();

	if (is_active_upgrade_skill)
	{
		CommonDataParam *common_data = role->GetCommonDataParam();
		if (NULL != common_data)
		{
			if (0 == (common_data->upgrade_skill_skill_active_flag & (1 << UpGradeSkillConfig::SKILL_TYPE_SHENYI)))
			{
				common_data->upgrade_skill_skill_active_flag |= (1 << UpGradeSkillConfig::SKILL_TYPE_SHENYI);
				if (-1 == common_data->upgrade_skill_next_skill_index)
				{
					common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_SHENYI;
				}

				role->SendGradeSkillInfo(role);
			}
		}
	}

 	shenyi_mng->PrintShenyi("UplevelEquip", old_equip_level);

 	return true;
}

bool ShenyiFunction::UplevelSkill(Role *role, int skill_idx, bool is_auto_buy)
{
	if (NULL == role || skill_idx < 0 || skill_idx >= ShenyiParam::SKILL_COUNT)
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

	ShenyiManager *shenyi_mng = role->GetShenyi();
	if (NULL == shenyi_mng)
	{
		return false;
	}

	ShenyiParam *shenyi = shenyi_mng->GetShenyiParam();
	if (NULL == shenyi)
	{
		return false;
	}

	if (!shenyi->IsShenyiActived())
	{
		role->NoticeNum(errornum::EN_SHENYI_NOT_EXIST);
		return false;
	}

	const ShenyiSkillConifg *target_skill_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSkillCfg(skill_idx, shenyi->skill_level_list[skill_idx] + 1);
	if (NULL == target_skill_cfg)
	{
		return false;
	}

	if (0 == target_skill_cfg->skill_type)
	{
		return false;
	}

	if (target_skill_cfg->shenyi_grade > shenyi->grade)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

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
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(role, consume_list, "ShenyiFunction::UplevelSkill"))
	{
		return false;
	}

	short skill_level = shenyi->skill_level_list[skill_idx];
	shenyi->skill_level_list[skill_idx]++;

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);
	role->GetShenyi()->SendShenyiInfo();

	shenyi_mng->PrintShenyi("UplevelSkill", skill_idx, skill_level);

	return true;
}

bool ShenyiFunction::UpStarLevel(Role *role,Protocol::CSShenyiUpStarLevel *msg)
{
// 	if (NULL == msg || NULL == role) return false;
// 
// 	if (msg->stuff_index < 0 || msg->stuff_index >= ShenyiParam::MAX_UP_STAR_ITEM_COUNT)
// 		return false;
// 
// 	Knapsack *knapsack = role->GetKnapsack();
// 	if (knapsack->IsLock())
// 	{
// 		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
// 		return false;
// 	}
// 
// 	if (role->GetSublock()->CheckHasLock())
// 	{
// 		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
// 		return false;
// 	}
// 
// 	ShenyiManager *shenyi_mng = role->GetShenyi();
// 	if (NULL == shenyi_mng)
// 	{
// 		return false;
// 	}
// 
// 	ShenyiParam *shenyi = shenyi_mng->GetShenyiParam();
// 	if (NULL == shenyi)
// 	{
// 		return false;
// 	}
// 
// 	if (!shenyi->IsShenyiActived())
// 	{
// 		role->NoticeNum(errornum::EN_SHENYI_NOT_EXIST);
// 		return false;
// 	}
// 
// 	short old_star_level = shenyi->star_level;
// 	short old_grade = shenyi->grade;
// 	short old_bless_val = shenyi->grade_bless_val;
// 
// 	const ShenyiUpStarExpCfg * curr_star_level_cfg = LOGIC_CONFIG->GetShenyiConfig().GetUpStarExpCfg(old_star_level);
// 	if (NULL == curr_star_level_cfg)
// 	{
// 		return false;
// 	}
// 
// 	const ShenyiUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetShenyiConfig().GetUpStarExpCfg(old_star_level + 1);
// 	if (NULL == next_star_level_cfg)
// 	{
// 		return false;
// 	}
// 
// 	const ShenyiUpStarStuffCfg * up_star_stuff_cfg = LOGIC_CONFIG->GetShenyiConfig().GetUpStarStuffCfg(msg->stuff_index);
// 	if (NULL == up_star_stuff_cfg)
// 	{
// 		return false;
// 	}
// 
// 	const ItemBase *item_base = ITEMPOOL->GetItem(up_star_stuff_cfg->up_star_item_id);
// 	if (NULL == item_base)
// 	{
// 		return false;
// 	}
// 
// 	static ItemExtern::ItemConsumeList consume_list; 
// 	consume_list.Reset();
// 
// 	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
// 	stuff_list[0].item_id = up_star_stuff_cfg->up_star_item_id;
// 	stuff_list[0].num = 1;
// 	stuff_list[0].buyable = 1 == msg->is_auto_buy ? true : false;
// 
// 	if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
// 	{
// 		return false;
// 	}
// 	
// 	bool is_use_not_bind_gold = false;
// 	if (consume_list.count <= 0 && consume_list.need_gold_num <= 0 && consume_list.need_bind_gold_num <= 0) return false;
// 
// 	{
// 		// 检查金币和元宝
// 		Money *money = knapsack->GetMoney();
// 		if (consume_list.need_bind_gold_num > 0 && !money->GoldBindMoreThan(consume_list.need_bind_gold_num))
// 		{
// 			is_use_not_bind_gold = true;
// 			if (consume_list.need_gold_num > 0 && !money->GoldMoreThan(consume_list.need_gold_num))
// 			{
// 				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 				return false;
// 			}
// 		}
// 	}
// 
// 	{
// 		// 消耗
// 
// 		// 物品
// 		if (consume_list.count > 0 && !knapsack->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "ShenyiUpStarLevel")) return false;
// 
// 		// 元宝
// 		if (is_use_not_bind_gold)
// 		{
// 			if (consume_list.need_gold_num > 0)
// 			{
// 				long long consume_gold = consume_list.need_gold_num, consume_gold_bind = 0;
// 				if (!knapsack->GetMoney()->UseGold(consume_gold, "ShenyiUpStarLevel")) return false;
// 
// 				EventHandler::Instance().OnShopBuyItem(role, consume_list.buy_count, consume_list.buy_list, consume_gold, consume_gold_bind);
// 			}
// 
// 		}
// 		else
// 		{
// 			if (consume_list.need_bind_gold_num > 0)
// 			{
// 				long long consume_gold = 0, consume_gold_bind = consume_list.need_bind_gold_num;
// 				if (!knapsack->GetMoney()->UseGoldBind(consume_gold_bind, "ShenyiUpStarLevel")) return false;
// 
// 				EventHandler::Instance().OnShopBuyItem(role, consume_list.buy_count, consume_list.buy_list, consume_gold, consume_gold_bind);
// 			}
// 		}
// 	}
// 
// 	// 结算
// 	{
// 		int rand_value = RandomNum(10000);
// 		if (rand_value < up_star_stuff_cfg->baoji_per)
// 		{
// 			shenyi->grade_bless_val += up_star_stuff_cfg->star_exp * up_star_stuff_cfg->baoji_mul;
// 		}
// 		else
// 		{
// 			shenyi->grade_bless_val += up_star_stuff_cfg->star_exp;
// 		}
// 
// 		int up_star_need_exp = curr_star_level_cfg->up_star_level_exp;
// 		while(shenyi->grade_bless_val >= up_star_need_exp)
// 		{	
// 			const ShenyiUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetShenyiConfig().GetUpStarExpCfg(shenyi->star_level + 1);
// 			if (NULL == next_star_level_cfg || up_star_need_exp <= 0)
// 			{
// 				shenyi->grade_bless_val = 0;
// 				break;
// 			}
// 
// 			shenyi->grade_bless_val -= up_star_need_exp;
// 			shenyi->star_level++;
// 
// 			if ((shenyi->grade - 1) != (shenyi->star_level / 10) && (shenyi->grade - 1)  < (shenyi->star_level / 10))
// 			{
// 				shenyi->grade = (shenyi->star_level / 10) + 1;
// 
// 				const ShenyiGradeConfig * grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(shenyi->grade);
// 				if (NULL != grade_cfg)
// 				{
// 					role->GetShenyi()->ActiveImage(grade_cfg->active_image_id, false);
// 					role->GetShenyi()->UseImage(grade_cfg->active_image_id);
// 
// 					if (grade_cfg->is_broadcast)
// 					{
// 						int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenyi_upgrade, 
// 							UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), grade_cfg->client_grade, grade_cfg->grade);
// 						if (sendlen > 0) 
// 						{
// 							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE);
// 						}
// 					}
// 				}
// 
// 				const ShenyiSkillConifg *skill_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSkillCfg(0, shenyi->skill_level_list[0] + 1);
// 				if (NULL != skill_cfg && shenyi->grade >= skill_cfg->shenyi_grade)
// 				{
// 					shenyi->skill_level_list[0]++;
// 				}
// 			}
// 
// 			up_star_need_exp = next_star_level_cfg->up_star_level_exp;
// 		}
// 
// 		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);
// 		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);			//形象赋灵增加百分比进阶属性
// 	}
// 
// 	role->GetShenyi()->SendShenyiInfo();
// 
// 	role->GetShenyi()->PrintShenyi("ShenyiUpgrade", old_star_level, old_bless_val);
// 	
// 	EventHandler::Instance().OnShenyiUpgrade(role, old_grade, shenyi->grade);
// 	EventHandler::Instance().OnShenyiUpStar(role, old_star_level, shenyi->star_level);

	return true;

}

int ShenyiFunction::UplevelAndSaveBless(Role *role, int add_star)
{
// 	ShenyiManager *shenyi_mgr = role->GetShenyi();
// 	if (NULL == shenyi_mgr)
// 	{
// 		return -1;
// 	}
// 
// 	ShenyiParam *shenyi_param = shenyi_mgr->GetShenyiParam();
// 	if (NULL == shenyi_param)
// 	{
// 		return -2;
// 	}
// 
// 	if (!shenyi_param->IsShenyiActived())
// 	{
// 		return -3;
// 	}
// 
// 	short old_star_level = shenyi_param->star_level;
// 	short old_grade = shenyi_param->grade;
// 	int old_bless_val = shenyi_param->grade_bless_val;
// 
// 	const ShenyiUpStarExpCfg * curr_star_level_cfg = LOGIC_CONFIG->GetShenyiConfig().GetUpStarExpCfg(old_star_level);
// 	if (NULL == curr_star_level_cfg)
// 	{
// 		return -4;
// 	}
// 
// 	const ShenyiUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetShenyiConfig().GetUpStarExpCfg(old_star_level + 1);
// 	if (NULL == next_star_level_cfg)
// 	{
// 		return -5;
// 	}
// 
// 	{
// 		shenyi_param->star_level += add_star;
// 		shenyi_param->grade_bless_val = old_bless_val;		// 升星助力保留祝福值
// 
// 		if ((shenyi_param->grade - 1) != (shenyi_param->star_level / 10) && (shenyi_param->grade - 1) < (shenyi_param->star_level / 10))
// 		{
// 			shenyi_param->grade = (shenyi_param->star_level / 10) + 1;
// 
// 			const ShenyiGradeConfig * grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(shenyi_param->grade);
// 			if (NULL != grade_cfg)
// 			{
// 				role->GetShenyi()->ActiveImage(grade_cfg->active_image_id, false);
// 				role->GetShenyi()->UseImage(grade_cfg->active_image_id);
// 
// 				if (grade_cfg->is_broadcast)
// 				{
// 					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenyi_upgrade,
// 						UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), grade_cfg->client_grade, grade_cfg->grade);
// 					if (sendlen > 0)
// 					{
// 						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE);
// 					}
// 				}
// 			}
// 
// 			const ShenyiSkillConifg *skill_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSkillCfg(0, shenyi_param->skill_level_list[0] + 1);
// 			if (NULL != skill_cfg && shenyi_param->grade >= skill_cfg->shenyi_grade)
// 			{
// 				shenyi_param->skill_level_list[0]++;
// 			}
// 		}
// 
// 		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);
// 		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);			//形象赋灵增加百分比进阶属性
// 	}
// 
// 	role->GetShenyi()->SendShenyiInfo();
// 
// 	role->GetShenyi()->PrintShenyi("ShenyiUpgrade", old_star_level, old_bless_val);
// 
// 	EventHandler::Instance().OnShenyiUpgrade(role, old_grade, shenyi_param->grade);
// 	EventHandler::Instance().OnShenyiUpStar(role, old_star_level, shenyi_param->star_level);
// 
// 	return shenyi_param->star_level;
	return 0;
}

bool ShenyiFunction::CheckCanUpGrade(Role* role)
{
// 	if (NULL == role)
// 	{
// 		return false;
// 	}
// 
// 	ShenyiParam* shenyi_param = role->GetShenyi()->GetShenyiParam();
// 	if (NULL == shenyi_param)
// 	{
// 		return false;
// 	}
// 
// 	if (!shenyi_param->IsShenyiActived())
// 	{
// 		return false;
// 	}
// 
// 	const ShenyiUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetShenyiConfig().GetUpStarExpCfg(shenyi_param->star_level + 1);
// 	if (NULL == next_star_level_cfg)
// 	{
// 		return false;
// 	}

	return true;
}


