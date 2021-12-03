#include "fightmountfunction.hpp"

#include "fightmountmanager.hpp"
#include "fightmountconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "other/upgradeskill/upgradeskillconfig.hpp"
#include "obj/character/role.h"
#include "other/event/eventhandler.hpp"

#include "servercommon/errornum.h"
#include "servercommon/struct/fightmountparam.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/commondata.hpp"

#include "protocal/msgfightmount.hpp"
#include "world.h"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "other/sublock/sublock.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "servercommon/activitydef.hpp"
#include "other/upgradesyscrit/upgradesyscrit.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/crossconfig.hpp"

bool FightMountFunction::Upgrade(Role *role, bool is_auto_buy, int repeat_times, int *old_level, int *coin_cost, int *gold_cost)
{
	if (NULL == role) return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	FightMountManager *mount_mng = role->GetFightMountManager();
	if (NULL == mount_mng)
	{
		return false;
	}

	FightMountParam *mount = mount_mng->GetFightMountParam();
	if (NULL == mount)
	{
		return false;
	}

	if (!mount->IsFightMountActived())
	{
		role->NoticeNum(errornum::EN_FIGHT_MOUNT_NOT_EXIST);
		return false;
	}

	short old_grade = mount->grade;
	short oldest_bless_val = mount->grade_bless_val;
	bool succeeded = false;

	if (NULL != old_level)
	{
		*old_level = old_grade;
	}

	const FightMountGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_FIGHT_MOUNT_CANNOT_UPGRADE);
		return false;
	}

	const FightMountGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		role->NoticeNum(errornum::EN_FIGHT_MOUNT_CANNOT_UPGRADE);
		return false;
	}

	for (int i = 0; i < repeat_times; ++i)
	{
		short old_bless_val = mount->grade_bless_val;
		const FightMountUpGradeProbConfig * upgrade_prob_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetUpGradeProbConfig(old_grade, old_bless_val);
		if (NULL == upgrade_prob_cfg)
		{
			role->NoticeNum(errornum::EN_FIGHT_MOUNT_CANNOT_UPGRADE);
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
		if (!ItemExtern::ConsumeItem(role, consume_list, "FightMountFunction::Upgrade"))
		{
			succeeded = true;
			break;
		}

		{
			// 生效
			int rand_val = RandomNum(10000);
			
			if (rand_val < upgrade_prob_cfg->succ_rate || mount->grade_bless_val >= curr_grade_cfg->bless_val_limit)
			{
				succeeded = true;
			}

			if (!succeeded)
			{
				int crit_value = UpgradeSysCrit::Instance().CalculateCritValue(role, UPGRADE_SYS_CRIT_TYPE_FIGHTMOUNT);
				mount->grade_bless_val += (upgrade_prob_cfg->RandAddBlessVal() * crit_value);

				if (mount->grade_bless_val > curr_grade_cfg->bless_val_limit) mount->grade_bless_val = curr_grade_cfg->bless_val_limit;

				//role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);

			}
			else
			{
				mount->grade++;
				mount->grade_bless_val = 0;
				// 进阶的时候达到阶数就激活技能,只升一级
				const FightMountSkillConifg *skill_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSkillCfg(0, mount->skill_level_list[0] + 1);
				if (NULL != skill_cfg && mount->grade >= skill_cfg->mount_grade &&  0 == mount->skill_level_list[0])
				{
					mount->skill_level_list[0]++;
				}
				
				const FightMountGradeConfig *grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(mount->grade);
				if (NULL == grade_cfg)
				{
					//return false;
					succeeded = true;
					break;
				}
				else
				{
					for (int i = 0; i < FightMountParam::EQUIP_COUNT; i++)
					{
						int special_img_add = 0;
						for (int j = 0; j < FightMountParam::MAX_SPECIAL_IMAGE_COUNT; j++)
						{
							const FightMountSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSpecialImgUpgradeCfg(j, mount->special_img_grade_list[j]);
							if (NULL != special_img_grade_cfg)
							{
								special_img_add += special_img_grade_cfg->equip_level;
							}
						}

						//					FightMountParam::EquipInfo &equip_info = mount->equip_info_list[i];
						//					int uplevel_exp = LOGIC_CONFIG->GetFightMountOpConfig().GetEquipUpLevelExp(i, equip_info.level);
						//					while (equip_info.exp >= uplevel_exp && equip_info.level < FightMountParam::EQUIP_MAX_LEVEL && equip_info.level < (grade_cfg->equip_level_limit + special_img_add))
						//					{
						//						if (equip_info.level == 0)
						//							break;
						//
						//						equip_info.exp -= uplevel_exp;
						//						equip_info.level++;
						//						uplevel_exp = LOGIC_CONFIG->GetFightMountOpConfig().GetEquipUpLevelExp(i, equip_info.level);
						//					}
					}
				}

				role->GetFightMountManager()->ActiveImage(target_grade_cfg->active_image_id, false);
				role->GetFightMountManager()->UseImage(target_grade_cfg->active_image_id);

				role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);

				if (target_grade_cfg->is_broadcast)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_fight_mount_upgrade,
						UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade, role->GetFightMountManager()->GetMedalSkillLevel());
					if (sendlen > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
					}
				}
				//role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_FIGHT_MOUNT, mount->grade);   // 进阶丹返还活动
				break;
			}
		}
	}
	role->GetFightMountManager()->SendFightMountInfo();
	role->GetFightMountManager()->PrintFightMount("FightMountUpgrade", old_grade, oldest_bless_val);

	EventHandler::Instance().OnFightMountUpgrade(role, old_grade, mount->grade);

	return !succeeded;
}

