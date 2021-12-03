#include "rolefbchallenge.hpp"
#include "protocal/msgfb.h"
#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "fbchallengeconfig.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "monster/monsterpool.h"
#include "other/event/eventhandler.hpp"
#include "gameworld/world.h"
#include "item/knapsack.h"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "gameworld/npc/transmit/fbmanager.hpp"
#include "scene/challengefbmanager/challengefbmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "other/vip/vip.hpp"
#include "gameworld/config/guildconfig.hpp"

RoleFBChallenge::RoleFBChallenge() : m_role(NULL)
{

}

RoleFBChallenge::~RoleFBChallenge()
{

}

void RoleFBChallenge::Init(Role *role, const ChallengeFBParam &param)
{
	m_role = role;
	m_param = param;	
}

void RoleFBChallenge::GetInitParam(ChallengeFBParam *param)
{
	*param = m_param;
}

void RoleFBChallenge::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
 	if (old_dayid != now_dayid)
 	{
 		m_param.OnResetData();
 		this->SendInfo();
 	}
}

void RoleFBChallenge::OnOperateReq(int type, int param)
{
	switch (type)
	{
	case Protocol::CHALLENGE_FB_OPERATE_TYPE_AUTO_FB:
	{
		this->AutoFBReq(param);
	}
	break;

	case Protocol::CHALLENGE_FB_OPERATE_TYPE_RESET_FB:
	{
		this->ResetLevel(param);
	}
	break;

	case Protocol::CHALLENGE_FB_OPERATE_TYPE_SEND_INFO_REQ:
	{
		this->SendInfo();
	}
	break;

	case Protocol::CHALLENGE_FB_OPERATE_TYPE_BUY_TIMES:
	{
		this->BuyFBTimes();
	}
	break;

	default:
		break;
	}
}

void RoleFBChallenge::SendInfo(short is_auto)
{
 	Protocol::SCChallengeFBInfo cfbi;
	cfbi.enter_count = m_param.enter_count;
	cfbi.buy_count = m_param.buy_count;
	cfbi.is_auto = is_auto;
	cfbi.reserve_sh = 0;
 
 	for (int i = 0; i < ChallengeFBParam::LEVEL_MAX_COUNT; ++ i)
 	{
 		cfbi.level_list[i].is_pass = m_param.level_list[i].is_pass;
 		cfbi.level_list[i].fight_layer = m_param.level_list[i].fight_layer;
 		cfbi.level_list[i].state = m_param.level_list[i].state;
		cfbi.level_list[i].history_max_reward = m_param.level_list[i].history_max_reward;
		cfbi.level_list[i].is_continue = (m_param.level_list[i].pass_time > 0 ? 1 : 0);
		cfbi.level_list[i].use_count = m_param.level_list[i].use_count;
 	}
 
 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cfbi, sizeof(cfbi));
}

bool RoleFBChallenge::CanEnter(int level)
{
 	if (level < 0 || level >= ChallengeFBParam::LEVEL_MAX_COUNT)
 	{
 		return false;
 	}
 
 	const ChallengeFBLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetChallengeFBConfig().GetLevelCfg(level);
 	if (NULL == lvl_cfg)
 	{
 		return false;
 	}
 
 	if (m_role->GetLevel() < lvl_cfg->role_level_limit)
 	{
 		return false;
 	}
 
 	if (level > 0)
 	{
 		if (0 == m_param.level_list[level - 1].is_pass) // 必须先通关前边的关卡，第一关除外
 		{
 			return false;
 		}
 	}

	if (m_param.level_list[level].history_max_reward >= CHALLENGE_FB_REWARD_THREE_STAR)
	{
		return false;
	}

	int day_free_times = LOGIC_CONFIG->GetChallengeFBConfig().GetDayFreeTimes();
	if (0 == m_param.level_list[level].fight_layer && m_param.level_list[level].use_count == 0 && m_param.enter_count >= day_free_times + m_param.buy_count)
	{
		m_role->NoticeNum(errornum::EN_CHALLENGE_FB_JOIN_TIMES_LIMIT);
		return false;
	}

 	if (!m_role->GetRoleStatusManager()->CanEnterFB())
 	{
 		return false;
 	}
 
 	return true;
}

