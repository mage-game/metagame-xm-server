#include "topranker.hpp"

#include "engineadapter.h"
#include "internalcomm.h"
#include "servercommon/string/globalstr.h"

#include "world.h"
#include "rankmanager.hpp"
#include "gamelog.h"
#include <set>
#include "gameworld/global/usercache/usercache.hpp"
#include "config/logicconfigmanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

#include "other/achieve/title/titleconfig.hpp"
#include "other/achieve/title/titlemanager.hpp"

TopRanker::TopRanker() : m_init_top_finish(false), m_last_update_time(0), m_top_cap_user_login_timestamp(0), m_top_cap_user_id(INVALID_USER_ID), m_top_meili_user_login_timestamp(0), m_top_meili_user_id(INVALID_USER_ID), m_the_first_ten_cap(0){
	memset(m_rank_type_map, 0, sizeof(m_rank_type_map));

	UNSTD_STATIC_CHECK(33 == RANK_TOP_TYPE_MAX);

	m_rank_type_map[RANK_TOP_TYPE_CAPABILITY] = PERSON_RANK_TYPE_CAPABILITY_ALL;
	m_rank_type_map[RANK_TOP_TYPE_LEVEL] = PERSON_RANK_TYPE_LEVEL;
	m_rank_type_map[RANK_TOP_TYPE_XIANNV] = PERSON_RANK_TYPE_XIANNV_CAPABILITY;
	m_rank_type_map[RANK_TOP_TYPE_CAPABILITY_CAMP_1] = PERSON_RANK_TYPE_CAPABILITY_CAMP_1;
	m_rank_type_map[RANK_TOP_TYPE_CAPABILITY_CAMP_2] = PERSON_RANK_TYPE_CAPABILITY_CAMP_2;
	m_rank_type_map[RANK_TOP_TYPE_CAPABILITY_CAMP_3] = PERSON_RANK_TYPE_CAPABILITY_CAMP_3;
	m_rank_type_map[RANK_TOP_TYPE_CHARM_MALE] = PERSON_RANK_TYPE_CHARM_MALE;
	m_rank_type_map[RANK_TOP_TYPE_CHARM_FEMALE] = PERSON_RANK_TYPE_CHARM_FEMALE;
	m_rank_type_map[RANK_TOP_TYPE_QUNXIANLUANDOU] = PERSON_RANK_TYPE_QUNXIANLUANDOU;
	m_rank_type_map[RANK_TOP_TYPE_ZHANSHENDIAN] = PERSON_RANK_TYPE_ZHANSHENDIAN;
	m_rank_type_map[RANK_TOP_TYPE_KILL_WORLDBOSS] = PERSON_RANK_TYPE_KILL_WORLDBOSS;
	m_rank_type_map[RANK_TOP_TYPE_RED_PAPER_CONSUME_GOLD] = PERSON_RANK_TYPE_RED_PAPER_CONSUME_GOLD;
	m_rank_type_map[RANK_TOP_TYPE_RED_PAPER_LUCKY_VALUE] = PERSON_RANK_TYPE_RED_PAPER_LUCKY_VALUE;
	m_rank_type_map[RANK_TOP_TYPE_MOUNT] = PERSON_RANK_TYPE_MOUNT;
	m_rank_type_map[RANK_TOP_TYPE_WING] = PERSON_RANK_TYPE_WING;
	m_rank_type_map[RANK_TOP_TYPE_HALO] = PERSON_RANK_TYPE_HALO;
	m_rank_type_map[RANK_TOP_TYPE_SHENGONG] = PERSON_RANK_TYPE_SHENGONG;
	m_rank_type_map[RANK_TOP_TYPE_SHENYI] = PERSON_RANK_TYPE_SHENYI;
	m_rank_type_map[RANK_TOP_TYPE_EQUIPT_STRENGTH_LEVEL] = PERSON_RANK_TYPE_EQUIP_STRENGTH_LEVEL;
	m_rank_type_map[RANK_TOP_TYPE_STONE_TOTAL_LEVEL] = PERSON_RANK_TYPE_STONE_TOATAL_LEVEL;
	m_rank_type_map[RANK_TOP_TYPE_FIGHT_MOUNT] = PERSON_RANK_TYPE_FIGHT_MOUNT;
	m_rank_type_map[RANK_TOP_TYPE_CHARM] = PERSON_RANK_TYPE_CHARM;
	m_rank_type_map[RANK_TOP_TYPE_WORLD_RIGHT_ANSWER] = PERSON_RANK_TYPE_WORLD_RIGHT_ANSWER;
	m_rank_type_map[RANK_TOP_TYPE_JINGLING] = PERSON_RANK_TYPE_CAPABILITY_JINGLING;
	m_rank_type_map[RANK_TOP_TYPE_DAY_CHARM] = PERSON_RANK_TYPE_DAY_CHARM;
	m_rank_type_map[RANK_TOP_TYPE_QILINBI] = PERSON_RANK_TYPE_QILINBI;
	m_rank_type_map[RANK_TOP_TYPE_TOUSHI] = PERSON_RANK_TYPE_TOUSHI;
	m_rank_type_map[RANK_TOP_TYPE_YAOSHI] = PERSON_RANK_TYPE_YAOSHI;
	m_rank_type_map[RANK_TOP_TYPE_LINGTONG] = PERSON_RANK_TYPE_LINGTONG;
	m_rank_type_map[RANK_TOP_TYPE_LINGGONG] = PERSON_RANK_TYPE_LINGGONG;
	m_rank_type_map[RANK_TOP_TYPE_LINGQI] = PERSON_RANK_TYPE_LINGQI;
	m_rank_type_map[RANK_TOP_TYPE_FlYPET] = PERSON_RANK_TYPE_FEICHONG;
	m_rank_type_map[RANK_TOP_TYPE_WEIYAN] = PERSON_RANK_TYPE_WEIYAN;
}

