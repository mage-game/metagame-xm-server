#include "rolefbtowerdefend.hpp"
#include "protocal/msgfb.h"
#include "obj/character/role.h"
#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "servercommon/errornum.h"
#include "monster/monsterpool.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/string/gameworldstr.h"
#include "gameworld/world.h"
#include "scene/scene.h"
#include "global/team/team.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "item/knapsack.h"
#include "task/newtaskmanager.h"
#include "other/vip/vip.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/sublock/sublock.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/fb/towerdefendteamconfig.hpp"
#include "gameworld/friend/teaminfo.hpp"
#include "gameworld/world.h"
#include "holidayguardconfig.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "global/team/teammanager.hpp"
#include "other/xiannv/xiannvmanager.hpp"
#include "gameworld/config/guildconfig.hpp"

RoleFBTowerDefend::RoleFBTowerDefend() : m_role(NULL)
{

}

RoleFBTowerDefend::~RoleFBTowerDefend()
{

}

void RoleFBTowerDefend::Init(Role *role, const TowerDefendFBParam &param)
{
	m_role = role;
	m_td_param = param;
}

void RoleFBTowerDefend::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		m_td_param.OnResetData();
		this->SendInfo();
	}
}

void RoleFBTowerDefend::GetInitParam(TowerDefendFBParam *param)
{
	*param = m_td_param;
}

void RoleFBTowerDefend::SendInfo()
{
	Protocol::SCTowerDefendRoleInfo tdri;
	tdri.personal_join_times = m_td_param.personal_join_times;
	tdri.personal_buy_join_times = m_td_param.personal_buy_join_times;
	tdri.personal_max_pass_level = m_td_param.personal_max_pass_level;
	tdri.personal_auto_fb_free_times = m_td_param.personal_auto_fb_free_times;
	tdri.personal_item_buy_join_times = m_td_param.personal_item_buy_join_time;
	tdri.personal_last_level_star = m_td_param.personal_last_level_star;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&tdri, sizeof(tdri));

	static Protocol::SCHolidayGuardRoleInfo schgri;
	schgri.personal_join_times = m_td_param.personal_holiday_guard_join_times;
	schgri.personal_kill_monster_count = m_td_param.personal_holiday_guard_kill_monster;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&schgri, sizeof(schgri));
}

bool RoleFBTowerDefend::PersonalFBCanEnter(int level)
{
	int free_join_times = LOGIC_CONFIG->GetTowerDefendConfig().GetOtherCfg().free_join_times;
	if (m_td_param.personal_join_times >= m_td_param.personal_buy_join_times + free_join_times + m_td_param.personal_item_buy_join_time)
	{
		return false;
	}

	const TowerDefendLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetLevelCfg(level);
	if (NULL == lvl_cfg)
	{
		return false;
	}

	if (m_role->GetLevel() < lvl_cfg->role_level)
	{
		return false;
	}

	if (level > m_td_param.personal_max_pass_level + 1)
	{
		return false;
	}

	if (!m_role->GetRoleStatusManager()->CanEnterFB())
	{
		return false;
	}

	return true;
}

