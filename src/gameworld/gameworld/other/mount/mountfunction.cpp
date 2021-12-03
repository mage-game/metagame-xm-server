#include "mountfunction.hpp"

#include "mountmanager.hpp"
#include "mountopconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "other/upgradeskill/upgradeskillconfig.hpp"
#include "obj/character/role.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/errornum.h"
#include "servercommon/struct/mountparam.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/commondata.hpp"
#include "protocal/msgmount.hpp"
#include "world.h"
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

bool MountFunction::Upgrade(Role *role, bool is_auto_buy, int repeat_times, int *old_level, int *coin_cost, int *gold_cost)
{
	if (NULL == role) return false;

	if (repeat_times <= 0)
	{
		return false;
	}

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	MountManager *mount_mng = role->GetMountManager();

	MountParam *mount = mount_mng->GetMountParam();
	if (NULL == mount)
	{
		role->NoticeNum(errornum::EN_MOUNT_NOT_EXIST);
		return false;
	}

	if (!mount->IsMountActived())
	{
		return false;
	}

	const short old_grade = mount->grade;
	const int old_bless_val = mount->grade_bless_val;

	if (NULL != old_level)
	{
		*old_level = old_grade;
	}

	const MountGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_MOUNT_CANNOT_UPGRADE);
		return false;
	}

	const MountGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		role->NoticeNum(errornum::EN_MOUNT_CANNOT_UPGRADE);
		return false;
	}

	bool continue_upgrade = MountFunction::CheckConsumeAndUpgrade(role, repeat_times, is_auto_buy);	//里面处理消耗与实际升阶

	if (mount->grade > old_grade)
	{
		unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
		const UpgradeRushReturnCfg* upgrade_rush_cfg = LOGIC_CONFIG->GetUpgradeRushConfig().GetUpgradeRushReturnCfg(UPGRADE_RUSH_TYPE_MOUNT, mount->grade);
		if (NULL != upgrade_rush_cfg)
		{
			if ((now_time - mount->last_upgrade_time) <= (unsigned int)(upgrade_rush_cfg->limit_time * SECOND_PER_HOUR))
			{
				role->GetKnapsack()->PutOrMail(upgrade_rush_cfg->reward_item, PUT_REASON_FABAO_UP);
			}
		}
		mount->last_upgrade_time = now_time;

		const MountSkillConifg* skill_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSkillCfg(0, mount->skill_level_list[0] + 1);
		if (NULL != skill_cfg && mount->grade >= skill_cfg->mount_grade)
		{
			mount->skill_level_list[0]++;
		}

		role->GetMountManager()->ActiveImage(target_grade_cfg->active_image_id, false);
		role->GetMountManager()->UseImage(target_grade_cfg->active_image_id);

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);

		if (target_grade_cfg->is_broadcast)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mount_upgrade,
				UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade, role->GetMountManager()->GetMedalSkillLevel());
			if (sendlen > 0)
			{
				if (target_grade_cfg->client_grade >= 5)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}

		role->GetMountManager()->SendMountInfo();
	}
	
	role->GetMountManager()->PrintMount("MountUpgrade", old_grade, old_bless_val);

	EventHandler::Instance().OnMountUpgrade(role, old_grade, mount->grade);

	return continue_upgrade;
}