TopRanker::~TopRanker()
{

}

TopRanker & TopRanker::Instance()
{
	static TopRanker tr;
	return tr;
}

void TopRanker::Init(const TopRankParam &top_param)
{
	memcpy(m_all_top_rank_list, top_param.all_top_rank_list, sizeof(m_all_top_rank_list));

	m_init_top_finish = true;
}

void TopRanker::GetInitParam(TopRankParam *top_param)
{
	UNSTD_STATIC_CHECK(sizeof(m_all_top_rank_list) == sizeof(top_param->all_top_rank_list));

	memcpy(top_param->all_top_rank_list, m_all_top_rank_list, sizeof(m_all_top_rank_list));
}

void TopRanker::OnEnterScene(Role *role)
{

}

void TopRanker::Update(unsigned long interval, time_t now_second)
{
	if (0 == m_last_update_time) m_last_update_time = now_second;

	if (now_second > m_last_update_time + 60 && !CrossConfig::Instance().IsHiddenServer())
	{
		m_last_update_time = now_second;

		for (int i = RANK_TOP_TYPE_MIN; i < RANK_TOP_TYPE_MAX; ++ i)
		{
			if (this->IsRealTimeRank(i))
			{
				this->DoOneRank(i);
			}
		}

		if (TOP_TEN >= 10 && 0 != m_all_top_rank_list[RANK_TOP_TYPE_CAPABILITY].uid_list[9])
		{
			UserCacheNode *user_cache_node = UserCacheManager::Instance().GetUserNode(m_all_top_rank_list[RANK_TOP_TYPE_CAPABILITY].uid_list[9]);
			if (NULL != user_cache_node)
			{
				m_the_first_ten_cap = user_cache_node->capability;
			}
		}
	}

	if (m_top_cap_user_login_timestamp > 0 && INVALID_USER_ID != m_top_cap_user_id)
	{
		if (EngineAdapter::Instance().Time() > m_top_cap_user_login_timestamp + 1)
		{
			if (UidToInt(m_top_cap_user_id) == m_all_top_rank_list[RANK_TOP_TYPE_CAPABILITY].uid_list[0])
			{
				Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(m_top_cap_user_id);
				if (NULL != role)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_toprank_capability_user_login,
						role->GetUID(), role->GetName(), (int)role->GetCamp());

					if (length > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
					}
				}
			}
			else
			{
				Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(m_top_cap_user_id);
				if (NULL != role)
				{
					static const int BROADCAST_CAP_RANK = 3;
					for (int i = 1; i < BROADCAST_CAP_RANK; i++)
					{
						if (UidToInt(m_top_cap_user_id) == m_all_top_rank_list[RANK_TOP_TYPE_CAPABILITY].uid_list[i])
						{
							int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_toprank_ten_capability_user_login, i+1, role->GetName());

							if (length > 0)
							{
								World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
							}

							break;
						}
					}
				}
			}

			m_top_cap_user_login_timestamp = 0;
			m_top_cap_user_id = INVALID_USER_ID;
		}
	}

	if (m_top_meili_user_login_timestamp > 0 && INVALID_USER_ID != m_top_meili_user_id)
	{
		if (EngineAdapter::Instance().Time() > m_top_meili_user_login_timestamp + 1)
		{
			if (UidToInt(m_top_meili_user_id) == m_all_top_rank_list[RANK_TOP_TYPE_CHARM].uid_list[0] )
			 {
				Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(m_top_meili_user_id);
				if (NULL != role)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_meilirank_capability_user_login,
						role->GetUID(), role->GetName(), (int)role->GetCamp());

					if (length > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
					}
				}
			}

			m_top_meili_user_login_timestamp = 0;
			m_top_meili_user_id = INVALID_USER_ID;
		}
	}
}

