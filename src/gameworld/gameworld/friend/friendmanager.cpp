#include "servercommon/serverclmempool.h"
#include "servercommon/servercommon.h"
#include "servercommon/errornum.h"
#include "servercommon/internalprotocal/gcprotocal.h"
#include "servercommon/string/gameworldstr.h"
#include "protocal/msgfriend.h"

#include "internalcomm.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "friendblessmanager.h"

#include "globalconfig/globalconfig.h"

#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "scene/activityshadow/activityshadow.hpp"

#include "friendmanager.h"
#include "servercommon/noticenum.h"
#include "world.h"
#include "item/itempool.h"
#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"
#include "other/shop/shop.hpp"
#include "item/other/otheritem.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/marryconfig.hpp"
#include "other/friendgift/friendgiftconfig.hpp"

#include "gamelog.h"

#include "rmiclient/rmiroleclient.h"

#include "scene/marryshadow/marryshadow.h"
#include "other/daycounter/daycounter.hpp"
#include "config/logicconfigmanager.hpp"

#include "global/usercache/relationcache.hpp"
#include "global/usercache/usercache.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "protocal/msgchatmsg.h"
#include "chat/chatmanager.h"
#include "other/shop/shop.hpp"
#include "friend/teaminfo.hpp"
#include "other/activedegree/activedegree.hpp"
#include "other/sublock/sublock.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/qingyuan/qingyuan.hpp"
#include "servercommon/serverconfig/crossrankrewardconfig.hpp"
#include "global/guild/roleguild.hpp"
#include "global/cross/crossmanager.h"
#include "config/guildconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "other/cross/rolecross.h"
#include "friendconfig.hpp"

FriendManager::FriendManager()
	: m_role(NULL), 
	  m_friend_num(0), m_max_friend_num(0), m_friend_list(NULL), m_friend_dirty_mark(NULL), m_friend_old_state(NULL), m_friend_list_is_change(false),
	  m_black_num(0), m_max_black_num(0), m_black_list(NULL), m_black_dirty_mark(NULL), m_black_old_state(NULL), m_black_list_is_change(false), 
	  m_enemy_num(0), m_max_enemy_num(0), m_enemy_list(NULL), m_enemy_dirty_mark(NULL), m_enemy_old_state(NULL), m_enemy_list_change(false),
	  m_all_give_flower_num(0), m_day_give_flower_num(0), m_all_charm(0), m_week_add_charm(0), m_day_add_charm(0), m_sent_to_client(false), m_friend_chked(false)
{
}

FriendManager::~FriendManager()
{
	if (NULL != m_friend_list)
	{
		ServerMsgMem *tmp = (ServerMsgMem*)m_friend_list;
		delete []tmp;
		tmp = (ServerMsgMem*)m_friend_dirty_mark;
		delete []tmp;
		tmp = (ServerMsgMem*)m_friend_old_state;
		delete []tmp;
	}

	if (NULL != m_enemy_list)
	{
		ServerMsgMem *tmp = (ServerMsgMem*)m_enemy_list;
		delete []tmp;
		tmp = (ServerMsgMem*)m_enemy_dirty_mark;
		delete []tmp;
		tmp = (ServerMsgMem*)m_enemy_old_state;
		delete []tmp;
	}
}

void FriendManager::Init(Role * role, const FriendListParam &friends, const BlacklistsParam & blacks, const EnemyListParam &enemys, 
						 int all_give_flower_num, int day_give_flower_num, int all_charm, int week_add_charm, int day_add_charm)
{
	m_role = role;

	int count = 0;
	int list_size = 0;
	int max_index = 0;

	m_week_add_charm = week_add_charm;
	m_day_add_charm = day_add_charm;

	if (0 == m_role->GetCommonDataParam()->next_clear_add_charm_time)
	{
		this->CheckClearAddCharm();
	}

	{
		max_index = 0;
		count = (friends.count < MAX_FRIEND_NUM) ? friends.count : MAX_FRIEND_NUM;

		for (int i = 0; i < count; ++i)
		{
			if (max_index < friends.friend_list[i].index)
			{
				max_index = friends.friend_list[i].index;
			}
		}

		list_size = (max_index + EXPAND_SIZE < MAX_FRIEND_NUM) ? (max_index + EXPAND_SIZE) : MAX_FRIEND_NUM;
		this->ResizeFriendList(list_size);

		for (int i = count - 1; i >= 0; --i)
		{
			int index = friends.friend_list[i].index;
			if (index < 0 || index >= m_max_friend_num) 
			{
				gamelog::g_log_serious_error.printf(LL_CRITICAL, "[FriendManager::Init Friend][[%d %s] friend index invalid:%d]", 
					UidToInt(m_role->GetUserId()), m_role->GetName(), index);
				continue;
			}

			if (m_friend_old_state[index]) 
			{
				gamelog::g_log_serious_error.printf(LL_CRITICAL, "[FriendManager::Init Friend][[%d %s] friend index duplicate:%d]", 
					UidToInt(m_role->GetUserId()), m_role->GetName(), index);
				continue;
			}

			m_friend_list[index].user_id = IntToUid(friends.friend_list[i].friend_uid);
			m_friend_list[index].intimacy = friends.friend_list[i].intimacy;
			m_friend_list[index].team_today_intimacy = friends.friend_list[i].team_today_intimacy;
			m_friend_list[index].gift_count = friends.friend_list[i].gift_count;
			m_friend_list[index].favorable_impression = friends.friend_list[i].favorable_impression;
			m_friend_list[index].day_chat_count = friends.friend_list[i].day_chat_count;
			m_friend_list[index].add_timestamp = friends.friend_list[i].add_timestamp;

			m_friend_old_state[index] = true;
			++m_friend_num;
		}
	}

	{
		max_index = 0;
		count = (blacks.count < MAX_BLACK_NUM) ? blacks.count : MAX_BLACK_NUM;

		for (int i = 0; i < count; ++i)
		{
			if (max_index < blacks.black_list[i].index)
			{
				max_index = blacks.black_list[i].index;
			}
		}

		list_size = (max_index + EXPAND_SIZE < MAX_BLACK_NUM) ? (max_index + EXPAND_SIZE) : MAX_BLACK_NUM;
		this->ResizeBlackList(list_size);

		for (int i = count - 1; i >= 0; --i)
		{
			int index = blacks.black_list[i].index;
			if (index < 0 || index >= m_max_black_num) 
			{
				gamelog::g_log_serious_error.printf(LL_CRITICAL, "[FriendManager::Init blacklist][[%d %s] index invalid:%d]", 
					UidToInt(m_role->GetUserId()), m_role->GetName(), index);
				continue;
			}

			if (m_black_old_state[index]) 
			{
				gamelog::g_log_serious_error.printf(LL_CRITICAL, "[FriendManager::Init blacklist][[%d %s] index duplicate:%d]", 
					UidToInt(m_role->GetUserId()), m_role->GetName(), index);
				continue;
			}

			m_black_list[index].user_id = IntToUid(blacks.black_list[i].uid);
			F_STRNCPY(m_black_list[index].gamename, blacks.black_list[i].gamename, sizeof(m_black_list[0].gamename));
			m_black_list[index].sex = blacks.black_list[i].sex;
			m_black_list[index].prof = blacks.black_list[i].prof;

			m_black_old_state[index] = true;
			++m_black_num;
		}
	}
	
	{
		max_index = 0;
		count = (enemys.count < MAX_ENEMY_NUM) ? enemys.count : MAX_ENEMY_NUM;

		for (int i = 0; i < count; ++i)
		{
			if (max_index < enemys.enemy_list[i].index)
			{
				max_index = enemys.enemy_list[i].index;
			}
		}

		list_size = (max_index + EXPAND_SIZE < MAX_ENEMY_NUM) ? (max_index + EXPAND_SIZE) : MAX_ENEMY_NUM;
		this->ResizeEnemyList(list_size);

		for (int i = count - 1; i >= 0; --i)
		{
			int index = enemys.enemy_list[i].index;
			if (index < 0 || index >= m_max_enemy_num) 
			{
				gamelog::g_log_serious_error.printf(LL_CRITICAL, "[FriendManager::Init enemy][[%d %s] enemy index invalid:%d]", 
					UidToInt(m_role->GetUserId()), m_role->GetName(), index);
				continue;
			}

			if (m_enemy_old_state[index]) 
			{
				gamelog::g_log_serious_error.printf(LL_CRITICAL, "[FriendManager::Init enemy][[%d %s] enemy index duplicate:%d]", 
					UidToInt(m_role->GetUserId()), m_role->GetName(), index);
				continue;
			}

			m_enemy_list[index].kill_count = enemys.enemy_list[i].kill_count;
			m_enemy_list[index].user_id = IntToUid(enemys.enemy_list[i].enemy_uid);
			m_enemy_list[index].plat_type = enemys.enemy_list[i].enemy_plat_type;
			m_enemy_list[index].last_kill_time = enemys.enemy_list[i].last_kill_time;
			m_enemy_list[index].be_kill_count = enemys.enemy_list[i].be_kill_count;

			m_enemy_old_state[index] = true;
			++m_enemy_num;
		}
	}

	{
		m_all_give_flower_num = all_give_flower_num;
		m_day_give_flower_num = day_give_flower_num;
		m_all_charm = all_charm;
	}
}

void FriendManager::GetInitParam(int *all_give_flower_num, int *day_give_flower_num, int *all_charm)
{
	*all_give_flower_num = m_all_give_flower_num;
	*day_give_flower_num = m_day_give_flower_num;
	*all_charm = m_all_charm;
}

void FriendManager::OnWeekChange()
{
	m_week_add_charm = 0;
}

void FriendManager::Update(UInt32 now_second)
{
	if (!CrossConfig::Instance().IsHiddenServer() && !m_friend_chked && m_sent_to_client)	// 检查残留的单向好友
	{
		m_friend_chked = true;

		int my_uid = m_role->GetUID();
		for (int i = 0; i < m_max_friend_num; ++i)
		{
			if (INVALID_USER_ID == m_friend_list[i].user_id || VIRTUAL_USER_ID == m_friend_list[i].user_id)
			{
				continue;
			}

			int friend_uid = UidToInt(m_friend_list[i].user_id);
			bool bret = RelationCacheManager::Instance().HasFriend(friend_uid, my_uid);
			if (!bret)
			{
				this->RemoveFriend(m_friend_list[i].user_id, "SingleRelation");
			}
		}

		this->FriendSave();
	}
}

void FriendManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		for (int i = 0; i < m_max_friend_num; ++i)
		{
			if (INVALID_USER_ID != m_friend_list[i].user_id && VIRTUAL_USER_ID != m_friend_list[i].user_id)
			{
				m_friend_list[i].team_today_intimacy = 0;
				m_friend_list[i].gift_count = 0;
				m_friend_list[i].day_chat_count = 0;
				m_friend_dirty_mark[i] = true;
			}
		}

		m_friend_list_is_change = true;

		m_day_give_flower_num = 0;
		m_day_add_charm = 0;

		this->CheckClearAddCharm();
	}
}

bool FriendManager::IsFriend(UserID user_id, int *index)
{
	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (m_friend_list[i].user_id == user_id)
		{
			if (NULL != index) *index = i;
			return true;
		}
	}

	return false;
}

bool FriendManager::AddFriend(UserID user_id, const char *reason)
{
	int index = -1;
	if (this->IsFriend(user_id, &index))
	{
		return false;
	}

	if (this->IsFriendFull())
	{
		m_role->NoticeNum(errornum::EN_FRIEND_NUM_MAX);
		return false;
	}

	if (this->IsBlack(user_id))
	{
		this->RemoveBlack(user_id, "AddFriend");
	}

	if (m_friend_num >= MAX_FRIEND_NUM) return false;
	
	if (m_friend_num >= m_max_friend_num)
	{
		int list_size = (m_max_friend_num + EXPAND_SIZE < MAX_FRIEND_NUM) ? (m_max_friend_num + EXPAND_SIZE) : MAX_FRIEND_NUM;
		this->ResizeFriendList(list_size);
	}

	index = -1;
	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (m_friend_list[i].user_id == INVALID_USER_ID)
		{
			index = i;
			break;
		}
	}
	if (-1 == index) return false;

	m_friend_list[index].user_id = user_id;
	m_friend_list[index].intimacy = 0;
	m_friend_list[index].team_today_intimacy = 0;
	m_friend_list[index].gift_count = 0;
	m_friend_list[index].favorable_impression = 0;
	m_friend_list[index].day_chat_count = 0;
	m_friend_list[index].add_timestamp = EngineAdapter::Instance().Time();

	m_friend_dirty_mark[index] = true;
	m_friend_list_is_change = true;

	++ m_friend_num;

	this->SendFriendChangeToRole(index, INVALID_USER_ID);
	
	EventHandler::Instance().OnAddFriend(m_role);

	RelationCacheManager::Instance().AddFriend(m_role->GetUID(), UidToInt(user_id));

	gamelog::g_log_friends.printf(LL_INFO, "[AddFriend Succ][user[%s %d] add: role_id:%d][Reason:%s]", 
		m_role->GetName(), UidToInt(m_role->GetUserId()), UidToInt(m_friend_list[index].user_id), reason);

	return true;
}

bool FriendManager::RemoveFriend(UserID user_id, const char *reason)
{
	if (m_role->GetLoverUid() == UidToInt(user_id))
	{
		//m_role->NoticeNum(errornum::EN_FRIEND_REMOVE_LOVER_LIMIT);
		return false;
	}

	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (m_friend_list[i].user_id == user_id)
		{
			gamelog::g_log_friends.printf(LL_INFO, "[RemoveFriend Succ][user[%s %d] remove: role_id:%d][Reason:%s]", 
				m_role->GetName(), UidToInt(m_role->GetUserId()), UidToInt(m_friend_list[i].user_id), reason);

			m_friend_list[i].intimacy = 0;
			m_friend_list[i].team_today_intimacy = 0;
			m_friend_list[i].user_id = INVALID_USER_ID;
			m_friend_list[i].gift_count = 0;
			m_friend_list[i].favorable_impression = 0;
			m_friend_list[i].day_chat_count = 0;

			m_friend_dirty_mark[i] = true;
			m_friend_list_is_change = true;
			--m_friend_num;

			this->SendFriendChangeToRole(i, user_id);

			RelationCacheManager::Instance().RemoveFriend(m_role->GetUID(), UidToInt(user_id));
			FriendBlessManager::Instance().OnRemoveFriend(m_role, UidToInt(user_id));

			return true;
		}
	}

	return false;
}

void FriendManager::IntimacyIncrease(Role *this_role, Role *other_role, int value, const char *reason, bool from_team)
{
	if (NULL == this_role || NULL == other_role || this_role == other_role) return;

	bool other_is_this_friend = false, this_is_other_friend = false;

	int intimacy = 0;
	if (this_role->GetFriendManager()->IntimacyIncreaseHelper(other_role->GetUserId(), value, false, reason, from_team, &intimacy))
	{
		other_is_this_friend = true;

		if (other_role->GetFriendManager()->IntimacyIncreaseHelper(this_role->GetUserId(), intimacy, true, reason, from_team))
		{
			this_is_other_friend = true;	
		}
	}

	if (other_is_this_friend != this_is_other_friend)
	{
		if (other_is_this_friend) this_role->GetFriendManager()->RemoveFriend(other_role->GetUserId(), "Intimacy Remove Friend");
		if (this_is_other_friend) other_role->GetFriendManager()->RemoveFriend(this_role->GetUserId(), "Intimacy Remove Friend");
	}
}

bool FriendManager::IntimacyIncreaseHelper(const UserID &user_id, int intimacy_value, bool is_set, const char *reason, bool from_team, int *p_curr_intimacy)
{
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG))
	{
		intimacy_value *= 2;
	}

	const int TEAM_TODAY_INTIMACY_LIMIT = GLOBALCONFIG->GetTeamTodayIntimacyLimit();

	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (user_id == m_friend_list[i].user_id)
		{
			int old_intimacy = m_friend_list[i].intimacy;

			if (is_set)
			{
				if (from_team) m_friend_list[i].team_today_intimacy += (intimacy_value - m_friend_list[i].intimacy);
				m_friend_list[i].intimacy = intimacy_value;
			}
			else
			{
				if (from_team)
				{
					if (m_friend_list[i].team_today_intimacy + intimacy_value >= TEAM_TODAY_INTIMACY_LIMIT)
					{
						m_friend_list[i].intimacy += (TEAM_TODAY_INTIMACY_LIMIT - m_friend_list[i].team_today_intimacy);
						m_friend_list[i].team_today_intimacy = TEAM_TODAY_INTIMACY_LIMIT;
					}
					else
					{
						m_friend_list[i].intimacy += intimacy_value;
						m_friend_list[i].team_today_intimacy += intimacy_value;
					}
				}
				else
				{
					m_friend_list[i].intimacy += intimacy_value;
				}
			}
			
			if (m_friend_list[i].intimacy < 0) m_friend_list[i].intimacy = 0;
			if (m_friend_list[i].team_today_intimacy < 0) m_friend_list[i].team_today_intimacy = 0;

			if (old_intimacy != m_friend_list[i].intimacy)
			{
				m_friend_dirty_mark[i] = true;
				m_friend_list_is_change = true;

				this->SendFriendChangeToRole(i, user_id);

				if (m_role->GetTeamInfo()->InTeam())
				{
					if (GLOBALCONFIG->CheckFriendAttrChange(old_intimacy, m_friend_list[i].intimacy))
					{
						m_role->GetTeamInfo()->ReCalcFriendBuff();
					}
				}

				static const int LOG_INTIMACY_PER_VALUE = 100;
				UNSTD_STATIC_CHECK(LOG_INTIMACY_PER_VALUE > 0);
				
				if (old_intimacy / LOG_INTIMACY_PER_VALUE != m_friend_list[i].intimacy / LOG_INTIMACY_PER_VALUE)
				{
					gamelog::g_log_friends.printf(LL_INFO, "[IntimacyIncrease Succ][user[%s %d] add: role_id:%d intimacy:%d team_today_intimacy:%d][Reason:%s]", 
						m_role->GetName(), UidToInt(m_role->GetUserId()),
						UidToInt(m_friend_list[i].user_id), m_friend_list[i].intimacy, m_friend_list[i].team_today_intimacy, reason);
				}
			}

			if (NULL != p_curr_intimacy) *p_curr_intimacy = m_friend_list[i].intimacy;

			return true;
		}
	}

	return false;
}

void FriendManager::OnRoleResetName()
{
	static ggprotocal::FriendResetName frn;
	frn.friend_role_id = UidToInt(m_role->GetUserId());
	m_role->GetName(frn.friend_new_name);

	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (INVALID_USER_ID != m_friend_list[i].user_id && VIRTUAL_USER_ID != m_friend_list[i].user_id)
		{
			World::GetInstWorld()->GetSceneManager()->SendMessageToGameWorld(m_friend_list[i].user_id, (const char*)&frn, sizeof(frn));
		}
	}
}

void FriendManager::OnResetFriendName(int friend_uid, GameName new_friend_name)
{
	if (0 == friend_uid || NULL == new_friend_name) return;

	UserID friend_user_id = IntToUid(friend_uid);
	new_friend_name[sizeof(GameName) - 1] = 0;

	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (m_friend_list[i].user_id == friend_user_id)
		{
			m_friend_dirty_mark[i] = true;
			m_friend_list_is_change = true;

			this->SendFriendChangeToRole(i, m_friend_list[i].user_id);
			break;
		}
	}

	for (int i = 0; i < m_max_black_num; ++i)
	{
		if (m_black_list[i].user_id == friend_user_id)
		{
			STRNCPY(m_black_list[i].gamename, new_friend_name, sizeof(GameName));
			m_black_dirty_mark[i] = true;
			m_black_list_is_change = true;

			this->SendBlackChangeToRole(i, m_black_list[i].user_id);
			break;
		}
	}
}

