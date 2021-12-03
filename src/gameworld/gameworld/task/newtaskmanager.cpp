#include "newtaskmanager.h"
#include "taskrecorder.h"
#include "item/itempool.h"
#include "taskpool.h"
#include "obj/character/role.h"
#include "scene/scene.h"
#include "scene/changescenedelayer.h"
#include "scene/scenemanager.h"
#include "monster/monsterpool.h"
#include "monster/monsterinitparam.h"
#include "servercommon/errornum.h"
#include "servercommon/servercommon.h"
#include "servercommon/string/gameworldstr.h"
#include "protocal/msgnpc.h"
#include "engineadapter.h"
#include "gamelog.h"
#include "other/event/eventhandler.hpp"
#include "gameworld/world.h"
#include "gameworld/globalconfig/globalconfig.h"
#include "config/logicconfigmanager.hpp"
#include "global/bossfamily/bossfamilyconfig.hpp"
#include "global/challengefield/challengefield.hpp"
#include "scene/speciallogic/worldspecial/jinghuahusongmgr.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "equipment/equipmentmanager.h"
#include "equipment/stonemananger.hpp"
#include "other/jingling/jinglingmanager.h"
#include "other/runesystem/runesystem.hpp"
#include "item/knapsack.h"
#include "friend/friendmanager.h"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "task/other/tumotask.hpp"
#include "task/other/husongtask.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/footprint/footprint.hpp"
#include "task/other/guildtask.hpp"
#include "task/other/datingtask.hpp"
#include "other/fb/rolepatafb.hpp"
#include "task/other/cishatask.hpp"
#include "other/chengjiu/chengjiu.h"
#include "other/fb/roledailyfb.hpp"
#include "task/other/paohuantask.hpp"
#include "other/xiannv/xiannvmanager.hpp"
#include "other/fb/rolefbtuitu.h"
#include "servercommon/commondata.hpp"
#include "npc/npc.h"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "protocal/msgnpc.h"
#include "other/daycounter/daycounter.hpp"
#include "other/specialappearance/specialappearance.hpp"
#include "other/zhuanzhitask.hpp"
#include "config/zhuanzhiconfig.hpp"
#include "config/personbossconfig.hpp"
#include "other/rolezhuanzhi/rolezhuanzhi.hpp"
#include "other/fb/rolefbarmordefend.hpp"
#include "other/fb/rolefbequip.hpp"
#include "other/fb/rolefbchallenge.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "other/fb/rolefbphase.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"
#include "other//greatesoldier/greatesoldier.hpp"
#include "servercommon/serverconfig/openserverconfig.hpp"
#include "crossserver/config/sharedconfig.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "global/droplog/droplogmanager.hpp"

NewTaskManager::NewTaskManager()
	: m_role(NULL), m_first(true)
{
	
}

NewTaskManager::~NewTaskManager()
{
	
}

bool NewTaskManager::Init(Role *role, const TaskParam &taskparam)
{
	m_role = role;

	for (int i = 0;i < taskparam.task_count && i < TaskParam::MAX_ACCEPTED_TASK_NUM; ++i)
	{
		const TaskInfo &tmp_info = taskparam.taskinfo_list[i];

		const TaskCfg * task_cfg = TASKPOOL->GetCfgByTaskID(tmp_info.task_id);
		if (NULL == task_cfg)
		{
			gamelog::g_log_task.printf(LL_INFO, "NewTaskManager::Init role(%d,%s) no config task(%d)",
				m_role->GetUID(), m_role->GetName(), tmp_info.task_id);
			continue;
		}

		if (task_cfg->ver != tmp_info.task_ver)
		{
			gamelog::g_log_task.printf(LL_INFO, "NewTaskManager::Init role(%d,%s) config task(%d) ver(%d) != my(%d)",
				m_role->GetUID(), m_role->GetName(), tmp_info.task_id,
				task_cfg->ver, tmp_info.task_ver);
			continue;
		}

		m_task_param.AddTask(tmp_info);
	}

	this->CheckCishaTaskValid(true);
	
	return true;
}

void NewTaskManager::GetOtherInitParam(TaskParam *param)
{
	*param = m_task_param;
}

void NewTaskManager::Update(time_t now_second)
{
	if (m_first)
	{
		m_first = false;
	
		if (0 == m_task_param.task_count && 1 == m_role->GetLevel() && m_role->GetTaskRecorder()->IsEmpty()) // 出生给任务
		{
			const TaskID ROLE_BIRTH_TASK_ID = GLOBALCONFIG->GetRoleBirthTaskId();
			if (INVALID_TASK_ID != ROLE_BIRTH_TASK_ID) this->AcceptTask(ROLE_BIRTH_TASK_ID);
		}

		for (int i = 0; i < m_task_param.task_count; ++ i)
		{
			const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(m_task_param.taskinfo_list[i].task_id);
			if (NULL != task_cfg)
			{
				if (TASK_CON_SATISFY_STATUS == task_cfg->condition) // 对满足状态的任务类型，上线检查是否满足条件
				{
					this->OnCheckSatisfyStatus(task_cfg->c_param1);
				}
			}
		}
	}

	/*if (!m_first)
	{*/
	int open_game_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;  // 开服天数

	std::set<int> auto_commit_task_id_set;

	for (int i = 0; i < m_task_param.task_count && i < TaskParam::MAX_ACCEPTED_TASK_NUM; ++i)
	{
		const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(m_task_param.taskinfo_list[i].task_id);
		if (nullptr != task_cfg)
		{
			// 检测超时时间
			if (task_cfg->auto_commit_time > 0)
			{
				if ((now_second - m_task_param.taskinfo_list[i].accept_time) >= task_cfg->auto_commit_time)
				{
					auto_commit_task_id_set.insert(m_task_param.taskinfo_list[i].task_id);
				}
			}

			// 检测开服天数
			if (task_cfg->auto_commit_openday > 0 && !CrossConfig::Instance().IsHiddenServer())
			{
				if (open_game_day > task_cfg->auto_commit_openday)
				{
					auto_commit_task_id_set.insert(m_task_param.taskinfo_list[i].task_id);
				}
			}
		}

		// 判断竞技场是否开启，关闭时仍未完成则自动提交
		if (!ChallengeField::Instance().IsChallengeOpen() && nullptr != task_cfg)
		{
			if (TASK_CON_DO_OPERATOR == task_cfg->condition && TASK_OPERATOR_TYPE_1V1_FIELD == task_cfg->c_param1)
			{
				auto_commit_task_id_set.insert(m_task_param.taskinfo_list[i].task_id);
			}
		}
	}

	for (std::set<int>::iterator it = auto_commit_task_id_set.begin(); it != auto_commit_task_id_set.end(); ++it)
	{
		this->RemoveTask(*it, true, true);
		this->OnCommitOneTask(*it);
	}
	//}
}

bool NewTaskManager::CanAcceptTask(TaskID task_id, bool notify)
{
	const TaskCfg *task = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task) return false;

	// 基本条件检查

	if (this->HasTask(task_id))					// 身上是否已经有该任务
	{
		return false;
	}

	if (!m_role->IsAlive())
	{
		return false;
	}

	if (task->min_level > 0 && m_role->GetLevel() < task->min_level)
	{
		if (notify) m_role->NoticeNum(errornum::EN_TASK_ACCEPT_LEVEL);
		return false;
	}

	if (task->max_level > 0 && m_role->GetLevel() > task->max_level)
	{
		if (notify) m_role->NoticeNum(errornum::EN_TASK_ACCEPT_MAX_LEVEL);
		return false;
	}

	if (task->pretaskid > 0)
	{
		if (!m_role->GetTaskRecorder()->IsFinished(task->pretaskid))
		{
			if (notify) m_role->NoticeNum(errornum::EN_TASK_ACCEPT_PRE_TASK);
			return false;
		}
	}

	//如是是特殊形象变身任务中，则不能接取别的特殊形象变身
	if (task->condition == TASK_CON_BIANSHEN_TALK && task->c_param1 != 0)
	{
		if (m_role->GetAcceptTaskAppearnParam() != 0)
		{
			if (!notify)
			{
				return false;
			}
			switch (m_role->GetAcceptTaskAppearn())
			{
			case Role::CHANGE_MODE_TASK_TYPE_GATHER:
				m_role->NoticeNum(errornum::EN_TASK_APPERAN_CHANGED_GATHER);
				break;
			case Role::CHANGE_MODE_TASK_TYPE_CHANGE_IMAGE:
				m_role->NoticeNum(errornum::EN_TASK_APPERAN_CHANGED_IMAGE);
				break;
			case Role::CHANGE_MODE_TASK_TYPE_FLY:
				m_role->NoticeNum(errornum::EN_TASK_APPERAN_CHANGED_FLY);
				break;
			default:
				break;
			}
			return false;
		}
	}

	// 特定任务类型检查
	switch(task->task_type)
	{
	case TASK_TYPE_TRUNK:
	case TASK_TYPE_BRANCH:
		{
			if (m_role->GetTaskRecorder()->IsFinished(task_id))
			{
				return false;
			}
		}
		break;

	case TASK_TYPE_HUSONG:
		{
			// 护送任务
			if (!m_role->GetHusongTask()->CanAcceptHusongTask(notify))
			{
				return false;
			}

			// 精华护送任务中不能再接护送任务
			if (JingHuaHuSongMgr::Instance().IsHusongStatus(m_role))
			{
				if (notify) m_role->NoticeNum(errornum::EN_TASK_ACCEPT_JINGHUA_LIMIT);
				return false;
			}
		}
		break;

	case TASK_TYPE_DAILY:
		{
			// 日常屠魔
			if (!m_role->GetDailyTask()->CanAcceptTask(notify))
			{
				return false;
			}
		}
		break;

	case TASK_TYPE_GUILD:
		{
			if (!m_role->GetGuildTask()->CanAcceptTask(task_id, notify))
			{
				return false;
			}
		}
		break;

	case TASK_TYPE_CISHA:
		{
			// 刺杀任务
			if (!m_role->GetCishaTask()->CanAcceptTask(task_id, notify))
			{
				return false;
			}
		}
		break;

	case TASK_TYPE_PAOHUAN:
		{
			// 跑环任务
			if (!m_role->GetPaoHuanTask()->CanAcceptTask(notify))
			{
				return false;
			}
		}
		break;

	case TASK_TYPE_ZHUANZHI:
		{
			// 转职任务
			if (!m_role->GetZhuanzhiTask()->CanAcceptTask(task_id))
			{
				return false;
			}
		}
		break;
	}

	return true;
}

bool NewTaskManager::CanCommitTask(TaskID task_id, bool notify, bool *has_task)
{
	const TaskCfg *task = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task) return false;

	// 身上有任务
	TaskInfo *task_info = m_task_param.GetTaskInfo(task_id);
	if (NULL == task_info)
	{
		return false;
	}
	if (NULL != has_task)
	{
		*has_task = true;
	}

	// 检查NPC距离 
	if (0 != task->commit_npc && TASK_TYPE_DAILY != task->task_type)
	{
		NPC *npc = m_role->GetScene()->GetNPCList()->GetNpc(task->commit_npc);
		if (NULL == npc)
		{
			return false;
		}

		if (!npc->CanTouch(m_role))
		{
			return false;
		}
	}

	// 任务是否已经完成
	if (this->IsComplete(task_info))
	{
		return true;
	}

	return false;
}

