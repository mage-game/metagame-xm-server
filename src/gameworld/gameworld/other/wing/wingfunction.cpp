#include "wingfunction.hpp"

#include "wingmanager.hpp"
#include "wingconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "other/upgradeskill/upgradeskillconfig.hpp"
#include "obj/character/role.h"
#include "other/event/eventhandler.hpp"

#include "servercommon/errornum.h"
#include "servercommon/wingdef.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/commondata.hpp"
#include "protocal/msgwing.hpp"

#include "world.h"
#include "gamelog.h"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "other/sublock/sublock.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "servercommon/activitydef.hpp"
#include "other/upgradesyscrit/upgradesyscrit.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/crossconfig.hpp"

bool WingFunction::Upgrade(Role *role, bool is_auto_buy, int repeat_times, int *old_level, int *coin_cost, int *gold_cost)
{
	if (NULL == role) return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	WingManager *wing_mng = role->GetWing();
	if (NULL == wing_mng)
	{
		return false;
	}

	WingParam *wing = wing_mng->GetWingParam();
	if (NULL == wing)
	{
		return false;
	}

	if (!wing->IsWingActived())
	{
		role->NoticeNum(errornum::EN_WING_NOT_EXIST);
		return false;
	}

	short old_grade = wing->grade;
	short oldest_bless_val = wing->grade_bless_val;
	bool succeeded = false;

	if (NULL != old_level)
	{
		*old_level = old_grade;
	}

	const WingGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_WING_CANNOT_UPGRADE);
		return false;
	}

	const WingGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		role->NoticeNum(errornum::EN_WING_CANNOT_UPGRADE);
		return false;
	}

	for (int i = 0; i < repeat_times; ++i)
	{
		short old_bless_val = wing->grade_bless_val;
		const WingUpGradeProbConfig * upgrade_prob_cfg = LOGIC_CONFIG->GetWingConfig().GetUpGradeProbConfig(old_grade, old_bless_val);
		if (NULL == upgrade_prob_cfg)
		{
			role->NoticeNum(errornum::EN_WING_CANNOT_UPGRADE);
			//return false;
			succeeded = true;
			break;
		}

		//UpGradeRushReturn upGradeCkfg = LOGIC_CONFIG->GetUpGradeRushCfg().GetUpgradeRushReturn(UpGradeRushParam::UPGRADE_RUSH_TYPE_WING, grade);

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
		if (!ItemExtern::ConsumeItem(role, consume_list, "WingFunction::Upgrade"))
		{
			succeeded = true;
			break;
		}

		{
			// 生效
			int rand_val = RandomNum(10000);
			
			if (rand_val < upgrade_prob_cfg->succ_rate || wing->grade_bless_val >= curr_grade_cfg->bless_val_limit)
			{
				succeeded = true;
			}

			if (!succeeded)
			{
				int crit_value = UpgradeSysCrit::Instance().CalculateCritValue(role, UPGRADE_SYS_CRIT_TYPE_WING);
				wing->grade_bless_val += (upgrade_prob_cfg->RandAddBlessVal() * crit_value);

				if (wing->grade_bless_val > curr_grade_cfg->bless_val_limit) wing->grade_bless_val = curr_grade_cfg->bless_val_limit;

				//role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);  //计算祝福值临时属性

			}
			else
			{
				wing->grade++;
				wing->grade_bless_val = 0;

				const WingSkillConifg *skill_cfg = LOGIC_CONFIG->GetWingConfig().GetSkillCfg(0, wing->skill_level_list[0] + 1);
				if (NULL != skill_cfg && wing->grade >= skill_cfg->wing_grade)
				{
					wing->skill_level_list[0]++;
				}

				const WingGradeConfig *grade_cfg = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(wing->grade);
				if (NULL == grade_cfg)
				{
					//return false;
					break;
				}
				else
				{
					for (int i = 0; i < WingParam::EQUIP_COUNT; i++)
					{
						int special_img_add = 0;
						for (int j = 0; j < WingParam::MAX_SPECIAL_IMAGE_COUNT; j++)
						{
							const WingSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetWingConfig().GetSpecialImgUpgradeCfg(j, wing->special_img_grade_list[j]);
							if (NULL != special_img_grade_cfg)
							{
								special_img_add += special_img_grade_cfg->equip_level;
							}
						}

						/*WingParam::EquipInfo &equip_info = wing->equip_info_list[i];
						int uplevel_exp = LOGIC_CONFIG->GetWingConfig().GetEquipUpLevelExp(i, equip_info.level);
						while (equip_info.exp >= uplevel_exp && equip_info.level < WingParam::EQUIP_MAX_LEVEL && equip_info.level < (grade_cfg->equip_level_limit + special_img_add))
						{
							if (equip_info.level == 0)
								break;

							equip_info.exp -= uplevel_exp;
							equip_info.level++;
							uplevel_exp = LOGIC_CONFIG->GetWingConfig().GetEquipUpLevelExp(i, equip_info.level);
						}*/
					}
				}

				role->GetWing()->ActiveImage(target_grade_cfg->active_image_id, false);

				role->GetWing()->UseImage(target_grade_cfg->active_image_id);

				role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);

				if (target_grade_cfg->is_broadcast)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wing_upgrade,
						UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade, role->GetWing()->GetMedalSkillLevel());
					if (sendlen > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
					}
				}
				//role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_WING, wing->grade);   // 进阶丹返还活动
				break;
			}
		}
	}

	role->GetWing()->SendWingInfo();
	role->GetWing()->PrintWing("WingUpgrade", old_grade, oldest_bless_val);

	EventHandler::Instance().OnWingUpgrade(role, old_grade, wing->grade);

	return !succeeded;
}

