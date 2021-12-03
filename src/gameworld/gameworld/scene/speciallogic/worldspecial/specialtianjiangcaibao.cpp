#include "specialtianjiangcaibao.h"

#include "obj/character/role.h"
#include "obj/character/monster.h"

#include "scene/activityshadow/activityshadow.hpp"

#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"

#include "engineadapter.h"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "gameworld/config/activityconfig/tianjiangcaibaoconfig.h"
#include "config/logicconfigmanager.hpp"
#include "globalconfig/globalconfig.h"

#include "monster/monsterpool.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "scene/scenemanager.h"

#include "other/event/eventhandler.hpp"
#include "gameworld/world.h"
#include <algorithm>
#include "item/knapsack.h"
#include "other/itemlevelreward/itemlevelreward.hpp"

SpecialTianJiangCaiBao::SpecialTianJiangCaiBao(Scene *scene)
: SpecialLogic(scene), m_is_finish(true), m_is_qianduoduo_notice(false), m_reflush_qianduoduo_time(0), m_cur_qianduoduo_num(0), 
m_is_bigqianduoduo_notice(false), m_reflush_bigqiandoudou_time(0), m_cur_bigqianduoduo_num(0)
{
}

SpecialTianJiangCaiBao::~SpecialTianJiangCaiBao()
{
}

void SpecialTianJiangCaiBao::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_finish) return;

	this->CheckBornQianduoduo(now_second);
	this->CheckBornBigQianduoduo(now_second);
}

void SpecialTianJiangCaiBao::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (NULL == cha || INVALID_OBJ_ID == enemy_id) return;

	Obj *attacker = m_scene->GetObj(enemy_id);
	if (NULL == attacker || Obj::OBJ_TYPE_ROLE != attacker->GetObjType()) return;

	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Monster *monster = (Monster*)cha;

		const TianJiangCaiBaoBigQianCfg &big_qian_cfg = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTianJiangCaiBaoBigQianCfg();
		const TianJiangCaiBaoQianduoduoCfg &qianduoduo_cfg = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTianJiangCaiBaoQianduoduoCfg();

		if (big_qian_cfg.bigqian_id != monster->GetMonsterId() && qianduoduo_cfg.qianduoduo_id != monster->GetMonsterId()) return;

		Role *attacker_role = (Role*)attacker;

		TianJiangCaiBaoUser *tjcb_user = this->GetTianJiangCaiBaoUser(attacker_role->GetUserId());
		if (NULL == tjcb_user) return;

		int get_bind_gold = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetAttackRewardGold();
		tjcb_user->get_total_gold += get_bind_gold;

		if (attacker_role->GetKnapsack()->GetMoney()->AddGoldBind(get_bind_gold, "tianjiangcaibao"))
		{
			this->SendTianJiangCaiBaoUserInfo(attacker_role);
		}
	}
}

bool SpecialTianJiangCaiBao::SpecialIsEnemy(Role *role, Obj *obj)
{
	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_MONSTER:
		{
			Monster *monster = (Monster*)obj;
			return monster->IsAlive();
		}
		break;
	}

	return false;
}

bool SpecialTianJiangCaiBao::SpecialIsFriend(Role *role, Obj *obj, bool check_alive /*= true*/)
{
	switch (obj->GetObjType())
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			Role *friend_role = (Role *)obj;
			return (!check_alive || friend_role->IsAlive());
		}
		break;
	}

	return false;
}

