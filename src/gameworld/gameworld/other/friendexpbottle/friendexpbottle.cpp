#include "friendexpbottle.hpp"
#include "obj/character/role.h"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/errornum.h"
#include "protocal/msgother.h"
#include "protocal/msgfriend.h"
#include "friend/friendmanager.h"
#include "other/capability/capability.hpp"

FriendExpBottle::FriendExpBottle() : m_role(NULL), m_next_add_bottle_exp_time(0), m_next_broadcast_time(0), m_next_auto_add_friend_num_time(0), m_is_notice(true)
{
	m_next_add_bottle_exp_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + m_param.next_add_bottle_exp_time;
	m_next_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + m_param.next_broadcast_time;
	const FriendExpBottleOtherCfg *other_cfg = LOGIC_CONFIG->GetFriendExpBottleConfig().GetFriendExpBottleOtherCfg();
	if (NULL != other_cfg)
	{
		m_next_auto_add_friend_num_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg->auto_add_friend_num_time;
	}
}

FriendExpBottle::~FriendExpBottle()
{

}

void FriendExpBottle::Init(Role *role, const FriendExpBottleParam &param)
{
	m_role = role;
	m_param = param;
}

void FriendExpBottle::GetInitParam(FriendExpBottleParam *param)
{
	if (NULL != param)
	{
		*param = m_param;
	}
}

void FriendExpBottle::Update(unsigned int now_second)
{
	const FriendExpBottleOtherCfg *other_cfg = LOGIC_CONFIG->GetFriendExpBottleConfig().GetFriendExpBottleOtherCfg();
	if (NULL != other_cfg)
	{
		if (now_second >= m_next_add_bottle_exp_time)
		{
			this->AddBottleExp();
			m_next_add_bottle_exp_time = now_second + other_cfg->get_exp_interval;
		}

		if (now_second >= m_next_auto_add_friend_num_time)
		{
			m_next_auto_add_friend_num_time = now_second + other_cfg->auto_add_friend_num_time;
			const ExpBottleLimitCfg *limit_cfg = LOGIC_CONFIG->GetFriendExpBottleConfig().GetExpBottleLimitCfg(m_param.get_exp_count);
			if (NULL != limit_cfg && m_role->GetLevel() >= limit_cfg->fetch_level_limit &&
				m_role->GetFriendManager()->GetFriendNumber() + m_param.auto_add_count < limit_cfg->min_friend_max_count)
			{
				++m_param.auto_add_count;
				this->SendFriendExpBottleInfo();
			}
		}
	}
}

void FriendExpBottle::OnRoleUpLevel(int level)
{
	const FriendExpBottleOtherCfg *other_cfg = LOGIC_CONFIG->GetFriendExpBottleConfig().GetFriendExpBottleOtherCfg();
	if (NULL != other_cfg)
	{
		if (FRIEND_EXP_BOTTLE_STATE_NOT_ACTIVE == m_param.state && level >= other_cfg->active_level)
		{
			m_param.state = FRIEND_EXP_BOTTLE_STATE_NOT_FULL;
			m_param.next_add_bottle_exp_time = other_cfg->get_exp_interval;
			m_next_add_bottle_exp_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + m_param.next_add_bottle_exp_time;
		}
	}
}

void FriendExpBottle::OnRoleLogin()
{
	this->SendFriendExpBottleInfo();
}

void FriendExpBottle::OnRoleLogout()
{
	if (m_next_add_bottle_exp_time - static_cast<unsigned int>(EngineAdapter::Instance().Time()) > 0)
	{
		m_param.next_add_bottle_exp_time = m_next_add_bottle_exp_time - static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}
	else
	{
		m_next_add_bottle_exp_time = 0;
	}

	if (m_next_broadcast_time - static_cast<unsigned int>(EngineAdapter::Instance().Time()) > 0)
	{
		m_param.next_broadcast_time = m_next_broadcast_time - static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}
	else
	{
		m_param.next_broadcast_time = 0;
	}
}

