#include "rolefbequip.hpp"
#include "protocal/msgfb.h"
#include "obj/character/role.h"
#include "fbequipconfig.hpp"
#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "world.h"
#include "gameworld/monster/monsterpool.h"
#include "gameworld/monster/monsterinitparam.h"
#include "gameworld/monster/drop/droppool.hpp"
#include "item/itempool.h"
#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"
#include "servercommon/string/gameworldstr.h"
#include "scene/scene.h"
#include "other/event/eventhandler.hpp"
#include "gameworld/item/knapsack.h"
#include "other/vip/vip.hpp"
#include "other/vip/vipconfig.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/sublock/sublock.hpp"
#include "gameworld/task/newtaskmanager.h"
#include "gameworld/npc/transmit/fbmanager.hpp"
#include "friend/teaminfo.hpp"
#include "scene/teamequipfbmanager/teamequipfbmanager.hpp"
#include "global/team/team.hpp"
#include "scene/roleequipfbmanager/roleequipfbmanager.hpp"
#include "global/team/teammanager.hpp"
#include "gameworld/config/guildconfig.hpp"

RoleFBEquip::RoleFBEquip() : m_role(NULL), m_equip_team_start_time(0), list_num(0)
{
	memset(item_list, 0, sizeof(item_list));
}

RoleFBEquip::~RoleFBEquip()
{

}

void RoleFBEquip::Init(Role *role, const NewEquipFBParam &newequipfb_param, const EquipFBInfo &team_param)
{
	m_role = role;

	m_neq_param = newequipfb_param;
	m_team_param = team_param;
}

void RoleFBEquip::GetInitParam(NewEquipFBParam *newequipfb_param, EquipFBInfo *team_param)
{
	*newequipfb_param = m_neq_param;
	*team_param = m_team_param;
}

void RoleFBEquip::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		m_team_param.DayReset();

		m_neq_param.today_vip_buy_times = 0;
		m_neq_param.today_item_buy_times = 0;
		m_neq_param.today_fight_all_times = 0;

		for (int chapter = 0; chapter < NEQFB_MAX_CHAPTER; ++ chapter)
		{
			for (int level = 0; level < NEQFB_MAX_LEVEL_PER_CHAPTER; ++ level)
			{
				m_neq_param.chapter_list[chapter].level_list[level].join_times = 0;
			}
		}

		this->SendNeqFBInfo();
	}
}

void RoleFBEquip::OnFBFinish(bool is_finish, bool is_passed, bool can_jump, bool is_all_over, bool is_leave)
{
	static Protocol::SCEquipFBResult efr;

	efr.is_finish = is_finish ? 1 : 0;
	efr.is_all_over = (is_all_over ? 1 : 0);
	efr.is_passed = is_passed ? 1 : 0;
	efr.can_jump = can_jump ? 1 : 0;
	efr.use_time = static_cast<int>(EngineAdapter::Instance().Time()) - m_equip_team_start_time;
	efr.have_pass_reward = 0;
	efr.is_leave = is_leave ? 1 : 0;
	efr.item_count = 0;
	memset(efr.fall_item_list, 0, sizeof(efr.fall_item_list));

	if (is_all_over)
	{
		int join_time = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_TEAM_EQUIP_FB_JOIN_TIMES);
		if (join_time <= LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().join_times)
		{
			efr.have_pass_reward = 1;	
		}
	}
	Team *team = m_role->GetTeamInfo()->GetMyTeam();
	if (NULL != team)
	{
		int scene_key = team->GetTeamSeq();
		efr.item_count = TeamEquipFBManager::Instance().GetTotalFallItem(m_role, scene_key, efr.fall_item_list, NEQFB_PICK_FALL_ITEM_RECORD_COUNT);
	}

	for (int i = 0; i < list_num; i++)
	{
		size_t item_count = static_cast<size_t>(efr.item_count);
		if (item_count >= 0 && item_count < NEQFB_MAX_DROP_COUNT)
		{
			efr.fall_item_list[item_count].item_id = item_list[i].item_id;
			efr.fall_item_list[item_count].item_num = item_list[i].num;

			const ItemBase* item_base = ITEMPOOL->GetItem(item_list[i].item_id);
			if (item_base != NULL)
			{
				efr.fall_item_list[item_count].item_color = item_base->GetColor();
			}

			++efr.item_count;
		}
	}

	if (efr.item_count > 0)		//防止玩家失败退出没有展示奖励
	{
		efr.have_pass_reward = 1;
	}

	int send_len = sizeof(efr) - (NEQFB_MAX_DROP_COUNT - efr.item_count) * sizeof(efr.fall_item_list[0]);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&efr, send_len);
}

void RoleFBEquip::OnPersonalFBEnter(int scene_id)
{
	
}