bool NewTaskManager::AcceptTask(TaskID task_id, bool notify, bool is_silent, bool is_check_can_accept)
{
	if (!this->CanAcceptTask(task_id, notify) && is_check_can_accept)
	{
		return false;
	}

	if (m_task_param.task_count >= TaskParam::MAX_ACCEPTED_TASK_NUM)
	{
		m_role->NoticeNum(errornum::EN_TASK_MAX_NUM);
		return false;
	}

	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task_cfg)
	{
		return false;
	}

	// 检查NPC距离
	if (0 != task_cfg->accept_npc && is_check_can_accept && TASK_TYPE_DAILY != task_cfg->task_type)
	{
		NPC *npc = m_role->GetScene()->GetNPCList()->GetNpc(task_cfg->accept_npc);
		if (NULL == npc)
		{
			return false;
		}

		if (!npc->CanTouch(m_role))
		{
			m_role->NoticeNum(errornum::EN_TOO_FAR_FROM_NPC);
			return false;
		}
	}

	if (!this->PreOperate(task_cfg))
	{
		return false;
	}

	TaskInfo taskinfo;
	taskinfo.task_id = task_id;
	taskinfo.accept_time = (unsigned int)(EngineAdapter::Instance().Time());
	taskinfo.task_ver = task_cfg->ver;
	taskinfo.task_condition = task_cfg->condition;
	taskinfo.progress_num = 0;

	m_task_param.AddTask(taskinfo);

	this->SendTaskInfo(&taskinfo, Protocol::SCTaskInfo::REASON_ACCEPT, task_cfg, is_silent);

	this->OnAddTask(task_id, false);

	ROLE_LOG_QUICK6(LOG_TYPE_ACCEPT_TASK, m_role, task_cfg->task_type, task_id, NULL, NULL);

	{
		gamelog::g_log_task.printf(LL_INFO, "[TaskManager::AddTask][user[%d %s] task_id:%d task_count:%d]",
				m_role->GetUID(), m_role->GetName(), (int)task_id, m_task_param.task_count);

		gamelog::g_log_task_stats.printf(LL_INFO, "[TaskResult::accept][role[%d %s] task_id:%d task_level:%d task_type:%d role_professional:%d]", 
			m_role->GetUID(), m_role->GetName(), (int)task_id, task_cfg->min_level, (int)task_cfg->task_type, (int)m_role->GetProf());
	}

	return true;
}

bool NewTaskManager::RemoveTask(TaskID task_id, bool is_complete, bool is_silent)
{
	int t_index = -1;
	TaskInfo *task_info = m_task_param.GetTaskInfo(task_id, &t_index);
	if (NULL == task_info)
	{
		return false;
	}

	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task_cfg)
	{
		return false;
	}

	if (is_complete)
	{
		if (!this->DoReward(task_cfg))
		{
			gamelog::g_log_task.printf(LL_ERROR, "[TaskManager::CommitTask DoReward fail][user[%d %s] task_id:%d complete:%d]",
				m_role->GetUID(), m_role->GetName(), (int)task_id, (int)is_complete);
			return false;
		}
	}

	if (task_cfg->accept_op > 0)
	{
		this->PreOperate(task_cfg, true);
	}

	gamelog::g_log_task.printf(LL_INFO, "[TaskManager::CommitTask][user[%d %s] role_level[%d], task_type[%d], task_id[%d] complete[%d], left_task:%d ]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), task_cfg->task_type, (int)task_id, (int)is_complete, m_task_param.task_count);

	if (is_complete)
	{
		ROLE_LOG_QUICK6(LOG_TYPE_COMMIT_TASK, m_role, task_cfg->task_type, task_id, NULL, NULL);

		{
			gamelog::g_log_task_stats.printf(LL_INFO, "[TaskResult::finish][role[%d %s] task_id:%d task_level:%d task_type:%d role_professional:%d]", 
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)task_id, task_cfg->min_level, task_cfg->task_type, (int)m_role->GetProf());
		}
	}
	else
	{
		// 主线和支线的放弃任务由这里记录，其他的任务类型放弃，各自记录。如日常、仙盟、护送任务
		if (TASK_TYPE_TRUNK == task_cfg->task_type || TASK_TYPE_BRANCH == task_cfg->task_type)
		{
			ROLE_LOG_QUICK6(LOG_TYPE_GIVEUP_TASK, m_role, task_cfg->task_type, task_id, NULL, NULL);

			gamelog::g_log_task_stats.printf(LL_INFO, "[TaskResult::quit][role[%d %s] task_id:%d task_level:%d task_type:%d role_professional:%d]", 
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)task_id, task_cfg->min_level, task_cfg->task_type, (int)m_role->GetProf());
		}
	}

	this->SendTaskInfo(task_info, Protocol::SCTaskInfo::REASON_REMOVE, task_cfg, is_silent);

	m_task_param.RemoveByIndex(t_index);

	switch (task_cfg->task_type)
	{
	case TASK_TYPE_TRUNK:
	case TASK_TYPE_BRANCH:
		m_role->GetTaskRecorder()->Complete(task_id);
		break;
	}

	this->OnRemoveTask(task_id, is_complete);

	if (TASK_TYPE_TRUNK == task_cfg->task_type || TASK_TYPE_BRANCH == task_cfg->task_type || TASK_TYPE_DAILY == task_cfg->task_type || TASK_TYPE_GUILD == task_cfg->task_type
		|| TASK_TYPE_ZHUANZHI == task_cfg->task_type)
	{
		this->CalcAcceptable(CALC_ACC_REASON_COMMIT, task_id);
	}

	return true;
}

bool NewTaskManager::CommitTask(TaskID task_id, bool notify, bool is_silent, bool is_force)
{
	if (!is_force)
	{
		if (!this->CanCommitTask(task_id, notify))
		{
			return false;
		}

		if (!this->OnTaskConsumeItem(task_id))
		{
			return false;
		}
	}

	if (!RemoveTask(task_id, true, is_silent))
	{
		return false;
	}

	this->OnCommitOneTask(task_id);

	return true;
}

bool NewTaskManager::GiveupTask(TaskID task_id, bool is_silent)
{
	if (!RemoveTask(task_id, false, is_silent))
	{
		return false;
	}

	return true;
}

bool NewTaskManager::HasTask(TaskID task_id, bool *is_complete, unsigned int *accept_time)
{
	TaskInfo *task_info = m_task_param.GetTaskInfo(task_id);
	if (NULL == task_info)
	{
		return false;
	}

	if (NULL != is_complete)
	{
		*is_complete = this->IsComplete(task_info);
	}

	if (NULL != accept_time)
	{
		*accept_time = task_info->accept_time;
	}
	
	return true;
} 

bool NewTaskManager::HasTaskType(int task_type)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(m_task_param.taskinfo_list[i].task_id);
		if (NULL != task_cfg)
		{
			if (task_type == task_cfg->task_type)
			{
				return true;
			}
		}
	}

	return false;
}

bool NewTaskManager::GetTaskIdByType(int task_type, int *task_id)
{
	if (NULL == task_id) return false;

	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(m_task_param.taskinfo_list[i].task_id);
		if (NULL != task_cfg)
		{
			if (task_type == task_cfg->task_type)
			{
				*task_id = m_task_param.taskinfo_list[i].task_id;
				return true;
			}
		}
	}

	return false;
}

bool NewTaskManager::HasTaskBianshen(int appearn, int param)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(m_task_param.taskinfo_list[i].task_id);
		if (nullptr != task_cfg)
		{
			if (TASK_CON_BIANSHEN_TALK == task_cfg->condition && 
				task_cfg->c_param1 == appearn && 
				task_cfg->c_param2 == param &&
				!this->IsComplete(&m_task_param.taskinfo_list[i], task_cfg))
			{
				return true;
			}
		}
	}

	return false;
}

void NewTaskManager::OnKillMonster(int scene_id, const Posi &pos, int monster_id, int level)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (TASK_CON_KILL_MONSTER == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_KILL_MONSTER, monster_id, 0);
		}
		else if (TASK_CON_KILL_BOSS_TYPE == m_task_param.taskinfo_list[i].task_condition)
		{
			// 精英boss
			const BossFamilyMikuBossInfoCfg * miku_boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetMikuBossInfo(monster_id);
			// 活跃boss
			const ActiveBossInfoCfg * active_boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetActiveBossInfo(monster_id);
			// vipboss
			const BossFamilyBossInfoCfg * vip_boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyBossInfo(monster_id);
			// 个人boss
			bool is_person_boss = LOGIC_CONFIG->GetPersonBossConfig().IsPersonBoss(monster_id);

			if (NULL != miku_boss_info_cfg)
			{
				this->CheckProgress(i, TASK_CON_KILL_BOSS_TYPE, MonsterInitParam::BOSS_TYPE_TASK_MIKU, 1);
			}
			else if (NULL != active_boss_info_cfg)
			{
				this->CheckProgress(i, TASK_CON_KILL_BOSS_TYPE, MonsterInitParam::BOSS_TYPE_TASK_ACTIVE, 1);
			}
			else if (NULL != vip_boss_info_cfg)
			{
				this->CheckProgress(i, TASK_CON_KILL_BOSS_TYPE, MonsterInitParam::BOSS_TYPE_TASK_VIP, 1);
			}
			else if (is_person_boss)
			{
				this->CheckProgress(i, TASK_CON_KILL_BOSS_TYPE, MonsterInitParam::BOSS_TYPE_TASK_PERSON, 1);
			}
			else
			{
				this->CheckProgress(i, TASK_CON_KILL_BOSS_TYPE, MonsterInitParam::BOSS_TYPE_TASK_NORMAL, 1);
			}
		}
		else if (TASK_CON_KILL_MONSTER_LEVEL == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_KILL_MONSTER_LEVEL, level, 0);
		}
	}
}

void NewTaskManager::OnAddKillMonsterTask(int monster_type)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (TASK_CON_KILL_BOSS_TYPE == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_KILL_BOSS_TYPE, monster_type, 0);
		}
	}
}

void NewTaskManager::OnTalkToNPC(UInt16 npc_id)
{
	if (NULL == m_role->GetScene())
	{
		return;
	}

	NPC *npc = m_role->GetScene()->GetNPCList()->GetNpc(npc_id);
	if (NULL == npc)
	{
		return;
	}

	if (!npc->CanTouch(m_role))
	{
		return;
	}

	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (TASK_CON_TALK == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_TALK, npc_id, 0);
		}

		if (TASK_CON_BIANSHEN_TALK == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_BIANSHEN_TALK, npc_id, 0);
		}
	}


	OnBianshenTalk(Role::CHANGE_MODE_TASK_TYPE_TALK_TO_NPC, npc_id);
}

void NewTaskManager::OnGather(int gather_id)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (TASK_CON_GATHER == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_GATHER, gather_id, 0);
		}
	}	

	OnBianshenTalk(Role::CHANGE_MODE_TASK_TYPE_GATHER, gather_id);
}