void FriendExpBottle::OnFriendExpBottleOperateReq(int type)
{
	switch (type)
	{
	case FRIEND_EXP_BOTTLE_OPERATE_TYPE_ADD_EXP:
		this->AddRoleExp();
		break;

	case FRIEND_EXP_BOTTLE_OPERATE_TYPE_BROADCAST:
		this->Broadcast();
		break;

	case FRIEND_EXP_BOTTLE_OPERATE_TYPE_REQ_INFO:
		this->SendFriendExpBottleInfo();
		break;
	}
}

void FriendExpBottle::AddRoleExp()
{
	if (NULL == m_role)
	{
		return;
	}

	bool can_get_add_role_exp = false;
	switch (m_param.state)
	{
	case FRIEND_EXP_BOTTLE_STATE_NOT_ACTIVE:
		m_role->NoticeNum(errornum::EN_EXP_BOTTLE_NOT_ACTIVE);
		return;

	case FRIEND_EXP_BOTTLE_STATE_NOT_FULL:
		m_role->NoticeNum(errornum::EN_EXP_BOTTLE_NOT_FULL);
		return;

	case FRIEND_EXP_BOTTLE_STATE_FRIEND_NOT_ENOUGH:
		{
			const ExpBottleLimitCfg *limit_cfg = LOGIC_CONFIG->GetFriendExpBottleConfig().GetExpBottleLimitCfg(m_param.get_exp_count);
			if (NULL != limit_cfg)
			{
				if (m_role->GetFriendManager()->GetFriendNumber() + m_param.auto_add_count < limit_cfg->min_friend_max_count)
				{
					m_role->NoticeNum(errornum::EN_EXP_BOTTLE_FRIEND_NOT_ENOUGH);
					return;
				}

				if (m_role->GetLevel() < limit_cfg->fetch_level_limit)
				{
					m_role->NoticeNum(errornum::EN_ITEM_LEVEL_LIMIT);
					return;
				}

				if (m_param.get_exp_count < limit_cfg->get_exp_times)
				{
					can_get_add_role_exp = true;
				}
			}
		}
		break;

	case FRIEND_EXP_BOTTLE_STATE_FULL:
		{
			can_get_add_role_exp = true;
		}
		break;
	}

	if (can_get_add_role_exp)
	{
		m_role->AddExp(m_param.exp, "FriendExpBottle", Role::EXP_ADD_FRIEND_EXP_BOTTLE);
		m_param.get_exp_count++;
		m_param.exp = 0;
		m_param.state = FRIEND_EXP_BOTTLE_STATE_NOT_FULL;
		m_is_notice = true;

		this->SendFriendExpBottleInfo();
	}
}

