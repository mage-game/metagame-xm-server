#ifndef __GONGCHENGZHAN_CONFIG_HPP__
#define __GONGCHENGZHAN_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "engineadapter.h"
#include <vector>
#include <map>

struct GczDailyRewardItemCfg
{
	GczDailyRewardItemCfg() : reward_type(0){}

	enum REWARD_TYPE
	{
		REWARD_TYPE_CHENZHU = 0,
		REWARD_TYPE_NORMAL = 1,
		REWARD_TYPE_MAX,
	};

	int reward_type;

	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];				// 邮件发放奖励
};

struct GczZhangongRewardItemCfg
{
	GczZhangongRewardItemCfg() : zhangong(0), shengwang_reward(0) {}
	
	int zhangong;							// 战功
	int shengwang_reward;					// 声望奖励
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct GongChengZhanWorshipExpConfig
{
	GongChengZhanWorshipExpConfig() : min_level(0),level(0), interval_addexp(0), click_addexp(0) {}

	int min_level;
	int level;
	int interval_addexp;
	long long click_addexp;
};

struct GczOtherCfg
{
	GczOtherCfg() : scene_id(0), zhangong_rank_refresh_time(0), boss1_id(0), boos1_pos(0, 0), boss2_id(0), boos2_pos(0, 0), relive1_pos(0, 0), relive2_pos(0, 0), 
		resource_zuo_xia_pos(0, 0), resource_you_shang_pos(0, 0), resource_pos(0, 0), gong_qiang_pos(0, 0), monster1_id(0) ,monster2_id(0), kill_monster1_zhangong(0), 
		kill_monster2_zhangong(0), hurt_role_01_zhangong(0), kill_role_zhangong(0), hurt_boos_01_zhangong(0), kill_boos_zhangong(0), kill_boos_win_zhangong(0), 
		kill_boos_attack_zhangong(0), online_will_get_zhangong(0), how_much_time_will_get_zhangong(0), chengzhu_skill_id(0), cz_fashion_yifu_id(0), cz_fashion_equi_id(0),
		cz_lover_fashion_yifu_id(0), cz_lover_fashion_equi_id(0), cz_gold_reward(0),
		cz_shengwang_reward(0), win_shengwang_reward(0), win_reward_gongzi(0), fail_shengwang_reward(0), huizhang_extra_reward_item_count(0), lucky_interval(0), luck_people_num(0), opengame_day(0),
		cz_wife_title_id(0), cz_husband_title_id(0), cz_guild_member_title_id(0),
		worship_scene_id(0), worship_start_time_s(0), worship_duration(0), worship_addexp_interval(0), worship_range(0), worship_click_time_limit(0),
		worship_click_cd(0), worship_gather_time(0), exp_factor(0), exp_factor_time(0), exp_factor_click(0), team_reward_item_count(0)
	{}

	int scene_id;						// 场景id
	int zhangong_rank_refresh_time;		// 刷新个人战功排名间隔时间

	int boss1_id;						// 城墙id
	Posi boos1_pos;						// 城墙位置

	int boss2_id;						// 旗帜id
	Posi boos2_pos;						// 旗帜位置

	Posi relive1_pos;					// 攻方复活点位置
	Posi relive2_pos;					// 守方复活点位置

	Posi resource_zuo_xia_pos;			// 资源区左下角坐标
	Posi resource_you_shang_pos;		// 资源区右上角坐标

	Posi resource_pos;					// 资源区位置
	Posi gong_qiang_pos;				// 攻方攻墙点

	int monster1_id;					// 攻方资源区小怪id
	int monster2_id;					// 守方资源区小怪id

	int kill_monster1_zhangong;			// 杀攻方资源区小怪获取战功
	int kill_monster2_zhangong;			// 杀防守资源区小怪获取战功
	
	int hurt_role_01_zhangong;			// 伤人0.1血获得战功
	int kill_role_zhangong;				// 杀人获得战功
	int hurt_boos_01_zhangong;			// 砍旗0.1血获得战功
	int kill_boos_zhangong;				// 砍旗最后一刀获得战功
	int kill_boos_win_zhangong;			// 砍旗后占领方获得战功
	int kill_boos_attack_zhangong;		// 砍旗后其他攻方获得战功
	int online_will_get_zhangong;		// 在线一段时间可以获得多少战功
	int how_much_time_will_get_zhangong;// 每隔多少秒可以获得战功