bool FightMountFunction::UpOneGrade(Role *role, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num)
{
	FightMountManager *fight_mount_mng = role->GetFightMountManager();
	if (NULL == fight_mount_mng)
	{
		return false;
	}

	FightMountParam *fight_mount = fight_mount_mng->GetFightMountParam();
	if (NULL == fight_mount)
	{
		return false;
	}

	if (!fight_mount->IsFightMountActived())
	{
		role->NoticeNum(errornum::EN_FIGHT_MOUNT_NOT_EXIST);
		return false;
	}

	if (fight_mount->grade < valid_min_grade)
	{
		role->NoticeNum(errornum::EN_FIGHT_MOUNT_UP_ONE_GRADE_LIMIT);
		return false;
	}
	else if (fight_mount->grade > valid_max_grade)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(return_item_id);
		if (item_base != nullptr)
		{
			for (int loop_time = 0; return_item_num > 0 && loop_time < 1000; ++loop_time)
			{
				int put_num = return_item_num > item_base->GetPileLimit() ? item_base->GetPileLimit() : return_item_num;
				return_item_num -= put_num;

				ItemConfigData return_item(return_item_id, true, put_num);
				role->GetKnapsack()->PutOrMail(return_item, PUT_REASON_FIGHTMOUNT_UPGRADE_ONE_RETURN);
			}

			return true;
		}
		return false;
	}

	int old_grade = fight_mount->grade;
	int old_bless_val = fight_mount->grade_bless_val;

	const FightMountGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_FIGHT_MOUNT_CANNOT_UPGRADE);
		return false;
	}

	const FightMountGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		role->NoticeNum(errornum::EN_FIGHT_MOUNT_CANNOT_UPGRADE);
		return false;
	}

	{
		fight_mount->grade++;
		fight_mount->grade_bless_val = 0;
		fight_mount->star_level = (fight_mount->grade -1) * 10;

		// 			// 进阶成功清空时间
		// 			mount->clear_upgrade_time = 0;

		const FightMountSkillConifg *skill_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSkillCfg(0, fight_mount->skill_level_list[0] + 1);
		if (NULL != skill_cfg && fight_mount->grade >= skill_cfg->mount_grade && 0 == fight_mount->skill_level_list[0])
		{
			fight_mount->skill_level_list[0]++;
		}

		role->GetFightMountManager()->ActiveImage(target_grade_cfg->active_image_id, false);
		role->GetFightMountManager()->UseImage(target_grade_cfg->active_image_id);

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);

		if (target_grade_cfg->is_broadcast)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_fight_mount_upgrade, 
				UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade, role->GetFightMountManager()->GetMedalSkillLevel());
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

	role->GetFightMountManager()->PrintFightMount("FightMountUpgrade", old_grade, old_bless_val);

	EventHandler::Instance().OnFightMountUpgrade(role, old_grade, fight_mount->grade);

	return true;
}

