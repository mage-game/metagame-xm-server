#include "rolefbarmordefend.hpp"
#include "protocal/msgfb.h"
#include "obj/character/role.h"
#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/errornum.h"
#include "monster/monsterpool.h"
#include "other/event/eventhandler.hpp"
#include "other/event/eventhandler.hpp"
#include "servercommon/string/gameworldstr.h"
#include "gameworld/world.h"
#include "scene/scene.h"
#include "global/team/team.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/sublock/sublock.hpp"
#include "gameworld/item/knapsack.h"
#include "other/vip/vip.hpp"
#include "other/vip/vipconfig.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "task/newtaskmanager.h"
#include "gameworld/config/guildconfig.hpp"

RoleFBArmorDefend::RoleFBArmorDefend() : m_role(NULL)
{

}

RoleFBArmorDefend::~RoleFBArmorDefend()
{

}

void RoleFBArmorDefend::Init(Role *role, const ArmorDefendFBParam &param)
{
	m_role = role;
	m_am_param = param;
}

void RoleFBArmorDefend::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		m_am_param.OnResetData();
		this->SendInfo();
	}
}

void RoleFBArmorDefend::GetInitParam(ArmorDefendFBParam *param)
{
	*param = m_am_param;
}

void RoleFBArmorDefend::SendInfo()
{
	Protocol::SCArmorDefendRoleInfo tdri;
	tdri.personal_join_times = m_am_param.personal_join_times;
	tdri.personal_buy_join_times = m_am_param.personal_buy_join_times;
	tdri.personal_max_pass_level = m_am_param.personal_max_pass_level;
	tdri.personal_auto_fb_free_times = m_am_param.personal_auto_fb_free_times;
	tdri.personal_item_buy_join_times = m_am_param.personal_item_buy_join_time;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&tdri, sizeof(tdri));
}

bool RoleFBArmorDefend::IsNewPlayer()
{
	int guid_task_id = LOGIC_CONFIG->GetArmorDefendConfig().GetNewPlayerTaskId();
	bool is_complete = false;
	if (!m_role->GetTaskManager()->HasTask(guid_task_id, &is_complete) || is_complete)
	{
		return false;
	}

	return true;
}

bool RoleFBArmorDefend::PersonalFBCanEnter(int level)
{
	int free_join_times = LOGIC_CONFIG->GetArmorDefendConfig().GetOtherCfg().free_join_times;
	if (m_am_param.personal_join_times >= m_am_param.personal_buy_join_times + free_join_times + m_am_param.personal_item_buy_join_time)
	{
		return false;
	}

	const ArmorDefendLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetArmorDefendConfig().GetLevelCfg(level);
	if (NULL == lvl_cfg)
	{
		return false;
	}

	if (m_role->GetLevel() < lvl_cfg->role_level)
	{
		return false;
	}

	if (level > m_am_param.personal_max_pass_level + 1)
	{
		return false;
	}

	if (!m_role->GetRoleStatusManager()->CanEnterFB())
	{
		return false;
	}

	return true;
}

void RoleFBArmorDefend::PersonalFBAutoFB(int level)
{
	int free_join_times = LOGIC_CONFIG->GetArmorDefendConfig().GetOtherCfg().free_join_times;
	if (m_am_param.personal_join_times >= m_am_param.personal_buy_join_times + free_join_times + m_am_param.personal_item_buy_join_time)
	{
		return;
	}

	const ArmorDefendLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetArmorDefendConfig().GetLevelCfg(level);
	if (NULL == lvl_cfg)
	{
		return;
	}

	if (m_role->GetLevel() < lvl_cfg->role_level)
	{
		return;
	}

	if (level > m_am_param.personal_max_pass_level)
	{
		return;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		return;
	}

	const ArmorDefendOtherCfg& other_cfg = LOGIC_CONFIG->GetArmorDefendConfig().GetOtherCfg();

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(other_cfg.auto_fb_empty_grid))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

