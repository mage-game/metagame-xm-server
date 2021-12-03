#ifndef _ROLEGLOBALMODULEINFO_HPP_
#define _ROLEGLOBALMODULEINFO_HPP_

#include "servercommon/rankdef.hpp"
#include"servercommon/activitydef.hpp"
#include "servercommon/crossdef.hpp"

#pragma pack(push, 4) 

struct PersonRankInfo
{
	PersonRankInfo() { this->Reset(); }

	void Reset()
	{
		memset(person_rank_list, 0, sizeof(person_rank_list));
	}

	int person_rank_list[RANK_TOP_TYPE_MAX];
};

struct CrossPersonRankInfo
{
	CrossPersonRankInfo() { this->Reset(); }

	void Reset()
	{
		memset(rank_list, 0, sizeof(rank_list));
	}

	int rank_list[CROSS_RANK_TOP_TYPE_MAX];
};

struct ActivityTitleInfo
{
	ActivityTitleInfo() { this->Reset(); }

	void Reset()
	{
		xianmengzhan_mengzhu_id = 0;
		gongchengzhan_mengzhu_id = 0;
		guildbattle_mengzhu_guild_id = 0;
		yizhandaodi_win_user_id = 0;
		yizhandaodi_second_user_id = 0;
		yizhandaodi_third_user_id = 0;
		memset(qunxian_luandou_user_id_list, 0, sizeof(qunxian_luandou_user_id_list));
	}

	int xianmengzhan_mengzhu_id;
	int gongchengzhan_mengzhu_id;
	GuildID guildbattle_mengzhu_guild_id;
	int yizhandaodi_win_user_id;
	int yizhandaodi_second_user_id;
	int yizhandaodi_third_user_id;

	int qunxian_luandou_user_id_list[QUNXIANLUANDOU_SIDE_MAX];
};

struct OtherTitleInfo
{
	OtherTitleInfo() { this->Reset(); }

	void Reset() 
	{
		challengefield_king_userid = 0;
		memset(camp_toprank3_uid, 0, sizeof(camp_toprank3_uid));
		memset(cross_fishing_top_rank_uid_list, 0, sizeof(cross_fishing_top_rank_uid_list));

		qingyuan_cap_top_male_uid = 0;
		qingyuan_cap_top_female_uid = 0;
		baby_cap_top_male_uid = 0;
		baby_cap_top_female_uid = 0;
		littlepet_cap_top_male_uid = 0;
		littlepet_cap_top_female_uid = 0;
	}

	long long challengefield_king_userid;

	UNSTD_STATIC_CHECK(9 == MAX_CAMP_RECORD_RANK3_NUM);
	int camp_toprank3_uid[MAX_CAMP_RECORD_RANK3_NUM];											// 前3三名. 3个阵营
	int cross_fishing_top_rank_uid_list[CROSS_FISHING_TOP_RANK_COUNT_MAX];
	int cross_pasture_top_rank_uid_list[CROSS_PASTURE_TOP_RANK_COUNT_MAX];

	int qingyuan_cap_top_male_uid;
	int qingyuan_cap_top_female_uid;
	int baby_cap_top_male_uid;
	int baby_cap_top_female_uid;
	int littlepet_cap_top_male_uid;
	int littlepet_cap_top_female_uid;
};

struct RoomActivityInfo
{
	RoomActivityInfo() { this->Reset(); }
	void Reset()
	{
		memset(mess_battle_rank_first, 0, sizeof(mess_battle_rank_first));
		memset(mess_battle_rank_second, 0, sizeof(mess_battle_rank_second));
		memset(mess_battle_rank_third, 0, sizeof(mess_battle_rank_third));

		memset(night_fight_rank_first, 0, sizeof(night_fight_rank_first));
		memset(night_fight_rank_second, 0, sizeof(night_fight_rank_second));
		memset(night_fight_rank_third, 0, sizeof(night_fight_rank_third));
	}
	int mess_battle_rank_first[CROSS_MESS_BATTLE_MAX_SCENE_NUM];
	int mess_battle_rank_second[CROSS_MESS_BATTLE_MAX_SCENE_NUM];
	int mess_battle_rank_third[CROSS_MESS_BATTLE_MAX_SCENE_NUM];

	int night_fight_rank_first[CROSS_NIGHT_FIGHT_MAX_SCENE_NUM];
	int night_fight_rank_second[CROSS_NIGHT_FIGHT_MAX_SCENE_NUM];
	int night_fight_rank_third[CROSS_NIGHT_FIGHT_MAX_SCENE_NUM];
};

struct CrossActivityTitleInfo
{
	CrossActivityTitleInfo() { this->Reset(); }
	void Reset()
	{
		memset(cross_xiuluotower_rank_title, 0, sizeof(cross_xiuluotower_rank_title));
	}
	int cross_xiuluotower_rank_title[CROSS_ACTIVITY_XIULUO_TOWER_RANK_TITLE_NUM];
};

enum SYNC_REASON
{
	SYNC_REASON_ENTER_SCENE = 0,						// 切场景
	SYNC_REASON_RANK,									// 排行榜
	SYNC_REASON_ACTIVITY,								// 活动
};

#pragma pack(pop)

#endif