void TopRanker::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->DoRank();
	}
}

void TopRanker::OnWeekChange()
{
	for (int i = RANK_TOP_TYPE_MIN; i < RANK_TOP_TYPE_MAX; ++ i)
	{
		if (this->IsWeekRank(i))
		{
			this->DoOneRank(i);
		}
	}

	RankManager::Instance().ClearPersonRankType(PERSON_RANK_TYPE_KILL_WORLDBOSS); // 击杀世界boss的排行榜需要每周清0
}

bool TopRanker::DoRank()
{
	static TopUserIDList tmp_top_rank_list[RANK_TOP_TYPE_MAX];
	memset(tmp_top_rank_list, 0, sizeof(tmp_top_rank_list));

	std::set<int> top_uid_two_day_set;

	{
		// 加入昨天各个榜的top们
		for (int i = RANK_TOP_TYPE_MIN; i < RANK_TOP_TYPE_MAX; ++i)
		{
			if (this->IsRealTimeRank(i) || this->IsWeekRank(i)) continue;	

			for (int j = 0; j < TOP_TEN; ++j)
			{
				if (0 != m_all_top_rank_list[i].uid_list[j])
				{
					top_uid_two_day_set.insert(m_all_top_rank_list[i].uid_list[j]);
				}			
			}
		}
	}

	{
		// 排top10
		for (int top_type = RANK_TOP_TYPE_MIN; top_type < RANK_TOP_TYPE_MAX; ++top_type)
		{
			if (this->IsRealTimeRank(top_type) || this->IsWeekRank(top_type)) continue;

			RankManager::Instance().GetPersonRank()->GetRankTopUser(m_rank_type_map[top_type], TOP_TEN, tmp_top_rank_list[top_type].user_id_list);

			this->Broadcast(top_type, m_all_top_rank_list[top_type].uid_list[0], UidToInt(tmp_top_rank_list[top_type].user_id_list[0]));

			for (int k = 0; k < TOP_TEN; ++k)
			{
				m_all_top_rank_list[top_type].uid_list[k] = UidToInt(tmp_top_rank_list[top_type].user_id_list[k]);
			}

			UNSTD_STATIC_CHECK(10 == TOP_TEN);

			gamelog::g_log_rank.printf(LL_INFO, 
				"TopRanker::DoRank top_rank_type:%d, uid_list:%d, %d, %d, %d, %d, %d, %d, %d, %d, %d", 
				top_type, 
				m_all_top_rank_list[top_type].uid_list[0],
				m_all_top_rank_list[top_type].uid_list[1],
				m_all_top_rank_list[top_type].uid_list[2],
				m_all_top_rank_list[top_type].uid_list[3],
				m_all_top_rank_list[top_type].uid_list[4],
				m_all_top_rank_list[top_type].uid_list[5],
				m_all_top_rank_list[top_type].uid_list[6],
				m_all_top_rank_list[top_type].uid_list[7],
				m_all_top_rank_list[top_type].uid_list[8],
				m_all_top_rank_list[top_type].uid_list[9]);
		}
	}

	{
		// 加入今天各个榜的top们
		for (int i = RANK_TOP_TYPE_MIN; i < RANK_TOP_TYPE_MAX; ++i)
		{
			if (this->IsRealTimeRank(i) || this->IsWeekRank(i)) continue;

			for (int j = 0; j < TOP_TEN; ++j)
			{
				if (0 != m_all_top_rank_list[i].uid_list[j])
				{
					top_uid_two_day_set.insert(m_all_top_rank_list[i].uid_list[j]);
				}			
			}
		}
	}

	{
		// 给可能变化的top们发通知
		for (std::set<int>::const_iterator iter = top_uid_two_day_set.begin(); top_uid_two_day_set.end() != iter; ++iter)
		{
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(*iter));
			if (NULL == role) continue;

			PersonRankInfo personrank_info;
			this->GetTopRankInfo(role->GetUID(), &personrank_info);
			role->GetTitleManager()->OnTopRankInfo(personrank_info, SYNC_REASON_RANK);
		}
	}

	return true;
}

