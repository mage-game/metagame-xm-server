#ifndef _ROLEFBEQUIP_HPP_
#define _ROLEFBEQUIP_HPP_

#include "servercommon/fbdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "neqfbconfig.hpp"
#include "protocal/msgfb.h"
class Role;
class FallingItem;
class ItemDataWrapper;

class RoleFBEquip
{
public:

	enum REWARD_TYPE
	{
		NO_REWARD = 0,
		GIVE_REWARD = 1,
	};

	RoleFBEquip();
	~RoleFBEquip();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const NewEquipFBParam &newequipfb_param, const EquipFBInfo &team_param);
	void GetInitParam(NewEquipFBParam *newequipfb_param, EquipFBInfo *team_param);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnPersonalFBEnter(int scene_id);
	void OnTeamFBMemberEnter(int scene_id,unsigned int fb_start_time = 0);

	void OnFBFinish(bool is_finish, bool is_passed, bool can_jump, bool is_all_over, bool is_leave = false);

	void SendFBInfo();

	void OnPersonalBuyMysteryItem(int shop_item_seq);
	void OnTeamBuyMysteryItem(int shop_item_seq);

	void OnPersonalPass(int real_layer);
	void OnTeamPass(int real_layer);

	void OnPassNeqFB(bool passed, int chapter, int level, int seconds);
	void OnNeqFBEnterReq(int chapter, int level);
	void OnNeqTakeStarReward(int chapter, int reward_seq);

	bool CanEnterNeq(int chapter, int level);

	void OnNeqVipBuyTimes();
	bool OnNeqItemBuyTimes(int times);
	void OnNeqAutoReq(int chapter, int level);
	//bool pubNeqAutoReq(int chapter, int level, int need_gold, Protocol::SCAutoFBRewardDetail &afrd);
	void SendNeqFBInfo(short info_type = Protocol::SCNeqFBInfo::NEQ_FB_INFO_DEFAULT);
	void SendNeqRollInfo(short reason, short hit_seq);
	void OnNeqEnter(int chapter, int level);

	void AddTodayFightAllTimes();	//消耗副本次数
	int GetTodayFightAllTimes();
	bool CanDailyFind(int *chapter = NULL, int *level = NULL);

	int GetNeqChapterTotalStar(int chapter);

	int GetNeqFBPassMaxLevel();														// 单人装备本通关最高层
	int GetNeqFBPassMaxChapter();													// 单人装备本通关最高章节
	int GetTeamFBPassMaxLevel() { return m_team_param.max_passed_real_layer; }		// 组队装备本通关最高层
	int GetTodayTeamFBPassMaxLevel() { return m_team_param.max_layer_today_pass; }	// 组队装备本今天通关最高层

	int GetTodayTeamFBMultipleRewardTimes() { return m_team_param.today_multiple_buy_times; }				// 组队装备本今天多倍奖励购买次数
	void AddTodayTeamFBMultipleRewardTimes(char times) { m_team_param.today_multiple_buy_times += times; }	// 组队装备本今天多倍奖励购买次数

	int GetMemberMinLayer() { return m_team_param.member_min_layer; }										// 组队成员中最小历史通过数
	void SetMemberMinLayer(char member_min_layer) { m_team_param.member_min_layer = member_min_layer; }		// 组队成员中最小历史通过数

	int GetPreLayerRewardFlag() { return m_team_param.can_give_prelayer_reward; }														// 获取历史奖励标志
	void SetPreLayerRewardFlag(char can_give_prelayer_reward) { m_team_param.can_give_prelayer_reward = can_give_prelayer_reward; }		// 设置历史奖励标志

	int GetSpecialLimitFlag() { return m_team_param.is_record_today_max_pass_level; }													// 是否记录当天最大通关数
	void SetSpecialLimitFlag(char flag) { m_team_param.is_record_today_max_pass_level = flag; }

	void GMAddScore(int score);

	void OnLeaveScene();

	int GetNeqMaxStar(int chapter, int level);
	int GetNeqChapterMaxStar(int chapter);
	const NewEquipFBLevel* GetPersonalTargetLevel(int chapter, int level) const { return &m_neq_param.chapter_list[chapter].level_list[level]; }
	const NewEquipFBParam *GetPersonalParam() const { return &m_neq_param; }

	int GiveEquipTeamFbLayerReward(int member_layer);								// 发放各层奖励

	void FetchDoubleReward(int oper_type);

	bool CheckSceneFetchDoubleReward();

	int GetDailyFindRewardTeam(int list_num_max,ItemConfigData * reward_list, long long *reward_exp);

private:
	
	Role *m_role;

	NewEquipFBParam m_neq_param;								// 单人装备副本信息
	EquipFBInfo m_team_param;									// 组队装备副本信息

	unsigned int m_equip_team_start_time;						// 组队本开始时间

	ItemConfigData item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	int list_num;
};


#endif