void FriendManager::OnRoleSetSex()
{
	static ggprotocal::FriendSetSex fss;
	fss.friend_role_id = UidToInt(m_role->GetUserId());
	fss.friend_plat_type = m_role->GetRoleCross()->GetOriginPlatType();
	fss.sex = m_role->GetSex();

	static MailSubject mail_subject = {0};
	static MailContentTxt mail_content_txt = {0};

	int length1 = SNPRINTF(mail_subject, sizeof(mail_subject), gamestring::g_role_set_sex_subject);
	int length2 = SNPRINTF(mail_content_txt, sizeof(mail_content_txt), gamestring::g_role_set_sex_content, 
		UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp());

	if (length1 > 0 && length2 > 0)
	{
		for (int i = 0; i < m_max_friend_num; ++i)
		{
			if (INVALID_USER_ID != m_friend_list[i].user_id && VIRTUAL_USER_ID != m_friend_list[i].user_id)
			{
				MailRoute::Instance().MailToUser(m_friend_list[i].user_id, mail_subject, mail_content_txt);

				World::GetInstWorld()->GetSceneManager()->SendMessageToGameWorld(m_friend_list[i].user_id, (const char*)&fss, sizeof(fss));
			}
		}
	}
}

void FriendManager::OnSetFriendSex(int friend_uid, int friend_plat_type, char sex)
{
	if (0 == friend_uid || sex < FEMALE || sex >= SEX_MAX) return;

	UserID friend_user_id = IntToUid(friend_uid);

	for (int i = 0; i < m_max_friend_num && i < MAX_FRIEND_NUM; ++i)
	{
		if (m_friend_list[i].user_id == friend_user_id)
		{
			m_friend_dirty_mark[i] = true;
			m_friend_list_is_change = true;

			this->SendFriendChangeToRole(i, m_friend_list[i].user_id);
			break;
		}
	}

	for (int i = 0; i < m_max_enemy_num; ++i)
	{
		if (m_enemy_list[i].user_id == friend_user_id && m_enemy_list[i].plat_type == friend_plat_type)
		{
			m_enemy_dirty_mark[i] = true;
			m_enemy_list_change = true;

			this->SendEnemyChangeToRole(i, m_enemy_list[i].user_id, m_enemy_list[i].plat_type);
			break;
		}
	}

	for (int i = 0; i < m_max_black_num; ++i)
	{
		if (m_black_list[i].user_id == friend_user_id)
		{
			m_black_list[i].sex = sex;
			m_black_dirty_mark[i] = true;
			m_black_list_is_change = true;

			this->SendBlackChangeToRole(i, m_black_list[i].user_id);
			break;
		}
	}
}

void FriendManager::OnRoleUpgradeLevel()
{
	static ggprotocal::FriendSetLevel frn;
	frn.friend_role_id = UidToInt(m_role->GetUserId());
	frn.level = m_role->GetLevel();

	for (int i = 0; i < m_max_friend_num && i < MAX_FRIEND_NUM; ++i)
	{
		if (INVALID_USER_ID != m_friend_list[i].user_id && VIRTUAL_USER_ID != m_friend_list[i].user_id)
		{
			World::GetInstWorld()->GetSceneManager()->SendMessageToGameWorld(m_friend_list[i].user_id, (const char*)&frn, sizeof(frn));
		}
	}
}

void FriendManager::OnSetFriendLevel(int friend_uid)
{
	if (0 == friend_uid) return;

	UserID friend_user_id = IntToUid(friend_uid);

	for (int i = 0; i < m_max_friend_num && i < MAX_FRIEND_NUM; ++i)
	{
		if (m_friend_list[i].user_id == friend_user_id)
		{
			m_friend_dirty_mark[i] = true;
			m_friend_list_is_change = true;

			this->SendFriendChangeToRole(i, m_friend_list[i].user_id);
			break;
		}
	}
}

void FriendManager::SystemMailToAllFriend(MailSubject subject, MailContentTxt contenttxt)
{
	static UserID mail_user_list[MAX_FRIEND_NUM];
	int mail_user_num = 0;

	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (VIRTUAL_USER_ID != m_friend_list[i].user_id && INVALID_USER_ID != m_friend_list[i].user_id)
		{
			mail_user_list[mail_user_num ++] = m_friend_list[i].user_id;
		}
	}

	if (mail_user_num > 0)
	{
		MailRoute::Instance().MailToMultUser(mail_user_list, mail_user_num, subject, contenttxt);
	}
}

void FriendManager::SendSongHuaInfo()
{	
	CommonDataParam *common_data = m_role->GetCommonDataParam();
	if (NULL == common_data) return;

	static Protocol::SCSoneHuaInfo pro;
	pro.daily_use_free_song_hua_times = common_data->daily_use_free_song_hua_times;
	
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void FriendManager::OnGiveFlower(ggprotocal::GiveFlower *gf_msg)
{
	if (NULL == gf_msg) return;
	if (gf_msg->target_uid <= 0) return;

	if (m_role->GetSublock()->CheckHasLock()) return;

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	//等级限制
	if (m_role->GetLevel() < LOGIC_CONFIG->getFriendGiftCfg().GetOtherCfg().start_level)
	{
		m_role->NoticeNum(errornum::EN_ITEM_LEVEL_LIMIT);
		return;
	}

	UserID target_user_id = IntToUid(gf_msg->target_uid);
	int flower_num = 0;
	ItemID item_id = gf_msg->item_id;

	if (0 == gf_msg->is_marry)
	{	
		CommonDataParam *common_data = m_role->GetCommonDataParam();
		if (NULL == common_data) return;

		const FfOtherCfg &star_item_cfg = LOGIC_CONFIG->getFriendGiftCfg().GetOtherCfg();

		const OtherItem *item = (const OtherItem *)ITEMPOOL->GetItem(gf_msg->item_id);
		if (NULL == item || ItemBase::I_TYPE_OTHER != item->GetItemType() || OtherItem::I_SUB_TYPE_FLOWER != item->GetSubType() || item->GetValue() <= 0)
		{
			m_role->NoticeNum(errornum::EN_FLOWER_ITEM_TYPE_ERROR);
			return;
		}

		if (common_data->daily_use_free_song_hua_times < star_item_cfg.free_song_hua_times && 1 == item->GetValue())
		{
			common_data->daily_use_free_song_hua_times++;

			// 免费送花后获得奖励
			static MailContentParam contentparam; contentparam.Reset();
			UNSTD_STATIC_CHECK(MAX_ATTACHMENT_ITEM_NUM >= 1);
			contentparam.item_list[0].item_id = star_item_cfg.free_song_hua_reward.item_id;
			contentparam.item_list[0].is_bind = star_item_cfg.free_song_hua_reward.is_bind ? 1 : 0;
			contentparam.item_list[0].num = star_item_cfg.free_song_hua_reward.num;

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_friend_song_hua_content);
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}

		    // 免费送花后自动私聊
			Protocol::CSSingleChat *sc = (Protocol::CSSingleChat *)gamestring::GAMESTRING_BUF;
			sc->msg_length = SNPRINTF(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSSingleChat),
				sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSSingleChat), gamestring::g_msg_friend_song_hua_chat_content);
			if (sc->msg_length > 0)
			{
				Role * trget_role = World::GetInstWorld()->GetSceneManager()->GetRole(target_user_id);
				if (trget_role != NULL)
				{
					sc->to_uid = gf_msg->target_uid;
					ChatManager::Instance().OnSingleChat(m_role, sc, sizeof(Protocol::CSSingleChat) + sc->msg_length, true);
				}
			}

			flower_num = item->GetValue();
			this->SendSongHuaInfo();
		}
		else if (gf_msg->grid_index < 0)																			// 1: 从商店自动购买
		{
			int gold_price = 0;
			int bind_gold_price = 0;
			if (!LOGIC_CONFIG->GetShop().GetShopItemGoldPrice(gf_msg->item_id, &gold_price, &bind_gold_price) || (gold_price <= 0 && bind_gold_price <= 0))
			{
				m_role->NoticeNum(errornum::EN_BUY_FLOWER_ERROR);
				return;
			}

			Int64 consume_gold = gold_price, consume_gold_bind = 0;
			if (!knapsack->GetMoney()->UseAllGold(gold_price, "GiveFlower"))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}

			ItemExtern::BuyItemInfo buy_info;
			buy_info.item_id = gf_msg->item_id;
			buy_info.item_count = 1;
			buy_info.consume_gold = static_cast<int>(consume_gold + consume_gold_bind);
			EventHandler::Instance().OnShopBuyItem(m_role, 1, &buy_info, consume_gold, consume_gold_bind);	

			flower_num = item->GetValue();
		}
		else																								// 2: 消耗背包中的花
		{
			ItemGridData grid_data;
			if (!knapsack->GetItemGridData(gf_msg->grid_index, &grid_data))
			{
				m_role->NoticeNum(errornum::EN_FUNC_ITEM_LOAD);
				return;
			}

			const OtherItem *item = (const OtherItem *)ITEMPOOL->GetItem(grid_data.item_id);
			if (NULL == item || ItemBase::I_TYPE_OTHER != item->GetItemType() || OtherItem::I_SUB_TYPE_FLOWER != item->GetSubType() || item->GetValue() <= 0)
			{
				m_role->NoticeNum(errornum::EN_FLOWER_ITEM_TYPE_ERROR);
				return;
			}

			if (!knapsack->ConsumeItemByIndex(gf_msg->grid_index, 1, "GiveFlower")) return;

			flower_num = item->GetValue();
			item_id = grid_data.item_id;
		}
	}
	else
	{
		if (m_role->GetQingyuan()->CheckCanGiveFlower())
		{
			m_role->GetQingyuan()->OnXunyouGiveFlower(&item_id, &flower_num);
		}
		else
		{
			return;
		}
	}

	m_all_give_flower_num += flower_num;
	m_day_give_flower_num += flower_num;

	int real_add_charm = flower_num;
	
	m_role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_SONG_HUA);

	this->AddAllCharm(real_add_charm); // 送花者也加魅力

	// 加情缘值
	if (m_role->IsMarry() && m_role->GetLoverUid() == gf_msg->target_uid)
	{
		m_role->GetQingyuan()->AddQingyuanValue(flower_num);
	}

	if (m_role->GetFriendManager()->IsFriend(target_user_id))
	{
		m_role->GetFriendManager()->IntimacyIncreaseHelper(target_user_id, real_add_charm, false, "GiveFlower", false);
	}

	m_role->NoticeNum(noticenum::NT_GIVE_FLOWER_SUCCESS);

	EventHandler::Instance().OnGiveFlower(m_role, flower_num);

	gamelog::g_log_sundry.printf(LL_INFO, "[GiveFlower] [user_id:%d user_name:%s] [target_uid:%d flower_num:%d]", 
		UidToInt(m_role->GetUserId()), m_role->GetName(), gf_msg->target_uid, flower_num);

	static ggprotocal::BeGiveFlower bgf;
	bgf.from_uid = UidToInt(m_role->GetUserId());
	bgf.flower_num = flower_num;
	bgf.from_camp = (short)m_role->GetCamp();
	bgf.is_anonymity = (short)gf_msg->is_anonymity;
	m_role->GetName(bgf.from_name);
	bgf.item_id = item_id;
	bgf.reserve = 0;

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(target_user_id);
	if (NULL != target_role)
	{
		target_role->GetFriendManager()->OnBeGiveFlower(&bgf);
	}
}

