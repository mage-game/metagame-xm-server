#ifndef __GUILD_BATTLE_CONFIG_NEW_HPP__
#define __GUILD_BATTLE_CONFIG_NEW_HPP__

#include "gamedef.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/activitydef.hpp"
#include <map>
#include <vector>

struct GuildBattleOtherCfg
{
	GuildBattleOtherCfg():scene_id(0), realive_pos(0,0), huizhang_extra_reward_item_count(0), opengame_day(0), lucky_interval(0), luck_people_num(0), get_score_interva(0), 
		get_score_interva_person(0), effective_distance_person(0), point_guild_score(0), point_person_score(0), big_point_guild_score(0),
		big_point_personscore(0), hurt_point_score(0), kill_score(0), assist_score(0), recalc_time(0), shield_times(0){}
	
	int scene_id;
	Posi realive_pos;
	int huizhang_extra_reward_item_count;
	ItemConfigData huizhang_extra_reward[MAX_ATTACHMENT_ITEM_NUM];
	int opengame_day;					// �ܻ�ȡ���⽱���Ŀ���ʱ�䣨֮�ڣ�
	int lucky_interval;					// ���˽������ʱ��
	ItemConfigData lucky_item;			// ���˽�����Ʒ
	int luck_people_num;				// ��������
	int get_score_interva;
	int get_score_interva_person;
	int effective_distance_person;
	int point_guild_score;
	int point_person_score;
	int big_point_guild_score;
	int big_point_personscore;
	int hurt_point_score;
	int kill_score;
	int assist_score;
	unsigned int recalc_time;
	unsigned int shield_times;
	int mount_special_image_id;
};

struct GuildBattlePersonalRewardCfg
{
	GuildBattlePersonalRewardCfg():reward_index(0),require_credit_min(0),require_credit_max(0),banggong(0), shengwang(0){}

	int reward_index;
	int require_credit_min;
	int require_credit_max;
	int banggong;
	int shengwang;

	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

struct GuildBattleRankRewardCfg
{
	GuildBattleRankRewardCfg():rank(0),base_role_exp(0), banggong(0), shengwang(0), gongzi(0), reward_item_count(0), collective_reward_item_count(0){}
	
	int rank;
	long long base_role_exp;
	int banggong;
	int shengwang;
	int gongzi;
	int reward_item_count;
	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];

	int collective_reward_item_count;
	ItemConfigData collective_reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

struct GuildBattlePointCfg
{
	GuildBattlePointCfg() : boss_id(0), realive_pos(0, 0) {}

	int boss_id;
	Posi realive_pos;
};

enum GUILD_BATTLE_FLAG_HP_WARNING
{
	GUILD_BATTLE_FLAG_HP_WARNING_20 = 20,
	GUILD_BATTLE_FLAG_HP_WARNING_50 = 50,
};

class GuildBattleConfigNew : public ConfigBase
	{
public:

	GuildBattleConfigNew();
	~GuildBattleConfigNew();

	bool Init(const std::string &configname, std::string *er);
	int GetGuildBattleEnterInfo(Posi *pos);
	int GetGuildBattleEnterStandy(Posi *pos) const;
	const GuildBattleOtherCfg & GuildBattleGetOtherCfg(){return m_other_cfg;}
	const GuildBattlePersonalRewardCfg * GetPersonalRewardCfg(int require_credit) const;
	const GuildBattlePersonalRewardCfg * GetPersonalRewardCfgByIndex(int index) const;
	const GuildBattleRankRewardCfg * GetRankRewardCfg(int rank)const;
	const GuildBattlePointCfg * GetGuildBattlePointCfg(int index) const;
	const ItemConfigData * GetDailyReward(int rank);
	bool IsFlag(int monster_id);
	int GetSosCost(int times);
	Posi GetClosestRealivePos(Posi pos);
	bool CheckWarning(int old_percent, int now_percent, int *warning_percent);
	unsigned int GetWarningInterval() { return m_warning_interval_s; }

private:
	int InitGuildBattleOtherCfg(PugiXmlNode RootElement);
	int InitGuildBattlePersonalRewardCfg(PugiXmlNode RootElemen);
	int InitGuildBattleRankRewardCfg(PugiXmlNode RootElemen);
	int InitGuildBattlePointCfg(PugiXmlNode RootElemen);
	int InitDailyRewardCfg(PugiXmlNode RootElemen);
	int InitSosCfg(PugiXmlNode RootElemen);
	int InitRealivePosCfg(PugiXmlNode RootElemen);

	GuildBattleOtherCfg m_other_cfg;

	std::map<int,GuildBattlePersonalRewardCfg> m_personal_reward_list;		// ���˻��ֽ����б�
	std::map<int,GuildBattleRankRewardCfg> m_rank_reward_list;				// �������������б�
	std::vector<GuildBattlePointCfg> m_point_vec;
	ItemConfigData m_daily_reward_item_list[GUILD_BATTLE_NEW_REWARD_CITY_COUNT][MAX_ATTACHMENT_ITEM_NUM];	//ÿ�ս���

	std::vector<int> m_sos_cost_cfg_vec;
	std::vector<Posi> m_realive_pos_cfg_vec;

	static const int WARNING_HP_LIST_LEN = 2;

	unsigned int m_warning_interval_s;											// ����ı������
	int m_warning_percent_list[WARNING_HP_LIST_LEN];							// ������Ѫ���ٷֱ��б�
};

#endif // __GUILD_BATTLE_CONFIG_NEW_HPP__
