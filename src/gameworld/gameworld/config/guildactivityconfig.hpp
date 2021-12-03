#ifndef _GUILD_ACTIVIY_CONFIG_HPP_
#define _GUILD_ACTIVIY_CONFIG_HPP_

#include "servercommon/serverdef.h"
#include "servercommon/configcommon.h"
#include "servercommon/guilddef.hpp"
#include "servercommon/configbase.h"

#include "gameworld/gamedef.h"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <servercommon/challengefielddef.hpp>

#include <vector>

static const int GUILD_MAX_SIGN_IN_CFG_COUNT = 10;
static const int GUILD_BOSS_STAR_LEVEL_COUNT = 3;					// 帮派boss最大星级数

struct GuildActivityOther
{
	GuildActivityOther() : boss_super_call_gold(0), boss_normal_call_max_count(0), boss_super_call_max_count(0), 
		boss_uplevel_item_id(0), boss_uplevel_item_exp(0), rank_update_interval_s(0), boss_notice_timelimit_min(0),
		signin_is_open(false), signin_need_level(0), signin_gongxian(0), signin_gold_bind(0), signin_title_need_num(0), exp_factor(0){}

	int boss_super_call_gold;
	int boss_normal_call_max_count;
	int boss_super_call_max_count;
	ItemID boss_uplevel_item_id;
	int boss_uplevel_item_exp;
	int rank_update_interval_s;
	int boss_notice_timelimit_min;

	bool signin_is_open;
	int signin_need_level;
	int signin_gongxian;	//签到仙盟贡献
	int signin_gold_bind;	//签到仙盟元宝
	int signin_title_need_num;	//签到称号需要本日签到人数
	double exp_factor;
	ItemConfigData signin_item[MAX_ATTACHMENT_ITEM_NUM];
};

struct GuildBossConfig
{
	GuildBossConfig() : level(0), uplevel_exp(0), boss_id(0), pos(0, 0), totem_exp_reward(0), boss_gather_bind_gold(0) {}

	int level;
	int uplevel_exp;
	int boss_id;
	Posi pos;
	int totem_exp_reward;
	int boss_gather_bind_gold;
	ItemConfigData normal_item_reward;
	ItemConfigData super_item_reward;
	ItemConfigData super_call_item_reward;
};

struct GuildBossGatherConfig
{
	GuildBossGatherConfig() : gather_id(0), pos(0, 0) {}

	int gather_id;
	Posi pos;
};

//boss排行奖励
struct GuildBossRankRewardCfg
{
	GuildBossRankRewardCfg() : min_rank(0), max_rank(0), reward_count(0) {}

	int min_rank;
	int max_rank;

	int reward_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

//评星配置
struct GuildBossStarLevelCfg
{
	GuildBossStarLevelCfg() : cost_time_s(0), star_level(0), gather_id(0), limit_num(0), reward_count(0) {}

	int cost_time_s;
	int star_level;
	int gather_id;
	int limit_num;

	int reward_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct GuildSigninCfg
{
	GuildSigninCfg() : index(0), need_count(0)
	{}

	int index;
	int need_count;
	ItemConfigData reward_item;
};

//天赐铜币----------------------------------------------------------------
struct TianCiTongBiGather
{
	TianCiTongBiGather() : gather_id(0), rate(0), reward_mojing_num(0), reward_maturity(0) {}

	int gather_id;
	int rate;
	int reward_mojing_num;
	int reward_maturity;
};

struct TianCiTongBiBigCoinFlushPos
{
	TianCiTongBiBigCoinFlushPos() : pos_x(0), pos_y(0), range(0), rate(0) {}

	int pos_x;
	int pos_y;
	int range;
	int rate;
};

struct TianCiTongBiGatherRefreshPos
{
	TianCiTongBiGatherRefreshPos() : pos_x(0), pos_y(0), gather_id(0) {}

	int pos_x;
	int pos_y;
	int gather_id;
};

class GuildActivityConfig : public ConfigBase
{
public:
	GuildActivityConfig();
	~GuildActivityConfig();

	bool Init(const std::string &path, std::string *err);

	const GuildActivityOther & OtherCfg() { return m_other_cfg; }
	const GuildBossConfig * GetBossCfg(int boss_level);
	int GetGuildBossMaxCount(){return m_boss_max_count;}

	int GetBossGatherCount() { return m_boss_gather_cfg_count; }
	const GuildBossGatherConfig * GetBossGatherCfg(int index);

	const GuildBossRankRewardCfg *GetRankRewardCfg(const int rank);

	const GuildBossStarLevelCfg *GetStarLevelCfg(int star_level);

	const GuildSigninCfg * GetSigninCfg(int signin_cout) const;

	int GetSignInCfgCount() { return m_signin_cfg_count; }

	int GetBossNoticeLeftMin() { return m_other_cfg.boss_notice_timelimit_min; }

