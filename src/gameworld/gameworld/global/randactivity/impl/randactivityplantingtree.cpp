#include "randactivityplantingtree.hpp"
#include "obj/character/role.h"
#include "world.h"
#include "servercommon/servercommon.h"
#include "config/logicconfigmanager.hpp"
#include "global/rank/personrank.hpp"
#include "global/rank/rankmanager.hpp"
#include "item/itempool.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "config/activityconfig/randactivityconfig.hpp"

RandActivityPlantingTree::RandActivityPlantingTree(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_PLANTING_TREE)
{

}

RandActivityPlantingTree::~RandActivityPlantingTree()
{

}

void RandActivityPlantingTree::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	m_tree_list.clear();
	m_role_planting_count_map.clear();

	// �����
	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		RankManager::Instance().ClearPersonRankType(PERSON_RANK_TYPE_PLANTING_TREE_PLANTING);
		RankManager::Instance().ClearPersonRankType(PERSON_RANK_TYPE_PLANTING_TREE_WATERING);
	}

	// ��������ʼ��������а���
	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		static PersonRankItem planting_rank_list[RA_TREE_PLANTING_RANK_MAX_COUNT];

		int planting_rank_list_count = RankManager::Instance().GetPersonRank()->GetRankTopItem(PERSON_RANK_TYPE_PLANTING_TREE_PLANTING, RA_TREE_PLANTING_RANK_MAX_COUNT, planting_rank_list);
		if (planting_rank_list_count > 0)
		{
			for (int i = 0; i < planting_rank_list_count && i < RA_TREE_PLANTING_RANK_MAX_COUNT; ++ i)
			{
				if (INVALID_USER_ID != planting_rank_list[i].user_id)
				{
					const RandActivityPlantingTreeRankRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeRankRewardCfg(i + 1);
					if (NULL != reward_cfg)
					{
						static MailContentParam contentparam; contentparam.Reset();

						for (int count = 0; count < MAX_ATTACHMENT_ITEM_NUM; ++count)
						{
							const ItemBase *item_base = ITEMPOOL->GetItem(reward_cfg->planting_rank_reward[count].item_id);
							if (NULL != item_base)
							{
								contentparam.AddItem(reward_cfg->planting_rank_reward[count]);
							}
						}

						int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_planting_tree_planting_reward,
							RAND_ACTIVITY_TYPE_PLANTING_TREE, static_cast<int>(planting_rank_list[i].rank_value), i + 1);
						if (length > 0)
						{
							MailRoute::MailToUser(planting_rank_list[i].user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);

							gamelog::g_log_rand_activity.printf(LL_INFO, "RandActivityPlantingTree::OnSpecialActivityStatusChange rank_type[%d], user[%d,%s], rank[%d], rank_value[%lld], reward_0[%s],reward_1[%s],reward_2[%s]",
								PERSON_RANK_TYPE_PLANTING_TREE_PLANTING, UidToInt(planting_rank_list[i].user_id), planting_rank_list[i].user_name, i + 1, planting_rank_list[i].rank_value,
								ItemConfigDataLog(&reward_cfg->planting_rank_reward[0], NULL), ItemConfigDataLog(&reward_cfg->planting_rank_reward[1], NULL), ItemConfigDataLog(&reward_cfg->planting_rank_reward[2], NULL));
						}
					}
				}
			}
		}

		static PersonRankItem watering_rank_list[RA_TREE_PLANTING_RANK_MAX_COUNT];

		int watering_rank_list_count = RankManager::Instance().GetPersonRank()->GetRankTopItem(PERSON_RANK_TYPE_PLANTING_TREE_WATERING, RA_TREE_PLANTING_RANK_MAX_COUNT, watering_rank_list);
		if (watering_rank_list_count > 0)
		{
			for (int i = 0; i < watering_rank_list_count && i < RA_TREE_PLANTING_RANK_MAX_COUNT; ++ i)
			{
				if (INVALID_USER_ID != watering_rank_list[i].user_id)
				{
					const RandActivityPlantingTreeRankRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeRankRewardCfg(i + 1);
					if (NULL != reward_cfg)
					{
						static MailContentParam contentparam; contentparam.Reset();

						for (int count = 0; count < MAX_ATTACHMENT_ITEM_NUM; ++count)
						{
							const ItemBase *item_base = ITEMPOOL->GetItem(reward_cfg->watering_rank_reward[count].item_id);
							if (NULL != item_base)
							{
								contentparam.AddItem(reward_cfg->watering_rank_reward[count]);
							}
						}

						int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_planting_tree_watering_reward,
							RAND_ACTIVITY_TYPE_PLANTING_TREE, static_cast<int>(watering_rank_list[i].rank_value), i + 1);
						if (length > 0)
						{
							MailRoute::MailToUser(watering_rank_list[i].user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);

							gamelog::g_log_rand_activity.printf(LL_INFO, "RandActivityPlantingTree::OnSpecialActivityStatusChange rank_type[%d], user[%d,%s], rank[%d], rank_value[%lld], reward_0[%s],reward_1[%s],reward_2[%s]",
								PERSON_RANK_TYPE_PLANTING_TREE_WATERING, UidToInt(planting_rank_list[i].user_id), planting_rank_list[i].user_name, i + 1, planting_rank_list[i].rank_value,
								ItemConfigDataLog(&reward_cfg->planting_rank_reward[0], NULL), ItemConfigDataLog(&reward_cfg->planting_rank_reward[1], NULL), ItemConfigDataLog(&reward_cfg->planting_rank_reward[2], NULL));
						}
					}	
				}
			}
		}

		// ��Ϊ�����ʱ�����а��������ж��Ƿ񷢲��뽱
