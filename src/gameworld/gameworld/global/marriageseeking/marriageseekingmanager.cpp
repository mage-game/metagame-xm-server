#include "marriageseekingmanager.h"

#include "global/offlineregister/offlineregistermanager.hpp"
#include "obj/character/role.h"
#include "world.h"
#include "global/usercache/usercache.hpp"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"

MarriageSeekingManager::MarriageSeekingManager() 
{
}

MarriageSeekingManager::~MarriageSeekingManager()
{

}

MarriageSeekingManager & MarriageSeekingManager::Instance()
{
	static MarriageSeekingManager _instance; 
	return _instance;
}

void MarriageSeekingList::InsertSeeking(Role *role, MarriageSeekingNotice seeking_notice)
{
	if (m_curour < 0 || m_curour >= MARRIAGE_SEEKING_MAX_COUNT) return;

	for (int i = 0; i < m_seeking_count && i < MARRIAGE_SEEKING_MAX_COUNT; i++)
	{
		if (role->GetUID() == m_seeking_list[i].uid)
		{
			F_STRNCPY(m_seeking_list[i].seeking_notice, seeking_notice, sizeof(m_seeking_list[i].seeking_notice));
			m_seeking_list[i].timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			return;
		}
	}

	{
		MarriageSeeking &seeking = m_seeking_list[m_curour];

		seeking.uid = role->GetUID();
		F_STRNCPY(seeking.seeking_notice, seeking_notice, sizeof(seeking.seeking_notice));
		seeking.timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}

	++ m_curour;
	m_curour = m_curour % MARRIAGE_SEEKING_MAX_COUNT;

	++ m_seeking_count;
	if (m_seeking_count >= MARRIAGE_SEEKING_MAX_COUNT) m_seeking_count = MARRIAGE_SEEKING_MAX_COUNT;
}

void MarriageSeekingList::RemoveSeeking(int uid)
{
	if (m_seeking_count <= 0) return;

	int start_cursor = (m_curour + MARRIAGE_SEEKING_MAX_COUNT - m_seeking_count) % MARRIAGE_SEEKING_MAX_COUNT;

	for (int i = 0; i < m_seeking_count; ++ i)
	{
		int query_cursor = (start_cursor + i) % MARRIAGE_SEEKING_MAX_COUNT;

		if (m_seeking_list[query_cursor].uid == uid)
		{
			int swap_cursor = (m_curour + MARRIAGE_SEEKING_MAX_COUNT - 1) % MARRIAGE_SEEKING_MAX_COUNT;
			m_seeking_list[query_cursor] = m_seeking_list[swap_cursor];
			m_curour = swap_cursor;
			-- m_seeking_count;

			break;
		}
	}
}

void MarriageSeekingManager::MarriageSeekingOperateType(Role *role, int type, MarriageSeekingNotice marriage_seeking_notice)
{
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	switch (type)
// 	{
// 	case MARRIAGE_SEEKING_TYPE_INFO:
// 		{
// 			this->SendMarriageSeekingNoticeList(role);
// 		}
// 		break;
// 
// 	case MARRIAGE_SEEKING_TYPE_INSERT:
// 		{
// 			this->InsertMarriageSeekingNotice(role,marriage_seeking_notice);
// 		}
// 		break;
// 	}
}

void MarriageSeekingManager::InsertMarriageSeekingNotice(Role *role, MarriageSeekingNotice seeking_notice)
{	
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	if (role->IsMarry())  // 结婚人的不能再征婚
// 	{
// 		role->NoticeNum(errornum::EN_MARRIAGE_SEEKING_MARRIED);
// 		return;
// 	}
// 
// 	MarriageSeekingList *seeklist = &m_girls_seeking_list;
// 	
// 	int sex = role->GetSex();
// 	if (MALE == sex)
// 	{
// 		seeklist = &m_boys_seeking_list;
// 	}
// 
// 	seeklist->InsertSeeking(role, seeking_notice);
// 	
// 	role->NoticeNum(errornum::EN_MARRIAGE_SEEKING_SUCCESS);
// 
// 	{
// 		// 传闻
// 		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_marry_seeking_notice, 
// 			role->GetUID(), role->GetName(), (int)role->GetCamp(), role->GetUID(), role->GetName(), role->GetSex());
// 		if (length > 0) 
// 		{
// 			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER);
// 		}
// 	}
// 
// 	this->SendMarriageSeekingNoticeList(role);
}

void MarriageSeekingManager::SendMarriageSeekingNoticeList(Role *role)
{
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	if (role->IsMarry()) // 结婚人的不能再征婚
// 	{
// 		return;
// 	}
// 
// 	MarriageSeekingList *seeklist = &m_girls_seeking_list;
// 	if (FEMALE == role->GetSex())
// 	{
// 		seeklist = &m_boys_seeking_list;
// 	}
// 	
// 	static Protocol::SCMarriageSeekingInfo msi;
// 
// 	int count = 0;
// 
// 	for (int i = 0; i < seeklist->m_seeking_count && count < MARRIAGE_SEEKING_MAX_COUNT; ++ i)
// 	{
// 		if (0 == seeklist->m_seeking_list[i].uid) continue;
// 
// 		UserCacheNode *user_cache_node = UserCacheManager::Instance().GetUserNode(seeklist->m_seeking_list[i].uid);
// 		if (NULL == user_cache_node) continue;
// 
// 		bool is_online = UserCacheManager::Instance().IsUserOnline(seeklist->m_seeking_list[i].uid);
// 		
// 		msi.marriage_seeking_list[count].user_id = user_cache_node->uid;
// 		msi.marriage_seeking_list[count].camp = user_cache_node->camp;
// 		msi.marriage_seeking_list[count].sex = user_cache_node->sex;
// 		msi.marriage_seeking_list[count].prof = user_cache_node->prof;
// 		msi.marriage_seeking_list[count].is_online = is_online;
// 		msi.marriage_seeking_list[count].level = user_cache_node->level;
// 		msi.marriage_seeking_list[count].timestamp = seeklist->m_seeking_list[i].timestamp;
// 		F_STRNCPY(msi.marriage_seeking_list[count].gamename, user_cache_node->user_name, sizeof(msi.marriage_seeking_list[0].gamename));
// 		F_STRNCPY(msi.marriage_seeking_list[count].marriage_seeking_notice, seeklist->m_seeking_list[i].seeking_notice, sizeof(msi.marriage_seeking_list[0].marriage_seeking_notice));
// 
// 		++count;
// 	}
// 
// 	msi.count = count;
// 
// 	int sendlen = sizeof(Protocol::SCMarriageSeekingInfo) - (MARRIAGE_SEEKING_MAX_COUNT - count) * sizeof(Protocol::MarriageSeekingInfo);
// 	EngineAdapter::Instance().NetSend(role->GetNetId(), (char *)&msi, sendlen);
}

void MarriageSeekingManager::OnChangeSex(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	MarriageSeekingList *handle_list = NULL;
	if (FEMALE == role->GetSex())
	{
		handle_list = &m_boys_seeking_list;
	}
	else
	{
		handle_list = &m_girls_seeking_list;
	}

	if (NULL != handle_list)
	{
		handle_list->RemoveSeeking(role->GetUID());
	}
}

void MarriageSeekingManager::OnMarry(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	MarriageSeekingList *handle_list = NULL;
	if (FEMALE == role->GetSex())
	{
		handle_list = &m_girls_seeking_list;
	}
	else
	{
		handle_list = &m_boys_seeking_list;
	}

	if (NULL != handle_list)
	{
		handle_list->RemoveSeeking(role->GetUID());
	}
}




