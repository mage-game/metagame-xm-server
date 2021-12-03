#include "specialzhuxie.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"

#include "scene/activityshadow/activityshadow.hpp"

#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"

#include "engineadapter.h"

#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "config/activityconfig/zhuxieconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "globalconfig/globalconfig.h"

#include "monster/monsterpool.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "scene/scenemanager.h"
#include "obj/otherobj/gatherobj.h"

#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include "friend/teaminfo.hpp"

SpecialZhuXie::SpecialZhuXie(Scene *scene)
	: SpecialLogic(scene), m_world_level(0), m_cur_monster_num(0), m_cur_gather_num(0), m_last_check_reward_time(0)
{
	for (int i = 0; i < ZHUXIE_BOSS_NUM; ++ i)
	{
		m_zhuxie_boss_info_list[i].Reset();
	}
}

SpecialZhuXie::~SpecialZhuXie()
{

}

void SpecialZhuXie::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_ZHUXIE)) return;

	this->CheckBornBoss(now_second);
	this->CheckBornMonster(now_second);
	this->CheckBornGather(now_second);
}

void SpecialZhuXie::CheckBornBoss(time_t now_second)
{
	for (int i = 0; i < ZHUXIE_BOSS_NUM; ++ i)
	{
		if ((0 == m_zhuxie_boss_info_list[i].next_boss_flush_time || now_second >= m_zhuxie_boss_info_list[i].next_boss_flush_time) && 
			INVALID_OBJ_ID == m_zhuxie_boss_info_list[i].boss_objid)
		{
			Posi born_pos;
			const ZhuXieReplaceBossCfg *replace_boss_cfg = LOGIC_CONFIG->GetZhuXieConfig().GetReplaceBossCfg(i, m_world_level);

			if (NULL != replace_boss_cfg && LOGIC_CONFIG->GetZhuXieConfig().GetRandomFlushPos(born_pos, replace_boss_cfg->pos_id_start, replace_boss_cfg->pos_id_len))
			{
				Monster *monster = MONSTERPOOL->CreateMonster(replace_boss_cfg->replace_boss_id, m_scene, born_pos);
				if (NULL != monster)
				{
					m_zhuxie_boss_info_list[i].next_boss_flush_time = 0;
					m_zhuxie_boss_info_list[i].boss_objid = monster->GetId();
				}
			}
		}
	}
}

void SpecialZhuXie::CheckBornMonster(time_t now_second)
{
	if (m_cur_monster_num >= ZHUXIE_MONSTER_BORN_LESS_NUM)
	{
		return;
	}

	for (int i = 0; i < ZHUXIE_MONSTER_NUM; ++ i)
	{
		Posi born_pos;
		const ZhuXieReplaceMonsterCfg *replace_monster_cfg = LOGIC_CONFIG->GetZhuXieConfig().GetReplaceMonsterCfg(i, m_world_level);
		
		if (NULL != replace_monster_cfg)
		{
			for (int k = 0; k < replace_monster_cfg->count; ++ k)
			{
				if (!LOGIC_CONFIG->GetZhuXieConfig().GetRandomFlushPos(born_pos, replace_monster_cfg->pos_id_start, replace_monster_cfg->pos_id_len))
				{
					continue;
				}

				Monster *monster = MONSTERPOOL->CreateMonster(replace_monster_cfg->replace_monster_id, m_scene, born_pos);
				if (NULL != monster)
				{
					++ m_cur_monster_num;
				}
			}
		}
	}
}

void SpecialZhuXie::CheckBornGather(time_t now_second)
{
	if (m_cur_gather_num >= ZHUXIE_GATHER_BORN_LESS_NUM)
	{
		return;
	}

	for (int i = 0; i < ZHUXIE_GATHER_NUM; ++ i)
	{
		const ZhuXieReplaceGatherCfg *replace_gather_cfg = LOGIC_CONFIG->GetZhuXieConfig().GetReplaceGatherCfg(i, m_world_level);
		Posi born_pos;
		if (NULL != replace_gather_cfg)
		{
			for (int k = 0; k < replace_gather_cfg->count; ++ k)
			{
				if (!LOGIC_CONFIG->GetZhuXieConfig().GetRandomFlushPos(born_pos, replace_gather_cfg->pos_id_start, replace_gather_cfg->pos_id_len))
				{
					continue;
				}

				GatherObj *gather_obj = new GatherObj();
				gather_obj->Init(NULL, replace_gather_cfg->replace_gather_id, replace_gather_cfg->gather_time * 1000, 0, true);
				gather_obj->SetPos(born_pos);
				m_scene->AddObj(gather_obj);
				m_gather_objid_vec.push_back(gather_obj->GetId());
				++ m_cur_gather_num;
			}
		}
	}
}