void NewTaskManager::OnEnterScene(int scene_id)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (TASK_CON_ENTER_SCENE == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_ENTER_SCENE, scene_id, 0);
		}
	}
}

void NewTaskManager::OnCommitOneTask(TaskID task_id)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (TASK_CON_COMMIT_TASK == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_COMMIT_TASK, task_id, 0);
		}
	}	
}

void NewTaskManager::OnPassFB(int fb_type)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (TASK_CON_PASS_FB == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_PASS_FB, fb_type, 0);
		}
	}	
}

void NewTaskManager::OnPassFBLayer(int fb_type, int param)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (TASK_CON_PASS_FB_LAYER == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_PASS_FB_LAYER, fb_type, param);
		}
	}
}

void NewTaskManager::OnDoOperator(int operator_type)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (TASK_CON_DO_OPERATOR == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_DO_OPERATOR, operator_type, 0);
		}
	}	
}

void NewTaskManager::OnCheckSatisfyStatus(int satisfy_status_type)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (TASK_CON_SATISFY_STATUS == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_SATISFY_STATUS, satisfy_status_type, 0);
		}
	}	
}

void NewTaskManager::OnUpLevel(int old_level, int level)
{
	if (level <= old_level)
	{
		this->CalcAcceptable(CALC_ACC_REASON_UPLEVEL, 0);
	}
	else
	{
		for (int i = old_level + 1; i <= level && i <= MAX_ROLE_LEVEL; i++)
		{
			this->CalcAcceptable(CALC_ACC_REASON_UPLEVEL, 0, i);
		}
	}
}

void NewTaskManager::OnGuildTaskStateChange()
{
	this->CalcAcceptable(CAL_ACC_REASON_GUILD_TASK_STATE_CHANGE, 0);
}

void NewTaskManager::OnZhuanzhiTaskStateChange()
{
	this->CalcAcceptable(CALC_ACC_REASON_ZHUANZHI_TASK_STATE_CHANGE, 0);
}

void NewTaskManager::OnEnterSceneType(int scene_type)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (TASK_CON_ENTER_SCENE_TYPE == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_ENTER_SCENE_TYPE, scene_type, 0);
		}
	}	
}

void NewTaskManager::OnPickUpItem(ItemID item_id, int item_num)
{
	const ItemBase *item = ITEMPOOL->GetItem(item_id);
	if (NULL != item)
	{
		for (int i = 0; i < m_task_param.task_count; ++ i)
		{
			if (TASK_CON_PICKUP_ITEM == m_task_param.taskinfo_list[i].task_condition)
			{
				this->CheckProgress(i, TASK_CON_PICKUP_ITEM, item->GetSearchType(), item_num);
			}
		}	
	}
}

void NewTaskManager::OnPassSpecialTask(int type)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (TASK_CON_PASS_SPECIAL_TASK == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_PASS_SPECIAL_TASK, type, 0);
		}
	}	
}

void NewTaskManager::OnPassDailyTaskFB(int scene_id)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (TASK_CON_PASS_DAILY_TASK_FB == m_task_param.taskinfo_list[i].task_condition)
		{
			this->CheckProgress(i, TASK_CON_PASS_DAILY_TASK_FB, scene_id, 0);
		}
	}	
}

void NewTaskManager::OnBianshenTalk(int type, int param)
{
	if (type == Role::CHANGE_MODE_TASK_TYPE_TALK_TO_NPC)
	{
		if (NULL == m_role->GetScene())
		{
			return;
		}

		NPC *npc = m_role->GetScene()->GetNPCList()->GetNpc(param);
		if (NULL == npc)
		{
			return;
		}

		if (!npc->CanTouch(m_role))
		{
			return;
		}

		for (int i = 0; i < m_task_param.task_count; ++i)
		{
			if (TASK_CON_BIANSHEN_TALK == m_task_param.taskinfo_list[i].task_condition)
			{
				this->CheckProgress(i, TASK_CON_BIANSHEN_TALK, type, param);
			}
		}
	}
	else if (type == Role::CHANGE_MODE_TASK_TYPE_GATHER)
	{
		for (int i = 0; i < m_task_param.task_count; ++i)
		{
			if (TASK_CON_BIANSHEN_TALK == m_task_param.taskinfo_list[i].task_condition)
			{
				this->CheckProgress(i, TASK_CON_BIANSHEN_TALK, type, param);
			}
		}
	}
}

bool NewTaskManager::OnTaskConsumeItem(TaskID task_id)
{
	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task_cfg)
	{
		return false;
	}

	switch (task_cfg->condition)
	{
		case TASK_CON_COMMIT_ITEM:
		{
			ItemID item_id = task_cfg->c_param1;
			int item_num = task_cfg->c_param2;;

			bool is_complete;
			bool has_task = this->HasTask(task_id, &is_complete);
			if (!has_task || !is_complete)
			{
				return false;
			}

			if (!m_role->GetKnapsack()->ConsumeItem(item_id, item_num, "OnTaskConsumeItem"))
			{
				return false;
			}
		}
	}

	return true;
}

void NewTaskManager::OnGetItem(ItemID item_id, int item_num)
{
	const ItemBase *item = ITEMPOOL->GetItem(item_id);
	if (NULL != item)
	{
		for (int i = 0; i < m_task_param.task_count; ++i)
		{
			if (TASK_CON_COMMIT_ITEM == m_task_param.taskinfo_list[i].task_condition)
			{
				this->CheckProgress(i, TASK_CON_COMMIT_ITEM, item_id, item_num);
			}
		}
	}
}

void NewTaskManager::SendTaskListToRole()
{
	Protocol::SCTaskListAck tls;

	tls.count = m_task_param.task_count;

	for (int i = 0; i < m_task_param.task_count && i < TaskParam::MAX_ACCEPTED_TASK_NUM; ++ i)
	{
		Protocol::SCTaskListAck::TaskItem &task_item = tls.taskinfo_list[i];
		const TaskInfo &task_info = m_task_param.taskinfo_list[i];

		task_item.task_id = task_info.task_id;
		task_item.task_ver = task_info.task_ver;
		task_item.task_condition = task_info.task_condition;
		task_item.progress_num = task_info.progress_num;
		task_item.is_complete = (this->IsComplete(&m_task_param.taskinfo_list[i]) ? 1 : 0);
		task_item.reserve_ch = 0;
		task_item.accept_time = task_info.accept_time;
	}

	int sendlen = sizeof(tls) - sizeof(tls.taskinfo_list) + sizeof(tls.taskinfo_list[0]) * tls.count;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tls, sendlen);
}

void NewTaskManager::SendTaskInfo(TaskInfo *taskinfo, int reason, const TaskCfg *task_cfg, bool is_silent)
{
	if (NULL == taskinfo)
	{
		return;
	}

	Protocol::SCTaskInfo scinfo;

	scinfo.reason = reason;
	scinfo.task_id = taskinfo->task_id;
	scinfo.is_complete = this->IsComplete(taskinfo, task_cfg);
	scinfo.param = taskinfo->progress_num;
	scinfo.accept_time = taskinfo->accept_time;
	scinfo.is_silent = (is_silent ? 1 : 0);

	if (task_cfg->condition == TASK_CON_BIANSHEN_TALK && scinfo.is_complete)
	{
		scinfo.param = 1;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&scinfo, sizeof(Protocol::SCTaskInfo));
}

void NewTaskManager::OnAddTask(TaskID task_id, bool is_init)
{
	m_acceptable_set.erase(task_id);
	this->SendAcceptableToRole();

	m_role->GetHusongTask()->OnAddTask(task_id, is_init);
	m_role->GetDailyTask()->OnAddTask(task_id, is_init);
	m_role->GetGuildTask()->OnAddTask(task_id, is_init);
	m_role->GetCishaTask()->OnAddTask(task_id, is_init);
	m_role->GetPaoHuanTask()->OnAddTask(task_id, is_init);
	m_role->GetZhuanzhiTask()->OnAddTask(task_id, is_init);

	if (!is_init)
	{
		const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
		if (NULL != task_cfg)
		{
			if (TASK_CON_SATISFY_STATUS == task_cfg->condition) // 对满足状态的任务类型，一添加任务就马上检查是否满足条件
			{
				this->OnCheckSatisfyStatus(task_cfg->c_param1);
			}
			else if (TASK_CON_DO_OPERATOR == task_cfg->condition) // 对于完成某事的任务类型，有些要一添加任务就修正成完成次数
			{
				this->AdjustDoOperatorTimes(task_id);
			}
			else if (TASK_CON_COMMIT_TASK == task_cfg->condition)
			{
				this->CheckTaskTypeStatus(task_id);
			}
			else if (TASK_CON_ENTER_SCENE_TYPE == task_cfg->condition)
			{
				this->CheckEnterSceneStatus(task_id);
			}
			else if (TASK_CON_KILL_BOSS_TYPE == task_cfg->condition)
			{
				this->OnAddKillMonsterTask(task_cfg->c_param1);
			}
			else if (TASK_CON_PASS_FB_LAYER == task_cfg->condition)
			{
				if (FBManager::FBCT_PATAFB == task_cfg->c_param1)
				{
					this->OnPassFBLayer(FBManager::FBCT_PATAFB);
				}

				else if(FBManager::FBCT_RUNE_TOWER == task_cfg->c_param1)
				{
					this->OnPassFBLayer(FBManager::FBCT_RUNE_TOWER);
				}

				else if (FBManager::FBCT_ARMOR_FB == task_cfg->c_param1)
				{
					this->OnPassFBLayer(FBManager::FBCT_ARMOR_FB);
				}

				else if (FBManager::FBCT_NEQ_FB == task_cfg->c_param1)
				{
					this->OnPassFBLayer(FBManager::FBCT_NEQ_FB);
				}

				else if (FBManager::FBCT_CHALLENGE == task_cfg->c_param1)
				{
					this->OnPassFBLayer(FBManager::FBCT_CHALLENGE);
				}

				else if (FBManager::FBCT_TOWERDEFEND_PERSONAL == task_cfg->c_param1)
				{
					this->OnPassFBLayer(FBManager::FBCT_TOWERDEFEND_PERSONAL);
				}

				else if (FBManager::FBCT_PHASE == task_cfg->c_param1)
				{
					this->OnPassFBLayer(FBManager::FBCT_PHASE, task_cfg->c_param2);
				}
			}
			else if (TASK_CON_BIANSHEN_TALK == task_cfg->condition && TASK_TYPE_DAILY != task_cfg->task_type)
			{
				if (task_cfg->c_param1 == Role::CHANGE_MODE_TASK_TYPE_CHANGE_IMAGE || task_cfg->c_param1 == Role::CHANGE_MODE_TASK_TYPE_FLY)
				{
					m_role->SetAcceptTaskAppearn(task_cfg->c_param1, task_cfg->c_param2);

					//接取后就完成的任务,状态由接取变更至可以提交
					if (this->IsComplete(task_id))
					{
						TaskInfo taskinfo;
						taskinfo.task_id = task_id;
						taskinfo.accept_time = (unsigned int)(EngineAdapter::Instance().Time());
						taskinfo.task_ver = task_cfg->ver;
						taskinfo.task_condition = task_cfg->condition;
						taskinfo.progress_num = 0;

						this->SendTaskInfo(&taskinfo, Protocol::SCTaskInfo::REASON_INFO_CHG, task_cfg);
					}
					m_role->GetGreateSoldier()->ForceClearBianshenStatus();
				}
			}
			else if (TASK_CON_COMMIT_ITEM == task_cfg->condition)
			{
				this->CheckOwnItemStatus(task_id);
			}
		} 
	}
}

