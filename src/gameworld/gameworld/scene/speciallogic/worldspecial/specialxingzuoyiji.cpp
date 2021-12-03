#include "specialxingzuoyiji.hpp"
#include "obj/character/role.h"
#include "obj/otherobj/gatherobj.h"

#include "scene/scenemanager.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/worldshadow/worldshadow.hpp"

#include "engineadapter.h"
#include "internalcomm.h"

#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "protocal/msgactivity.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/xingzuoyijiconfig.hpp"
#include "monster/monsterpool.h"
#include "scene/generater/gathergenerater.h"
#include "world.h"
#include "item/knapsack.h"
#include "other/roleactivity/roleactivity.hpp"
#include "servercommon/activedegreedef.hpp"
#include "other/activedegree/activedegree.hpp"

SpecialXingzuoYiji::SpecialXingzuoYiji(Scene *scene) : SpecialLogic(scene), m_is_finish(true), m_next_boss_refresh_time(0), m_is_refresh_gather(true),
	m_temp_next_refresh_gather_time(0), m_boss_dead_count(0), m_now_gather_num(0), m_now_boss_num(0), m_last_notice_time(0), m_notice_times(0), m_is_notice_end(false),
	m_day_id(0), m_next_refresh_gather_time(0), m_last_broadcast_gather_time(0)
{
	this->CalNextRefreshGatherTime();
}

SpecialXingzuoYiji::~SpecialXingzuoYiji()
{
}

void SpecialXingzuoYiji::OnActivityStatusChange(int status)
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

void SpecialXingzuoYiji::SendtoRoleNotInScene(Role *role)
{
	const XingzuoYijiOtherCfg * other_cfg = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetOtherCfg();
	if (NULL != other_cfg)
	{
		static Protocol::SCXingzuoYijiChangeBoxAndBoss box_and_boss_info;
		box_and_boss_info.now_boss_num = static_cast<short>(m_now_boss_num);
		box_and_boss_info.now_box_num = static_cast<short>(m_now_gather_num);
		box_and_boss_info.next_box_refresh_time = m_next_refresh_gather_time;
		box_and_boss_info.next_boss_refresh_time = m_next_boss_refresh_time;
		box_and_boss_info.can_gather_num = 0;
		UNSTD_STATIC_CHECK(XingzuoYiJiConfig::MAX_GRADE_BOX == XINGZUOYIJI_BOX_TYPE_MAX);
		for (int i = 0; i < XingzuoYiJiConfig::MAX_GRADE_BOX; ++i)
		{
			box_and_boss_info.gather_box_num[i] = role->GetRoleActivity()->GetParam()->xingzuoyiji_gather_box_num[i];
		}

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&box_and_boss_info, sizeof(box_and_boss_info));
	}
}

