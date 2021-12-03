#ifndef _GUILD_CONFIG_HPP_
#define _GUILD_CONFIG_HPP_

#include "servercommon/serverdef.h"
#include "servercommon/configcommon.h"
#include "servercommon/guilddef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/maildef.hpp"

#include "gameworld/gamedef.h"
#include <stdlib.h>
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/global/guildparam.hpp"
#include "servercommon/maildef.hpp"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <string>
#include <set>

enum GUILD_MEMBER_LUCKY_COLOR
{
	GUILD_MEMBER_LUCKY_COLOR_INVALID = 0,

	GUILD_MEMBER_LUCKY_COLOR_WHITE,															// 白色
	GUILD_MEMBER_LUCKY_COLOR_GREEN,															// 绿色
	GUILD_MEMBER_LUCKY_COLOR_BLUE,															// 蓝色
	GUILD_MEMBER_LUCKY_COLOR_PURPLE,														// 紫色
	GUILD_MEMBER_LUCKY_COLOR_ORANGE,														// 橙色

	GUILD_MEMBER_LUCKY_COLOR_MAX,
};

enum GUILD_LUCKY_FACTOR_TYPE
{
	GUILD_LUCKY_FACTOR_TYPE_INVALID = 0,

	GUILD_LUCKY_FACTOR_TYPE_GUILDTASK,													// 仙盟任务加成
	GUILD_LUCKY_FACTOR_TYPE_BOSSDROP,													// 仙盟boss掉落几率
	GUILD_LUCKY_FACTOR_TYPE_FBDROP,														// 仙盟fb掉落几率

	GUILD_LUCKY_FACTOR_TYPE_MAX,
};

struct GuildOtherConfig
{
	GuildOtherConfig() : guild_create_coin(0), guild_create_item_id(0), guild_jianshe_item_id(0), guild_delate_item_id(0), guild_station_scene_id(0),
		guild_back_station_pos(0, 0), guild_lucky_zhufu_times(0), 
		guild_init_exp_1(0), creator_get_gongxian1(0), guild_init_exp_2(0), creator_get_gongxian2(0),
		box_free_up_count(0), box_up_gold(0), box_start_time(0), box_assist_max_count(0), 
		box_assist_cd(0), box_assist_cd_limit(0), box_assist_cd_gold(0), box_limit_time(0),
		totem_bless_interval(0), finish_task_totem_exp_reward(0), join_guild_level_limit(0),
		guild_call_in_level_limit(0), guild_call_in_free_time(0), guild_call_in_cost_gold(0),
		atuo_join_guild_cd(0), free_create_guild_times(0), extend_member_itemID(0), daily_relive_modulus(0), craete_guild_init_relive_times(0),
		singal_chat_max_time(0), singal_chat_interval(0), today_saizi_count(0), siai_cold_down(0), siai_mojing(0),
		chengyuan_event_notice_vip_level(0), first_change_avatar_give_bind_coin(0), exp_factor(0), juanxian_gold_add_exp(0),
		auto_kickout_level_limit(0), auto_kickout_vip_limit(0), auto_kickout_time_s_limit(0), auto_kickout_member_critical_per(0),
		kill_cross_boss_open_day(0),kill_cross_boss_mengzhu_skill(0), need_gongzi(0), min_give_gongzi_member_num(0), storage_constant_item_id(0), constant_item_storage_score(0), broadcast_grade(0)

	{
	}

	int guild_create_coin;													// 创建军团花费铜币
	int guild_create_item_id;												// 创建军团令

	int guild_jianshe_item_id;												// 军团建设令
	int guild_delate_item_id;												// 军团弹劾令
	
	int guild_station_scene_id;												// 军团驻地ID
	Posi guild_back_station_pos;											// 进入军团驻地坐标

	int guild_lucky_zhufu_times;

