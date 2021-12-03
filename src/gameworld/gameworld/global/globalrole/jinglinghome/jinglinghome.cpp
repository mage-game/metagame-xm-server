#include "jinglinghome.hpp"

#include "world.h"
#include "servercommon/errornum.h"
#include "engineadapter.h"

#include "global/globalrole/globalrole.hpp"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/usercache/usercache.hpp"

#include "obj/character/role.h"
#include "protocal/msgjingling.h"

#include "config/logicconfigmanager.hpp"
#include "other/jingling/jinglingconfig.hpp"
#include "other/jingling/jinglingmanager.h"
#include "item/itemextern.hpp"
#include "item/knapsack.h"
#include "other/daycounter/daycounter.hpp"

JingLingHome::JingLingHome(GlobalRole *g_role) : IGlobalRoleItem(g_role)
{
}

JingLingHome::~JingLingHome()
{
}

void JingLingHome::Init(const JingLingHomeParam &param)
{
	m_param = param;
}

void JingLingHome::GetAttr(JingLingHomeParam &param)
{
	param = m_param;
}

void JingLingHome::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	memset(m_param.today_record_list, 0, sizeof(m_param.today_record_list));
}

void JingLingHome::OnInfoReq(Role *role)
{
	this->CalcReward();
	this->SendInfo(role, Protocol::JING_LING_HOME_REASON_DEF);
	if (role->GetUID() == m_role_id)
	{
		this->SendRobRecord(role);
	}
}

void JingLingHome::OnPutJingLing(Role *role, short jingling_index, short home_index)
{
	if (home_index < 0 || home_index >= JINGLING_MAX_TAKEON_NUM)
	{
		return;
	}

	const ItemGridData *grid_data = role->GetJingLingMgr()->GetJingLingData(jingling_index);
	if (NULL == grid_data)
	{
		return;
	}

	for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; i++)
	{
		if (m_param.jingling_list[i].item_id == grid_data->item_id)
		{
			return;
		}
	}

	this->CalcReward();

	unsigned int now_time = (unsigned int)(EngineAdapter::Instance().Time());

	m_param.jingling_list[home_index].item_id = grid_data->item_id;
	m_param.jingling_list[home_index].capability = JingLingManager::CalcJingLingCapability(grid_data);
	m_param.jingling_list[home_index].reward_beging_time = now_time;
	if (0 == m_param.jingling_list[home_index].last_get_time)
	{
		m_param.jingling_list[home_index].last_get_time = now_time;
	}

	this->OnDataChange();

	this->SendInfo(role, Protocol::JING_LING_HOME_REASON_PUT);
}

void JingLingHome::OnTakeOutJingLing(Role *role, short home_index)
{
	if (home_index < 0 || home_index >= JINGLING_MAX_TAKEON_NUM)
	{
		return;
	}

	this->CalcReward();

	m_param.jingling_list[home_index].item_id = 0;
	m_param.jingling_list[home_index].capability = 0;
	m_param.jingling_list[home_index].reward_beging_time = 0;

	this->OnDataChange();

	this->SendInfo(role, Protocol::JING_LING_HOME_REASON_TAKE_OUT);
}

