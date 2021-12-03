#include "specialshuijing.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"

#include "scene/activityshadow/activityshadow.hpp"

#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"

#include "engineadapter.h"

#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "config/activityconfig/shuijingconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "globalconfig/globalconfig.h"

#include "monster/monsterpool.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "scene/scenemanager.h"
#include "scene/generater/gathergenerater.h"

#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include "other/shop/roleshop.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "other/event/eventhandler.hpp"

SpecialShuiJing::SpecialShuiJing(Scene *scene)
	: SpecialLogic(scene), m_is_init(0), m_big_shuijing_num(0), m_next_notice_gather_buff_info_time(0), m_big_shuijing_notify(true)
{
	m_big_shuijing_next_flush_time.clear();
}

SpecialShuiJing::~SpecialShuiJing()
{
}

void SpecialShuiJing::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	std::vector<int>::iterator iter = m_big_shuijing_next_flush_time.begin();
	for(; m_big_shuijing_next_flush_time.end() != iter;)
	{
		int reflush_notice_time = LOGIC_CONFIG->GetShuiJingCfg().GetOtherCfg().reflush_notice_time;
		if (0 < reflush_notice_time && now_second >= *iter - reflush_notice_time && !m_big_shuijing_notify)
		{
			int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_big_shuijing_refresh, 
				reflush_notice_time);
			if (str_len > 0) 
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
			}
			m_big_shuijing_notify = true;
			++iter;
		}
		else if(now_second >= *iter)
		{
			iter = m_big_shuijing_next_flush_time.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	if (!m_is_init)
	{
		// 初始化
		m_is_init = true;

		if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_SHUIJING))
		{
			int gather_num = (int)m_scene->GatherObjNum();
			for (int i = 0; i < gather_num; i++)
			{
				GatherObj *gather = m_scene->GetGatherObjByIndex(i);
				if (NULL != gather && LOGIC_CONFIG->GetShuiJingCfg().IsBestShuiJing(gather->GatherId()))
				{
					if (NULL != gather->GetGatherGenerater()) gather->GetGatherGenerater()->Gather(NULL);
					m_scene->DeleteObj(gather->GetId());
				}
			}

			this->GatherPauseBigShuiJing();
		}
	}

	if(static_cast<unsigned int>(now_second) > m_next_notice_gather_buff_info_time)
	{
		m_next_notice_gather_buff_info_time = static_cast<unsigned int>(now_second) + 1;

		int total_role_num = m_scene->RoleNum();
		for (int role_num = 0; role_num < total_role_num; role_num++)
		{
			Role *temp_role = m_scene->GetRoleByIndex(role_num);
			if (NULL != temp_role)
			{
				JoinerItem *target_joiner = this->GetJoinerItem(temp_role->GetUID());
				if (NULL != target_joiner)
				{
					if (!target_joiner->is_brocast_gather_buff && temp_role->GetRoleActivity()->GetParam()->shuijing_gather_buff_time < static_cast<unsigned int>(now_second))
					{
						target_joiner->is_brocast_gather_buff = true;
						this->NotifyGatherBuffInfo(temp_role, 0);
					}
				}
			}
		}
	}
}

void SpecialShuiJing::OnAddObj(Obj *obj)
{
	if (NULL == obj) return;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role *)obj;
		this->SendPlayerInfoToPlayer(role);
		EventHandler::Instance().OnEnterShuijing(role);
	}
	else if (Obj::OBJ_TYPE_GATHER == obj->GetObjType())
	{
		GatherObj *gather = (GatherObj *)obj;
		if (LOGIC_CONFIG->GetShuiJingCfg().IsBestShuiJing(gather->GatherId()))
		{
			this->UpdateBigGatherNum();

			if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_SHUIJING))
			{
				int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_big_shuijing_already_refresh);
				if (str_len > 0) 
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
				}
			}
		}
	}
}

void SpecialShuiJing::OnRemoveObj(Obj *obj)
{
	if (nullptr != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
	{
		GatherObj *gather = (GatherObj *)obj;
		if (LOGIC_CONFIG->GetShuiJingCfg().IsBestShuiJing(gather->GatherId()))
		{
			this->UpdateBigGatherNum();
			GatherGenerater *gather_generater = gather->GetGatherGenerater();
			if (NULL != gather_generater)
			{
				if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_SHUIJING))
				{
					gather_generater->SetPause(true);
				}
				else
				{
					int next_flush_time = gather_generater->GetGatherNextFlushTime();
					if (0 < next_flush_time)
					{
						m_big_shuijing_next_flush_time.push_back(next_flush_time);
						m_big_shuijing_notify = false;
					}
				}
			}
		}

		this->SendPlayerInfoToPlayer();
	}
}

