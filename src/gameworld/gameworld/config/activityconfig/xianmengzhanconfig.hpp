#ifndef __XIAN_MENG_ZHAN_CONFIG_HPP__
#define __XIAN_MENG_ZHAN_CONFIG_HPP__

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

class XianMengZhanConfig : public ConfigBase
{
public:

	struct DefendAreaCfg
	{
		DefendAreaCfg() : user_realive_pos(0, 0), gather_pos(0, 0), gather_id(0)
		{
		}

		Posi user_realive_pos;
		Posi gather_pos;
		int gather_id;
	};

	struct RankRewardCfg
	{
		const static int REWARD_DETAIL_MAX_COUNT = 32;

		struct RewardDetail
		{
			RewardDetail() : need_level(0), xianhun(0), exp(0), gongxian(0), honor(0) {}

			int need_level;
			int xianhun;
			int exp;
			int gongxian;
			int honor;
		};

		RankRewardCfg() : need_rank(0), reward_detail_count(0) {}

		int need_rank;
		int reward_detail_count;
		RewardDetail reward_detail_list[REWARD_DETAIL_MAX_COUNT];
	};

	XianMengZhanConfig();
	~XianMengZhanConfig();

	bool Init(const std::string &path, std::string *err);

	int  GetEnterInfo(Posi *pos, int enter_area = -1);
	bool GetRealivePos(int defend_area, int enter_area, Posi *pos);

	int  GetGuildCallCount() { return m_guild_call_count_limit; }
	int  GetGuildCallGoldCost() { return m_guild_call_gold_cost; }
	int  GetGuildCallCDTime() { return m_guild_call_cd_time; }

	Posi& GetCenterAreaPos() { return m_center_area_pos; }
	int  GetCenterAreaRange() { return m_center_area_range; }

	int  GetHasCenterRewardInterval() { return m_has_center_reward_interval; }
	int  GetHasCenterRewardData() { return m_has_center_reward_data; }

	int GetCenterAreaGatherID() { return m_center_area_gather_id; }
	
	int	 GetDefendAreaRewardInterval() { return m_defend_area_reward_interval; }
	int  GetRewardHasDefendArea() { return m_reward_has_defend_area; }

	bool GetRewardKill(const Posi &dead_pos, int *reward_killer_score, int *reward_dead_score);
	const XianMengZhanConfig::RankRewardCfg::RewardDetail * GetRankReward(int guild_rank, int role_level);

	int GetFreeRewardIntervalS() { return m_free_jifen_interval_s; }
	int GetFreeReward() { return m_free_jifen; }
	bool GetDefendAreaGatherInfo(int defend_area, int *gather_id, Posi *pos);
	const ItemConfigData * GetRewardGuildRankGift(int guild_rank, bool *is_in_rank);

	int GetEnterAreaCount();

private:
	int            m_scene_id;

	Posi           m_realive_area_list[XIANMENGZHAN_REALIVE_AREA_NUM];

	int            m_guild_call_count_limit;
	int            m_guild_call_gold_cost;
	int            m_guild_call_cd_time;

	Posi           m_center_area_pos;
	int			   m_center_area_range;
	int			   m_has_center_reward_data;
	int            m_has_center_reward_interval;
	int			   m_center_area_gather_id;

	DefendAreaCfg  m_defend_area_list[XIANMENGZHAN_DEFEND_AREA_NUM];

	int			   m_defend_area_reward_interval;
	int			   m_reward_has_defend_area;

	int            m_reward_center_area_kill;
	int            m_reward_center_area_dead;
	int            m_reward_not_center_area_kill;
	int            m_reward_not_center_area_dead;

	int            m_reward_rank_gift_count;
	RankRewardCfg  m_reward_rank_gift_list[XIANMENGZHAN_REWARD_GUILD_RANK_MAX];

	int			   m_reward_guild_max_rank; 
	ItemConfigData m_reward_guild_rank_list[XIANMENGZHAN_REWARD_GUILD_RANK_GIFT_MAX + 1];

	int			   m_free_jifen_interval_s;
	int			   m_free_jifen;
};


#endif // __XIAN_MENG_ZHAN_CONFIG_HPP__