bool WingFunction::UpOneGrade(Role *role, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num)
{
	WingManager *wing_mng = role->GetWing();
	if (NULL == wing_mng)
	{
		return false;
	}

	WingParam *wing = wing_mng->GetWingParam();
	if (NULL == wing)
	{
		return false;
	}

	if (!wing->IsWingActived())
	{
		role->NoticeNum(errornum::EN_WING_NOT_EXIST);
		return false;
	}

	if (wing->grade < valid_min_grade)
	{
		role->NoticeNum(errornum::EN_WING_UP_ONE_GRADE_LIMIT);
		return false;
	}
	else if (wing->grade > valid_max_grade)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(return_item_id);
		if (item_base != nullptr)
		{
			for (int loop_time = 0; return_item_num > 0 && loop_time < 1000; ++loop_time)
			{
				int put_num = return_item_num > item_base->GetPileLimit() ? item_base->GetPileLimit() : return_item_num;
				return_item_num -= put_num;

				ItemConfigData return_item(return_item_id, true, put_num);
				role->GetKnapsack()->PutOrMail(return_item, PUT_REASON_WING_UPGRADE_ONE_RETURN);
			}

			return true;
		}
		return false;
	}

	int old_grade = wing->grade;
	int old_bless_val = wing->grade_bless_val;

	const WingGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_WING_CANNOT_UPGRADE);
		return false;
	}

	const WingGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		role->NoticeNum(errornum::EN_WING_CANNOT_UPGRADE);
		return false;
	}

	{
		wing->grade++;
		wing->grade_bless_val = 0;
		wing->star_level = (wing->grade -1) * 10;

		// 进阶成功清空时间
		wing->clear_upgrade_time = 0;

		const WingSkillConifg *skill_cfg = LOGIC_CONFIG->GetWingConfig().GetSkillCfg(0, wing->skill_level_list[0] + 1);
		if (NULL != skill_cfg && wing->grade >= skill_cfg->wing_grade)
		{
			wing->skill_level_list[0]++;
		}

		role->GetWing()->ActiveImage(target_grade_cfg->active_image_id, false);
		role->GetWing()->UseImage(target_grade_cfg->active_image_id);

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);

		if (target_grade_cfg->is_broadcast)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wing_upgrade, 
				UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade, role->GetWing()->GetMedalSkillLevel());
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

	role->GetWing()->PrintWing("WingUpgrade", old_grade, old_bless_val);

	EventHandler::Instance().OnWingUpgrade(role, old_grade, wing->grade);

	return true;
}

bool WingFunction::GMResetGrade(Role *role)
{
	if (NULL == role) return false;

	WingParam *wing = role->GetWing()->GetWingParam();
	if (NULL == wing)
	{
		return false;
	}

	wing->grade = 1;
	wing->grade_bless_val = 0;

	role->GetWing()->SendWingInfo();

	return true;
}