bool SpecialShuiJing::SpecialCanGather(Role *role, GatherObj *gather)
{
	if (NULL == role || NULL == gather) return false;

	if (!LOGIC_CONFIG->GetShuiJingCfg().IsBestShuiJing(gather->GatherId()) && 
		role->GetRoleActivity()->GetParam()->shuijing_gather_count >= LOGIC_CONFIG->GetShuiJingCfg().GetOtherCfg().gather_max_times)
	{
		role->NoticeNum(errornum::EN_SHUIJING_GATHER_TIMES_LIMIT);
		return false;
	}

	return true;
}

void SpecialShuiJing::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (NULL == role) return;

	const ShuiJingGatherCfg *gather_cfg = LOGIC_CONFIG->GetShuiJingCfg().GetGatherCfgByGatherId(gather_id);
	if (NULL == gather_cfg) return;

	int reward_bind_gold = gather_cfg->bind_gold;
	int reward_mojing = gather_cfg->mojing;
	int reward_shengwang = gather_cfg->shengwang;

	RoleActivityParam *activity_param = role->GetRoleActivity()->GetParam();

	activity_param->shuijing_day_bind_gold += reward_bind_gold;
	role->GetKnapsack()->GetMoney()->AddGoldBind(reward_bind_gold, "Shuijing");

	activity_param->shuijing_day_mojing += reward_mojing;
	role->GetRoleShop()->AddChestShopMojing(reward_mojing, "Shuijing");

	activity_param->shuijing_day_shengwang += reward_shengwang;
	role->GetRoleShop()->AddChestShopShengwang(reward_shengwang, "Shuijing");

	// 至尊水晶不算入采集次数
	if (LOGIC_CONFIG->GetShuiJingCfg().IsBestShuiJing(gather_id))
	{
		int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_big_shuijing_gather, 
			role->GetUID(), role->GetName(), (int)role->GetCamp());
		if (str_len > 0) 
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
		}
	}
	else
	{
		activity_param->shuijing_gather_count++;
	}

	if (gather_cfg->is_broadcast)
	{
		int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_gather_type_shuijing, 
			role->GetUID(), role->GetName(), (int)role->GetCamp());
		if (len > 0) 
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, len, SYS_MSG_ACTIVE_NOTICE);
		}
	}

	this->SendPlayerInfoToPlayer(role);
	this->CalcShuijingGatherTimes(role ,gather_id);

	EventHandler::Instance().OnShuiJingGather(role);
}