bool MountFunction::UpOneGrade(Role *role, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num)
{
	MountManager *mount_mng = role->GetMountManager();
	if (NULL == mount_mng)
	{
		return false;
	}

	MountParam *mount = mount_mng->GetMountParam();
	if (NULL == mount)
	{
		return false;
	}

	if (!mount->IsMountActived())
	{
		role->NoticeNum(errornum::EN_MOUNT_NOT_EXIST);
		return false;
	}

	if (mount->grade < valid_min_grade)
	{
		role->NoticeNum(errornum::EN_MOUNT_UP_ONE_GRADE_LIMIT);
		return false;
	}
	else if (mount->grade > valid_max_grade)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(return_item_id);
		if (item_base != nullptr)
		{
			for (int loop_time = 0; return_item_num > 0 && loop_time < 1000; ++loop_time)
			{
				int put_num = return_item_num > item_base->GetPileLimit() ? item_base->GetPileLimit() : return_item_num;
				return_item_num -= put_num;

				ItemConfigData return_item(return_item_id, true, put_num);
				role->GetKnapsack()->PutOrMail(return_item, PUT_REASON_MOUNT_UPGRADE_ONE_RETURN);
			}

			return true;
		}
		return false;
	}

	int old_grade = mount->grade;
	int old_bless_val = mount->grade_bless_val;

	const MountGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_MOUNT_CANNOT_UPGRADE);
		return false;
	}

	const MountGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		role->NoticeNum(errornum::EN_MOUNT_CANNOT_UPGRADE);
		return false;
	}

	{
		mount->grade++;
		mount->grade_bless_val = 0;
		mount->star_level = (mount->grade -1) * 10;

		// 			// 进阶成功清空时间
		// 			mount->clear_upgrade_time = 0;

		const MountSkillConifg *skill_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSkillCfg(0, mount->skill_level_list[0] + 1);
		if (NULL != skill_cfg && mount->grade >= skill_cfg->mount_grade)
		{
			mount->skill_level_list[0]++;
		}

		role->GetMountManager()->ActiveImage(target_grade_cfg->active_image_id, false);
		role->GetMountManager()->UseImage(target_grade_cfg->active_image_id);

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);

		if (target_grade_cfg->is_broadcast)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mount_upgrade, 
				UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade, role->GetMountManager()->GetMedalSkillLevel());
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

	role->GetMountManager()->PrintMount("MountUpgrade", old_grade, old_bless_val);

	EventHandler::Instance().OnMountUpgrade(role, old_grade, mount->grade);

	return true;
}

bool MountFunction::UpgradeByItem(Role *role, int target_grade, int reward_item_count)
{
	if (NULL == role || target_grade <= 0 || reward_item_count <= 0)
	{
		return false;
	}

	MountManager *mount_mng = role->GetMountManager();
	if (NULL == mount_mng)
	{
		return false;
	}

	MountParam *mount = mount_mng->GetMountParam();
	if (NULL == mount)
	{
		return false;
	}

	if (!mount->IsMountActived())
	{
		role->NoticeNum(errornum::EN_MOUNT_NOT_EXIST);
		return false;
	}

	if (mount->grade >= target_grade)
	{
		ItemConfigData reward_item;
		reward_item.item_id = LOGIC_CONFIG->GetMountOpConfig().GetUseJinjieCardRewardItemID();
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
		const MountGradeConfig *target_grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(target_grade);
		if (NULL == target_grade_cfg)
		{
			return false;
		}

		int old_grade  = mount->grade;
		mount->grade = static_cast<short>(target_grade);
		mount->grade_bless_val = 0;

		for (int i = 1; i <= target_grade_cfg->active_image_id; ++ i)
		{
			role->GetMountManager()->ActiveImage(i, false);
		}

		role->GetMountManager()->UseImage(target_grade_cfg->active_image_id);

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);

		if (target_grade_cfg->is_broadcast)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mount_upgrade, 
				UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade, role->GetMountManager()->GetMedalSkillLevel());
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

		role->GetMountManager()->PrintMount("MountUpgradeByItems", old_grade, mount->grade, mount->grade_bless_val);

		for (int to_grade = old_grade + 1; to_grade <= mount->grade; ++to_grade)
		{
			EventHandler::Instance().OnMountUpgrade(role, to_grade - 1, to_grade);
		}
	}

	return true;
}

bool MountFunction::GMResetGrade(Role *role)
{
	if (NULL == role) return false;

	MountParam *mount = role->GetMountManager()->GetMountParam();
	if (NULL == mount)
	{
		return false;
	}

	mount->grade = 1;
	mount->grade_bless_val = 0;

	role->GetMountManager()->SendMountInfo();

	return true;
}

bool MountFunction::ActiveMount(Role *role, bool is_recal)
{
	if (NULL == role) return false;

	// 检查等级
	MountManager *mount_mng = role->GetMountManager();

	MountParam *mount = mount_mng->GetMountParam();
	if (NULL == mount)
	{
		return false;
	}
	if (!mount->IsMountActived())
	{
		mount->level = role->GetLevel();
		mount->grade = 1;
		mount->grade_bless_val = 0;
		mount->star_level = 0;

		const MountGradeConfig *grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(1);
		if (NULL != grade_cfg)
		{
			mount_mng->ActiveImage(grade_cfg->active_image_id, false);
			mount_mng->UseImage(grade_cfg->active_image_id);
		}

		const MountSkillConifg *skill_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSkillCfg(0, 1);
		if (NULL != skill_cfg && mount->grade >= skill_cfg->mount_grade)
		{
			mount->skill_level_list[0] = 1;
		}
		
		if (is_recal)
		{
			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);
		}

		mount_mng->SendMountInfo();

		mount_mng->PrintMount("ActiveMount");

		role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_MOUNT, 0, 0);
		JinjiesysFuncHelper::Instance().OnActiveJinjieSys(role, JINJIE_TYPE_MOUNT);
	}

	// 激活传闻
	int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mount_active_content, role->GetUID(), role->GetName(),
		(int)role->GetCamp(), mount->grade, role->GetMountManager()->GetMedalSkillLevel());
	if (len > 0) 
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
	}

	return true;
}