void RoleFBChallenge::AutoFBReq(int level)
{
	Scene * scene = m_role->GetScene();
	if (NULL == scene)
	{
		return;
	}
	else
	{
		if (!Scene::IsStaticScene(scene->GetSceneType())) //活动副本中不允许扫荡
		{
			m_role->NoticeNum(errornum::EN_FB_ATUO_NOT_IN_STATIC_SCENE);
			return;
		}
	}

	if (level < 0 || level >= ChallengeFBParam::LEVEL_MAX_COUNT)
	{
		return;
	}

	ChallengeFBParam::Level &level_param = m_param.level_list[level];

	if (0 == level_param.is_pass)
	{
		m_role->NoticeNum(errornum::EN_CHALLENGE_FB_CURR_LEVEL_LOCK);
		return;
	}
 
	const ChallengeFBLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetChallengeFBConfig().GetLevelCfg(level);
	if (NULL == lvl_cfg)
	{
		return;
	}

	if (m_role->GetLevel() < lvl_cfg->role_level_limit)
	{
		m_role->NoticeNum(errornum::EN_CHALLENGE_FB_LEVEL_LIMIT);
		return;
	}

	int day_free_times = LOGIC_CONFIG->GetChallengeFBConfig().GetDayFreeTimes();
	if (m_param.enter_count >= day_free_times + m_param.buy_count)
	{
		m_role->NoticeNum(errornum::EN_CHALLENGE_FB_JOIN_TIMES_LIMIT);
		return;
	}

	int reward_flag = m_param.level_list[level].history_max_reward;
	if (reward_flag < LOGIC_CONFIG->GetChallengeFBConfig().GetAutoNeedStar())
	{
		return;
	}

	//// 计算奖励
	int index_count = 0;
	int reward_coin = 0;
	long long reward_exp = 0;
	int reward_yuanli = 0;
	static ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT];
	memset(reward_item_list, 0, sizeof(reward_item_list));
	{
		for (int i = 0; i < ChallengeFBLevelCfg::LAYER_PER_LEVEL; ++i)
		{
			const ChallengeFBLevelCfg::Layer &layer_cfg = lvl_cfg->layer_list[i];

			if (layer_cfg.boss_id > 0 && layer_cfg.boss_pos.x > 0 && layer_cfg.boss_pos.y > 0)
			{
				MONSTERPOOL->GetMonsterDrop(layer_cfg.boss_id, reward_item_list, &index_count, &reward_exp, &reward_coin, &reward_yuanli);
			}
		}
	}

	//// 给予
	if (index_count > 0)
	{
		if (m_role->GetKnapsack()->CheckEmptyGridNoLessThan(index_count))
		{
			m_role->GetKnapsack()->PutList(index_count, reward_item_list, PUT_REASON_CHALLENGE_FB);
		}
		else
		{
			for (int i = 0; i < index_count && i <= MonsterInitParam::MAX_DROP_ITEM_COUNT - MAX_ATTACHMENT_ITEM_NUM; i += MAX_ATTACHMENT_ITEM_NUM)
			{
				m_role->GetKnapsack()->PutListOrMail(reward_item_list + i, PUT_REASON_CHALLENGE_FB);
			}
		}
	}

	if (reward_coin > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddCoinBind(reward_coin, "PassAutoFBChallenge");
	}

	if (reward_exp > 0)
	{
		m_role->AddExp(reward_exp, "PassAutoFBChallenge", Role::EXP_ADD_REASON_DEFAULT);
	}

	//场景评级掉落
	ItemConfigData *item = LOGIC_CONFIG->GetChallengeFBConfig().GetRewardItemList(level, reward_flag);
	if (NULL != item)
	{
		m_role->GetKnapsack()->PutListOrMail(item, PUT_REASON_CHALLENGE_FB);
	}

	++m_param.enter_count;

	this->SendInfo(1);
	EventHandler::Instance().OnPassChallengeFB(m_role, lvl_cfg->level);

	{//场景评级掉落(用于扫荡成功后客户端显示)
		Protocol::SCAutoFBRewardDetail afrd;
		afrd.fb_type = FBManager::FBCT_CHALLENGE;

		afrd.item_count = 0;
		for (int i = 0;i< MonsterInitParam::MAX_DROP_ITEM_COUNT && afrd.item_count < Protocol::SCAutoFBRewardDetail::REWARD_ITEM_MAX_COUNT; ++i)
		{
			if (0 != reward_item_list[i].item_id && reward_item_list[i].num > 0)
			{
				afrd.item_list[afrd.item_count].item_id = reward_item_list[i].item_id;
				afrd.item_list[afrd.item_count].num = reward_item_list[i].num;

				afrd.item_count++;
			}
		}

		int len = sizeof(afrd) - sizeof(afrd.item_list[0])*(Protocol::SCAutoFBRewardDetail::REWARD_ITEM_MAX_COUNT - afrd.item_count);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&afrd, len);
	}
}