void SpecialZhuXie::OnCharacterDie(Character *dead_cha, ObjID killer_objid)
{
	if (NULL == dead_cha || INVALID_OBJ_ID == killer_objid) return;

	if (Obj::OBJ_TYPE_ROLE == dead_cha->GetObjType())
	{
		Role *dead_role = (Role*)dead_cha;
		Role *kill_role = m_scene->GetPrincipal(killer_objid);
		this->OnKillRole(kill_role, dead_role);
	}
	else if (Obj::OBJ_TYPE_MONSTER == dead_cha->GetObjType())
	{
		Monster *dead_monster = (Monster*)dead_cha;
		Role *kill_role = m_scene->GetPrincipal(killer_objid);

		if (NULL == kill_role || NULL == dead_monster)
		{
			return;
		}
		
		if (dead_monster->IsBossMonster())
		{
			this->OnKillBoss(kill_role, dead_monster);
		}
		else
		{
			this->OnKillMonster(kill_role, dead_monster);
		}
	}
}

void SpecialZhuXie::OnKillRole(Role *role, Role *dead_role)
{
	if (NULL == role || NULL == dead_role)
	{
		return;
	}

	SpecialZhuXieUser *zhuxie_user = ActivityShadow::Instance().GetZhuXieUser(role->GetUserId(), m_world_level);
	
	if (NULL == zhuxie_user)
	{
		return;
	}
	
	int can_add_max_shengwang = LOGIC_CONFIG->GetZhuXieConfig().GetKillRoleGetMaxShengWang() - zhuxie_user->kill_role_get_total_shengwang;
	if (can_add_max_shengwang >= LOGIC_CONFIG->GetZhuXieConfig().GetKillRoleGetShengWang())
	{
		can_add_max_shengwang = LOGIC_CONFIG->GetZhuXieConfig().GetKillRoleGetShengWang();
	}

	role->AddShengwang(can_add_max_shengwang, "KillRoleInZhuXie");
	zhuxie_user->kill_role_get_total_shengwang += can_add_max_shengwang;
	
	this->CheckUserTaskInfo(role, ZHUXIE_TASK_TYPE_KILL_ROLE, 0);
}

void SpecialZhuXie::OnKillBoss(Role *role, Monster *boss)
{
	if (NULL == role || NULL == boss)
	{
		return;
	}

	this->CheckUserTaskInfo(role, ZHUXIE_TASK_TYPE_KILL_BOSS, boss->GetMonsterId());

	for (int i = 0; i < ZHUXIE_BOSS_NUM; i++)
	{
		ZhuXieBossInfo &boss_info = m_zhuxie_boss_info_list[i];
		if (boss_info.boss_objid == boss->GetId())
		{
			const ZhuXieReplaceBossCfg *boss_cfg = LOGIC_CONFIG->GetZhuXieConfig().GetZhuXieReplacBossCfgById(boss->GetMonsterId());
			if (NULL == boss_cfg)
			{
				continue;
			}

			boss_info.boss_objid = INVALID_OBJ_ID;
			boss_info.next_boss_flush_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + boss_cfg->flush_time_on_die;
			this->SendZhuXieInfoToAllUser();

			break;
		}
	}
}

void SpecialZhuXie::OnKillMonster(Role *role, Monster *monster)
{
	if (NULL == role || NULL == monster)
	{
		return;
	}

	//获得奖励
	{
		const ZhuXieReward *reward_cfg = LOGIC_CONFIG->GetZhuXieConfig().GetKillMonsterReward(monster->GetMonsterId());
		if (NULL != reward_cfg)
		{
			role->AddShengwang(reward_cfg->shengwang, "ZhuXieKillMonsterGet");
			role->GetKnapsack()->PutOrMail(reward_cfg->item, PUT_REASON_ZHUXIE_KILL_MONSTER);
		}
	}

	this->CheckUserTaskInfo(role, ZHUXIE_TASK_TYPE_KILL_MONSTER, monster->GetMonsterId());
	
	-- m_cur_monster_num;
	if (m_cur_monster_num < 0)
	{
		m_cur_monster_num = 0;
	}
}