	int guild_init_exp_1;													// 仙盟1级初始财富
	int creator_get_gongxian1;												// 仙盟1级创建者获得贡献
	int guild_init_exp_2;													// 仙盟2级初始财富
	int creator_get_gongxian2;												// 仙盟2级创建者获得贡献

	int box_free_up_count;													// 宝箱免费升级次数
	int box_up_gold;														// 宝箱升级元宝
	int box_start_time;														// 宝箱开始时间
	int box_assist_max_count;												// 宝箱协助他人最大次数

	int box_assist_cd;														// 宝箱协助CD
	int box_assist_cd_limit;												// 宝箱协助CD限制
	int box_assist_cd_gold;													// 宝箱消除协助cd消耗元宝
	int box_limit_time;														// 宝箱重进仙盟限制时间

	int totem_bless_interval;												// 图腾祝福间隔
	int finish_task_totem_exp_reward;										// 完成全部公会任务图腾经验奖励
	int join_guild_level_limit;												// 军团功能最低等级

	int guild_call_in_level_limit;											// 公会招募等级限制
	int guild_call_in_free_time;											// 公会招募免费次数
	int guild_call_in_cost_gold;											// 公会招募所需元宝

	unsigned int atuo_join_guild_cd;										// 一键加入公会CD

	int free_create_guild_times;											// 免费创1级帮次数
	
	ItemID extend_member_itemID;											// 扩展公会成员物品ID
	int daily_relive_modulus;												// 每天重置复活系数
	int craete_guild_init_relive_times;										// 创建公会初始复活次数

	short singal_chat_max_time;												// 红包私聊次数限制
	int singal_chat_interval;												// 红包私聊间隔

	int today_saizi_count;													// 每天骰子数量
	int siai_cold_down;														// 骰子冷却时间
	int siai_mojing;														// 投骰子获得魔精

	int chengyuan_event_notice_vip_level;									// 仙盟成员事件提醒vip等级
	int first_change_avatar_give_bind_coin;									// 第一次变更公会头像发绑元
	double exp_factor;														// 经验系数
	int day_juanxian_gold_limit;											// 每日捐元宝上限
	int juanxian_gold_add_exp;                                              // 仙盟捐献1元宝等于x贡献
	int auto_kickout_level_limit;											// 公会自动踢人等级限制
	int auto_kickout_vip_limit;												// 公会自动踢人VIP限制
	int auto_kickout_time_s_limit;											// 公会自动踢人时间(秒)限制
	int auto_kickout_member_critical_per;									// 公会自动踢人成员人数临界百分比（向上取整)
	int kill_cross_boss_open_day;											// 神域boss击杀榜开启天数
	int kill_cross_boss_mengzhu_skill;										// 神域boss击杀榜盟主技能
	ItemConfigData kill_cross_boss_reward[MAX_ATTACHMENT_ITEM_NUM];			// 神域boss击杀榜仙盟奖励
	int need_gongzi;														// 目标工资
	int min_give_gongzi_member_num;											// 不足X人补齐
	ItemID storage_constant_item_id;										// 仙盟仓库固定物品ID
	int constant_item_storage_score;										// 仙盟仓库固定物品积分
	int broadcast_grade;													// 红色X阶及以上才播报传闻
};

struct GuildJuanXianConfig
{
	struct ConfigItem
	{
		ConfigItem() : item_id(0), add_gongxian(0), add_guild_exp(0)
		{

		}

		ItemID item_id;
		int add_gongxian;
		int add_guild_exp;
	};

	std::map<ItemID, ConfigItem> juanxian_map;
};

struct GuildLevelConfig
{
	struct ConfigItem
	{
		ConfigItem() : level(0), max_exp(0), max_member(0), comsume_exp_perday(0)
		{
		}

		int level;
		int max_exp;
		int max_member;
		int comsume_exp_perday;
		ItemConfigData daily_reward_item;
	};

	ConfigItem cfg_list[GUILD_LEVEL_MAX + 1];
};

