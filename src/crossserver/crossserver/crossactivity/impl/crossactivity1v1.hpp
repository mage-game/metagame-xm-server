#ifndef __CROSS_ACTIVITY_1V1_HPP__
#define __CROSS_ACTIVITY_1V1_HPP__

#include <map>
#include <set>
#include <vector>
#include <algorithm>

#include "engineadapter.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/serverdef.h"
#include "servercommon/crossdef.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/internalprotocal/crossglobalprotocal.h"

#include "crossactivity/crossactivity.hpp"

#include "protocal/msgcross.h"

struct CrossActivity1V1UserRank
{
	CrossActivity1V1UserRank() : count(0) {}

	int count;
	CrossActivity1V1RankItem user_rank_list[CROSS_ACTIVITY_1V1_USER_RANK_MAX];

	typedef std::map<long long, int> UserRankIndexMap;
	typedef std::map<long long, int>::iterator UserRankIndexMapIt;
	UserRankIndexMap user_rank_map;

	CrossActivity1V1RankItem * GetUserRankItem(long long uuid)
	{
		UserRankIndexMapIt it = user_rank_map.find(uuid);
		if (it != user_rank_map.end() && it->second >= 0 && it->second < CROSS_ACTIVITY_1V1_USER_RANK_MAX)
		{
			return &user_rank_list[it->second];
		}

		return NULL;
	}

	// 获取前几名数据
	int GetTopRankItem(int max_item_count, CrossActivity1V1RankItem *item_list)
	{
		int temp_list_size = 0;
		static CrossActivity1V1RankItem *temp_list[CROSS_ACTIVITY_1V1_USER_RANK_MAX] = { NULL };
		memset(temp_list, 0, sizeof(temp_list));

		for (int i = 0; i < CROSS_ACTIVITY_1V1_USER_RANK_MAX; i++)
		{
			if (0 != user_rank_list[i].role_id)
			{
				temp_list[temp_list_size] = &user_rank_list[i];
				++temp_list_size;
			}
		}

		// 排序
		std::stable_sort(std::begin(temp_list), std::end(temp_list), [](const CrossActivity1V1RankItem* tmp_itm1, const CrossActivity1V1RankItem* tmp_itm2) -> bool {
			if (NULL == tmp_itm1 || NULL == tmp_itm2) return false;

			if (tmp_itm1->score == tmp_itm2->score)
			{
				return tmp_itm1->capability > tmp_itm2->capability;
			}
			else
			{
				return tmp_itm1->score > tmp_itm2->score;
			}
		});

		int count = 0;
		for (int i = 0; i < temp_list_size && count < max_item_count; ++i)
		{
			if (NULL != temp_list[i])
			{
				item_list[count] = *temp_list[i];
				++count;
			}
		}

		return count;
	}
};

class CrossActivity1V1 : public CrossActivity
{
public:
	CrossActivity1V1(CrossActivityManager *cross_activity_manager);
	virtual ~CrossActivity1V1();

	virtual void Init(const CrossActivityData &data);
	virtual void GetInitParam(CrossActivityData *data);
	virtual void OnRegisterGameServer(NetID global_netid);
	virtual void OnGSDisconnect(NetID global_netid, const UniqueServerID &unique_server_id);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnDayChange();

	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0);

	virtual void OnCrossUserLogin(CrossUser *cross_user);

	void OnWeekChange();
	void OnSeasonChange();

	void OnSendCross1v1MonthInfo();
	void OnMatchQuery(NetID global_netid, const UniqueUserID &unique_user_id, int req_type);              // 查询匹配结果
	void OnQueryWeekRecord(NetID global_netid, const UniqueUserID &unique_user_id);

	void OnMatchReq(NetID global_netid, crossgameprotocal::GameCrossActivity1V1MatchReq *gcamr);          // 匹配请求
	void OnMatchCancel(long long unique_user_id);														  // 取消匹配

	void OnFightResult(NetID global_netid, crossgameprotocal::GameCrossCross1V1FightResult *gccfr);       // 决斗结束
	void OnGlobalSyncRankList(NetID global_netid, crossgameprotocal::GameCrossCross1V1RankList *gccrl);	  // 同步排名
	void OnSyncRoleRankBaseInfo(NetID global_netid, crossgameprotocal::GameCrossActivity1V1RoleRankBaseInfo *gcarrbi);
	struct MatchUnit
	{
		MatchUnit() 
		{ 
			uuid = 0; prof = 0; sex = 0; camp = 0; reserve_ch = 0; level = 0; jingjie = 0; dur_win_count = 0;
			score = 0; day_join_count = 0; grade = 0; had_match_times = 0; rev_sh = 0;
			capability = 0; memset(name, 0, sizeof(name)); total_join_times = 0; total_win_times = 0; gongxun = 0;
		}

		long long uuid;
		char prof;
		char sex;
		char camp;
		char reserve_ch;
		short level;
		short jingjie;
		int dur_win_count;
		int score;
		short day_join_count;
		short grade;
		short had_match_times;
		short rev_sh;
		long long capability;
		int total_join_times;
		int total_win_times;
		int gongxun;
		GameName name;
		GameName oppo_name;
		RoleAppearance appearance;
	};

private:
	time_t CalcActivityNextOpenTime();
	virtual void OnChangeToNextStatus();

	void DoMatch();																						// 匹配
	int GetOppoIndexByUnit(MatchUnit match_unit, int match_index);										// 获取筛选匹配到的index

	int GetFightSeq();																					// 副本key
	int GetRankInterval();																				// 刷新排名间隔

	void PutMatchToFightUnit(MatchUnit &match_unit, CrossActivity1v1FightUnit &fight_unit, int side, bool is_with_robot = false); 
	void UpdataUserRank(const CrossActivity1V1RankItem &rank_item);

	void ClearMatchInfoByUniqueServerID(const UniqueServerID &unique_server_id);						// 清楚匹配信息
	//////////////////////////////////////////////////////////////////////////

	std::vector<MatchUnit> m_matcher_list;								// 匹配信息列表
	std::map<long long, MatchUnit> m_last_fight_matcher_map;			// 匹配成功列表
	std::set<long long> m_uuid_set;										// 请求匹配列表
	std::map<long long, CrossActivity1v1FightUnit> m_match_map;			// 匹配双方信息列表

	unsigned int m_next_match_time;
	unsigned int m_next_rank_time;
	time_t m_next_sync_min_score_time;									// 下一次同步1v1排行榜最低积分到Game的时间
	ActivityDataParam m_data_param;


	int m_fight_seq;
	int m_open_cfg_idx;
	struct CrossWeekRecord1v1List
	{
		CrossWeekRecord1v1List() : win_this_week(0), lose_this_week(0), record_count(0) {}

		short win_this_week;
		short lose_this_week;
		int record_count;
		CrossWeekRecord1v1 record_list[CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT];
	};
	std::map<long long, CrossWeekRecord1v1List> m_cross_week_record_1v1_map;

	CrossActivity1V1UserRank m_user_rank;

	std::set<long long> m_join_activity_uuid;
};

#endif // __CROSS_ACTIVITY_1V1_HPP__