bool FightMountFunction::UpgradeByItem(Role *role, int target_grade, int reward_item_count)
{
	if (NULL == role || target_grade <= 0 || reward_item_count <= 0)
	{
		return false;
	}

	FightMountManager *mount_mng = role->GetFightMountManager();
	if (NULL == mount_mng)
	{
		return false;
	}

	FightMountParam *mount = mount_mng->GetFightMountParam();
	if (NULL == mount)
	{
		return false;
	}

	if (!mount->IsFightMountActived())
	{
		role->NoticeNum(errornum::EN_FIGHT_MOUNT_NOT_EXIST);
		return false;
	}

	if (mount->grade >= target_grade)
	{
		ItemConfigData reward_item;
		reward_item.item_id = LOGIC_CONFIG->GetFightMountOpConfig().GetUseJinjieCardRewardItemID();
		reward_item.num = reward_item_count;
		reward_item.is_bind = true;

		if (!role->GetKnapsack()->Put(reward_item, PUT_REASON_USE_MOUNT_JINJIE_CARD))
		{
			role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return false;
		}
	}
	else
	{
		const FightMountGradeConfig *target_grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(target_grade);
		if (NULL == target_grade_cfg)
		{
			return false;
		}

		int old_grade  = mount->grade;
		mount->grade = static_cast<short>(target_grade);
		mount->grade_bless_val = 0;

		for (int i = 1; i <= target_grade_cfg->active_image_id; ++ i)
		{
			role->GetFightMountManager()->ActiveImage(i, false);
		}

		role->GetFightMountManager()->UseImage(target_grade_cfg->active_image_id);

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);

		if (target_grade_cfg->is_broadcast)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_fight_mount_upgrade, 
				UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade, role->GetFightMountManager()->GetMedalSkillLevel());
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

		role->GetFightMountManager()->PrintFightMount("FightMountUpgradeByItems", old_grade, mount->grade, mount->grade_bless_val);

		for (int to_grade = old_grade + 1; to_grade <= mount->grade; ++to_grade)
		{
			EventHandler::Instance().OnFightMountUpgrade(role, to_grade - 1, to_grade);
		}
	}

	return true;
}

bool FightMountFunction::GMResetGrade(Role *role)
{
	if (NULL == role) return false;

	FightMountParam *mount = role->GetFightMountManager()->GetFightMountParam();
	if (NULL == mount)
	{
		return false;
	}

	mount->grade = 1;
	mount->grade_bless_val = 0;

	role->GetFightMountManager()->SendFightMountInfo();

	return true;
}

bool FightMountFunction::ActiveFightMount(Role *role, bool is_recal)
{
	if (NULL == role) return false;

	// 检查等级
	FightMountManager *mount_mng = role->GetFightMountManager();
	if (NULL == mount_mng)
	{
		return false;
	}

	FightMountParam *mount = mount_mng->GetFightMountParam();
	if (NULL == mount)
	{
		return false;
	}
	if (!mount->IsFightMountActived())
	{
		mount->star_level = 0;
		mount->level = role->GetLevel();
		mount->grade = 1;
		mount->grade_bless_val = 0;

		const FightMountGradeConfig *grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(1);
		if (NULL != grade_cfg)
		{
			mount_mng->ActiveImage(grade_cfg->active_image_id, false);
			mount_mng->UseImage(grade_cfg->active_image_id);
		}

		const FightMountSkillConifg *skill_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSkillCfg(0, 1);
		if (NULL != skill_cfg && mount->grade >= skill_cfg->mount_grade)
		{
			mount->skill_level_list[0] = 1;
		}
		
		if (is_recal)
		{
			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
		}

		mount_mng->SendFightMountInfo();

		mount_mng->PrintFightMount("ActiveFightMount");

		role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_FIGHT_MOUNT, 0, 0);
		JinjiesysFuncHelper::Instance().OnActiveJinjieSys(role, JINJIE_TYPE_FIGHT_MOUNT);
	}

	return true;
}