void SpecialZhuXie::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (NULL == role) return;

	//获得奖励
	{
		const ZhuXieReward *reward_cfg = LOGIC_CONFIG->GetZhuXieConfig().GetGatherReward(gather_id);
		if (NULL != reward_cfg)
		{
			role->AddShengwang(reward_cfg->shengwang, "ZhuXieKillMonsterGet");
			role->GetKnapsack()->PutOrMail(reward_cfg->item, PUT_REASON_ZHUXIE_GATHER);
		}
	}

	std::vector<int>::iterator iter;
	for(iter = m_gather_objid_vec.begin(); iter != m_gather_objid_vec.end(); ++ iter)
	{
		if (*iter == obj_id)
		{
			m_gather_objid_vec.erase(iter);
			break;
		}
	}

	this->CheckUserTaskInfo(role, ZHUXIE_TASK_TYPE_GATHER, gather_id);

	-- m_cur_gather_num;
	if (m_cur_gather_num < 0)
	{
		m_cur_gather_num = 0;
	}
}

void SpecialZhuXie::OnRoleEnterScene(Role *role)
{
	SpecialZhuXieUser *zhuxie_user = ActivityShadow::Instance().GetZhuXieUser(role->GetUserId(), m_world_level);
	if (NULL == zhuxie_user) return;

	zhuxie_user->role_level = role->GetLevel();
	role->GetName(zhuxie_user->user_name);

	this->SendZhuXieUserInfo(role);
	EventHandler::Instance().OnEnterZhuXieZhanChang(role);
}

bool SpecialZhuXie::SpecialRelive(Role *role, int realive_type)
{
	if (NULL == role) return false;

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME != realive_type) 
	{
		return false;
	}

	Posi realive_pos(0, 0);
	LOGIC_CONFIG->GetZhuXieConfig().GetZhuXieFBEnterInfo(&realive_pos);

	if (realive_pos.x == 0 || realive_pos.y == 0)
	{
		return false;
	}

	role->ReAlive(50, 100, realive_pos);

	return true;
}

void SpecialZhuXie::OnActivityStatusChange(int status)
{
	switch (status)
	{
	case ACTIVITY_STATUS_OPEN:
		{
			this->OnActivityStart();
		}
		break;

	case ACTIVITY_STATUS_CLOSE:
		{
			this->OnActivityClose();
		}
		break;
	}
}

void SpecialZhuXie::OnFetchTaskReward(Role *role, int task_id)
{
	if (NULL == role) return;

	SpecialZhuXieUser *zhuxie_user = ActivityShadow::Instance().GetZhuXieUser(role->GetUserId(), m_world_level);
	if (NULL == zhuxie_user) return;

	for (std::vector<ZhuXieTaskInfo>::iterator it = zhuxie_user->zhuxietask_list.begin(), end = zhuxie_user->zhuxietask_list.end(); 
		it != end; ++it)
	{
		ZhuXieTaskInfo &taskinfo = *it;
		if (taskinfo.task_id == task_id && taskinfo.IsFinish() && !taskinfo.IsFetchReward())
		{
			const ZhuXieTaskInfo *task_cfg = LOGIC_CONFIG->GetZhuXieConfig().GetZhuXieTaskCfg(taskinfo.task_id);
			if (NULL == task_cfg)
			{
				role->NoticeNum(errornum::EN_FETCH_ZHUXIE_TASK_REWARD_FAIL);
				return;
			}
			
			role->GetKnapsack()->PutOrMail(task_cfg->reward_item1, PUT_REASON_ZHUXIE_ACTIVITY_REWARD);
			role->GetKnapsack()->PutOrMail(task_cfg->reward_item2, PUT_REASON_ZHUXIE_ACTIVITY_REWARD);
			role->GetKnapsack()->PutOrMail(task_cfg->reward_item3, PUT_REASON_ZHUXIE_ACTIVITY_REWARD);
			taskinfo.is_fetch_reward = 1;
			this->SendZhuXieUserInfo(role); 

			return;
		}
	}
}

