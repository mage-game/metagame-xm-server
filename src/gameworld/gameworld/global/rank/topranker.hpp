#ifndef _TOPRANKER_HPP_
#define _TOPRANKER_HPP_

#include "servercommon/rankdef.hpp"
#include "servercommon/struct/roleglobalmoduleinfo.hpp"
#include "obj/character/role.h"

const static int GET_TITLE_MIN_LEVEL_LIMIT = 40;		// 获得称号的最小等级限制

class TopRanker
{
public:
	static TopRanker & Instance();

	void Init(const TopRankParam &top_param);
	void GetInitParam(TopRankParam *top_param);

	void OnEnterScene(Role *role);
	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnWeekChange();

	bool IsLoadFinish() { return m_init_top_finish; }

	void GetTopRankInfo(int uid, PersonRankInfo *personrank_info);
	int GetTopUser(int top_rank_type, GameName user_name);
	int GetTopUser(int top_rank_type, GameName user_name, char *sex, char *prof, long long *avatar_timestamp);
	int GetTopAverageCapability();

	int GetTopRankIndex(int top_rank_type, int uid);

	void OnUserLogin(Role *role);

	void OnRoleChangeSex(Role *role, int from_sex, int to_sex);

	int GetTheFirstTenCap(){ return m_the_first_ten_cap; }
	
	int GetPersonRankType(int top_rank_type);

	int GetTopRankUid(int rank_type, int rank);

private:
	TopRanker();
	~TopRanker();

	bool DoRank();
	bool DoOneRank(int top_rank_type);
	void Broadcast(int top_type, int old_uid, int now_uid);
	bool IsRealTimeRank(int top_rank_type);
	bool IsWeekRank(int top_rank_type);

	bool m_init_top_finish;

	TopUidList m_all_top_rank_list[RANK_TOP_TYPE_MAX];

	int m_rank_type_map[RANK_TOP_TYPE_MAX];

	time_t m_last_update_time;

	time_t m_top_cap_user_login_timestamp;
	UserID m_top_cap_user_id;

	time_t m_top_meili_user_login_timestamp;
	UserID m_top_meili_user_id;
	
	int m_the_first_ten_cap;	
	
	struct TopUserIDList
	{
		UserID user_id_list[TOP_TEN];
	};
};

#endif