struct GuildExchangeConfig
{
	struct ConfigItem
	{
		ConfigItem() : item_id(0), guild_level_limit(0), cost_gongxian(0), max_day_exchange_num(0)
		{

		}

		ItemID item_id;
		int guild_level_limit;
		int cost_gongxian;
		int max_day_exchange_num;
	};

	std::map<ItemID, ConfigItem> exchange_map;
};

struct MemberLuckBlessingCfg
{
	MemberLuckBlessingCfg()
		: task_factor(0), boss_drop_factor(0), fb_drop_factor(0)
	{
		memset(upgrade_prob_list, 0, sizeof(upgrade_prob_list));
	}

	int task_factor;																		// 仙盟任务奖励加成 要除100
	int boss_drop_factor;																	// 仙盟BOSS 掉落几率
	int fb_drop_factor;																		// 仙盟密境 掉落几率

	int upgrade_prob_list[GUILD_MEMBER_LUCKY_COLOR_MAX];									// 提升到其它颜色的概率
};

struct GuildStorgeCfg
{
	GuildStorgeCfg()
	{
		memset(storge_open_grid_list, 0, sizeof(storge_open_grid_list));
	}

	int storge_open_grid_list[GUILD_LEVEL_MAX + 1];
};

// 仙盟技能
struct GuildSkillCfg
{
	struct ConfigItem
	{
		ConfigItem() : skill_index(0), level(0), uplevel_gongxian(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0),
			shanbi(0), baoji(0), jianren(0), zhufuyiji_per(0), 
			guild_level_limit(0), uplevel_stuff_id(0), uplevel_stuff_count(0)
		{
		}

		int skill_index;
		int level;
		int uplevel_gongxian;
		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
		Attribute zhufuyiji_per;

		int guild_level_limit;
		int uplevel_stuff_id;
		int uplevel_stuff_count;
	};

	ConfigItem item_list[GUILD_SKILL_COUNT][MAX_GUILD_SKILL_LEVEL + 1];
};

// 仙盟图腾
struct GuildTotemCfg
{
	struct ConfigItem
	{
		ConfigItem() : level(0), uplevel_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0),
			leader_maxhp(0), leader_gongji(0), leader_fangyu(0), leader_mingzhong(0), leader_shanbi(0), leader_baoji(0), leader_jianren(0),
			bless_exp(0), totem_hp(0)
		{
		}

		int level;
		int uplevel_exp;
		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
		Attribute leader_maxhp;
		Attribute leader_gongji;
		Attribute leader_fangyu;
		Attribute leader_mingzhong;
		Attribute leader_shanbi;
		Attribute leader_baoji;
		Attribute leader_jianren;
		long long bless_exp;
		int totem_hp;
	};

	ConfigItem item_list[MAX_GUILD_TOTEM_LEVEL + 1];
};

// 仙盟宝箱
struct GuildBoxCfg
{
	struct ConfigItem
	{
		ConfigItem() : level(0), be_assist_reward_bind_gold(0), init_rate(0), uplevel_rate(0), box_open_time(0), box_assist_dec_time(0), can_be_assist_max_count(0), item_reward_count(0), probability(0)
		{
		}

		int level;
		//ItemConfigData item_reward;
		ItemConfigData assist_reward;
		int be_assist_reward_bind_gold;
		int init_rate;
		int uplevel_rate;
		int box_open_time;
		int box_assist_dec_time;
		int can_be_assist_max_count;

		int item_reward_count;
		ItemConfigData item_reward_list[MAX_ATTACHMENT_ITEM_NUM];

		ItemConfigData random_reward;
		int probability;
	};

	ConfigItem item_list[MAX_GUILD_BOX_LEVEL + 1];
};

// 领土福利
struct GuildTerritoryWelfCfg
{
	struct ConfigItem
	{
		ConfigItem():territory_index(0), banggong_one_limit(0), banggong_two_limit(0), banggong_three_limit(0), banggong_four_limit(0){}