bool TopRanker::DoOneRank(int top_rank_type)
{
	if (top_rank_type < RANK_TOP_TYPE_MIN || top_rank_type >= RANK_TOP_TYPE_MAX) return false;

	static TopUserIDList top_user_idlist;
	memset(&top_user_idlist, 0, sizeof(top_user_idlist));

	int old_top_uid = m_all_top_rank_list[top_rank_type].uid_list[0];

	RankManager::Instance().GetPersonRank()->GetRankTopUser(m_rank_type_map[top_rank_type], TOP_TEN, top_user_idlist.user_id_list);
	
	std::set<int> top_uid_set;

	for (int i = 0; i < TOP_TEN; ++i)
	{
		int new_user_id = UidToInt(top_user_idlist.user_id_list[i]);

		if (new_user_id != m_all_top_rank_list[top_rank_type].uid_list[i])
		{
			int old_top_user_id = m_all_top_rank_list[top_rank_type].uid_list[0];

			top_uid_set.insert(new_user_id);
			top_uid_set.insert(m_all_top_rank_list[top_rank_type].uid_list[i]);

			m_all_top_rank_list[top_rank_type].uid_list[i] = 0;

			const PersonRankItem *rank_item =  RankManager::Instance().GetPersonRank()->GetPersonRankItem(m_rank_type_map[top_rank_type], IntToUid(new_user_id));
			if (NULL != rank_item && rank_item->level >= PERSON_RANK_LEVEL_LIMIT)
			{
				if (0 == i)
				{
					this->Broadcast(top_rank_type, old_top_user_id, new_user_id);
				}

				m_all_top_rank_list[top_rank_type].uid_list[i] = new_user_id;
			}
		}
	}

	for (std::set<int>::const_iterator iter = top_uid_set.begin(); top_uid_set.end() != iter; ++iter)
	{
		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(*iter));
		if (NULL == role) continue;

		PersonRankInfo personrank_info;

		this->GetTopRankInfo(role->GetUID(),  &personrank_info);
		role->GetTitleManager()->OnTopRankInfo(personrank_info, SYNC_REASON_RANK);
	}

	switch (top_rank_type)
	{
	case RANK_TOP_TYPE_CAPABILITY:
		{
			int new_top_uid = m_all_top_rank_list[RANK_TOP_TYPE_CAPABILITY].uid_list[0];

			if (old_top_uid > 0 && new_top_uid > 0 && old_top_uid != new_top_uid)
			{
				UserCacheNode *old_user = UserCacheManager::Instance().GetUserNode(old_top_uid);
				UserCacheNode *new_user = UserCacheManager::Instance().GetUserNode(new_top_uid);

				if (NULL != old_user && NULL != new_user )
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_toprank_capability_top_user_changed_notice,
						new_user->uid, new_user->user_name, (int)new_user->camp, old_user->uid, old_user->user_name, (int)old_user->camp);

					if (length > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
					}
				}
			}
		}
		break;
	}

	return true;
}

int TopRanker::GetTopRankUid(int rank_type, int rank)
{
	if (rank_type >= RANK_TOP_TYPE_MIN && rank_type < RANK_TOP_TYPE_MAX)
	{
		if (rank >= 0 && rank < TOP_TEN)
		{
			return m_all_top_rank_list[rank_type].uid_list[rank];
		}
	}

	return -1;
}