void SpecialXingzuoYiji::SkipGather(Role *role, int quality)
{
	const XingzuoYijiSkipCfg *cfg = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetSkipCfg(quality);
	if (NULL == cfg)
	{
		return;
	}

	const XingzuoYijiOtherCfg *other_cfg = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetOtherCfg();
	if (NULL == other_cfg)
	{
		return;
	}

	if (role->GetLevel() < cfg->limit_level)
	{
		return;
	}

	int times = other_cfg->common_box_gather_limit - role->GetRoleActivity()->GetParam()->xingzuoyiji_gather_box_num[XingzuoYiJiConfig::SENIOR_BOX]
		- role->GetRoleActivity()->GetParam()->xingzuoyiji_gather_box_num[XingzuoYiJiConfig::MEDIUM_BOX];

	int consume = times * cfg->consume;
	if (consume <= 0)
	{
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseAllGold(consume, "Skip xingzuoyiji gather"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	for (int i = 0; i < times; ++i)
	{
		this->GetBoxItem(role, quality);
	}

	role->GetRoleActivity()->GetParam()->xingzuoyiji_gather_box_num[quality] += times;
	this->SendtoRoleNotInScene(role);
}

bool SpecialXingzuoYiji::CanDestroy()
{
	return false;
}

void SpecialXingzuoYiji::OnRoleEnterScene(Role *role)
{
	if (m_joiner_map.end() == m_joiner_map.find(role->GetUID()))
	{
		JoinerItem joiner;
		joiner.uid = role->GetUID();
		for (int i = 0; i < XingzuoYiJiConfig::MAX_GRADE_BOX; ++i)
		{
			joiner.gather_num_list[i] = role->GetRoleActivity()->GetParam()->xingzuoyiji_gather_box_num[i];
		}

		m_joiner_map[role->GetUID()] = joiner;
	}

	this->SyncRoleInfo(role);
}

void SpecialXingzuoYiji::Update(unsigned long interval, time_t now_second)
{
	static const unsigned int NOTICE_INTERVAL = 60;
	static const int MAX_NOTICE_TIME = 5;

	if (EngineAdapter::Instance().DayID() != m_day_id)
	{
		m_day_id = EngineAdapter::Instance().DayID();
		this->XingzuoYijiReset();
		m_joiner_map.clear();
		this->CalNextRefreshGatherTime();
	}

	if (m_notice_times > 0 && m_notice_times <= MAX_NOTICE_TIME)
	{
		if (static_cast<unsigned int>(now_second) >= m_last_notice_time + NOTICE_INTERVAL || 0 == m_last_notice_time )
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xingzuoyiji_refresh_notice, 280);
			if (length > 0)
			{
				//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				++m_notice_times;
				m_last_notice_time = static_cast<unsigned int>(now_second);
			}
		}
	}

	const XingzuoYijiOtherCfg *other_cfg = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetOtherCfg();
	if (NULL != other_cfg)
	{
		if (!m_is_finish)
		{
			// 刷新boss
			if (0 != m_next_boss_refresh_time && 0 == m_now_boss_num &&
				static_cast<unsigned int>(now_second) > m_next_boss_refresh_time)
			{
				this->RefreshBoss();
			}
		}
		else if (static_cast<unsigned int>(now_second) > m_next_refresh_gather_time && m_is_refresh_gather)
		{
			this->RefreshGather(COMMON_BOX);
		}
	}
}

void SpecialXingzuoYiji::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead) return;

	// 死亡者为BOSS
	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Role *killer = m_scene->GetPrincipal(killer_id);
		if (NULL != killer && Obj::OBJ_TYPE_ROLE == killer->GetObjType())
		{
			JoinerItem *joiner_killer = this->GetJoinerItem(killer->GetUID());
			if (NULL != joiner_killer)
			{
				this->GetBoxItem(killer, XingzuoYiJiConfig::MOLONG_BOX);
				++joiner_killer->gather_num_list[XingzuoYiJiConfig::MOLONG_BOX];
				killer->GetRoleActivity()->AddXingzuoYijiGatherBox(XingzuoYiJiConfig::MOLONG_BOX);
				killer->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_XINGZUOYIJI_GATHER);
			}
		}

		m_now_boss_num--;
		m_boss_dead_count++;

		if (MAX_GATHER_TIMES == m_boss_dead_count)
		{
			//传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xingzuoyiji_good_end_notice, 280);
			if (length > 0)
			{
				//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}

			m_is_notice_end = false;
			this->RefreshGather(GREAT_BOX);
			m_is_finish = true;
		}
		else
		{
			const XingzuoYijiOtherCfg *other_cfg = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetOtherCfg();
			if (NULL != other_cfg)
			{
				m_next_boss_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg->refresh_time;
			}

			this->RefreshGather(GREAT_BOX);
		}
	}
}

bool SpecialXingzuoYiji::SpecialCanGather(Role *role, GatherObj *gather)
{
	if (NULL == role || NULL == gather)
	{
		return false;
	}

	int box_type = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetBoxType(gather->GatherId());

	if (XingzuoYiJiConfig::SENIOR_BOX == box_type || XingzuoYiJiConfig::MEDIUM_BOX == box_type)
	{
		int gather_common_box_count = role->GetRoleActivity()->GetParam()->xingzuoyiji_gather_box_num[XingzuoYiJiConfig::SENIOR_BOX] +
			role->GetRoleActivity()->GetParam()->xingzuoyiji_gather_box_num[XingzuoYiJiConfig::MEDIUM_BOX];
		const XingzuoYijiOtherCfg *other_cfg = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetOtherCfg();
		if (NULL != other_cfg && gather_common_box_count >= other_cfg->common_box_gather_limit)
		{
			role->NoticeNum(errornum::EN_XINGZUOYIJI_GATHER_LIMIT);
			return false;
		}
	}

	return true;
}