void JingLingHome::OnQuick(Role *role, short home_index)
{
	if (home_index < 0 || home_index >= JINGLING_MAX_TAKEON_NUM)
	{
		return;
	}

	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (other_cfg == nullptr)
	{
		return;
	}

	if (m_param.jingling_list[home_index].reward_beging_time < (unsigned int)other_cfg->home_quick_time)
	{
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg->home_quick_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	role->GetKnapsack()->GetMoney()->UseGold(other_cfg->home_quick_gold, "JingLingHomeQuick");

	m_param.jingling_list[home_index].reward_beging_time -= other_cfg->home_quick_time;
	if (m_param.jingling_list[home_index].last_get_time > (unsigned int)other_cfg->home_quick_time)
	{
		m_param.jingling_list[home_index].last_get_time -= other_cfg->home_quick_time;
	}

	this->CalcReward();

	this->SendInfo(role, Protocol::JING_LING_HOME_REASON_QUICK);
}

void JingLingHome::OnGetReward(Role *role, short home_index)
{
	if (home_index < 0 || home_index >= JINGLING_MAX_TAKEON_NUM)
	{
		return;
	}

	this->CalcReward();

	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (other_cfg == nullptr)
	{
		return;
	}

	unsigned int now_time = (unsigned int)(EngineAdapter::Instance().Time());

	JingLingHomeItem &item = m_param.jingling_list[home_index];
	if (item.reward_times <= 0)
	{
		return;
	}

	if (item.last_get_time + other_cfg->home_get_reward_interval > now_time)
	{
		role->NoticeNum(errornum::EN_JINGLING_HOME_GET_TIME_LIMIT);
		return;
	}

	item.reward_times = 0;
	item.reward_beging_time = now_time;
	item.last_get_time = now_time;

	role->AddLingJing(item.reward_lingjing, "JingLingHome");
	item.reward_lingjing = 0;

	if (item.reward_hunli > 0)
	{
		role->AddHunli(item.reward_hunli, "JingLingHome");
		item.reward_hunli = 0;
	}

	static const int MAX_INDEX_COUNT = JING_LING_HOME_REWARD_ITEM_MAX * MAX_ATTACHMENT_ITEM_NUM;
	ItemConfigData item_data_list[MAX_INDEX_COUNT];
	int index_count = 0;

	for (int i = 0; i < JING_LING_HOME_REWARD_ITEM_MAX; i++)
	{
		if (0 == item.reward_item_list[i].item_id) break;

		if (!ItemExtern::PutInItemConfigList(MAX_INDEX_COUNT, item_data_list, &index_count,
			item.reward_item_list[i].item_id, item.reward_item_list[i].item_num, true))
		{
			break;
		}
	}

	memset(item.reward_item_list, 0, sizeof(item.reward_item_list));

	if (index_count > 0)
	{
		if (role->GetKnapsack()->CheckEmptyGridNoLessThan(index_count))
		{
			role->GetKnapsack()->PutList(index_count, item_data_list, PUT_REASON_JING_LING_HOME);
		}
		else
		{
			for (int i = 0; i < index_count && i <= MAX_INDEX_COUNT - MAX_ATTACHMENT_ITEM_NUM; i += MAX_ATTACHMENT_ITEM_NUM)
			{
				role->GetKnapsack()->PutListOrMail(item_data_list + i, PUT_REASON_JING_LING_HOME);
			}
		}
	}

	this->OnDataChange();

	this->SendInfo(role, Protocol::JING_LING_HOME_REASON_GET_REWARD);
}

void JingLingHome::OnBeRob(Role *role, short jingling_index, short home_index)
{
	if (jingling_index < 0 || jingling_index >= JINGLING_MAX_TAKEON_NUM
		|| home_index < 0 || home_index >= JINGLING_MAX_TAKEON_NUM)
	{
		return;
	}

	this->CalcReward();

	const ItemGridData *grid_data = role->GetJingLingMgr()->GetJingLingData(jingling_index);
	if (NULL == grid_data)
	{
		return;
	}

	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (other_cfg == nullptr)
	{
		return;
	}

	if (!role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_JING_LING_HOME_ROB_COUNT, other_cfg->home_rob_times_limit))
	{
		role->NoticeNum(errornum::EN_JINGLING_HOME_ROB_TIMES_NOT_ENOUGH);
		return;
	}

	JingLingHome *jingling_home = GLOBAL_ROLE_MGR.GeJingLingHome(role->GetUID());
	if (NULL == jingling_home)
	{
		return;
	}

	if (jingling_home->GetRobTimes(m_role_id) >= other_cfg->home_rob_repeat_limit)
	{
		role->NoticeNum(errornum::EN_JINGLING_HOME_ROB_REPEAT_TOO_MORE);
		return;
	}

	JingLingHomeItem &item = m_param.jingling_list[home_index];
	if (item.reward_times <= 0)
	{
		return;
	}

	int atk_capability = JingLingManager::CalcJingLingCapability(grid_data);
	if (atk_capability <= 0)
	{
		return;
	}

	int reason = Protocol::JING_LING_HOME_REASON_ROB_LOST;

	Protocol::SCJingLingHomeRobAck pro;
	pro.role_id = m_role_id;
	pro.rob_lingjing = 0;
	pro.rob_hunli = 0;
	pro.is_win = 0;
	pro.item_count = 0;

	if (RandomNum(100) < ((atk_capability - item.capability) * 100 / atk_capability + 50))
	{
		role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_JING_LING_HOME_ROB_COUNT);
		reason = Protocol::JING_LING_HOME_REASON_ROB_WIN;
		pro.is_win = 1;

		// 灵精
		if (other_cfg->home_rob_lingjing_per > 0)
		{
			pro.rob_lingjing = item.reward_lingjing * other_cfg->home_rob_lingjing_per / 100;
			if (pro.rob_lingjing > other_cfg->home_rob_lingjing_max) pro.rob_lingjing = other_cfg->home_rob_lingjing_max;

			if (pro.rob_lingjing > 0)
			{
				item.reward_lingjing -= pro.rob_lingjing;
				role->AddLingJing(pro.rob_lingjing, "JingLingHomeRob");
			}
		}

		// 魂力
		if (other_cfg->home_rob_hunli_per > 0)
		{
			pro.rob_hunli = item.reward_hunli * other_cfg->home_rob_hunli_per / 100;
			if (pro.rob_hunli > other_cfg->home_rob_hunli_max) pro.rob_hunli = other_cfg->home_rob_hunli_max;

			if (pro.rob_hunli > 0)
			{
				item.reward_hunli -= pro.rob_hunli;
				role->AddHunli(pro.rob_hunli, "JingLingHomeRob");
			}
		}

		// 物品
		if (other_cfg->home_rob_item_min > 0 && other_cfg->home_rob_item_max >= other_cfg->home_rob_item_min)
		{
			int reward_item_count = other_cfg->home_rob_item_min + RandomNum(other_cfg->home_rob_item_max - other_cfg->home_rob_item_min + 1);

			static const int MAX_INDEX_COUNT = JING_LING_HOME_REWARD_ITEM_MAX * MAX_ATTACHMENT_ITEM_NUM;
			static ItemConfigData item_data_list[MAX_INDEX_COUNT];
			int index_count = 0;

			int total_item_num = 0;
			int max_index = -1;
			for (int i = 0; i < JING_LING_HOME_REWARD_ITEM_MAX; i++)
			{
				if (0 == item.reward_item_list[i].item_id) break;

				max_index = i;
				total_item_num += item.reward_item_list[i].item_num;
			}

			for (int i = 0; i < reward_item_count && total_item_num > 0 && max_index >= 0; i++)
			{
				int rand_num = RandomNum(total_item_num);

				for (int k = 0; k < JING_LING_HOME_REWARD_ITEM_MAX; k++)
				{
					if (rand_num < item.reward_item_list[k].item_num)
					{
						if (ItemExtern::PutInItemConfigList(MAX_INDEX_COUNT, item_data_list, &index_count, item.reward_item_list[k].item_id, 1, true))
						{
							total_item_num--;
							item.reward_item_list[k].item_num--;

							if (pro.item_count < JING_LING_HOME_REWARD_ITEM_MAX)
							{
								pro.item_list[pro.item_count].item_id = item.reward_item_list[max_index].item_id;
								pro.item_list[pro.item_count].item_num = 1;
								pro.item_count++;
							}

							if (item.reward_item_list[k].item_num <= 0)
							{
								item.reward_item_list[k] = item.reward_item_list[max_index];
								item.reward_item_list[max_index].item_id = 0;
								item.reward_item_list[max_index].item_num = 0;
								max_index--;
							}
						}
						break;
					}
					rand_num -= item.reward_item_list[k].item_num;
				}
			}

			if (index_count > 0)
			{
				int empty_num = role->GetKnapsack()->GetCurrKnapscakEmptyGridNum();
				if (empty_num > index_count)
				{
					role->GetKnapsack()->PutList(index_count, item_data_list, PUT_REASON_JING_LING_HOME);
				}
				else
				{
					for (int i = 0; i < index_count && i <= MAX_INDEX_COUNT - MAX_ATTACHMENT_ITEM_NUM; i += MAX_ATTACHMENT_ITEM_NUM)
					{
						role->GetKnapsack()->PutListOrMail(item_data_list + i, PUT_REASON_JING_LING_HOME);
					}
				}
			}
		}

		// 记录
		jingling_home->RecordRob(m_role_id);

		int index = 0;
		unsigned int min_time = m_param.be_rob_record_list[0].rob_time;
		for (int i = 1; i < JING_LING_HOME_ROB_RECORD_MAX; i++)
		{
			if (min_time > m_param.be_rob_record_list[i].rob_time)
			{
				index = i;
				min_time = m_param.be_rob_record_list[i].rob_time;
			}
		}
		if (index >= 0 && index < JING_LING_HOME_ROB_RECORD_MAX)
		{
			m_param.be_rob_record_list[index].role_id = role->GetUID();
			m_param.be_rob_record_list[index].rob_time = (unsigned int)EngineAdapter::Instance().Time();
		}

		this->OnDataChange();

		Role *self_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role_id));
		if (NULL != self_role)
		{
			this->SendRobRecord(self_role);
		}
	}

	this->SendInfo(role, reason);

	int length = sizeof(pro) - (JING_LING_HOME_REWARD_ITEM_MAX - pro.item_count) * sizeof(pro.item_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, length);
}