	int chengzhu_skill_id;				// 城主主动技能id
	int cz_fashion_yifu_id;				// 城主获得时装卡,衣服
	int cz_fashion_equi_id;				// 城主获得时装卡,武器
	int cz_lover_fashion_yifu_id;		// 城主伴侣时装衣服
	int cz_lover_fashion_equi_id;		// 城主伴侣时装武器

	int cz_gold_reward;					// 城主获得非绑元宝
	int cz_shengwang_reward;            // 城主获得声望
	ItemConfigData cz_reward_item[MAX_ATTACHMENT_ITEM_NUM];			// 胜利方会长物品奖励

	int win_shengwang_reward;           // 胜利方声望奖励
	int win_reward_gongzi;				// 胜利方工资奖励
	ItemConfigData win_reward_item[MAX_ATTACHMENT_ITEM_NUM];		// 胜利方物品奖励

	int fail_shengwang_reward;          // 失败方声望奖励
	ItemConfigData fail_reward_item[MAX_ATTACHMENT_ITEM_NUM];	// 失败方物品奖励

	int huizhang_extra_reward_item_count;
	ItemConfigData huizhang_extra_reward[MAX_ATTACHMENT_ITEM_NUM];

	int lucky_interval;					// 幸运奖励间隔时间
	ItemConfigData lucky_item;			// 幸运奖励物品
	int luck_people_num;				// 幸运人数
	int opengame_day;

	int cz_wife_title_id;				// 城主夫人称号
	int cz_husband_title_id;			// 城主夫君称号
	int cz_guild_member_title_id;		// 城主公会成员称号

	int worship_scene_id;
	Posi worship_pos;
	int worship_start_time_s;
	int worship_duration;
	int worship_addexp_interval;
	int worship_range;
	int worship_click_time_limit;		// 攻城战膜拜点击次数限制
	int worship_click_cd;				// 攻城战膜拜点击cd
	int worship_gather_time;			// 膜拜采集时长

	double exp_factor;
	double exp_factor_time;
	double exp_factor_click;

	int team_reward_item_count;											// 集体奖励
	ItemConfigData team_reward_item[MAX_ATTACHMENT_ITEM_NUM];			// 集体奖励
};

class GongChengZhanConfig : public ConfigBase
{
public:
	GongChengZhanConfig();
	~GongChengZhanConfig();

	bool Init(const std::string &path, std::string *err);
	const GczOtherCfg &GetOtherCfg() { return m_other_cfg; }
	GczZhangongRewardItemCfg *GetGczZhangongRewardItemCfgForZhangong(int zhangong);
	int GetEnterPos(bool is_shou_site, Posi *enter_pos);
	const GczDailyRewardItemCfg *GetDailyRewardItemCfg(int reward_type);
	bool GetChengZhuShizhuang(int *yf_part_type, int *yf_part_index, int *wq_part_type, int *wq_part_index, bool is_lover = false);
	const GongChengZhanWorshipExpConfig * GetWorshipExpCfg(const int role_level);
	int GetSosCost(int times);

private:

	GczOtherCfg m_other_cfg;
	
	int InitOther(PugiXmlNode RootElement);
	int InitZhangongReward(PugiXmlNode RootElement);
	int InitDailyReward(PugiXmlNode RootElement);
	int InitWorshipCfg(PugiXmlNode RootElement);
	int InitSosCfg(PugiXmlNode RootElemen);
	std::vector<GczZhangongRewardItemCfg> m_zhangong_reward_vec;
	std::vector<GczDailyRewardItemCfg> m_daily_reward_vec;

	std::vector<GongChengZhanWorshipExpConfig> m_worship_exp_cfg_vec;
	std::vector<int> m_sos_cost_cfg_vec;
};

#endif // __GONGCHENGZHAN_CONFIG_HPP__