void RoleFBEquip::SendFBInfo()
{
	static Protocol::SCEquipFBInfo efi;
	efi.flag = m_team_param.flag;
	efi.max_layer_today_entered = m_team_param.max_layer_today_entered;
	memcpy(efi.mysterylayer_list, m_team_param.mysterylayer_list, sizeof(efi.mysterylayer_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&efi, sizeof(efi));
}

void RoleFBEquip::OnPersonalBuyMysteryItem(int shop_item_seq)
{
	
}

void RoleFBEquip::OnTeamFBMemberEnter(int scene_id, unsigned int fb_start_time)
{
	const FBEquipLayerCfg *layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgBySceneID(scene_id);
	if (NULL == layer_cfg) return;

	if (fb_start_time > 0 && fb_start_time != m_equip_team_start_time)
	{
		m_equip_team_start_time = fb_start_time;
	}

	// 组队的话 队友进入的最高层也算自己进过了
	if (layer_cfg->layer > m_team_param.max_layer_today_entered)
	{
		m_team_param.max_layer_today_entered = static_cast<char>(layer_cfg->layer);
	}
}

void RoleFBEquip::OnTeamBuyMysteryItem(int shop_item_seq)
{
	if (shop_item_seq < 0 || shop_item_seq >= FB_EQUIP_MAX_GOODS_SEQ) return;

	const FBEquipMysteryShopItem *item_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamMysteryShopCfg(shop_item_seq);
	if (NULL == item_cfg)
	{
		m_role->NoticeNum(errornum::EN_NONEXIST_TO_BUY);
		return;
	}

	{
		// 是否通关神秘层
		/*if (m_team_param.max_layer_today_entered < (char)item_cfg->mystery_layer)
		{
			m_role->NoticeNum(errornum::EN_FB_NOT_PASSED);
			return;
		}*/

		// 背包容量
		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}

		// 元宝
		if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(item_cfg->gold_price))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		// 限购
		if (m_team_param.mysterylayer_list[shop_item_seq] >= item_cfg->buy_limit)
		{
			m_role->NoticeNum(errornum::EN_SHOP_HAS_BOUGHT_LIMIT_ITEM);
			return;
		}
	}

	{
		// 扣钱
		if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(item_cfg->gold_price, "FBEquipMysteryLayerBuy")) return;

		// 计次
		++ m_team_param.mysterylayer_list[shop_item_seq];

	}

	this->SendFBInfo();

	m_role->NoticeNum(noticenum::NT_SHOP_BUY_SUCC);
}

void RoleFBEquip::OnPersonalPass(int real_layer)
{

}

void RoleFBEquip::OnTeamPass(int real_layer)
{
	if (real_layer > m_team_param.max_passed_real_layer)
	{
		m_team_param.max_passed_real_layer = real_layer;
	}

	if (real_layer > m_team_param.max_layer_today_pass)
	{
		if (m_team_param.is_record_today_max_pass_level == GIVE_REWARD)
		{
			m_team_param.max_layer_today_pass = real_layer;
		}
	}

	EventHandler::Instance().OnEquipTeamFBPass(m_role, real_layer);
}

//////////////////////////////////////////////////////////////////////////
int RoleFBEquip::GetNeqChapterTotalStar(int chapter)
{
	if (chapter < 0 || chapter >= NEQFB_MAX_CHAPTER) return 0;

	int total_star = 0;
	for (int i = 0; i < NEQFB_MAX_LEVEL_PER_CHAPTER; i++)
	{
		if (m_neq_param.chapter_list[chapter].level_list[i].max_star > 0)
		{
			total_star += m_neq_param.chapter_list[chapter].level_list[i].max_star;
		}
	}

	return total_star;
}