void RoleFBTowerDefend::PersonalFBAutoFB(int level)
{
	int free_join_times = LOGIC_CONFIG->GetTowerDefendConfig().GetOtherCfg().free_join_times;
	if (m_td_param.personal_join_times >= m_td_param.personal_buy_join_times + free_join_times + m_td_param.personal_item_buy_join_time)
	{
		return;
	}

	const TowerDefendLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetLevelCfg(level);
	if (NULL == lvl_cfg)
	{
		return;
	}

	if (m_role->GetLevel() < lvl_cfg->role_level)
	{
		return;
	}

	if (level > m_td_param.personal_max_pass_level)
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

	const TowerDefendOtherCfg& other_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetOtherCfg();

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(other_cfg.auto_fb_empty_grid))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	bool is_level_free = (m_role->GetLevel() >= AUTO_FB_FREE_WITH_LEVEL);
	bool is_free = false;
	int free_autofb_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_TOWERDEFEND_FB_FREE_AUTO_TIMES);

	if (!is_level_free)
	{
		if (m_td_param.personal_auto_fb_free_times < free_autofb_times)
		{
			is_free = true;
		}
		else
		{
			if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg.auto_cost))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
		}
	}

	// 计算掉落
	int index_count = 0;
	int reward_coin = 0;
	long long reward_exp = 0;
	int reward_yuanli = 0;
	static ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT];
	memset(reward_item_list, 0, sizeof(reward_item_list));

	for (int i = 0; i < lvl_cfg->wave_count && i < PERSONAL_TOWERDEFEND_MAX_WAVE_PER_LEVEL; ++i)
	{
		this->AutoFbDrophelper(lvl_cfg->wave_list[i], reward_item_list, &index_count, &reward_exp, &reward_coin, &reward_yuanli);
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(index_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!is_level_free)
	{
		// 消耗
		if (!is_free)
		{
			if (m_role->GetSublock()->CheckHasLock())
			{
				return;
			}

			if (!m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg.auto_cost, "TwoerDefendAutoFB"))
			{
				return;
			}
		}
		else
		{
			++m_td_param.personal_auto_fb_free_times;
		}
	}

	++m_td_param.personal_join_times;

	// 给予
	if (index_count > 0)
	{
		m_role->GetKnapsack()->PutList(index_count, reward_item_list, PUT_REASON_FB_TOWERDEFEND_TEAM);
	}

	if (reward_coin > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddCoinBind(reward_coin, "PassAutoFB_TowerDefend");
	}

	if (reward_exp > 0)
	{
		m_role->AddExp(reward_exp, "PassAutoFB_TowerDefend", Role::EXP_ADD_REASON_DEFAULT);
	}

	if (reward_yuanli > 0)
	{
		m_role->AddYuanli(reward_yuanli, "PassAutoFB_TowerDefend");
	}

	{
		Protocol::SCAutoFBRewardDetail afrd;
		afrd.fb_type = FBManager::FBCT_TOWERDEFEND_PERSONAL;
		afrd.reward_coin = reward_coin;
		afrd.reward_exp = reward_exp;
		afrd.reward_yuanli = reward_yuanli;
		afrd.reward_xianhun = 0;
		afrd.reserve_sh = 0;

		for (int i = 0; i < index_count && afrd.item_count < Protocol::SCAutoFBRewardDetail::REWARD_ITEM_MAX_COUNT; ++i)
		{
			afrd.item_list[afrd.item_count].item_id = reward_item_list[i].item_id;
			afrd.item_list[afrd.item_count].num = reward_item_list[i].num;
			afrd.item_list[afrd.item_count].is_bind = reward_item_list[i].is_bind;

			++afrd.item_count;
		}

		int send_len = sizeof(afrd) - sizeof(afrd.item_list[0]) * (Protocol::SCAutoFBRewardDetail::REWARD_ITEM_MAX_COUNT - afrd.item_count);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&afrd, send_len);
	}

	this->SendInfo();

	EventHandler::Instance().OnEnterFBTowerDefendPerson(m_role, true);
	EventHandler::Instance().OnPassTowerDefendFB(m_role, lvl_cfg->level);
}

void RoleFBTowerDefend::OnPersonalFBEnter()
{
	if (m_td_param.is_new_player == 0)
	{
		m_td_param.is_new_player = 1;
	}
	else
	{
		++m_td_param.personal_join_times;
	}

	this->SendInfo();
}