		short territory_index;
		short banggong_one_limit;
		short banggong_two_limit;
		short banggong_three_limit;
		short banggong_four_limit;
		ItemConfigData phase_one_reward;
		ItemConfigData phase_two_reward;
		ItemConfigData phase_three_reward;
		ItemConfigData phase_four_reward;
		ItemConfigData guild_president_extra_reward;
	};

	ConfigItem item_list[MAX_TERRITORY_WELF_COUNT + 1];
};

// boss公会红包配置
struct BossGuildRedbagCfg
{
	BossGuildRedbagCfg():index(0), kill_boss_times(0), max_diamond(0), guild_relive_times(0){}
	
	int index;
	int kill_boss_times;
	int max_diamond;
	int guild_relive_times;
};

// 骰子配置
struct GuildSaiziCfg
{
	GuildSaiziCfg():rank1(-1), rank2(-1), reward_item_count(0){}

	int rank1;
	int rank2;
	
	int reward_item_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

const static int GUILD_EXTEND_MEMBER_CFG_MAX_COUNT = 20;
struct GuildExtendMemberCfg
{
	GuildExtendMemberCfg() : min_extend_member_num(0), need_item_count(0){}

	int min_extend_member_num;
	int need_item_count;
};

enum GUILD_RED_PAPER_TYPE
{
	GUILD_RED_PAPER_INVALID = -1,

	PAPER_TYPE_VIP_LEVEL_UP,		// vip等级
	PAPER_TYPE_BUY_GOLD_VIP,		// 购买黄金会员
	PAPER_TYPE_BUY_LEVEL_INVEST,	// 购买等级投资
	PAPER_TYPE_BUY_MONTH_INVEST,	// 购买月卡投资
	PAPER_TYPE_BUY_HUNDRED_REWARD,	// 购买百倍返利
	PAPER_TYPE_BUY_ZERO_GIFT,		// 购买零元礼包
	PAPER_TYPE_DAY_FIRST_CHONGZHI,	// 每日首充
	PAPER_TYPE_DAY_TOTOAL_CHONGZHI,	// 每日累充
	PAPER_TYPE_LUCKY_PAPER,			// 幸运红包
	PAPER_TYPE_GUILD_QUESTION,		// 仙盟答题红包
	PAPER_TYPE_BUY_HUNDRED2_REWARD,	// 购买百倍返利2

	GUILD_RED_PAPER_MAX,
};

struct GuildRedPaperCfg
{
	GuildRedPaperCfg(): paper_seq(-1), paper_type(GUILD_RED_PAPER_INVALID), paper_level(0), cond_param(0), bind_gold(0)
	{
	}

	int paper_seq;
	int paper_type;
	int paper_level;
	int cond_param;
	int bind_gold;
};

struct GuildMazeCfg
{
	GuildMazeCfg() : layer(0), fail_down_layer(0), fail_cd(0) {}

