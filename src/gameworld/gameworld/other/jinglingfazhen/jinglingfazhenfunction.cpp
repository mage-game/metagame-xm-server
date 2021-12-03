#include "jinglingfazhenfunction.hpp"

#include "jinglingfazhenmanager.hpp"
#include "jinglingfazhenconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "other/event/eventhandler.hpp"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/jinglingfazhendef.hpp"
#include "servercommon/string/gameworldstr.h"
#include "scene/guildshadow/guildshadow.hpp"
#include "scene/scene.h"
#include "servercommon/activitydef.hpp"

#include "world.h"
#include "gamelog.h"
#include "item/knapsack.h"

bool JinglingFazhenFunction::GMResetGrade(Role *role)
{
	if (NULL == role) return false;

	JinglingFazhenParam *jingling_fazhen = role->GetJinglingFazhen()->GetJinglingFazhenParam();
	if (NULL == jingling_fazhen)
	{
		role->NoticeNum(errornum::EN_JINGLING_FAZHEN_NOT_EXIST);
		return false;
	}

	jingling_fazhen->grade = 1;
	jingling_fazhen->grade_bless_val = 0;

	role->GetJinglingFazhen()->SendJinglingFazhenInfo();

	return true;
}

bool JinglingFazhenFunction::ActiveJinglingFazhen(Role *role, bool is_recal)
{
	if (NULL == role) return false;

	// 检查等级
	JinglingFazhenManager *jingling_fazhen_mng = role->GetJinglingFazhen();

	JinglingFazhenParam *jingling_fazhen = jingling_fazhen_mng->GetJinglingFazhenParam();
	if (NULL == jingling_fazhen)
	{
		return false;
	}

	if (!jingling_fazhen->IsJinglingFazhenActived())
	{
		jingling_fazhen->grade = 1;
		jingling_fazhen->grade_bless_val = 0;

		const JinglingFazhenGradeConfig *grade_cfg = LOGIC_CONFIG->GetJinglingFazhenCfg().GetGradeConfig(1);
		if (NULL != grade_cfg)
		{
			jingling_fazhen_mng->ActiveImage(grade_cfg->active_image_id, false);
			jingling_fazhen_mng->UseImage(grade_cfg->active_image_id);
		}

		if (is_recal)
		{
			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING_FAZHEN);
		}

		jingling_fazhen_mng->SendJinglingFazhenInfo();

		jingling_fazhen_mng->PrintJinglingFazhen("ActiveJinglingFazhen");
	}

	return true;
}