void SpecialTianJiangCaiBao::OnCharacterDie(Character *dead_cha, ObjID killer_objid)
{
	if (NULL == dead_cha || INVALID_OBJ_ID == killer_objid) return;

	if (Obj::OBJ_TYPE_MONSTER == dead_cha->GetObjType())
	{
		Role *kill_role = m_scene->GetPrincipal(killer_objid);
		if (NULL == kill_role) return;

		const TianJiangCaiBaoBigQianCfg & big_qian_cfg = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTianJiangCaiBaoBigQianCfg();
		const TianJiangCaiBaoQianduoduoCfg &qianduoduo_cfg = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTianJiangCaiBaoQianduoduoCfg();

		Monster *dead_monster = (Monster*)dead_cha;
		if (dead_monster->GetMonsterId() == big_qian_cfg.bigqian_id)
		{
			this->OnKillBigQianduoduo(kill_role);
		}
		else if (dead_monster->GetMonsterId() == qianduoduo_cfg.qianduoduo_id)
		{
			this->OnKillQianduoduo(kill_role);
		}

		this->CheckTaskProgress(kill_role, TIAN_JIANG_CAI_BAO_TASK_TYPE_MONSTER, dead_monster->GetMonsterId());
	}

	if (Obj::OBJ_TYPE_ROLE == dead_cha->GetObjType())
	{
		Role *dead_role = (Role *)dead_cha;

		TianJiangCaiBaoUser *dead_user = this->GetTianJiangCaiBaoUser(dead_role->GetUserId());
		if (NULL != dead_user)
		{
			Obj *killer_obj = m_scene->GetObj(killer_objid);
			if (Obj::OBJ_TYPE_ROLE == killer_obj->GetObjType())
			{
				Role *killer_role = (Role *)killer_obj;
				if (NULL != killer_role)
				{
					TianJiangCaiBaoUser *killer_user = this->GetTianJiangCaiBaoUser(killer_role->GetUserId());
					if (NULL != killer_user)
					{
						for (std::map<UserID, time_t>::iterator it = dead_user->hurt_map.begin(), 
							end = dead_user->hurt_map.end(); it != end; ++ it)
						{
							if (it->second + ASSIST_LIMIT_TIME >= EngineAdapter::Instance().Time() && it->first != killer_role->GetUserId())
							{
								TianJiangCaiBaoUser *joiner_assist = this->GetTianJiangCaiBaoUser(it->first);
								if (NULL != joiner_assist)
								{
									joiner_assist->assist++;
									joiner_assist->lianzhan++;
									Role *assist_role = m_scene->GetRoleByUID(UidToInt(joiner_assist->user_id));
									if (NULL != assist_role)
									{
										this->CheckLianZhanNotice(assist_role, dead_role, true);
									}
								}
							}
						}

						killer_user->lianzhan++;

						this->CheckLianZhanNotice(killer_role, dead_role);
					}
				}
			}

			dead_user->lianzhan = 0;
			dead_user->hurt_map.clear();
		}
	}
}

void SpecialTianJiangCaiBao::OnHurtOther(Character *cha, Character *target)
{
	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		Role *hurt_role = (Role *)cha;
		Role *target_role = (Role *)target;

		if (NULL == hurt_role || NULL == target_role)
		{
			return;
		}

		TianJiangCaiBaoUser *target_joiner = this->GetTianJiangCaiBaoUser(target_role->GetUserId());
		if (NULL != target_joiner)
		{
			target_joiner->hurt_map[hurt_role->GetUserId()] = EngineAdapter::Instance().Time();
		}
	}
}

void SpecialTianJiangCaiBao::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (NULL == role) return;

	role->NoticeNum(noticenum::NT_TIANJIANGCAIBAO_GATHER_SUCC);

	this->CheckTaskProgress(role, TIAN_JIANG_CAI_BAO_TASK_TYPE_GATHER, gather_id);
}

void SpecialTianJiangCaiBao::OnRoleEnterScene(Role *role)
{
	if (m_tianjiangcaibao_usermap.find(role->GetUserId()) == m_tianjiangcaibao_usermap.end())
	{
		ROLE_LOG_QUICK6(LOG_TYPE_ACTIVITY_TIANJIANGBAO, role, role->GetLevel(), 0, "Join", NULL);
	}

	TianJiangCaiBaoUser *tjcb_user = this->GetTianJiangCaiBaoUser(role->GetUserId());
	if (NULL == tjcb_user) return;

	this->SendTianJiangCaiBaoUserInfo(role);
	EventHandler::Instance().OnEnterTianJiangCaiBao(role);
}

void SpecialTianJiangCaiBao::OnActivityStatusChange(int status)
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

TianJiangCaiBaoUser * SpecialTianJiangCaiBao::GetTianJiangCaiBaoUser(const UserID &user_id)
{
	TianJiangCaiBaoUserMapIt it = m_tianjiangcaibao_usermap.find(user_id);
	if (it != m_tianjiangcaibao_usermap.end())
	{
		return &it->second;
	}

	TianJiangCaiBaoUser &tjcb_user = m_tianjiangcaibao_usermap[user_id];
	tjcb_user.user_id = user_id;

	std::map<int, TianJiangCaiBaoTaskInfo> &task_cfg_map = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTianJiangCaiBaoTaskCfgMap();
	tjcb_user.task_list.reserve(task_cfg_map.size());

	for (std::map<int, TianJiangCaiBaoTaskInfo>::iterator it = task_cfg_map.begin(); it != task_cfg_map.end(); it++)
	{
		tjcb_user.task_list.push_back(it->second.task_id);
	}
  
// 	int task_size = (int)tjcb_user.task_list.size();
// 	for (int i = 0; i < task_size; i++)
// 	{
// 		int rand_index = RandomNum(task_size);
// 		int temp_task_id = tjcb_user.task_list[i];
// 		tjcb_user.task_list[i] = tjcb_user.task_list[rand_index];
// 		tjcb_user.task_list[rand_index] = temp_task_id;
// 	}

	if (!tjcb_user.task_list.empty())
	{
		tjcb_user.curr_task_id = tjcb_user.task_list[0];
	}

	return &tjcb_user;
}