void NewTaskManager::OnRemoveTask(TaskID task_id, bool is_complete)
{
	m_role->GetDailyTask()->OnRemoveTask(task_id, is_complete);
	m_role->GetHusongTask()->OnRemoveTask(task_id, is_complete);
	m_role->GetDatingTask()->OnRemoveTask(task_id, is_complete);
	m_role->GetGuildTask()->OnRemoveTask(task_id, is_complete);
	m_role->GetCishaTask()->OnRemoveTask(task_id, is_complete);
	m_role->GetPaoHuanTask()->OnRemoveTask(task_id, is_complete);
	m_role->GetZhuanzhiTask()->OnRemoveTask(task_id, is_complete);

	auto config = TASKPOOL->GetCfgByTaskID(task_id);
	if (config)
	{
		if (config->condition == TASK_CON_BIANSHEN_TALK )
		{//如果删除任务时, 当前任务是变身任务. 则通知客户端变回原形象
			switch (config->c_param1)
			{
			case Role::CHANGE_MODE_TASK_TYPE_CHANGE_IMAGE:
				m_role->SetAcceptTaskAppearn(Role::CHANGE_MODE_TASK_TYPE_CHANGE_IMAGE, 0);
				m_role->SetAcceptTaskAppearn(Role::CHANGE_MODE_TASK_TYPE_GATHER, 0);
				break;
			case Role::CHANGE_MODE_TASK_TYPE_GATHER:
				m_role->SetAcceptTaskAppearn(Role::CHANGE_MODE_TASK_TYPE_GATHER, 0);
				break;
			case Role::CHANGE_MODE_TASK_TYPE_FLY:
				m_role->SetAcceptTaskAppearn(Role::CHANGE_MODE_TASK_TYPE_FLY, 0);
				break;
			default:
				m_role->SetAcceptTaskAppearn(Role::CHANGE_MODE_TASK_TYPE_GATHER, 0);
				break;
			}
		}

		if (config->task_type == TASK_TYPE_DAILY && config->condition == TASK_CON_TALK)
		{
			//日常护送任务减速
			this->RemoveDownSpeedTaskEffetc();
		}
	}

	if (is_complete) 
	{
		EventHandler::Instance().OnCompleteTask(m_role, task_id);
	}
}

bool NewTaskManager::ForceCompleteTask(TaskID task_id)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		if (m_task_param.taskinfo_list[i].task_id == task_id)
		{
			const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
			if (NULL == task_cfg)
			{
				return false;
			}

			switch (task_cfg->condition)
			{
			case TASK_CON_TALK:
			case TASK_CON_KILL_MONSTER:
			case TASK_CON_GATHER:
			case TASK_CON_PASS_FB:
			case TASK_CON_ENTER_SCENE:
			case TASK_CON_COMMIT_TASK:
			case TASK_CON_DO_OPERATOR:
			case TASK_CON_KILL_BOSS_TYPE:
			case TASK_CON_PASS_FB_LAYER:
			case TASK_CON_COMMIT_ITEM:
			case TASK_CON_KILL_MONSTER_LEVEL:
				m_task_param.taskinfo_list[i].progress_num = task_cfg->c_param2;
				break;

			case TASK_CON_SATISFY_STATUS:
			case TASK_CON_PASS_SPECIAL_TASK:
			case TASK_CON_PASS_DAILY_TASK_FB:
				m_task_param.taskinfo_list[i].progress_num = 1;
				break;

			case TASK_CON_BIANSHEN_TALK:
				{
					m_role->SetAcceptTaskAppearn(task_cfg->c_param1, task_cfg->c_param2);
				}

				break;

			}

			this->SendTaskInfo(&m_task_param.taskinfo_list[i], Protocol::SCTaskInfo::REASON_INFO_CHG, task_cfg);

			return true;
		}
	}

	return false;
}

void NewTaskManager::GMClearTrunkTask(TaskID start_task_id, bool is_clear_zhi_xian, bool add_start_task)
{
	std::set<TaskID> remove_task;

	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		const TaskCfg *task = TASKPOOL->GetCfgByTaskID(m_task_param.taskinfo_list[i].task_id);
		if (NULL != task && task->task_id >= start_task_id)
		{
			if (TASK_TYPE_TRUNK == task->task_type)
			{
				remove_task.insert(task->task_id);
			}
			else if (TASK_TYPE_BRANCH == task->task_type && is_clear_zhi_xian) // 支线的也清除
			{
				remove_task.insert(task->task_id);
			}
		}
	}

	for (std::set<TaskID>::iterator it = remove_task.begin(); it != remove_task.end(); ++ it)
	{
			this->GiveupTask(*it);
	}

	m_role->GetTaskRecorder()->GMClearTrunkTask(start_task_id);

	if (add_start_task)
	{
		this->AcceptTask(start_task_id, false, false, false);
	}

	this->SendTaskListToRole();
}

void NewTaskManager::GMAddTask(TaskID task_id)
{
	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task_cfg) return;

	if (TASK_TYPE_TRUNK == task_cfg->task_type || TASK_TYPE_BRANCH == task_cfg->task_type)
	{
		if (this->HasTask(task_id)) 
		{
			return;
		}

		if (task_cfg->min_level > 0 && m_role->GetLevel() < task_cfg->min_level)
		{
			return;
		}

		if (task_cfg->max_level > 0 && m_role->GetLevel() > task_cfg->max_level)
		{
			return;
		}

		if (task_cfg->camp > 0 && m_role->GetCamp() != task_cfg->camp)
		{
			return;
		}

		if (task_cfg->pretaskid > 0)
		{
			if (!m_role->GetTaskRecorder()->IsFinished(task_cfg->pretaskid))
			{
				return;
			}
		}

		if (m_role->GetTaskRecorder()->IsFinished(task_id))
		{
			return;
		}

		if (m_task_param.task_count >= TaskParam::MAX_ACCEPTED_TASK_NUM)
		{
			return;
		}

		if (!this->PreOperate(task_cfg))
		{
			return;
		}

		TaskInfo taskinfo;
		taskinfo.task_id = task_id;
		taskinfo.accept_time = (unsigned int)(EngineAdapter::Instance().Time());
		taskinfo.task_ver = task_cfg->ver;
		taskinfo.task_condition = task_cfg->condition;
		taskinfo.progress_num = 0;

		m_task_param.AddTask(taskinfo);

		this->SendTaskInfo(&taskinfo, Protocol::SCTaskInfo::REASON_ACCEPT, task_cfg);

		this->OnAddTask(task_id, false);
	}
}

void NewTaskManager::GMCommitAllTask()
{
	int try_count = 0;
	while(m_task_param.task_count > 0 && try_count < 256)
	{
		TaskID task_id = m_task_param.taskinfo_list[0].task_id;

		this->ForceCompleteTask(task_id);
		this->GMCommitTask(task_id);

		++ try_count;
	}
}

void NewTaskManager::GMCommitTask(TaskID task_id)
{
	int t_index = -1;
	TaskInfo *task_info = m_task_param.GetTaskInfo(task_id, &t_index);
	if (NULL == task_info)
	{
		return;
	}

	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task_cfg)
	{
		return;
	}

	if (!this->DoReward(task_cfg))
	{
		return;
	}

	if (task_cfg->accept_op > 0)
	{
		this->PreOperate(task_cfg, true);
	}

	this->SendTaskInfo(task_info, Protocol::SCTaskInfo::REASON_REMOVE, task_cfg);

	m_task_param.RemoveByIndex(t_index);

	switch (task_cfg->task_type)
	{
	case TASK_TYPE_TRUNK:
	case TASK_TYPE_BRANCH:
		m_role->GetTaskRecorder()->Complete(task_id);
		break;
	}

	this->OnRemoveTask(task_id, true);

	if (TASK_TYPE_TRUNK == task_cfg->task_type || TASK_TYPE_BRANCH == task_cfg->task_type || TASK_TYPE_GUILD == task_cfg->task_type)
	{
		this->CalcAcceptable(CALC_ACC_REASON_COMMIT, task_id);
	}
}