void RoleFBChallenge::OnFBFinish(bool is_passed, int scene_id, bool is_active_leave_fb, unsigned int start_time)
{
 	{
 		// 在这里修正fight_layer，下面的 逻辑保持以前的不动
 		int belong_layer = -1;
 		const ChallengeFBLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetChallengeFBConfig().GetLevelCfgBySceneID(scene_id, &belong_layer);
		if (NULL == lvl_cfg)
		{
			return;
		}
 		
 		if (NULL != lvl_cfg && lvl_cfg->level >= 0 && lvl_cfg->level < ChallengeFBParam::LEVEL_MAX_COUNT && 
 			belong_layer >= 0 && belong_layer < ChallengeFBLevelCfg::LAYER_PER_LEVEL)
 		{
 			int fight_layer = belong_layer;
 			if (is_passed)
 			{
 				fight_layer += 1;
 			}
 
 			m_param.level_list[lvl_cfg->level].fight_layer = static_cast<char>(fight_layer);
		}

		unsigned int use_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) - start_time;
		if (is_passed && use_time> 0 && lvl_cfg->level >= 0 && lvl_cfg->level < ChallengeFBParam::LEVEL_MAX_COUNT)
		{
			m_param.level_list[lvl_cfg->level].pass_time += use_time;
		}
 	}
 
 	if (is_passed)
 	{
 		const ChallengeFBLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetChallengeFBConfig().GetLevelCfgByEndSceneID(scene_id);
 		if (NULL == lvl_cfg)
 		{
 			return;
 		}
 		if (lvl_cfg->level < 0 || lvl_cfg->level >= ChallengeFBParam::LEVEL_MAX_COUNT)
 		{
 			return;
 		}
 
 		m_role->Recover();
 
		ChallengeFBParam::Level &level_param = m_param.level_list[lvl_cfg->level];
 		if (0 == level_param.is_pass)
 		{
 		//	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_challenge_fb_pass,
 		//		m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), lvl_cfg->level + 1);
 
 		//	if (length > 0)
 		//	{
 		//		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER);
 		//	}

			auto guild_rumor_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildRumorCfg(GUILD_EVENT_TYPE_RUMOR_FB_CHALLENGE);
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
								EventHandler::Instance().OnAddGuildRumorEvent(guild_id, m_role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_FB_CHALLENGE, lvl_cfg->level + 1);
							}
						}
					}
				}
			}

		}

		if (lvl_cfg->level >= 0)
		{
			level_param.is_pass = 1;
			level_param.fight_layer = 0;
			level_param.use_count = 0;

			int reward_flag = CHALLENGE_FB_REWARD_NOT;
			//发奖励
			{
				ItemConfigData *item = LOGIC_CONFIG->GetChallengeFBConfig().GetRewardItemList(lvl_cfg->level, level_param.pass_time, reward_flag);
				if (NULL != item)
				{
					if (static_cast<short>(reward_flag) > level_param.history_max_reward)
					{
						level_param.history_max_reward = static_cast<short>(reward_flag);
					}

					//不发评级礼包
					Knapsack *knapsack = m_role->GetKnapsack();
					if (NULL != knapsack)
					{
						knapsack->PutListOrMail(item, PUT_REASON_CHALLENGE_FB);
					}
				}
			}

			Protocol::SCChallengePassLevel cpl;
			cpl.level = lvl_cfg->level;
			cpl.is_pass = 1;
			cpl.fight_layer = ChallengeFBLevelCfg::LAYER_PER_LEVEL - 1;
			cpl.is_active_leave_fb = is_active_leave_fb ? 1 : 0;
			cpl.reward_flag = reward_flag;
			cpl.pass_time = level_param.pass_time;

			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cpl, sizeof(cpl));

			EventHandler::Instance().OnPassChallengeFB(m_role, lvl_cfg->level);

			level_param.pass_time = 0;
		}
 	}
 	else
 	{
 		// 通关评级（失败）
 		int belong_layer = -1;
 		const ChallengeFBLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetChallengeFBConfig().GetLevelCfgBySceneID(scene_id, &belong_layer);
 		if (NULL == lvl_cfg)
 		{
 			return;
 		}
 		if (lvl_cfg->level < 0 || lvl_cfg->level >= ChallengeFBParam::LEVEL_MAX_COUNT)
 		{
 			return;
		}

 		Protocol::SCChallengePassLevel cpl;
 		cpl.level = lvl_cfg->level;
 		cpl.is_pass = 0;
 		cpl.fight_layer = belong_layer;
 		cpl.is_active_leave_fb = is_active_leave_fb ? 1 : 0;
		cpl.reward_flag = CHALLENGE_FB_REWARD_NOT;
		cpl.pass_time = 0;
 		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cpl, sizeof(cpl));
 	}
}