void RoleFBTowerDefend::OnPersonalFBFinish(bool is_pass, int scene_id, int star_level)
{
	const TowerDefendLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetLevelCfgBySceneID(scene_id);
	if (NULL != lvl_cfg)
	{
		if (is_pass)
		{
			if (m_td_param.personal_max_pass_level < lvl_cfg->level && star_level > m_td_param.personal_last_level_star)
			{
				m_td_param.personal_last_level_star = star_level;
			}
			
			short pass_star = LOGIC_CONFIG->GetTowerDefendConfig().GetPassStar();
			if (lvl_cfg->level > m_td_param.personal_max_pass_level && star_level == pass_star)
			{

				m_td_param.personal_max_pass_level = lvl_cfg->level;
				m_td_param.personal_last_level_star = 0;

				EventHandler::Instance().OnPassTowerDefendFB(m_role, lvl_cfg->level);

				{//江湖传闻
					auto guild_rumor_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildRumorCfg(GUILD_EVENT_TYPE_RUMOR_ARMDEFEND_FB);
					if (guild_rumor_cfg != NULL)
					{
						if (lvl_cfg->level + 1 >= guild_rumor_cfg->param_1)
						{
							if (guild_rumor_cfg->param_2 > 0)
							{
								if ((lvl_cfg->level + 1) % guild_rumor_cfg->param_2 == guild_rumor_cfg->param_3)
								{
									//江湖传闻
									EventHandler::Instance().OnAddGuildRumorEvent(m_role->GetGuildID(), m_role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_ARMDEFEND_FB, lvl_cfg->level + 1);		//防具本江湖传闻
								}
							}
						}
					}
				}
			}
		}

		this->SendInfo();

		// 日志
		gamelog::g_log_fb.printf(LL_INFO, "Towerdefend::OnFinish user[%d, %s] role_level[%d], is_pass[%d], level[%d], star_level[%d]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), (is_pass ? 1 : 0), lvl_cfg->level, star_level);
	}
}

void RoleFBTowerDefend::PersonalBuyJoinTimes()
{
	int can_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_TOWERDEFEND_FB_BUY_TIMES);
	if (m_td_param.personal_buy_join_times >= can_buy_times)
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

	int need_gold = LOGIC_CONFIG->GetTowerDefendConfig().GetBuyJoinTimesCfg(m_td_param.personal_buy_join_times + 1);
	if (need_gold <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "TowerDefendBuyJoinTimes"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_role->GetRoleActivity()->AddTurntableScore(need_gold);

	++m_td_param.personal_buy_join_times;
	this->SendInfo();
}

bool RoleFBTowerDefend::PersonalItemAddJoinTimes(int times)
{
	if (times <= 0 || (m_td_param.personal_item_buy_join_time + times) >= SHRT_MAX) return false;

	m_td_param.personal_item_buy_join_time += times;

	this->SendInfo();

	return true;
}

bool RoleFBTowerDefend::PersonalHolidayGuardFBCanEnter()
{
	const int free_join_times = LOGIC_CONFIG->GetHolidayGuardConfig().GetOtherCfg().free_join_times;
	if (m_td_param.personal_holiday_guard_join_times >= free_join_times)
	{
		return false;
	}

	if (!m_role->GetRoleStatusManager()->CanEnterFB())
	{
		return false;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_HOLIDAY_GUARD))
	{
		return false;
	}

	unsigned int begin_time = 0, end_time = 0, now_time = 0;

	now_time = (unsigned int)EngineAdapter::Instance().Time();

	unsigned int zero_time = (unsigned int)GetZeroTime(now_time);

	if (LOGIC_CONFIG->GetHolidayGuardConfig().GetFBBeginTimeAndEndTime(&begin_time, &end_time))
	{
		begin_time += zero_time;
		end_time += zero_time;
	}

	if (now_time < begin_time || now_time >= end_time)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return false;
	}

	return true;
}

void RoleFBTowerDefend::OnPersonalHolidayGuardFBEnter()
{
	++m_td_param.personal_holiday_guard_join_times;
	this->SendInfo();
}

void RoleFBTowerDefend::SetPersonalHolidayGuardFBKillMonsterCount(short kill_count)
{
	m_td_param.personal_holiday_guard_kill_monster += kill_count;

	this->SendInfo();
}


