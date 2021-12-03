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

	void AddTodayFightAllTimes();	//���ĸ�������
	int GetTodayFightAllTimes();
	bool CanDailyFind(int *chapter = NULL, int *level = NULL);

	int GetNeqChapterTotalStar(int chapter);

	int GetNeqFBPassMaxLevel();														// ����װ����ͨ����߲�
	int GetNeqFBPassMaxChapter();													// ����װ����ͨ������½�
	int GetTeamFBPassMaxLevel() { return m_team_param.max_passed_real_layer; }		// ���װ����ͨ����߲�
	int GetTodayTeamFBPassMaxLevel() { return m_team_param.max_layer_today_pass; }	// ���װ��������ͨ����߲�

	int GetTodayTeamFBMultipleRewardTimes() { return m_team_param.today_multiple_buy_times; }				// ���װ��������౶�����������
	void AddTodayTeamFBMultipleRewardTimes(char times) { m_team_param.today_multiple_buy_times += times; }	// ���װ��������౶�����������

	int GetMemberMinLayer() { return m_team_param.member_min_layer; }										// ��ӳ�Ա����С��ʷͨ����
	void SetMemberMinLayer(char member_min_layer) { m_team_param.member_min_layer = member_min_layer; }		// ��ӳ�Ա����С��ʷͨ����

	int GetPreLayerRewardFlag() { return m_team_param.can_give_prelayer_reward; }														// ��ȡ��ʷ������־
	void SetPreLayerRewardFlag(char can_give_prelayer_reward) { m_team_param.can_give_prelayer_reward = can_give_prelayer_reward; }		// ������ʷ������־

	int GetSpecialLimitFlag() { return m_team_param.is_record_today_max_pass_level; }													// �Ƿ��¼�������ͨ����
	void SetSpecialLimitFlag(char flag) { m_team_param.is_record_today_max_pass_level = flag; }

	void GMAddScore(int score);

	void OnLeaveScene();

	int GetNeqMaxStar(int chapter, int level);
	int GetNeqChapterMaxStar(int chapter);
	const NewEquipFBLevel* GetPersonalTargetLevel(int chapter, int level) const { return &m_neq_param.chapter_list[chapter].level_list[level]; }
	const NewEquipFBParam *GetPersonalParam() const { return &m_neq_param; }

	int GiveEquipTeamFbLayerReward(int member_layer);								// ���Ÿ��㽱��

	void FetchDoubleReward(int oper_type);

	bool CheckSceneFetchDoubleReward();

	int GetDailyFindRewardTeam(int list_num_max,ItemConfigData * reward_list, long long *reward_exp);

private:
	
	Role *m_role;

	NewEquipFBParam m_neq_param;								// ����װ��������Ϣ
	EquipFBInfo m_team_param;									// ���װ��������Ϣ

	unsigned int m_equip_team_start_time;						// ��ӱ���ʼʱ��

	ItemConfigData item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	int list_num;
};


#endif

