#include "specialfbguide.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"
#include "config/logicconfigmanager.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "other/fb/fbguideconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "protocal/msgfight.h"
#include "world.h"
#include "npc/transmit/fbmanager.hpp"
#include "servercommon/errornum.h"
#include "item/knapsack.h"
#include "task/newtaskmanager.h"
#include "other/rolegoal/rolegoal.hpp"

SpecialFbGuide::SpecialFbGuide(Scene *scene)
	: SpecialLogic(scene), m_is_finish(false)
{
}

SpecialFbGuide::~SpecialFbGuide()
{
}

void SpecialFbGuide::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (!m_is_finish)
	{
		this->OnFinish(role, role->GetPos().x, role->GetPos().y, 0);
	}
	this->AutoPickFallingItem(role);
}

void SpecialFbGuide::OnFinish(Role *role, int monster_x, int monster_y, int monster_id)
{
	if (NULL == role || m_is_finish)
	{
		return;
	}
	m_is_finish = true;

	const FbGuideCfg *fb_cfg = LOGIC_CONFIG->GetFbGuideConfig().GetFbGuideCfgBySceneId(m_scene->GetSceneID());
	if (NULL == fb_cfg) return;

	if (0 == fb_cfg->is_target_type && role->GetTaskManager()->IsComplete(fb_cfg->task_id))
	{
		role->NoticeNum(errornum::EN_TACK_FINISH_YET_ERROR);
		return;
	}

	if (1 == fb_cfg->is_target_type && role->GetRoleGoal()->IsFinishGoal(fb_cfg->target_type))
	{
		role->NoticeNum(errornum::EN_GOAL_FINISH_YET_ERROR);
		return;
	}
	
	// 掉落
	int drop_index = 0;
	const std::vector<UInt16> *drop_id_list = &fb_cfg->drop_id_list1;

	if (PROF_TYPE_PROF_2 == role->GetBaseProf())
	{
		drop_id_list = &fb_cfg->drop_id_list2;
	}
	else if(PROF_TYPE_PROF_3 == role->GetBaseProf())
	{
		drop_id_list = &fb_cfg->drop_id_list3;
	}

	if (0 != drop_id_list->size())
	{
		Posi posi(monster_x, monster_y);

		std::vector<UInt16>::const_iterator it = drop_id_list->begin();
		for (; it != drop_id_list->end(); ++it)
		{
			DROPPOOL->DropHelper(*it, m_scene, posi, role->GetUserId(), role->GetProf(), monster_id, false, false, &drop_index);
		}
	}

	// 奖励
	if (0 != fb_cfg->reward_item_list[0].item_id)
	{
		role->GetKnapsack()->PutListOrMail(fb_cfg->reward_item_list, PUT_REASON_FB_GUIDE);
	}

	if (0 == fb_cfg->is_target_type)
	{
		// 完成任务
		role->GetTaskManager()->OnPassFB(FBManager::FBCT_FBGUIDE);
	}
	else
	{
		// 完成目标
		if (COND_TYPE_FB_GUIDE_YUBIAO_TYPE == fb_cfg->target_type || COND_TYPE_FB_GUIDE_SHUIJING_TYPE == fb_cfg->target_type || COND_TYPE_FB_GUIDE_BOSS_TYPE == fb_cfg->target_type)
		{
			role->GetRoleGoal()->RoleGoalAddRecordTimes(fb_cfg->target_type, 1, 0);
			role->GetRoleGoal()->CheckCond(fb_cfg->target_type, 1, 0);
		}
	}
}

void SpecialFbGuide::OnCreateGather(Role *role, int gather_x, int gather_y, int gather_id, int gather_time)
{
	if (NULL == role || m_is_finish)
	{
		return;
	}

	const FbGuideCfg *fb_cfg = LOGIC_CONFIG->GetFbGuideConfig().GetFbGuideCfgBySceneId(m_scene->GetSceneID());
	if (NULL == fb_cfg) return;
	
	if (m_scene->GatherObjNum() <= FB_GUIDE_GATHER_NUM_MAX && LOGIC_CONFIG->GetFbGuideConfig().IsLawFulGatherId(m_scene->GetSceneID(), gather_id))
	{
		GatherObj *gather_obj = new GatherObj();
		gather_obj->Init(NULL, gather_id, gather_time * 1000, 0, true);
		Posi posi(gather_x, gather_y);
		gather_obj->SetPos(posi);
		m_scene->AddObj(gather_obj);
	}
}

void SpecialFbGuide::SetObjPos(Role * role, Protocol::CSFunOpenSetObjToPos *cmd)
{
	if (m_is_finish || NULL == role || NULL == cmd) return;

	Obj *obj = m_scene->GetObj(cmd->obj_id);
	if (NULL == obj) return;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *temp_role	= (Role*)obj;
		if (NULL != temp_role)
		{
			temp_role->ResetPos(cmd->pos_x, cmd->pos_y);
		}
	}
	else if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *temp_monster = (Monster *)obj;
		if (NULL != temp_monster)
		{
			temp_monster->ResetPos(cmd->pos_x, cmd->pos_y);
		}
	}
}