bool MountFunction::MountSpecialImgUpgrade(Role *role, short special_img_id)
{
	if (NULL == role)
	{
		return false;
	}

	if (special_img_id <= 0 || special_img_id >= MountParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return false;
	}

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	MountManager *mount_mng = role->GetMountManager();
	if (NULL == mount_mng)
	{
		return false;
	}

	MountParam *mount = mount_mng->GetMountParam();
	if (NULL == mount)
	{
		return false;
	}

	if (!mount->IsSpecialImageActive(special_img_id))
	{
		role->NoticeNum(errornum::EN_MOUNT_IMG_NOT_ACTIVE);
		return false;
	}

	const int old_grade = mount->special_img_grade_list[special_img_id];
	if (old_grade >= MountParam::MAX_UPGRADE_LIMIT)
	{
		role->NoticeNum(errornum::EN_MOUNT_SPECIAL_IMG_GRADE_LIMIT);
		return false;
	}

	const MountSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSpecialImgUpgradeCfg(special_img_id, old_grade);
	if (NULL == upgrade_cfg)
	{
		return false;
	}
	const MountSpecialImgUpgradeCfg *next_upgrade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSpecialImgUpgradeCfg(special_img_id, old_grade + 1);
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
	if (!ItemExtern::ConsumeItem(role, consume_list, "MountSpecialImgUpgrade"))
	{
		return false;
	}

	// 生效
	++ mount->special_img_grade_list[special_img_id];

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);

	role->GetMountManager()->SendMountInfo();

	role->GetMountManager()->PrintMount("MountSpecialImgUpgrade", old_grade, mount->special_img_grade_list[special_img_id]);

	return true;
}

bool MountFunction::UplevelEquip(Role *role, int equip_idx)
{
	if (NULL == role || equip_idx < 0 || equip_idx >= MountParam::EQUIP_COUNT)
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

	MountManager *mount_mng = role->GetMountManager();

	MountParam *mount = mount_mng->GetMountParam();
	if (NULL == mount)
	{
		return false;
	}

	if (!mount->IsMountActived())
	{
		return false;
	}

	const MountEquipConifg *equip_cfg = LOGIC_CONFIG->GetMountOpConfig().GetEquipCfg(equip_idx);
	if (NULL == equip_cfg)
	{
		return false;
	}

	short level = mount->equip_level_list[equip_idx];
	//if (level >= equip_cfg->max_level - 1)
	//{
	//	role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
	//	return false;
	//}

	if (level < 0)
	{
		return false;
	}

	//策划需求，装备升级级数由坐骑阶数决定
	const MountGradeConfig * cur_grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(mount->grade);
	if (cur_grade_cfg == NULL)
	{
		return false;
	}

	if (level >= cur_grade_cfg->equip_level_toplimit)
	{
		role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
		return false;
	}

	if (mount->grade < LOGIC_CONFIG->GetMountOpConfig().GetActiveEquipGrade())
	{
		return false;
	}

	short old_level = level;

	// 消耗
	if (!role->GetKnapsack()->ConsumeItem(equip_cfg->level_cfg[level].item.item_id, equip_cfg->level_cfg[level].item.num, "MountEquipUplevel"))
	{
		return false;
	}

	// 结算

	mount->equip_level_list[equip_idx]++;

	bool is_active_upgrade_skill = false;
	int min_equip_level = mount_mng->RefreshMountMinEquipLevel();
	if (LOGIC_CONFIG->GetMountOpConfig().GetActiveEquipSkillLevel() <= min_equip_level)
	{
		int equip_skill_level = LOGIC_CONFIG->GetMountOpConfig().GetEquipSkillLevel(min_equip_level);
		if (equip_skill_level > 0)
		{
			mount->equip_skill_level = equip_skill_level;
			is_active_upgrade_skill = true;
		}
	}

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);
	role->GetMountManager()->SendMountInfo();

	if (is_active_upgrade_skill)
	{
		CommonDataParam *common_data = role->GetCommonDataParam();
		if (NULL != common_data)
		{
			if (0 == (common_data->upgrade_skill_skill_active_flag & (1 << UpGradeSkillConfig::SKILL_TYPE_MOUNT)))
			{
				common_data->upgrade_skill_skill_active_flag |= (1 << UpGradeSkillConfig::SKILL_TYPE_MOUNT);
				if (-1 == common_data->upgrade_skill_next_skill_index)
				{
					common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_MOUNT;
				}
				
				role->SendGradeSkillInfo(role);
			}
		}
	}

	mount_mng->PrintMount("UplevelEquip", old_level);

	return true;
}

