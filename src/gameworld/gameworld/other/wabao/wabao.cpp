#include "wabao.hpp"
#include "obj/character/role.h"
#include "scene/sceneregister/sceneregister.hpp"
#include "wabaoconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "scene/scene.h"
#include "protocal/msgother.h"
#include "servercommon/errornum.h"
#include "monster/monsterpool.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "servercommon/string/gameworldstr.h"
#include "item/itempool.h"
#include "other/route/mailroute.hpp"
#include "servercommon/noticenum.h"
#include "world.h"
#include "globalconfig/globalconfig.h"
#include "other/event/eventhandler.hpp"
#include "item/expense/norexitem.hpp"
#include "item/knapsack.h"
#include "other/activedegree/activedegree.hpp"
#include "other/sublock/sublock.hpp"
#include "other/shengesystem/shengesystem.hpp"

Wabao::Wabao() : m_role(NULL), m_baozang_scene_id(0), m_baozang_pos_x(0), m_baozang_pos_y(0), m_baotu_count(0), m_wabao_reward_type(0), m_baotu_fetch_flag(0), m_shouhuzhe_time(0)
{

}

Wabao::~Wabao()
{

}

void Wabao::Init(Role *role, int baozang_scene_id, int baozang_pos_x, int baozang_pos_y, int baotu_count, int wabao_reward_type, unsigned int fetch_flag, unsigned int shouhuzhe_time)
{
	m_role = role;
	m_baozang_scene_id = baozang_scene_id;
	m_baozang_pos_x = baozang_pos_x;
	m_baozang_pos_y = baozang_pos_y;
	m_baotu_count = baotu_count;
	m_wabao_reward_type = wabao_reward_type;
	m_baotu_fetch_flag = fetch_flag;
	m_shouhuzhe_time = shouhuzhe_time;
}

void Wabao::GetInitParam(int *baozang_scene_id, int *baozang_pos_x, int *baozang_pos_y, int *baotu_count, int *wabao_reward_type, unsigned int *fetch_flag, unsigned int *shouhuzhe_time)
{
	*baozang_scene_id = m_baozang_scene_id;
	*baozang_pos_x = m_baozang_pos_x;
	*baozang_pos_y = m_baozang_pos_y;
	*baotu_count = m_baotu_count;
	*wabao_reward_type = m_wabao_reward_type;
	*fetch_flag = m_baotu_fetch_flag;
	*shouhuzhe_time = m_shouhuzhe_time;
}

void Wabao::OnDayChange(int old_dayid, int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		m_baotu_fetch_flag = 0;
	}
}

void Wabao::OnOperaReq(int opera_type, short is_killed)
{
	if (m_role->GetLevel() < LOGIC_CONFIG->GetWabaoCfg().GetOtherCfg().open_level)
	{
		return;
	}
	switch(opera_type)
	{
	case Protocol::CSWabaoOperaReq::OPERA_TYPE_START:
		{
			this->OnStart();
		}
		break;

	case Protocol::CSWabaoOperaReq::OPERA_TYPE_DIG:
		{
			this->OnDig();
		}
		break;

	case Protocol::CSWabaoOperaReq::OPERA_TYPE_QUICK_COMPLETE:
		{
			this->OnQuickComplete();
		}
		break;


	case Protocol::CSWabaoOperaReq::OPERA_TYPE_GET_INFO:
		{
			this->SendWabaoInfo();
		}
		break;

	case Protocol::CSWabaoOperaReq::OPERA_TYPE_SHOUHUZHE_TIME:
		{
			this->OnShouhuzheOperaReq(is_killed);
		}
		break;

	}	
}

void Wabao::SendWabaoInfo()
{
	Protocol::SCWabaoInfo sbi;
	sbi.baozang_scene_id = m_baozang_scene_id;
	sbi.baozang_pos_x = m_baozang_pos_x;
	sbi.baozang_pos_y = m_baozang_pos_y;
	sbi.baotu_count = m_baotu_count;
	sbi.wabao_reward_type = m_wabao_reward_type;
	sbi.wabao_reward_count = 0;
	sbi.shouhuzhe_time = m_shouhuzhe_time;
	memset(sbi.wabao_reward_list, 0, sizeof(sbi.wabao_reward_list));
	sbi.is_quick_complete = 0;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sbi, sizeof(sbi));
}