void FriendExpBottle::Broadcast()
{
	const FriendExpBottleOtherCfg *other_cfg = LOGIC_CONFIG->GetFriendExpBottleConfig().GetFriendExpBottleOtherCfg();
	if (NULL != other_cfg && NULL != m_role)
	{
		if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) >= m_next_broadcast_time)
		{
			m_param.next_broadcast_time = other_cfg->broadcast_cd;
			m_next_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + m_param.next_broadcast_time;
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_friend_exp_bottle_search_friend,
				m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), m_role->GetUID());

			if (length > 0)
			{
				// World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ONLY_CHAT_WORLD);
			}
		}

		static const int SCENE_MANAGER_RANDOM_GET_ROLE_MAX_COUNT = 16;

		int uid_list[SCENE_MANAGER_RANDOM_GET_ROLE_MAX_COUNT] = { 0 };

		World::GetInstWorld()->GetSceneManager()->RandomGetRoleList(uid_list);
		for (int i = 0; i < SCENE_MANAGER_RANDOM_GET_ROLE_MAX_COUNT; i++)
		{
			if (m_role->GetFriendManager()->IsFriendFull())   // 好友已经达到最大数量
			{
				m_role->NoticeNum(errornum::EN_FRIEND_NUM_MAX);
				return;
			}

			if (0 == uid_list[i])
			{
				continue;
			}

			UserID target_user_id = IntToUid(uid_list[i]);

			if (target_user_id == m_role->GetUserId()) return;

			if (m_role->GetFriendManager()->IsBlack(target_user_id)) // 邀请者在自己的黑名单中
			{
				continue;
			}

			if (m_role->GetFriendManager()->IsFriend(target_user_id)) // 已经是好友
			{
				continue;
			}

			Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(target_user_id);
			if (NULL == target_role)
			{
				m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
			}
			else
			{
				if (!target_role->GetFriendManager()->IsFriendFull()) // 好友已满 服务端先过滤
				{
					Protocol::SCAddFriendRoute af_route;
					m_role->GetName(af_route.req_gamename);
					af_route.req_user_id = m_role->GetUID();
					af_route.req_avatar = m_role->GetAvatar();
					af_route.req_sex = m_role->GetSex();
					af_route.req_prof = m_role->GetProf();
					af_route.req_camp = m_role->GetCamp();
					af_route.req_level = m_role->GetLevel();
					af_route.req_avatar_timestamp = m_role->GetAvatarTimeStamp();
					af_route.req_total_capability = m_role->GetCapability()->GetTotalCapability();
					EngineAdapter::Instance().NetSend(target_role->GetNetId(), (char*)&af_route, sizeof(Protocol::SCAddFriendRoute));
				}
			}
		}
	}
}

void FriendExpBottle::AddBottleExp()
{
	if (NULL == m_role || FRIEND_EXP_BOTTLE_STATE_NOT_ACTIVE == m_param.state)
	{
		return;
	}

	unsigned long long add_exp = m_param.exp;
	const FriendExpBottleOtherCfg *other_cfg = LOGIC_CONFIG->GetFriendExpBottleConfig().GetFriendExpBottleOtherCfg();
	if (NULL != other_cfg)
	{
		FriendManager *friend_manager = m_role->GetFriendManager();
		if (NULL != friend_manager)
		{
			add_exp += other_cfg->friend_extra_add_exp * (friend_manager->GetFriendNumber() + m_param.auto_add_count);
			add_exp += other_cfg->per_minute_exp;

			const ExpBottleLimitCfg *limit_cfg = LOGIC_CONFIG->GetFriendExpBottleConfig().GetExpBottleLimitCfg(m_param.get_exp_count);
			if (NULL != limit_cfg)
			{
				if (add_exp >= limit_cfg->exp_limit)
				{
					m_param.exp = limit_cfg->exp_limit;

					if (m_role->GetFriendManager()->GetFriendNumber() + m_param.auto_add_count >= limit_cfg->min_friend_max_count &&
						m_role->GetLevel() >= limit_cfg->fetch_level_limit && m_param.get_exp_count < limit_cfg->get_exp_times)
					{
						m_param.state = FRIEND_EXP_BOTTLE_STATE_FULL;
					}
					else
					{
						m_param.state = FRIEND_EXP_BOTTLE_STATE_FRIEND_NOT_ENOUGH;
					}

					if (m_is_notice)
					{
						this->SendFriendExpBottleInfo();
						m_is_notice = false;
					}
				}
				else
				{
					m_param.exp = add_exp;
				}
			}

			m_param.next_add_bottle_exp_time = other_cfg->get_exp_interval;
		}
	}
}

void FriendExpBottle::SendFriendExpBottleInfo()
{
	static Protocol::SCFriendExpBottleAddExp add_exp;
	add_exp.next_broadcast_time = m_next_broadcast_time;
	add_exp.exp = m_param.exp;
	add_exp.get_exp_count = m_param.get_exp_count;
	add_exp.auto_add_friend_count = m_param.auto_add_count;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&add_exp, sizeof(add_exp));
}