void FriendManager::OnBeGiveFlower(ggprotocal::BeGiveFlower *bgf_msg)
{
	if (NULL == bgf_msg) return;
	if (bgf_msg->from_uid <= 0) return;

	UserID from_user_id = IntToUid(bgf_msg->from_uid);

	int real_add_charm = bgf_msg->flower_num;
	
	this->AddAllCharm(real_add_charm);

	// 加情缘值
	if (m_role->IsMarry() && m_role->GetLoverUid() == bgf_msg->from_uid)
	{
		m_role->GetQingyuan()->AddQingyuanValue(bgf_msg->flower_num);
	}

	if (m_role->GetFriendManager()->IsFriend(from_user_id))
	{
		m_role->GetFriendManager()->IntimacyIncreaseHelper(from_user_id, real_add_charm, false, "BeGiveFlower", false);
	}

	static Protocol::SCGiveFlower gf;
	gf.target_uid = UidToInt(m_role->GetUserId());
	gf.from_uid = bgf_msg->from_uid;
	gf.flower_num = bgf_msg->flower_num;
	gf.is_anonymity = bgf_msg->is_anonymity;
	m_role->GetName(gf.target_name);
	gf.item_id = bgf_msg->item_id;
	gf.reserve = 0;
	F_STRNCPY(gf.from_name, bgf_msg->from_name, sizeof(gf.from_name));

	if (bgf_msg->flower_num >= GIVE_FLOWER_BROADCAST_NUM)
	{
		World::GetInstWorld()->SendToAllGateway((const char *)&gf, sizeof(gf));

		int length = 0;
		if (0 == bgf_msg->is_anonymity)
		{
			bgf_msg->from_name[sizeof(bgf_msg->from_name) - 1] = 0;
			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_flower_notice, 
				bgf_msg->from_uid, bgf_msg->from_name, (int)bgf_msg->from_camp, 
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), 
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), bgf_msg->item_id, 
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp());
		}
		else
		{
			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_flower_notice_anonymity, 
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), bgf_msg->item_id);
		}

		if (length > 0)
		{
			if (bgf_msg->item_id == 26904)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
			else
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
			}
		}
	}
	else
	{
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&gf, sizeof(gf));
		Role * role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(gf.target_uid));
		if (NULL != role)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&gf, sizeof(gf));
		}
	}

	EventHandler::Instance().OnBeGiveFlower(m_role);

	gamelog::g_log_sundry.printf(LL_INFO, "[BeGiveFlower] [user_id:%d user_name:%s] [from_uid:%d flower_num:%d]", 
		UidToInt(m_role->GetUserId()), m_role->GetName(), bgf_msg->from_uid,  (int)bgf_msg->flower_num);
}

bool FriendManager::SetAllCharm(int charm)
{
	if (charm < 0) return false;

	m_all_charm = charm;

	this->SendAllCharm();

	return true;
}

void FriendManager::SendAllCharm()
{
	Protocol::SCAllCharmChange acc;
	acc.uid = m_role->GetUID();
	acc.all_charm = m_all_charm;
	acc.day_charm = m_day_add_charm;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&acc, sizeof(acc));
}

void FriendManager::AddAllCharm(int value)
{
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG))
	{
		value *= 2;
	}

	m_week_add_charm += value;
	m_day_add_charm += value;

	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
	if (CrossConfig::Instance().IsHiddenServer())
	{
		const UniqueServerID unique_server_id = m_role->GetRoleCross()->GetOriginalUniqueServerID();
		opengame_day = CrossManager::Instance().GetGameServerDayIndex(unique_server_id) + 1;
	}

	int limi_open_day = CrossRankRewardConfig::Instance().GetCrossRankLimitOpenDay(CROSS_PERSON_RANK_TYPE_ADD_CHARM);
	if (limi_open_day <= 0 || opengame_day >= limi_open_day)
	{
		m_role->GetCommonDataParam()->add_charm_for_rank += value;
	}
	
	m_all_charm += value;
	if (m_all_charm <= 0)
	{
		m_all_charm = 0;
	}

	this->SendAllCharm();
}

bool FriendManager::IsEnemy(UniqueUserID unique_userid)
{
	for (int i = 0; i < m_max_enemy_num; ++i)
	{
		if (m_enemy_list[i].user_id == unique_userid.user_id && m_enemy_list[i].plat_type == unique_userid.plat_type)
		{
			return true;
		}
	}

	return false;
}

bool FriendManager::AddEnemy(UniqueUserID unique_user_id, bool is_be_kill, const char *reason)
{
	UserID user_id = unique_user_id.user_id;
	int plat_type = unique_user_id.plat_type;

	for (int i = 0; i < m_max_enemy_num; ++i)
	{
		if (m_enemy_list[i].user_id == user_id && m_enemy_list[i].plat_type == plat_type)
		{

			if (is_be_kill)
			{
				++m_enemy_list[i].be_kill_count;

				++m_role->GetRoleGuild()->GetRoleGuildParam()->killed_count;

				//江湖传闻
				GuildID guild_id = m_role->GetGuildID();
				if (INVALID_GUILD_ID != guild_id)
				{
					auto guild_rumor_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildRumorCfg(GUILD_EVENT_TYPE_RUMOR_KILLED);
					if (guild_rumor_cfg != NULL)
					{
						if (m_role->GetRoleGuild()->GetRoleGuildParam()->killed_count == guild_rumor_cfg->param_1 || m_role->GetRoleGuild()->GetRoleGuildParam()->killed_count == guild_rumor_cfg->param_2
							|| m_role->GetRoleGuild()->GetRoleGuildParam()->killed_count == guild_rumor_cfg->param_3)
						{
							EventHandler::Instance().OnAddGuildRumorEvent(guild_id, m_role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_KILLED, m_role->GetRoleGuild()->GetRoleGuildParam()->killed_count);
						}
					}
				}

			}
			else
			{
				++m_enemy_list[i].kill_count;

				++m_role->GetRoleGuild()->GetRoleGuildParam()->kill_count;

				//江湖传闻
				GuildID guild_id = m_role->GetGuildID();
				if (INVALID_GUILD_ID != guild_id)
				{
					auto guild_rumor_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildRumorCfg(GUILD_EVENT_TYPE_RUMOR_KILL);
					if (guild_rumor_cfg != NULL)
					{
						if (m_role->GetRoleGuild()->GetRoleGuildParam()->kill_count == guild_rumor_cfg->param_1 || m_role->GetRoleGuild()->GetRoleGuildParam()->kill_count == guild_rumor_cfg->param_2
							|| m_role->GetRoleGuild()->GetRoleGuildParam()->kill_count == guild_rumor_cfg->param_3)
						{
							EventHandler::Instance().OnAddGuildRumorEvent(guild_id, m_role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_KILL, m_role->GetRoleGuild()->GetRoleGuildParam()->kill_count);
						}
					}
				}
			}

			m_enemy_list->last_kill_time = EngineAdapter::Instance().Time();

			m_enemy_dirty_mark[i] = true;
			m_enemy_list_change = true;

			this->SendEnemyChangeToRole(i, INVALID_USER_ID, -1);

			gamelog::g_log_friends.printf(LL_INFO, "[AddEnemy Succ][user[%s %d] add: role_id:%d plat_type:%d killcount:%d bekillcount:%d][Reason:%s]", 
				m_role->GetName(), UidToInt(m_role->GetUserId()), 
				UidToInt(m_enemy_list[i].user_id), m_enemy_list[i].plat_type, m_enemy_list[i].kill_count, m_enemy_list[i].be_kill_count, reason);

			return true;
		}
	}

	if (m_enemy_num >= m_max_enemy_num)
	{
		int list_size = (m_max_enemy_num + EXPAND_SIZE < MAX_ENEMY_NUM) ? (m_max_enemy_num + EXPAND_SIZE) : MAX_ENEMY_NUM;
		this->ResizeEnemyList(list_size);
	}

	int index = 0;

	if (m_enemy_num >= MAX_ENEMY_NUM)
	{
		for (int i = 1; i < m_max_enemy_num; ++i)
		{
			index = (m_enemy_list[index].last_kill_time > m_enemy_list[i].last_kill_time) ? i : index;
		}
	}
	else
	{
		for (int i = 0; i < m_max_enemy_num; ++i)
		{
			if (m_enemy_list[i].user_id == INVALID_USER_ID)
			{
				index = i;
				break;
			}
		}
	}

	m_enemy_list[index].user_id = user_id;
	m_enemy_list[index].plat_type = plat_type;

	if (is_be_kill)
	{
		++m_enemy_list[index].be_kill_count;
	}
	else
	{
		++m_enemy_list[index].kill_count;
	}

	m_enemy_list[index].last_kill_time = EngineAdapter::Instance().Time();
	m_enemy_num = (m_enemy_num >= MAX_ENEMY_NUM) ? MAX_ENEMY_NUM : m_enemy_num + 1;

	m_enemy_dirty_mark[index] = true;
	m_enemy_list_change = true;

	// 添加仇人，私聊狠话
	{
		// 回显信息给自己，以对方名义发送给自己
		int rand_str_index = RandomNum(10);
		std::string add_enemy_be_kill = "add_enemy_be_kill_";
		std::string add_enemy_kill_other = "add_enemy_kill_other_";
		//Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(UniqueUserIDToLongLong(unique_user_id));
		//if (nullptr != target_role)
		//{
		//	Protocol::CSSingleChat *sc = (Protocol::CSSingleChat *)gamestring::GAMESTRING_BUF;
		//	sc->msg_length = 0;
		//	if (is_be_kill)
		//	{
		//		sc->msg_length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSSingleChat),
		//			sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSSingleChat), (add_enemy_be_kill + std::to_string(rand_str_index)).c_str());
		//	}
		//	//else
		//	//{
		//	//	sc->msg_length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSSingleChat),
		//	//		sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSSingleChat), (add_enemy_kill_other + std::to_string(rand_str_index)).c_str());
		//	//}
		//	if (sc->msg_length > 0)
		//	{
		//		sc->to_uuid = UniqueUserIDToLongLong(m_role->GetUniqueUserID());
		//		sc->is_echo = 1; // 回显标记
		//		sc->special_param = 1;  // 仇人私聊
		//		ChatManager::Instance().OnSingleChat(target_role, sc, sizeof(Protocol::CSSingleChat) + sc->msg_length, true);
		//	}
		//}

		//发送给对方一句私聊
		Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(UniqueUserIDToLongLong(unique_user_id));
		if (nullptr != target_role)
		{
			// 同一个服务器的才可以发私聊
			if ((target_role->GetRoleCross()->GetOriginMergeServerID() == m_role->GetRoleCross()->GetOriginMergeServerID()) &&
				(unique_user_id.plat_type == m_role->GetUniqueUserID().plat_type))
			{
				Protocol::CSSingleChat *sc = (Protocol::CSSingleChat *)gamestring::GAMESTRING_BUF;
				sc->msg_length = 0;
				if (is_be_kill)
				{
					sc->msg_length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSSingleChat),
						sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSSingleChat), (add_enemy_be_kill + std::to_string(rand_str_index)).c_str());
				}
				//else
				//{
				//	sc->msg_length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSSingleChat),
				//		sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSSingleChat), (add_enemy_kill_other + std::to_string(rand_str_index)).c_str());
				//}
				if (sc->msg_length > 0)
				{
					sc->to_uuid = UniqueUserIDToLongLong(unique_user_id);
					sc->is_echo = 0;
					sc->special_param = 1;  // 仇人私聊
					ChatManager::Instance().OnSingleChat(m_role, sc, sizeof(Protocol::CSSingleChat) + sc->msg_length, true);
				}
			}
		}
	}

	this->SendEnemyChangeToRole(index, INVALID_USER_ID, -1);

	gamelog::g_log_friends.printf(LL_INFO, "[AddEnemy Succ][user[%s %d] add: role_id:%d plat_type:%d killcount:%d bekillcount:%d][Reason:%s]", 
		m_role->GetName(), UidToInt(m_role->GetUserId()), 
		UidToInt(m_enemy_list[index].user_id), m_enemy_list[index].plat_type, m_enemy_list[index].kill_count, m_enemy_list[index].be_kill_count, reason);

	return true;
}