int RoleFBTowerDefend::GetDailyFindRewardTeam(int list_num_max,ItemConfigData * reward_list, int * xiannv_shengwu)
{
	int wave = m_role->GetFBTowerDefend()->GetTeamPassdeWave();

	int pass_wave = static_cast<int>(ceil(0.8 * wave));

	if (pass_wave < 0 || pass_wave > 50) return 0;

	// 获取的所有掉落物列表
	int list_num = 0;

	for (int wave = 0; wave <= pass_wave; wave++)
	{
		const TeamTowerDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamWaveCfg(wave);
		if (NULL == wave_cfg)
		{
			continue;
		}

		std::vector<UInt16>::const_iterator it = wave_cfg->dropid_list.begin();
		for (; it != wave_cfg->dropid_list.end(); ++it)
		{
			int drop_item_count = 0;
			ItemConfigData drop_item_list[DropConfig::DROP_ITEM_PROB_MAX];
			drop_item_count = DROPPOOL->GetDropItemList(*it, drop_item_list);

			for (int i = 0; i < drop_item_count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
			{
				if (!ItemExtern::PutInItemConfigList(list_num_max, reward_list, &list_num,
					drop_item_list[i].item_id, drop_item_list[i].num, drop_item_list[i].is_bind))
				{
					break;
				}
			}
		}

		if (xiannv_shengwu != NULL)
		{
			*xiannv_shengwu += wave_cfg->sweep_calue;
		}
	}

	//特殊处理，前面如果随机不出物品，从第1层的掉落再随机100次，出现物品后马上停止
	if (list_num == 0)
	{
		for (int j = 0; j < 100; j++)
		{
			const TeamTowerDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamWaveCfg(0);
			if (wave_cfg != NULL)
			{
				std::vector<UInt16>::const_iterator it = wave_cfg->dropid_list.begin();
				for (; it != wave_cfg->dropid_list.end(); ++it)
				{
					int drop_item_count = 0;
					ItemConfigData drop_item_list[DropConfig::DROP_ITEM_PROB_MAX];
					drop_item_count = DROPPOOL->GetDropItemList(*it, drop_item_list);

					for (int i = 0; i < drop_item_count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
					{
						if (!ItemExtern::PutInItemConfigList(list_num_max, reward_list, &list_num,
							drop_item_list[i].item_id, drop_item_list[i].num, drop_item_list[i].is_bind))
						{
							break;
						}
					}
				}
			}

			if (list_num > 0)break;
		}
	}

// 	std::vector<UInt16>::const_iterator extra_reward_it = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg().dropid_list.begin();
// 	for (; extra_reward_it != LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg().dropid_list.end(); ++extra_reward_it)
// 	{
// 		int extra_reward_count = 0;
// 		ItemConfigData extra_reward_list[DropConfig::DROP_ITEM_PROB_MAX];
// 		extra_reward_count = DROPPOOL->GetDropItemList(*extra_reward_it, extra_reward_list);
// 
// 		for (int i = 0; i < extra_reward_count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
// 		{
// 			if (!ItemExtern::PutInItemConfigList(list_num_max, reward_list, &list_num,
// 				extra_reward_list[i].item_id, extra_reward_list[i].num, extra_reward_list[i].is_bind))
// 			{
// 				break;
// 			}
// 		}
// 	}

	return list_num;
}

void RoleFBTowerDefend::LeaderSetAttrType(int uid, char type)
{
	if (0 == uid) // 客户端发0的意思是查询 
	{
		this->BroadcastAttrType();
	}
	else if (type >= TEAM_TOWERDEFEND_ATTRTYPE_INVALID && type < TEAM_TOWERDEFEND_ATTRTYPE_MAX)
	{
		if (!m_role->GetTeamInfo()->InTeam())
		{
			return;
		}

		Team *team = m_role->GetTeamInfo()->GetMyTeam();
		if (NULL == team)
		{
			return;
		}

		if (!team->IsLeader(m_role->GetUserId()))
		{
			m_role->NoticeNum(errornum::EN_TEAM_TOWERDEFEND_MUST_BE_TEAM_LEADER);
			return;
		}

		if (NULL == team->GetTeamMember(IntToUid(uid)))
		{
			return;
		}

		Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
		if (NULL == target_role)
		{
			return;
		}

		target_role->GetFBTowerDefend()->SetTeamAttrType(type);
	}
}

void RoleFBTowerDefend::BroadcastAttrType()
{
	static Protocol::SCTeamTowerDefendAttrType ttdat;
	memset(ttdat.attr_list, 0, sizeof(ttdat.attr_list));
	if (m_role->GetTeamInfo()->InTeam())
	{
		Team *team = m_role->GetTeamInfo()->GetMyTeam();
		if (NULL != team)
		{
			int join_user_list[MAX_TEAM_MEMBER_NUM]; memset(join_user_list, 0, sizeof(join_user_list));
			int join_user_count = team->GetMemberUidList(join_user_list, MAX_TEAM_MEMBER_NUM);

			for (int i = 0; i < join_user_count; ++i)
			{
				Role *join_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(join_user_list[i]));
				if (NULL != join_role)
				{
					ttdat.attr_list[i].uid = join_role->GetUID();
					ttdat.attr_list[i].attr_type = join_role->GetFBTowerDefend()->GetTeamAttrType();
				}
			}

			team->BroadcastToAllMember((const char *)&ttdat, sizeof(ttdat));
		}
	}
	else
	{
		ttdat.attr_list[0].uid = m_role->GetUID();
		ttdat.attr_list[0].attr_type = m_td_param.team_attrtype;
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&ttdat, sizeof(ttdat));
	}
}

