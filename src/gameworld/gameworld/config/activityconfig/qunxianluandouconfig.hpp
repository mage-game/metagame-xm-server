#ifndef __QUN_XIAN_LUAN_DOU_CONFIG_HPP__
#define __QUN_XIAN_LUAN_DOU_CONFIG_HPP__

#include "servercommon/activitydef.hpp"
#include "gamedef.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct QXLDOtherCfg
{
	QXLDOtherCfg():scene_id(0), room_max_role_num(0), lucky_interval(0), luck_people_num(0),
		shenshi_birthpos(0, 0), shenshi_hp(0), robshenshi_honor(0), robshenshi_sidehonor(0),
		kill_honor(0), assist_honor(0), free_reward_interval_s(0), free_reward_honor(0),
		robot_max_count(0), robot_create_start_interval(0), robot_create_interval(0), robot_birth_pos(0, 0),
		first_reward_item_count(0)
	{}

	int scene_id;
	int room_max_role_num;
	int lucky_interval;
	int luck_people_num;
	ItemConfigData lucky_item;

	Posi shenshi_birthpos;
	int shenshi_hp;
	int robshenshi_honor;
	int robshenshi_sidehonor;

	int kill_honor;
	int assist_honor;

	int free_reward_interval_s;
	int free_reward_honor;

	int robot_max_count;
	unsigned int robot_create_start_interval;
	unsigned int robot_create_interval;
	Posi robot_birth_pos;

	int first_reward_item_count;
	ItemConfigData first_reward[MAX_ATTACHMENT_ITEM_NUM];
};


struct QXLDSideRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 64;

	struct ConfigItem
	{
		ConfigItem() : role_min_level(0), role_max_level(0), win_honor(0), win_exp(0),
			lose_honor(0), lose_exp(0){}

		int role_min_level;
		int role_max_level;
		int win_honor;
		int win_exp;
		ItemConfigData win_item;
		int lose_honor;
		int lose_exp;
		ItemConfigData lose_item;
	};

	QXLDSideRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

class QunXianLuanDouConfig : public ConfigBase
{
public:
	QunXianLuanDouConfig();
	~QunXianLuanDouConfig();

	struct ScoreRewardItem
	{
		ScoreRewardItem() : reward_index(0), need_score(0), reward_honor(0), reward_item_count(0) {}

		int reward_index;
		int need_score;
		int reward_honor;
		int reward_item_count;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
	};


	bool Init(const std::string &path, std::string *err);

	int GetRoomMaxRoleNum() { return m_other_cfg.room_max_role_num; }
	int GetEnterInfo(int side, Posi *enterpos = NULL);
	bool GetRealivePos(int side, Posi *realive_pos);
	bool GetDefenderPos(int side, Posi *defender_pos);
	int GetDefenderMonsterId(int side);
	bool GetShenShiBirthInfo(int *hp, Posi *birth_pos);
	
	int GetKillHonor() { return m_other_cfg.kill_honor; }
	int GetAssistHonor() { return m_other_cfg.assist_honor; }
	int GetRobShenShiHonor() { return m_other_cfg.robshenshi_honor; }
	int GetRobShenShiSideHonor() { return m_other_cfg.robshenshi_sidehonor; }

	int GetFreeRewardIntervalS() { return m_other_cfg.free_reward_interval_s; }
	int GetFreeRewardHonor() { return m_other_cfg.free_reward_honor; }
	const QunXianLuanDouConfig::ScoreRewardItem * GetScoreRewardHonor(int score);
	const QXLDSideRewardConfig::ConfigItem * GetSideRewardCfg(int role_level);
	int GetOpenServerReward(ItemConfigData reward[MAX_ATTACHMENT_ITEM_NUM]);

	int GetLuckInterval() { return m_other_cfg.lucky_interval; }
	int GetLuckPeopleNum() { return m_other_cfg.luck_people_num; }
	ItemConfigData &GetLuckItem() { return m_other_cfg.lucky_item; }

	int GetRobotMaxNum() { return m_other_cfg.robot_max_count; }
	unsigned int GetCreateRobotInterval() { return m_other_cfg.robot_create_interval; }
	unsigned int GetCreateRobotStartInterval() { return m_other_cfg.robot_create_start_interval; }
	Posi GetCreateRobotBirthPos() { return m_other_cfg.robot_birth_pos; }

	int GetTitleID(int side);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitPosCfg(PugiXmlNode RootElement);
	int InitSideRewardCfg(PugiXmlNode RootElement);
	int InitScoreRewardCfg(PugiXmlNode RootElement);
	int InitRankTitleCfg(PugiXmlNode RootElement);

	Posi m_realive_poslist[QUNXIANLUANDOU_SIDE_MAX];					// ∏¥ªÓ◊¯±Í
	Posi m_defender_poslist[QUNXIANLUANDOU_SIDE_MAX];					//  ÿŒ¿◊¯±Í
	int m_defender_monster_id_list[QUNXIANLUANDOU_SIDE_MAX];			//  ÿŒ¿π÷id

	QXLDOtherCfg m_other_cfg;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const static int SCORE_REWARD_ITEM_MAX_COUNT = 32; 
	
	int m_score_reward_cfg_count;										// ª˝∑÷Ω±¿¯≈‰÷√ ˝
	ScoreRewardItem m_score_reward_cfg_list[SCORE_REWARD_ITEM_MAX_COUNT];// ª˝∑÷Ω±¿¯≈‰÷√¡–±Ì

	QXLDSideRewardConfig m_side_reward_cfg;								// ’Û”™ §∏∫Ω±¿¯≈‰÷√

	int m_rank_user_reward_title_id_list[QUNXIANLUANDOU_SIDE_MAX];
	
};

#endif // __QUN_XIAN_LUAN_DOU_CONFIG_HPP__