bool JinglingFazhenFunction::UpStarLevel(Role *role,Protocol::CSJinglingFazhenUpStarLevel *msg)
{
	if (NULL == msg || NULL == role) return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	JinglingFazhenManager *fazhen_mng = role->GetJinglingFazhen();

	JinglingFazhenParam *fazhen = fazhen_mng->GetJinglingFazhenParam();
	if (NULL == fazhen)
	{
		role->NoticeNum(errornum::EN_JINGLING_FAZHEN_NOT_EXIST);
		return false;
	}

	if (!fazhen->IsJinglingFazhenActived())
	{
		return false;
	}

	short old_grade = fazhen->grade;
	short old_bless_val = fazhen->grade_bless_val;

	const JinglingFazhenGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetJinglingFazhenCfg().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_JINGLING_FAZHEN_CANNOT_UPGRADE);
		return false;
	}

	const JinglingFazhenGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetJinglingFazhenCfg().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		role->NoticeNum(errornum::EN_JINGLING_FAZHEN_CANNOT_UPGRADE);
		return false;
	}

	const JinglingFazhenUpGradeProbConfig * upgrade_prob_cfg = LOGIC_CONFIG->GetJinglingFazhenCfg().GetUpGradeProbConfig(old_grade, old_bless_val);
	if (NULL == upgrade_prob_cfg)
	{
		role->NoticeNum(errornum::EN_JINGLING_FAZHEN_CANNOT_UPGRADE);
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		// 进阶符
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = curr_grade_cfg->upgrade_stuff_id;
		stuff_list[0].num = curr_grade_cfg->upgrade_stuff_count;
		stuff_list[0].buyable = msg->is_auto_buy ? true : false;

		if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(role, consume_list, "JinglingFazhenFunction::UpStarLevel"))
	{
		return false;
	}

	{
		// 生效
		int rand_val = RandomNum(10000);
		bool succeeded = false;
		if (rand_val < upgrade_prob_cfg->succ_rate || fazhen->grade_bless_val >= curr_grade_cfg->bless_val_limit)
		{
			succeeded = true;
		}

		if (!succeeded)
		{
			if (1 == curr_grade_cfg->is_clear_bless && 0 == fazhen->clear_upgrade_time)
			{
				fazhen->clear_upgrade_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 24 * 3600;
			}

			fazhen->grade_bless_val += upgrade_prob_cfg->RandAddBlessVal();

			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);

			if (fazhen->grade_bless_val > curr_grade_cfg->bless_val_limit) fazhen->grade_bless_val = curr_grade_cfg->bless_val_limit;
		}
		else
		{
			fazhen->grade++;
			fazhen->grade_bless_val = 0;

			// 进阶成功清空时间
			fazhen->clear_upgrade_time = 0;

			role->GetJinglingFazhen()->ActiveImage(target_grade_cfg->active_image_id, false);
			role->GetJinglingFazhen()->UseImage(target_grade_cfg->active_image_id);

			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING_FAZHEN);

			role->NoticeNum(noticenum::NT_JINGLING_FAZHEN_LIFT_SUCC);

			if (target_grade_cfg->is_broadcast)
			{
				int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_jingling_fazhen_upgrade_notice, 
					UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), target_grade_cfg->client_grade);
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
	}

	role->GetJinglingFazhen()->SendJinglingFazhenInfo();

	role->GetJinglingFazhen()->PrintJinglingFazhen("JinglingFazhenUpgrade", old_grade, old_bless_val);

	return true;
}

bool JinglingFazhenFunction::JinglingFazhenSpecialImgUpgrade(Role *role, short special_img_id)
{
	if (NULL == role)
	{
		return false;
	}

	if (special_img_id <= 0 || special_img_id > JinglingFazhenParam::MAX_SPECIAL_IMAGE_ID)
	{
		return false;
	}

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	JinglingFazhenManager *jinglingfazhen_mng = role->GetJinglingFazhen();

	JinglingFazhenParam *jinglingfazhen = jinglingfazhen_mng->GetJinglingFazhenParam();
	if (NULL == jinglingfazhen)
	{
		role->NoticeNum(errornum::EN_JINGLING_FAZHEN_NOT_EXIST);
		return false;
	}

	if (!jinglingfazhen->IsJinglingFazhenActived())
	{
		return false;
	}

	const int old_grade = jinglingfazhen->special_img_grade_list[special_img_id];
	if (old_grade >= JinglingFazhenParam::MAX_UPGRADE_LIMIT)
	{
		return false;
	}

	const JinglingFazhenSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetJinglingFazhenCfg().GetSpecialImgUpgradeCfg(special_img_id, old_grade + 1);
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
	if (!ItemExtern::ConsumeItem(role, consume_list, "JinglingFazhenFunction::JinglingFazhenSpecialImgUpgrade"))
	{
		return false;
	}

	// 生效
	++ jinglingfazhen->special_img_grade_list[special_img_id];
	const JinglingFazhenGradeConfig *grade_cfg = LOGIC_CONFIG->GetJinglingFazhenCfg().GetGradeConfig(jinglingfazhen->grade);
	if(NULL == grade_cfg)
	{
		return false;
	}

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING_FAZHEN);

	role->GetJinglingFazhen()->SendJinglingFazhenInfo();

	role->GetJinglingFazhen()->PrintJinglingFazhen("JinglingFazhenSpecialImgUpgrade", old_grade, jinglingfazhen->special_img_grade_list[special_img_id]);

	return true;
}