bool MountFunction::UplevelSkill(Role *role, int skill_idx, bool is_auto_buy)
{
	if (NULL == role || skill_idx < 0 || skill_idx >= MountParam::SKILL_COUNT)
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

	MountManager *mount_mng = role->GetMountManager();
	if (NULL == mount_mng)
	{
		return false;
	}

	MountParam *mount = mount_mng->GetMountParam();
	if (NULL == mount)
	{
		return false;
	}

	if (!mount->IsMountActived())
	{
		role->NoticeNum(errornum::EN_MOUNT_NOT_EXIST);
		return false;
	}

	const MountSkillConifg *target_skill_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSkillCfg(skill_idx, mount->skill_level_list[skill_idx] + 1);
	if (NULL == target_skill_cfg)
	{
		return false;
	}

	if (0 == target_skill_cfg->skill_type)
	{
		return false;
	}

	if (target_skill_cfg->mount_grade > mount->grade)
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

		// 消耗
		if (!ItemExtern::ConsumeItem(role, consume_list, "MountFunction::UplevelSkill"))
		{
			return false;
		}
	}

 	short skill_level = mount->skill_level_list[skill_idx];
	mount->skill_level_list[skill_idx]++;

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);
	role->GetMountManager()->SendMountInfo();

	mount_mng->PrintMount("UplevelSkill", skill_idx, skill_level);

	return true;
}

bool MountFunction::UpStarLevel(Role *role, Protocol::CSMountUpStarLevel *msg)
{
	if (NULL == msg || NULL == role) return false;

	if (msg->stuff_index < 0 || msg->stuff_index >= MountParam::MAX_UP_STAR_ITEM_COUNT)
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

	MountManager *mount_mng = role->GetMountManager();
	if (NULL == mount_mng)
	{
		return false;
	}

	MountParam *mount = mount_mng->GetMountParam();
	if (NULL == mount)
	{
		return false;
	}

	if (!mount->IsMountActived())
	{
		role->NoticeNum(errornum::EN_MOUNT_NOT_EXIST);
		return false;
	}

	short old_star_level = mount->star_level;
	short old_grade = mount->grade;
	int old_bless_val = mount->grade_bless_val;

	const MountUpStarExpCfg * curr_star_level_cfg = LOGIC_CONFIG->GetMountOpConfig().GetUpStarExpCfg(old_star_level);
	if (NULL == curr_star_level_cfg)
	{
		return false;
	}

	const MountUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetMountOpConfig().GetUpStarExpCfg(old_star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return false;
	}

	const MountUpStarStuffCfg * up_star_stuff_cfg = LOGIC_CONFIG->GetMountOpConfig().GetUpStarStuffCfg(msg->stuff_index);
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
	if (!ItemExtern::ConsumeItem(role, consume_list, "MountUpStarLevel"))
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
			const MountUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetMountOpConfig().GetUpStarExpCfg(mount->star_level + 1);
			if (NULL == next_star_level_cfg || up_star_need_exp <= 0)
			{
				mount->grade_bless_val = 0;
				break;
			}

			mount->grade_bless_val -= up_star_need_exp;
			mount->star_level++;

			if ((mount->grade - 1) != (mount->star_level / 10) && (mount->grade - 1) < (mount->star_level / 10))
			{
				mount->grade = (mount->star_level / 10) + 1;

				const MountGradeConfig * grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(mount->grade);
				if (NULL != grade_cfg)
				{
					role->GetMountManager()->ActiveImage(grade_cfg->active_image_id, false);
					role->GetMountManager()->UseImage(grade_cfg->active_image_id);

					if (grade_cfg->is_broadcast)
					{
						int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mount_upgrade, 
							UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), grade_cfg->client_grade, grade_cfg->grade, role->GetMountManager()->GetMedalSkillLevel());
						if (sendlen > 0) 
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
						}
					}
				}

				const MountSkillConifg *skill_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSkillCfg(0, mount->skill_level_list[0] + 1);
				if (NULL != skill_cfg && mount->grade >= skill_cfg->mount_grade)
				{
					mount->skill_level_list[0]++;
				}

			}

			up_star_need_exp = next_star_level_cfg->up_star_level_exp;
		}

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);			//形象赋灵增加百分比进阶属性
	}

	role->GetMountManager()->SendMountInfo();

	role->GetMountManager()->PrintMount("MountUpgrade", old_star_level, old_bless_val);

	EventHandler::Instance().OnMountUpgrade(role, old_grade, mount->grade);
	EventHandler::Instance().OnMountUpStar(role, old_star_level, mount->star_level);

	return true;
}

