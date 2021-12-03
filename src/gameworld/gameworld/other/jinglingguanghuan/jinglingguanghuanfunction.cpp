#include "jinglingguanghuanfunction.hpp"

#include "jinglingguanghuanmanager.hpp"
#include "jinglingguanghuanconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "other/event/eventhandler.hpp"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/jinglingguanghuandef.hpp"
#include "servercommon/string/gameworldstr.h"
#include "scene/guildshadow/guildshadow.hpp"
#include "scene/scene.h"
#include "servercommon/activitydef.hpp"

#include "world.h"
#include "gamelog.h"
#include "item/knapsack.h"

bool JinglingGuanghuanFunction::GMResetGrade(Role *role)
{
	if (NULL == role) return false;

	JinglingGuanghuanParam *jingling_guanghuan = role->GetJinglingGuanghuan()->GetJinglingGuanghuanParam();
	if (NULL == jingling_guanghuan)
	{
		role->NoticeNum(errornum::EN_JINGLING_GUANGHUAN_NOT_EXIST);
		return false;
	}

	jingling_guanghuan->grade = 1;
	jingling_guanghuan->grade_bless_val = 0;

	role->GetJinglingGuanghuan()->SendJinglingGuanghuanInfo();

	return true;
}

bool JinglingGuanghuanFunction::ActiveJinglingGuanghuan(Role *role, bool is_recal)
{
	if (NULL == role) return false;

	// 检查等级
	JinglingGuanghuanManager *jingling_guanghuan_mng = role->GetJinglingGuanghuan();

	JinglingGuanghuanParam *jingling_guanghuan = jingling_guanghuan_mng->GetJinglingGuanghuanParam();
	if (NULL == jingling_guanghuan)
	{
		return false;
	}

	if (!jingling_guanghuan->IsJinglingGuanghuanActived())
	{
		jingling_guanghuan->grade = 1;
		jingling_guanghuan->grade_bless_val = 0;

		const JinglingGuanghuanGradeConfig *grade_cfg = LOGIC_CONFIG->GetJinglingGuanghuanCfg().GetGradeConfig(1);
		if (NULL != grade_cfg)
		{
			jingling_guanghuan_mng->ActiveImage(grade_cfg->active_image_id, false);
			jingling_guanghuan_mng->UseImage(grade_cfg->active_image_id);
		}

		if (is_recal)
		{
			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING_GUANGHUAN);
		}

		jingling_guanghuan_mng->SendJinglingGuanghuanInfo();

		jingling_guanghuan_mng->PrintJinglingGuanghuan("ActiveJinglingGuanghuan");
	}

	return true;
}