void SpecialZhuXie::OnActivityStart()
{
	m_world_level = WorldShadow::Instance().GetWorldLevel();
}

void SpecialZhuXie::OnActivityClose()
{
	m_cur_gather_num = 0;
	m_cur_monster_num = 0;

	// 移除boss和怪物
	this->KillAllMonster();
	for (int i = 0; i < ZHUXIE_BOSS_NUM; ++ i)
	{
		m_zhuxie_boss_info_list[i].Reset();
	}

	// 移除gather
	int gather_size = static_cast<int>(m_gather_objid_vec.size());
	for (int  i = 0; i < gather_size; ++ i)
	{
		m_scene->DeleteObj(m_gather_objid_vec[i]);
	}
	m_gather_objid_vec.clear();
	
	this->DelayKickOutAllRole();
}

void SpecialZhuXie::CheckUserTaskInfo(Role *role, short task_type, int param_id)
{
	if (NULL == role) return;

	Role *member_list[16] = {NULL};
	int team_rolenum = role->GetTeamInfo()->GetOtherMemberInTheSameScene(member_list, sizeof(member_list) / sizeof(member_list[0]));

	if (team_rolenum < static_cast<int>(sizeof(member_list) / sizeof(member_list[0]))) member_list[team_rolenum ++] = role;

	for (int i = 0; i < team_rolenum; i++)
	{
		if (NULL == member_list[i]) continue;

		SpecialZhuXieUser *zhuxie_user = ActivityShadow::Instance().GetZhuXieUser(member_list[i]->GetUserId(), m_world_level);
		if (NULL != zhuxie_user)
		{
			for (std::vector<ZhuXieTaskInfo>::iterator it = zhuxie_user->zhuxietask_list.begin(), end = zhuxie_user->zhuxietask_list.end();
				it != end; ++ it)
			{
				ZhuXieTaskInfo &taskinfo = *it;
				if (!taskinfo.IsFinish() && taskinfo.IsMatch(task_type, param_id))
				{
					++ taskinfo.param_value;
					this->SendZhuXieUserInfo(member_list[i]);
				}
			}
		}
	}
}

void SpecialZhuXie::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
{
	if (NULL != str_buff && str_len > 0) 
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, notice_type))
		{
			m_scene->SendToRole((const char*)&sm, sendlen);
		}
	}
}

void SpecialZhuXie::SendZhuXieInfoToAllUser()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			this->SendZhuXieUserInfo(role);
		}
	}
}

void SpecialZhuXie::SendZhuXieUserInfo(Role *role)
{
	if (NULL == role) return;

	SpecialZhuXieUser *zhuxie_user = ActivityShadow::Instance().GetZhuXieUser(role->GetUserId(), m_world_level);
	if (NULL == zhuxie_user) return;

	static Protocol::SCZhuXieUserInfo cmd;

	for (int i = 0; i < ZHUXIE_BOSS_NUM; ++ i)
	{
		cmd.boss_next_flush_time[i] = m_zhuxie_boss_info_list[i].next_boss_flush_time; 
	}

	int task_count = 0;
	for (std::vector<ZhuXieTaskInfo>::iterator it = zhuxie_user->zhuxietask_list.begin(), end = zhuxie_user->zhuxietask_list.end();
		it != end; ++ it)
	{
		if (task_count >= 0 && task_count < ZHUXIE_TASK_MAX)
		{
			const ZhuXieTaskInfo *task_cfg = LOGIC_CONFIG->GetZhuXieConfig().GetZhuXieTaskCfg(it->task_id);
			if (NULL == task_cfg)
			{
				continue;
			}

			cmd.taskinfo_list[task_count].is_fetch_reward = it->is_fetch_reward;
			cmd.taskinfo_list[task_count].task_id = it->task_id;
			cmd.taskinfo_list[task_count].param_value = it->param_value;
			cmd.taskinfo_list[task_count].max_value = it->max_value;

			++task_count;
		}
		else
		{
			break;
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

bool SpecialZhuXie::SpecialCanMountOn(Role *role)
{
	return false;
}