void RoleFBTowerDefend::OnJoinTeam()
{
	m_td_param.team_attrtype = TEAM_TOWERDEFEND_ATTRTYPE_INVALID;

	this->BroadcastAttrType();
}

void RoleFBTowerDefend::OnLeaveTeam()
{
	m_td_param.team_attrtype = TEAM_TOWERDEFEND_ATTRTYPE_INVALID;

	this->BroadcastAttrType();
}

void RoleFBTowerDefend::SetTeamAttrType(char type)
{
	if (type >= TEAM_TOWERDEFEND_ATTRTYPE_INVALID && type < TEAM_TOWERDEFEND_ATTRTYPE_MAX)
	{
		m_td_param.team_attrtype = type;
	}

	this->BroadcastAttrType();
}

void RoleFBTowerDefend::SetTeamMemberMinWave(short min_wave)
{
	if (min_wave >= 0 && min_wave <= LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamDefendWaveCount())
	{
		m_td_param.team_member_min_pass = min_wave;
	}
	else
	{
		m_td_param.team_member_min_pass = 0;
	}
}
	
short RoleFBTowerDefend::GetTeamMemberMinWave()
{
	return m_td_param.team_member_min_pass;
}
void RoleFBTowerDefend::GiveTeamWaveReward(ItemConfigData *item_list, int max_num, int & angry_value)
{
	if (m_td_param.team_member_min_pass <= 0 || m_td_param.team_member_min_pass > LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamDefendWaveCount()) return;

	// 获取的所有掉落物列表
	int list_num = 0;

	// 当前成员最小层之前层次的奖励掉落逐一解析
	for (int wave = 0; wave < m_td_param.team_member_min_pass; wave++)
	{
		const TeamTowerDefendWaveCfg *wave_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamWaveCfg(wave);
		if (nullptr == wave_cfg)
		{
			continue;
		}

		std::vector<UInt16>::const_iterator it = wave_cfg->dropid_list.begin();
		for (; it != wave_cfg->dropid_list.end(); ++it)
		{
			int drop_item_count = 0;
			static ItemConfigData drop_item_list[DropConfig::DROP_ITEM_PROB_MAX];
			drop_item_count = DROPPOOL->GetDropItemList(*it, drop_item_list);

			for (int i = 0; i < drop_item_count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
			{
				if (!ItemExtern::PutInItemConfigList(max_num, item_list, &list_num,
					drop_item_list[i].item_id, drop_item_list[i].num, drop_item_list[i].is_bind))
				{
					break;
				}
			}
		}

		angry_value += wave_cfg->sweep_calue;
	}
	
	// 发放奖励
	for (int i = 0; i < list_num && i <= max_num - MAX_ATTACHMENT_ITEM_NUM; i += MAX_ATTACHMENT_ITEM_NUM)
	{
		m_role->GetKnapsack()->PutListOrMail(item_list + i, PUT_REASON_TOWER_DEFEND_TEAM_WAVE_JUMP);
	}

	m_role->GetXiannvManager()->AddShengwuEssence(angry_value, "RoleFBTowerDefend::GiveTeamWaveReward");

}

void RoleFBTowerDefend::OnLogin()
{
	
}

void RoleFBTowerDefend::AutoFbDrophelper(const TowerDefendWaveCfg &wave_cfg, ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT], 
										 int *index_count, long long *reward_exp, int *reward_coin, int *reward_yuanli)
{
	if (NULL == index_count)
	{
		return;
	}

	for (int i = 0; i < TOWERDEFEND_PATH_MAX; ++i)
	{
		int monster_count = wave_cfg.monster_count_list[i];

		for (int j = 0; j < monster_count; ++j)
		{
			if (wave_cfg.monster_id_list[i] > 0)
			{
				MONSTERPOOL->GetMonsterDrop(wave_cfg.monster_id_list[i], reward_item_list, index_count, reward_exp, reward_coin, reward_yuanli);
			}
		}
	}
}

void RoleFBTowerDefend::SetTeamPassedWave(int wave) 
{
	if (wave > m_td_param.team_max_wave_passed)
	{
		m_td_param.team_max_wave_passed = wave;
	}
}