bool WingFunction::ActiveWing(Role *role, bool is_recal)
{
	if (NULL == role) return false;

	// 检查等级
	WingManager *wing_mng = role->GetWing();

	WingParam *wing = wing_mng->GetWingParam();
	if (NULL == wing)
	{
		return false;
	}

	if (!wing->IsWingActived())
	{
		wing->level = role->GetLevel();
		wing->grade = 1;
		wing->grade_bless_val = 0;
		wing->star_level = 0;

		const WingGradeConfig *grade_cfg = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(1);
		if (NULL != grade_cfg)
		{
			wing_mng->ActiveImage(grade_cfg->active_image_id, false);
			wing_mng->UseImage(grade_cfg->active_image_id);
		}

		const WingSkillConifg *skill_cfg = LOGIC_CONFIG->GetWingConfig().GetSkillCfg(0, 1);
		if (NULL != skill_cfg && wing->grade >= skill_cfg->wing_grade)
		{
			wing->skill_level_list[0] = 1;
		}

		if (is_recal)
		{
			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);
		}

		wing_mng->SendWingInfo();

		wing_mng->PrintWing("ActiveWing");

		role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_WING, 0, 0);
		JinjiesysFuncHelper::Instance().OnActiveJinjieSys(role, JINJIE_TYPE_WING);

	}

	// 激活传闻
	int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wing_active_content, role->GetUID(), role->GetName(),
		(int)role->GetCamp(), wing->grade, role->GetWing()->GetMedalSkillLevel());
	if (len > 0) 
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
	}

	return true;
}

bool WingFunction::WingSpecialImgUpgrade(Role *role, short special_img_id)
{
	if (NULL == role)
	{
		return false;
	}

	if (special_img_id <= 0 || special_img_id >= WingParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return false;
	}

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	WingManager *wing_mng = role->GetWing();
	if (NULL == wing_mng)
	{
		return false;
	}

	WingParam *wing = wing_mng->GetWingParam();
	if (NULL == wing)
	{
		return false;
	}

	if (!wing->IsSpecialImageActive(special_img_id))
	{
		role->NoticeNum(errornum::EN_WING_IMG_NOT_ACTIVE);
		return false;
	}

	const int old_grade = wing->special_img_grade_list[special_img_id];
	if (old_grade >= WingParam::MAX_UPGRADE_LIMIT)
	{
		role->NoticeNum(errornum::EN_WING_SPECIAL_IMG_GRADE_LIMIT);
		return false;
	}

	const WingSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetWingConfig().GetSpecialImgUpgradeCfg(special_img_id, old_grade);
	if (NULL == upgrade_cfg)
	{
		return false;
	}
	const WingSpecialImgUpgradeCfg *next_upgrade_cfg = LOGIC_CONFIG->GetWingConfig().GetSpecialImgUpgradeCfg(special_img_id, old_grade + 1);
	if (NULL == next_upgrade_cfg)
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
	if (!ItemExtern::ConsumeItem(role, consume_list, "WingSpecialImgUpgrade"))
	{
		return false;
	}

	// 生效
	++ wing->special_img_grade_list[special_img_id];

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);

	role->GetWing()->SendWingInfo();

	role->GetWing()->PrintWing("WingSpecialImgUpgrade", old_grade, wing->special_img_grade_list[special_img_id]);

	return true;
}

bool WingFunction::UplevelEquip(Role *role, int equip_idx)
{
	if (NULL == role || equip_idx < 0 || equip_idx >= WingParam::EQUIP_COUNT)
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

	WingManager *wing_mng = role->GetWing();

	WingParam *wing = wing_mng->GetWingParam();
	if (NULL == wing)
	{
		return false;
	}

	if (!wing->IsWingActived())
	{
		return false;
	}

	const WingEquipConifg *equip_cfg = LOGIC_CONFIG->GetWingConfig().GetEquipCfg(equip_idx);
	if (NULL == equip_cfg)
	{
		return false;
	}

	short level = wing->equip_level_list[equip_idx];

	//if (level >= equip_cfg->max_level - 1)
	//{
	//	role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
	//	return false;
	//}

	if (level < 0)
	{
		return false;
	}

	//策划需求，装备升级级数由羽翼阶数决定
	const WingGradeConfig * cur_grade_cfg = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(wing->grade);
	if (cur_grade_cfg == NULL)
	{
		return false;
	}

	if (level >= cur_grade_cfg->equip_level_toplimit)
	{
		role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
		return false;
	}

	if (wing->grade < LOGIC_CONFIG->GetWingConfig().GetActiveEquipGrade())
	{
		return false;
	}

	short old_level = level;

	// 消耗

	if (!role->GetKnapsack()->ConsumeItem(equip_cfg->level_cfg[level].item.item_id, equip_cfg->level_cfg[level].item.num, "WingEquipUplevel"))
	{
		return false;
	}

	// 结算
	wing->equip_level_list[equip_idx]++;

	bool is_active_upgrade_skill = false;
	int min_equip_level = wing_mng->RefreshWingMinEquipLevel();
	if (LOGIC_CONFIG->GetWingConfig().GetActiveEquipSkillLevel() <= min_equip_level)
	{
		int equip_skill_level = LOGIC_CONFIG->GetWingConfig().GetEquipSkillLevel(min_equip_level);
		if (equip_skill_level > 0)
		{
			wing->equip_skill_level = equip_skill_level;
			is_active_upgrade_skill = true;
		}
	}

	role->GetWing()->SendWingInfo();
	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);

	if (is_active_upgrade_skill)
	{
		CommonDataParam *common_data = role->GetCommonDataParam();
		if (NULL != common_data)
		{
			if (0 == (common_data->upgrade_skill_skill_active_flag & (1 << UpGradeSkillConfig::SKILL_TYPE_WING)))
			{
				common_data->upgrade_skill_skill_active_flag |= (1 << UpGradeSkillConfig::SKILL_TYPE_WING);
				if (-1 == common_data->upgrade_skill_next_skill_index)
				{
					common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_WING;
				}

				role->SendGradeSkillInfo(role);
			}
		}
	}

	wing_mng->PrintWing("UplevelEquip", old_level);

	return true;
}