void JingLingHome::OnRefreshList(Role *role)
{
	this->SendListInfo(role);
}

void JingLingHome::OnReadRobRecord(Role *role)
{
	m_param.read_rob_record_time = (unsigned int)EngineAdapter::Instance().Time();
	this->OnDataChange();
	this->SendRobRecord(role);
}

void JingLingHome::RecordRob(int role_id)
{
	bool is_record = false;

	for (int i = 0; i < JING_LING_HOME_ROB_RECORD_MAX; i++)
	{
		if (0 == m_param.today_record_list[i])
		{
			is_record = true;
			m_param.today_record_list[i] = role_id;
			break;
		}
	}

	if (!is_record)
	{
		for (int i = 0; i < JING_LING_HOME_ROB_RECORD_MAX - 1; i++)
		{
			m_param.today_record_list[i] = m_param.today_record_list[i + 1];
		}

		m_param.today_record_list[JING_LING_HOME_ROB_RECORD_MAX - 1] = role_id;
	}
}

int JingLingHome::GetRobTimes(int role_id)
{
	int times = 0;
	for (int i = 0; i < JING_LING_HOME_ROB_RECORD_MAX; i++)
	{
		if (0 == m_param.today_record_list[i]) break;

		if (role_id == m_param.today_record_list[i])
		{
			times++;
		}
	}

	return times;
}