bool FightMountFunction::FightMountSpecialImgUpgrade(Role *role, short special_img_id)
{
	if (NULL == role)
	{
		return false;
	}

	if (special_img_id <= 0 || special_img_id >= FightMountParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return false;
	}

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	FightMountManager *mount_mng = role->GetFightMountManager();
	if (NULL == mount_mng)
	{
		return false;
	}

	FightMountParam *mount = mount_mng->GetFightMountParam();
	if (NULL == mount)
	{
		return false;
	}

	if (!mount->IsSpecialImageActive(special_img_id))
	{
		role->NoticeNum(errornum::EN_FIGHT_MOUNT_IMG_NOT_ACTIVE);
		return false;
	}

	const int old_grade = mount->special_img_grade_list[special_img_id];
	if (old_grade >= FightMountParam::MAX_UPGRADE_LIMIT)
	{
		role->NoticeNum(errornum::EN_MOUNT_SPECIAL_IMG_GRADE_LIMIT);
		return false;
	}

	const FightMountSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSpecialImgUpgradeCfg(special_img_id, old_grade + 1);
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
	if (!ItemExtern::ConsumeItem(role, consume_list, "FightMountFunction::FightMountSpecialImgUpgrade"))
	{
		return false;
	}

	// 生效
	++ mount->special_img_grade_list[special_img_id];

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);

	role->GetFightMountManager()->SendFightMountInfo();

	role->GetFightMountManager()->PrintFightMount("FightMountSpecialImgUpgrade", old_grade, mount->special_img_grade_list[special_img_id]);

	return true;
}

bool FightMountFunction::UplevelSkill(Role *role, int skill_idx, bool is_auto_buy)
{
	if (NULL == role || skill_idx < 0 || skill_idx >= FightMountParam::SKILL_COUNT)
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

	FightMountManager *fight_mount_mng = role->GetFightMountManager();
	if (NULL == fight_mount_mng)
	{
		return false;
	}

	FightMountParam *fight_mount = fight_mount_mng->GetFightMountParam();
	if (NULL == fight_mount)
	{
		return false;
	}

	if (!fight_mount->IsFightMountActived())
	{
		role->NoticeNum(errornum::EN_FIGHT_MOUNT_NOT_EXIST);
		return false;
	}

	const FightMountSkillConifg *target_skill_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSkillCfg(skill_idx, fight_mount->skill_level_list[skill_idx] + 1);
	if (NULL == target_skill_cfg)
	{
		return false;
	}

	if (0 == target_skill_cfg->skill_type)
	{
		return false;
	}

	if (target_skill_cfg->mount_grade > fight_mount->grade)
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
	if (!ItemExtern::ConsumeItem(role, consume_list, "FightMountFunction::UplevelSkill"))
	{
		return false;
	}

	short skill_level = fight_mount->skill_level_list[skill_idx];
	fight_mount->skill_level_list[skill_idx]++;

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
	fight_mount_mng->SendFightMountInfo();

	fight_mount_mng->PrintFightMount("UplevelSkill", skill_idx, skill_level);
	return true;
}