bool JinglingGuanghuanFunction::UpStarLevel(Role *role,Protocol::CSJinglingGuanghuanUpStarLevel *msg)
{
	if (NULL == msg || NULL == role) return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	JinglingGuanghuanManager *guanghuan_mng = role->GetJinglingGuanghuan();

	JinglingGuanghuanParam *guanghuan = guanghuan_mng->GetJinglingGuanghuanParam();
	if (NULL == guanghuan)
	{
		role->NoticeNum(errornum::EN_JINGLING_GUANGHUAN_NOT_EXIST);
		return false;
	}

	if (!guanghuan->IsJinglingGuanghuanActived())
	{
		return false;
	}

	short old_grade = guanghuan->grade;
	short old_bless_val = guanghuan->grade_bless_val;

	const JinglingGuanghuanGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetJinglingGuanghuanCfg().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		role->NoticeNum(errornum::EN_JINGLING_GUANGHUAN_CANNOT_UPGRADE);
		return false;
	}

	const JinglingGuanghuanGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetJinglingGuanghuanCfg().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		role->NoticeNum(errornum::EN_JINGLING_GUANGHUAN_CANNOT_UPGRADE);
		return false;
	}

	const JinglingGuanghuanUpGradeProbConfig * upgrade_prob_cfg = LOGIC_CONFIG->GetJinglingGuanghuanCfg().GetUpGradeProbConfig(old_grade, old_bless_val);
	if (NULL == upgrade_prob_cfg)
	{
		role->NoticeNum(errornum::EN_JINGLING_GUANGHUAN_CANNOT_UPGRADE);
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
	if (!ItemExtern::ConsumeItem(role, consume_list, "JinglingGuanghuanFunction::UpStarLevel"))
	{
		return false;
	}

	{
		// 生效
		int rand_val = RandomNum(10000);
		bool succeeded = false;
		if (rand_val < upgrade_prob_cfg->succ_rate || guanghuan->grade_bless_val >= curr_grade_cfg->bless_val_limit)
		{
			succeeded = true;
		}

		if (!succeeded)
		{
			if (1 == curr_grade_cfg->is_clear_bless && 0 == guanghuan->clear_upgrade_time)
			{
				guanghuan->clear_upgrade_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 24 * 3600;
			}

			guanghuan->grade_bless_val += upgrade_prob_cfg->RandAddBlessVal();

			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);

			if (guanghuan->grade_bless_val > curr_grade_cfg->bless_val_limit) guanghuan->grade_bless_val = curr_grade_cfg->bless_val_limit;
		}
		else
		{
			guanghuan->grade++;
			guanghuan->grade_bless_val = 0;

			// 进阶成功清空时间
			guanghuan->clear_upgrade_time = 0;

			role->GetJinglingGuanghuan()->ActiveImage(target_grade_cfg->active_image_id, false);
			role->GetJinglingGuanghuan()->UseImage(target_grade_cfg->active_image_id);

			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING_GUANGHUAN);

			role->NoticeNum(noticenum::NT_JINGLING_GUANGHUAN_LIFT_SUCC);

			if (target_grade_cfg->is_broadcast)
			{
				int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_jingling_halo_upgrade_notice, 
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

	role->GetJinglingGuanghuan()->SendJinglingGuanghuanInfo();

	role->GetJinglingGuanghuan()->PrintJinglingGuanghuan("JinglingGuanghuanUpgrade", old_grade, old_bless_val);

	return true;
}

bool JinglingGuanghuanFunction::JinglingGuanghuanSpecialImgUpgrade(Role *role, short special_img_id)
{
	if (NULL == role)
	{
		return false;
	}

	if (special_img_id <= 0 || special_img_id > JinglingGuanghuanParam::MAX_SPECIAL_IMAGE_ID)
	{
		return false;
	}

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	JinglingGuanghuanManager *jinglingguanghuan_mng = role->GetJinglingGuanghuan();

	JinglingGuanghuanParam *jinglingguanghuan = jinglingguanghuan_mng->GetJinglingGuanghuanParam();
	if (NULL == jinglingguanghuan)
	{
		role->NoticeNum(errornum::EN_JINGLING_GUANGHUAN_NOT_EXIST);
		return false;
	}

	if (!jinglingguanghuan->IsJinglingGuanghuanActived())
	{
		return false;
	}

	const int old_grade = jinglingguanghuan->special_img_grade_list[special_img_id];
	if (old_grade >= JinglingGuanghuanParam::MAX_UPGRADE_LIMIT)
	{
		return false;
	}

	const JinglingGuanghuanSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetJinglingGuanghuanCfg().GetSpecialImgUpgradeCfg(special_img_id, old_grade + 1);
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
	if (!ItemExtern::ConsumeItem(role, consume_list, "JinglingGuanghuanFunction::JinglingGuanghuanSpecialImgUpgrade"))
	{
		return false;
	}

	// 生效
	++ jinglingguanghuan->special_img_grade_list[special_img_id];
	const JinglingGuanghuanGradeConfig *grade_cfg = LOGIC_CONFIG->GetJinglingGuanghuanCfg().GetGradeConfig(jinglingguanghuan->grade);
	if(NULL == grade_cfg)
	{
		return false;
	}

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING_GUANGHUAN);

	role->GetJinglingGuanghuan()->SendJinglingGuanghuanInfo();

	role->GetJinglingGuanghuan()->PrintJinglingGuanghuan("JinglingGuanghuanSpecialImgUpgrade", old_grade, jinglingguanghuan->special_img_grade_list[special_img_id]);

	return true;
}