	int layer;
	int fail_down_layer;
	int fail_cd;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct GuildMazeRankRewardCfg
{
	GuildMazeRankRewardCfg() : rank(0) {}

	int rank;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct BiaoCheLvInfo
{
	BiaoCheLvInfo() : total_guild_exp(0), rob_guild_exp(0), succ_reward_exp_factor(0), fail_reward_exp_factor(0),
		succ_reward_count(0), fail_reward_count(0), rob_reward_count(0), succ_gongxian(0), fail_gongxian(0), rob_gongxian(0){}

	int total_guild_exp;
	int rob_guild_exp;

	double succ_reward_exp_factor;
	double fail_reward_exp_factor;

	int succ_reward_count;
	ItemConfigData succ_reward_item_list[MAX_ATTACHMENT_ITEM_NUM];

	int fail_reward_count;
	ItemConfigData fail_reward_item_list[MAX_ATTACHMENT_ITEM_NUM];

	int rob_reward_count;
	ItemConfigData rob_reward_item_list[MAX_ATTACHMENT_ITEM_NUM];

	int succ_gongxian;
	int fail_gongxian;
	int rob_gongxian;
};

struct GuildRumorCfg
{
	GuildRumorCfg() :rumor_type(0),param_1(0),param_2(0),param_3(0) {}
	int rumor_type;
	int param_1;
	int param_2;
	int param_3;
};

struct GuildPlatNameCfg
{
	GuildPlatNameCfg() : /*currency_type(0), */gold(0) {}
	//int currency_type;
	Int64 gold;
};

// 仙盟工资--------------------
struct GuildGongziCfg
{
	GuildGongziCfg(){}

	int guild_post = 0;
	int rate = 0;
};

struct GuildGongziGiveCfg
{
	GuildGongziGiveCfg() {}

	int gongzi_give_type = 0;
	int give_gongzi = 0;
};

class GuildConfig : public ConfigBase
{
public:
	GuildConfig();
	~GuildConfig();

	bool Init(const std::string &path, std::string *err);

	GuildOtherConfig & GetGuildOtherConfig() { return m_other_cfg; }
	int GetGuildCreateCoin() { return m_other_cfg.guild_create_coin; }
	int GetGuildCreateItemId() { return m_other_cfg.guild_create_item_id; }

	int GetGuildJiansheItemId() { return m_other_cfg.guild_jianshe_item_id; }
	int GetGuildDelateItemId() { return m_other_cfg.guild_delate_item_id; }

	int GetGuildStationSceneId() { return m_other_cfg.guild_station_scene_id; }
	Posi GetGuildBackStationPos() { return m_other_cfg.guild_back_station_pos; }
	
	const GuildJuanXianConfig::ConfigItem * GetJuanXianConfig(ItemID item_id);
	const GuildLevelConfig::ConfigItem * GetLevelConfig(int level);
	const GuildExchangeConfig::ConfigItem * GetExchangeConfig(ItemID item_id);
	int GetGuildReduceExpDay(int guild_level);

	int GetGuildLuckyFactor(int lucky_color, int lucky_type);
	int GetGuildLuckyZhufuTimes() { return m_other_cfg.guild_lucky_zhufu_times; }
	int GetGuildInitExp1() { return m_other_cfg.guild_init_exp_1; }
	int GetGuildInitExp2() { return m_other_cfg.guild_init_exp_2; }
	int GetGuildCreatorGetGongXian1() { return m_other_cfg.creator_get_gongxian1; }
	int GetGuildCreatorGetGongXian2() { return m_other_cfg.creator_get_gongxian2; }
	unsigned int GetGuildAutoJoinCD(){ return m_other_cfg.atuo_join_guild_cd;}
	int GetGuildFreeCreateGuildTimes(){ return m_other_cfg.free_create_guild_times;}
	ItemID GetGuildExtendMemberItemID(){ return m_other_cfg.extend_member_itemID;}	
	int GetGuildExtendMemberNeedItemCount(int cur_member_count);	
	
	GuildTerritoryWelfCfg::ConfigItem* GetGuildTerritoryWelf(int index);
	
	MemberLuckBlessingCfg * GetMemberLuckBlessingCfg(int lucky_color);
	int GetGuildStorgeOpenCount(int level);

	const GuildSkillCfg::ConfigItem * GetSkillConfig(int skill_index, int skill_level);
	const GuildTotemCfg::ConfigItem * GetTutemConfig(int tutem_level);

	GuildBoxCfg::ConfigItem * GetGuildBoxCfg(int box_level);
	int RandGuildBoxLevel();
	BossGuildRedbagCfg * GetBossGuildRedbagCfg(int index);
	BossGuildRedbagCfg * GetBossGuildRedbagCfgByKillBossTimes(int times);
	int GetPostReliveTimes(int post);
	std::map<int, GuildRedPaperCfg> & GetGuildRedPaperCfgMap(){ return m_guild_red_paper_map;}
	int GetGuildRedPaperSeq(int paper_type, int cond_parm);
	GuildSaiziCfg * GetGuildSaiziCfg(int rank);

	GuildMazeCfg * GetGuildMazeCfg(int layer);
	GuildMazeRankRewardCfg * GetGuildMazeRankRewardCfg(int rank);

	int GetGuildInitMemberByPlatName(std::string plat_name);

	const GuildPlatNameCfg *GetGuildPlatNameCfg(std::string plat_name);

	bool NowCanStartGuildYunBiao(int hour, int min);
	bool GetBiaoChePos(int seq, Posi *get_pos);
	int GetBiaoCheGoalPercent(int seq);
	int GetBiaoCheSceneID() { return m_biaoche_sceneid; }
	int GetBiaoCheMonsterID() { return m_biaoche_monsterid; }
	int GetBiaoCheAddMaxHp(int world_lv);
	int GetBiaoCheExpInterval() { return m_husong_exp_interval; }
	int GetBiaoCheReportPosInterval() { return m_biaoche_report_pos_interval; }
	int GetBiaoCheBehurtReportInterval() { return m_biaoche_be_hurt_report_interval; }
	int GetGaveHusongExpRange() { return m_husong_exp_range; }
	int GetMemberRobBiaoCheCountMax() { return m_member_rob_count_max; }
	int GetGuildRobBiaoCheCountMax() { return m_guild_rob_count_max; }
	int GetTotalBiaoCheGuildExp(int biaoche_lv);
	int GetRobBiaoCheGuildExp(int biaoche_lv);
	int GetBiaoCheAddExp(int role_level);

	const BiaoCheLvInfo *GetBiaoCheInfoCfg(int biaoche_lv);

	const GuildRumorCfg *GetGuildRumorCfg(int rumor_type);

	// 仙盟工资 -------------------------------------------------------
	int GetGongziRate(int guild_post);   // 获取工资比例
	const GuildGongziGiveCfg *GetGongziGiveCfg(int boss_id);

	bool CheckItemRare(ItemID item_id);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitJuanXianCfg(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitExchangeCfg(PugiXmlNode RootElement);
	int InitMemberLuckyCfg(PugiXmlNode RootElement);
	int InitStorgeCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	int InitTotemCfg(PugiXmlNode RootElement);
	int InitBoxCfg(PugiXmlNode RootElement);
	int InitTerrityoryWelfCfg(PugiXmlNode RootElement);
	int InitExtendMemberCfg(PugiXmlNode RootElement);
	int InitPostReliveTimesCfg(PugiXmlNode RootElement);
	int InitBossGuildRedbagCfg(PugiXmlNode RootElement);
	int InitGuildRedPaperCfg(PugiXmlNode RootElement);
	int InitGuildSaiziCfg(PugiXmlNode RootElement);
	int InitGuildMazeCfg(PugiXmlNode RootElement);
	int InitGuildMazeRankRewardCfg(PugiXmlNode RootElement);
	int InitGuilInitMemberCfg(PugiXmlNode RootElement);
	int InitPlatNameCfg(PugiXmlNode RootElement);

	int InitYunbiaoOtherCfg(PugiXmlNode RootElement);
	int InitYunbiaoInfoCfg(PugiXmlNode RootElement);
	int InitYunbiaoExpCfg(PugiXmlNode RootElement);
	int InitYunbiaoAttrCfg(PugiXmlNode RootElement);
	int InitYunbiaoPathCfg(PugiXmlNode RootElement);
	int InitGuildRumorCfg(PugiXmlNode RootElement);

	int InitRareItemCfg(PugiXmlNode RootElement);

	GuildOtherConfig m_other_cfg;
	GuildLevelConfig m_level_cfg;
	GuildJuanXianConfig m_juanxian_cfg;
	GuildExchangeConfig m_exchange_cfg;
	GuildStorgeCfg m_storge_cfg;
	GuildSkillCfg m_skill_cfg;
	GuildTotemCfg m_totem_cfg;
	GuildBoxCfg m_box_cfg;
	GuildTerritoryWelfCfg m_territory_welf_cfg;
	int m_guild_box_total_rate;

	MemberLuckBlessingCfg m_luck_blessing_cfg_list[GUILD_MEMBER_LUCKY_COLOR_MAX];				// 运势祝福配置

	int m_guild_extend_member_cfg_count;
	GuildExtendMemberCfg m_extend_member_cfg_list[GUILD_EXTEND_MEMBER_CFG_MAX_COUNT];			// 扩展公会成员配置

	std::vector<BossGuildRedbagCfg> m_boss_guild_redbag_cfg;
	int m_post_relive_times[GUILD_POST_MAX];

	std::map<int, GuildRedPaperCfg> m_guild_red_paper_map;

	GuildSaiziCfg m_guild_saizi_cfg[MAX_GUILD_SAIZI_REWARD];

	GuildMazeCfg m_maze_cfg_list[GUILD_MAZE_LAYER_MAX + 1];
	std::vector<GuildMazeRankRewardCfg> m_maze_rank_reward_vec;

	std::map<std::string, int> m_plat_init_member_map;

	//镖车------------------------------------------------------------------------------
	struct YunBiaoLimitTm
	{
		YunBiaoLimitTm() : hour(0), minute(0) {}

		int hour;
		int minute;
	};

	YunBiaoLimitTm m_start_limit;							// 开始时间
	YunBiaoLimitTm m_end_limit;								// 结束时间

	static const int BIAOCHE_POS_MAX = 32;

	int m_biaoche_sceneid;									// 镖车场景
	int m_pos_count;										// 镖车移动坐标个数
	Posi m_pos_list[BIAOCHE_POS_MAX];						// 镖车坐标点
	int m_goal_percent[BIAOCHE_POS_MAX];					// 镖车进度百分比

	int m_biaoche_report_pos_interval;						// 镖车报告位置的间隔
	int m_husong_exp_interval;								// 护送获取经验的间隔
	int m_biaoche_be_hurt_report_interval;					// 镖车被攻击播报的间隔
	int m_husong_exp_range;									// 护送获取经验的范围
	int m_guild_rob_count_max;								// 仙盟抢劫奖励最大次数
	int m_member_rob_count_max;								// 成员抢劫奖励最大次数

	BiaoCheLvInfo m_biaoche_lvinfo_list[GUILD_LEVEL_MAX + 1];

	struct BiaocheHusongExpCfg
	{
		BiaocheHusongExpCfg() : role_level(0), role_add_exp(0) {}

		int role_level;
		int role_add_exp;
	};

	std::vector<BiaocheHusongExpCfg> m_biaoche_husong_exp_vec;

	static const int BIAOCHE_ATTR_MAX = 80;
	struct BiaoCheAttr
	{
		BiaoCheAttr() : world_minlv(0), max_hp(0) {}

		int world_minlv;
		int max_hp;
	};
	BiaoCheAttr m_biaoche_attr_list[BIAOCHE_ATTR_MAX];

	int m_biaoche_monsterid;								// 镖车怪物ID

	std::map<int, GuildRumorCfg> m_guild_rumor_map;

	std::map<std::string, GuildPlatNameCfg> m_plat_name_consume_map;

	// 仙盟工资 -------------------------------------------------------
	int InitGuilGongziCfg(PugiXmlNode RootElement);
	int InitGuildGongziGiveCfg(PugiXmlNode RootElement);

	GuildGongziCfg m_gongzi_cfg_list[GUILD_POST_MAX];
	std::map<int, GuildGongziGiveCfg> m_gongzi_give_map;

	std::set<ItemID> rare_item_set;
};

#endif	// _GUILD_CONFIG_HPP_