void RoleFBEquip::SendNeqFBInfo(short info_type)
{
	Protocol::SCNeqFBInfo scinfo;
	scinfo.info_type = info_type;
	scinfo.reverse_sh = 0;
	scinfo.toal_buy_times = m_neq_param.today_vip_buy_times + m_neq_param.today_item_buy_times;
	scinfo.today_fight_all_times = m_neq_param.today_fight_all_times;
	scinfo.today_vip_buy_times = m_neq_param.today_vip_buy_times;
	scinfo.today_item_buy_times = m_neq_param.today_item_buy_times;

	UNSTD_STATIC_CHECK(sizeof(scinfo.chapter_list) / sizeof(scinfo.chapter_list[0]) == sizeof(m_neq_param.chapter_list) / sizeof(m_neq_param.chapter_list[0]));

	for (int i = 0; i < NEQFB_MAX_CHAPTER; ++i)
	{
		scinfo.chapter_list[i].reward_flag = m_neq_param.chapter_list[i].reward_flag;

		for (int j = 0; j < NEQFB_MAX_LEVEL_PER_CHAPTER; ++j)
		{
			scinfo.chapter_list[i].level_list[j].max_star = m_neq_param.chapter_list[i].level_list[j].max_star;
			scinfo.chapter_list[i].level_list[j].join_times = m_neq_param.chapter_list[i].level_list[j].join_times;
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scinfo, sizeof(scinfo));
}

bool RoleFBEquip::CanDailyFind(int *chapter , int *level)
{
	bool can_find = false;
	for (int i = 0; i < NEQFB_MAX_CHAPTER; i++)
	{
		NewEquipFBChapter & fb_chapter = m_neq_param.chapter_list[i];

		for (int j = 0; j < NEQFB_MAX_LEVEL_PER_CHAPTER; j++)
		{
			if (fb_chapter.level_list[j].max_star == 3)
			{
				can_find = true;
				if (NULL != chapter && NULL != level)
				{
					*chapter = i;
					*level = j;
				}
			}

			if (fb_chapter.level_list[j].max_star <= 0)
			{
				break;
			}
		}
	}
	return can_find;
}

int RoleFBEquip::GetNeqFBPassMaxLevel()
{
	int pass_max_level = 0;

	for (int i = 0; i < NEQFB_MAX_CHAPTER; i ++)
	{
		NewEquipFBChapter &chapter = m_neq_param.chapter_list[i];

		for (int j = 0; j < NEQFB_MAX_LEVEL_PER_CHAPTER; j ++)
		{
			if (chapter.level_list[j].max_star <= 0)
			{
				break;
			}

			++ pass_max_level;
		}
	}

	return pass_max_level;
}

int RoleFBEquip::GetNeqFBPassMaxChapter()
{
	int pass_level = this->GetNeqFBPassMaxLevel();
	int pass_chapter = pass_level / NEQFB_MAX_LEVEL_PER_CHAPTER;

	return pass_chapter;
}

void RoleFBEquip::GMAddScore(int score)
{

}

void RoleFBEquip::OnLeaveScene()
{
	//this->CheckRollState();
}

void RoleFBEquip::OnNeqFBEnterReq(int chapter, int level)
{
	if (chapter < 0 || chapter >= NEQFB_MAX_CHAPTER || level < 0 || level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
	{
		return;
	}

	const NeqFBLevelCfg *lvl_cfg = LOGIC_CONFIG->GetNeqFBConfig().GetLevelCfg(chapter, level);
	if (NULL == lvl_cfg)
	{
		return;
	}

	// 是否通关前关
	if (chapter > 0 || level > 0)
	{
		int pre_chapter = -1, pre_level = -1;
		
		if (level > 0)
		{
			pre_level = level - 1;
			pre_chapter = chapter;
		}
		else 
		{
			pre_level = NEQFB_MAX_LEVEL_PER_CHAPTER - 1;
			pre_chapter = chapter - 1;
		}

		if (pre_chapter >= 0 && pre_level >= 0)
		{
			if (m_neq_param.chapter_list[pre_chapter].level_list[pre_level].max_star <= 0)
			{
				m_role->NoticeNum(errornum::EN_NEQ_PRELEVEL_NOT_PASS);
				return;
			}
		}
	}

	// 角色等级
	if (m_role->GetLevel() < lvl_cfg->role_level)
	{
		m_role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}

	// 检查次数
	if (m_neq_param.today_fight_all_times >= LOGIC_CONFIG->GetNeqFBConfig().GetDayTotalCount() + m_neq_param.today_vip_buy_times + m_neq_param.today_item_buy_times)
	{
		m_role->NoticeNum(errornum::EN_NEQ_TIMES_LIMIT);
		return;
	}

	Posi enter_pos;
	enter_pos.x = lvl_cfg->pos_x;
	enter_pos.y = lvl_cfg->pos_y;

	World::GetInstWorld()->GetSceneManager()->GoTo(m_role, lvl_cfg->scene_id, UidToInt(m_role->GetUserId()), enter_pos); 
}

void RoleFBEquip::OnPassNeqFB(bool passed, int chapter, int level, int seconds)
{
	Protocol::SCNeqPass scnp;

	if (!passed)				// 闯关失败
	{
		scnp.pass_result = 0;
		scnp.pass_sec = 0;
		scnp.pass_star = 0;
		scnp.reward_score = 0;
	}
	else						// 闯关成功
	{
		if (chapter < 0 || chapter >= NEQFB_MAX_CHAPTER || level < 0 || level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
		{
			return;
		}

		const NeqFBLevelCfg *lvl_cfg = LOGIC_CONFIG->GetNeqFBConfig().GetLevelCfg(chapter, level);
		if (NULL == lvl_cfg)
		{
			return;
		}

		char got_star = 1;

		if (seconds <= lvl_cfg->sec_3_star)
		{
			got_star = 3;
		}
		else if (seconds <= lvl_cfg->sec_2_star)
		{
			got_star = 2;
		}
		
		// 更新数据
		NewEquipFBLevel &target_level = m_neq_param.chapter_list[chapter].level_list[level];

		if (0 == target_level.max_star && NEQFB_MAX_LEVEL_PER_CHAPTER - 1 == level)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_new_equip_fb_pass,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), chapter + 1);

			if (length > 0)
			{
				// World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER);
			}

			//在此处触发，每章只会触发一次
			//江湖传闻
			auto guild_rumor_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildRumorCfg(GUILD_EVENT_TYPE_RUMOR_WEAPON_FB);
			if (guild_rumor_cfg != NULL)
			{
				if (chapter + 1 >= guild_rumor_cfg->param_1)
				{
					if (guild_rumor_cfg->param_2 > 0)
					{
						if ((chapter + 1) % guild_rumor_cfg->param_2 == guild_rumor_cfg->param_3)
						{
							GuildID guild_id = m_role->GetGuildID();
							if (INVALID_GUILD_ID != guild_id)
							{
								EventHandler::Instance().OnAddGuildRumorEvent(guild_id, m_role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_WEAPON_FB, chapter + 1);
							}
						}
					}
				}
			}
		}

		if (got_star > target_level.max_star)
		{
			target_level.max_star = got_star;
		}

		this->AddTodayFightAllTimes();				//通关才扣次数
		++ target_level.join_times;

		this->SendNeqFBInfo();

		EventHandler::Instance().OnPassEquipFb(m_role, chapter, level);

		scnp.pass_result = 1;
		scnp.pass_sec = seconds;
		scnp.pass_star = got_star;

		RoleEquipFBManager::Instance().StartRoll(m_role, got_star, chapter, level);

		{
			//  日志
			gamelog::g_log_fb.printf(LL_INFO, "OnPass user[%s %d] role_level[%d], chapter[%d], level[%d], use_time[%d]",
				m_role->GetName(), m_role->GetUID(), m_role->GetLevel(), chapter, level, seconds);
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scnp, sizeof(scnp));
}

void RoleFBEquip::OnNeqTakeStarReward(int chapter, int reward_seq)
{
	if (chapter < 0 || chapter >= NEQFB_MAX_CHAPTER || reward_seq < 0)
	{
		return;
	}

	const NeqFBStarRewardList *reward_list = LOGIC_CONFIG->GetNeqFBConfig().GetStarRewardList(chapter, reward_seq);
	if (NULL == reward_list)
	{
		return;
	}

	if (0 != (m_neq_param.chapter_list[chapter].reward_flag & (1 << reward_seq)))
	{
		m_role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
		return;
	}

	int total_star = 0;
	for (int i = 0; i < NEQFB_MAX_LEVEL_PER_CHAPTER; ++i)
	{
		total_star += m_neq_param.chapter_list[chapter].level_list[i].max_star;
	}

	if (total_star < reward_list->need_star)
	{
		m_role->NoticeNum(errornum::EN_REWARD_NOT_SATISFY);
		return;
	}

	int need_space = reward_list->reward_count;

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_space))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (reward_list->reward_count > 0)
	{
		m_role->GetKnapsack()->PutList(reward_list->reward_count, reward_list->item_list, PUT_REASON_NEQ_STAR_REWARD);
	}

	if (reward_list->add_challenge_count > 0)
	{
		m_neq_param.today_item_buy_times += reward_list->add_challenge_count; // 奖励增加可挑战次数
	}

	m_neq_param.chapter_list[chapter].reward_flag |= (1 << reward_seq);

	this->SendNeqFBInfo(Protocol::SCNeqFBInfo::NEQ_FB_INFO_REWARD);

	m_role->NoticeNum(noticenum::NT_EXPDITION_GET_REWARD_SUCC);
	//CommitLog2ComsumeGoldOrGotItem();
}