bool FriendManager::RemoveEnemy(UserID user_id, int plat_type, const char *reason)
{
	for (int i = 0; i < m_max_enemy_num; ++i)
	{
		if (m_enemy_list[i].user_id == user_id && m_enemy_list[i].plat_type == plat_type)
		{
			gamelog::g_log_friends.printf(LL_INFO, "[RemoveBlack Succ][user[%s %d] remove: role_id:%d plat_type:%d][Reason:%s]", 
				m_role->GetName(), UidToInt(m_role->GetUserId()), 
				UidToInt(m_enemy_list[i].user_id), m_enemy_list[i].plat_type, reason);

			m_enemy_list[i].user_id = INVALID_USER_ID;
			m_enemy_list[i].plat_type = -1;
			m_enemy_list[i].last_kill_time = 0;
			m_enemy_list[i].kill_count = 0;
			m_enemy_list[i].be_kill_count = 0;

			m_enemy_dirty_mark[i] = true;
			m_enemy_list_change = true;

			--m_enemy_num;

			SendEnemyChangeToRole(i, user_id, plat_type);

			return true;
		}
	}

	return false;
}

bool FriendManager::AddBlack(const UserID &user_id, const GameName gamename, char sex, char prof, const char *reason)
{
	if (m_role->GetSublock()->CheckHasLock()) return false;

	if (m_role->GetLoverUid() == UidToInt(user_id))
	{
		m_role->NoticeNum(errornum::EN_ADD_BLACK_LOVER_LIMIT);
		return false;
	}

	if (this->IsBlack(user_id))
	{
		m_role->NoticeNum(errornum::EN_IN_YOUR_BLACKLISTS);
		return false;
	}

	if (m_black_num >= MAX_BLACK_NUM)
	{
		m_role->NoticeNum(errornum::EN_BLACKLISTS_NUM_MAX);
		return false;
	}

	this->RemoveFriend(user_id, "Black Remove Friend");

	if (m_black_num >= m_max_black_num)
	{
		int list_size = (m_max_black_num + EXPAND_SIZE < MAX_BLACK_NUM) ? (m_max_black_num + EXPAND_SIZE) : MAX_BLACK_NUM;
		this->ResizeBlackList(list_size);
	}

	int index = -1;
	for (int i = 0; i < m_max_black_num; ++i)
	{
		if (m_black_list[i].user_id == INVALID_USER_ID)
		{
			index = i; 
			break;
		}
	}
	if (-1 == index) return false;

	m_black_list[index].user_id = user_id;
	F_STRNCPY(m_black_list[index].gamename, gamename, sizeof(m_black_list[0].gamename));
	m_black_list[index].sex = sex;
	m_black_list[index].prof = prof;

	m_black_dirty_mark[index] = true;
	m_black_list_is_change = true;
	++m_black_num;

	this->SendBlackChangeToRole(index, user_id);

	gamelog::g_log_friends.printf(LL_INFO, "[AddBlack Succ][user[%s %d] add: role_id:%d name:%s sex:%d prof:%d][Reason:%s]", 
		m_role->GetName(), UidToInt(m_role->GetUserId()),
		UidToInt(m_black_list[index].user_id), m_black_list[index].gamename, m_black_list[index].sex, m_black_list[index].prof, reason);

	return true;
}

bool FriendManager::RemoveBlack(const UserID &user_id, const char *reason)
{
	for (int i = 0; i < m_max_black_num; ++i)
	{
		if (m_black_list[i].user_id == user_id)
		{
			gamelog::g_log_friends.printf(LL_INFO, "[RemoveBlack Succ][user[%s %d] remove: role_id:%d name:%s sex:%d prof:%d][Reason:%s]", 
				m_role->GetName(), UidToInt(m_role->GetUserId()),
				UidToInt(m_black_list[i].user_id), m_black_list[i].gamename, m_black_list[i].sex, m_black_list[i].prof, reason);

			m_black_list[i].user_id = INVALID_USER_ID;
			memset(m_black_list[i].gamename, 0, sizeof(m_black_list[i].gamename));
			m_black_list[i].sex = FEMALE;
			m_black_list[i].prof = 0;

			m_black_dirty_mark[i] = true;
			m_black_list_is_change = true;
			--m_black_num;

			this->SendBlackChangeToRole(i, user_id);

			return true;
		}
	}

	return false;
}

bool FriendManager::IsBlack(const UserID &user_id)
{
	for (int i = 0; i < m_max_black_num; ++i)
	{
		if (m_black_list[i].user_id == user_id)
		{
			return true;
		}
	}

	return false;
}

void FriendManager::DecOfflinkLongTimeFriend()
{
	CommonDataParam	* common_data = m_role->GetCommonDataParam();
	if (NULL == common_data) return;

	if (0 == common_data->is_auto_dec_offline_friend)
	{
		return;
	}

	for (int i = 0; i < m_max_friend_num; ++i)
	{
		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(m_friend_list[i].user_id);
		if (NULL != role) continue;

		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(m_friend_list[i].user_id);
		if (NULL != user_node)
		{
			if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > user_node->last_logout_timestamp + DEC_FRIEND_OFFLINE_TIME * 24 * 60 * 60)
			{
				if (m_friend_list[i].intimacy < DEC_FRIEND_OFFLINE_INTIMACY)
				{
					this->RemoveFriend(m_friend_list[i].user_id, "DecOfflinkLongTimeFriend");
				}
			}
		}
	}
}

void FriendManager::SendFriendListToRole()
{
	CommonDataParam	* common_data = m_role->GetCommonDataParam();
	if (NULL == common_data) return;

	this->DecOfflinkLongTimeFriend();

	static Protocol::SCFriendInfoAck fia;
	fia.is_auto_dec_friend = common_data->is_auto_dec_offline_friend;
	int count = 0;

	for (int i = 0; i < m_max_friend_num && count < MAX_FRIEND_NUM; ++i)
	{
		if (m_friend_list[i].user_id == INVALID_USER_ID) continue;

		UserCacheNode *user_cache_node = UserCacheManager::Instance().GetUserNode(UidToInt(m_friend_list[i].user_id));
		if (NULL == user_cache_node) continue;

		bool is_online = UserCacheManager::Instance().IsUserOnline(m_friend_list[i].user_id);

		fia.friend_list[count].sex = user_cache_node->sex;
		fia.friend_list[count].prof = user_cache_node->prof;
		fia.friend_list[count].camp = user_cache_node->camp;
		fia.friend_list[count].is_online = (is_online ? 1 : 0);
		F_STRNCPY(fia.friend_list[count].gamename, user_cache_node->user_name, sizeof(fia.friend_list[0].gamename));
		fia.friend_list[count].user_id = UidToInt(m_friend_list[i].user_id);
		fia.friend_list[count].intimacy = m_friend_list[i].intimacy;
		fia.friend_list[count].level = user_cache_node->level;
		fia.friend_list[count].reserve_sh = 0;
		fia.friend_list[count].capability = user_cache_node->capability;
		fia.friend_list[count].avatar_timestamp = user_cache_node->avatar_timestamp;
		fia.friend_list[count].last_logout_timestamp = user_cache_node->last_logout_timestamp;
		fia.friend_list[count].gift_count = m_friend_list[i].gift_count;
		fia.friend_list[count].favorable_impression = m_friend_list[i].favorable_impression;
		fia.friend_list[count].day_chat_count = m_friend_list[i].day_chat_count;
		fia.friend_list[count].add_timestamp = (unsigned int)m_friend_list[i].add_timestamp;
		
		++count;
	}

	fia.count = count;

	int sendlen = sizeof(Protocol::SCFriendInfoAck) - (MAX_FRIEND_NUM - count) * sizeof(Protocol::FriendInfo);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&fia, sendlen);

	m_sent_to_client = true;
}