int MountFunction::UplevelAndSaveBlessValue(Role *role, int add_star)
{
	if (NULL == role) return -1;

	MountParam *mount = role->GetMountManager()->GetMountParam();
	if (NULL == mount)
	{
		return -1;
	}

	if (!mount->IsMountActived())
	{
		return -2;
	}

	short old_star_level = mount->star_level;
	short old_grade = mount->grade;
	int old_bless_val = mount->grade_bless_val;

	const MountUpStarExpCfg * curr_star_level_cfg = LOGIC_CONFIG->GetMountOpConfig().GetUpStarExpCfg(old_star_level);
	if (NULL == curr_star_level_cfg)
	{
		return -3;
	}

	const MountUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetMountOpConfig().GetUpStarExpCfg(old_star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return -4;
	}

	{
		mount->star_level += add_star;
		mount->grade_bless_val = old_bless_val;			// 升星助力中，升星后返还原来的祝福值
														//检查激活技能
		if ((mount->grade - 1) != (mount->star_level / 10) && (mount->grade - 1) < (mount->star_level / 10))
		{
			mount->grade = (mount->star_level / 10) + 1;

			const MountGradeConfig * grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(mount->grade);
			if (NULL != grade_cfg)
			{
				role->GetMountManager()->ActiveImage(grade_cfg->active_image_id, false);
				role->GetMountManager()->UseImage(grade_cfg->active_image_id);

				if (grade_cfg->is_broadcast)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mount_upgrade,
						UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), grade_cfg->client_grade, grade_cfg->grade, role->GetMountManager()->GetMedalSkillLevel());
					if (sendlen > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
					}
				}
			}

			const MountSkillConifg *skill_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSkillCfg(0, mount->skill_level_list[0] + 1);
			if (NULL != skill_cfg && mount->grade >= skill_cfg->mount_grade)
			{
				mount->skill_level_list[0]++;
			}
		}

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);			//形象赋灵增加百分比进阶属性
	}

	role->GetMountManager()->SendMountInfo();

	role->GetMountManager()->PrintMount("MountUpgrade", old_star_level, old_bless_val);

	EventHandler::Instance().OnMountUpgrade(role, old_grade, mount->grade);
	EventHandler::Instance().OnMountUpStar(role, old_star_level, mount->star_level);

	return mount->star_level;
}

bool MountFunction::CheckCanUpGrade(Role* role)         //其实就是检查是否已激活并且是否存在下一星阶
{
	if (NULL == role)
	{
		return false;
	}

	MountParam* mount_param = role->GetMountManager()->GetMountParam();
	if (NULL == mount_param)
	{
		return false;
	}

	if (!mount_param->IsMountActived())
	{
		return false;
	}

	const MountUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetMountOpConfig().GetUpStarExpCfg(mount_param->star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return false;
	}

	return true;
}

