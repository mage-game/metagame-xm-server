#include "cmdhandler.hpp"

#include "systemcmddef.h"
#include "obj/character/role.h"
#include "engineadapter.h"

#include "other/event/eventhandler.hpp"
#include "gamelog.h"
#include "item/knapsack.h"
#include "task/newtaskmanager.h"
#include "friend/friendmanager.h"
#include "other/daycounter/daycounter.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "task/other/tumotask.hpp"
#include "task/other/husongtask.hpp"

CmdHandler::CmdHandler()
{

}

CmdHandler::~CmdHandler()
{

}

CmdHandler & CmdHandler::Instance()
{
	static CmdHandler cmdhandler;
	return cmdhandler;
}

bool CmdHandler::OnCmdToRole(Role *role, int effect_type, int effect_param, int effect_param1)
{
	bool ret = false;
	switch (effect_type)
	{
	case systemcmd::EFFECT_AUTHORITY:
		{
			ret = role->SetAuthority(effect_param);
		}
		break;

	case systemcmd::EFFECT_USE_GOLD:
		{
			if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(effect_param))
			{
				effect_param = (int)role->GetKnapsack()->GetMoney()->GetGold();
			}

			ret = role->GetKnapsack()->GetMoney()->UseGold(effect_param, "SystemCmdUse", false);
		}
		break;

	case systemcmd::EFFECT_USE_GOLD_BIND:
		{
			if (!role->GetKnapsack()->GetMoney()->GoldBindMoreThan(effect_param))
			{
				effect_param = (int)role->GetKnapsack()->GetMoney()->GetGoldBind();
			}

			ret = role->GetKnapsack()->GetMoney()->UseGoldBind(effect_param, "SystemCmdUse");
		}
		break;

	case systemcmd::EFFECT_USE_COIN_BIND:
		{
			if (!role->GetKnapsack()->GetMoney()->CoinBindMoreThan(effect_param))
			{
				effect_param = (int)role->GetKnapsack()->GetMoney()->GetCoinBind();
			}

			ret = role->GetKnapsack()->GetMoney()->UseCoinBind(effect_param, "SystemCmdUse");
		}
		break;

	case systemcmd::EFFECT_ADD_HISTORY_CHONGZHI:
		{
			ret = effect_param > 0;

			EventHandler::Instance().OnAddChongzhi(role, effect_param);
		}
		break;

	case systemcmd::EFFECT_SET_CHARM:
		{
			ret = role->GetFriendManager()->SetAllCharm(effect_param);
		}
		break;

	case systemcmd::EFEECT_ADD_UNIQUE_TITLE:
		{
			ret = role->GetTitleManager()->AddTitle(effect_param, true, true, true, true);
		}
		break;

	case systemcmd::EFFECT_COMPLETE_TASK:
		{
			ret = role->GetTaskManager()->ForceCompleteTask(effect_param);
		}
		break;

	case systemcmd::EFFECT_SET_LEVEL:
		{
			ret = (effect_param > 0 && effect_param <= MAX_ROLE_LEVEL);

			role->GMSetLevel(effect_param);
		}
		break;

	case systemcmd::EFFECT_CONSUME_ITEM:
		{

			ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
			ItemExtern::ItemConsumeConfig stuff(effect_param, false, effect_param1);

			if (!ItemExtern::GetConsumeListByOrder(role, false, 1, &stuff, &consume_list))
			{
				ret = false;
			}

			if (consume_list.count > 0)
			{
				ret = role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "system::cmdtorole");
			}
		}
		break;
	}

	return ret;
}

bool CmdHandler::OnCmdToRoleCount(Role *role, int count_type, int count, bool only_reduce)
{
	bool ret = false;
	switch (count_type)
	{
	case systemcmd::RCT_TUMO_TASK:
		ret = role->GetDailyTask()->SetCount(count, only_reduce);
		break;

	case systemcmd::RCT_HUSONG_TASK:
		ret = role->GetHusongTask()->SetCount(count, only_reduce);
		break;
	}

	return ret;
}

bool CmdHandler::OnCmdToRoleFbCount(Role *role, int day_count_id, int count, bool only_reduce)
{
	if (day_count_id < DayCounter::DAYCOUNT_ID_FB_START || day_count_id > DayCounter::DAYCOUNT_ID_FB_END) return false;

	if (only_reduce && count >= role->GetDayCounter()->GetDayCount(day_count_id)) return false;

	role->GetDayCounter()->DayCountSet(day_count_id, count);

	return true;
}

bool CmdHandler::OnAddMountWithData(Role *role, int mount_id, int grade_level, int strengthen_level)
{
	if (NULL == role) return false;

	/*const MountGradeConfig *target_grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(grade_level);
	if (NULL == target_grade_cfg || (0 != strengthen_level && NULL == LOGIC_CONFIG->GetMountOpConfig().GetStrengthBaseCfg(strengthen_level)))
	{
		return false;
	}

	int taking_index = 0, mount_cap = 0;
	MountManager *mount_mng = role->GetMountManager();

	MountItem *mount = mount_mng->GetTakingMountItem(&taking_index, &mount_cap);
	if (NULL == mount)
	{
		if (mount_mng->AddMount(mount_id, &taking_index))
		{
			mount = mount_mng->GetMountItem(taking_index);
		}
	}

	if (NULL == mount) return false;
	
	mount->GetMountAttr().grade = grade_level;
	mount->GetMountAttr().strength_level = strengthen_level;

	MountConfig *mountconfig = MOUNTPOOL->GetMountConfig(target_grade_cfg->active_image_id);
	if (NULL != mountconfig) mountconfig->GetMountName(mount->GetMountAttr().mountname);

	mount_mng->ActiveImage(target_grade_cfg->active_image_id, false);
	mount_mng->UseImage(taking_index, target_grade_cfg->active_image_id);

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);

	mount_mng->SetDirty(taking_index);*/

	return true;
}

bool CmdHandler::OnAddPetWithData(Role *role, int pet_id, int level, int wuxing, int grow, int zizhi)
{
	

	return false;
}