void NewTaskManager::CalcAcceptable(int reason, TaskID task_id, int up_level)
{
	// 只管主线和支线

	int old_acceptable_count = (int)(m_acceptable_set.size());
	short task_count = 0;
	int task_id_list[MAX_NEXT_TASK_COUNT] = {0};

	if (CALC_ACC_REASON_INIT == reason)
	{
		// 初始化，则全扫描所有主线和支线
		int role_level = m_role->GetLevel();
		int level = 1;
		for (; level <= role_level; ++level)
		{
			task_count = 0;
			TASKPOOL->GetTaskAtLevel(level, task_count, task_id_list);

			for (int i = 0; i < task_count; ++i)
			{
				if (this->CanAcceptTask(task_id_list[i], false))
				{
					m_acceptable_set.insert(task_id_list[i]);
				}
			}
		}

		// 计算可接日常任务（用于不能自动接取的任务）
// 		TaskID daily_task_id = m_role->GetDailyTask()->GetAcceptabelDailyTask();
// 		if (daily_task_id > 0)
// 		{
// 			m_acceptable_set.insert(daily_task_id);
// 		}

		// 计算可接仙盟任务
		const std::set<int> &guild_task_list = TASKPOOL->GetGuildTaskList();

		for (std::set<int>::const_iterator it = guild_task_list.begin(); it != guild_task_list.end(); ++ it)
		{
			int task_id = *it;
			if (this->CanAcceptTask(task_id, false))
			{
				m_acceptable_set.insert(task_id);
			}
		}

		// 计算可接转职任务
		const std::set<int> zhuanzhi_task_list = TASKPOOL->GetZhuanzhiTaskList();

		for (std::set<int>::const_iterator it = zhuanzhi_task_list.begin(); it != zhuanzhi_task_list.end(); ++it)
		{
			int task_id = *it;
			if (this->CanAcceptTask(task_id, false))
			{
				m_acceptable_set.insert(task_id);
			}
		}
	}
	else if (CALC_ACC_REASON_UPLEVEL == reason)
	{
		// 把不可接的任务删掉
		bool need_send = false;
		for (std::set<int>::iterator it = m_acceptable_set.begin(); it != m_acceptable_set.end();)
		{
			if (!this->CanAcceptTask(*it, false))
			{
				m_acceptable_set.erase(it ++);

				need_send = true;
			}
			else
			{
				++ it;
			}
		}

		// 计算新增可接的任务
		TASKPOOL->GetTaskAtLevel(up_level > 0 ? up_level : m_role->GetLevel(), task_count, task_id_list);

		for (int i = 0; i < task_count; ++i)
		{
			if (this->CanAcceptTask(task_id_list[i], false))
			{
				m_acceptable_set.insert(task_id_list[i]);
			}
		}

		// 计算可接日常任务（用于不能自动接取的任务）
// 		TaskID daily_task_id = m_role->GetDailyTask()->GetAcceptabelDailyTask();
// 		if (daily_task_id > 0)
// 		{
// 			m_acceptable_set.insert(daily_task_id);
// 		}

		if (need_send || old_acceptable_count != (int)m_acceptable_set.size())
		{
			this->SendAcceptableToRole();
		}
	}
	else if (CALC_ACC_REASON_COMMIT == reason)
	{
		TASKPOOL->GetNextTask(task_id, task_count, task_id_list);

		int can_accept_count = 0;
		for (int i = 0; i < task_count; ++i)
		{
			if (this->CanAcceptTask(task_id_list[i], false))
			{
				m_acceptable_set.insert(task_id_list[i]);

				can_accept_count++;
			}
		}

		// 处理仙盟任务跨等级段时接不到任务的问题
		if(0 == can_accept_count)
		{
			const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
			if (nullptr != task_cfg && task_cfg->task_type == TASK_TYPE_GUILD)
			{
				const std::set<int> &guild_task_list = TASKPOOL->GetGuildTaskList();

				for (std::set<int>::const_iterator it = guild_task_list.begin(); it != guild_task_list.end(); ++it)
				{
					int task_id = *it;
					if (this->CanAcceptTask(task_id, false))
					{
						m_acceptable_set.insert(task_id);
					}
				}
			}
		}

		// 计算可接日常任务（用于不能自动接取的任务）
// 		TaskID daily_task_id = m_role->GetDailyTask()->GetAcceptabelDailyTask();
// 		if (daily_task_id > 0)
// 		{
// 			m_acceptable_set.insert(daily_task_id);
// 		}


		if (old_acceptable_count != (int)m_acceptable_set.size())
		{
			this->SendAcceptableToRole();
		}
	}
	else if (CAL_ACC_REASON_GUILD_TASK_STATE_CHANGE == reason)
	{
		bool need_send = false;

		// 把可接列表的仙盟任务删除，重新计算可接的仙盟任务
		for (std::set<int>::iterator it = m_acceptable_set.begin(); it != m_acceptable_set.end();)
		{
			const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(*it);
			if (NULL != task_cfg && TASK_TYPE_GUILD == task_cfg->task_type)
			{
				m_acceptable_set.erase(it ++);

				need_send = true;
			}
			else
			{
				++ it;
			}
		}

		const std::set<int> &guild_task_list = TASKPOOL->GetGuildTaskList();

		for (std::set<int>::const_iterator it = guild_task_list.begin(); it != guild_task_list.end(); ++ it)
		{
			int task_id = *it;
			if (this->CanAcceptTask(task_id, false))
			{
				m_acceptable_set.insert(task_id);

				need_send = true;
			}
		}

		if (need_send)
		{
			this->SendAcceptableToRole();
		}
	}
	else if (CALC_ACC_REASON_ZHUANZHI_TASK_STATE_CHANGE == reason)
	{
		bool need_send = false;

		// 把可接列表的转职任务删除，重新计算可接的转职任务
		for (std::set<int>::iterator it = m_acceptable_set.begin(); it != m_acceptable_set.end();)
		{
			const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(*it);
			if (NULL != task_cfg && TASK_TYPE_ZHUANZHI == task_cfg->task_type)
			{
				m_acceptable_set.erase(it++);

				need_send = true;
			}
			else
			{
				++it;
			}
		}

		const std::set<int> &zhuanzhi_task_list = TASKPOOL->GetZhuanzhiTaskList();

		for (std::set<int>::const_iterator it = zhuanzhi_task_list.begin(); it != zhuanzhi_task_list.end(); ++it)
		{
			int task_id = *it;
			if (this->CanAcceptTask(task_id, false))
			{
				m_acceptable_set.insert(task_id);

				need_send = true;
			}
		}

		if (need_send)
		{
			this->SendAcceptableToRole();
		}
	}

	{
		// 没有配置接取任务npc的主线和支线任务要自动接取
		std::set<int> tmp_acceptable_set(m_acceptable_set);

		if (old_acceptable_count != (int)tmp_acceptable_set.size())
		{
			for (std::set<int>::iterator it = tmp_acceptable_set.begin(); it != tmp_acceptable_set.end(); ++ it)
			{
				const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(*it);
				if (NULL != task_cfg)
				{
					if (task_cfg->accept_npc <= 0)
					{
						if (TASK_TYPE_BRANCH == task_cfg->task_type || TASK_TYPE_TRUNK == task_cfg->task_type)
						{
							bool is_silent = (CALC_ACC_REASON_INIT == reason);

							this->AcceptTask(task_cfg->task_id, false, is_silent);
						}
					}
				}
			}
		}
	}
}

bool NewTaskManager::IsComplete(TaskInfo *task_info, const TaskCfg *task_cfg)
{
	if (NULL == task_info)
	{
		return false;
	}

	if (NULL == task_cfg)
	{
		task_cfg = TASKPOOL->GetCfgByTaskID(task_info->task_id);
	}

	if (NULL == task_cfg)
	{
		return false;
	}

	bool is_finish = false;

	switch (task_cfg->condition)
	{
	case TASK_CON_TALK:
	case TASK_CON_KILL_MONSTER:
	case TASK_CON_KILL_BOSS_TYPE:
	case TASK_CON_GATHER:
	case TASK_CON_PASS_FB:
	case TASK_CON_ENTER_SCENE:
	case TASK_CON_COMMIT_TASK:
	case TASK_CON_DO_OPERATOR:
	case TASK_CON_ENTER_SCENE_TYPE:
	case TASK_CON_PICKUP_ITEM:
	case TASK_CON_PASS_FB_LAYER:
	case TASK_CON_COMMIT_ITEM:
	case TASK_CON_KILL_MONSTER_LEVEL:
		{
			if (task_info->progress_num >= task_cfg->c_param2)
			{
				is_finish = true;
			}
		}
		break;

	case TASK_CON_NOTHING:
		{
			is_finish = true;
		}
		break;

	case TASK_CON_SATISFY_STATUS:
	case TASK_CON_PASS_SPECIAL_TASK:
	case TASK_CON_PASS_DAILY_TASK_FB:
		{
			if (task_info->progress_num >= 1)
			{
				is_finish = true;
			}
		}
		break;

	case TASK_CON_BIANSHEN_TALK:
		{
			if (task_cfg->c_param1 == m_role->GetAcceptTaskAppearn() && task_cfg->c_param2 == m_role->GetAcceptTaskAppearnParam())
			{
				is_finish = true;
			}
			else if (task_cfg->c_param1 == Role::CHANGE_MODE_TASK_TYPE_CHANGE_IMAGE && (TASK_TYPE_DAILY != task_cfg->task_type))
			{//变身对话任务接取后立即完成状态
				is_finish = true;
			}
		}
		break;

	}

	return is_finish;
}

bool NewTaskManager::IsComplete(TaskID task_id)
{
	TaskInfo *task_info = m_task_param.GetTaskInfo(task_id);
	if (NULL == task_info)
	{
		return false;
	}

	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_info->task_id);
	if (NULL == task_cfg)
	{
		return false;
	}

	bool is_finish = false;

	switch (task_cfg->condition)
	{
	case TASK_CON_TALK:
	case TASK_CON_KILL_MONSTER:
	case TASK_CON_KILL_BOSS_TYPE:
	case TASK_CON_GATHER:
	case TASK_CON_PASS_FB:
	case TASK_CON_ENTER_SCENE:
	case TASK_CON_COMMIT_TASK:
	case TASK_CON_DO_OPERATOR:
	case TASK_CON_ENTER_SCENE_TYPE:
	case TASK_CON_PICKUP_ITEM:
	case TASK_CON_PASS_FB_LAYER:
		{
			if (task_info->progress_num >= task_cfg->c_param2)
			{
				is_finish = true;
			}
		}
		break;

	case TASK_CON_NOTHING:
		{
			is_finish = true;
		}
		break;

	case TASK_CON_SATISFY_STATUS:
	case TASK_CON_PASS_SPECIAL_TASK:
	case TASK_CON_PASS_DAILY_TASK_FB:
		{
			if (task_info->progress_num >= 1)
			{
				is_finish = true;
			}
		}
		break;
	
	case TASK_CON_BIANSHEN_TALK:
		{
			if (task_cfg->c_param1 == m_role->GetAcceptTaskAppearn() && task_cfg->c_param2 == m_role->GetAcceptTaskAppearnParam())
			{
				is_finish = true;
			}
		}
		break;
	}

	return is_finish;
}

bool NewTaskManager::PreOperate(const TaskCfg *task_cfg, bool is_reverse)
{
	if (NULL == task_cfg)
	{
		return false;
	}

	switch (task_cfg->accept_op)
	{
	case TASK_ACCEPT_OP_FLY:
		{
			if (!is_reverse)	// 正向操作
			{
				m_role->SetMoveMode(Role::MOVE_MODE_FLY, Posi(0, 0), task_cfg->a_param1);
			}
			else
			{
				m_role->SetMoveMode(Role::MOVE_MODE_NORMAL);
			}
		}
		break;

	case TASK_ACCEPT_OP_MOVE:
		{
			if (!is_reverse)	// 正向操作
			{
				if (NULL == m_role->GetScene()) return false;

				if (!m_role->GetRoleStatusManager()->CanTransmit()) return false;

				Scene *scene = m_role->GetScene();
				ChangeSceneDelayer *timer = new ChangeSceneDelayer(World::GetInstWorld()->GetSceneManager(), scene->GetIndex(), m_role->GetId(), 
					m_role->GetUserId(), task_cfg->a_param1, 0, Posi(task_cfg->a_param2, task_cfg->a_param3));

				EngineAdapter::Instance().CreateTimer(1, timer);

				return true;
			}
		}
		break;

	case TASK_ACCEPT_OP_FLY_ON_ACCEPT:
		{
			if (!is_reverse)	// 正向操作
			{
				m_role->SetMoveMode(Role::MOVE_MODE_FLY, Posi(0, 0), task_cfg->a_param1);
			}
		}
		break;

	case TASK_ACCEPT_OP_MOVE_NORMAL_ON_REMOVE:
		{
			if (is_reverse)	// 反向操作
			{
				m_role->SetMoveMode(Role::MOVE_MODE_NORMAL);
			}
		}
		break;

	case TASK_ACCEPT_OP_ADD_SKILL_ON_ACCEPT:
		{
			if (!is_reverse)	// 正向操作
			{
				m_role->GetSkillManager()->AddSkillByTask(m_role, m_role->GetProf(), task_cfg->a_param1);
			}
		}
		break;

	case TASK_ACCEPT_OP_ADD_SKILL_ON_REMOVE:
		{
			if (is_reverse)	// 反向操作
			{
				m_role->GetSkillManager()->AddSkillByTask(m_role, m_role->GetProf(), task_cfg->a_param1);
			}
		}
		break;
	}

	return true;
}

