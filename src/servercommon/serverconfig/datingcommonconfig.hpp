#ifndef __DATING_CONFIG_HPP__
#define __DATING_CONFIG_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/configcommon.h"

#include "servercommon/datingdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class DatingCommonConfig : public ConfigBase
{
public:
	DatingCommonConfig();
	~DatingCommonConfig();

	bool Init(const std::string &path, std::string *err);

	int GetDatingSceneId() { return m_dating_scene_id; }
	int GetDatingGoldRefreshCost() { return m_gold_refresh_target_list_cost; }
	int GetDatingPresentMoreExpCostCoin() { return m_present_more_exp_cost_coin; }
	int GetDatingDoubleExpCostGold() { return m_present_double_exp_cost_gold; }
	int GetDatingInviteCostGold() { return m_invite_cost_gold; }
	int GetDatingRewardExpBase() { return m_reward_exp_base; }
	int GetDatingRewardCoinBindBase() { return m_reward_coin_bind_base; }
	int GetDatingRewardIntimacyBase() { return m_reward_intimacy_base; }
	int GetDatingQuestionNum() { return m_question_num; }

	const ItemConfigData * GetDatingPresentItem(int present_type);

	bool GetDatingUpdateAddExpPos(Posi &pos, int *range);
	int GetDatingUpdateAddExp(int level);

private:

	struct UpdateAddExpItem
	{
		UpdateAddExpItem() : low_level(0), high_level(0), add_exp(0) {}

		int low_level;
		int high_level;
		int add_exp;
	};

	int m_dating_scene_id;
	int m_gold_refresh_target_list_cost;
	int m_present_more_exp_cost_coin;
	int m_present_double_exp_cost_gold;
	int m_invite_cost_gold;
	int m_reward_exp_base;
	int m_reward_coin_bind_base;
	int m_reward_intimacy_base;

	Posi m_update_add_exp_pos;
	int m_update_add_exp_range;
	int m_update_add_exp;

	int m_question_num;

	ItemConfigData m_more_exp_present_item;
	ItemConfigData m_double_exp_present_item;

	int m_update_add_exp_count;
	UpdateAddExpItem m_update_add_exp_list[DATING_UPDATE_ADD_EXP_ITEM_COUNT];
};

#endif	// __DATING_CONFIG_HPP__