void FriendManager::SendEnemyListToRole()
{
	static Protocol::SCEnemyListACK ela;
	int count = 0;

	for (int i = 0; i < m_max_enemy_num && count < MAX_ENEMY_NUM; ++i)
	{
		if (m_enemy_list[i].user_id == INVALID_USER_ID) continue;

		//UserCacheNode *user_cache_node = UserCacheManager::Instance().GetUserNode(UidToInt(m_enemy_list[i].user_id));
		//if (NULL == user_cache_node) continue;

		//bool is_online = UserCacheManager::Instance().IsUserOnline(m_enemy_list[i].user_id);

		//ela.enemy_list[count].sex = user_cache_node->sex;
		//ela.enemy_list[count].prof = user_cache_node->prof;
		//ela.enemy_list[count].camp = user_cache_node->camp;
		//ela.enemy_list[count].is_online = (is_online ? 1 : 0);
		//ela.enemy_list[count].reserve_sh = 0;
		//F_STRNCPY(ela.enemy_list[count].gamename, user_cache_node->user_name, sizeof(ela.enemy_list[0].gamename));
		ela.enemy_list[count].kill_count = m_enemy_list[i].kill_count;
		ela.enemy_list[count].be_kill_count = m_enemy_list[i].be_kill_count;
		ela.enemy_list[count].last_kill_time = m_enemy_list[i].last_kill_time;
		//ela.enemy_list[count].user_id = UidToInt(m_enemy_list[i].user_id);
		ela.enemy_list[count].unique_user_id = UniqueUserIDToLongLong(UniqueUserID(m_enemy_list[i].plat_type, m_enemy_list[i].user_id));
		ela.enemy_list[count].index = i;
		//ela.enemy_list[count].level = user_cache_node->level;
		//ela.enemy_list[count].capability = user_cache_node->capability;
		//ela.enemy_list[count].avatar_timestamp = user_cache_node->avatar_timestamp;

		++count;
	}

	ela.count = count;

	int sendlen = sizeof(Protocol::SCEnemyListACK) - (MAX_ENEMY_NUM - count) * sizeof(Protocol::EnemyItem);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char *)&ela, sendlen);
}

void FriendManager::SendBlackListToRole() const
{
	static Protocol::SCBlacklistsACK scbl;
	int count = 0;

	for (int i = 0; i < m_max_black_num && count < MAX_BLACK_NUM; ++i)
	{
		if (m_black_list[i].user_id == INVALID_USER_ID) continue;	

		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(m_black_list[i].user_id);
		if (NULL == user_node)
		{
			continue;
		}

		scbl.blacklists[count].user_id = UidToInt(m_black_list[i].user_id);
		F_STRNCPY(scbl.blacklists[count].gamename, m_black_list[i].gamename, sizeof(scbl.blacklists[0].gamename));
		scbl.blacklists[count].sex = m_black_list[i].sex;
		scbl.blacklists[count].prof = m_black_list[i].prof;
		scbl.blacklists[count].level = user_node->level;
		scbl.blacklists[count].avatar_timestamp = user_node->avatar_timestamp;

		++count;
	}

	scbl.count = count;

	int sendlen = sizeof(Protocol::SCBlacklistsACK) - (MAX_BLACK_NUM - count) * sizeof(Protocol::BlacklistsItem);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&scbl, sendlen);
}

void FriendManager::GetFriendList(FriendListParam *friend_list_param) const
{
	int count = 0;

	for (int i = 0; i < m_max_friend_num && count < MAX_FRIEND_NUM; ++i)
	{
		if (m_friend_list[i].user_id != INVALID_USER_ID)
		{
			friend_list_param->friend_list[count].change_state = structcommon::CS_NONE;
			friend_list_param->friend_list[count].friend_uid = UidToInt(m_friend_list[i].user_id);
			friend_list_param->friend_list[count].intimacy = m_friend_list[i].intimacy;
			friend_list_param->friend_list[count].team_today_intimacy = m_friend_list[i].team_today_intimacy;
			friend_list_param->friend_list[count].gift_count = m_friend_list[i].gift_count;
			friend_list_param->friend_list[count].favorable_impression = m_friend_list[i].favorable_impression;
			friend_list_param->friend_list[count].day_chat_count = m_friend_list[i].day_chat_count;
			friend_list_param->friend_list[count].add_timestamp = m_friend_list[i].add_timestamp;
			friend_list_param->friend_list[count].index = i;

			++count;
		}
	}

	friend_list_param->count = count;
}

void FriendManager::GetBlackList(BlacklistsParam *black_list_param) const
{
	int count = 0;

	for (int i = 0; i < m_max_black_num && count < MAX_BLACK_NUM; ++i)
	{
		if (m_black_list[i].user_id != INVALID_USER_ID)
		{
			black_list_param->black_list[count].change_state = structcommon::CS_NONE;

			black_list_param->black_list[count].uid = UidToInt(m_black_list[i].user_id);
			F_STRNCPY(black_list_param->black_list[count].gamename, m_black_list[i].gamename, sizeof(black_list_param->black_list[0].gamename));
			black_list_param->black_list[count].sex = m_black_list[i].sex;
			black_list_param->black_list[count].prof = m_black_list[i].prof;

			black_list_param->black_list[count].index = i;

			++count;
		}
	}

	black_list_param->count = count;
}

void FriendManager::GetEnemyList(EnemyListParam *enemy_list_param) const
{
	int count = 0;

	for (int i = 0; i < m_max_enemy_num && count < MAX_ENEMY_NUM; ++i)
	{
		if (m_enemy_list[i].user_id != INVALID_USER_ID)
		{
			enemy_list_param->enemy_list[count].change_state = structcommon::CS_NONE;

			enemy_list_param->enemy_list[count].enemy_uid = UidToInt(m_enemy_list[i].user_id);
			enemy_list_param->enemy_list[count].index = i;
			enemy_list_param->enemy_list[count].enemy_plat_type = m_enemy_list[i].plat_type;
			enemy_list_param->enemy_list[count].kill_count = m_enemy_list[i].kill_count;
			enemy_list_param->enemy_list[count].last_kill_time = m_enemy_list[i].last_kill_time;
			enemy_list_param->enemy_list[count].be_kill_count = m_enemy_list[i].be_kill_count;

			++count;
		}
	}

	enemy_list_param->count = count;
}

void FriendManager::GetChangeFriendList(FriendListParam *friend_list_param) const
{
	if (!m_friend_list_is_change)
	{
		friend_list_param->count = 0;
		return;
	}

	int count = 0;

	for (int i = 0; i < m_max_friend_num && count < MAX_FRIEND_NUM; ++i)
	{
		if (!m_friend_dirty_mark[i]) continue;

		if (m_friend_list[i].user_id == INVALID_USER_ID)
		{
			if (!m_friend_old_state[i]) continue; 

			friend_list_param->friend_list[count].change_state = structcommon::CS_DELETE;
			friend_list_param->friend_list[count].index = i;

			friend_list_param->friend_list[count].friend_uid = 0;
			friend_list_param->friend_list[count].intimacy = 0;
			friend_list_param->friend_list[count].team_today_intimacy = 0;
			friend_list_param->friend_list[count].gift_count = 0;
			friend_list_param->friend_list[count].favorable_impression = 0;
			friend_list_param->friend_list[count].day_chat_count = 0;
			friend_list_param->friend_list[count].add_timestamp = 0;
		}
		else
		{
			friend_list_param->friend_list[count].change_state = (m_friend_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT);
			friend_list_param->friend_list[count].index = i;

			friend_list_param->friend_list[count].friend_uid = UidToInt(m_friend_list[i].user_id);
			friend_list_param->friend_list[count].intimacy = m_friend_list[i].intimacy;
			friend_list_param->friend_list[count].team_today_intimacy = m_friend_list[i].team_today_intimacy;
			friend_list_param->friend_list[count].gift_count = m_friend_list[i].gift_count;
			friend_list_param->friend_list[count].favorable_impression = m_friend_list[i].favorable_impression;
			friend_list_param->friend_list[count].day_chat_count = m_friend_list[i].day_chat_count;
			friend_list_param->friend_list[count].add_timestamp = m_friend_list[i].add_timestamp;
		}

		++count;
	}

	friend_list_param->count = count;
}

void FriendManager::GetChangeBlackList(BlacklistsParam *black_list_param) const
{
	if (!m_black_list_is_change)
	{
		black_list_param->count = 0;
		return;
	}

	int count = 0;

	for (int i = 0; i < m_max_black_num && count < MAX_BLACK_NUM; ++i)
	{
		if (!m_black_dirty_mark[i]) continue;

		if (m_black_list[i].user_id == INVALID_USER_ID)
		{
			if (!m_black_old_state[i]) continue; 

			black_list_param->black_list[count].change_state = structcommon::CS_DELETE;
			black_list_param->black_list[count].index = i;

			black_list_param->black_list[count].uid = UidToInt(m_black_list[i].user_id);
			memset(black_list_param->black_list[count].gamename, 0, sizeof(black_list_param->black_list[0].gamename));
			black_list_param->black_list[count].sex = 0;
			black_list_param->black_list[count].prof = 0;
		}
		else
		{
			black_list_param->black_list[count].change_state = m_black_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT;
			black_list_param->black_list[count].index = i;

			black_list_param->black_list[count].uid = UidToInt(m_black_list[i].user_id);
			F_STRNCPY(black_list_param->black_list[count].gamename, m_black_list[i].gamename, sizeof(black_list_param->black_list[0].gamename));
			black_list_param->black_list[count].sex = m_black_list[i].sex;
			black_list_param->black_list[count].prof = m_black_list[i].prof;
		}

		++count;
	}

	black_list_param->count = count;
}