bool NewTaskManager::DoReward(const TaskCfg *task_cfg)
{
	if (NULL == task_cfg)
	{
		return false;
	}
	
	this->taskCommitRandomReward(task_cfg->task_type, 1, 0);

	switch (task_cfg->task_type)
	{
		case TASK_TYPE_TRUNK:
		case TASK_TYPE_BRANCH:
			{
				return this->DoRewardTRunk(task_cfg);
			}
			break;

		case TASK_TYPE_DAILY:
			{
				return m_role->GetDailyTask()->DoReward(task_cfg);
			}
			break;

		case TASK_TYPE_HUSONG:
			{
				return true;
			}
			break;

		case  TASK_TYPE_GUILD:
			{
				return m_role->GetGuildTask()->DoReward(task_cfg);
			}
			break;

		case  TASK_TYPE_CISHA:
			{
				return m_role->GetCishaTask()->DoReward(task_cfg);
			}
			break;

		case  TASK_TYPE_PAOHUAN:
			{
				return m_role->GetPaoHuanTask()->DoReward(task_cfg);
			}
			break;
		case TASK_TYPE_ZHUANZHI:
			{
				return true;
			}
			break;
	}
	
	return false;
}

bool NewTaskManager::DoRewardTRunk(const TaskCfg *task_cfg)
{
	if (NULL == task_cfg)
	{
		return false;
	}

	if (TASK_TYPE_TRUNK != task_cfg->task_type && TASK_TYPE_BRANCH != task_cfg->task_type)
	{
		return false;
	}

	// 检查背包格子
	int need_grid = task_cfg->normal_item_count;

	if (m_role->GetBaseProf() == PROF_TYPE_PROF_1)
	{
		need_grid += task_cfg->prof_item_count_1;
	}
	else if (m_role->GetBaseProf() == PROF_TYPE_PROF_2)
	{
		need_grid += task_cfg->prof_item_count_2;
	}
	else if (m_role->GetBaseProf() == PROF_TYPE_PROF_3)
	{
		need_grid += task_cfg->prof_item_count_3;
	}
	else if (m_role->GetBaseProf() == PROF_TYPE_PROF_4)
	{
		need_grid += task_cfg->prof_item_count_4;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_grid))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	// 给物品
	static const int MAX_REWARD_ITEM_NUM = 16;
	static ItemConfigData item_list[MAX_REWARD_ITEM_NUM];
	int item_num = 0;

	for (int i = 0; i < task_cfg->normal_item_count && item_num < MAX_REWARD_ITEM_NUM; ++i)
	{
		item_list[item_num] = task_cfg->normal_item_list[i];
		++item_num;
	}

	if (m_role->GetBaseProf() == PROF_TYPE_PROF_1)
	{
		for (int i = 0; i < task_cfg->prof_item_count_1 && item_num < MAX_REWARD_ITEM_NUM; ++i)
		{
			item_list[item_num] = task_cfg->prof_item_list_1[i];
			++item_num;
		}
	}
	else if (m_role->GetBaseProf() == PROF_TYPE_PROF_2)
	{
		for (int i = 0; i < task_cfg->prof_item_count_2 && item_num < MAX_REWARD_ITEM_NUM; ++i)
		{
			item_list[item_num] = task_cfg->prof_item_list_2[i];
			++item_num;
		}
	}
	else if (m_role->GetBaseProf() == PROF_TYPE_PROF_3)
	{
		for (int i = 0; i < task_cfg->prof_item_count_3 && item_num < MAX_REWARD_ITEM_NUM; ++i)
		{
			item_list[item_num] = task_cfg->prof_item_list_3[i];
			++item_num;
		}
	}
	else if (m_role->GetBaseProf() == PROF_TYPE_PROF_4)
	{
		for (int i = 0; i < task_cfg->prof_item_count_4 && item_num < MAX_REWARD_ITEM_NUM; ++i)
		{
			item_list[item_num] = task_cfg->prof_item_list_4[i];
			++item_num;
		}
	}

	if (item_num > 0)
	{
		int static out_index_list[MAX_REWARD_ITEM_NUM];
		memset(out_index_list, -1 , sizeof(out_index_list));
		
		if (TASK_TYPE_BRANCH == task_cfg->task_type)
		{
			if (!m_role->GetKnapsack()->PutList(item_num, item_list, PUT_REASON_ZHIXIAN_TASK_REWARD, out_index_list))
			{
				return false;
			}
		}
		else
		{
			if (!m_role->GetKnapsack()->PutList(item_num, item_list, PUT_REASON_TASK_REWARD, out_index_list))
			{
				return false;
			}
		}

		//为装备类加随机传奇属性
		{
			for (int i = 0; i < MAX_REWARD_ITEM_NUM; ++ i)
			{
				if (out_index_list[i] >= 0)
				{
					ItemGridData item_grid_data;
					if (m_role->GetKnapsack()->GetItemGridData(out_index_list[i], &item_grid_data) && 
						EquipmentManager::GetRandomSpeicalAttr(&item_grid_data))
					{
						m_role->GetKnapsack()->SetItemDirty(out_index_list[i]);
					}
				}
			}
		}
	}

	if (task_cfg->coin_bind > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddCoinBind(task_cfg->coin_bind, "TaskReward");
	}

	if (task_cfg->exp > 0)
	{
		m_role->AddExp(task_cfg->exp, "TaskReward", Role::EXP_ADD_REASON_TRUNK_TASK);
	}

	if (task_cfg->nv_wa_shi > 0)
	{
		m_role->AddNvWaShi(task_cfg->nv_wa_shi, "TaskReward");
	}

	if (task_cfg->yuanli > 0)
	{
		m_role->AddYuanli(task_cfg->yuanli, "TaskReward");
	}

	return true;
}

void NewTaskManager::CheckProgress(int t_index, int task_condtion, int param1, int param2)
{
	if (t_index < 0 || t_index >= m_task_param.task_count || m_task_param.taskinfo_list[t_index].task_id <= 0)
	{
		return;
	}

	TaskInfo &tmp_task = m_task_param.taskinfo_list[t_index];

	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(tmp_task.task_id);
	if (NULL == task_cfg)
	{
		gamelog::g_log_task.printf(LL_ERROR, "NewTaskManager::CheckProgress role(%d,%s) no cfg %d",
			m_role->GetUID(), m_role->GetName(), tmp_task.task_id);
		return;
	}

	if (task_cfg->condition != tmp_task.task_condition)
	{
		gamelog::g_log_task.printf(LL_ERROR, "NewTaskManager::CheckProgress role(%d,%s) task %d cond %d != my %d",
			m_role->GetUID(), m_role->GetName(), tmp_task.task_id, task_cfg->condition, tmp_task.task_condition);
		return;
	}

	bool is_complete = this->IsComplete(&tmp_task, task_cfg);
	if (TASK_CON_COMMIT_ITEM != task_condtion && is_complete) // 已经完成不要在继续检查
	{
		return;
	}

	bool progressed = false;

	switch (task_condtion)
	{
	case TASK_CON_KILL_MONSTER:
	case TASK_CON_TALK:
	case TASK_CON_GATHER:
	case TASK_CON_ENTER_SCENE:
	case TASK_CON_PASS_FB:
	case TASK_CON_DO_OPERATOR:
	case TASK_CON_ENTER_SCENE_TYPE:
	{
		if (param1 == task_cfg->c_param1)
		{
			if (task_cfg->task_type == TASK_TYPE_DAILY && TASK_CON_TALK == task_condtion)
			{
				//日常护送任务减速
				this->AddDownSpeedTaskEffect();
			}
			tmp_task.progress_num += 1;
			progressed = true;
		}
	}
	break;

	case TASK_CON_KILL_BOSS_TYPE:
	{
		if (param1 == task_cfg->c_param1)
		{
			tmp_task.progress_num += param2;
			progressed = true;
		}
	}
	break;

	case TASK_CON_COMMIT_TASK:
	{
		const TaskCfg *tmp_cfg = TASKPOOL->GetCfgByTaskID(param1);
		if (NULL != tmp_cfg)
		{
			if (tmp_cfg->task_type == task_cfg->c_param1)	// 提交的任务类型 等于 条件的任务类型
			{
				tmp_task.progress_num += 1;
				progressed = true;
			}
		}
	}
	break;

	case TASK_CON_SATISFY_STATUS:
	{
		if (tmp_task.progress_num <= 0 && task_cfg->c_param1 == param1)
		{
			if (this->CheckSatisfyStatus(task_cfg->c_param1, task_cfg->c_param2, task_cfg->c_param3))
			{
				tmp_task.progress_num = 1;
				progressed = true;
			}
		}
	}
	break;

	case TASK_CON_PICKUP_ITEM:
	{
		if (param1 == task_cfg->c_param1)
		{
			int tmp_num = tmp_task.progress_num + param2;
			if (tmp_num > 30000)
			{
				tmp_num = 30000;
			}

			tmp_task.progress_num = static_cast<short>(tmp_num);
			progressed = true;
		}
	}
	break;

	case TASK_CON_PASS_FB_LAYER:
	{
		if (param1 == task_cfg->c_param1)
		{
			switch (param1)
			{
			case FBManager::FBCT_PATAFB:
			{
				tmp_task.progress_num = m_role->GetRolePataFB()->GetPassLevel();
				progressed = true;
			}
			break;

			case FBManager::FBCT_RUNE_TOWER:
			{
				tmp_task.progress_num = m_role->GetRuneSystem()->GetRuneTowerPassLayer();
				progressed = true;
			}
			break;

			case FBManager::FBCT_XINMO_FB:
			{
				tmp_task.progress_num += 1;
				progressed = true;
			}
			break;

			case FBManager::FBCT_ARMOR_FB:
			{
				tmp_task.progress_num = m_role->GetRoleFbArmorDefend()->GetArmoedefendPassLayer();
				progressed = true;
			}
			break;

			case FBManager::FBCT_NEQ_FB:
			{
				tmp_task.progress_num = m_role->GetFBEquip()->GetNeqFBPassMaxLevel();
				progressed = true;
			}
			break;

			case FBManager::FBCT_CHALLENGE:
			{
				tmp_task.progress_num = m_role->GetFBChallenge()->GetPassLevel();
				progressed = true;
			}
			break;

			case FBManager::FBCT_TOWERDEFEND_PERSONAL:
			{
				tmp_task.progress_num = m_role->GetFBTowerDefend()->GetPersonalFBPassLevel();
				progressed = true;
			}
			break;

			case FBManager::FBCT_PHASE:
			{
				tmp_task.progress_num = m_role->GetRoleFBPhase()->IsPassLevel(param2 - 1) ? param2 : 0;
				progressed = true;
			}
			break;

			}
		}
	}
	break;

	case TASK_CON_PASS_SPECIAL_TASK:
	{
		if (param1 == task_cfg->c_param1)
		{
			tmp_task.progress_num = 1;
			progressed = true;
		}
	}
	break;

	case TASK_CON_PASS_DAILY_TASK_FB:
	{
		if (param1 == task_cfg->c_param1)
		{
			tmp_task.progress_num = 1;
			progressed = true;
		}
	}
	break;

	case TASK_CON_BIANSHEN_TALK:
	{
		if (param1 == task_cfg->c_param1 && param2 == task_cfg->c_param2)
		{
			m_role->SetAcceptTaskAppearn(task_cfg->c_param1, task_cfg->c_param2);
			progressed = true;
		}
		else if (TASK_TYPE_DAILY == task_cfg->task_type 
			&& (Role::CHANGE_MODE_TASK_TYPE_CHANGE_IMAGE == task_cfg->c_param1)
			&& param1 == task_cfg->c_param3)
		{
			m_role->SetAcceptTaskAppearn(task_cfg->c_param1, task_cfg->c_param2);
			progressed = true;
		} else if (Role::CHANGE_MODE_TASK_TYPE_FLY == task_cfg->c_param1 
			&& param1 == task_cfg->c_param3)
		{
			m_role->SetAcceptTaskAppearn(task_cfg->c_param1, task_cfg->c_param2);
			progressed = true;
		}
	}
	break;

	case TASK_CON_COMMIT_ITEM:
	{
		int item_count = m_role->GetKnapsack()->GetItemNum(task_cfg->c_param1);
		if (item_count >= 0)
		{
			tmp_task.progress_num = item_count;
			progressed = true;
		}
	}
	break;

	case TASK_CON_KILL_MONSTER_LEVEL:
	{
		if (task_cfg->c_param1 <= param1 && param1 <= task_cfg->c_param3)
		{
			tmp_task.progress_num += 1;
			progressed = true;
		}
	}
	break;

	}

	if (progressed)
	{
		this->SendTaskInfo(&tmp_task, Protocol::SCTaskInfo::REASON_INFO_CHG, task_cfg);

		if (NULL != task_cfg && this->IsComplete(&tmp_task, task_cfg)) // 统计用
		{
			ROLE_LOG_QUICK6(LOG_TYPE_COMPLETE_TASK, m_role, task_cfg->task_type, task_cfg->task_id, NULL, NULL);

			gamelog::g_log_task_stats.printf(LL_INFO, "[TaskResult::Complete][role[%d %s] task_id:%d task_level:%d task_type:%d role_professional:%d]", 
				UidToInt(m_role->GetUserId()), m_role->GetName(), task_cfg->task_id, task_cfg->min_level, task_cfg->task_type, (int)m_role->GetProf());
		}
	}
}