// 		RankManager::Instance().ClearPersonRankType(PERSON_RANK_TYPE_PLANTING_TREE_PLANTING);
// 		RankManager::Instance().ClearPersonRankType(PERSON_RANK_TYPE_PLANTING_TREE_WATERING);
	}
}

void RandActivityPlantingTree::Update(unsigned long interval, time_t now_second)
{
	RandActivity::Update(interval, now_second);

	std::set<long long> tree_to_erase_list;			// ��ɾ����tree_key�б�

	for (std::map<long long, TreeInfo>::iterator tree_it = m_tree_list.begin(); tree_it != m_tree_list.end(); ++ tree_it)
	{
		TreeInfo &tree = tree_it->second;

		// �����ʱ�䵽
		if (now_second >= tree.vanish_time)
		{
			// ��ҵ�ǰֲ�������� -1
			std::map<int, int>::iterator role_planting_count_it = m_role_planting_count_map.find(tree.owner_uid);
			if (role_planting_count_it != m_role_planting_count_map.end())
			{
				-- m_role_planting_count_map[tree.owner_uid];

				if (m_role_planting_count_map[tree.owner_uid] <= 0)
				{
					m_role_planting_count_map.erase(role_planting_count_it);
				}
			}

			tree_to_erase_list.insert(tree_it->first);
		}
	}

	// ɾ����
	for (std::set<long long>::iterator tree_to_erase_it = tree_to_erase_list.begin(); tree_to_erase_it != tree_to_erase_list.end(); ++ tree_to_erase_it)
	{
		m_tree_list.erase(*tree_to_erase_it);
	}
}

void RandActivityPlantingTree::OnPlantingTree(int scene_id, ObjID obj_id, GameName role_name, int role_uid)
{
	if (NULL == role_name)
	{
		return;
	}

	long long key = ConvertParamToLongLong(scene_id, obj_id);

	if (m_tree_list.find(key) == m_tree_list.end())
	{
		m_tree_list[key].owner_uid = role_uid;
		F_STRNCPY(m_tree_list[key].owner_name, role_name, sizeof(GameName));
		m_tree_list[key].vanish_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeTreeLivingTime() * SECOND_PER_MIN;
		m_tree_list[key].watering_times = 0;
		m_tree_list[key].watering_role_uid_list.clear();
	}

	++ m_role_planting_count_map[role_uid];
}

bool RandActivityPlantingTree::OnWateringTree(int scene_id, ObjID obj_id, int role_uid)
{
	long long key = ConvertParamToLongLong(scene_id, obj_id);

	std::map<long long, TreeInfo>::iterator it = m_tree_list.find(key);

	if (it == m_tree_list.end())
	{
		return false;
	}

	TreeInfo &tree = it->second;

	tree.watering_role_uid_list.insert(role_uid);
	++ tree.watering_times;

	if (tree.watering_times >= LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeMaxWateringTimes())
	{
		// ��ҵ�ǰֲ�������� -1
		std::map<int, int>::iterator role_planting_count_it = m_role_planting_count_map.find(tree.owner_uid);
		if (role_planting_count_it != m_role_planting_count_map.end())
		{
			-- m_role_planting_count_map[tree.owner_uid];

			if (m_role_planting_count_map[tree.owner_uid] <= 0)
			{
				m_role_planting_count_map.erase(role_planting_count_it);
			}
		}

		// ɾ����
		m_tree_list.erase(it);
	}

	return true;
}

const RandActivityPlantingTree::TreeInfo* RandActivityPlantingTree::GetTreeInfo(long long key)
{
	std::map<long long, TreeInfo>::iterator it = m_tree_list.find(key);

	if (it == m_tree_list.end())
	{
		return NULL;
	}

	return &(it->second);
}

int RandActivityPlantingTree::GetRolePlantingTreeCount(int role_uid)
{
	int count = 0;

	std::map<int, int>::iterator it = m_role_planting_count_map.find(role_uid);

	if (it != m_role_planting_count_map.end())
	{
		count = it->second;
	}

	return count;
}

bool RandActivityPlantingTree::HasRoleWateredTree(int role_uid, long long key)
{
	std::map<long long, TreeInfo>::iterator it = m_tree_list.find(key);

	if (it == m_tree_list.end())
	{
		return false;
	}

	TreeInfo &tree = it->second;

	if (tree.watering_role_uid_list.find(role_uid) != tree.watering_role_uid_list.end())
	{
		return true;
	}

	return false;
}