void Wabao::OnStart()
{
	if (SceneRegister::Instance().CheckSceneIsExist(m_baozang_scene_id))
	{
		return;
	}

	if (m_baotu_count <= 0)
	{
		m_role->NoticeNum(errornum::EN_WABAO_JOIN_TIME_LIMIT);
		return;
	}

	if (!LOGIC_CONFIG->GetWabaoCfg().GetRefreshPos(&m_baozang_scene_id, &m_baozang_pos_x, &m_baozang_pos_y))
	{
		return;
	}

	this->SendWabaoInfo();
}

void Wabao::OnDig()
{
	const static int BAOZANG_DIG_OUT_MAX_LENGTH = 10;
	const static int BAOZANG_DIG_TARGET_POS_LONG_LONG_AWAY = 100;
	const static int BAOZANG_DIG_TARGET_POS_LONG_AWAY = 50;
	const static int BAOZANG_DIG_TARGET_POS_MID_LONG_AWAY = 30;
	const static int BAOZANG_DIG_TARGET_POS_SAMLL_LONG_AWAY = 20;

	if (!SceneRegister::Instance().CheckSceneIsExist(m_baozang_scene_id))
	{
		return;
	}

	if (m_role->GetScene()->GetSceneID() != m_baozang_scene_id)
	{
		m_role->NoticeNum(errornum::EN_WABAO_BAOZANG_NOT_IN_SAME_SCENE);
		return;
	}

	if (WabaoBaozangRewardConfig::BAOZANG_TYPE_INVALID != m_wabao_reward_type)
	{
		return;
	}

	Posi baozang_pos(m_baozang_pos_x, m_baozang_pos_y);

	if ((baozang_pos - m_role->GetPos()).IsLongerThan(BAOZANG_DIG_OUT_MAX_LENGTH))
	{
		if ((baozang_pos - m_role->GetPos()).IsLongerThan(BAOZANG_DIG_TARGET_POS_LONG_LONG_AWAY))
		{
			m_role->NoticeNum(errornum::EN_DIG_BAOZANG_LONG_LONG_AWAY);
		}
		else if ((baozang_pos - m_role->GetPos()).IsLongerThan(BAOZANG_DIG_TARGET_POS_LONG_AWAY))
		{
			m_role->NoticeNum(errornum::EN_DIG_BAOZANG_LONG_AWAY);
		}
		else if ((baozang_pos - m_role->GetPos()).IsLongerThan(BAOZANG_DIG_TARGET_POS_MID_LONG_AWAY))
		{
			m_role->NoticeNum(errornum::EN_DIG_BAOZANG_MID_LONG_AWAY);
		}
		else if ((baozang_pos - m_role->GetPos()).IsLongerThan(BAOZANG_DIG_TARGET_POS_SAMLL_LONG_AWAY))
		{
			m_role->NoticeNum(errornum::EN_DIG_BAOZANG_SMALL_LONG_AWAY);
		}
		else
		{
			m_role->NoticeNum(errornum::EN_DIG_BAOZANG_NEARBY);
		}

		return;
	}
	
	// 活跃奖励：挖宝次数
	m_role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_WABAO);

	// 挖宝过程中两种情况的判断
	const WabaoOtherConfig &other_cfg = LOGIC_CONFIG->GetWabaoCfg().GetOtherCfg();
	if (RandomNum(100) <= other_cfg.succ_prob)
	{
		m_wabao_reward_type = WabaoBaozangRewardConfig::BAOZANG_TYPE_GOLD;
		this->OnFetchReward(false);
	}
	else
	{
		m_wabao_reward_type = WabaoBaozangRewardConfig::BAOZANG_TYPE_OTHER;
		m_shouhuzhe_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.shouhuzhe_time;
		this->SendWabaoInfo();
	}

	{
		// 日志
		gamelog::g_log_wabao.printf(LL_INFO, "Wabao::OnDig user[%d, %s], level[%d]", m_role->GetUID(), m_role->GetName(), m_role->GetLevel());

		ROLE_LOG_QUICK6(LOG_TYPE_WABAO, m_role, m_role->GetLevel(), 0, "Dig", NULL);
	}
}