bool WingFunction::UplevelSkill(Role *role, int skill_idx, bool is_auto_buy)
{
	if (NULL == role || skill_idx < 0 || skill_idx >= WingParam::SKILL_COUNT)
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

	WingManager *wing_mng = role->GetWing();
	if (NULL == wing_mng)
	{
		return false;
	}

	WingParam *wing = wing_mng->GetWingParam();
	if (NULL == wing)
	{
		return false;
	}

	if (!wing->IsWingActived())
	{
		return false;
	}

	const WingSkillConifg *target_skill_cfg = LOGIC_CONFIG->GetWingConfig().GetSkillCfg(skill_idx, wing->skill_level_list[skill_idx] + 1);
	if (NULL == target_skill_cfg)
	{
		return false;
	}

	if (0 == target_skill_cfg->skill_type)
	{
		return false;
	}

	if (target_skill_cfg->wing_grade > wing->grade)
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

		// 消耗
		if (!ItemExtern::ConsumeItem(role, consume_list, "WingFunction::UplevelSkill"))
		{
			return false;
		}
	}

	short skill_level = wing->skill_level_list[skill_idx];
	wing->skill_level_list[skill_idx]++;

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);
	role->GetWing()->SendWingInfo();

	wing_mng->PrintWing("UplevelSkill", skill_idx, skill_level);

	return true;
}

bool WingFunction::UpStarLevel(Role *role,Protocol::CSWingUpStarLevel *msg)
{
	if (NULL == msg || NULL == role) return false;

	if (msg->stuff_index < 0 || msg->stuff_index >= WingParam::MAX_UP_STAR_ITEM_COUNT)
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

	WingManager *wing_mng = role->GetWing();
	if (NULL == wing_mng)
	{
		return false;
	}

	WingParam *wing = wing_mng->GetWingParam();
	if (NULL == wing)
	{
		return false;
	}

	if (!wing->IsWingActived())
	{
		role->NoticeNum(errornum::EN_WING_NOT_EXIST);
		return false;
	}

	short old_star_level = wing->star_level;
	short old_grade = wing->grade;
	int old_bless_val = wing->grade_bless_val;

	const WingUpStarExpCfg * curr_star_level_cfg = LOGIC_CONFIG->GetWingConfig().GetUpStarExpCfg(old_star_level);
	if (NULL == curr_star_level_cfg)
	{
		return false;
	}

	const WingUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetWingConfig().GetUpStarExpCfg(old_star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return false;
	}

	const WingUpStarStuffCfg * up_star_stuff_cfg = LOGIC_CONFIG->GetWingConfig().GetUpStarStuffCfg(msg->stuff_index);
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
	if (!ItemExtern::ConsumeItem(role, consume_list, "WingFunction::UpStarLevel"))
	{
		return false;
	}

	// 结算
	{
		int rand_value = RandomNum(10000);
		if (rand_value < up_star_stuff_cfg->baoji_per)
		{
			wing->grade_bless_val += up_star_stuff_cfg->star_exp * up_star_stuff_cfg->baoji_mul;
		}
		else
		{
			wing->grade_bless_val += up_star_stuff_cfg->star_exp;
		}

		int up_star_need_exp = curr_star_level_cfg->up_star_level_exp;
		while(wing->grade_bless_val >= up_star_need_exp)
		{	
			const WingUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetWingConfig().GetUpStarExpCfg(wing->star_level + 1);
			if (NULL == next_star_level_cfg || up_star_need_exp <= 0)
			{
				wing->grade_bless_val = 0;
				break;
			}

			wing->grade_bless_val -= up_star_need_exp;
			wing->star_level++;

			if ((wing->grade - 1) != (wing->star_level / 10) && (wing->grade - 1)  < (wing->star_level / 10))
			{
				wing->grade = (wing->star_level / 10) + 1;

				const WingGradeConfig * grade_cfg = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(wing->grade);
				if (NULL != grade_cfg)
				{
					role->GetWing()->ActiveImage(grade_cfg->active_image_id, false);
					role->GetWing()->UseImage(grade_cfg->active_image_id);

					if (grade_cfg->is_broadcast)
					{
						int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wing_upgrade, 
							UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), grade_cfg->client_grade, grade_cfg->grade, role->GetWing()->GetMedalSkillLevel());
						if (sendlen > 0) 
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
						}
					}
				}

				const WingSkillConifg *skill_cfg = LOGIC_CONFIG->GetWingConfig().GetSkillCfg(0, wing->skill_level_list[0] + 1);
				if (NULL != skill_cfg && wing->grade >= skill_cfg->wing_grade)
				{
					wing->skill_level_list[0]++;
				}
			}

			up_star_need_exp = next_star_level_cfg->up_star_level_exp;
		}

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);			//形象赋灵增加百分比进阶属性
	}

	role->GetWing()->SendWingInfo();

	role->GetWing()->PrintWing("WingUpgrade", old_star_level, old_bless_val);

	EventHandler::Instance().OnWingUpgrade(role, old_grade, wing->grade);
	EventHandler::Instance().OnWingUpStar(role, old_star_level, wing->star_level);

	return true;

}