void SpecialTianJiangCaiBao::CheckBornQianduoduo(time_t now_second)
{
	if (now_second < m_reflush_qianduoduo_time - TIANJIANGCAIBAO_REFLUSH_NOTICE_TIME || 0 == m_reflush_qianduoduo_time)
	{
		return;
	}

	if (!m_is_qianduoduo_notice)
	{
		m_is_qianduoduo_notice = true;

		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_tianjiangcaibao_reflush_qianduoduo_notice, TIANJIANGCAIBAO_REFLUSH_NOTICE_TIME);
		if (length > 0)
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
		}
	}
	else if (now_second >= m_reflush_qianduoduo_time)
	{
		const TianJiangCaiBaoQianduoduoCfg &qianduoduo_cfg = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTianJiangCaiBaoQianduoduoCfg();

		m_is_qianduoduo_notice = false;
		m_reflush_qianduoduo_time = now_second + qianduoduo_cfg.reflush_time_m * 60;
		if (m_reflush_qianduoduo_time > (time_t)(ActivityShadow::Instance().GetActivityEndTime(ACTIVITY_TYPE_TIANJIANGCAIBAO) - qianduoduo_cfg.last_reflush_time_m * 60))
		{
			m_reflush_qianduoduo_time = 0;
		}

		for (int i = 0; i < 1000 && m_cur_qianduoduo_num < qianduoduo_cfg.count; ++ i)
		{
			const Posi *born_pos = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetRandomFlushPos();
			if (NULL != born_pos && NULL != MONSTERPOOL->CreateMonster(static_cast<UInt16>(qianduoduo_cfg.qianduoduo_id), m_scene, *born_pos))
			{
				++ m_cur_qianduoduo_num;
			}
		}

		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_tianjiangcaibao_already_reflush_qianduoduo_notice);
		if (length > 0)
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
		}

		this->SendTianJiangCaiBaoInfoToAllUser();
	}
}

void SpecialTianJiangCaiBao::CheckBornBigQianduoduo(time_t now_second)
{
	if (now_second < m_reflush_bigqiandoudou_time - TIANJIANGCAIBAO_REFLUSH_NOTICE_TIME || 0 == m_reflush_bigqiandoudou_time)
	{
		return;
	}

	if (!m_is_bigqianduoduo_notice)
	{
		m_is_bigqianduoduo_notice = true;

		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_tianjiangcaibao_reflush_bigqian_notice, TIANJIANGCAIBAO_REFLUSH_NOTICE_TIME);
		if (length > 0)
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
		}
	}
	else if (now_second >= m_reflush_bigqiandoudou_time)
	{
		m_is_bigqianduoduo_notice = false;
		m_reflush_bigqiandoudou_time = 0;

		const TianJiangCaiBaoBigQianCfg &bigqian_cfg = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTianJiangCaiBaoBigQianCfg();
		MONSTERPOOL->CreateMonster(bigqian_cfg.bigqian_id, m_scene, bigqian_cfg.born_pos);
		++m_cur_bigqianduoduo_num;

		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_tianjiangcaibao_already_reflush_bigqian_notice);
		if (length > 0)
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
		}

		this->SendTianJiangCaiBaoInfoToAllUser();
	}
}

void SpecialTianJiangCaiBao::OnKillQianduoduo(Role *role)
{
	--m_cur_qianduoduo_num;

	if (NULL == role) return;

	const TianJiangCaiBaoQianduoduoCfg &qianduoduo_cfg = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTianJiangCaiBaoQianduoduoCfg();

	TianJiangCaiBaoUser *tjcb_user = this->GetTianJiangCaiBaoUser(role->GetUserId());
	if (NULL == tjcb_user) return;

	int get_bind_gold = qianduoduo_cfg.kill_min_gold;
	if (qianduoduo_cfg.kill_max_gold > qianduoduo_cfg.kill_min_gold)
	{
		get_bind_gold += RandomNum(qianduoduo_cfg.kill_max_gold - qianduoduo_cfg.kill_min_gold + 1);
	}

	tjcb_user->get_total_gold += get_bind_gold;

	{
		//传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_tianjiangcaibao_kill_goblin, role->GetUID(),
			role->GetName(), (int)role->GetCamp());
		if (length > 0) 
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
		}
	}

	role->GetKnapsack()->GetMoney()->AddGoldBind(get_bind_gold , "tianjiangcaibao");

	this->SendTianJiangCaiBaoInfoToAllUser();
}