bool FightMountFunction::UplevelEquip(Role *role, int equip_idx)
{
	if (NULL == role || equip_idx < 0 || equip_idx >= FightMountParam::EQUIP_COUNT)
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

	FightMountManager *mount_mng = role->GetFightMountManager();
	if (NULL == mount_mng)
	{
		return false;
	}

	FightMountParam *mount = mount_mng->GetFightMountParam();
	if (NULL == mount)
	{
		return false;
	}

	if (!mount->IsFightMountActived())
	{
		role->NoticeNum(errornum::EN_FIGHT_MOUNT_NOT_EXIST);
		return false;
	}

	if (mount->grade < LOGIC_CONFIG->GetFightMountOpConfig().GetActiveEquipGrade())
	{
		return false;
	}

	const FightMountEquipConifg *equip_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetEquipCfg(equip_idx);
	if (NULL == equip_cfg)
	{
		return false;
	}

	short &equip_level = mount->equip_level_list[equip_idx];

	//if (equip_level >= equip_cfg->max_level - 1)
	//{
	//	role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
	//	return false;
	//}

	if (equip_level < 0)
	{
		return false;
	}

	//策划需求，装备升级级数由战骑阶数决定
	const FightMountGradeConfig * cur_grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(mount->grade);
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
		if (!role->GetKnapsack()->ConsumeItem(item.item_id, item.num, "FightMountEquipUpLevel"))
		{
			return false;
		}
	}
	short old_equip_level = equip_level;

	// 结算
	equip_level++;

	bool is_active_upgrade_skill = false;
	int min_equip_level = mount_mng->RefreshFightMountMinEquipLevel();
	if (LOGIC_CONFIG->GetFightMountOpConfig().GetActiveEquipSkillLevel() <= min_equip_level)
	{
		int equip_skill_level = LOGIC_CONFIG->GetFightMountOpConfig().GetEquipSkillLevel(min_equip_level);
		if (equip_skill_level > 0)
		{
			mount->equip_skill_level = equip_skill_level;
			is_active_upgrade_skill = true;
		}
	}

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
	role->GetFightMountManager()->SendFightMountInfo();

	if (is_active_upgrade_skill)
	{
		CommonDataParam *common_data = role->GetCommonDataParam();
		if (NULL != common_data)
		{
			if (0 == (common_data->upgrade_skill_skill_active_flag & (1 << UpGradeSkillConfig::SKILL_TYPE_FIGHT_MOUNT)))
			{
				common_data->upgrade_skill_skill_active_flag |= (1 << UpGradeSkillConfig::SKILL_TYPE_FIGHT_MOUNT);

				if (-1 == common_data->upgrade_skill_next_skill_index)
				{
					common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_FIGHT_MOUNT;
				}

				role->SendGradeSkillInfo(role);
			}
		}
	}

	mount_mng->PrintFightMount("UplevelEquip", old_equip_level);

	return true;
}

// bool FightMountFunction::UplevelSkill(Role *role, int skill_idx, bool is_auto_buy)
// {
// 	if (NULL == role || skill_idx < 0 || skill_idx >= FightMountParam::SKILL_COUNT)
// 	{
// 		return false;
// 	}
// 
// 	if (role->GetSublock()->CheckHasLock())
// 	{
// 		return false;
// 	}
// 
// 	if (role->GetKnapsack()->IsLock())
// 	{
// 		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
// 		return false;
// 	}
// 
// 	FightMountManager *mount_mng = role->GetFightMountManager();
// 
// 	FightMountParam *mount = mount_mng->GetFightMountParam();
// 	if (NULL == mount)
// 	{
// 		role->NoticeNum(errornum::EN_MOUNT_NOT_EXIST);
// 		return false;
// 	}
// 
// 	if (!mount->IsFightMountActived())
// 	{
// 		return false;
// 	}
// 
// 	const FightMountSkillConifg *target_skill_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSkillCfg(skill_idx, mount->skill_level_list[skill_idx] + 1);
// 	if (NULL == target_skill_cfg)
// 	{
// 		return false;
// 	}
// 
// 	if (0 == target_skill_cfg->skill_type)
// 	{
// 		return false;
// 	}
// 
// 	if (target_skill_cfg->mount_grade > mount->grade)
// 	{
// 		return false;
// 	}
// 
// 	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
// 	
// 	{
// 		// 升级材料
// 		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
// 		stuff_list[0].item_id = target_skill_cfg->uplevel_stuff_id;
// 		stuff_list[0].num = target_skill_cfg->uplevel_stuff_num;
// 		stuff_list[0].buyable = is_auto_buy ? true : false;
// 
// 		if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
// 		{
// 			return false;
// 		}
// 	}
// 
// 	{
// 		// 消耗
// 
// 		// 物品
// 		if (consume_list.count > 0 && !role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "FightMountSkillUplevel")) return false;
// 
// 		// 元宝
// 		if (consume_list.need_gold_num > 0) 
// 		{
// 			if (!role->GetKnapsack()->GetMoney()->UseAllGold(consume_list.need_gold_num, "FightMountSkillUplevel")) return false;
// 			EventHandler::Instance().OnShopBuyItem(role, consume_list.buy_count, consume_list.buy_list, consume_list.need_gold_num, 0);
// 		}
// 	}
// 
//  	short skill_level = mount->skill_level_list[skill_idx];
// 	mount->skill_level_list[skill_idx]++;
// 
// 	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
// 	role->GetFightMountManager()->SendFightMountInfo();
// 
// 	mount_mng->PrintFightMount("UplevelSkill", skill_idx, skill_level);
// 
// 	return true;
// }