void TopRanker::Broadcast(int top_type, int old_uid, int now_uid)
{
	if (top_type < RANK_TOP_TYPE_MIN || top_type >= RANK_TOP_TYPE_MAX || 0 == now_uid) return;

	int str_len = 0;

	GameName now_user_name;
	char now_user_sex = 0, now_user_camp = 0;
	int now_user_level = 0;
	if (!RankManager::Instance().GetPersonRank()->GetRankUserInfo(m_rank_type_map[top_type], IntToUid(now_uid), now_user_name, &now_user_sex, &now_user_camp, &now_user_level))
	{
		return;
	}

	int title_id = LOGIC_CONFIG->GetTitlePool().GetTopRankTitle(top_type, 1,now_user_level);
	if (title_id <= 0) return;

	const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(title_id);
	if (NULL != title_cfg)
	{
		if (title_cfg->is_broadcast)
		{
			if (0 == old_uid)
			{
				str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_toprank_new_first_broadcast,
					now_uid, now_user_name, now_user_camp, title_id);
			}
			else if (old_uid != now_uid)
			{
				GameName old_user_name;
				char old_user_sex = 0;
				char old_user_camp = 0;
				if (!RankManager::Instance().GetPersonRank()->GetRankUserInfo(m_rank_type_map[top_type], IntToUid(old_uid), old_user_name, &old_user_sex, &old_user_camp))
				{
					return;
				}

				str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_toprank_change_first_broadcast,
					now_uid, now_user_name, now_user_camp, 
					old_uid, old_user_name, old_user_camp,
					title_id);
			}

			if (str_len > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_CENTER_ROLL_2, 0, 0);
			}
		}
	}
}

bool TopRanker::IsRealTimeRank(int top_rank_type)
{
	if (RANK_TOP_TYPE_CAPABILITY == top_rank_type ||
		RANK_TOP_TYPE_LEVEL == top_rank_type ||
		RANK_TOP_TYPE_XIANNV == top_rank_type ||
		RANK_TOP_TYPE_CAPABILITY_CAMP_1 == top_rank_type ||
		RANK_TOP_TYPE_CAPABILITY_CAMP_2 == top_rank_type ||
		RANK_TOP_TYPE_CAPABILITY_CAMP_3 == top_rank_type ||
		RANK_TOP_TYPE_QUNXIANLUANDOU == top_rank_type ||
		RANK_TOP_TYPE_ZHANSHENDIAN == top_rank_type ||
		RANK_TOP_TYPE_RED_PAPER_CONSUME_GOLD == top_rank_type ||
		RANK_TOP_TYPE_MOUNT == top_rank_type ||
		RANK_TOP_TYPE_WING == top_rank_type ||
		RANK_TOP_TYPE_HALO == top_rank_type ||
		RANK_TOP_TYPE_SHENGONG == top_rank_type ||
		RANK_TOP_TYPE_SHENYI == top_rank_type ||
		RANK_TOP_TYPE_EQUIPT_STRENGTH_LEVEL == top_rank_type ||
		RANK_TOP_TYPE_STONE_TOTAL_LEVEL == top_rank_type ||
		RANK_TOP_TYPE_FIGHT_MOUNT == top_rank_type ||
		RANK_TOP_TYPE_CHARM == top_rank_type ||
		RANK_TOP_TYPE_JINGLING == top_rank_type ||
		RANK_TOP_TYPE_QILINBI == top_rank_type || 
		RANK_TOP_TYPE_TOUSHI == top_rank_type || 
		RANK_TOP_TYPE_YAOSHI == top_rank_type ||
		RANK_TOP_TYPE_LINGTONG == top_rank_type ||
		RANK_TOP_TYPE_LINGGONG == top_rank_type ||
		RANK_TOP_TYPE_LINGQI == top_rank_type)
	{
		return true;
	}

	return false;
}

bool TopRanker::IsWeekRank(int top_rank_type)
{
	if (RANK_TOP_TYPE_KILL_WORLDBOSS == top_rank_type) 
	{
		return true;
	}

	return false;
}

void TopRanker::GetTopRankInfo(int uid, PersonRankInfo *personrank_info)
{
	if (NULL == personrank_info) return;

	for (int top_type = RANK_TOP_TYPE_MIN; top_type < RANK_TOP_TYPE_MAX; ++top_type)
	{
		for (int rank = 0; rank < TOP_TEN; ++rank)
		{
			if (uid == m_all_top_rank_list[top_type].uid_list[rank])
			{
				personrank_info->person_rank_list[top_type] = rank + 1;
				break;
			}
		}
	}
}

int TopRanker::GetTopUser(int top_rank_type, GameName user_name)
{
	memset(user_name, 0, sizeof(GameName));

	if (top_rank_type >= RANK_TOP_TYPE_MIN && top_rank_type < RANK_TOP_TYPE_MAX)
	{
		int uid = m_all_top_rank_list[top_rank_type].uid_list[0];
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(uid);
		if (NULL != user_node)
		{
			user_node->GetName(user_name);
			return uid;
		}
	}

	return 0;
}