bool JingLingHome::CanRob()
{
	for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; i++)
	{
		if (0 != m_param.jingling_list[i].item_id)
		{
			return true;
		}
	}

	return false;
}

void JingLingHome::OnTakeOffJingLing(Role *role, ItemID item_id)
{
	short home_index = -1;
	for (short i = 0; i < JINGLING_MAX_TAKEON_NUM; i++)
	{
		if (item_id == m_param.jingling_list[i].item_id)
		{
			home_index = i;
			break;
		}
	}

	if (home_index >= 0)
	{
		this->OnTakeOutJingLing(role, home_index);
		this->OnDataChange();
	}
}

void JingLingHome::OnJingLingCapabilityChange(Role *role)
{
	bool is_change = false;

	for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; i++)
	{
		const ItemGridData *grid_data = role->GetJingLingMgr()->GetJingLingData(i);
		if (NULL == grid_data)
		{
			continue;
		}

		for (int k = 0; k < JINGLING_MAX_TAKEON_NUM; k++)
		{
			if (m_param.jingling_list[k].item_id == grid_data->item_id)
			{
				int capability = JingLingManager::CalcJingLingCapability(grid_data);
				if (capability != m_param.jingling_list[k].capability)
				{
					is_change = true;
					m_param.jingling_list[k].capability = capability;
				}
				break;
			}
		}
	}

	if (is_change)
	{
		this->OnDataChange();
	}
}