bool SpecialShuiJing::SpecialRelive(Role *role, int realive_type)
{
	if (NULL == role) return true;

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		// 免费复活，有倒计时限制
		const ShuiJingOtherCfg &other_cfg = LOGIC_CONFIG->GetShuiJingCfg().GetOtherCfg();
		if (role->GetDeadTime() < other_cfg.relive_time * 1000 - 1000)
		{
			return true;
		}

		role->ReAlive(SPECIAL_ACTIVITY_FREE_REALIVE_HP_PER, 100, other_cfg.realive_pos);
	}
	else
	{
		// 消耗次数复活
		if (role->GetRoleActivity()->GetParam()->shuijing_free_relive_count < LOGIC_CONFIG->GetShuiJingCfg().GetOtherCfg().free_relive_count)
		{
			if (!role->ReAlive(100, 100))
			{
				return true;
			}

			role->GetRoleActivity()->GetParam()->shuijing_free_relive_count++;
			this->SendPlayerInfoToPlayer(role);
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool SpecialShuiJing::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (NULL == role || NULL == obj)
	{
		return false;
	}

	if (role->GetId() == obj->GetId())
	{
		return false;
	}

	return true;
}

bool SpecialShuiJing::SpecialStopGahterOnAttack(Role *role)
{
	if (role->GetRoleActivity()->GetParam()->shuijing_gather_buff_time >= (unsigned int)(EngineAdapter::Instance().Time()))
	{
		return false;
	}

	return true;
}

void SpecialShuiJing::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		Role *dead_role = (Role *)dead;

		JoinerItem *dead_user = this->GetJoinerItem(dead_role->GetUID());
		if (NULL != dead_user)
		{
			Obj *killer_obj = m_scene->GetObj(killer_id);
			if (NULL != killer_obj)
			{
				if (Obj::OBJ_TYPE_ROLE == killer_obj->GetObjType())
				{
					Role *killer_role = (Role *)killer_obj;
					if (NULL != killer_role)
					{
						JoinerItem *killer_user = this->GetJoinerItem(killer_role->GetUID());
						if (NULL != killer_user)
						{
							for (std::map<UserID, time_t>::iterator it = dead_user->hurt_map.begin(),
								end = dead_user->hurt_map.end(); it != end; ++it)
							{
								if (it->second + ASSIST_LIMIT_TIME >= EngineAdapter::Instance().Time() && it->first != killer_role->GetUserId())
								{
									JoinerItem *joiner_assist = this->GetJoinerItem(UidToInt(it->first));
									if (NULL != joiner_assist)
									{
										joiner_assist->assist++;
										joiner_assist->lianzhan++;
										Role *assist_role = m_scene->GetRoleByUID(joiner_assist->uid);
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
			}

			dead_user->lianzhan = 0;
			dead_user->hurt_map.clear();
		}
	}
}

void SpecialShuiJing::OnHurtOther(Character *cha, Character *target)
{
	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		Role *hurt_role = (Role *)cha;
		Role *target_role = (Role *)target;

		if (NULL == hurt_role || NULL == target_role)
		{
			return;
		}

		JoinerItem *target_joiner = this->GetJoinerItem(target_role->GetUID());
		if (NULL != target_joiner)
		{
			target_joiner->hurt_map[hurt_role->GetUserId()] = EngineAdapter::Instance().Time();
		}
	}
}

void SpecialShuiJing::OnBuyGatherBuff(Role *role)
{
	if (NULL == role) return;

	const ShuiJingOtherCfg &other_cfg = LOGIC_CONFIG->GetShuiJingCfg().GetOtherCfg();
	if (!role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.gather_buff_gold, "ShuiJingBuyBuff"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	role->GetRoleActivity()->GetParam()->shuijing_gather_buff_time = (std::max)(role->GetRoleActivity()->GetParam()->shuijing_gather_buff_time + other_cfg.gather_buff_time,
		(unsigned int)(EngineAdapter::Instance().Time()) + other_cfg.gather_buff_time);

	JoinerItem *buy_joiner = this->GetJoinerItem(role->GetUID());
	if (NULL != buy_joiner)
	{
		buy_joiner->is_brocast_gather_buff = false;
	}

	this->SendPlayerInfoToPlayer(role);
	this->NotifyGatherBuffInfo(role, 1);
}

void SpecialShuiJing::OnActivityStatusChange(int status)
{
	if (ACTIVITY_STATUS_OPEN == status)	
	{
		this->OnActivityOpen();
	}
	else if (ACTIVITY_STATUS_CLOSE == status)
	{
		this->OnActivityClose();
	}
}

void SpecialShuiJing::SendPlayerInfoToPlayer(Role *role)
{

	unsigned int big_shuijing_next_flush_time = m_big_shuijing_next_flush_time.empty() ? 0 : *m_big_shuijing_next_flush_time.begin();

	if (nullptr != role)
	{
		static Protocol::SCShuijingPlayerInfo spi;

		RoleActivityParam *activity_param = role->GetRoleActivity()->GetParam();
		spi.total_bind_gold = activity_param->shuijing_day_bind_gold;
		spi.total_mojing = activity_param->shuijing_day_mojing;
		spi.total_shengwang = activity_param->shuijing_day_shengwang;
		spi.free_relive_times = activity_param->shuijing_free_relive_count;
		spi.cur_gather_times = activity_param->shuijing_gather_count;
		spi.gather_buff_time = activity_param->shuijing_gather_buff_time;
		spi.big_shuijing_num = m_big_shuijing_num;
		spi.big_shuijing_next_flush_time = big_shuijing_next_flush_time;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&spi, sizeof(spi));
	}
	else
	{
		static Protocol::SCShuijingSceneInfo scene_info;

		scene_info.big_shuijing_num = m_big_shuijing_num;
		scene_info.big_shuijing_next_flush_time = big_shuijing_next_flush_time;

		m_scene->SendToRole((const char*)&scene_info, sizeof(scene_info));
	}
}

SpecialShuiJing::JoinerItem* SpecialShuiJing::GetJoinerItem(int uid)
{
	std::map<int, JoinerItem *>::iterator iter = m_joiner_map.find(uid);
	if (m_joiner_map.end() == iter)
	{
		return NULL;
	}

	return iter->second;
}

void SpecialShuiJing::OnRoleEnterScene(Role *role)
{
	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
	if (NULL == joiner_item)
	{
		joiner_item = new JoinerItem();
		joiner_item->uid = role->GetUID();
		m_joiner_map[joiner_item->uid] = joiner_item;
		role->GetRoleActivity()->ResetShuijingData();
	}

	this->SendJoinerTaskInfo(role);
}

void SpecialShuiJing::OnRoleLeaveScene(Role *role, bool is_logout)
{

}

void SpecialShuiJing::FetchShuijingTaskReward(Role *role ,int task_id)
{
	if (NULL == role) return;

	if (task_id < 0 || task_id >= MAX_SHUIJING_TASK_COUNT) return;

	JoinerItem *joiner = this->GetJoinerItem(role->GetUID());
	if (NULL == joiner) return;

	if (joiner->IsFetchTaskReward(task_id))
	{
		return;
	}

	const ShuiJingTaskCfg *task_cfg = LOGIC_CONFIG->GetShuiJingCfg().GetTaskCfgByTaskID(task_id);
	if (NULL == task_cfg) return;


	switch(task_cfg->shuijing)
	{
	case SHUIJING_TOTAL:
		{
			if (task_cfg->need_gather_count < joiner->gather_shuijing_total_num)
			{
				return;
			}
		}
		break;
	case SHUIJING_BIG:
		{
			if (task_cfg->need_gather_count < joiner->gather_big_shuijing_total_num)
			{
				return;
			}
		}
		break;

	case SHUIJING_BIG_DIAMOND:
		{
			if (task_cfg->need_gather_count < joiner->gather_diamond_big_shuijing_num)
			{
				return;
			}
		}
		break;

	case SHUIJING_BEST:
		{
			if (task_cfg->need_gather_count < joiner->gather_best_shuijing_count)
			{
				return;
			}
		}
		break;

	default:
		{
			return;
		}
	}

	//int exp_add_per = LOGIC_CONFIG->GetShuiJingCfg().GetTaskExpPerByLevle(role->GetLevel());

	//long long add_exp = task_cfg->reward_exp;

	//if (exp_add_per > 0)
	//{
	//	add_exp = static_cast<long long>(exp_add_per * 0.01f * add_exp);
	//}

	//策划需求，修改增加经验规则（2018-1-3）
	long long add_exp = static_cast<long long>(task_cfg->exp_factor_task * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));

	role->AddExp(add_exp, "ShuijingTask", Role::EXP_ADD_REASON_DEFAULT);
	
	joiner->SetFetchTaskRewardFlag(task_id);

	this->SendJoinerTaskInfo(role);

	if (0 != task_cfg->is_notice)
	{
		int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_finish_shuijing_task,
			role->GetUID(), role->GetName(), (int)role->GetCamp());
		if (len > 0)
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, len, SYS_MSG_ACTIVE_NOTICE);
		}
	}

	EventHandler::Instance().OnShuijingFinishTask(role);
}