void Wabao::OnQuickComplete()
{
	if (m_baotu_count <= 0)
	{
		m_role->NoticeNum(errornum::EN_WABAO_JOIN_TIME_LIMIT);
		return;
	}

	Protocol::SCWabaoInfo sbi;
	sbi.baozang_scene_id = 0;
	sbi.baozang_pos_x = 0;
	sbi.baozang_pos_y = 0;
	sbi.shouhuzhe_time = m_shouhuzhe_time;
	sbi.wabao_reward_count = 0;
	sbi.wabao_reward_type = WabaoBaozangRewardConfig::BAOZANG_TYPE_GOLD;
	sbi.is_quick_complete = 1;
	memset(sbi.wabao_reward_list, 0, sizeof(sbi.wabao_reward_list));

	int count = m_baotu_count;
	for (int i = 0; i < count; ++ i)
	{
		const WabaoOtherConfig &other_cfg = LOGIC_CONFIG->GetWabaoCfg().GetOtherCfg();
		if (m_baotu_count <= 0)
		{
			m_role->NoticeNum(errornum::EN_WABAO_JOIN_TIME_LIMIT);
			return;
		}

		if (WabaoBaozangRewardConfig::BAOZANG_TYPE_INVALID != m_wabao_reward_type)
		{
			return;
		}

		if (m_role->GetSublock()->CheckHasLock())
		{
			return;
		}

		if (m_role->GetKnapsack()->IsLock())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
			return;
		}

		if (RandomNum(100) <= other_cfg.succ_prob)
		{
			m_wabao_reward_type = WabaoBaozangRewardConfig::BAOZANG_TYPE_GOLD;
		}
		else
		{
			m_wabao_reward_type = WabaoBaozangRewardConfig::BAOZANG_TYPE_VIOLET_GOLD;
		}

		const WabaoBaozangRewardConfig &reward_cfg = LOGIC_CONFIG->GetWabaoCfg().GetBaozangRewardCfg();
		if (m_wabao_reward_type <= WabaoBaozangRewardConfig::BAOZANG_TYPE_INVALID || m_wabao_reward_type >= WabaoBaozangRewardConfig::BAOZANG_TYPE_OTHER)
		{
			return;
		}
		const WabaoBaozangRewardConfig::ConfigItem &reward_item = reward_cfg.cfg_list[m_wabao_reward_type];

		if (reward_item.total_weight <= 0 || reward_item.cfg_vec.empty())
		{
			return;
		}

		int need_gold = LOGIC_CONFIG->GetWabaoCfg().GetOtherCfg().quick_complete_cost;
		if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(need_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		// 消耗
		if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "WabaoQuickComplete"))
		{
			return;
		}

		// 活跃奖励：挖宝次数
		m_role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_WABAO);

		{
			--m_baotu_count;
			m_wabao_reward_type = WabaoBaozangRewardConfig::BAOZANG_TYPE_INVALID;

			// 根据权重获取随机奖励
			for (int i = 0; i < reward_item.reward_limit; i++)
			{
				int rand_weight = RandomNum(reward_item.total_weight);
				{
					for (std::vector<WabaoBaozangRewardItemConfig>::const_iterator it = reward_item.cfg_vec.begin(); it != reward_item.cfg_vec.end(); it++)
					{
						const WabaoBaozangRewardItemConfig &info = *it;
						if (rand_weight < info.weight)
						{
							if (sbi.wabao_reward_count < WabaoBaozangRewardConfig::MAX_REWARD_LIMIT)
							{
								sbi.wabao_reward_list[sbi.wabao_reward_count++] = info.index;
							}

							// 神格物品直接放入神格虚拟背包
							WabaoBaozangRewardItemConfig info_copy = info;
							for (int attach_num = 0; attach_num < MAX_ATTACHMENT_ITEM_NUM; attach_num++)
							{
								const ItemBase *item_base = ITEMPOOL->GetItem(info.reward_item[attach_num].item_id);
								if (NULL != item_base)
								{
									if (ItemBase::I_TYPE_EXPENSE == item_base->GetItemType())
									{
										const NorexItem *item_norex = dynamic_cast<const NorexItem*>(item_base);
										if (NorexItem::I_NOREX_ITEM_PUT_SHENGE == item_norex->GetUseType() && m_role->GetShengeSystem()->CheckHasSpace())
										{
											if (m_role->GetShengeSystem()->AddShengeByItemId(item_norex->GetItemId()))
											{
												info_copy.reward_item[attach_num].item_id = 0;
											}
										}
									}
								}
							}

							m_role->GetKnapsack()->PutListOrMail(info_copy.reward_item, PUT_REASON_WABAO);
							break;
						}
						else
						{
							rand_weight -= info.weight;
						}
					}
				}
			}

			EventHandler::Instance().OnCompleteWaBao(m_role);
		}

		{
			// 日志
			gamelog::g_log_wabao.printf(LL_INFO, "Wabao::OnQuickComplete user[%d, %s], level[%d], use_all_gold[%d]", m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), need_gold);

			ROLE_LOG_QUICK6(LOG_TYPE_WABAO, m_role, m_role->GetLevel(), 0, "OnQuickComplete", NULL);
		}
	}

	sbi.baotu_count = m_baotu_count;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sbi, sizeof(sbi));
}