// 	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
// 	bool is_level_free = (m_role->GetLevel() >= AUTO_FB_FREE_WITH_LEVEL);
// 	bool is_free = false;
// 	int free_autofb_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_ARDEFEND_FB_FREE_AUTO_TIMES);
// 
// 	if (!is_level_free)
// 	{
// 		if (m_am_param.personal_auto_fb_free_times < free_autofb_times)
// 		{
// 			is_free =true;
// 		}
// 		else
// 		{
// 			ItemExtern::ItemConsumeConfig stuff;
// 			stuff.item_id = other_cfg.auto_item_id;
// 			stuff.num = other_cfg.auto_item_count;
// 			stuff.buyable = true;
// 
// 			if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, &stuff, &consume_list))
// 			{
// 				return;
// 			}
// 
// 			if (consume_list.need_gold_num > 0)
// 			{
// 				if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(consume_list.need_gold_num))
// 				{
// 					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 					return;
// 				}
// 			}
// 		}
// 	}

	// 计算掉落
	int index_count = 0;
	int reward_coin = 0;
	long long reward_exp = 0;
	int reward_yuanli = 0;
	static ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT];
	memset(reward_item_list, 0, sizeof(reward_item_list));

	for (int i = 0; i < lvl_cfg->wave_count && i < PERSONAL_TOWERDEFEND_MAX_WAVE_PER_LEVEL; ++ i)
	{
		this->AutoFbDrophelper(lvl_cfg->wave_list[i], reward_item_list, &index_count, &reward_exp, &reward_coin, &reward_yuanli);
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(index_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

// 	if (!is_level_free)
// 	{
// 		// 消耗
// 		if (!is_free)
// 		{
// 			if (m_role->GetSublock()->CheckHasLock())
// 			{
// 				return;
// 			}
// 
// 			if (consume_list.count > 0 && !m_role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "ArmorDefendAutoFB"))
// 			{
// 				return;
// 			}
// 
// 			long long consume_gold = 0, consume_gold_bind = 0;
// 			if (consume_list.need_gold_num > 0 && !m_role->GetKnapsack()->GetMoney()->UseAllGold(consume_list.need_gold_num, "TwoerDefendAutoFB", true, &consume_gold, &consume_gold_bind))
// 			{
// 				return;
// 			}
// 
// 			EventHandler::Instance().OnShopBuyItem(m_role, consume_list.buy_count, consume_list.buy_list, consume_gold, consume_gold_bind);
// 		}
// 		else
// 		{
// 			++ m_am_param.personal_auto_fb_free_times;
// 		}
// 	}

	++ m_am_param.personal_join_times;

	// 给予
	if (index_count > 0)
	{
		m_role->GetKnapsack()->PutList(index_count, reward_item_list, PUT_REASON_ARMOT_DEFEND_AUTO);
	}

	if (reward_coin > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddCoinBind(reward_coin, "PassAutoFB_ArmorDefend");
	}

	if (reward_exp > 0)
	{
		m_role->AddExp(reward_exp, "PassAutoFB_ArmorDefend", Role::EXP_ADD_REASON_DEFAULT);
	}

	if (reward_yuanli > 0)
	{
		m_role->AddYuanli(reward_yuanli, "PassAutoFB_ArmorDefend");
	}

	{
		Protocol::SCAutoFBRewardDetail afrd;
		afrd.fb_type = FBManager::FBCT_ARMOR_FB;
		afrd.reward_coin = reward_coin;
		afrd.reward_exp = reward_exp;
		afrd.reward_yuanli = reward_yuanli;
		afrd.reward_xianhun = 0;
		afrd.reserve_sh = 0;

		afrd.item_count = 0;
		for (int i = 0; i < index_count && afrd.item_count < Protocol::SCAutoFBRewardDetail::REWARD_ITEM_MAX_COUNT; ++ i)
		{
			afrd.item_list[afrd.item_count].item_id = reward_item_list[i].item_id;
			afrd.item_list[afrd.item_count].num = reward_item_list[i].num;
			afrd.item_list[afrd.item_count].is_bind = reward_item_list[i].is_bind;

			++ afrd.item_count;
		}

		int send_len = sizeof(afrd) - sizeof(afrd.item_list[0]) * (Protocol::SCAutoFBRewardDetail::REWARD_ITEM_MAX_COUNT - afrd.item_count);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&afrd, send_len);
	}

	this->SendInfo();

 	EventHandler::Instance().OnEnterArmorDefendFB(m_role,true);
 	EventHandler::Instance().OnPassArmordefendFB(m_role);

//	m_role->GetTaskManager()->OnEnterSceneType(Scene::SCENE_TYPE_PERSONAL_TOWERDEFEND);

	//CommitLog2ComsumeGoldOrGotItem();
}


void RoleFBArmorDefend::OnPersonalFBEnter()
{
	if (!IsNewPlayer())
	{
		++m_am_param.personal_join_times;
		this->SendInfo();
	}
}