void SpecialShuiJing::SendJoinerTaskInfo(Role *role)
{
	if(NULL == role) return;
	
	JoinerItem *joiner = this->GetJoinerItem(role->GetUID());
	if (NULL == joiner) return;

	Protocol::SCShuijingTaskInfo cmd;
	cmd.gather_shuijing_total_num = joiner->gather_shuijing_total_num;
	cmd.gather_big_shuijing_total_num = joiner->gather_big_shuijing_total_num;
	cmd.gather_diamond_big_shuijing_num = joiner->gather_diamond_big_shuijing_num;
	cmd.gather_best_shuijing_count = joiner->gather_best_shuijing_count;
	cmd.fetch_task_reward_flag = joiner->fetch_task_reward_flag;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void SpecialShuiJing::CalcShuijingGatherTimes(Role *role, int gather_id)
{
	if (NULL == role) return;

	JoinerItem *joiner = this->GetJoinerItem(role->GetUID());
	if (NULL == joiner) return;

	const ShuiJingGatherCfg *gather_cfg = LOGIC_CONFIG->GetShuiJingCfg().GetGatherCfgByGatherId(gather_id);
	if (NULL == gather_cfg) return;

	joiner->gather_shuijing_total_num++;

	if (SHUIJING_TYPE_BIG == gather_cfg->gather_type)
	{
		joiner->gather_big_shuijing_total_num++;
	}

	if(SHUIJING_TYPE_BIG == gather_cfg->gather_type && SHUIJING_SUB_TYPE_DIAMOND == gather_cfg->gather_sub_type)
	{
		joiner->gather_diamond_big_shuijing_num++;
	}

	if(SHUIJING_TYPE_BEST == gather_cfg->gather_type)
	{
		joiner->gather_best_shuijing_count++;
	}

	this->SendJoinerTaskInfo(role);
}

int SpecialShuiJing::GetSpecialParam(Role *role)
{
	if (NULL == role) return 0;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	return role->GetRoleActivity()->GetParam()->shuijing_gather_buff_time > now_sec;
}

void SpecialShuiJing::NotifyGatherBuffInfo(Role *role, short is_has_buff)
{
	if (NULL == role) return;

	Protocol::SCNoticeGatherBuffInfo cmd;
	cmd.obj_id = role->GetId();
	cmd.is_gather_wudi = is_has_buff;

	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
}

void SpecialShuiJing::OnActivityOpen()
{
	SceneMonsterLogic::GatherGeneraterList &generater_list = m_scene->GetMonsterLogic()->GetGatherGeneraterList();
	for (SceneMonsterLogic::GatherGeneraterList::iterator it = generater_list.begin(); it != generater_list.end(); it++)
	{
		if (LOGIC_CONFIG->GetShuiJingCfg().IsBestShuiJing((*it)->GetGatherId()))
		{
			(*it)->SetPause(false);
			(*it)->RefreshNow();
		}
	}
	m_big_shuijing_next_flush_time.clear();
}

void SpecialShuiJing::OnActivityClose()
{
	this->GatherPauseBigShuiJing();
	m_big_shuijing_next_flush_time.clear();
	this->DelayKickOutAllRole();

	std::map<int, JoinerItem *>::iterator iter;
	for (iter = m_joiner_map.begin(); m_joiner_map.end() != iter; ++ iter)
	{
		delete iter->second;
	}
	m_joiner_map.clear();
}

Role *SpecialShuiJing::GetRole(ObjID obj_id)
{
	Obj *obj = m_scene->GetObj(obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return  (Role *)obj;
	}

	return NULL;
}

void SpecialShuiJing::UpdateBigGatherNum()
{
	int old_num = m_big_shuijing_num;

	m_big_shuijing_num = 0;
	int gather_num = m_scene->GatherObjNum();
	for (int i = 0; i < gather_num; i++)
	{
		GatherObj *gather_obj = m_scene->GetGatherObjByIndex(i);
		if (NULL != gather_obj && LOGIC_CONFIG->GetShuiJingCfg().IsBestShuiJing(gather_obj->GatherId()))
		{
			m_big_shuijing_num++;
		}
	}

	if (old_num != m_big_shuijing_num)
	{
		int role_num = m_scene->RoleNum();
		for (int i = 0; i < role_num; i++)
		{
			Role *role = m_scene->GetRoleByIndex(i);
			if (NULL != role)
			{
				this->SendPlayerInfoToPlayer(role);
			}
		}
	}
}

void SpecialShuiJing::GatherPauseBigShuiJing()
{
	SceneMonsterLogic::GatherGeneraterList &generater_list = m_scene->GetMonsterLogic()->GetGatherGeneraterList();
	for (SceneMonsterLogic::GatherGeneraterList::iterator it = generater_list.begin(); it != generater_list.end(); it++)
	{
		if (LOGIC_CONFIG->GetShuiJingCfg().IsBestShuiJing((*it)->GetGatherId()))
		{
			(*it)->SetPause(true);
		}
	}
}

void SpecialShuiJing::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
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

void SpecialShuiJing::CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist)
{
	return;
	if (NULL != kill_role && NULL != dead_role)
	{
		JoinerItem *killer = this->GetJoinerItem(kill_role->GetUID());
		JoinerItem *dead = this->GetJoinerItem(dead_role->GetUID());
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