void FriendManager::GetChangeEnemyList(EnemyListParam *enemy_list_param) const
{
	if (!m_enemy_list_change)
	{
		enemy_list_param->count = 0;
		return;
	}

	int count = 0;

	for (int i = 0; i < m_max_enemy_num && count < MAX_ENEMY_NUM; ++i)
	{
		if (!m_enemy_dirty_mark[i]) continue;

		if (m_enemy_list[i].user_id == INVALID_USER_ID)	
		{
			if (!m_enemy_old_state[i]) continue;

			enemy_list_param->enemy_list[count].change_state = structcommon::CS_DELETE;
			enemy_list_param->enemy_list[count].enemy_uid = 0;
			enemy_list_param->enemy_list[count].index = i;
			enemy_list_param->enemy_list[count].enemy_plat_type = -1;
			enemy_list_param->enemy_list[count].kill_count = 0;
			enemy_list_param->enemy_list[count].last_kill_time = 0;
			enemy_list_param->enemy_list[count].be_kill_count = 0;
		}
		else
		{
			enemy_list_param->enemy_list[count].change_state = m_enemy_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT;
			enemy_list_param->enemy_list[count].enemy_uid = UidToInt(m_enemy_list[i].user_id);
			enemy_list_param->enemy_list[count].index = i;
			enemy_list_param->enemy_list[count].enemy_plat_type = m_enemy_list[i].plat_type;
			enemy_list_param->enemy_list[count].kill_count = m_enemy_list[i].kill_count;
			enemy_list_param->enemy_list[count].last_kill_time = m_enemy_list[i].last_kill_time;
			enemy_list_param->enemy_list[count].be_kill_count = m_enemy_list[i].be_kill_count;
		}

		++count;
	}

	enemy_list_param->count = count;
}

void FriendManager::ClearDirtyMark()
{
	this->ClearFriendDirtyMark();
	this->ClearEnemyDirtyMark();
	this->ClearBlackDirtyMark();
}

void FriendManager::ClearFriendDirtyMark()
{
	if (m_friend_list_is_change)
	{
		memset(m_friend_dirty_mark, 0, sizeof(bool) * m_max_friend_num);
		memset(m_friend_old_state, 0, sizeof(bool) * m_max_friend_num);

		for (int i = 0; i < m_max_friend_num; ++i)
		{
			m_friend_old_state[i] = (m_friend_list[i].user_id != INVALID_USER_ID);
		}

		m_friend_list_is_change = false;
	}
}

void FriendManager::ClearEnemyDirtyMark()
{
	if (m_enemy_list_change)
	{
		memset(m_enemy_dirty_mark, 0, sizeof(bool) * m_max_enemy_num);
		memset(m_enemy_old_state, 0, sizeof(bool) * m_max_enemy_num);

		for (int i = 0; i < m_max_enemy_num; ++i )
		{
			m_enemy_old_state[i] = (m_enemy_list[i].user_id != INVALID_USER_ID);
		}

		m_enemy_list_change = false;
	}
}

void FriendManager::ClearBlackDirtyMark()
{
	if (m_black_list_is_change)
	{
		memset(m_black_dirty_mark, 0, sizeof(bool) * m_max_black_num);
		memset(m_black_old_state, 0, sizeof(bool) * m_max_black_num);

		for (int i = 0; i < m_max_black_num; ++i)
		{
			m_black_old_state[i] = (m_black_list[i].user_id != INVALID_USER_ID);
		}

		m_black_list_is_change = false;
	}
}


int FriendManager::GetMaxFriendNum()
{
	return GLOBALCONFIG->GetFriendMaxNumber(m_role->GetLevel());
}

int FriendManager::GetFriendIntimacy(const UserID &friend_user_id)
{
	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (m_friend_list[i].user_id == friend_user_id)
		{
			return m_friend_list[i].intimacy;
		}
	}

	return 0;
}

int FriendManager::GetFriendGiftCount(const UserID &friend_user_id)
{
	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (m_friend_list[i].user_id == friend_user_id)
		{
			return m_friend_list[i].gift_count;
		}
	}

	return 0;
}

void FriendManager::SetFriendGiftCount(const UserID &friend_user_id, int gift_count_value)
{
	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (m_friend_list[i].user_id == friend_user_id)
		{
			m_friend_list[i].gift_count += gift_count_value;
		}
	}
}

void FriendManager::ClearIntimacy(const UserID &user_id, const char *reason)
{
	if (NULL == reason) return;

	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (m_friend_list[i].user_id == user_id)
		{
			gamelog::g_log_friends.printf(LL_INFO, "[ClearIntimacy Succ][user[%s %d] other: role_id:%d intimacy_befor_clear:%d][Reason:%s]", 
				m_role->GetName(), UidToInt(m_role->GetUserId()),
				UidToInt(m_friend_list[i].user_id), m_friend_list[i].intimacy, reason);

			m_friend_list[i].intimacy = 0;

			m_friend_dirty_mark[i] = true;
			m_friend_list_is_change = true;

			this->SendFriendChangeToRole(i, user_id);

			return;
		}
	}
}

void FriendManager::SendFriendChangeToRole(int index, const UserID &user_id) const
{
	if (index < 0 || index >= m_max_friend_num || index >= MAX_FRIEND_NUM) return;

	Protocol::SCChangeFriend cf;

	if (m_friend_list[index].user_id == INVALID_USER_ID)
	{
		cf.changestate = Protocol::FRIEND_DELETE;

		cf.reserved = 0;
		cf.sex = FEMALE;
		cf.prof = 0;
		cf.gamename[0] = 0;
		cf.user_id = UidToInt(user_id);
		cf.intimacy = 0;
		cf.camp = 0;
		cf.is_online = 0;
		cf.level = 0;
		cf.capability = 0;
		cf.avatar_timestamp = 0;
		cf.last_logout_timestamp = 0;
		cf.gift_count = 0;
		cf.favorable_impression = 0;
		cf.day_chat_count = 0;
		cf.add_timestamp = 0;
	}
	else
	{
		cf.changestate = Protocol::FRIEND_UPDATE;

		cf.reserved = 0;
		cf.user_id = UidToInt(m_friend_list[index].user_id);
		cf.intimacy = m_friend_list[index].intimacy;
		cf.gift_count = m_friend_list[index].gift_count;
		cf.favorable_impression = m_friend_list[index].favorable_impression;
		cf.day_chat_count = m_friend_list[index].day_chat_count;
		cf.add_timestamp = (unsigned int)m_friend_list[index].add_timestamp;

		UserCacheNode *user_cache_node = UserCacheManager::Instance().GetUserNode(UidToInt(m_friend_list[index].user_id));
		if (NULL != user_cache_node)
		{
			bool is_online = UserCacheManager::Instance().IsUserOnline(m_friend_list[index].user_id);

			cf.sex = user_cache_node->sex;
			cf.prof = user_cache_node->prof;
			F_STRNCPY(cf.gamename, user_cache_node->user_name, sizeof(cf.gamename));
			cf.camp = user_cache_node->camp;
			cf.is_online = (is_online ? 1: 0);
			cf.level = user_cache_node->level;
			cf.capability = user_cache_node->capability;
			cf.avatar_timestamp = user_cache_node->avatar_timestamp;
			cf.last_logout_timestamp = user_cache_node->last_logout_timestamp;
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cf, sizeof(cf));
}

void FriendManager::SendEnemyChangeToRole(int index, const UserID & user_id, int plat_type) const
{
	if (index < 0 || index >= m_max_enemy_num) return;

	Protocol::SCChangeEnemy ce;
	ce.index = index;

	if (m_enemy_list[index].user_id == INVALID_USER_ID)
	{
		ce.changstate =	Protocol::ENEMY_DELETE;
		//ce.camp = 0;
		//ce.sex = FEMALE;
		//ce.prof = 0;
		//memset(ce.gamename, 0, sizeof(GameName));
		ce.kill_count = 0;
		ce.be_kill_count = 0;
		ce.last_kill_time = 0;
		//ce.user_id = UidToInt(user_id);
		ce.unique_user_id = UniqueUserIDToLongLong(UniqueUserID(plat_type, user_id));
		//ce.level = 0;
		//ce.is_online = 0;
		ce.reserve_sh = 0;
		//ce.capability = 0;
		//ce.avatar_timestamp = 0;
	}
	else
	{
		//bool is_online = UserCacheManager::Instance().IsUserOnline(m_enemy_list[index].user_id);

		ce.changstate = Protocol::ENEMY_UPDATE;
		ce.kill_count = m_enemy_list[index].kill_count;
		ce.be_kill_count = m_enemy_list[index].be_kill_count;
		//ce.user_id = UidToInt(m_enemy_list[index].user_id);
		ce.unique_user_id = UniqueUserIDToLongLong(UniqueUserID(m_enemy_list[index].plat_type, m_enemy_list[index].user_id));
		ce.last_kill_time = m_enemy_list[index].last_kill_time;
		//ce.is_online = (is_online ? 1 : 0);
		ce.reserve_sh = 0;
		//ce.avatar_timestamp = 0;

		//UserCacheNode *user_cache_node = UserCacheManager::Instance().GetUserNode(UidToInt(m_enemy_list[index].user_id));
		//if (NULL != user_cache_node)
		//{
		//	ce.sex = user_cache_node->sex;
		//	ce.prof = user_cache_node->prof;
		//	ce.level = user_cache_node->level;
		//	ce.camp = user_cache_node->camp;
		//	ce.capability = user_cache_node->capability;
		//	ce.avatar_timestamp = user_cache_node->avatar_timestamp;
		//	F_STRNCPY(ce.gamename, user_cache_node->user_name, sizeof(ce.gamename));
		//}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char *)&ce, sizeof(ce));
}

void FriendManager::SendBlackChangeToRole(int index, const UserID &user_id) const
{
	if (index < 0 || index >= m_max_black_num) return;

	Protocol::SCChangeBlacklist cb;
	cb.index = index;
	cb.reserved = 0;

	if (m_black_list[index].user_id == INVALID_USER_ID)
	{
		cb.changstate =	Protocol::BLACK_DELETE;
		cb.user_id = UidToInt(user_id);
		memset(cb.gamename, 0, sizeof(GameName));
		cb.sex = 0;
		cb.prof = 0;
	}
	else
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(m_black_list[index].user_id);

		cb.changstate = Protocol::BLACK_UPDATE;
		cb.user_id = UidToInt(m_black_list[index].user_id);
		F_STRNCPY(cb.gamename, m_black_list[index].gamename, sizeof(cb.gamename));
		cb.sex = m_black_list[index].sex;
		cb.prof = m_black_list[index].prof;	
		cb.level = user_node->level;
		cb.avatar_timestamp = user_node->avatar_timestamp;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char *)&cb, sizeof(cb));
}