void RoleFBEquip::OnNeqVipBuyTimes()
{
	int vip_level = m_role->GetVip()->GetVipLevel();
	int buyable_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(vip_level, VAT_NEQ_FB_BUY_TIMES);
	
	if (m_neq_param.today_vip_buy_times >= buyable_times)
	{
		m_role->NoticeNum(errornum::EN_FB_NEQ_BUY_TIMES_UP);
		return;
	}

	int need_gold = LOGIC_CONFIG->GetNeqFBConfig().GetBuyTimesGold();
	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "NeqBuyTimes"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_neq_param.today_vip_buy_times += 1;

	this->SendNeqFBInfo(Protocol::SCNeqFBInfo::NEQ_FB_INFO_VIP_BUY_TIME);
	//CommitLog2ComsumeGoldOrGotItem();
}

bool RoleFBEquip::OnNeqItemBuyTimes(int times)
{
	if (times <= 0 || times > 1000 || m_neq_param.today_item_buy_times >= 10000) return false;

	m_neq_param.today_item_buy_times += times;

	this->SendNeqFBInfo(Protocol::SCNeqFBInfo::NEQ_FB_INFO_ITEM_BUY_TIME);

	return true;
}

void RoleFBEquip::OnNeqAutoReq(int chapter, int level)
{
	if (chapter < 0 || chapter >= NEQFB_MAX_CHAPTER || level < 0 || level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
	{
		return;
	}

	const NeqFBLevelCfg *lvl_cfg = LOGIC_CONFIG->GetNeqFBConfig().GetLevelCfg(chapter, level);
	if (NULL == lvl_cfg)
	{
		return;
	}

	if (m_role->GetScene()->GetSceneType() == Scene::SCENE_TYPE_PERSONAL_EQUIPFB)
	{
		m_role->NoticeNum(errornum::EN_FB_OPERATE_LIMIT);
		return;
	}

	// 角色等级
	if (m_role->GetLevel() < lvl_cfg->role_level)
	{
		m_role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
		return;
	}

	int min_knap_space = LOGIC_CONFIG->GetNeqFBConfig().GetAutoMinKnapGridCount();
	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(min_knap_space))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	NewEquipFBLevel &target_level = m_neq_param.chapter_list[chapter].level_list[level];

	// 通关了
	if (target_level.max_star < LOGIC_CONFIG->GetNeqFBConfig().GetAutoLimitStar())
	{
		m_role->NoticeNum(errornum::EN_FB_NEQ_NOT_MAX_STAR);
		return;
	}

	// 今日总次数
	if (m_neq_param.today_fight_all_times >= LOGIC_CONFIG->GetNeqFBConfig().GetDayTotalCount() + m_neq_param.today_vip_buy_times + m_neq_param.today_item_buy_times)
	{
		m_role->NoticeNum(errornum::EN_NEQ_TIMES_LIMIT);
		return;
	}

	// 本层次数
	if (target_level.join_times >= lvl_cfg->limit_times)
	{
		m_role->NoticeNum(errornum::EN_NEQ_LEVEL_TIMES_LIMIT);
		return;
	}

	// 检查消耗
	//static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
// 	bool is_level_free = (m_role->GetLevel() >= AUTO_FB_FREE_WITH_LEVEL);
// 	bool is_free = false;
// 	int free_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_NEQ_FB_FREE_AUTO_TIMES);

// 	if (!is_level_free)
// 	{
// 		if (free_times > 0 && m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_NEQ_FREE_AUTO_FB, free_times))
// 		{
// 			is_free = true;
// 		}
// 		else
// 		{
// 			ItemExtern::ItemConsumeConfig stuff;
// 			stuff.item_id = LOGIC_CONFIG->GetNeqFBConfig().GetAutoItemId();
// 			stuff.num = LOGIC_CONFIG->GetNeqFBConfig().GetAutoItemCount();
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

	// 掉落
	int index_count = 0;
	int reward_coin = 0;
	int reward_yuanli = 0;
	long long reward_exp = 0;
	ItemConfigData reward_item_list[NEQFB_MAX_DROP_COUNT];

	if (lvl_cfg->boss_id > 0 && lvl_cfg->boss_x > 0 && lvl_cfg->boss_y > 0)
	{
		MONSTERPOOL->GetMonsterDrop(lvl_cfg->boss_id, reward_item_list, &index_count, &reward_exp, &reward_coin, &reward_yuanli);
	}

	if (lvl_cfg->monster_id > 0)
	{
		for (int i = 0; i < NeqFBLevelCfg::MAX_MONSTER_TO_FLUSH; ++ i)
		{
			if (lvl_cfg->monster_pos_list[i].x > 0 && lvl_cfg->monster_pos_list[i].y > 0)
			{
				MONSTERPOOL->GetMonsterDrop(lvl_cfg->monster_id, reward_item_list, &index_count, &reward_exp, &reward_coin, &reward_yuanli);
			}
		}
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(index_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

// 	if (!is_level_free)
// 	{
// 		if (!is_free)
// 		{
// 			if (m_role->GetSublock()->CheckHasLock())
// 			{
// 				return;
// 			}
// 
// 			if (consume_list.count > 0 && !m_role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "NeqAuto"))
// 			{
// 				return;
// 			}
// 
// 			long long consume_gold = 0, consume_gold_bind = 0;
// 			if (consume_list.need_gold_num > 0 && !m_role->GetKnapsack()->GetMoney()->UseAllGold(consume_list.need_gold_num, "NeqAuto", true, &consume_gold, &consume_gold_bind))
// 			{
// 				return;
// 			}
// 
// 			EventHandler::Instance().OnShopBuyItem(m_role, consume_list.buy_count, consume_list.buy_list, consume_gold, consume_gold_bind);
// 		}
// 		else
// 		{
// 			m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_NEQ_FREE_AUTO_FB);
// 		}
// 	}

	//m_neq_param.today_fight_all_times += 1;
	this->AddTodayFightAllTimes();
	m_neq_param.chapter_list[chapter].level_list[level].join_times += 1;

	if (index_count > 0 && !m_role->GetKnapsack()->PutList(index_count, reward_item_list, PUT_REASON_NEQ_AUTO))
	{
		return;
	}

	this->SendNeqFBInfo();

	if (reward_coin > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddCoinBind(reward_coin, "PassAutoFBNewEquip");
	}

	if (reward_exp > 0)
	{
		m_role->AddExp(reward_exp, "PassAutoFBNewEquip", Role::EXP_ADD_REASON_DEFAULT);
	}

	if (reward_yuanli > 0)
	{
		m_role->AddYuanli(reward_yuanli, "PassAutoFBNewEquip");
	}

	{
		RoleEquipFBManager::Instance().StartRoll(m_role, m_neq_param.chapter_list[chapter].level_list[level].max_star, chapter, level);

		Protocol::SCNeqPass scnp;
		scnp.pass_result = 1;
		scnp.pass_sec = 0;
		scnp.pass_star = m_neq_param.chapter_list[chapter].level_list[level].max_star;
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scnp, sizeof(scnp));
	}

	m_role->NoticeNum(noticenum::NT_NEQ_AUTO_SUCC);

	EventHandler::Instance().OnPassEquipFb(m_role, chapter, level);
	EventHandler::Instance().OnEnterFBEquip(m_role, true,false);

	m_role->GetTaskManager()->OnEnterSceneType(Scene::SCENE_TYPE_PERSONAL_EQUIPFB);
}

// bool RoleFBEquip::pubNeqAutoReq(int chapter, int level, int need_gold, Protocol::SCAutoFBRewardDetail & afrd)
// {
// 	if (chapter < 0 || chapter >= NEQFB_MAX_CHAPTER || level < 0 || level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
// 	{
// 		return false;
// 	}
// 
// 	const NeqFBLevelCfg *lvl_cfg = LOGIC_CONFIG->GetNeqFBConfig().GetLevelCfg(chapter, level);
// 	if (NULL == lvl_cfg)
// 	{
// 		return false;
// 	}
// 
// 	// 角色等级
// 	if (m_role->GetLevel() < lvl_cfg->role_level)
// 	{
// 		m_role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
// 		return false;
// 	}
// 
// 	int min_knap_space = LOGIC_CONFIG->GetNeqFBConfig().GetAutoMinKnapGridCount();
// 	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(min_knap_space))
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 		return false;
// 	}
// 
// 	if (m_role->GetKnapsack()->IsLock())
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
// 		return false;
// 	}
// 
// 	NewEquipFBLevel &target_level = m_neq_param.chapter_list[chapter].level_list[level];
// 
// 	// 通关了
// 	if (target_level.max_star <= 0)
// 	{
// 		m_role->NoticeNum(errornum::EN_FB_NEQ_NOT_MAX_STAR);
// 		return false;
// 	}
// 
// 	// 今日总次数
// 	if (m_neq_param.today_fight_all_times >= LOGIC_CONFIG->GetNeqFBConfig().GetDayTotalCount() + m_neq_param.today_vip_buy_times + m_neq_param.today_item_buy_times)
// 	{
// 		m_role->NoticeNum(errornum::EN_NEQ_TIMES_LIMIT);
// 		return false;
// 	}
// 
// 	// 本层次数
// 	if (target_level.join_times >= lvl_cfg->limit_times)
// 	{
// 		m_role->NoticeNum(errornum::EN_NEQ_LEVEL_TIMES_LIMIT);
// 		return false;
// 	}
// 
// 	// 检查消耗
// 	bool is_level_free = false;
// 
// 	bool is_free = false;
// 	int free_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_NEQ_FB_FREE_AUTO_TIMES);
// 
// 	if (!is_level_free)
// 	{
// 		if (free_times > 0 && m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_NEQ_FREE_AUTO_FB, free_times))
// 		{
// 			is_free = true;
// 		}
// 		else
// 		{
// 			if (need_gold > 0)
// 			{
// 				if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(need_gold))
// 				{
// 					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 					return false;
// 				}
// 			}
// 		}
// 	}
// 
// 	// 掉落
// 	int index_count = 0;
// 	int reward_coin = 0;
// 	int reward_yuanli = 0;
// 	int reward_exp = 0;
// 	ItemConfigData reward_item_list[NEQFB_MAX_DROP_COUNT];
// 
// 	if (lvl_cfg->boss_id > 0 && lvl_cfg->boss_x > 0 && lvl_cfg->boss_y > 0)
// 	{
// 		MONSTERPOOL->GetMonsterDrop(lvl_cfg->boss_id, reward_item_list, &index_count, &reward_exp, &reward_coin, &reward_yuanli);
// 	}
// 
// 	if (lvl_cfg->monster_id > 0)
// 	{
// 		for (int i = 0; i < NeqFBLevelCfg::MAX_MONSTER_TO_FLUSH; ++i)
// 		{
// 			if (lvl_cfg->monster_pos_list[i].x > 0 && lvl_cfg->monster_pos_list[i].y > 0)
// 			{
// 				MONSTERPOOL->GetMonsterDrop(lvl_cfg->monster_id, reward_item_list, &index_count, &reward_exp, &reward_coin, &reward_yuanli);
// 			}
// 		}
// 	}
// 
// 	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(index_count))
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 		return false;
// 	}
// 
// 	if (!is_level_free)
// 	{
// 		if (!is_free)
// 		{
// 			if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "RoleFBEquip::pubNeqAutoReq"))
// 			{
// 				return false;
// 			}
// 		}
// 		else
// 		{
// 			m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_NEQ_FREE_AUTO_FB);
// 		}
// 	}
// 
// 	m_neq_param.today_fight_all_times += 1;
// 	m_neq_param.chapter_list[chapter].level_list[level].join_times += 1;
// 
// 	if (index_count > 0 && !m_role->GetKnapsack()->PutList(index_count, reward_item_list, PUT_REASON_NEQ_AUTO))
// 	{
// 		return false;
// 	}
// 
// 	if (reward_coin > 0)
// 	{
// 		m_role->GetKnapsack()->GetMoney()->AddCoinBind(reward_coin, "PassAutoFBNewEquip");
// 	}
// 
// 	if (reward_exp > 0)
// 	{
// 		m_role->AddExp(reward_exp, "PassAutoFBNewEquip");
// 	}
// 
// 	if (reward_yuanli > 0)
// 	{
// 		m_role->AddYuanli(reward_yuanli, "PassAutoFBNewEquip");
// 	}
// 
// 	{
// 		afrd.fb_type = FBManager::FBCT_NEQ_FB;
// 		afrd.reward_coin = reward_coin;
// 		afrd.reward_exp = reward_exp;
// 		afrd.reward_yuanli = reward_yuanli;
// 		afrd.reward_xianhun = 0;
// 		afrd.reserve_sh = 0;
// 
// 		for (int i = 0; i < index_count && afrd.item_count < Protocol::SCAutoFBRewardDetail::REWARD_ITEM_MAX_COUNT; ++i)
// 		{
// 			afrd.item_list[afrd.item_count].item_id = reward_item_list[i].item_id;
// 			afrd.item_list[afrd.item_count].num = reward_item_list[i].num;
// 			afrd.item_list[afrd.item_count].is_bind = reward_item_list[i].is_bind;
// 
// 			++afrd.item_count;
// 		}
// 
// 		// 模拟客户手动翻牌行为
// 		this->pubStartRoll(m_neq_param.chapter_list[chapter].level_list[level].max_star, chapter, level);
// 		this->pubOnNeqRollReq(afrd);
// 		this->pubOnNeqRollReq(afrd);
// 		this->pubOnNeqRollReq(afrd);		
// 		this->pubCheckRollState();
// 
// 	}
// 
// 	EventHandler::Instance().OnPassEquipFb(m_role, chapter, level);
// 	m_role->GetTaskManager()->OnEnterSceneType(Scene::SCENE_TYPE_PERSONAL_EQUIPFB);
// 	//CommitLog2ComsumeGoldOrGotItem();
// 	return true;
// }

bool RoleFBEquip::CanEnterNeq(int chapter, int level)
{
	if (chapter < 0 || chapter >= NEQFB_MAX_CHAPTER || level < 0 || level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
	{
		return false;
	}

	const NeqFBLevelCfg *lvl_cfg = LOGIC_CONFIG->GetNeqFBConfig().GetLevelCfg(chapter, level);
	if (NULL == lvl_cfg)
	{
		return false;
	}

	// 是否通关前关
	if (chapter > 0 || level > 0)
	{
		int pre_chapter = -1, pre_level = -1;

		if (level > 0)
		{
			pre_level = level - 1;
			pre_chapter = chapter;
		}
		else 
		{
			pre_level = NEQFB_MAX_LEVEL_PER_CHAPTER - 1;
			pre_chapter = chapter - 1;
		}

		if (pre_chapter >= 0 && pre_level >= 0)
		{
			if (m_neq_param.chapter_list[pre_chapter].level_list[pre_level].max_star <= 0)
			{
				m_role->NoticeNum(errornum::EN_NEQ_PRELEVEL_NOT_PASS);
				return false;
			}
		}
	}

	// 角色等级
	if (m_role->GetLevel() < lvl_cfg->role_level)
	{
		m_role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
		return false;
	}

	if (!m_role->GetRoleStatusManager()->CanEnterFB())
	{
		return false;
	}

	// 检查次数
	if (m_neq_param.today_fight_all_times >= LOGIC_CONFIG->GetNeqFBConfig().GetDayTotalCount() + m_neq_param.today_vip_buy_times + m_neq_param.today_item_buy_times)
	{
		m_role->NoticeNum(errornum::EN_NEQ_TIMES_LIMIT);
		return false;
	}
	
	if (m_neq_param.chapter_list[chapter].level_list[level].join_times >= lvl_cfg->limit_times)
	{
		m_role->NoticeNum(errornum::EN_NEQ_LEVEL_TIMES_LIMIT);
		return false;
	}

	return true;
}

void RoleFBEquip::OnNeqEnter(int chapter, int level)
{

}

void RoleFBEquip::AddTodayFightAllTimes()
{
	if (SHRT_MAX == m_neq_param.today_fight_all_times)
	{
		return;
	}
	++m_neq_param.today_fight_all_times;
}

int RoleFBEquip::GetTodayFightAllTimes()
{
	return m_neq_param.today_fight_all_times;
}
int RoleFBEquip::GetNeqMaxStar(int chapter, int level)
{
	if (chapter < 0 || chapter >= NEQFB_MAX_CHAPTER || level < 0 || level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
	{
		return 0;
	}

	return m_neq_param.chapter_list[chapter].level_list[level].max_star;
}

int RoleFBEquip::GetNeqChapterMaxStar(int chapter)
{
	int chapter_total_star = 0;

	if (chapter >= 0 && chapter < NEQFB_MAX_CHAPTER)
	{
		for (int level = 0; level < NEQFB_MAX_LEVEL_PER_CHAPTER; ++ level)
		{
			chapter_total_star += m_neq_param.chapter_list[chapter].level_list[level].max_star;
		}
	}

	return chapter_total_star;
}

int RoleFBEquip::GiveEquipTeamFbLayerReward(int member_layer)
{
	if (m_team_param.can_give_prelayer_reward == NO_REWARD) return 0;

	if (member_layer < 0 || member_layer > FB_EQUIP_MAX_LAYER_ID) return 0;

	// 获取的所有掉落物列表
	//int list_num = 0;
	//static ItemConfigData item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	memset(item_list, 0, sizeof(item_list));

	// 当前成员最小层之前层次的奖励掉落逐一解析
	for (int layer = 1; layer < member_layer; layer++)
	{
		const FBEquipLayerCfg *layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgByLayer(layer);
		if (nullptr == layer_cfg)
		{
			continue;
		}

		std::vector<UInt16>::const_iterator it = layer_cfg->dropid_list.begin();
		for (; it != layer_cfg->dropid_list.end(); ++it)
		{
			int drop_item_count = 0;
			static ItemConfigData drop_item_list[DropConfig::DROP_ITEM_PROB_MAX];
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

	// 发放奖励
	for (int i = 0; i < list_num && i <= ItemNamespace::MAX_KNAPSACK_GRID_NUM - MAX_ATTACHMENT_ITEM_NUM; i += MAX_ATTACHMENT_ITEM_NUM)
	{
		m_role->GetKnapsack()->PutListOrMail(item_list + i, PUT_REASON_EQUIP_TEAM_LAYER_JUMP);
	}

	//额外奖励
	{
		int extra_item_count = 0;
		ItemConfigData extra_item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];

		std::vector<UInt16>::const_iterator extra_reward_it = LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().dropid_list.begin();
		for (; extra_reward_it != LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().dropid_list.end(); ++extra_reward_it)
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

		// 发放额外奖励（没次数不给额外奖励，智霖需求2019.4.26）
		int join_time = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_TEAM_EQUIP_FB_JOIN_TIMES);
		if (join_time <= LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().join_times)
		{
			for (int i = 0; i < extra_item_count && i <= ItemNamespace::MAX_KNAPSACK_GRID_NUM - MAX_ATTACHMENT_ITEM_NUM; i += MAX_ATTACHMENT_ITEM_NUM)
			{
				m_role->GetKnapsack()->PutListOrMail(extra_item_list + i, PUT_REASON_EQUIP_TEAM_FB_EXTRA_REWARD);
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
	}

	m_team_param.can_give_prelayer_reward = NO_REWARD;

	return list_num;
}

void RoleFBEquip::FetchDoubleReward(int oper_type)
{
	Protocol::SCFetchDoubleRewardResult info;
	info.fb_type = 0;
	info.reserve_sh = 0;
	info.item_count = 0;
	info.xiannv_shengwu = 0;
	memset(info.item_list, 0, sizeof(info.item_list));

	char today_buy_times = m_role->GetFBEquip()->GetTodayTeamFBMultipleRewardTimes();

	switch (oper_type)
	{
		case Protocol::CSFetchDoubleRewardReq::OPERA_TYPE_INFO_REQ:
		{
			//通知客户端
			info.fb_type = TeamManager::TEAM_TYPE_EQUIP_TEAM_FB;
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
				//今天购买次数已达上限
				m_role->NoticeNum(errornum::EN_BUY_MULTIPLE_REWARD_LIMIT);
				return;
			}

			//检查vip等级限制
			if (m_role->GetVip()->GetVipLevel() < LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().double_reward_need_vip_level)
			{
				m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGHT);
				return;
			}

			//消耗元宝
			if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().buy_double_reward_need_gold, "BuyFBEquipDoubleReward"))
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
				//今天购买次数已达上限
				m_role->NoticeNum(errornum::EN_BUY_MULTIPLE_REWARD_LIMIT);
				return;
			}

			//检查vip等级限制
			if (m_role->GetVip()->GetVipLevel() < LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().triple_reward_need_vip_level)
			{
				m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGHT);
				return;
			}

			//消耗元宝
			if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().buy_triple_reward_need_gold, "BuyFBEquipTripleReward"))
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
	int pass_level = m_role->GetFBEquip()->GetTodayTeamFBPassMaxLevel();

	if (pass_level < 0 || pass_level > FB_EQUIP_MAX_LAYER_ID) return;

	if (pass_level == 0)
	{
		pass_level = 1;
	}

	// 获取的所有掉落物列表
	int list_num = 0;
	ItemConfigData item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	memset(item_list, 0, sizeof(item_list));

	for (int layer = 1; layer <= pass_level; layer++)
	{
		const FBEquipLayerCfg *layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgByLayer(layer);
		if (nullptr == layer_cfg)
		{
			continue;
		}

		std::vector<UInt16>::const_iterator it = layer_cfg->dropid_list.begin();
		for (; it != layer_cfg->dropid_list.end(); ++it)
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

	//特殊处理，前面如果随机不出物品，从第1层的掉落再随机100次，出现物品后马上停止
	if (list_num == 0)
	{
		for (int j = 0; j < 100; j++)
		{
			const FBEquipLayerCfg *layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgByLayer(1);
			if (layer_cfg != NULL)
			{
				std::vector<UInt16>::const_iterator it = layer_cfg->dropid_list.begin();
				for (; it != layer_cfg->dropid_list.end(); ++it)
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
		m_role->GetKnapsack()->PutListOrMail(item_list + i, PUT_REASON_EQUIP_TEAM_BUY_MULTIPLE_REWARD);	
	}

	{
		//额外奖励
		{
			int extra_item_count = 0;
			ItemConfigData extra_item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];

			std::vector<UInt16>::const_iterator extra_reward_it = LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().dropid_list.begin();
			for (; extra_reward_it != LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().dropid_list.end(); ++extra_reward_it)
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
				m_role->GetKnapsack()->PutListOrMail(extra_item_list + i, PUT_REASON_EQUIP_TEAM_FB_EXTRA_REWARD);
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
	}

	for (int i = 0; i < list_num && i < ItemNamespace::MAX_KNAPSACK_GRID_NUM; i++)		//合并相同物品
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

	m_role->GetFBEquip()->AddTodayTeamFBMultipleRewardTimes(1);
	EventHandler::Instance().OnEnterEquipTeamFb(m_role);

	//通知客户端
	info.fb_type = TeamManager::TEAM_TYPE_EQUIP_TEAM_FB;
	info.today_buy_times = m_role->GetFBEquip()->GetTodayTeamFBMultipleRewardTimes();

	int len = sizeof(info) - sizeof(info.item_list[0]) * (ItemNamespace::MAX_KNAPSACK_GRID_NUM - info.item_count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, len);
}

bool RoleFBEquip::CheckSceneFetchDoubleReward()
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

int RoleFBEquip::GetDailyFindRewardTeam(int list_num_max,ItemConfigData * reward_list, long long *reward_exp)
{
	int level = m_role->GetFBEquip()->GetTeamFBPassMaxLevel();

	int pass_level = static_cast<int>(ceil(0.8 * level));

	if (pass_level < 0 || pass_level > FB_EQUIP_MAX_LAYER_ID) return 0;

	// 获取的所有掉落物列表
	int list_num = 0;

	for (int layer = 1; layer <= pass_level; layer++)
	{
		const FBEquipLayerCfg *layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgByLayer(layer);
		if (nullptr == layer_cfg)
		{
			continue;
		}

		std::vector<UInt16>::const_iterator it = layer_cfg->dropid_list.begin();
		for (; it != layer_cfg->dropid_list.end(); ++it)
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

	//特殊处理，前面如果随机不出物品，从第1层的掉落再随机100次，出现物品后马上停止
	if (list_num == 0)
	{
		for (int j = 0; j < 100; j++)
		{
			const FBEquipLayerCfg *layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgByLayer(1);
			if (layer_cfg != NULL)
			{
				std::vector<UInt16>::const_iterator it = layer_cfg->dropid_list.begin();
				for (; it != layer_cfg->dropid_list.end(); ++it)
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

// 	std::vector<UInt16>::const_iterator extra_reward_it = LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().dropid_list.begin();
// 	for (; extra_reward_it != LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().dropid_list.end(); ++extra_reward_it)
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