void RoleFBTowerDefend::FetchDoubleReward(int oper_type)
{
	Protocol::SCFetchDoubleRewardResult info;
	info.fb_type = 0;
	info.reserve_sh = 0;
	info.item_count = 0;
	info.xiannv_shengwu = 0;
	memset(info.item_list, 0, sizeof(info.item_list));

	char today_buy_times = m_role->GetFBTowerDefend()->GetTodayTDMultipleRewardTimes();

	switch (oper_type)
	{
	case Protocol::CSFetchDoubleRewardReq::OPERA_TYPE_INFO_REQ:
	{
		//通知客户端
		info.fb_type = TeamManager::TEAM_TYPE_TEAM_TOWERDEFEND;
		info.today_buy_times = today_buy_times;
		int len = sizeof(info) - sizeof(info.item_list[0]) * (ItemNamespace::MAX_KNAPSACK_GRID_NUM - 1);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, len);

		return;
	}
	break;

	case Protocol::CSFetchDoubleRewardReq::OPERA_TYPE_BUY_DOUBLE_REWARD:
	{
		if (!CheckSceneFetchDoubleReward())
		{
			return;
		}
		
		if (today_buy_times >= 2)
		{
			m_role->NoticeNum(errornum::EN_BUY_MULTIPLE_REWARD_LIMIT);
			return;
		}

		//检查vip等级限制
		if (m_role->GetVip()->GetVipLevel() < LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg().double_reward_need_vip_level)
		{
			m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGHT);
			return;
		}

		//消耗元宝
		if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg().buy_double_reward_need_gold, "BuyTeamTDDoubleReward"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}
	break;

	case Protocol::CSFetchDoubleRewardReq::OPERA_TYPE_BUY_TRIPLE_REWARD:
	{
		if (!CheckSceneFetchDoubleReward())
		{
			return;
		}

		if (today_buy_times >= 2)
		{
			m_role->NoticeNum(errornum::EN_BUY_MULTIPLE_REWARD_LIMIT);
			return;
		}

		//检查vip等级限制
		if (m_role->GetVip()->GetVipLevel() < LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg().triple_reward_need_vip_level)
		{
			m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGHT);
			return;
		}

		//消耗元宝
		if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg().buy_triple_reward_need_gold, "BuyTeamTDTripleReward"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}
	break;

	default:
	{
		return;
	}
	break;

	}

	//奖励
	int pass_wave = m_role->GetFBTowerDefend()->GetTeamPassdeWave();

	if (pass_wave < 0 || pass_wave > 50) return;

	// 获取的所有掉落物列表
	int list_num = 0;
	ItemConfigData item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	memset(item_list, 0, sizeof(item_list));

	for (int wave = 0; wave <= pass_wave; wave++)
	{
		const TeamTowerDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamWaveCfg(wave);
		if (NULL == wave_cfg)
		{
			continue;
		}

		std::vector<UInt16>::const_iterator it = wave_cfg->dropid_list.begin();
		for (; it != wave_cfg->dropid_list.end(); ++it)
		{
			int drop_item_count = 0;
			ItemConfigData drop_item_list[DropConfig::DROP_ITEM_PROB_MAX];
			drop_item_count = DROPPOOL->GetDropItemList(*it, drop_item_list);

			for (int i = 0; i < drop_item_count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
			{
				if (!ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, item_list, &list_num,
					drop_item_list[i].item_id, drop_item_list[i].num, drop_item_list[i].is_bind))
				{
					break;
				}
			}
		}

		info.xiannv_shengwu += wave_cfg->sweep_calue;
	}

	//特殊处理，前面如果随机不出物品，从第1层的掉落再随机100次，出现物品后马上停止
	if (list_num == 0)
	{
		for (int j = 0; j < 100; j++)
		{
			const TeamTowerDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamWaveCfg(0);
			if (wave_cfg != NULL)
			{
				std::vector<UInt16>::const_iterator it = wave_cfg->dropid_list.begin();
				for (; it != wave_cfg->dropid_list.end(); ++it)
				{
					int drop_item_count = 0;
					ItemConfigData drop_item_list[DropConfig::DROP_ITEM_PROB_MAX];
					drop_item_count = DROPPOOL->GetDropItemList(*it, drop_item_list);

					for (int i = 0; i < drop_item_count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
					{
						if (!ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, item_list, &list_num,
							drop_item_list[i].item_id, drop_item_list[i].num, drop_item_list[i].is_bind))
						{
							break;
						}
					}
				}
			}

			if (list_num > 0)break;
		}
	}

	// 发放奖励
	for (int i = 0; i < list_num && i <= ItemNamespace::MAX_KNAPSACK_GRID_NUM - MAX_ATTACHMENT_ITEM_NUM; i += MAX_ATTACHMENT_ITEM_NUM)
	{
		m_role->GetKnapsack()->PutListOrMail(item_list + i, PUT_REASON_TOWER_DEFEND_TEAM_BUY_MULTIPLE_REWARD);
	}

	//额外奖励
	{
		int extra_item_count = 0;
		ItemConfigData extra_item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];

		std::vector<UInt16>::const_iterator extra_reward_it = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg().dropid_list.begin();
		for (; extra_reward_it != LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg().dropid_list.end(); ++extra_reward_it)
		{
			int extra_reward_count = 0;
			ItemConfigData extra_reward_list[DropConfig::DROP_ITEM_PROB_MAX];
			extra_reward_count = DROPPOOL->GetDropItemList(*extra_reward_it, extra_reward_list);

			for (int i = 0; i < extra_reward_count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
			{
				if (!ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, extra_item_list, &extra_item_count,
					extra_reward_list[i].item_id, extra_reward_list[i].num, extra_reward_list[i].is_bind))
				{
					break;
				}
			}
		}

		// 发放额外奖励
		for (int i = 0; i < extra_item_count && i <= ItemNamespace::MAX_KNAPSACK_GRID_NUM - MAX_ATTACHMENT_ITEM_NUM; i += MAX_ATTACHMENT_ITEM_NUM)
		{
			m_role->GetKnapsack()->PutListOrMail(extra_item_list + i, PUT_REASON_TOWERDEFENF_TEAM_FB_EXTRA_REWARD);
		}

		for (int j = 0; j < extra_item_count && j < ItemNamespace::MAX_KNAPSACK_GRID_NUM; j++)
		{
			bool is_find = false;

			for (int i = 0; i < list_num && i < ItemNamespace::MAX_KNAPSACK_GRID_NUM; i++)
			{
				if (item_list[i].item_id == extra_item_list[j].item_id && item_list[i].is_bind == extra_item_list[j].is_bind)
				{
					item_list[i].num += extra_item_list[j].num;
					is_find = true;

					break;
				}
			}

			if (!is_find && list_num < ItemNamespace::MAX_KNAPSACK_GRID_NUM)
			{
				item_list[list_num].item_id = extra_item_list[j].item_id;
				item_list[list_num].num = extra_item_list[j].num;
				item_list[list_num].is_bind = extra_item_list[j].is_bind;

				list_num++;
			}
		}
	}

	for (int i = 0; i < list_num && i < ItemNamespace::MAX_KNAPSACK_GRID_NUM; i++)
	{
		bool is_find = false;
		for (int j = 0; j < info.item_count && j < ItemNamespace::MAX_KNAPSACK_GRID_NUM; j++)
		{
			if (info.item_list[j].item_id == item_list[i].item_id)
			{
				info.item_list[j].num += item_list[i].num;
				is_find = true;
				continue;
			}
		}

		if (!is_find && info.item_count >= 0 && info.item_count < ItemNamespace::MAX_KNAPSACK_GRID_NUM)
		{
			info.item_list[info.item_count].item_id = item_list[i].item_id;
			info.item_list[info.item_count].num = item_list[i].num;

			++info.item_count;
		}
	}

	m_role->GetXiannvManager()->AddShengwuEssence(info.xiannv_shengwu, "TeamTowerDefen::BuyMultipleReward");
	m_role->GetFBTowerDefend()->AddTodayTDMultipleRewardTimes(1);
	EventHandler::Instance().OnEnterFBTowerDefendTeam(m_role, true);

	//通知客户端
	info.fb_type = TeamManager::TEAM_TYPE_TEAM_TOWERDEFEND;
	info.today_buy_times = m_role->GetFBTowerDefend()->GetTodayTDMultipleRewardTimes();
	
	int len = sizeof(info) - sizeof(info.item_list[0]) * (ItemNamespace::MAX_KNAPSACK_GRID_NUM - info.item_count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, len);
}

bool RoleFBTowerDefend::CheckSceneFetchDoubleReward()
{
	Scene * scene = m_role->GetScene();
	if (NULL == scene)
	{
		return false;
	}
	else
	{
		if (!Scene::IsStaticScene(scene->GetSceneType())) //活动副本中不允许扫荡
		{
			m_role->NoticeNum(errornum::EN_BUY_MUITIPLE_SCENE_LIMIT);
			return false;
		}
	}

	return true;
}