void SpecialTianJiangCaiBao::OnKillBigQianduoduo(Role *role)
{
	--m_cur_bigqianduoduo_num;

	if (NULL == role) return;

	const TianJiangCaiBaoBigQianCfg &big_qian_cfg = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTianJiangCaiBaoBigQianCfg();

	TianJiangCaiBaoUser *tianjiangcaibao_user = this->GetTianJiangCaiBaoUser(role->GetUserId());
	if (NULL == tianjiangcaibao_user) return;

	tianjiangcaibao_user->get_total_gold += big_qian_cfg.kill_gold;

	role->GetKnapsack()->GetMoney()->AddGoldBind(big_qian_cfg.kill_gold, "TianJiangCaiBao");

	//传闻
	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_tianjiangcaibao_kill_boss_moster_notice, role->GetUID(),
		role->GetName(), (int)role->GetCamp());
	if (length > 0) 
	{
		this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
	}

	this->SendTianJiangCaiBaoInfoToAllUser();
}

void SpecialTianJiangCaiBao::CheckTaskProgress(Role *role, int task_type, int param_id)
{
	TianJiangCaiBaoUser *tjcb_user = this->GetTianJiangCaiBaoUser(role->GetUserId());
	if (NULL == tjcb_user) return;

	if (tjcb_user->curr_task_id < 0) return;

	const TianJiangCaiBaoTaskInfo *task_cfg = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTianJiangCaiBaoTaskCfg(tjcb_user->curr_task_id);
	if (NULL == task_cfg || task_cfg->task_type != task_type || task_cfg->param_id != param_id) return;

	tjcb_user->curr_task_param++;
	if (tjcb_user->curr_task_param >= task_cfg->param_count)
	{
		tjcb_user->task_finish_num++;

		//const int task_exp_reward = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTaskRewardExp(role->GetLevel());

		//策划需求，修改增加经验规则（2018-1-3）
		long long task_exp_reward = static_cast<long long>(task_cfg->exp_factor_task * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));

		if (task_exp_reward > 0)
		{
			role->AddExp(task_exp_reward, "TianJiangCaiBao", Role::EXP_ADD_REASON_DEFAULT);
		}
	
		role->GetKnapsack()->PutListOrMail(task_cfg->reward_list, PUT_REASON_TIANJIANGCAIBAO_REWARD);
	
		for(int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			if (task_cfg->reward_list[i].item_id > 0)
			{
				tjcb_user->role_get_item_map[task_cfg->reward_list[i].item_id] += task_cfg->reward_list[i].num;
			}
		}

		const TianJiangCaiBaoRewardCfg * reward_cfg = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTaskReward();
		if (NULL != reward_cfg)
		{
			for (int i = 0; i < MAX_REWARD_CFG_COUNT; ++ i)
			{
				if (tjcb_user->task_finish_num >= reward_cfg[i].complete_task_num && 0 == tjcb_user->is_fetch_reward[i])
				{
					tjcb_user->is_fetch_reward[i] = 1;

					role->GetKnapsack()->PutOrMail(reward_cfg[i].item1, PUT_REASON_TIANJIANGCAIBAO_REWARD);
					tjcb_user->role_get_item_map[reward_cfg[i].item1.item_id] += reward_cfg[i].item1.num;

					role->GetKnapsack()->PutOrMail(reward_cfg[i].item2, PUT_REASON_TIANJIANGCAIBAO_REWARD);
					tjcb_user->role_get_item_map[reward_cfg[i].item2.item_id] += reward_cfg[i].item2.num;

					role->GetKnapsack()->PutOrMail(reward_cfg[i].item3, PUT_REASON_TIANJIANGCAIBAO_REWARD);
					tjcb_user->role_get_item_map[reward_cfg[i].item3.item_id] += reward_cfg[i].item3.num;

// 					role->GetKnapsack()->PutOrMail(reward_cfg->item4, PUT_REASON_TIANJIANGCAIBAO_REWARD);
// 					tjcb_user->role_get_item_map[reward_cfg->item4.item_id] += reward_cfg->item4.num;

					break;
				}
			}
		}

		role->NoticeNum(noticenum::NT_TIANJIANGCAIBAO_GET_REWARD_ITEM);

		{
			//传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_tianjiangcaibao_finish_task, role->GetUID(),
				role->GetName(), (int)role->GetCamp(), tjcb_user->curr_task_id);
			if (length > 0) 
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
			}
		}

		tjcb_user->curr_task_param = 0;
		if (tjcb_user->task_finish_num < (int)tjcb_user->task_list.size())
		{
			tjcb_user->curr_task_id = tjcb_user->task_list[tjcb_user->task_finish_num];
		}
		else
		{
			tjcb_user->curr_task_id = -1;
		}

		EventHandler::Instance().OnTianjiangcaibaoFinishTask(role);
	}

	this->SendTianJiangCaiBaoUserInfo(role);
}