bool MountFunction::CheckConsumeAndUpgrade(Role* role, int repeat_times, bool is_auto_buy)
{
	if (NULL == role)
	{
		return false;
	}
	if (repeat_times <= 0)
	{
		return false;
	}
	MountParam *mount = role->GetMountManager()->GetMountParam();
	if (NULL == mount)
	{
		role->NoticeNum(errornum::EN_MOUNT_NOT_EXIST);
		return false;
	}
	bool continue_upgrade = true;		//是否继续升阶
	bool succeeded = false;				//是否升阶成功
	const short old_grade = mount->grade;
	const MountGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_MOUNT_CANNOT_UPGRADE);
		return false;
	}

	int bless_val = mount->grade_bless_val;	//祝福值
	short grade_val = mount->grade;			//阶数

	ItemExtern::ItemConsumeList consume_list;												//消耗列表
	ItemExtern::ItemConsumeList last_consume_list;											//上次的消耗列表(这个表示一定能消耗)
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	int upgrade_stuff2_num = role->GetKnapsack()->Count(curr_grade_cfg->upgrade_stuff2_id); //限时物品2数量
	int stuff_count = 0;												//消耗物品类型数

	for (int i = 0; i < repeat_times; ++i)
	{
		const MountUpGradeProbConfig * upgrade_prob_cfg = LOGIC_CONFIG->GetMountOpConfig().GetUpGradeProbConfig(old_grade, bless_val);
		if (NULL == upgrade_prob_cfg)
		{
			role->NoticeNum(errornum::EN_MOUNT_CANNOT_UPGRADE);
			return false;
		}

		{
			// 如果有限时进阶丹, 则优先使用
			if (upgrade_stuff2_num > 0)
			{
				bool push_succ = false;
				for (int i = 0; i < stuff_count; i++)
				{
					if (stuff_list[i].item_id == curr_grade_cfg->upgrade_stuff2_id)
					{
						stuff_list[i].num += (upgrade_stuff2_num >= curr_grade_cfg->upgrade_stuff_count ? curr_grade_cfg->upgrade_stuff_count : upgrade_stuff2_num);
						push_succ = true;
						break;
					}
				}

				if (!push_succ)
				{
					if (stuff_count >= static_cast<int>(static_array_size(stuff_list)) || upgrade_stuff2_num == 0)
					{
						break;
					}

					stuff_list[stuff_count].item_id = curr_grade_cfg->upgrade_stuff2_id;
					stuff_list[stuff_count].num = (upgrade_stuff2_num >= curr_grade_cfg->upgrade_stuff_count ? curr_grade_cfg->upgrade_stuff_count : upgrade_stuff2_num);
					stuff_list[stuff_count].buyable = is_auto_buy;
					stuff_count++;
				}
			}

			// 如果限时进阶丹用完了，还不够一次批量使用，就使用普通进阶丹
			if (upgrade_stuff2_num < curr_grade_cfg->upgrade_stuff_count)
			{
				bool push_succ = false;
				for (int i = 0; i < stuff_count; i++)
				{
					if (stuff_list[i].item_id == curr_grade_cfg->upgrade_stuff_id)
					{
						stuff_list[i].num += curr_grade_cfg->upgrade_stuff_count - upgrade_stuff2_num;
						push_succ = true;
						break;
					}
				}

				if (!push_succ)
				{
					if (stuff_count >= static_cast<int>(static_array_size(stuff_list)))
					{
						break;
					}

					stuff_list[stuff_count].item_id = curr_grade_cfg->upgrade_stuff_id;
					stuff_list[stuff_count].num = curr_grade_cfg->upgrade_stuff_count - upgrade_stuff2_num;
					stuff_list[stuff_count].buyable = is_auto_buy;
					stuff_count++;
				}
			}
			//获取消耗列表
			if (!ItemExtern::GetConsumeListByOrder(role, false, stuff_count, stuff_list, &consume_list, false))
			{
				continue_upgrade = false;
				break;
			}
			//检查绑元，元宝数量
			if (!ItemExtern::CheckBindGoldAndGoldByConsumeList(role, consume_list))
			{
				continue_upgrade = false;
				break;
			}
			last_consume_list.Set(consume_list);
			upgrade_stuff2_num -= curr_grade_cfg->upgrade_stuff_count;
			if (upgrade_stuff2_num < 0)
			{
				upgrade_stuff2_num = 0;
			}
		}

		{
			// 生效
			int rand_val = RandomNum(10000);
			if (rand_val < upgrade_prob_cfg->succ_rate || bless_val >= curr_grade_cfg->bless_val_limit)
			{
				succeeded = true;
			}

			if (false == succeeded)
			{
				int crit_value = UpgradeSysCrit::Instance().CalculateCritValue(role, UPGRADE_SYS_CRIT_TYPE_MOUNT);
				int new_bless_val = bless_val + (upgrade_prob_cfg->RandAddBlessVal() * crit_value);
				if (new_bless_val > curr_grade_cfg->bless_val_limit) new_bless_val = curr_grade_cfg->bless_val_limit;
				bless_val = new_bless_val;
			}
			else
			{
				grade_val += 1;
				bless_val = 0;
				continue_upgrade = false;
				break;
			}
		}
	}
	// 消耗
	if (!ItemExtern::ConsumeItem(role, last_consume_list, "MountFunction::Upgrade"))
	{
		return false;
	}
	mount->grade_bless_val = bless_val;		//祝福值改变
	if (grade_val > mount->grade)
	{
		mount->grade = grade_val;			//阶数改变
	}
	return continue_upgrade;
}