// 获取宝图奖励
void Wabao::OnFetchBaotuReward(int total_degree)
{
	const WabaoBaotuConfig &baotu_cfg = LOGIC_CONFIG->GetWabaoCfg().GetBaotuCfg();
	const WabaoOtherConfig &other_cfg = LOGIC_CONFIG->GetWabaoCfg().GetOtherCfg();
	if (m_role->GetLevel() < other_cfg.open_level)
	{
		return;
	}

	for (int i = baotu_cfg.cfg_count - 1; i >= 0; -- i)
	{
		if (total_degree >= baotu_cfg.cfg_list[i].total_degree)
		{
			if (0 != (m_baotu_fetch_flag & (1 << i)))
			{
				break;
			}
			m_baotu_count += baotu_cfg.cfg_list[i].baotu_count;
			m_baotu_fetch_flag |= 1 << i;
		}
	}
	m_baotu_count = m_baotu_count > other_cfg.max_baotu_limit ? other_cfg.max_baotu_limit : m_baotu_count;

	m_wabao_reward_type = WabaoBaozangRewardConfig::BAOZANG_TYPE_INVALID;
	this->SendWabaoInfo();
}

// 获取宝箱奖励
void Wabao::OnFetchReward(bool is_quick_completed)
{
	if (m_baotu_count <= 0 || (!is_quick_completed && 0 == m_baozang_scene_id))
	{
		return;
	}

	// 挖到宝藏
	const WabaoBaozangRewardConfig &reward_cfg = LOGIC_CONFIG->GetWabaoCfg().GetBaozangRewardCfg();
	if (m_wabao_reward_type <= WabaoBaozangRewardConfig::BAOZANG_TYPE_INVALID || m_wabao_reward_type >= WabaoBaozangRewardConfig::BAOZANG_TYPE_OTHER)
	{
		return;
	}
	const WabaoBaozangRewardConfig::ConfigItem &reward_item = reward_cfg.cfg_list[m_wabao_reward_type];
	
	if (reward_item.total_weight <= 0 || reward_item.cfg_vec.empty())
	{
		return;
	}

	{
		// 消耗
		m_baozang_scene_id = 0;
		m_baozang_pos_x = 0;
		m_baozang_pos_y = 0;
		m_shouhuzhe_time = 0;
		--m_baotu_count;
	}

	// 发送奖励时只需奖励和宝图数量的信息
	Protocol::SCWabaoInfo sbi;
	sbi.baozang_scene_id = m_baozang_scene_id;
	sbi.baozang_pos_x = m_baozang_pos_x;
	sbi.baozang_pos_y = m_baozang_pos_y;
	sbi.baotu_count = m_baotu_count;
	sbi.wabao_reward_type = m_wabao_reward_type;
	sbi.wabao_reward_count = 0;
	sbi.shouhuzhe_time = m_shouhuzhe_time;
	sbi.is_quick_complete = is_quick_completed ? 1 : 0;
	memset(sbi.wabao_reward_list, 0, sizeof(sbi.wabao_reward_list));
	m_wabao_reward_type = WabaoBaozangRewardConfig::BAOZANG_TYPE_INVALID;
	
	// 根据权重获取随机奖励
	for (int i = 0; i < reward_item.reward_limit; i++)
	{
		int rand_weight = RandomNum(reward_item.total_weight);
		{
			for (std::vector<WabaoBaozangRewardItemConfig>::const_iterator it = reward_item.cfg_vec.begin(); it != reward_item.cfg_vec.end(); it++)
			{
				const WabaoBaozangRewardItemConfig &info = *it;
				if (rand_weight < info.weight)
				{
					if (sbi.wabao_reward_count < reward_item.reward_limit && sbi.wabao_reward_count < WabaoBaozangRewardConfig::MAX_REWARD_LIMIT)
					{
						sbi.wabao_reward_list[sbi.wabao_reward_count ++] = info.index;
					}

					// 神格物品直接放入神格虚拟背包
					WabaoBaozangRewardItemConfig info_copy = info;
					for (int attach_num = 0; attach_num < MAX_ATTACHMENT_ITEM_NUM; attach_num++)
					{
						const ItemBase *item_base = ITEMPOOL->GetItem(info.reward_item[attach_num].item_id);
						if (NULL != item_base)
						{
							if (ItemBase::I_TYPE_EXPENSE == item_base->GetItemType())
							{
								const NorexItem *item_norex = dynamic_cast<const NorexItem*>(item_base);
								if (NorexItem::I_NOREX_ITEM_PUT_SHENGE == item_norex->GetUseType() && m_role->GetShengeSystem()->CheckHasSpace())
								{
									if (m_role->GetShengeSystem()->AddShengeByItemId(item_norex->GetItemId()))
									{
										info_copy.reward_item[attach_num].item_id = 0;
									}
								}
							}
						}
					}

					m_role->GetKnapsack()->PutListOrMail(info_copy.reward_item, PUT_REASON_WABAO);
					break;
				}
				else
				{
					rand_weight -= info.weight;
				}
			}
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sbi, sizeof(sbi));
	EventHandler::Instance().OnCompleteWaBao(m_role);
}

// 客户端未能在时间的限制内返回结果则自动发送银色宝箱
void Wabao::Update(unsigned int now_second)
{
	if (0 != m_shouhuzhe_time && now_second > m_shouhuzhe_time)		
	{
		m_shouhuzhe_time = 0;
		m_wabao_reward_type = WabaoBaozangRewardConfig::BAOZANG_TYPE_SILVERY;
		this->OnFetchReward(false);
	}
}

// 在规定时间内击败守护者，守护者会扔下一个紫金宝箱，否则会留下一个银宝箱
void Wabao::OnShouhuzheOperaReq(short is_killed)
{
	m_shouhuzhe_time = 0;
	if (0 != is_killed)
	{
		m_wabao_reward_type = WabaoBaozangRewardConfig::BAOZANG_TYPE_VIOLET_GOLD;
		this->OnFetchReward(false);
	}
	else
	{
		m_wabao_reward_type = WabaoBaozangRewardConfig::BAOZANG_TYPE_SILVERY;
		this->OnFetchReward(false);
	}
}

// 达到挖宝开放等级免费送宝图次数
void Wabao::OnRoleLevelChange(int old_level, int level)
{
	const WabaoOtherConfig &other_cfg = LOGIC_CONFIG->GetWabaoCfg().GetOtherCfg();
	if (old_level < other_cfg.open_level && level >= other_cfg.open_level)
	{
		m_baotu_count = other_cfg.init_baotu_times;
		this->OnFetchBaotuReward(m_role->GetActiveDegree()->GetTotalDegree());
	}
}