bool FightMountFunction::UpStarLevel(Role *role,Protocol::CSFightMountUpStarLevel *msg)
{
	if (NULL == msg || NULL == role) return false;

	if (msg->stuff_index < 0 || msg->stuff_index >= FightMountParam::MAX_UP_STAR_ITEM_COUNT)
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

	FightMountManager *mount_mng = role->GetFightMountManager();
	if (NULL == mount_mng)
	{
		return false;
	}

	FightMountParam *mount = mount_mng->GetFightMountParam();
	if (NULL == mount)
	{
		return false;
	}

	if (!mount->IsFightMountActived())
	{
		role->NoticeNum(errornum::EN_FIGHT_MOUNT_NOT_EXIST);
		return false;
	}

	short old_star_level = mount->star_level;
	short old_grade = mount->grade;
	int old_bless_val = mount->grade_bless_val;

	const FightMountUpStarExpCfg * curr_star_level_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetUpStarExpCfg(old_star_level);
	if (NULL == curr_star_level_cfg)
	{
		return false;
	}

	const FightMountUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetUpStarExpCfg(old_star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return false;
	}

	const FightMountUpStarStuffCfg * up_star_stuff_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetUpStarStuffCfg(msg->stuff_index);
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
	if (!ItemExtern::ConsumeItem(role, consume_list, "FightMountFunction::UpStarLevel"))
	{
		return false;
	}

	// 结算
	{
		int rand_value = RandomNum(10000);
		if (rand_value < up_star_stuff_cfg->baoji_per)
		{
			mount->grade_bless_val += up_star_stuff_cfg->star_exp * up_star_stuff_cfg->baoji_mul;
		}
		else
		{
			mount->grade_bless_val += up_star_stuff_cfg->star_exp;
		}

		int up_star_need_exp = curr_star_level_cfg->up_star_level_exp;
		while(mount->grade_bless_val >= up_star_need_exp)
		{	
			const FightMountUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetUpStarExpCfg(mount->star_level + 1);
			if (NULL == next_star_level_cfg || up_star_need_exp <= 0)
			{
				mount->grade_bless_val = 0;
				break;
			}

			mount->grade_bless_val -= up_star_need_exp;
			mount->star_level++;

			if ((mount->grade - 1) != (mount->star_level / 10) && (mount->grade - 1)  < (mount->star_level / 10))
			{
				mount->grade = (mount->star_level / 10) + 1;

				const FightMountGradeConfig * grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(mount->grade);
				if (NULL != grade_cfg)
				{
					role->GetFightMountManager()->ActiveImage(grade_cfg->active_image_id, false);
					role->GetFightMountManager()->UseImage(grade_cfg->active_image_id);

					if (grade_cfg->is_broadcast)
					{
						int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_fight_mount_upgrade, 
							UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), grade_cfg->client_grade, grade_cfg->grade, role->GetFightMountManager()->GetMedalSkillLevel());
						if (sendlen > 0) 
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
						}
					}
				}
				
				/*	const FightMountSkillConifg *skill_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSkillCfg(0, mount->skill_level_list[0] + 1);
					if (NULL != skill_cfg && mount->grade >= skill_cfg->mount_grade)
					{
						mount->skill_level_list[0]++;
					}		*/
			}

			up_star_need_exp = next_star_level_cfg->up_star_level_exp;
		}

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);			//形象赋灵增加百分比进阶属性
	}

	role->GetFightMountManager()->SendFightMountInfo();

	role->GetFightMountManager()->PrintFightMount("FightMountUpgrade", old_star_level, old_bless_val);

	EventHandler::Instance().OnFightMountUpgrade(role, old_grade, mount->grade);
	EventHandler::Instance().OnFightMountUpStar(role, old_star_level, mount->star_level);

	return true;

}