int TopRanker::GetTopUser(int top_rank_type, GameName user_name, char *sex, char *prof, long long *avatar_timestamp)
{
	memset(user_name, 0, sizeof(GameName));
	*sex = 0;
	*prof = 0;
	*avatar_timestamp = 0;

	if (top_rank_type >= RANK_TOP_TYPE_MIN && top_rank_type < RANK_TOP_TYPE_MAX)
	{
		int uid = m_all_top_rank_list[top_rank_type].uid_list[0];
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(uid);
		if (NULL != user_node)
		{
			user_node->GetName(user_name);
			*sex = user_node->sex;
			*prof = user_node->prof;
			*avatar_timestamp = user_node->avatar_timestamp;
			return uid;
		}
	}

	return 0;
}

int TopRanker::GetTopAverageCapability()
{
	int total_capability = 0;
	int num = 0;

	for (int i = 0; i < TOP_TEN; ++ i)
	{
		int uid = m_all_top_rank_list[RANK_TOP_TYPE_CAPABILITY].uid_list[i];
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(uid);
		if (NULL != user_node)
		{
			total_capability += user_node->capability;
			++ num;
		}
	}
	
	if (0 == num) return 0;

	return total_capability / num;
}

int TopRanker::GetTopRankIndex(int top_rank_type, int uid)
{
	if (top_rank_type < 0 || top_rank_type >= RANK_TOP_TYPE_MAX) return 9999999;

	for (int i = 0; i < TOP_TEN; ++ i)
	{
		if (m_all_top_rank_list[top_rank_type].uid_list[i] == uid) return i;
	}

	return 9999999;
}

void TopRanker::OnUserLogin(Role *role)
{
	if (NULL != role)
	{
		for (int i = 0; i < TOP_TEN; i++)
		{
			if (0 != m_all_top_rank_list[RANK_TOP_TYPE_CAPABILITY].uid_list[i] && m_all_top_rank_list[RANK_TOP_TYPE_CAPABILITY].uid_list[i] == role->GetUID())
			{
				m_top_cap_user_login_timestamp = EngineAdapter::Instance().Time();
				m_top_cap_user_id = role->GetUserId();
				break;
			}
		}

		if (0 != m_all_top_rank_list[RANK_TOP_TYPE_CHARM].uid_list[0] && m_all_top_rank_list[RANK_TOP_TYPE_CHARM].uid_list[0] == role->GetUID())
		{
			m_top_meili_user_login_timestamp = EngineAdapter::Instance().Time();
			m_top_meili_user_id = role->GetUserId();
		}
	}
}

int TopRanker::GetPersonRankType(int top_rank_type)
{
	if (top_rank_type < 0 || top_rank_type >= RANK_TOP_TYPE_MAX) return -1;
	return m_rank_type_map[top_rank_type];
}

void TopRanker::OnRoleChangeSex(Role *role, int from_sex, int to_sex)
{
	if (NULL == role || from_sex == to_sex) return;

	int sensive_rank_type = RANK_TOP_TYPE_MAX;
	{
		if (FEMALE == from_sex)
		{
			sensive_rank_type = RANK_TOP_TYPE_CHARM_FEMALE;
		}
		else if (MALE == from_sex)
		{
			sensive_rank_type = RANK_TOP_TYPE_CHARM_MALE;
		}
	}
	if (sensive_rank_type < 0 || sensive_rank_type >= RANK_TOP_TYPE_MAX)
	{
		return;
	}

	TopUidList & top_uid_list = m_all_top_rank_list[sensive_rank_type];
	int uid_locate_idx = TOP_TEN;

	for (int i = 0; i < TOP_TEN; ++ i)
	{
		if (top_uid_list.uid_list[i] == role->GetUID())
		{
			uid_locate_idx = i;
			break;
		}
	}

	if (uid_locate_idx >= 0 && uid_locate_idx < TOP_TEN)
	{
		for (int i = uid_locate_idx + 1; i < TOP_TEN; ++ i)
		{
			top_uid_list.uid_list[i - 1] = top_uid_list.uid_list[i];
		}

		top_uid_list.uid_list[TOP_TEN - 1] = 0;

		PersonRankInfo personrank_info;
		this->GetTopRankInfo(role->GetUID(),  &personrank_info);
		role->GetTitleManager()->OnTopRankInfo(personrank_info, SYNC_REASON_RANK);
	}
}