void RoleFBArmorDefend::OnPersonalFBFinish(bool is_pass, int scene_id, int clear_wave_count)
{
	const ArmorDefendLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetArmorDefendConfig().GetLevelCfgBySceneID(scene_id);
	if (NULL != lvl_cfg)
	{
		if (is_pass)
		{
			if (lvl_cfg->level > m_am_param.personal_max_pass_level)
			{
				m_am_param.personal_max_pass_level = lvl_cfg->level;
				this->SendInfo();

// 				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_towerdefender_fb_pass,
// 					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), (int)m_am_param.personal_max_pass_level + 1);

// 				if (length > 0)
// 				{
// 					 World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER);
// 				}

				auto guild_rumor_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildRumorCfg(GUILD_EVENT_TYPE_RUMOR_FB_TOWER_DEFEND);
				if (guild_rumor_cfg != NULL)
				{
					if (lvl_cfg->level + 1 >= guild_rumor_cfg->param_1)
					{
						if (guild_rumor_cfg->param_2 > 0)
						{
							if ((lvl_cfg->level + 1) % guild_rumor_cfg->param_2 == guild_rumor_cfg->param_3)
							{
								GuildID guild_id = m_role->GetGuildID();
								if (INVALID_GUILD_ID != guild_id)
								{
									EventHandler::Instance().OnAddGuildRumorEvent(guild_id, m_role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_FB_TOWER_DEFEND, lvl_cfg->level + 1);
								}
							}
						}

					}
				}
			}

			EventHandler::Instance().OnPassArmordefendFB(m_role);
		}

		{
			// 日志
			gamelog::g_log_fb.printf(LL_INFO, "Towerdefend::OnFinish user[%d, %s] role_level[%d], is_pass[%d], level[%d], clear_wave[%d]", 
				m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), (is_pass ? 1 : 0), lvl_cfg->level, clear_wave_count);
		}
	}
}

void RoleFBArmorDefend::PersonalBuyJoinTimes()
{
	int can_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_ARMOR_DEFEND_FB_BUY_TIMES);
	if (m_am_param.personal_buy_join_times >= can_buy_times)
	{
		m_role->NoticeNum(errornum::EN_TOWERDEFEND_FB_BUY_LIMIT_TIMES);
		return;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		return;
	}

	int need_gold = LOGIC_CONFIG->GetArmorDefendConfig().GetBuyJoinTimesCfg(m_am_param.personal_buy_join_times + 1);
	if (need_gold <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "TeamBuyJoinTimes"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	++ m_am_param.personal_buy_join_times;
	this->SendInfo();
	//CommitLog2ComsumeGoldOrGotItem();
}

bool RoleFBArmorDefend::PersonalItemAddJoinTimes(int times)
{
	if (times <= 0 || (m_am_param.personal_item_buy_join_time + times) >= SHRT_MAX) return false;

	m_am_param.personal_item_buy_join_time += times;

	this->SendInfo();

	return true;
}


void RoleFBArmorDefend::OnLogin()
{
	
}

const ArmorDefendFBParam* RoleFBArmorDefend::GetTowerDefFBParam(int level) const
{
	int free_join_times = LOGIC_CONFIG->GetArmorDefendConfig().GetOtherCfg().free_join_times;
	if (m_am_param.personal_join_times >= m_am_param.personal_buy_join_times + free_join_times + m_am_param.personal_item_buy_join_time)
	{
		return NULL;
	}

	const ArmorDefendLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetArmorDefendConfig().GetLevelCfg(level);
	if (NULL == lvl_cfg || m_role->GetLevel() < lvl_cfg->role_level) return NULL;

	if (level > m_am_param.personal_max_pass_level || m_role->GetSublock()->CheckHasLock() || m_role->GetKnapsack()->IsLock())
	{
		return NULL;
	}

	
	return &m_am_param;
}

void RoleFBArmorDefend::AutoFbDrophelper(const ArmorDefendWaveCfg &wave_cfg, ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT], 
										 int *index_count, long long *reward_exp, int *reward_coin, int *reward_yuanli)
{
	if (NULL == index_count)
	{
		return;
	}

	for (int i = 0; i < TOWERDEFEND_PATH_MAX; ++ i)
	{
		int monster_count = wave_cfg.monster_count_list[i];

		for (int j = 0; j < monster_count; ++ j)
		{
			if (wave_cfg.monster_id_list[i] > 0)
			{
				MONSTERPOOL->GetMonsterDrop(wave_cfg.monster_id_list[i], reward_item_list, index_count, reward_exp, reward_coin, reward_yuanli);
			}
		}
	}
}