void NewTaskManager::SendAcceptableToRole()
{
	static Protocol::SCTaskAccpetableTaskList atl;

	int count = 0;
	for (std::set<int>::iterator it = m_acceptable_set.begin(); m_acceptable_set.end() != it; ++ it)
	{
		if (count < Protocol::SCTaskAccpetableTaskList::ACCEPTABLE_TASK_MAX_COUNT)
		{
			atl.accpetable_task_list[count] = static_cast<TaskID>(*it);
			++ count;
		}
		else
		{
			break;
		}
	}
	atl.count = count;

	unsigned int sendlen = sizeof(atl) - (Protocol::SCTaskAccpetableTaskList::ACCEPTABLE_TASK_MAX_COUNT - count) * sizeof(atl.accpetable_task_list[0]);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&atl, sendlen);
}

TaskInfo * NewTaskManager::GetTaskInfo(TaskID task_id)
{
	return m_task_param.GetTaskInfo(task_id);
}

void NewTaskManager::OnRoleInitFinish()
{
	this->CalcAcceptable(CALC_ACC_REASON_INIT, 0);
}

void NewTaskManager::CheckCishaTaskValid(bool is_init)
{
// 	std::set<TaskID> remove_taskid_set;
// 
// 	for (int i = 0; i < m_task_param.task_count; ++ i)
// 	{
// 		const TaskInfo &tmp_info = m_task_param.taskinfo_list[i];
// 
// 		const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(tmp_info.task_id);
// 		if (NULL != task_cfg && TASK_TYPE_CISHA == task_cfg->task_type)
// 		{
// 			if (!LOGIC_CONFIG->GetCishaTaskCfg().Acceptable(m_role->GetCamp(), task_cfg->task_id))
// 			{
// 				remove_taskid_set.insert(task_cfg->task_id);
// 			}
// 		}
// 	}
// 
// 	for (std::set<TaskID>::iterator it = remove_taskid_set.begin(); remove_taskid_set.end() != it; ++ it)
// 	{
// 		this->GiveupTask(*it, is_init);
// 	}
}

bool NewTaskManager::GetCurZhuanzhiTaskid(int *task_id)
{
	if (NULL == task_id) return false;

	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(m_task_param.taskinfo_list[i].task_id);
		if (NULL != task_cfg)
		{
			if (TASK_TYPE_ZHUANZHI == task_cfg->task_type)
			{
				*task_id = m_task_param.taskinfo_list[i].task_id;
				return true;
			}
		}
	}

	const std::set<int> zhuanzhi_task_list = TASKPOOL->GetZhuanzhiTaskList();

	for (std::set<int>::const_iterator it = zhuanzhi_task_list.begin(); it != zhuanzhi_task_list.end(); ++it)
	{
		int taskid = *it;
		if (this->CanAcceptTask(taskid, false))
		{
			*task_id = taskid;
			return true;
		}
	}

	return false;
}