void FriendManager::ResizeFriendList(int size)
{
	if (m_max_friend_num >= size || size > MAX_FRIEND_NUM) return;

	FriendItem *friend_list_new = (FriendItem *)(new ServerMsgMem[sizeof(FriendItem) * size]);
	bool *friend_dirty_mark_new = (bool *)(new ServerMsgMem[sizeof(bool) * size]);
	bool *friend_old_state_new = (bool *)(new ServerMsgMem[sizeof(bool) * size]);

	memset(friend_list_new, 0, sizeof(FriendItem) * size);
	memset(friend_dirty_mark_new, 0, sizeof(bool) * size);
	memset(friend_old_state_new, 0, sizeof(bool) * size);

	if (NULL != m_friend_list)
	{
		memcpy(friend_list_new, m_friend_list, sizeof(FriendItem) * m_max_friend_num);
		memcpy(friend_dirty_mark_new, m_friend_dirty_mark, sizeof(bool) * m_max_friend_num);
		memcpy(friend_old_state_new, m_friend_old_state, sizeof(bool) * m_max_friend_num);

		ServerMsgMem *tmp = (ServerMsgMem*)m_friend_list;
		delete []tmp;
		tmp = (ServerMsgMem*)m_friend_dirty_mark;
		delete []tmp;
		tmp = (ServerMsgMem*)m_friend_old_state;
		delete []tmp;
	}

	for (int i = m_max_friend_num; i < size; ++i)
	{
		friend_list_new[i].user_id = INVALID_USER_ID;
	}

	m_friend_list = friend_list_new;
	m_friend_dirty_mark = friend_dirty_mark_new;
	m_friend_old_state = friend_old_state_new;

	m_max_friend_num = size;
}

void FriendManager::ResizeBlackList(int size)
{
	if (m_max_black_num >= size || size > MAX_BLACK_NUM) return;

	BlackItem *black_list_new = (BlackItem *)(new ServerMsgMem[sizeof(BlackItem) * size]);
	bool *black_dirty_mark_new = (bool *)(new ServerMsgMem[sizeof(bool) * size]);
	bool *black_old_state_new = (bool *)(new ServerMsgMem[sizeof(bool) * size]);

	memset(black_list_new, 0, sizeof(BlackItem) * size);
	memset(black_dirty_mark_new, 0, sizeof(bool) * size);
	memset(black_old_state_new, 0, sizeof(bool) * size);

	if (NULL != m_black_list)
	{
		memcpy(black_list_new, m_black_list, sizeof(BlackItem) * m_max_black_num);
		memcpy(black_dirty_mark_new, m_black_dirty_mark, sizeof(bool) * m_max_black_num);
		memcpy(black_old_state_new, m_black_old_state, sizeof(bool) * m_max_black_num);

		ServerMsgMem *tmp = (ServerMsgMem*)m_black_list;
		delete []tmp;
		tmp = (ServerMsgMem*)m_black_dirty_mark;
		delete []tmp;
		tmp = (ServerMsgMem*)m_black_old_state;
		delete []tmp;
	}

	for (int i = m_max_black_num; i < size; ++i)
	{
		black_list_new[i].user_id = INVALID_USER_ID;
	}

	m_black_list = black_list_new;
	m_black_dirty_mark = black_dirty_mark_new;
	m_black_old_state = black_old_state_new;

	m_max_black_num = size;
}

void FriendManager::ResizeEnemyList(int size)
{
	if (m_max_enemy_num >= size || size > MAX_ENEMY_NUM) return;

	EnemyItem *enemy_list_new = (EnemyItem *)(new ServerMsgMem[sizeof(EnemyItem) * size]);
	bool *enemy_dirty_mark_new = (bool *)(new ServerMsgMem[sizeof(bool) * size]);
	bool *enemy_old_state_new = (bool *)(new ServerMsgMem[sizeof(bool) * size]);

	memset(enemy_list_new, 0, sizeof(EnemyItem) * size);
	memset(enemy_dirty_mark_new, 0, sizeof(bool) * size);
	memset(enemy_old_state_new, 0, sizeof(bool) * size);

	if (NULL != m_enemy_list)
	{
		memcpy(enemy_list_new, m_enemy_list, sizeof(EnemyItem) * m_max_enemy_num);
		memcpy(enemy_dirty_mark_new, m_enemy_dirty_mark, sizeof(bool) * m_max_enemy_num);
		memcpy(enemy_old_state_new, m_enemy_old_state, sizeof(bool) * m_max_enemy_num);

		ServerMsgMem *tmp = (ServerMsgMem*)m_enemy_list;
		delete []tmp;
		tmp = (ServerMsgMem*)m_enemy_dirty_mark;
		delete []tmp;
		tmp = (ServerMsgMem*)m_enemy_old_state;
		delete []tmp;
	}

	for (int i = m_max_enemy_num; i < size; ++i)
	{
		enemy_list_new[i].user_id = INVALID_USER_ID;
		enemy_list_new[i].plat_type = -1;
	}

	m_enemy_list = enemy_list_new;
	m_enemy_dirty_mark = enemy_dirty_mark_new;
	m_enemy_old_state = enemy_old_state_new;

	m_max_enemy_num = size;
}

bool FriendManager::IsFriendFull()
{
	int MAX_FRIEND_NUM = this->GetMaxFriendNum();
	return m_friend_num >= MAX_FRIEND_NUM;
}

void FriendManager::FriendSave()
{
	static FriendListParam friends;
	static BlacklistsParam blacks;

	friends.count = 0; blacks.count = 0;
	this->GetChangeFriendList(&friends);
	this->GetChangeBlackList(&blacks);

	if (friends.count > 0 || blacks.count > 0)
	{
		RMIFriendSaveBackObject *rsbo = new RMIFriendSaveBackObject();
		rsbo->log_user_id = m_role->GetUserId();

		RMIRoleClient rc;
		rc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());
		bool ret = rc.FriendSaveAsyn(UidToInt(m_role->GetUserId()), friends, blacks, rsbo);
		if (ret)
		{
			this->ClearFriendDirtyMark();
			this->ClearBlackDirtyMark();
		}
	}
}

void FriendManager::CheckClearAddCharm()
{
	unsigned int &next_clear_time = m_role->GetCommonDataParam()->next_clear_add_charm_time;
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_sec >= next_clear_time)
	{
		m_role->GetCommonDataParam()->add_charm_for_rank = 0;
		const CrossRankRewardDateCfg *date_cfg = CrossRankRewardConfig::Instance().GetDateCfg(CROSS_PERSON_RANK_TYPE_ADD_CHARM);
		if (nullptr != date_cfg)
		{
			const tm *tm_now = EngineAdapter::Instance().LocalTime();
			if (nullptr != tm_now)
			{
				int day_count = 0;
				for (int i = tm_now->tm_wday + 1; i < DAY_PER_WEEK; ++i)
				{
					if (date_cfg->CanReward(i))
					{
						day_count = i - tm_now->tm_wday;
						break;
					}
				}

				if (0 == day_count)
				{
					for (int i = 0; i <= tm_now->tm_wday; ++i)
					{
						if (date_cfg->CanReward(i))
						{
							day_count = i + DAY_PER_WEEK - tm_now->tm_wday;
							break;
						}
					}
				}

				next_clear_time = now_sec + EngineAdapter::Instance().NextDayInterval(0, 0, 0) + (day_count - 1) * SECOND_PER_DAY;
			}
		}
	}
}

int FriendManager::GetFriendUidList(int * friend_list_uid, int length)
{
	int count = 0;

	if (friend_list_uid == nullptr) return count;

	for (int i = 0; (i < m_friend_num && i < MAX_FRIEND_NUM) &&
		(count < MAX_FRIEND_NUM && count < length); ++i)
	{
		if (m_friend_list[i].user_id != INVALID_USER_ID)
		{
			friend_list_uid[count] = UidToInt(m_friend_list[i].user_id);
			++count;
		}
	}

	return count;
}

int FriendManager::GetFavorableImpression(const UserID & friend_user_id)
{
	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (m_friend_list[i].user_id == friend_user_id)
		{
			return m_friend_list[i].favorable_impression;
		}
	}
	return 0;
}

bool FriendManager::AddFavorableImpression(const UserID & friend_user_id)
{
	const FriendOtherCfg &other_cfg = LOGIC_CONFIG->GetFriendConfig().GetOtherCfg();
	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (m_friend_list[i].user_id == friend_user_id)
		{
			if (m_friend_list[i].day_chat_count >= other_cfg.max_day_favorable_impression)
			{
				return false;
			}
			++m_friend_list[i].day_chat_count;
			m_friend_list[i].favorable_impression += other_cfg.add_favorable_impression;
			if (m_friend_list[i].favorable_impression > other_cfg.max_favorable_impression)
			{
				m_friend_list[i].favorable_impression = other_cfg.max_favorable_impression;
			}
			m_friend_dirty_mark[i] = true;
			m_friend_list_is_change = true;
			this->SendFriendChangeToRole(i, friend_user_id);
			return true;
		}
	}
	return false;
}

bool FriendManager::SetFavorableImpression(const UserID & friend_user_id, int number)
{
	const FriendOtherCfg &other_cfg = LOGIC_CONFIG->GetFriendConfig().GetOtherCfg();
	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (m_friend_list[i].user_id == friend_user_id)
		{
			/*if (m_friend_list[i].day_chat_count >= other_cfg.max_day_favorable_impression)
			{
				return false;
			}
			++m_friend_list[i].day_chat_count;*/
			m_friend_list[i].favorable_impression = number;
			if (m_friend_list[i].favorable_impression > other_cfg.max_favorable_impression)
			{
				m_friend_list[i].favorable_impression = other_cfg.max_favorable_impression;
			}
			m_friend_dirty_mark[i] = true;
			m_friend_list_is_change = true;
			this->SendFriendChangeToRole(i, friend_user_id);
			return true;
		}
	}
	return false;
}

int FriendManager::GetDayChatCount(const UserID & friend_user_id)
{
	for (int i = 0; i < m_max_friend_num; ++i)
	{
		if (m_friend_list[i].user_id == friend_user_id)
		{
			return m_friend_list[i].day_chat_count;
		}
	}
	return 0;
}