int WingFunction::UplevelAndSaveBless(Role *role, int add_star)
{
	WingManager *wing_mng = role->GetWing();
	if (NULL == wing_mng)
	{
		return -1;
	}

	WingParam *wing = wing_mng->GetWingParam();
	if (NULL == wing)
	{
		return -2;
	}

	if (!wing->IsWingActived())
	{
		return -3;
	}

	short old_star_level = wing->star_level;
	short old_grade = wing->grade;
	int old_bless_val = wing->grade_bless_val;

	const WingUpStarExpCfg * curr_star_level_cfg = LOGIC_CONFIG->GetWingConfig().GetUpStarExpCfg(old_star_level);
	if (NULL == curr_star_level_cfg)
	{
		return -4;
	}

	const WingUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetWingConfig().GetUpStarExpCfg(old_star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return -5;
	}

	{
		wing->star_level += add_star;
		wing->grade_bless_val = old_bless_val;			// 升星助力保留祝福值

		if ((wing->grade - 1) != (wing->star_level / 10) && (wing->grade - 1) < (wing->star_level / 10))
		{
			wing->grade = (wing->star_level / 10) + 1;

			const WingGradeConfig * grade_cfg = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(wing->grade);
			if (NULL != grade_cfg)
			{
				role->GetWing()->ActiveImage(grade_cfg->active_image_id, false);
				role->GetWing()->UseImage(grade_cfg->active_image_id);

				if (grade_cfg->is_broadcast)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wing_upgrade,
						UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), grade_cfg->client_grade, grade_cfg->grade, role->GetWing()->GetMedalSkillLevel());
					if (sendlen > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
					}
				}
			}

			const WingSkillConifg *skill_cfg = LOGIC_CONFIG->GetWingConfig().GetSkillCfg(0, wing->skill_level_list[0] + 1);
			if (NULL != skill_cfg && wing->grade >= skill_cfg->wing_grade)
			{
				wing->skill_level_list[0]++;
			}
		}

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);			//形象赋灵增加百分比进阶属性
	}

	role->GetWing()->SendWingInfo();

	role->GetWing()->PrintWing("WingUpgrade", old_star_level, old_bless_val);

	EventHandler::Instance().OnWingUpgrade(role, old_grade, wing->grade);
	EventHandler::Instance().OnWingUpStar(role, old_star_level, wing->star_level);

	return wing->star_level;
}

bool WingFunction::CheckCanUpGrade(Role* role)
{
	if (NULL == role)
	{
		return false;
	}

	WingParam* wing_param = role->GetWing()->GetWingParam();
	if (NULL == wing_param)
	{
		return false;
	}

	if (!wing_param->IsWingActived())
	{
		return false;
	}

	const WingUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetWingConfig().GetUpStarExpCfg(wing_param->star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return false;
	}

	return true;
}