void SpecialXingzuoYiji::OnStartGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (NULL == role || gather_id <= 0)
	{
		return;
	}

	int box_type = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetBoxType(gather_id);

	if (XingzuoYiJiConfig::SENIOR_BOX == box_type || XingzuoYiJiConfig::MEDIUM_BOX == box_type)
	{
		static const unsigned int GATHER_BROADCASTTIME = 30;
		if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) >= m_last_broadcast_gather_time + GATHER_BROADCASTTIME)
		{
			m_last_broadcast_gather_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xingzuoyiji_gather_common_box, 280,
				role->GetUID(), role->GetName(), role->GetCamp());
			if (length > 0)
			{
				//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
			}
		}
	}
	else if (XingzuoYiJiConfig::MOLONG_BOX == box_type)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xingzuoyiji_gather_molong_box, 280,
			role->GetUID(), role->GetName(), role->GetCamp());
		if (length > 0)
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
		}
	}
}

void SpecialXingzuoYiji::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (NULL == role || gather_id <= 0)
	{
		return;
	}

	--m_now_gather_num;

	int box_type = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetBoxType(gather_id);
	int gather_type = GetGatherType(box_type);

	if (box_type >= XingzuoYiJiConfig::SENIOR_BOX && box_type <= XingzuoYiJiConfig::MAX_GRADE_BOX)
	{
		JoinerItem *joiner = this->GetJoinerItem(role->GetUID());
		if (NULL != joiner)
		{
			const XingzuoYijiOtherCfg *other_cfg = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetOtherCfg();
			if (NULL != other_cfg)
			{
				switch(gather_type)
				{
				case COMMON_BOX:
					{
						Role *role = m_scene->GetRoleByUID(joiner->uid);
						if (NULL != role)
						{
							int gather_common_box_count = role->GetRoleActivity()->GetParam()->xingzuoyiji_gather_box_num[XingzuoYiJiConfig::SENIOR_BOX] +
									role->GetRoleActivity()->GetParam()->xingzuoyiji_gather_box_num[XingzuoYiJiConfig::MEDIUM_BOX];
							if (gather_common_box_count >= other_cfg->common_box_gather_limit)
							{
								return;
							}

							role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_XINGZUOYIJI_GATHER);
							role->GetRoleActivity()->AddXingzuoYijiGatherBox(box_type);
						}

						joiner->gather_num_list[box_type]++;
						joiner->once_gather_num_list[box_type]++;

						m_next_boss_refresh_time = 0;
						this->SyncInfoToAll();
					}
					break;

				case GREAT_BOX:
					{
						if (joiner->once_gather_num_list[XingzuoYiJiConfig::JUNIOR_BOX] + joiner->once_gather_num_list[XingzuoYiJiConfig::MOLONG_BOX] >= other_cfg->great_box_gather_limit)
						{
							return;
						}

						Role *role = m_scene->GetRoleByUID(joiner->uid);
						if (NULL != role)
						{
							role->GetRoleActivity()->AddXingzuoYijiGatherBox(box_type);
							role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_XINGZUOYIJI_GATHER);
						}

						joiner->gather_num_list[box_type]++;
						joiner->once_gather_num_list[box_type]++;
						if (m_is_finish)
						{
							m_next_refresh_gather_time = m_temp_next_refresh_gather_time;
							m_next_boss_refresh_time = 0;
							this->SyncInfoToAll();
						}
						else
						{
							this->SyncInfoToAll();
						}
					}
					break;

				default:
					return;
				}
			}
		}

		this->GetBoxItem(role, box_type);
	}

	if (m_now_gather_num <= 0 && !m_is_finish)
	{
		this->RefreshBoss();
	}
}

void SpecialXingzuoYiji::OnActivityStart()
{
	m_is_finish = false;
	m_is_refresh_gather = false;
	m_next_boss_refresh_time = 0;
	m_temp_next_refresh_gather_time = m_next_refresh_gather_time;
	m_next_refresh_gather_time = 0;
	m_boss_dead_count = 0;
	m_now_gather_num = 0;
	m_now_boss_num = 0;
	m_last_notice_time = 0;
	m_notice_times = 1;
	m_is_notice_end = true;

	this->XingzuoYijiReset();

	this->RefreshBoss();
}