bool NewTaskManager::CheckSatisfyStatus(int satisfy_type, int param1, int param2)
{
	switch (satisfy_type)
	{
	case TASK_SATISFY_STATUS_TYPE_MOUNT_GRADE:
		{
			MountParam *mount_param = m_role->GetMountManager()->GetMountParam();
			if (NULL != mount_param)
			{
				if (mount_param->grade >= param1)
				{
					return true;
				}
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_OWN_FRIEND:
		{
			int friend_num = m_role->GetFriendManager()->GetFriendNumber();
			if (friend_num >= param1)
			{
				return true;
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_JINGLING_MAX_LEVEL:
		{
			if (m_role->GetJingLingMgr()->GetJingLingMaxLevel() >= param1)
			{
				return true;
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_MOUNT_STAR:
		{
			MountParam *mount_param = m_role->GetMountManager()->GetMountParam();
			if (NULL != mount_param)
			{
				if (mount_param->star_level >= param1)
				{
					return true;
				}
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_WING_STAR:
		{
			WingParam *wing_param = m_role->GetWing()->GetWingParam();
			if (NULL != wing_param)
			{
				if (wing_param->star_level >= param1)
				{
					return true;
				}
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_HALO_STAR:
		{
			HaloParam *halo_param = m_role->GetHalo()->GetHaloParam();
			if (NULL != halo_param)
			{
				if (halo_param->star_level >= param1)
				{
					return true;
				}
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_SHENGONG_STAR:
		{
			ShengongParam *shengong_param = m_role->GetShengong()->GetShengongParam();
			if (NULL != shengong_param)
			{
				if (shengong_param->star_level >= param1)
				{
					return true;
				}
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_SHENYI_STAR:
		{
			ShenyiParam *shenyi_param = m_role->GetShenyi()->GetShenyiParam();
			if (NULL != shenyi_param)
			{
				if (shenyi_param->star_level >= param1)
				{
					return true;
				}
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_FIGHT_MOUNT_STAR:
		{
			FightMountParam *fight_mount_param = m_role->GetFightMountManager()->GetFightMountParam();
			if (NULL != fight_mount_param)
			{
				if (fight_mount_param->star_level >= param1)
				{
					return true;
				}
			}
		}
		break;


	case TASK_SATISFY_STATUS_TYPE_WING_GRADE:
		{
			WingParam *wing_param = m_role->GetWing()->GetWingParam();
			if (NULL != wing_param)
			{
				if (wing_param->grade >= param1)
				{
					return true;
				}
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_HALO_GRADE:
		{
			HaloParam *halo_param = m_role->GetHalo()->GetHaloParam();
			if (NULL != halo_param)
			{
				if (halo_param->grade >= param1)
				{
					return true;
				}
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_SHENGONG_GRADE:
		{
			ShengongParam *shengong_param = m_role->GetShengong()->GetShengongParam();
			if (NULL != shengong_param)
			{
				if (shengong_param->grade >= param1)
				{
					return true;
				}
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_SHENYI_GRADE:
		{
			ShenyiParam *shenyi_param = m_role->GetShenyi()->GetShenyiParam();
			if (NULL != shenyi_param)
			{
				if (shenyi_param->grade >= param1)
				{
					return true;
				}
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_FIGHT_MOUNT_GRADE:
		{
			FightMountParam *fight_mount_param = m_role->GetFightMountManager()->GetFightMountParam();
			if (NULL != fight_mount_param)
			{
				if (fight_mount_param->grade >= param1)
				{
					return true;
				}
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_CHENGJIU_TITLE:
		{
			short title_level = 0;
			m_role->GetChengJiuMgr()->GetChengJiuTitleLevel(&title_level);
			if (title_level >= param1)
			{
				return true;
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_PUT_JINLING_COUNT:
		{
			int jingling_count = m_role->GetJingLingMgr()->GetPutOnJinglingCount();
			if (jingling_count >= param1)
			{
				return true;
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_PATA_FB_LEVEL:
		{
			int pass_level = m_role->GetRolePataFB()->GetPassLevel();
			if (pass_level >= param1)
			{
				return true;
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_EXP_FB_WAVE:
		{
			short pass_max_wave = m_role->GetRoleDailyFb()->GetExpFbHistoryEnterTimes();
			if (pass_max_wave >= param1)
			{
				return true;
			}
		}
		break;

	case TASK_SATISFY_STATUS_TYPE_EQUIP_TOTAL_STRENGTH_LEVEL:
	{
		int total_strength_level = m_role->GetEquipmentManager()->GetTotalStrengthen();
		if (total_strength_level >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_EQUIP_TOTAL_STAR_LEVEL:
	{
		int total_star_level = m_role->GetEquipmentManager()->GetTotalStarLevel();
		if (total_star_level >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_ACTIVE_XIANNV:
	{
		int active_xiannv_count = m_role->GetXiannvManager()->GetActiveXiannvCount();
		if (active_xiannv_count >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_INLAY_STONE:
	{
		if (m_role->GetStoneManager()->IsInlayStoneByLevel(param1))
		{
			return true;
		}
	}
	break;

	case TASK_STATIFY_STATUS_TYPE_TUIFU_NORMAL_FB:
	{
		if (m_role->GetTuituFb()->IsPassFb(TUITU_FB_TYPE_NORMAL, param1, param2))
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_FOOTPRINT_GRADE:
	{
		FootprintParam *footprint_param = m_role->GetFootPrint()->GetFootprintParam();
		if (NULL != footprint_param)
		{
			if (footprint_param->grade >= param1)
			{
				return true;
			}
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_FOOTPRINT_STAR:
	{
		FootprintParam *footprint_param = m_role->GetFootPrint()->GetFootprintParam();
		if (NULL != footprint_param)
		{
			if (footprint_param->star_level >= param1)
			{
				return true;
			}
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_RED_EQUIP_WEAR_COUNT:
	{
		if (m_role->GetEquipmentManager()->GetRedEquipCount(param2) >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_LIGHT_UP_TIANMING:
	{
		short tianming_level = m_role->GetRoleZhuanZhi()->GetTianMingLevel();
		if (tianming_level >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_LIGHT_UP_CANGLONG:
	{
		short canglong_level = m_role->GetRoleZhuanZhi()->GetCangLongLevel();
		if (canglong_level >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_LIGHT_UP_SHENGXIAO:
	{
		short shengxiao_level = m_role->GetRoleZhuanZhi()->GetShengXiaoLevel();
		if (shengxiao_level >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_LIGHT_UP_YUANQI:
	{
		short yuanqi_level = m_role->GetRoleZhuanZhi()->GetYuanQiLevel();
		if (yuanqi_level >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_LIGHT_UP_SHENQI:
	{
		short shenqi_level = m_role->GetRoleZhuanZhi()->GetShenQiLevel();
		if (shenqi_level >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_LIGHT_UP_XINGXIU:
	{
		short xingxiu_level = m_role->GetRoleZhuanZhi()->GetXingXiuLevel();
		if (xingxiu_level >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_CON_TO_PROF6:
	{
		short canglong_level = m_role->GetRoleZhuanZhi()->GetCangLongLevel();
		if (canglong_level >= MAX_ZHUANZHI_CANGLONG_LEVEL && m_role->GetLevel() >= UP_PROF6_ROLE_LEVEL_LIMIT)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_ONE_ZHUAN:
	{
		if (m_role->GetProfLevel() >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_PUT_ON_ZHUANZHI_EQUIP:
	{
		int put_on_count = m_role->GetZhuanzhiEquip()->GetSatisfyPutonCount(1, 1);
		if (put_on_count >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_JINGJIE_LEVEL:
	{
		int jingjie_level = m_role->GetJingJieLevel();
		if (jingjie_level >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_PHASE_FB_LEVEL:
	{
		int pass_level = m_role->GetRoleFBPhase()->GetPassLevel(param1);
		if (pass_level >= param2)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_PUT_ON_EQUIP:
	{
		if (m_role->GetEquipmentManager()->GetEquipCount(param2) >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_TOWER_DEFEND_FB_ENTER_COUNT:
	{
		int enter_personal_fb_times = m_role->GetFBTowerDefend()->GetPersonalFbEnterTimes();
		if (enter_personal_fb_times >= param1)
		{
			return true;
		}
	}
	break;

	case TASK_SATISFY_STATUS_TYPE_KILL_BOSS_TYPE:
	{
		int kill_boss_count = m_role->GetKillBossNum(param1);
		if (kill_boss_count >= param2)
		{
			return true;
		}
	}
	break;

	}

	return false;

}
void NewTaskManager::AdjustDoOperatorTimes(int task_id)
{
	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task_cfg || TASK_CON_DO_OPERATOR != task_cfg->condition)
	{
		return;
	}
	
	int adjust_times = 0;

	for (int i = 0; i < adjust_times; ++ i)
	{
		this->OnDoOperator(task_cfg->c_param1);
	}
}

void NewTaskManager::CheckTaskTypeStatus(int task_id)
{
	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task_cfg || TASK_CON_COMMIT_TASK != task_cfg->condition)
	{
		return;
	}

	int adjust_times = 0, example_task_id = INVALID_TASK_ID; 
	{
		switch(task_cfg->c_param1) // 任务类型
		{
		case TASK_TYPE_GUILD: 
			{
				if (m_role->GetGuildTask()->AllTaskComplete())
				{
					adjust_times = task_cfg->c_param2;
					example_task_id = TASKPOOL->GetGuildExampleTaskID();
				}
			}
			break;

		case TASK_TYPE_DAILY: 
			{
				if (m_role->GetDailyTask()->AllTaskComplete())
				{
					adjust_times = task_cfg->c_param2;
					example_task_id = TASKPOOL->GetDailyExampleTaskID();
				}
			}
			break;
		}
	}

	for (int i = 0; i < adjust_times; ++ i)
	{
		this->OnCommitOneTask(example_task_id);
	}
}


void NewTaskManager::CheckEnterSceneStatus(int task_id)
{
	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task_cfg || TASK_CON_ENTER_SCENE_TYPE != task_cfg->condition)
	{
		return;
	}

	int adjust_times = 0; 
	{
		switch(task_cfg->c_param1) 
		{
		case Scene::SCENE_TYPE_EXPFB: 
			{
				if (m_role->GetEnergy() < 30)
				{
					adjust_times = task_cfg->c_param2;
				}
			}
			break;
		}
	}

	for (int i = 0; i < adjust_times; ++ i)
	{
		this->OnEnterSceneType(task_cfg->c_param1);
	}
}

void NewTaskManager::AddBianshenTaskEffect()
{
	this->RemoveBianshenTaskEffetc();

	EffectMoveSpeed *effect_speed = new EffectMoveSpeed(m_role->GetId(), 1, 1440000, EBT_CHIHUAN, 1, EffectBase::PRODUCT_METHOD_BIANSHEN_TASK);
	effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
	int value = static_cast<int>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED) * 0.5f);
	effect_speed->SetSave(true);
	effect_speed->SetMoveSpeed(-value, -50);
	m_role->AddEffect(effect_speed);

	gamelog::g_log_task.printf(LL_INFO, "%s AddMoveBuff", m_role->GetName());
}

void NewTaskManager::RemoveBianshenTaskEffetc()
{
	bool ok = m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_CHIHUAN, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_BIANSHEN_TASK);
	gamelog::g_log_task.printf(LL_INFO, "%s RemoveMoveBuff  %s", m_role->GetName(), ok ? "true":"false");
}

void NewTaskManager::AddFlyTaskEffect()
{
	this->RemoveFlyTaskEffetc();

	EffectMoveSpeed *effect_speed = new EffectMoveSpeed(m_role->GetId(), 1, 1440000, EBT_JIASU, 1, EffectBase::PRODUCT_METHOD_BIANSHEN_TASK);
	effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
	int value = static_cast<int>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED) * 0.25f);
	effect_speed->SetSave(true);
	effect_speed->SetMoveSpeed(value, 25);
	m_role->AddEffect(effect_speed);

	gamelog::g_log_task.printf(LL_INFO, "%s AddFlyTaskEffect", m_role->GetName());
}

void NewTaskManager::RemoveFlyTaskEffetc()
{
	bool ok = m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_JIASU, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_BIANSHEN_TASK);
	gamelog::g_log_task.printf(LL_INFO, "%s AddFlyTaskEffect  %s", m_role->GetName(), ok ? "true" : "false");
}

void NewTaskManager::AddDownSpeedTaskEffect()
{
	this->RemoveDownSpeedTaskEffetc();

	EffectMoveSpeed *effect_speed = new EffectMoveSpeed(m_role->GetId(), EffectBase::PRODUCT_ID_DAILY_TASK_ACCEPT, 1440000, EBT_CHIHUAN, 1, EffectBase::PRODUCT_METHOD_BIANSHEN_TASK);
	effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
	int value = static_cast<int>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED) * 0.5f);
	effect_speed->SetSave(true);
	effect_speed->SetMoveSpeed(-value, -50);
	m_role->AddEffect(effect_speed);

	gamelog::g_log_task.printf(LL_INFO, "%s AddMoveBuff", m_role->GetName());
}

void NewTaskManager::RemoveDownSpeedTaskEffetc()
{
	bool ok = m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_CHIHUAN, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_BIANSHEN_TASK, EffectBase::PRODUCT_ID_DAILY_TASK_ACCEPT);
	gamelog::g_log_task.printf(LL_INFO, "%s RemoveMoveBuff  %s", m_role->GetName(), ok ? "true" : "false");
}

void NewTaskManager::taskCommitRandomReward(int task_type, int count, int is_one_key, int param1, long long param2)
{
	int before_time = -1; // 之前的记录
	switch (task_type)
	{
	case TASK_TYPE_DAILY:
		before_time = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_COMMIT_DAILY_TASK_COUNT);
		break;
	case TASK_TYPE_GUILD:
		before_time = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_GUILD_TASK_COMPLETE_COUNT);
		break;
	case TASK_TYPE_PAOHUAN:
		before_time = m_role->GetPaoHuanTask()->getDayCompleteCount();
		break;
	default:
		break;
	}
	if (before_time < 0)
	{
		return;
	}

	int finish_time = before_time + count;	// 现在即将完成的次数
	
	Protocol::SCTaskRollReward reward_p;
	reward_p.task_type = task_type;
	reward_p.is_one_key_finish = is_one_key;
	reward_p.param_1 = param1;
	reward_p.param_2 = param2;
	bool has_cfg = false;

	int iter_count = 0;
	before_time += 1;
	for (;(iter_count < Protocol::MAX_TASK_RANDOM_NUM) && (before_time <= finish_time); before_time++)
	{
		if ((before_time % TASKPOOL->GetTaskRollFinishTime() == 0) && (before_time != 0))
		{
			PerRandomReward* random_reward_cfg = TASKPOOL->GetRandomReward(task_type);
			if (random_reward_cfg != NULL)
			{
				has_cfg = true;
				// 给物品
				m_role->GetKnapsack()->PutOrMail(random_reward_cfg->item_data, PUT_REASON_LUCKYROLL);

				Protocol::SCTaskRollReward::RewardItem item;
				item.task_index = random_reward_cfg->index;
				item.task_type = random_reward_cfg->task_type;

				reward_p.reward_item_list[iter_count++] = item;
			}
		}
	}

	reward_p.count = iter_count;
	bool is_bopen = DropLogManager::Instance().IsBopenBegin();
	if (has_cfg && !is_bopen)
	{
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&reward_p, sizeof(reward_p));
	}
}

void NewTaskManager::CheckOwnItemStatus(int task_id)
{
	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task_cfg || TASK_CON_COMMIT_ITEM != task_cfg->condition)
	{
		return;
	}

	int item_count = m_role->GetKnapsack()->GetItemNum(task_cfg->c_param1);
	if (item_count >= 0)
	{
		m_role->GetTaskManager()->OnGetItem(task_cfg->c_param1, item_count);
	}
}

void NewTaskManager::GetTaskDropIdList(std::set<UInt16>* drip_id_list)
{
	for (int i = 0; i < m_task_param.task_count; ++i)
	{
		const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(m_task_param.taskinfo_list[i].task_id);
		if (NULL == task_cfg) continue;

		if ((TASK_CON_COMMIT_ITEM == task_cfg->condition)
			|| (TASK_CON_SATISFY_STATUS == task_cfg->condition && TASK_SATISFY_STATUS_TYPE_LIGHT_UP_TIANMING == task_cfg->c_param1)
			|| (TASK_CON_SATISFY_STATUS == task_cfg->condition && TASK_SATISFY_STATUS_TYPE_LIGHT_UP_CANGLONG == task_cfg->c_param1)
			|| (TASK_CON_SATISFY_STATUS == task_cfg->condition && TASK_SATISFY_STATUS_TYPE_LIGHT_UP_SHENGXIAO == task_cfg->c_param1)
			|| (TASK_CON_SATISFY_STATUS == task_cfg->condition && TASK_SATISFY_STATUS_TYPE_LIGHT_UP_YUANQI == task_cfg->c_param1)
			|| (TASK_CON_SATISFY_STATUS == task_cfg->condition && TASK_SATISFY_STATUS_TYPE_LIGHT_UP_SHENQI == task_cfg->c_param1)
			|| (TASK_CON_SATISFY_STATUS == task_cfg->condition && TASK_SATISFY_STATUS_TYPE_LIGHT_UP_XINGXIU == task_cfg->c_param1))
		{
			UInt16 drop_id = task_cfg->c_param3;
			drip_id_list->insert(drop_id);
		}
	}
}