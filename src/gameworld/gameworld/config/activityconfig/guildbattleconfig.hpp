// #ifndef __GUILD_BATTLE_CONFIG_HPP__
// #define __GUILD_BATTLE_CONFIG_HPP__
// 
// #include "gamedef.h"
// #include "servercommon/struct/itemlistparam.h"
// #include "servercommon/maildef.hpp"
// #include <map>
// #include <vector>
// 
// struct GuildBattleOtherCfg
// {
// 	GuildBattleOtherCfg():scene_id(0), realive_pos(0,0), boss_refresh_pos(0,0), recalc_time(0),
// 		shield_times_1(0), shield_times_2(0), silver_box_count(0), glod_box_count(0),rand_x(0), rand_y(0), gold_box_id(0),
// 		sliver_box_id(0), common_box_id(0), token_id(0), refresh_rare_monster_count(0),
// 		huizhang_extra_reward_item_count(0), lucky_interval(0), luck_people_num(0){}
// 	
// 	int scene_id;
// 	Posi realive_pos;
// 	Posi boss_refresh_pos;
// 	unsigned int recalc_time;
// 	unsigned int shield_times_1;
// 	unsigned int shield_times_2;
// 	int silver_box_count;
// 	int glod_box_count;
// 	int rand_x;							// 怪物掉落采集物离怪物范围x
// 	int rand_y;							// 怪物掉落采集物离怪物范围y
// 	int gold_box_id;
// 	int sliver_box_id;
// 	int common_box_id;
// 	int token_id;
// 	int refresh_rare_monster_count;
// 	int huizhang_extra_reward_item_count;
// 	ItemConfigData huizhang_extra_reward[MAX_ATTACHMENT_ITEM_NUM];
// 	int lucky_interval;					// 幸运奖励间隔时间
// 	ItemConfigData lucky_item;			// 幸运奖励物品
// 	int luck_people_num;				// 幸运人数
// };
// 
// struct GuildBattleMonsterCfg
// {
// 	GuildBattleMonsterCfg():monster_id(0), first_refresh_time_s(0), refresh_interval_s(0),
// 		kill_personal_credit(0), kill_guild_credit(0), world_level_min(0), world_level_max(0), is_boss(0), is_rare_monster(0),
// 		prepare_notice_time_s(0)
// 	{}
// 
//     int monster_id;
// 	unsigned int first_refresh_time_s;
// 	unsigned int refresh_interval_s;
// 	int kill_personal_credit;
// 	int kill_guild_credit;
// 	short world_level_min;
// 	short world_level_max;
// 	short is_boss;
// 	short is_rare_monster;
// 	unsigned int prepare_notice_time_s;
// };
// 
// struct GuildBattleGoodsCreditCfg
// {
// 	GuildBattleGoodsCreditCfg():gather_id(0),gather_time_s(0),reward_credit_personal(0),reward_credit_guild(0),speed_slow_down(0){}
// 
// 	int gather_id;
// 	unsigned int gather_time_s;
// 	int reward_credit_personal;
// 	int reward_credit_guild;
// 	short speed_slow_down;
// };
// 
// struct GuildBattleKillOtherCreditCfg
// {
// 	GuildBattleKillOtherCreditCfg():kill_count_min(0),kill_count_max(0),reward_preson_credit(0),reward_guild_credit(0), kill_count_index(0){}
// 
// 	short kill_count_min;
// 	short kill_count_max;
// 	int reward_preson_credit;
// 	int reward_guild_credit;
// 	short kill_count_index;
// };
// 
// struct GuildBattlePersonalRewardCfg
// {
// 	GuildBattlePersonalRewardCfg():reward_index(0),require_credit_min(0),require_credit_max(0),banggong(0), shengwang(0){}
// 
// 	int reward_index;
// 	int require_credit_min;
// 	int require_credit_max;
// 	int banggong;
// 	int shengwang;
// 
// 	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
// };
// 
// struct GuildBattleRankRewardCfg
// {
// 	GuildBattleRankRewardCfg():rank(0),base_role_exp(0), banggong(0), shengwang(0), reward_item_count(0){}
// 	
// 	int rank;
// 	int base_role_exp;
// 	int banggong;
// 	int shengwang;
// 	int reward_item_count;
// 	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
// };
// 
// struct GuildBattleRareMonsterPos
// {
// 	GuildBattleRareMonsterPos():scene_id(0),rare_monster_pos(0,0){}
// 
// 	int scene_id;
// 	Posi rare_monster_pos;
// };
// 
// class GuildBattleConfig
// 	{
// public:
// 
// 	GuildBattleConfig();
// 	~GuildBattleConfig();
// 
// 	bool Init(const std::string &configname, std::string *er);
// 	int GetGuildBattleEnterInfo(Posi *pos);
// 
// 	const GuildBattleOtherCfg& GuildBattleGetOtherCfg(){return m_other_cfg;}
// 	const int GetGlodBoxCount(){return m_other_cfg.glod_box_count;}
// 	const int GetSilverBoxCount(){return m_other_cfg.silver_box_count;}
// 	const int GetGlodBoxId(){return m_other_cfg.gold_box_id;}
// 	const int GetSilverBoxId(){return m_other_cfg.sliver_box_id;}
// 	const int GetCommonBoxId(){return m_other_cfg.common_box_id;}
// 	const int GetTokenBoxId(){return m_other_cfg.token_id;}
// 	const int GetRefreshRareMonsterCount(){return m_other_cfg.refresh_rare_monster_count;}
// 	const GuildBattleMonsterCfg * GuildBattleGetMonsterCfg(int monster_id) const;
// 	const GuildBattleMonsterCfg * GetBossMonsterByWorldLevel(int world_level);
// 	const GuildBattleMonsterCfg * GetRareMonsterByWorldLevel(int world_level);
// 	const GuildBattleGoodsCreditCfg * GetGoodsCreditCfg(int gather_id) const;
// 	const GuildBattleKillOtherCreditCfg *GetKillOtherCreditCfg(int kill_count) const;
// 	const GuildBattlePersonalRewardCfg * GetPersonalRewardCfg(int require_credit) const;
// 	const GuildBattlePersonalRewardCfg * GetPersonalRewardCfgByIndex(int index) const;
// 	const GuildBattleRankRewardCfg * GetRankRewardCfg(int rank)const;
// 	const GuildBattleRareMonsterPos* GetRareMonsterRandPost() const;
// 	const Posi * GetSilverBoxRandPost() const;
// 
// 
// private:
// 	int InitGuildBattleOtherCfg(TiXmlElement *RootElement);
// 	int InitGuildBattleMonsterCfg(TiXmlElement *RootElement);
// 	int InitGuildBattleGoodsCreditCfg(TiXmlElement *RootElemen);
// 	int InitGuildBattleKillOtherCreditCfg(TiXmlElement *RootElemen);
// 	int InitGuildBattlePersonalRewardCfg(TiXmlElement *RootElemen);
// 	int InitGuildBattleRankRewardCfg(TiXmlElement *RootElemen);
// 	int InitGuildBattleRareMonsterPos(TiXmlElement *RootElemen);
// 	int InitGuildBattleSliverBoxPos(TiXmlElement *RootElemen);
// 
// 	GuildBattleOtherCfg m_other_cfg;
// 	std::map<int,GuildBattleMonsterCfg>m_monster_cfg_list;					// 怪物列表
// 	std::map<int,GuildBattleGoodsCreditCfg>m_goods_credit_list;				// 物品积分列表
// 	std::map<int,GuildBattleKillOtherCreditCfg>m_kill_other_credit_list;	// 击杀积分列表
// 	std::map<int,GuildBattlePersonalRewardCfg>m_personal_reward_list;		// 个人积分奖励列表
// 	std::map<int,GuildBattleRankRewardCfg>m_rank_reward_list;				// 积分排名奖励列表
// 	std::vector<GuildBattleRareMonsterPos>m_rare_monster_pos;				// 稀有怪物随机坐标
// 	std::vector<Posi>m_silver_box_pos;										// 银箱子随机坐标
// };
// 
// 
// #endif // __SPECIAL_GUILD_BATTLE_HPP__