void SpecialXingzuoYiji::OnActivityClose()
{
	m_is_finish = true;
	m_is_refresh_gather = true;
	m_next_boss_refresh_time = 0;
	m_boss_dead_count = 0;
	m_now_gather_num = 0;
	m_now_boss_num = 0;

	this->XingzuoYijiReset();

	this->CalNextRefreshGatherTime();

	if (m_is_notice_end)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xingzuoyiji_normal_end_notice, 280);
		if (length > 0)
		{
			//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}

		m_is_notice_end = false;
	}

	this->SyncInfoToAll();
}

void SpecialXingzuoYiji::XingzuoYijiReset()
{
	this->KillAllMonster();

	int gather_num = (int)m_scene->GatherObjNum();
	for (int i = 0; i < gather_num; i++)
	{
		GatherObj *gather = m_scene->GetGatherObjByIndex(i);
		if (NULL != gather)
		{
			if (NULL != gather->GetGatherGenerater()) gather->GetGatherGenerater()->Gather(NULL);
			m_scene->DeleteObj(gather->GetId());
		}
	}
	m_now_gather_num = 0;
}

SpecialXingzuoYiji::JoinerItem* SpecialXingzuoYiji::GetJoinerItem(int uid)
{
	return &m_joiner_map[uid];
}


void SpecialXingzuoYiji::SyncRoleInfo(Role *role)
{
	if (NULL == role) return;

	JoinerItem * joiner = this->GetJoinerItem(role->GetUID());
	if (NULL != joiner)
	{
		const XingzuoYijiOtherCfg * other_cfg = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetOtherCfg();
		if (NULL != other_cfg)
		{
			static Protocol::SCXingzuoYijiChangeBoxAndBoss box_and_boss_info;
			box_and_boss_info.now_boss_num = m_now_boss_num;
			box_and_boss_info.now_box_num = m_now_gather_num;
			box_and_boss_info.next_box_refresh_time = m_next_refresh_gather_time;
			box_and_boss_info.next_boss_refresh_time = m_next_boss_refresh_time;
			box_and_boss_info.can_gather_num = other_cfg->great_box_gather_limit - 
				joiner->once_gather_num_list[XingzuoYiJiConfig::JUNIOR_BOX] - joiner->once_gather_num_list[XingzuoYiJiConfig::MOLONG_BOX];

			memcpy(box_and_boss_info.gather_box_num, joiner->gather_num_list, sizeof(box_and_boss_info.gather_box_num));

			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&box_and_boss_info, sizeof(box_and_boss_info));
		}
	}
}

void SpecialXingzuoYiji::SyncInfoToAll()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		this->SyncRoleInfo(temp_role);
	}
}

void SpecialXingzuoYiji::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
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

void SpecialXingzuoYiji::RefreshBoss()
{
	if (m_is_finish)
	{
		return;
	}

	const XingzuoYijiOtherCfg *other_cfg = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetOtherCfg();
	if (NULL != other_cfg)
	{
		if (m_boss_dead_count == other_cfg->boss_max_refresh_times)
		{
			m_is_finish = true;
			return;
		}
	}	
	
	//刷新boss
	int world_level = WorldShadow::Instance().GetWorldLevel();

	const XingzuoYijiBossCfg *boss_cfg = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetBossCfg(world_level);
	if (NULL == boss_cfg)
	{
		return;
	}

	int monster_id = 0;

	switch(m_boss_dead_count)
	{
	case 0:
		monster_id = boss_cfg->monster_id;
		break;
	case 1:
		monster_id = boss_cfg->monster_id_second;
		break;
	case 2:
		monster_id = boss_cfg->monster_id_third;
		break;
	default:
		return;
	}

	this->XingzuoYijiReset();

	Monster *monster = MONSTERPOOL->CreateMonster(monster_id, m_scene, boss_cfg->boss_pos);
	if (NULL == monster)
	{
		return;
	}

	m_now_boss_num++;
	m_now_gather_num = 0;
	m_next_boss_refresh_time = 0;
	m_next_refresh_gather_time = 0;

	this->SyncInfoToAll();
}