void JingLingHome::CalcReward()
{
	unsigned int now_time = (unsigned int)(EngineAdapter::Instance().Time());
	const JingLingOtherCfg *other_cfg = LOGIC_CONFIG->GetJingLingCfg().GetOtherCfg();
	if (other_cfg == nullptr || other_cfg->home_reward_interval <= 0)
	{
		return;
	}

	bool is_change = false;

	short role_level = 0;
	UserCacheNode *cache_node = UserCacheManager::Instance().GetUserNode(m_role_id);
	if (NULL != cache_node)
	{
		role_level = cache_node->level;
	}

	for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; i++)
	{
		JingLingHomeItem &item = m_param.jingling_list[i];
		if (0 == item.item_id) continue;

		const JingLingTalentAttrItemCfg *talent_cfg = LOGIC_CONFIG->GetJingLingCfg().GetTalentAttrCfg(item.item_id);
		if (NULL == talent_cfg) continue;

		if (now_time <= item.reward_beging_time) continue;

		int reward_times = (int)(now_time - item.reward_beging_time) / other_cfg->home_reward_interval;
		if (reward_times + item.reward_times > other_cfg->home_reward_times_limit)
		{
			reward_times = other_cfg->home_reward_times_limit - item.reward_times;
		}

		if (reward_times <= 0) continue;

		is_change = true;

		item.reward_times += reward_times;
		item.reward_beging_time += other_cfg->home_reward_interval * reward_times;
		item.reward_lingjing += talent_cfg->home_reward_lingjing * reward_times;
		if (role_level >= other_cfg->home_reward_hunli_level)
		{
			item.reward_hunli += talent_cfg->home_reward_hunli * reward_times;
		}

		for (int k = 0; k < reward_times; k++)
		{
			if (talent_cfg->home_other_rate > 0 && RandomNum(10000) < talent_cfg->home_other_rate)
			{
				const ItemConfigData *item_cfg = LOGIC_CONFIG->GetJingLingCfg().RandHomeOtherRewardItem(talent_cfg->home_other_type);
				if (NULL == item_cfg) continue;

				for (int j = 0; j < JING_LING_HOME_REWARD_ITEM_MAX; j++)
				{
					if (item_cfg->item_id == item.reward_item_list[j].item_id || 0 == item.reward_item_list[j].item_id)
					{
						item.reward_item_list[j].item_id = item_cfg->item_id;
						item.reward_item_list[j].item_num += item_cfg->num;
						break;
					}
				}
			}
		}
	}

	if (is_change)
	{
		this->OnDataChange();
	}
}

void JingLingHome::SendInfo(Role *role, int reason)
{
	Protocol::SCJingLingHomeInfo pro;
	pro.reason = reason;
	pro.rob_times_of_me = 0;
	JingLingHome *jingling_home = GLOBAL_ROLE_MGR.GeJingLingHome(role->GetUID());
	if (NULL != jingling_home)
	{
		pro.rob_times_of_me = jingling_home->GetRobTimes(m_role_id);
	}
	pro.role_id = m_role_id;
	UserCacheManager::Instance().GetUserName(m_role_id, pro.name);
	memcpy(pro.item_list, m_param.jingling_list, sizeof(pro.item_list));
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));
}

bool JingLingHomeCheckRole(GlobalRole *global_role, void *param)
{
	return global_role->GetOwnerUid() != *((int*)param) && global_role->GetJingLingHome()->CanRob();
}

void JingLingHome::SendListInfo(Role *role)
{
	Protocol::SCJingLingHomeListInfo pro;
	pro.info_count = 0;

	int uid = role->GetUID();
	static GlobalRole *global_role_list[Protocol::SCJingLingHomeListInfo::LIST_MAX];
	int count = GLOBAL_ROLE_MGR.RandGetRole(global_role_list, Protocol::SCJingLingHomeListInfo::LIST_MAX, JingLingHomeCheckRole, &uid);

	for (int i = 0; i < count && i < Protocol::SCJingLingHomeListInfo::LIST_MAX; i++)
	{
		pro.info_list[i].role_id = global_role_list[i]->GetOwnerUid();
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(pro.info_list[i].role_id);
		if (NULL != user_node)
		{
			pro.info_list[i].prof = user_node->prof;
			pro.info_list[i].sex = user_node->sex;
			pro.info_list[i].camp = user_node->camp;
			pro.info_list[i].vip_level = user_node->vip_level;
			pro.info_list[i].avatar_timestamp = user_node->avatar_timestamp;
			user_node->GetName(pro.info_list[i].name);
		}
		pro.info_count++;
	}

	int length = sizeof(pro) - (Protocol::SCJingLingHomeListInfo::LIST_MAX - pro.info_count) * sizeof(pro.info_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, length);
}

void JingLingHome::SendRobRecord(Role *role)
{
	Protocol::SCJingLingHomeRobRecord pro;
	pro.read_rob_record_time = m_param.read_rob_record_time;
	pro.record_count = 0;
	for (int i = 0; i < JING_LING_HOME_ROB_RECORD_MAX; i++)
	{
		if (0 == m_param.be_rob_record_list[i].role_id) break;

		pro.rob_record_list[i].role_id = m_param.be_rob_record_list[i].role_id;
		pro.rob_record_list[i].rob_time = m_param.be_rob_record_list[i].rob_time;
		UserCacheManager::Instance().GetUserName(m_param.be_rob_record_list[i].role_id, pro.rob_record_list[i].name);
		pro.record_count++;
	}

	int length = sizeof(pro) - (JING_LING_HOME_ROB_RECORD_MAX - pro.record_count) * sizeof(pro.rob_record_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, length);
}