int FightMountFunction::UplevelAndSaveBless(Role *role, int add_star)
{
	FightMountManager *fightmount_mng = role->GetFightMountManager();
	if (NULL == fightmount_mng)
	{
		return -1;
	}

	FightMountParam *fightmount = fightmount_mng->GetFightMountParam();
	if (NULL == fightmount)
	{
		return -2;
	}

	if (!fightmount->IsFightMountActived())
	{
		return -3;
	}

	short old_star_level = fightmount->star_level;
	short old_grade = fightmount->grade;
	int old_bless_val = fightmount->grade_bless_val;

	const FightMountUpStarExpCfg * curr_star_level_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetUpStarExpCfg(old_star_level);
	if (NULL == curr_star_level_cfg)
	{
		return -4;
	}

	const FightMountUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetUpStarExpCfg(old_star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return -5;
	}

	{
		fightmount->star_level += add_star;
		fightmount->grade_bless_val = old_bless_val;

		if ((fightmount->grade - 1) != (fightmount->star_level / 10) && (fightmount->grade - 1) < (fightmount->star_level / 10))
		{
			fightmount->grade = (fightmount->star_level / 10) + 1;

			const FightMountGradeConfig * grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(fightmount->grade);
			if (NULL != grade_cfg)
			{
				role->GetFightMountManager()->ActiveImage(grade_cfg->active_image_id, false);
				role->GetFightMountManager()->UseImage(grade_cfg->active_image_id);

				if (grade_cfg->is_broadcast)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_fight_mount_upgrade,
						UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), grade_cfg->client_grade, grade_cfg->grade, role->GetFightMountManager()->GetMedalSkillLevel());
					if (sendlen > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
					}
				}
			}

			const FightMountSkillConifg *skill_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSkillCfg(0, fightmount->skill_level_list[0] + 1);
			if (NULL != skill_cfg && fightmount->grade >= skill_cfg->mount_grade)
			{
				fightmount->skill_level_list[0]++;
			}
		}

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);			//形象赋灵增加百分比进阶属性
	}

	role->GetFightMountManager()->SendFightMountInfo();

	role->GetFightMountManager()->PrintFightMount("FightMountUpgrade", old_star_level, old_bless_val);

	EventHandler::Instance().OnFightMountUpgrade(role, old_grade, fightmount->grade);
	EventHandler::Instance().OnFightMountUpStar(role, old_star_level, fightmount->star_level);

	return fightmount->star_level;
}

bool FightMountFunction::CheckCanUpGrade(Role* role)
{
	if (NULL == role)
	{
		return false;
	}

	FightMountParam* fightmount_param = role->GetFightMountManager()->GetFightMountParam();
	if (NULL == fightmount_param)
	{
		return false;
	}

	if (!fightmount_param->IsFightMountActived())
	{
		return false;
	}

	const FightMountUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetUpStarExpCfg(fightmount_param->star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return false;
	}

	return true;
}