void SpecialTianJiangCaiBao::SendTianJiangCaiBaoInfoToAllUser()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			this->SendTianJiangCaiBaoUserInfo(role);
		}
	}
}

void SpecialTianJiangCaiBao::SendTianJiangCaiBaoUserInfo(Role *role)
{
	if (NULL == role) return;

	TianJiangCaiBaoUser *tjcb_user = this->GetTianJiangCaiBaoUser(role->GetUserId());
	if (NULL == tjcb_user) return;

	static Protocol::SCTianJiangCaiBaoUserInfo cmd;

	cmd.bigqianduoduo_reflush_time = (unsigned int)m_reflush_bigqiandoudou_time;
	cmd.qianduoduo_reflush_time = (unsigned int)m_reflush_qianduoduo_time;
	cmd.cur_qianduoduo_num = m_cur_qianduoduo_num;
	cmd.cur_bigqianduoduo_num = m_cur_bigqianduoduo_num;
	cmd.get_total_gold = tjcb_user->get_total_gold;
	cmd.is_finish = m_is_finish ? 1 : 0;
	cmd.curr_task_id = tjcb_user->curr_task_id;
	cmd.curr_task_param = tjcb_user->curr_task_param;
	cmd.task_finish_num = tjcb_user->task_finish_num;
	
	cmd.reward_count = 0;
	for(std::map<int, int>::iterator it = tjcb_user->role_get_item_map.begin(), end = tjcb_user->role_get_item_map.end(); it != end; ++it)
	{
		cmd.reward_list[cmd.reward_count].item_id = it->first;
		cmd.reward_list[cmd.reward_count].num = it->second;
		cmd.reward_count++;

		if (cmd.reward_count >= TIANJIANGCAIBAO_MAX_REWARD_ITEM_COUNT) break;
	}

	int length = sizeof(cmd) - (TIANJIANGCAIBAO_MAX_REWARD_ITEM_COUNT - cmd.reward_count) * sizeof(cmd.reward_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, length);
}

void SpecialTianJiangCaiBao::OnActivityStart()
{
	m_is_finish = false;

	m_tianjiangcaibao_usermap.clear();

	m_is_qianduoduo_notice = true;
	m_reflush_qianduoduo_time = EngineAdapter::Instance().Time();

	const TianJiangCaiBaoBigQianCfg &big_qian_cfg = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTianJiangCaiBaoBigQianCfg();
	m_is_bigqianduoduo_notice = false;
	m_reflush_bigqiandoudou_time = EngineAdapter::Instance().Time() + big_qian_cfg.reflush_time_m * 60;
}

void SpecialTianJiangCaiBao::OnActivityClose()
{
	m_is_finish = true;

	m_cur_qianduoduo_num = 0;
	m_reflush_qianduoduo_time = 0;

	m_cur_bigqianduoduo_num = 0;
	m_reflush_bigqiandoudou_time = 0;

	this->KillAllMonster();

	// 自动传出
	this->SendTianJiangCaiBaoInfoToAllUser();
	this->DelayKickOutAllRole();
}

void SpecialTianJiangCaiBao::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
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

void SpecialTianJiangCaiBao::CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist)
{
	return;
	if (NULL != kill_role && NULL != dead_role)
	{
		TianJiangCaiBaoUser *killer = this->GetTianJiangCaiBaoUser(kill_role->GetUserId());
		TianJiangCaiBaoUser *dead = this->GetTianJiangCaiBaoUser(dead_role->GetUserId());
		if (NULL != killer && NULL != dead)
		{
			if (!is_assist && dead->lianzhan >= 3)
			{
				int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_stoplianzhan_notice,
					UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), 
					UidToInt(dead_role->GetUserId()), dead_role->GetName(), (int)dead_role->GetCamp(), dead->lianzhan);
				if (str_len > 0) 
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
				}
			}

			if (killer->lianzhan > 0)
			{
				int str_len = 0;
				int notice_type = SYS_MSG_ACTIVE_NOTICE;

				if (0 == killer->lianzhan % 3)
				{
					str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), 
						killer->lianzhan, killer->lianzhan);
				}

				if (str_len > 0) this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, notice_type);
			}
		}
	}
}