void SpecialXingzuoYiji::RefreshGather(int gather_type)
{
	this->XingzuoYijiReset();

	const XingzuoYijiOtherCfg *other_cfg = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetOtherCfg();
	if (NULL == other_cfg)
	{
		return;
	}

	int count = 0;
	int i = XingzuoYiJiConfig::SENIOR_BOX;
	switch(gather_type)
	{
	case COMMON_BOX:
		{
			count = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetCommonBoxCfgCount();

			m_next_boss_refresh_time = 0;
			m_next_refresh_gather_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg->common_box_refresh_time;
		}
		break;

	case GREAT_BOX:
		{
			count = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetGreatBoxCfgCount();
			i = XingzuoYiJiConfig::JUNIOR_BOX;

			this->ResetJoinerOnceGather(GREAT_BOX);

			m_next_boss_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg->refresh_time;
			m_next_refresh_gather_time = 0;

			if (m_boss_dead_count == MAX_GATHER_TIMES)
			{
				m_next_refresh_gather_time = m_temp_next_refresh_gather_time;
				m_next_boss_refresh_time = 0;
			}

			//传闻
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xingzuoyiji_box_refresh_notice, 280);
				if (length > 0)
				{
					//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
		break;

	default:
		return;
	}

	count += i;

	std::vector<Posi>m_rand_pos = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetRandomPos();
	std::vector<Posi>::iterator it = m_rand_pos.begin();

	for (; i < count; i++)
	{
		const XingzuoYijiBoxTypeCfg* box_type_cfg = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetBoxTypeCfg(i);
		if (NULL != box_type_cfg)
		{
			for (int j = 0; j < box_type_cfg->gather_num; ++j)
			{
				if (it != m_rand_pos.end())
				{
					GatherObj *gather_obj = new GatherObj();
					gather_obj->SetPos(*it);
					gather_obj->Init(NULL, box_type_cfg->gather_id, box_type_cfg->gather_time_s, 0, true);
					m_scene->AddObj(gather_obj);
					++it;
					++m_now_gather_num;
				}
			}
		}
	}

	this->SyncInfoToAll();
}

void SpecialXingzuoYiji::GetBoxItem(Role* role, int box_type)
{
	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	int world_level = WorldShadow::Instance().GetWorldLevel();

	const ItemConfigData *item = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetBoxItem(world_level, box_type);
	if (NULL != item)
	{
		const ItemBase *itembase = ITEMPOOL->GetItem(item->item_id);
		if (NULL != itembase)
		{
			knapsack->PutOrMail(*item, PUT_REASON_XZYJ_HELP_BOX_REWARD);
		}
	}
}

int SpecialXingzuoYiji::GetGatherType(int box_type)
{
	if (box_type >= XingzuoYiJiConfig::SENIOR_BOX && box_type <= XingzuoYiJiConfig::MEDIUM_BOX)
	{
		return COMMON_BOX;
	}

	if (box_type >= XingzuoYiJiConfig::JUNIOR_BOX && box_type <= XingzuoYiJiConfig::MOLONG_BOX)
	{
		return GREAT_BOX;
	}

	return -1;
}

void SpecialXingzuoYiji::ResetJoinerOnceGather(int box_type)
{
	for(std::map<int, JoinerItem>::iterator it = m_joiner_map.begin(); it != m_joiner_map.end(); ++it)
	{
		switch (box_type)
		{
		case COMMON_BOX:
			{
				it->second.once_gather_num_list[XingzuoYiJiConfig::SENIOR_BOX] = 0;
				it->second.once_gather_num_list[XingzuoYiJiConfig::MEDIUM_BOX] = 0;
			}
			break;
		case GREAT_BOX:
			{
				it->second.once_gather_num_list[XingzuoYiJiConfig::JUNIOR_BOX] = 0;
				it->second.once_gather_num_list[XingzuoYiJiConfig::MOLONG_BOX] = 0;
			}
			break;
		default:
			return;
		}
	}
}

void SpecialXingzuoYiji::CalNextRefreshGatherTime()
{
	time_t today_zero_time = GetZeroTime(EngineAdapter::Instance().Time());
	time_t today_sec = EngineAdapter::Instance().Time() - today_zero_time;
	if (static_cast<unsigned int>(today_sec) < REFRESH_COMMON_GATHER_DAY_INTERVAL)
	{
		m_next_refresh_gather_time = static_cast<unsigned int>(today_zero_time) + REFRESH_COMMON_GATHER_DAY_INTERVAL;
	}
	else
	{
		const XingzuoYijiOtherCfg *other_cfg = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetOtherCfg();
		if (NULL != other_cfg && 0 != other_cfg->common_box_refresh_time)
		{
			m_next_refresh_gather_time = static_cast<unsigned int>(today_zero_time) +
				(static_cast<unsigned int>(today_sec) / other_cfg->common_box_refresh_time + 1) * other_cfg->common_box_refresh_time;
		}
	}
}