void RoleFBChallenge::OnEnterFB(int level)
{
	if (level < 0 || level >= ChallengeFBParam::LEVEL_MAX_COUNT)
	{
		return;
	}

	if (0 == m_param.level_list[level].pass_time && 0 == m_param.level_list[level].use_count)
	{
		++m_param.enter_count;
		m_param.level_list[level].use_count = 1;
	}
	this->SendInfo();
}

void RoleFBChallenge::SyncLevelInfo(int scene_id)
{
 	int belong_layer = -1;
 	const ChallengeFBLevelCfg::ConfigItem *cfg = LOGIC_CONFIG->GetChallengeFBConfig().GetLevelCfgBySceneID(scene_id, &belong_layer);
 	if (NULL == cfg) 
 	{
 		return;
 	}
	
 	Protocol::SCChallengePassLevel cpl;
 	cpl.level = cfg->level;
 	cpl.is_pass = 2;
 	cpl.fight_layer = belong_layer;
	cpl.reward_flag = CHALLENGE_FB_REWARD_NOT;
	cpl.pass_time = 0;
	if (cfg->level >= 0 && cfg->level < ChallengeFBParam::LEVEL_MAX_COUNT)
	{
		ChallengeFBParam::Level &level_param = m_param.level_list[cfg->level];
		cpl.pass_time = level_param.pass_time;
	}
	
 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cpl, sizeof(cpl));
}

int RoleFBChallenge::GetEnterCount()
{
	return m_param.enter_count;
}

int RoleFBChallenge::GetFightLayer(int level)
{
 	if (level < 0 || level >= ChallengeFBParam::LEVEL_MAX_COUNT)
 	{
 		return -1;
 	}
 
 	return m_param.level_list[level].fight_layer;
}

int RoleFBChallenge::GetPassLevel()
{
	int pass_level = -1;
	for (int level = 0; level < ChallengeFBParam::LEVEL_MAX_COUNT; level ++)
	{
		if (m_param.level_list[level].is_pass)
		{
			pass_level = level;
		}
	}

	return pass_level;
}

void RoleFBChallenge::ResetLevel(int level)
{
 	if (level < 0 || level >= ChallengeFBParam::LEVEL_MAX_COUNT)
 	{
 		return;
 	}

	if (0 != m_param.level_list[level].is_pass)
	{
		return;
	}

	if (m_param.level_list[level].fight_layer <= 0)
	{
		m_role->NoticeNum(errornum::EN_CHALLENGE_FB_RESET_LAYER_LIMIT);
		return;
	}

	m_param.level_list[level].fight_layer = 0;
	m_param.level_list[level].pass_time = 0;
	m_param.level_list[level].use_count = 0;

	ChallengeFBManager::Instance().OnFbFinish(m_role);			//重置副本也重置掉落统计

	this->SendInfo();
}

void RoleFBChallenge::BuyFBTimes()
{
	int can_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_CHALLENGE_FB_BUY_TIMES);
	
	//if (m_param.buy_count >= LOGIC_CONFIG->GetChallengeFBConfig().GetDayMaxBuyTimes())

	//改成用vip等级来判断可购买次数
	if (m_param.buy_count >= can_buy_times)
	{
		m_role->NoticeNum(errornum::EN_CHALLENGE_FB_BUY_TIMES_LIMIT);
		return;
	}

	const ChallengeFBBuyCostCfg *buy_cost_cfg = LOGIC_CONFIG->GetChallengeFBConfig().GetBuyCostCfg(m_param.buy_count);
	if (NULL == buy_cost_cfg) return;

	if (buy_cost_cfg->gold_cost > 0 && !m_role->GetKnapsack()->GetMoney()->UseAllGold(buy_cost_cfg->gold_cost, "BuyChanllengeTimes"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	++m_param.buy_count;

	this->SendInfo();
}
