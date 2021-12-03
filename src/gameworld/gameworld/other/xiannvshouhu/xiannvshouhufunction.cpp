#include "xiannvshouhufunction.hpp"

#include "xiannvshouhumanager.hpp"
#include "xiannvshouhuconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "other/event/eventhandler.hpp"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/xiannvshouhudef.hpp"
#include "servercommon/string/gameworldstr.h"
#include "scene/guildshadow/guildshadow.hpp"
#include "scene/scene.h"
#include "servercommon/activitydef.hpp"

#include "world.h"
#include "gamelog.h"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "other/sublock/sublock.hpp"

bool XiannvShouhuFunction::GMResetGrade(Role *role)
{
	if (NULL == role) return false;

	XiannvShouhuParam *xiannv_shouhu = role->GetXiannvShouhu()->GetXiannvShouhuParam();
	if (NULL == xiannv_shouhu)
	{
		role->NoticeNum(errornum::EN_XIANNV_SHOUHU_NOT_EXIST);
		return false;
	}

	xiannv_shouhu->grade = 1;
	xiannv_shouhu->grade_bless_val = 0;

	role->GetXiannvShouhu()->SendXiannvShouhuInfo();

	return true;
}

bool XiannvShouhuFunction::ActiveXiannvShouhu(Role *role, bool is_recal)
{
	if (NULL == role) return false;

	// 检查等级
	XiannvShouhuManager *xiannv_shouhu_mng = role->GetXiannvShouhu();
	if (NULL == xiannv_shouhu_mng)
	{
		role->NoticeNum(errornum::EN_XIANNV_SHOUHU_NOT_EXIST);
		return false;
	}

	XiannvShouhuParam *xiannv_shouhu = xiannv_shouhu_mng->GetXiannvShouhuParam();
	if (NULL == xiannv_shouhu)
	{
		role->NoticeNum(errornum::EN_XIANNV_SHOUHU_NOT_EXIST);
		return false;
	}

	if (!xiannv_shouhu->IsXiannvShouhuActived())
	{
		xiannv_shouhu->star_level = 0;
		xiannv_shouhu->grade = 1;
		xiannv_shouhu->grade_bless_val = 0;

		const XiannvShouhuGradeConfig *grade_cfg = LOGIC_CONFIG->GetXiannvShouhuCfg().GetGradeConfig(1);
		if (NULL != grade_cfg)
		{
			xiannv_shouhu_mng->ActiveImage(grade_cfg->active_image_id, false);
			xiannv_shouhu_mng->UseImage(grade_cfg->active_image_id);
		}

		if (is_recal)
		{
			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANNV_SHOUHU);
		}

		xiannv_shouhu_mng->SendXiannvShouhuInfo();

		xiannv_shouhu_mng->PrintXiannvShouhu("ActiveXiannvShouhu");
	}

	return true;
}

bool XiannvShouhuFunction::UpStarLevel(Role *role,Protocol::CSXiannvShouhuUpStarLevel *msg)
{
	if (NULL == msg || NULL == role) return false;

	if (msg->stuff_index < 0 || msg->stuff_index >= XiannvShouhuParam::MAX_UP_STAR_ITEM_COUNT)
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

	XiannvShouhuManager *xiannv_shouhu_mng = role->GetXiannvShouhu();

	XiannvShouhuParam *xiannv_shouhu = xiannv_shouhu_mng->GetXiannvShouhuParam();
	if (NULL == xiannv_shouhu)
	{
		role->NoticeNum(errornum::EN_XIANNV_SHOUHU_NOT_EXIST);
		return false;
	}

	if (!xiannv_shouhu->IsXiannvShouhuActived())
	{
		return false;
	}

	short old_star_level = xiannv_shouhu->star_level;
	short old_bless_val = xiannv_shouhu->grade_bless_val;

	const XiannvShouhuUpStarExpCfg * curr_star_level_cfg = LOGIC_CONFIG->GetXiannvShouhuCfg().GetUpStarExpCfg(old_star_level);
	if (NULL == curr_star_level_cfg)
	{
		return false;
	}

	const XiannvShouhuUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetXiannvShouhuCfg().GetUpStarExpCfg(old_star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return false;
	}

	const XiannvShouhuUpStarStuffCfg * up_star_stuff_cfg = LOGIC_CONFIG->GetXiannvShouhuCfg().GetUpStarStuffCfg(msg->stuff_index);
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
	if (!ItemExtern::ConsumeItem(role, consume_list, "XiannvShouhuFunction::UpStarLevel"))
	{
		return false;
	}

	// 结算
	{
		int rand_value = RandomNum(10000);
		if (rand_value < up_star_stuff_cfg->baoji_per)
		{
			xiannv_shouhu->grade_bless_val += up_star_stuff_cfg->star_exp * up_star_stuff_cfg->baoji_mul;
		}
		else
		{
			xiannv_shouhu->grade_bless_val += up_star_stuff_cfg->star_exp;
		}

		int up_star_need_exp = curr_star_level_cfg->up_star_level_exp;
		while(xiannv_shouhu->grade_bless_val >= up_star_need_exp)
		{	
			const XiannvShouhuUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetXiannvShouhuCfg().GetUpStarExpCfg(xiannv_shouhu->star_level + 1);
			if (NULL == next_star_level_cfg || up_star_need_exp <= 0)
			{
				xiannv_shouhu->grade_bless_val = 0;
				break;
			}

			xiannv_shouhu->grade_bless_val -= up_star_need_exp;
			xiannv_shouhu->star_level++;

			if ((xiannv_shouhu->grade - 1) != (xiannv_shouhu->star_level / 10) && (xiannv_shouhu->grade - 1)  < (xiannv_shouhu->star_level / 10))
			{
				xiannv_shouhu->grade = (xiannv_shouhu->star_level / 10) + 1;

				const XiannvShouhuGradeConfig * grade_cfg = LOGIC_CONFIG->GetXiannvShouhuCfg().GetGradeConfig(xiannv_shouhu->grade);
				if (grade_cfg != nullptr)
				{
					role->GetXiannvShouhu()->ActiveImage(grade_cfg->active_image_id, false);
					role->GetXiannvShouhu()->UseImage(grade_cfg->active_image_id);
				}
			}
			
			up_star_need_exp = next_star_level_cfg->up_star_level_exp;
		}

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANNV_SHOUHU);	
	}

	role->GetXiannvShouhu()->SendXiannvShouhuInfo();

	role->GetXiannvShouhu()->PrintXiannvShouhu("XiannvShouhuUpgrade", old_star_level, old_bless_val);
	
	return true;

}