	//天赐铜币----------------------------------------------------------------
	int GetTianCiTongBiLastTime() { return m_tianci_tongbi_last_time; }
	int GetTianCiTongBiStandyTime() { return m_tianci_tongbi_standy_time; }
	int GetTianCiTongBiGatherNum() { return m_tianci_tongbi_gather_refresh_pos_count; }
	int GetTianCiTongBiFlushBigCoinNum() { return m_tianci_tongbi_flush_big_coin_num; }
	int GetTianCiTongBiFlushBigCoinNeedMaturity() { return m_tianci_tongbi_flush_bigcoin_need_maturity; }
	int GetTianCiTongBiCoinRewardInfo() { return m_tianci_tongbi_gold_bind_num; }
	int GetTianCiTongBiCoinRewardExp(int role_level);
	int GetTianCiTongBiCoinExpCountLimit() { return m_tianci_tongbi_exp_count_limit; }
	int GetTianCiTongBiCoinGatherId() { return m_tianci_tongbi_bigcoin_gather_id; }
	int GetCoinGatherDisappearTime() { return m_bigcoin_gather_disppear_time_s; }
	int GetTianCiTongBiNpcMaxTouchDistance() { return m_tianci_tongbi_max_npc_touch_distance; }
	int GetTianCiTongBiAcquireGoldBindLimit() { return m_tianci_tongbi_acquire_gold_bind_limit; }

	int RandTianCiTongBiGatherId();
	int GetTianCiTongBiGatherType(int gather_id);
	TianCiTongBiGather * GetTianCiTongBiGatherReward(int gather_type);
	TianCiTongBiGatherRefreshPos *GetTianCiTongBiGatherPos(int &gather_pos_num);
	bool GetTianCiTongBiBigCoinPos(int &pos_x, int &pos_y, int &range);
	int GetTianCiTreeNpcInfo(Posi &pos);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitBossCfg(PugiXmlNode RootElement);
	int InitBossGatherCfg(PugiXmlNode RootElement);
	int InitBossRankRewardCfg(PugiXmlNode RootElement);
	int InitBossStarLevelCfg(PugiXmlNode RootElement);
	int InitSigninCfg(PugiXmlNode RootElement);
	int InitTianCiTongBiGather(PugiXmlNode RootElement);
	int InitTianCiTongBiGatherRefreshPos(PugiXmlNode RootElement);
	int InitTianCiTongBiBigCoinFlushPos(PugiXmlNode RootElement);
	int InitTianCiTongBiBigExpBallCfg(PugiXmlNode RootElement);

	GuildActivityOther m_other_cfg;

	int m_boss_max_count;
	GuildBossConfig m_boss_cfg_list[GUILD_BOSS_MAX_LEVEL];

	static const int MAX_GUILD_BOSS_GATHER_COUNT = 100;
	int m_boss_gather_cfg_count;
	GuildBossGatherConfig m_boss_gather_cfg_list[MAX_GUILD_BOSS_GATHER_COUNT];

	std::vector<GuildBossRankRewardCfg> m_rank_reward_vec;

	GuildBossStarLevelCfg m_star_level_cfg_list[GUILD_BOSS_STAR_LEVEL_COUNT];	// 星级配置

	int m_signin_cfg_count;
	GuildSigninCfg m_signin_cfg[GUILD_MAX_SIGN_IN_CFG_COUNT];

	//天赐铜币----------------------------------------------------------------
	int m_tianci_tongbi_last_time;							// 持续时间
	int m_tianci_tongbi_standy_time;						// 倒计时
	unsigned int m_tianci_tongbi_max_npc_touch_distance;	// npc接触距离
	int m_tianci_tongbi_flush_bigcoin_need_maturity;		// 大铜币采集物刷新所需成熟度
	int m_tianci_tongbi_flush_big_coin_num;					// 大铜币采集物刷新数量
	int m_tianci_tongbi_gold_bind_num;						// 大铜币采集物获得绑元数
	int m_tianci_tongbi_exp_count_limit;					// 大铜币采集物采集次数限制
	int m_tianci_tongbi_bigcoin_gather_id;					// 大铜币采集物id
	int m_bigcoin_gather_disppear_time_s;					// 大铜币采集物消失时间
	Posi m_tianci_tongbi_npc_pos;							// 提交npc坐标
	int m_tianci_tongbi_acquire_gold_bind_limit;			// 大铜币采集物获得绑元限制


	TianCiTongBiGather m_tianci_tongbi_gather_list[GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX];

	int m_tianci_tongbi_gather_refresh_pos_count;
	TianCiTongBiGatherRefreshPos m_tianci_tongbi_gather_refresh_pos_list[GUILD_TIANCI_TONGBI_GATHER_REFRESH_POS_MAX];

	int m_tianci_tongbi_bigcoin_flush_pos_count;
	TianCiTongBiBigCoinFlushPos m_tianci_tongbi_bigcoin_flush_pos_list[GUILD_TIANCI_TONGBI_BIGCOIN_FLUSH_POS_MAX];
	int m_tianci_tongbi_bigcoin_flush_pos_total_rate;
	
	// 经验球配置
	std::map<int, int> m_exp_ball_map;
};

#endif	// _GUILD_ACTIVIY_CONFIG_HPP_

