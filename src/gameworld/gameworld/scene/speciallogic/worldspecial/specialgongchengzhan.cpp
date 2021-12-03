#include "specialgongchengzhan.hpp"

#include <math.h>
#include <string>
#include <algorithm>

#include "other/event/eventhandler.hpp"
#include "obj/character/monster.h"
#include "obj/character/role.h"
#include "scene/scenemanager.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/worldshadow/worldshadow.hpp"
#include "monster/monsterpool.h"
#include "item/itempool.h"
#include "world.h"
#include "gamelog.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectinvisible.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "internalcomm.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/servercommon.h"
#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"
#include "gameworld/skill/skillpool.hpp"
#include "other/route/mailroute.hpp"
#include "protocal/msgscene.h"
#include "other/event/eventhandler.hpp"
#include "global/rank/rankmanager.hpp"
#include "global/usercache/usercache.hpp"
#include "global/activity/activitymanager.hpp"
#include "ai/monsterai/monsteraibase.hpp"
#include "ai/monsterai/specialai/monsteraigongchengzhan.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"
#include "globalconfig/globalconfig.h"
#include "ai/monsterai/specialai/monsteraibattlefield.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "global/rank/topranker.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "ai/monsterai/specialai/monsteraigongchengzhan.hpp"
#include "global/usercache/usercache.hpp"
#include "config/activityconfig/gongchengzhanconfig.hpp"
#include "item/knapsack.h"
#include "other/shop/roleshop.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/specialappearance/specialappearance.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "config/logicconfigmanager.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "servercommon/errornum.h"
#include "other/vip/vip.hpp"
#include "gameworld/global/activity/activitymsgmanager.h"
#include "gameworld/protocal/msgchatmsg.h"
#include "config/worshipconfig.hpp"
#include "global/cross/crossmanager.h"
#include "other/cross/rolecross.h"

SpecialGongChengZhan::SpecialGongChengZhan(Scene *scene) : SpecialLogic(scene), m_is_finish(false), m_is_po_cheng(false), m_po_cheng_times(0), m_is_po_qiang(false), m_last_shoucheng_time(0), m_shou_guild_id(0), 
m_last_check_shoucheng_time(0),m_last_first_guild_broadcast_time(0), m_last_best_killer_broadcast_time(0), m_flag_has_be_hit_notice_flag(false), m_flag_low_hp_broadcast_flag(0),m_cheng_zhu_prof(0),
m_kick_out_all_role_time(0), m_worship_start_time(0),m_need_sync_flag_info(false),m_last_sync_flag_info_time(0),m_flag_max_hp(0),m_flag_cur_hp(0)
{

}

SpecialGongChengZhan::~SpecialGongChengZhan()
{
	// 清理数据
	{
		for (std::map<int, GuildItem *>::iterator iter = m_guild_map.begin(); m_guild_map.end() != iter; ++iter)
		{
			delete iter->second;
		}
		m_guild_map.clear();
	}

	{
		for (std::map<int, JoinerItem *>::iterator iter = m_joiner_map.begin(); m_joiner_map.end() != iter; ++iter)
		{
			delete iter->second;
		}
		m_joiner_map.clear();
	}
}

struct FitParam
{
	FitParam() : role_id(0), guild_id(0)
	{}

	int role_id;
	GuildID guild_id;
};

bool SortFuncByShouchenTime(const SpecialGongChengZhan::GuildItem *rankitem_1, const SpecialGongChengZhan::GuildItem *rankitem_2)
{
	if (NULL == rankitem_1 || NULL == rankitem_2) return true;

	if (rankitem_1->shouchen_time != rankitem_2->shouchen_time)
	{
		return rankitem_1->shouchen_time > rankitem_2->shouchen_time;
	}

	return rankitem_1->member_uid_vec.size() > rankitem_2->member_uid_vec.size();
}

bool SortFuncByZhanGong(const SpecialGongChengZhan::JoinerItem *zhangong_1, const SpecialGongChengZhan::JoinerItem *zhangong_2)
{
	if (NULL == zhangong_1 || NULL == zhangong_2) return true;

	if (zhangong_1->zhangong != zhangong_2->zhangong)
	{
		return zhangong_1->zhangong > zhangong_2->zhangong;
	}

	return zhangong_1->add_zhangong_time < zhangong_2->add_zhangong_time;
}

void SpecialGongChengZhan::GongchengzhanInit()
{
	const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();
	this->CreateBoss();
	m_is_finish = false;
	m_is_po_cheng = false;
	m_last_zhangong_reward_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.how_much_time_will_get_zhangong;
	m_refresh_zhangong_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.zhangong_rank_refresh_time;
	m_last_shoucheng_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	this->SyncGlobalInfoToAll();
}

void SpecialGongChengZhan::OnActivityStatusChange(int status)
{
	switch (status)
	{
	case  ACTIVITY_STATUS_STANDY:
	{
		this->OnActivityStandy();
	}
	break;
	case ACTIVITY_STATUS_OPEN:
		{
			this->OnActivityStart();
		}
		break;

	case ACTIVITY_STATUS_CLOSE:
		{
			this->OnActivityClose();
		}
		break;
	}
}

void SpecialGongChengZhan::OnActivityStandy()
{
	// 清理数据
	{
		for (std::map<int, GuildItem *>::iterator iter = m_guild_map.begin(); m_guild_map.end() != iter; ++iter)
		{
			delete iter->second;
		}
		m_guild_map.clear();
	}

	{
		for (std::map<int, JoinerItem *>::iterator iter = m_joiner_map.begin(); m_joiner_map.end() != iter; ++iter)
		{
			delete iter->second;
		}
		m_joiner_map.clear();
	}
	m_guild_rank_vec.clear();
	m_personal_rank_vec.clear();
	m_worship_user_map.clear();

	m_is_finish = false;
	m_is_po_qiang = false;
	m_po_cheng_times = 0;
	m_luck.next_lucky_timestamp = 0;
	m_luck.luck_user_count = 0;
	m_worship_start_time = 0;
	m_kick_out_all_role_time = 0;
	m_shou_guild_id = WorldStatus::Instance().GetGCZWinGuild();
	m_cheng_zhu_prof = WorldStatus::Instance().GetGCZChengzhuProf();
	if (INVALID_GUILD_ID != WorldStatus::Instance().GetGCZWinGuild())
	{
		GuildItem *guild_item = new GuildItem();
		if (NULL != guild_item)
		{
			guild_item->guild_id = WorldStatus::Instance().GetGCZWinGuild();
			m_guild_map[guild_item->guild_id] = guild_item;
		}
	}
	m_flag_max_hp = 0;
	m_flag_cur_hp = 0;
}

void SpecialGongChengZhan::OnActivityStart()
{
	m_is_po_qiang = false;
	m_po_cheng_times = 0;
	m_luck.next_lucky_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg().lucky_interval;
	m_luck.luck_user_count = 0;
	m_worship_user_map.clear();
	m_worship_start_time = 0;
	this->GongchengzhanInit();
	this->SendLuckyInfoToUser();
}

void SpecialGongChengZhan::OnActivityClose()
{
	m_is_finish = true;

	// 增加守城时间
	GuildItem *win_guild = this->GetGuildItem(m_shou_guild_id);
	if(NULL != win_guild) 
	{
		win_guild->shouchen_time += static_cast<unsigned int>(EngineAdapter::Instance().Time() - m_last_shoucheng_time);
	}

	this->CalcGuildRank();

	//注意，由于重载了UpdateChenZhuInfo，一定要注意是传nullptr作为指针的重载类型，如果传null会进入重载UpdateChenZhuInfo(GuildID guild_id)
	this->UpdateChenZhuInfo(nullptr);

	// 胜利公会提示语
	{
		// 成功方
		int chengzhu_id = WorldStatus::Instance().GetGCZChengzhuID();
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(chengzhu_id);
		Guild *guild = GuildManager::Instance().GetGuild(WorldStatus::Instance().GetGCZWinGuild());
		if (NULL != user_node && NULL != guild)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_gongchengzhan_finish_occupy_notice, WorldStatus::Instance().GetGCZWinGuild(), guild->GetGuildName(), chengzhu_id, user_node->user_name, user_node->camp);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
			}

			std::map<int, JoinerItem *>::iterator it;
			int temp_max_score = 0;
			int member_uid = 0;
			for (it = m_joiner_map.begin(); m_joiner_map.end() != it; ++it)
			{
				if (it->second->guild_id == guild->GetGuildID())
				{
					if (it->second->zhangong > temp_max_score)
					{
						temp_max_score = it->second->zhangong;
						member_uid = it->second->uid;
					}
				}
			}
			UserCacheNode *member_node = UserCacheManager::Instance().GetUserNode(member_uid);
			if (member_node != NULL)
			{
				EventHandler::Instance().OnAddGuildRumorEvent(guild->GetGuildID(), user_node->user_name, member_node->user_name, GUILD_EVENT_TYPE_RUMOR_GONGCHENG_SUCC);
			}
		}

		// 所有失败方
		std::map<int, GuildItem *>::iterator it_guild;
		for (it_guild = m_guild_map.begin(); m_guild_map.end() != it_guild; ++it_guild)
		{
			if (it_guild->second->guild_id != WorldStatus::Instance().GetGCZWinGuild())
			{
				std::map<int, JoinerItem *>::iterator member_it;
				int temp_max_score = 0;
				int member_uid = 0;
				for (member_it = m_joiner_map.begin(); m_joiner_map.end() != member_it; ++member_it)
				{
					if (member_it->second->guild_id == it_guild->second->guild_id)
					{
						if (member_it->second->zhangong > temp_max_score)
						{
							temp_max_score = member_it->second->zhangong;
							member_uid = member_it->second->uid;
						}
					}
				}

				UserCacheNode *member_node = UserCacheManager::Instance().GetUserNode(member_uid);
				if (member_node != NULL)
				{
					EventHandler::Instance().OnAddGuildRumorEvent(it_guild->second->guild_id, member_node->user_name, NULL, GUILD_EVENT_TYPE_RUMOR_GONGCHENG_FAIL);
				}
			}
		}
	}

	this->MailRewardOnActClose();
	this->SyncRewardInfo();

	this->SendGuildInfoStatisticAll();
	this->SendResultInGuildChat();

	// 因为在这个场景复活界面玩家不能手动点击复活。防止活动结束时玩家不能复活的问题，这里全补满血
	this->AllRoleReLive();
	this->KillAllMonster();
	//this->DelayKickOutAllRole();
	
	int kick_out_time = 120; // 服务端多延迟两分钟
	const WorshipOtherConfig *other_cfg = LOGIC_CONFIG->GetWorshipConfig().GetOtherConfig(ACTIVITY_TYPE_GONGCHENGZHAN);
	if (other_cfg)
	{
		kick_out_time += other_cfg->worship_time;
	}
	m_kick_out_all_role_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + kick_out_time; // 延迟踢出

	m_worship_start_time = EngineAdapter::Instance().Time();

	m_guild_rank_vec.clear();
	m_personal_rank_vec.clear();
	m_worship_user_map.clear();
}

void SpecialGongChengZhan::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_finish)
	{
		if (m_kick_out_all_role_time > 0 && now_second >= m_kick_out_all_role_time)
		{
			m_kick_out_all_role_time = 0;
			this->DelayKickOutAllRole();
		}

		// 活动结束后，才开始计算膜拜
		this->CheckWorshipReward(now_second);

		return;
	}

	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GONGCHENGZHAN)) return;

	this->RewardZhangongForAPeriodOfTime(now_second);
	this->RefreshZhangong(now_second);

	if (m_luck.next_lucky_timestamp > 0 && now_second >= m_luck.next_lucky_timestamp)
	{
		m_luck.next_lucky_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg().lucky_interval;

		this->GaveLuckyItemToUser();
	}

	{
		unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		if (now_sec> m_last_check_shoucheng_time + 3)
		{
			m_last_check_shoucheng_time = now_sec;
			GuildItem *win_guild = this->GetGuildItem(m_shou_guild_id);
			if (NULL != win_guild)
			{
				win_guild->shouchen_time += now_sec - m_last_shoucheng_time;
			}
 
			this->CalcGuildRank();

			m_last_shoucheng_time = now_sec;
		}

		// 传闻
		if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > m_last_first_guild_broadcast_time + FIRST_GUILD_INTERVAL)
		{
			m_last_first_guild_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			if (!m_guild_rank_vec.empty())
			{
				Guild *guild = GuildManager::Instance().GetGuild(m_guild_rank_vec[0]->guild_id);
				if (NULL != guild)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_first_guild, guild->GetGuildName());
					if (length > 0) 
					{
						this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
					}
				}

			}
		}

		if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > m_last_best_killer_broadcast_time + BEST_KILLER_INTERVAL)
		{
			m_last_best_killer_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			int killer_id = 0;

			if (m_personal_rank_vec.size() > 0)
			{
				killer_id = m_personal_rank_vec[0]->uid;
			}

			if (killer_id > 0)
			{
				JoinerItem * joiner = m_joiner_map[killer_id];
				if (NULL != joiner && joiner->kill_num > 1)
				{
					const UserCacheNode *cache_user = UserCacheManager::Instance().GetUserNode(killer_id);
					if (NULL != cache_user)
					{
						int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_best_killer,
							cache_user->uid, cache_user->user_name, (int)cache_user->camp, joiner->kill_num);
						if (length > 0) 
						{
							this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
						}
					}
				}
			}

			//
			//Role *killer = m_scene->GetRoleByUID(killer_id);
			//if (NULL != killer && NULL != m_joiner_map[killer_id])
			//{
			//	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_best_killer,
			//		killer->GetUID(), killer->GetName(), (int)killer->GetCamp(), m_joiner_map[killer_id]->kill_num);

			//	if (length > 0) 
			//	{
			//		this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
			//	}

			//	m_last_best_killer_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			//}
		}
	}
	if (m_is_po_cheng)
	{
		this->GongchengzhanInit();
	}

	if (m_need_sync_flag_info && static_cast<unsigned int>(EngineAdapter::Instance().Time()) >= m_last_sync_flag_info_time +1)
	{
		m_need_sync_flag_info = true;
		m_last_sync_flag_info_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		this->SyncFlagInfo();
	}
}

void SpecialGongChengZhan::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
{
	if (NULL != str_buff && str_len > 0) 
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, notice_type))
		{
			m_scene->SendToRole((const char*)&sm, sendlen);
		}
	}
}

void SpecialGongChengZhan::SendLuckyInfoToUser(Role *role)
{
	if (NULL == role)
	{
		int sendlen = sizeof(m_luck) - (ZHANCHANG_LUACY_ROLE_MAX - m_luck.luck_user_count) * sizeof(m_luck.luck_user_namelist[0]);
		m_scene->SendToRole((const char*)&m_luck, sendlen);
	}
	else
	{
		int sendlen = sizeof(m_luck) - (ZHANCHANG_LUACY_ROLE_MAX - m_luck.luck_user_count) * sizeof(m_luck.luck_user_namelist[0]);
		if (sendlen > 0 && sendlen <= static_cast<int>(sizeof(m_luck)))
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&m_luck, sendlen);
		}
	}
}

void SpecialGongChengZhan::GaveLuckyItemToUser()
{
	int luck_people_num = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg().luck_people_num;

	int lucky_role_count = 0;
	int lucky_role_idx_list[ZHANCHANG_LUACY_ROLE_MAX] = {0};

	{
		std::map<int, int> idx_map;

		int max_rolenum = m_scene->RoleNum();
		for (int i = 0; i < max_rolenum; i++)  // 通过map来打乱idx，用于随机
		{
			idx_map.insert(std::make_pair(RandomNum(1024), i));
		}

		int loop_count = 0;
		while (!idx_map.empty() && lucky_role_count < luck_people_num && ++ loop_count < 128)
		{
			int rand_index = RandomNum(static_cast<int>(idx_map.size()));
			std::map<int, int>::iterator it = idx_map.begin();
			std::advance(it, rand_index);

			lucky_role_idx_list[lucky_role_count ++] = it->second;
			idx_map.erase(it);
		}
	}

	m_luck.luck_user_count = 0;

	char str_format[256] = {0};
	int max_length = sizeof(str_format) / sizeof(str_format[0]);
	int str_length = 0;

	const ItemConfigData &lucky_item = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg().lucky_item;

	for (int i = 0; i < lucky_role_count && i < ZHANCHANG_LUACY_ROLE_MAX; i++)
	{
		int role_idx = lucky_role_idx_list[i];
		if (role_idx < (int)m_scene->RoleNum())
		{
			Role *lucky_role = m_scene->GetRoleByIndex(role_idx);
			if (NULL != lucky_role)
			{
				const ItemBase *itembase = ITEMPOOL->GetItem(lucky_item.item_id);
				if (NULL != itembase)
				{
					lucky_role->GetKnapsack()->PutOrMail(lucky_item, PUT_REASON_GCZ_LUCK_REWARD);
				}

				if (m_luck.luck_user_count < ZHANCHANG_LUACY_ROLE_MAX)
				{
					lucky_role->GetName(m_luck.luck_user_namelist[m_luck.luck_user_count]);
					++ m_luck.luck_user_count;
				}

				if (i < 5)	// 最多打包5个名字 不用ZHANCHANG_LUACY_ROLE_MAX 防止改动会导致字符串不够
				{
					int temp_length = SNPRINTF(str_format + str_length, max_length - str_length, "{r;%d;%s;%d} ", 
						UidToInt(lucky_role->GetUserId()), lucky_role->GetName(), lucky_role->GetCamp());
					str_length += temp_length;
				}
			}
		}
	}

	this->SendLuckyInfoToUser();

	{
		int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wuxingguaji_lucky_notice, 
			str_format);
		if (str_len > 0) 
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
		}
	}
}

// 每一段时间奖励一次战功
void SpecialGongChengZhan::RewardZhangongForAPeriodOfTime(time_t now_second)
{
	const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();

	if (now_second < m_last_zhangong_reward_time)
	{
		return;
	}

	// 在场景里的角色才奖励
	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		if(NULL != temp_role)
		{
			JoinerItem *joiner_item = this->GetJoinerItem(temp_role->GetUID());
			if(NULL != joiner_item)
			{
				const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();
				this->AddZhangong(joiner_item, other_cfg.online_will_get_zhangong);
			}
		}
	}

	m_last_zhangong_reward_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.how_much_time_will_get_zhangong;
}

// 刷新个人战功的排名
void SpecialGongChengZhan::RefreshZhangong(time_t now_second)
{
	if (now_second < m_refresh_zhangong_time)
	{
		return;
	}

	this->CalcPersonalRank();
	this->SyncRoleInfoToAll();

	this->CalcGuildPersonalRank();

	const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();
	m_refresh_zhangong_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.zhangong_rank_refresh_time;
}

void SpecialGongChengZhan::CreateBoss()
{ 
	const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();
	
	// 旗
	{
		Monster * monster = MONSTERPOOL->CreateMonster(other_cfg.boss2_id, m_scene, other_cfg.boos2_pos);
		if (NULL != monster)
		{
			m_flag_max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			m_flag_cur_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			this->SyncFlagInfo();
		}
		m_flag_has_be_hit_notice_flag = false;
		m_flag_low_hp_broadcast_flag = 0;
	}

	if (!m_is_po_qiang)
	{
		// 守门怪
		Monster * monster = MONSTERPOOL->CreateMonster(other_cfg.boss1_id, m_scene, other_cfg.boos1_pos);
		if (NULL != monster)
		{
			if (MonsterInitParam::AITYPE_SPECIALAI_GONGCHENGZHAN == monster->GetAIType())
			{
				MonsterAIBase *ai = monster->GetMonsterAI();
				if (NULL != ai)
				{
					MonsterAIGongchengzhan *gcz_shou_ai = (MonsterAIGongchengzhan*)ai;
					gcz_shou_ai->SetSite(SITE_TYPE_SHOU);
				}
			}
		}
	}
}

void SpecialGongChengZhan::OnRoleEnterScene(Role *role)
{
	if (m_is_finish || NULL == role || INVALID_GUILD_ID == role->GetGuildID())// !ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GONGCHENGZHAN))
	{
		this->DelayKickOutRole(role);
		return;
	}

	GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());
	if (NULL == guild_item)
	{
		guild_item = new GuildItem();
		guild_item->guild_id = role->GetGuildID();
		m_guild_map[guild_item->guild_id] = guild_item;
	}

	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
	if (NULL == joiner_item)
	{
		joiner_item = new JoinerItem();
		joiner_item->uid = role->GetUID();
		joiner_item->guild_id = role->GetGuildID();
		joiner_item->zhangong = 0;
		role->GetName(joiner_item->role_name);
		joiner_item->guild_post = role->GetGuildPost();
		m_joiner_map[joiner_item->uid] = joiner_item;

		guild_item->member_uid_vec.push_back(role->GetUID());
		guild_item->member_joiner_vec.push_back(joiner_item);

		ROLE_LOG_QUICK6(LOG_TYPE_ACTIVITY_GONGCHENGZHAN, role, role->GetLevel(), 0, "Join", NULL);
	}

	this->CalcGuildRank();
	this->SyncGlobalInfoToAll();
	this->SyncRoleInfo(role);
	this->SendLuckyInfoToUser(role);
	this->SyncGuildMvpInfo(role->GetGuildID(), role);
	this->SyncFlagInfo(role);

	EventHandler::Instance().OnEnterGongchengzhan(role);
}

void SpecialGongChengZhan::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL != role)
	{
		role->GetSpecialAppearance()->SetFbAppearance(0, 0, 0, true);
	}
}

// 是否是敌人(守方不能攻击旗和墙）
bool SpecialGongChengZhan::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (m_is_finish)
	{
		return false;
	}

	if (NULL == role || NULL == obj || role == obj) return false;

	const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();

	if(Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster *)obj;
		if(this->GetRoleIsShouSite(role))
		{
			if(monster->GetMonsterId() == other_cfg.boss1_id || monster->GetMonsterId() == other_cfg.boss2_id)
			{
				return false;
			}
		}
		else
		{
			// 没有破墙，不能攻击旗
			if(!m_is_po_qiang && monster->GetMonsterId() == other_cfg.boss2_id)
			{
				return false;
			}
		}

		return monster->IsAlive();
	}

	else if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *target = (Role *)obj;

		GuildItem *target_guild_item = this->GetGuildItem(target->GetGuildID());
		if(NULL == target_guild_item) return false;

		GuildItem *role_guild_item = this->GetGuildItem(role->GetGuildID());
		if(NULL == role_guild_item) return false;
		
		// 攻方不能打攻方，守方不能打守方
		if (this->IsShouRole(target_guild_item) == this->IsShouRole(role_guild_item))
		{
			return false;
		}
		
// 		//	没有城主且没有破墙，为和平模式
// 		if (INVALID_GUILD_ID == WorldStatus::Instance().GetGCZWinGuild() && !m_is_po_qiang)
// 		{
// 			return false;
// 		}
// 		
// 		// 有城主且是第一次攻城，为和平模式
// 		if (WorldStatus::Instance().GetGCZWinGuild() != INVALID_GUILD_ID && 0 == m_po_cheng_times)
// 		{
// 			return false;
// 		}

// 		if (0 != role->GetGuildID() && role->GetGuildID() == target->GetGuildID())
// 		{
// 			return false;
// 		}

		// 角色在资源区不可以互相攻击
// 		if(this->IsPosInArea(&role->GetPos(), &other_cfg.resource_zuo_xia_pos, &other_cfg.resource_you_shang_pos) || this->IsPosInArea(&target->GetPos(), &other_cfg.resource_zuo_xia_pos, &other_cfg.resource_you_shang_pos))
// 		{
// 			return false;
// 		}

		return target->IsAlive();
	}

	return false;
}

void SpecialGongChengZhan::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	Obj *enemy_obj = m_scene->GetObj(enemy_id);
	if (NULL == cha || NULL == enemy_obj || Obj::OBJ_TYPE_ROLE != enemy_obj->GetObjType()) return;

	Role *attacker = (Role *)enemy_obj;

	JoinerItem * joiner_item = this->GetJoinerItem(attacker->GetUID());
	if (NULL == joiner_item) return;

	const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();

	// 攻击角色
	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		joiner_item->hurt_roles += -delta_hp;
// 		Role *role = (Role*)cha;
// 		Attribute now_hp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);			// 当前血量
// 		if (0 != now_hp)
// 		{
// 			int delta_hp_temp = delta_hp;
// 			if(-delta_hp_temp > now_hp)
// 			{
// 				delta_hp_temp = -now_hp;
// 			}
// 
// 			int beishu = 0;
// 
// 			Attribute max_hp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);	// 最大血量
// 			int temp_hp = static_cast<int>(max_hp * 0.001);
// 			joiner_item->attack_qi_hp += -delta_hp_temp;
// 			if(joiner_item->attack_qi_hp >= temp_hp && temp_hp > 0)
// 			{
// 				beishu = joiner_item->attack_qi_hp / temp_hp;
// 				joiner_item->attack_qi_hp -= beishu * temp_hp;
// 			}
// 
// 			if(beishu > 1000)
// 			{
// 				beishu = 1000;
// 			}
// 			int zhangong = other_cfg.hurt_role_01_zhangong * beishu;
// 			if(zhangong > 0)
// 			{
// 				this->AddZhangong(joiner_item, zhangong);
// 				this->SyncRoleInfo(attacker);
// 			}
// 		}
	}

	// 攻击旗子
	else if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Monster *monster = (Monster*)cha;
		if (monster->GetMonsterId() == other_cfg.boss2_id)
		{
			Attribute now_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);			// 当前血量
			m_flag_cur_hp = now_hp;
			m_need_sync_flag_info = true;

			if (0 != now_hp)
			{
				Attribute delta_hp_temp = delta_hp;
				if(-delta_hp_temp > now_hp)
				{
					delta_hp_temp = -now_hp;
				}

				int beishu = 0;

				Attribute max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);	// 最大血量
				Attribute temp_hp = static_cast<Attribute>(max_hp * 0.001);
				joiner_item->attack_qi_hp += -delta_hp;
				if(joiner_item->attack_qi_hp >= temp_hp && temp_hp > (long long)0)
				{
					beishu = static_cast<int>(joiner_item->attack_qi_hp / temp_hp);
					joiner_item->attack_qi_hp -= beishu * temp_hp;
				}

				if(beishu > 1000)
				{
					beishu = 1000;
				}
				int zhangong = other_cfg.hurt_boos_01_zhangong * beishu;
				if(zhangong > 0)
				{
					this->AddZhangong(joiner_item, zhangong);
					this->SyncRoleInfo(attacker);
				}

				long long &hurt = m_hurt_flag_map[attacker->GetUID()];
				hurt += -delta_hp;
			}

			this->OnFlagBeAttacked(monster);

			joiner_item->hurt_flags -= delta_hp;
		}
	}
}

void SpecialGongChengZhan::OnAttackCalcHurt(Character *cha, ObjID enemy_id, Attribute &delta_hp)
{
	Obj *enemy_obj = m_scene->GetObj(enemy_id);
	if (NULL == cha || NULL == enemy_obj || Obj::OBJ_TYPE_ROLE != enemy_obj->GetObjType()) return;

	Role *attacker = (Role *)enemy_obj;

	JoinerItem * joiner_item = this->GetJoinerItem(attacker->GetUID());
	if (NULL == joiner_item) return;

	// 攻击角色
	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		Role *role = (Role*)cha;

		GuildItem *attacker_guild_item = this->GetGuildItem(attacker->GetGuildID());
		if(NULL == attacker_guild_item) return;

		GuildItem *role_guild_item = this->GetGuildItem(role->GetGuildID());
		if(NULL == role_guild_item) return;

		// 攻方不能打攻方，守方不能打守方
		if (this->IsShouRole(attacker_guild_item) == this->IsShouRole(role_guild_item))
		{
			delta_hp = 0;
		}
	}

	//const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();
	
	// 攻击角色
	//if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	//{
		//Role *role = (Role*)cha;

		// 角色在资源区防止互相攻击溅射伤害
// 		if(this->IsPosInArea(&attacker->GetPos(), &other_cfg.resource_zuo_xia_pos, &other_cfg.resource_you_shang_pos) || this->IsPosInArea(&role->GetPos(), &other_cfg.resource_zuo_xia_pos, &other_cfg.resource_you_shang_pos))
// 		{
// 			delta_hp = 0;
// 		}
	//}
}

int SpecialGongChengZhan::GetSpecialParam(Role *role) 
{
	if (NULL == role) return -1;

	if (this->GetRoleIsShouSite(role))
	{
		return 1;
	}
	return 0;
}

void SpecialGongChengZhan::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead || m_is_finish) return;

	Obj *killer_obj = m_scene->GetObj(killer_id);

	if(NULL == killer_obj) return;

	const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();

	if(Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		Role *death = (Role*)dead;
		SpecialGongChengZhan::JoinerItem *joiner_death = this->GetJoinerItem(death->GetUID());
		if (NULL != joiner_death)
		{
			if(Obj::OBJ_TYPE_ROLE == killer_obj->GetObjType())
			{
				Role *killer = m_scene->GetPrincipal(killer_id);
				if(NULL != killer)
				{
					SpecialGongChengZhan::JoinerItem *joiner_item = this->GetJoinerItem(killer->GetUID());
					if (NULL != joiner_item)
					{
						for (std::map<UserID, time_t>::iterator it = joiner_death->hurt_map.begin(), 
							end = joiner_death->hurt_map.end(); it != end; ++ it)
						{
							if (it->second + ASSIST_LIMIT_TIME >= EngineAdapter::Instance().Time() && it->first != killer->GetUserId())
							{
								JoinerItem *joiner_assist = this->GetJoinerItem(UidToInt(it->first));
								if (NULL != joiner_assist)
								{
									joiner_assist->assist++;
									joiner_assist->lianzhan++;
									Role *assist_role = m_scene->GetRoleByUID(joiner_assist->uid);
									if (NULL != assist_role)
									{
										this->CheckLianZhanNotice(assist_role, death, true);
									}
								}
							}
						}

						joiner_item->kill_num++;
						joiner_item->lianzhan++;

						this->CheckLianZhanNotice(killer, death);

						joiner_death->lianzhan = 0;
						joiner_death->hurt_map.clear();

						this->AddZhangong(joiner_item, other_cfg.kill_role_zhangong);
						this->SyncRoleInfo(killer);
					}
				}
			}
			else
			{
				joiner_death->lianzhan = 0;
				joiner_death->hurt_map.clear();
			}
		}

// 		{
// 			GuildOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
// 
// 			if (death->GetGuildPost() > GUILD_POST_CHENG_YUAN || death->GetVip()->GetVipLevel() > other_cfg.chengyuan_event_notice_vip_level)
// 			{
// 				int content_len = 0;
// 
// 				if (RandomNum(10000) < 5000)
// 				{
// 					content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_officer_be_killed,
// 						death->GetGuildPost(), death->GetUID(), death->GetName(), death->GetCamp());
// 				}
// 				else
// 				{
// 					content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_chengyuan_be_killed,
// 						death->GetGuildPost(),death->GetUID(), death->GetName(), death->GetCamp(), m_scene->GetSceneName(), death->GetPos().x, death->GetPos().y, m_scene->GetSceneID(),
// 						m_scene->GetSceneKey(), 0);
// 				}
// 
// 				if (content_len > 0)
// 				{
// 					int sendlen = 0;
// 					static Protocol::SCSystemMsg sm;
// 
// 					if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, content_len, SYS_MSG_EVENT_TYPE_COMMON_NOTICE))
// 					{
// 						m_scene->SendToRole((const char *)&sm, sendlen, [death](Role *role)->bool{
// 							if (death->GetGuildID() == role->GetGuildID())
// 							{
// 								return true;
// 							}
// 
// 							return  false;
// 						});
// 					}
// 				}
// 			}
// 		}
	}
	else if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		if (Obj::OBJ_TYPE_ROLE == killer_obj->GetObjType())
		{
			Role *killer = m_scene->GetPrincipal(killer_id);
			if(NULL == killer) return;

			JoinerItem * joiner_item = this->GetJoinerItem(killer->GetUID());
			if(NULL == joiner_item) return;

			Monster *monster = (Monster *)dead;

// 			// 杀攻方资源怪
// 			if (monster->GetMonsterId() == other_cfg.monster1_id)				
// 			{
// 				this->AddZhangong(joiner_item, other_cfg.kill_monster1_zhangong);
// 				this->SyncRoleInfo(killer);
// 			}
// 			// 杀守方资源怪
// 			else if (monster->GetMonsterId() == other_cfg.monster2_id)				
// 			{
// 				this->AddZhangong(joiner_item, other_cfg.kill_monster2_zhangong);
// 				this->SyncRoleInfo(killer);
// 			}

			// 杀了资源怪
// 			if (monster->GetMonsterId() == other_cfg.monster1_id)				
// 			{
// 				if (this->GetRoleIsShouSite(killer))
// 				{
// 					this->AddZhangong(joiner_item, other_cfg.kill_monster2_zhangong);
// 				}
// 				else
// 				{
// 					this->AddZhangong(joiner_item, other_cfg.kill_monster1_zhangong);
// 				}
// 
// 				this->SyncRoleInfo(killer);
// 			}
			// 杀了城墙
			if (monster->GetMonsterId() == other_cfg.boss1_id)					
			{
				this->NotifySideSystemMsg(NOTICE_TYPE_PO_MENG);
				m_is_po_qiang = true;
				this->SyncGlobalInfoToAll();

			}
			// 杀了旗帜
			else if (monster->GetMonsterId() == other_cfg.boss2_id)					
			{
				int role_id = 0;
				long long max_hurt = 0;
				for (auto it = m_hurt_flag_map.begin(); it != m_hurt_flag_map.end(); ++it)
				{
					if (it->second > max_hurt)
					{
						max_hurt = it->second;
						role_id = it->first;
					}
				}

				GuildID old_shouchen_guild_id = m_shou_guild_id;
				GuildID shouchen_guild_id = joiner_item->guild_id;
				if (role_id != 0)
				{
					if (nullptr != this->GetJoinerItem(role_id))
					{
						//dps第一名
						joiner_item = this->GetJoinerItem(role_id);
						shouchen_guild_id = joiner_item->guild_id;
					}

					this->AddZhangong(joiner_item, other_cfg.kill_boos_zhangong);
					joiner_item->kill_flags++;

					Role *dps_killer = m_scene->GetRoleByUID(role_id);
					if (nullptr != dps_killer)
					{
						this->SyncRoleInfo(dps_killer);
					}
					else
					{
						this->SyncRoleInfo(killer);
					}
				}

				m_is_po_cheng = true;

				// 增加守方的守城时间
				GuildItem *win_guild = this->GetGuildItem(m_shou_guild_id);
				if(NULL != win_guild) 
				{
					win_guild->shouchen_time += static_cast<unsigned int>(EngineAdapter::Instance().Time() - m_last_shoucheng_time);
				}
			
				m_po_cheng_times++;

				this->CalcGuildRank();

				// 奖励给角色的战功，破城的攻方，跟普通攻方奖励的战功不一样
				this->CalcRoleZhangongWhenPoCheng(shouchen_guild_id);

				this->UpdateChenZhuInfo(shouchen_guild_id);

				this->NotifySideSystemMsg(NOTICE_TYPE_PO_QI);

				this->SyncGlobalInfoToAll();

				this->SyncRoleInfoToAll();

				//this->ResetPosAll();

				//this->GongchengzhanInit();

				m_hurt_flag_map.clear();
				
				// 被占领传闻
				{
					int send_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_flag_be_occupied_notice,
						monster->GetMonsterId());

					World::GetInstWorld()->GetSceneManager()->SystemMsgToGuild(old_shouchen_guild_id, gamestring::GAMESTRING_BUF, send_len, SYS_MSG_EVENT_TYPE_SPECIAL_NOTICE);
				}
			}
		}
	}
}

void SpecialGongChengZhan::OnHurtOther(Character *cha, Character *target)
{
	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		Role *hurt_role = (Role *)cha;
		Role *target_role = (Role *)target;

		if (NULL == hurt_role || NULL == target_role)
		{
			return;
		}

		JoinerItem *target_joiner = this->GetJoinerItem(target_role->GetUID());
		if (NULL != target_joiner)
		{
			target_joiner->hurt_map[hurt_role->GetUserId()] = EngineAdapter::Instance().Time();
		}
	}
}

// 个人战功每达到一个等级，就发一次奖励
void SpecialGongChengZhan::RewardOfZhangong(JoinerItem *joiner_item)
{
	if (NULL == joiner_item) return;
	
	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(joiner_item->uid));
	if (NULL == role) return;

	role->GetRoleGoal()->CheckBattleFieldCond(ACTIVITY_TYPE_GONGCHENGZHAN, joiner_item->zhangong);

	GczZhangongRewardItemCfg *rewardItem = LOGIC_CONFIG->GetGongChengZhanConfig().GetGczZhangongRewardItemCfgForZhangong(joiner_item->zhangong);
	if(NULL != rewardItem && NULL != role)
	{
		if(joiner_item->zhangong_reward_mark != rewardItem->zhangong)
		{
			joiner_item->zhangong_reward_mark = rewardItem->zhangong;
			
			// 保存获得过的奖励
			for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
			{
				if (rewardItem->reward_item_list[i].item_id != 0 && rewardItem->reward_item_list[i].num != 0)
				{
					this->AddRewardInfo(joiner_item, rewardItem->reward_item_list[i]);
				}	
			}
			joiner_item->shengwang_reward += rewardItem->shengwang_reward;

			//直接发奖励
			role->GetKnapsack()->PutListOrMail(rewardItem->reward_item_list, PUT_REASON_GCZ_ACTIVITY_ZHANGONG);
			role->GetRoleShop()->AddChestShopShengwang(rewardItem->shengwang_reward, "GCZRewardOfZhangong");

			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_fetch_target_reward,
					role->GetUID(), role->GetName(), (int)role->GetCamp(), rewardItem->zhangong);
				if (length > 0) 
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				}
			}
		}
	}
}

void SpecialGongChengZhan::MailRewardOnActClose()
{
	if (static_cast<int>(m_guild_rank_vec.size()) <= 0 || !m_is_finish)
	{
		return;
	}

	const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();
	
	if(m_guild_rank_vec.empty()) return;
	GuildItem *win_guild_item = m_guild_rank_vec[0];
	if(NULL == win_guild_item) return;
	
	// 胜利的公会活动奖励
	{
		Guild *guild = GuildManager::Instance().GetGuild(win_guild_item->guild_id);
		if(NULL != guild)
		{
			// 工资
			{
				guild->AddGuildGongzi(other_cfg.win_reward_gongzi);

				int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat), "guild_gongzi_gongchengzhan_chat",
					other_cfg.win_reward_gongzi);
				if (length > 0)
				{
					Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)gamestring::GAMESTRING_BUF;
					cc->channel_type = chatdef::CHANNEL_TYPE_GUILD_SYSTEM;
					cc->msg_length = length;

					guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat), chatdef::ORIGIN_TYPE_GUILD_SYSTEM_MSG);
				}
			}

			int chengzhu_id = guild->GetGuildMemberManager().GetTuanZhangUid();
			int rank_size = static_cast<int>(win_guild_item->member_uid_vec.size());
			for (int i = 0; i < rank_size; ++ i)
			{
				int role_id = win_guild_item->member_uid_vec[i];
				
				JoinerItem *joiner_item = this->GetJoinerItem(role_id);
				if(NULL == joiner_item) continue;
				
				Role *joiner_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id));
				if (NULL != joiner_role)
				{
					EventHandler::Instance().OnGongChengZhanWin(joiner_role);
				}
				
				static MailContentParam contentparam; 
				contentparam.Reset();

				ItemConfigData item_data[MAX_ATTACHMENT_ITEM_NUM];	
				int length1 = 0;
				if(chengzhu_id == role_id)
				{	
					// 发放奖励给胜利公会的会长
					contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_CONVERTSHOP_SHENGWANG, other_cfg.cz_shengwang_reward);

					joiner_item->gold_reward += 0;
					joiner_item->shengwang_reward += other_cfg.cz_shengwang_reward;

					int open_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
					if (NULL != joiner_role && CrossConfig::Instance().IsHiddenServer())
					{
						const UniqueServerID unique_server_id = joiner_role->GetRoleCross()->GetOriginalUniqueServerID();
						open_day = CrossManager::Instance().GetGameServerDayIndex(unique_server_id) + 1;
					}
					bool is_first = 0 != WorldStatus::Instance().GetCommonData().is_gongchengzhan_first;
					if (open_day <= other_cfg.opengame_day && !is_first)
					{
						contentparam.gold = other_cfg.cz_gold_reward;
						joiner_item->gold_reward += other_cfg.cz_gold_reward;
						WorldStatus::Instance().GetCommonData().is_gongchengzhan_first = 1;
					}
	
					//item_data[0] = other_cfg.cz_reward_item;
					for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
					{
						if (other_cfg.cz_reward_item[i].item_id <= 0 || other_cfg.cz_reward_item[i].num <= 0)
						{
							break;
						}
						item_data[i].item_id = other_cfg.cz_reward_item[i].item_id;
						item_data[i].is_bind = other_cfg.cz_reward_item[i].is_bind;
						item_data[i].num = other_cfg.cz_reward_item[i].num;
					}
					length1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_win_huizhang_reward_content);

					if (0 == WorldStatus::Instance().GetOpenServerGCZID() && RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_OPEN_SERVER_BATTLE))
					{
						UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(chengzhu_id);
						if (NULL != user_node)
						{
							WorldStatus::Instance().SetOpenServerGCZID(chengzhu_id);
							MailContentParam contentparam; contentparam.Reset();

							const GczOtherCfg& other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();
							for (int item_count = 0; item_count < other_cfg.huizhang_extra_reward_item_count && item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
							{
								if (other_cfg.huizhang_extra_reward[item_count].num > 0)
								{
									contentparam.AddItem(other_cfg.huizhang_extra_reward[item_count].item_id, other_cfg.huizhang_extra_reward[item_count].num, other_cfg.huizhang_extra_reward[item_count].is_bind);
									LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, user_node->uid, user_node->user_name, NULL, user_node->level, other_cfg.huizhang_extra_reward[item_count].item_id, NULL, NULL, other_cfg.huizhang_extra_reward[item_count].num, other_cfg.huizhang_extra_reward[item_count].is_bind, 0, 0);
								}
							}

							int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_gcz_rankreward_open_server);

							if (length_1 > 0)
							{
								MailRoute::MailToUser(IntToUid(user_node->uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
							}
						}
					}
				}
				else
				{	
					// 发放给普通会员
					contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_CONVERTSHOP_SHENGWANG, other_cfg.win_shengwang_reward);
					joiner_item->shengwang_reward += other_cfg.win_shengwang_reward;

					for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
					{
						if (other_cfg.win_reward_item[i].item_id <= 0 || other_cfg.win_reward_item[i].num <= 0)
						{
							break;
						}
						item_data[i].item_id = other_cfg.win_reward_item[i].item_id;
						item_data[i].is_bind = other_cfg.win_reward_item[i].is_bind;
						item_data[i].num = other_cfg.win_reward_item[i].num;
					}
					length1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_win_huiyuan_reward_content);
				}

				this->AddRewardListInfo(joiner_item, item_data, MAX_ATTACHMENT_ITEM_NUM);
				
				for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
				{
					contentparam.AddItem(item_data[i]);
				}

				if (length1 > 0)
				{
					MailRoute::MailToUser(IntToUid(role_id), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}

			//集体奖励
			{
				MailContentParam contentparam_1; contentparam_1.Reset();

				for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
				{
					if (other_cfg.team_reward_item[i].item_id <= 0 || other_cfg.team_reward_item[i].num <= 0)
					{
						break;
					}

					if (i == 0)
					{
						int length = SNPRINTF(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat),
							gamestring::g_guild_activity_team_reward_notice, ACTIVITY_TYPE_GONGCHENGZHAN, other_cfg.team_reward_item[i].item_id,
							other_cfg.team_reward_item[i].num);

						if (length > 0)
						{
							Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)(gamestring::GAMESTRING_BUF);
							cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
							cc->msg_length = length;

							guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
						}
					}

					contentparam_1.AddItem(other_cfg.team_reward_item[i].item_id, other_cfg.team_reward_item[i].num, other_cfg.team_reward_item[i].is_bind);
				}

				int length2 = SNPRINTF(contentparam_1.contenttxt, sizeof(contentparam_1.contenttxt), gamestring::g_gongchengzhan_team_reward_content);

				if (length2 > 0)
				{
					MailRoute::MailToUser(IntToUid(chengzhu_id), SYSTEM_MAIL_REASON_INVALID, contentparam_1);
				}
			}
		}
	}

	// 失败玩家获得奖励
	for (std::map<int, GuildItem *>::iterator it = m_guild_map.begin(); m_guild_map.end() != it; ++ it)
	{
		GuildItem *guild_item = it->second;
		if (NULL == guild_item) continue;

		if (win_guild_item->guild_id == guild_item->guild_id) continue;	 

		for (int i = 0; i < static_cast<int>(guild_item->member_uid_vec.size()); ++ i)
		{
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(guild_item->member_uid_vec[i]));
			if (NULL == role) continue;

			static MailContentParam contentparam; 
			contentparam.Reset();
			ItemConfigData item_data[MAX_ATTACHMENT_ITEM_NUM];
			for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
			{
				if (other_cfg.fail_reward_item[i].item_id <= 0 || other_cfg.fail_reward_item[i].num <= 0)
				{
					break;
				}
				item_data[i].item_id = other_cfg.fail_reward_item[i].item_id;
				item_data[i].is_bind = other_cfg.fail_reward_item[i].is_bind;
				item_data[i].num = other_cfg.fail_reward_item[i].num;
			}

			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_CONVERTSHOP_SHENGWANG, other_cfg.fail_shengwang_reward);
			
			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_fail_reward_content);
			
			for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
			{
				contentparam.AddItem(item_data[i]);
			}
			
			if (length > 0)
			{
				MailRoute::MailToUser(IntToUid(guild_item->member_uid_vec[i]), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
			
			JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
			if(NULL != joiner_item)
			{
				joiner_item->shengwang_reward += other_cfg.fail_shengwang_reward;
				this->AddRewardListInfo(joiner_item, item_data, MAX_ATTACHMENT_ITEM_NUM);
			}
		}
	}

	// 活动结束时最后守城的获得奖励
// 	{
// 
// 		GuildItem *guild_item = this->GetGuildItem(m_shou_guild_id);
// 		if(NULL != guild_item)
// 		{	int rank_size = static_cast<int>(guild_item->member_uid_vec.size());
// 		for (int i = 0; i < rank_size; ++ i)
// 		{
// 			int role_id = guild_item->member_uid_vec[i];
// 
// 			static MailContentParam contentparam; 
// 			contentparam.Reset();
// 
// 			contentparam.item_list[0].item_id = other_cfg.reward_item3.item_id;
// 			contentparam.item_list[0].is_bind = other_cfg.reward_item3.is_bind;
// 			contentparam.item_list[0].num = other_cfg.reward_item3.num;
// 
// 			JoinerItem *joiner_item = this->GetJoinerItem(role_id);
// 			if(NULL != joiner_item)
// 			{
// 				this->AddRewardInfo(joiner_item, other_cfg.reward_item3);
// 			}
// 
// 			int length1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_last_chenzhu_reward_content);
// 			if (length1 > 0)
// 			{
// 				MailRoute::MailToUser(IntToUid(role_id), SYSTEM_MAIL_REASON_INVALID, contentparam);
// 			}
// 
// 		}
// 		}
// 	}
}

void SpecialGongChengZhan::UpdateChenZhuInfo(Role *role)
{
	if (NULL != role)
	{
		int win_guild_id = role->GetGuildID();
		Guild *guild = GuildManager::Instance().GetGuild(win_guild_id);
		if (NULL != guild)
		{
			//int chengzhu_id = guild->GetGuildMemberManager().GetTuanZhangUid();
			//WorldStatus::Instance().OnSyncGCZInfo(win_guild_id, chengzhu_id);	
			m_shou_guild_id = win_guild_id;
			m_cheng_zhu_prof = role->GetProf();
		}
	}
	else
	{
		if (m_guild_rank_vec.empty()) return;
		GuildItem *guild_item = m_guild_rank_vec[0];
		if(NULL != guild_item)
		{
			Guild *guild = GuildManager::Instance().GetGuild(guild_item->guild_id);
			if(NULL != guild)
			{
				int chengzhu_id = guild->GetGuildMemberManager().GetTuanZhangUid();
				WorldStatus::Instance().OnSyncGCZInfo(guild_item->guild_id, chengzhu_id);
			}
		}
	}
}

void SpecialGongChengZhan::UpdateChenZhuInfo(GuildID guild_id)
{
	if (guild_id != INVALID_GUILD_ID)
	{
		int win_guild_id = guild_id;
		Guild *guild = GuildManager::Instance().GetGuild(win_guild_id);
		if (NULL != guild)
		{
			m_shou_guild_id = win_guild_id; 
			m_cheng_zhu_prof = guild->GetTuanzhangProf();
		}
	}
}

// 当砍掉战旗时，增加攻方的每个成员的战功
void SpecialGongChengZhan::CalcRoleZhangongWhenPoCheng(GuildID guild_id)
{
	const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();

	for (std::map<int, GuildItem *>::iterator it = m_guild_map.begin(); m_guild_map.end() != it; ++ it)
	{
		GuildItem *guild_item = it->second;
		if (NULL == guild_item)
		{
			continue;
		}

		if (m_shou_guild_id == guild_item->guild_id)	
		{
			continue;
		}

		for (int i = 0; i < static_cast<int>(guild_item->member_uid_vec.size()); ++ i)
		{
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(guild_item->member_uid_vec[i]));
			if (NULL == role)
			{
				continue;
			}

			JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
			if (NULL == joiner_item)
			{
				continue;
			}

			if(guild_item->guild_id == guild_id)
			{
				// 新城主的公会加战功
				this->AddZhangong(joiner_item, other_cfg.kill_boos_win_zhangong);		
			}
			else
			{
				// 其他攻方的公会加战功
				this->AddZhangong(joiner_item, other_cfg.kill_boos_attack_zhangong);	
			}
		}
	}
}

// 按战功进行个人排名
void SpecialGongChengZhan::CalcPersonalRank()
{
	int old_uid = 0;
	if (m_personal_rank_vec.size() > 0)
	{
		old_uid = m_personal_rank_vec[0]->uid;
	}

	m_personal_rank_vec.clear();

	for (std::map<int, JoinerItem *>::iterator it = m_joiner_map.begin(); m_joiner_map.end() != it; ++ it)
	{
		if (NULL != it->second)
		{
			m_personal_rank_vec.push_back(it->second);
		}
	}

	std::sort(m_personal_rank_vec.begin(), m_personal_rank_vec.end(), &SortFuncByZhanGong);

	if (m_personal_rank_vec.size() > 0)
	{
		if (m_personal_rank_vec[0]->uid != old_uid && old_uid != 0)
		{
			ActivityMsgManager::RankTopItem first;
			first.uid = m_personal_rank_vec[0]->uid;
			F_STRNCPY(first.user_name, m_personal_rank_vec[0]->role_name, sizeof(GameName));
			ActivityMsgManager::Instance().RankFirstMsg(m_scene, first);
		}

		if (ActivityMsgManager::Instance().IsNeedRankTopMsg(m_scene))
		{
			ActivityMsgManager::RankTopItem top[ACTIVITY_RANK_TOP_MSG_MAX];
			int i = 0;
			for (; i < ACTIVITY_RANK_TOP_MSG_MAX && i < (int)m_personal_rank_vec.size(); ++i)
			{
				top[i].uid = m_personal_rank_vec[i]->uid;
				F_STRNCPY(top[i].user_name, m_personal_rank_vec[i]->role_name, sizeof(GameName));
			}
			ActivityMsgManager::Instance().RankTopMsg(m_scene, top, i);
		}
	}
}

void SpecialGongChengZhan::CalcGuildPersonalRank()
{
	for (std::map<int, GuildItem*>::iterator gd_it = m_guild_map.begin(); gd_it != m_guild_map.end(); ++gd_it)
	{
		if (gd_it->second->member_joiner_vec.size()>0)
		{
			int old_mvp = gd_it->second->member_joiner_vec[0]->uid;
			std::sort(gd_it->second->member_joiner_vec.begin(), gd_it->second->member_joiner_vec.end(), &SortFuncByZhanGong);
			if (old_mvp != gd_it->second->member_joiner_vec[0]->uid)
			{
				this->SyncGuildMvpInfo(gd_it->first);
			}
		}
	}
}

// 按守城时间排名
void SpecialGongChengZhan::CalcGuildRank()
{
	std::vector<GuildItem*> guild_rank = m_guild_rank_vec ;
	m_guild_rank_vec.clear();

	for (std::map<int, GuildItem *>::iterator it = m_guild_map.begin(); m_guild_map.end() != it; ++ it)
	{
		if (NULL != it->second)
		{
			m_guild_rank_vec.push_back(it->second);
		}
	}

	std::sort(m_guild_rank_vec.begin(), m_guild_rank_vec.end(), &SortFuncByShouchenTime);

	if (guild_rank != m_guild_rank_vec)
	{
		this->SyncGlobalInfoToAll();
	}
}

void SpecialGongChengZhan::SyncGlobalInfoToAll()
{
	static Protocol::SCGCZGlobalInfo cmd;
	cmd.is_finish = m_is_finish ? 1 : 0;
	cmd.is_pochen = m_is_po_cheng ? 1 : 0;
	cmd.is_poqiang = m_is_po_qiang ? 1 : 0;
	cmd.cheng_zhu_prof = m_cheng_zhu_prof;

	cmd.shou_guild_id = m_shou_guild_id;
	cmd.rank_count = 0;

	cmd.shou_guild_name[0] = '\0';
	cmd.po_cheng_times = m_po_cheng_times;

	Guild *shou_guild = GuildManager::Instance().GetGuild(cmd.shou_guild_id);
	if(NULL != shou_guild)
	{
		shou_guild->GetGuildName(cmd.shou_guild_name);
		cmd.totem_level = shou_guild->GetTotemLevel();
	}

	int shoucheng_time = 0;

	GuildItem *guild_item_chengzhu = this->GetGuildItem(cmd.shou_guild_id);
	if(NULL != guild_item_chengzhu)
	{
		shoucheng_time = guild_item_chengzhu->shouchen_time;
	}

	cmd.current_shou_cheng_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) - m_last_shoucheng_time + shoucheng_time;

	// 公会排名
	int gild_size = static_cast<int>(m_guild_rank_vec.size());
	for (int i = 0; i < Protocol::SCGCZGlobalInfo::RANK_NUM && i < gild_size; ++ i)
	{
		Protocol::SCGCZGlobalInfo::RankItem &rank_item = cmd.rank_list[i];

		GuildItem *guild_item = m_guild_rank_vec[i];
		if (NULL == guild_item) continue;

		Guild *guild = GuildManager::Instance().GetGuild(guild_item->guild_id);
		if (NULL == guild) continue;

		rank_item.guild_id = guild_item->guild_id;
		rank_item.shouchen_time = guild_item->shouchen_time;
		guild->GetGuildName(rank_item.guild_name);

		++ cmd.rank_count;
	}

	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL == role || INVALID_GUILD_ID == role->GetGuildID()) continue;

		GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());
		if (NULL == guild_item)  continue;

		cmd.self_guild_rank_pos = this->GetGuildRankPosByGuildId(guild_item->guild_id);

		unsigned int sendlen = sizeof(cmd) - sizeof(cmd.rank_list[0]) * (Protocol::SCGCZGlobalInfo::RANK_NUM - cmd.rank_count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sendlen);
	}
}

int SpecialGongChengZhan::GetGuildRankPosByGuildId(GuildID guild_id)
{
	for (int i = 0; i < static_cast<int>(m_guild_rank_vec.size()); ++i)
	{
		if (m_guild_rank_vec[i] != nullptr && m_guild_rank_vec[i]->guild_id == guild_id)
		{
			return i + 1;
		}
	}

	return 0;
}

void SpecialGongChengZhan::SyncRoleInfoToAll()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		this->SyncRoleInfo(temp_role);
	}
}

void SpecialGongChengZhan::SyncRoleInfo(Role *role)
{
	if (NULL == role) return;

	GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());
	if (NULL == guild_item)
	{
		return;
	}

	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
	if (NULL == joiner_item)
	{
		return;
	}

	static Protocol::SCGCZRoleInfo cmd;
	cmd.is_shousite = this->IsShouRole(guild_item) ? 1 : 0;
	cmd.sos_times = joiner_item->sos_times;
	cmd.reserve_2 = 0;
	cmd.zhangong = joiner_item->zhangong;
	cmd.rank_count = 0;

	// 个人战功排名
	int role_size = static_cast<int>(m_personal_rank_vec.size());
	for (int i = 0; i < Protocol::SCGCZRoleInfo::RANK_NUM && i < role_size; ++ i)
	{
		Protocol::SCGCZRoleInfo::RankItem &rank_item = cmd.rank_list[i];

		JoinerItem *item = m_personal_rank_vec[i];
		if (NULL == item) continue;

		rank_item.role_id = item->uid;
		rank_item.zhangong = item->zhangong;
		F_STRNCPY(rank_item.role_name, item->role_name, sizeof(GameName));

		++ cmd.rank_count;
	}

	unsigned int sendlen = sizeof(cmd) - sizeof(cmd.rank_list[0]) * (Protocol::SCGCZRoleInfo::RANK_NUM - cmd.rank_count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sendlen);
}

// 保存角色在攻城战中获得的奖励，活动结束后做展示
void SpecialGongChengZhan::AddRewardInfo(JoinerItem *joiner_item, ItemConfigData reward_item)
{
	if (NULL == joiner_item)
	{
		return;
	}
	if (reward_item.item_id <= 0 || reward_item.num <= 0)
	{
		return;
	}
	bool is_exist = false;
	for(int i = 0; i < static_cast<int>(joiner_item->reward_item_vec.size()); ++ i)
	{
		if(reward_item.item_id == joiner_item->reward_item_vec[i].item_id)
		{
			joiner_item->reward_item_vec[i].num += reward_item.num;
			is_exist = true;
			break;
		}
	}

	if(!is_exist)
	{
		joiner_item->reward_item_vec.push_back(reward_item);
	}
}

void SpecialGongChengZhan::AddRewardListInfo(JoinerItem *joiner_item, ItemConfigData *reward_item, int reward_count)
{
	if (NULL == joiner_item || NULL == reward_item || reward_count <= 0 || reward_count > MAX_ATTACHMENT_ITEM_NUM)
	{
		return;
	}

	for (int j = 0; j < reward_count; ++j)
	{
		if (reward_item[j].item_id <= 0 || reward_item[j].num <= 0)
		{
			break;
		}
		bool is_exist = false;
		for (int i = 0; i < static_cast<int>(joiner_item->reward_item_vec.size()); ++i)
		{
			if (reward_item[j].item_id == joiner_item->reward_item_vec[i].item_id)
			{
				joiner_item->reward_item_vec[i].num += reward_item[j].num;
				is_exist = true;
				break;
			}
		}
		if (!is_exist)
		{
			joiner_item->reward_item_vec.push_back(reward_item[j]);
		}
	}
}

void SpecialGongChengZhan::SyncRewardInfo()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL == role) continue;

		GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());
		if (NULL == guild_item)
		{
			return;
		}

		JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
		if (NULL == joiner_item)
		{
			return;
		}

		static Protocol::SCGCZRewardInfo cmd;
		cmd.honor_reward = joiner_item->honor_reward;
		cmd.gold_reward = joiner_item->gold_reward;
		cmd.shengwang_reward = joiner_item->shengwang_reward;
		cmd.chengzhu_uid = WorldStatus::Instance().GetGCZChengzhuID();
		cmd.item_count = 0;

		for(int i = 0; i < static_cast<int>(joiner_item->reward_item_vec.size()) && i < Protocol::SCGCZRewardInfo::MAX_ITEM_COUNT; ++ i)
		{
			Protocol::SCGCZRewardInfo::ItemInfo &item_list = cmd.item_list[i];
			item_list.reward_item_id = joiner_item->reward_item_vec[i].item_id;
			item_list.reward_item_num = joiner_item->reward_item_vec[i].num;

			++ cmd.item_count;
		}

		unsigned int sendlen = sizeof(cmd) - sizeof(cmd.item_list[0]) * (Protocol::SCGCZRewardInfo::MAX_ITEM_COUNT - cmd.item_count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sendlen);
	}
}

bool SpecialGongChengZhan::GetRoleIsShouSite(Role *role)
{
	if (NULL == role) return false;
	return role->GetGuildID() == m_shou_guild_id;
}

int SpecialGongChengZhan::GetRoleSite(Role *role)
{
	if (NULL == role) return false;

	if (role->GetGuildID() == m_shou_guild_id)
	{
		return SITE_TYPE_SHOU;
	}

	return SITE_TYPE_GONG;
}

SpecialGongChengZhan::GuildItem *SpecialGongChengZhan::GetGuildItem(GuildID guild_id)
{
	std::map<int, GuildItem *>::iterator iter = m_guild_map.find(guild_id);
	if (m_guild_map.end() == iter)
	{
		return NULL;
	}

	return iter->second;
}

SpecialGongChengZhan::JoinerItem *SpecialGongChengZhan::GetJoinerItem(int uid)
{
	std::map<int, JoinerItem *>::iterator iter = m_joiner_map.find(uid);
	if (m_joiner_map.end() == iter)
	{
		return NULL;
	}

	return iter->second;
}

void SpecialGongChengZhan::AddZhangong(JoinerItem *joiner,int zhanggong)
{
	joiner->zhangong += zhanggong;
	this->RewardOfZhangong(joiner);
	joiner->add_zhangong_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
}

void SpecialGongChengZhan::ResetPosAll()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		this->ResetPos(temp_role);
	}
}

// 复活面板
bool SpecialGongChengZhan::SpecialRelive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		Posi realive_pos(0, 0);
		LOGIC_CONFIG->GetGongChengZhanConfig().GetEnterPos(this->GetRoleIsShouSite(role), &realive_pos);
		if (realive_pos.x == 0 || realive_pos.y == 0)
		{
			return false;
		}

		role->ReAlive(SPECIAL_ACTIVITY_FREE_REALIVE_HP_PER, 100, realive_pos);// 免费复活，只加百分之50的血

		return true;
	}

	return false;
}

void SpecialGongChengZhan::ResetPos(Role *role)
{
	if (m_is_finish || NULL == role)
	{
		return;
	}

	const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();

	Posi realive_pos(0, 0);
	LOGIC_CONFIG->GetGongChengZhanConfig().GetEnterPos(this->GetRoleIsShouSite(role), &realive_pos);
	if (realive_pos.x == 0 || realive_pos.y == 0)
	{
		return;
	}

	if(role->IsAlive() && !this->IsPosInArea(&role->GetPos(), &other_cfg.resource_zuo_xia_pos, &other_cfg.resource_you_shang_pos))
	{
		role->ResetPos(realive_pos.x, realive_pos.y);
	}
}

void SpecialGongChengZhan::OnGczChangePlace(Role *role, int place_type)
{
	if (m_is_finish || NULL == role)
	{
		return;
	}

	// 攻击状态不能传送
	if(role->GetRoleStatusManager()->IsInFightStatus())
	{
		return;
	}

	const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();

	GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());

	if(role->IsAlive() && NULL != guild_item)
	{
		if(Protocol::GCZ_GOTO_GONG_PLACE == place_type && !this->IsShouRole(guild_item))
		{
			role->ResetPos(other_cfg.gong_qiang_pos.x, other_cfg.gong_qiang_pos.y);
		}
		else if(Protocol::GCZ_GOTO_SHOU_PLACE == place_type && this->IsShouRole(guild_item))
		{
			role->ResetPos(other_cfg.relive2_pos.x, other_cfg.relive2_pos.y);
		}
		else if(Protocol::GCZ_GOTO_ZIYUAN_PLACE == place_type)
		{
			role->ResetPos(other_cfg.resource_pos.x, other_cfg.resource_pos.y);
		}
	}
}

bool SpecialGongChengZhan::OnSosCost(Protocol::CSSendGuildSosReq *req, Role *role)
{
	if (nullptr == role || nullptr == req)
	{
		return false;
	}

	JoinerItem *joiner = this->GetJoinerItem(role->GetUID());
	if (nullptr == joiner)
	{
		return false;
	}

	GuildID sos_guild_id = role->GetGuildID();

	int cost = LOGIC_CONFIG->GetGongChengZhanConfig().GetSosCost(joiner->sos_times);
	if (cost > 0)
	{
		if (!role->GetKnapsack()->GetMoney()->UseAllGold(cost, __FUNCTION__))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return false;
		}
	}

	++joiner->sos_times;

	static Protocol::SCGuildMemberSos gms;
	gms.sos_type = req->sos_type;
	gms.member_uid = UidToInt(role->GetUserId());
	role->GetName(gms.member_name);
	gms.member_scene_id = role->GetScene()->GetSceneID();
	gms.member_pos_x = role->GetPos().x;
	gms.member_pos_y = role->GetPos().y;
	gms.enemy_uid = 0;
	memset(gms.enemy_name, 0, sizeof(gms.enemy_name));
	gms.enemy_camp = 0;
	gms.enemy_guild_id = INVALID_GUILD_ID;
	memset(gms.enemy_guild_name, 0, sizeof(gms.enemy_guild_name));

// 	FitParam param;
// 	param.guild_id = sos_guild_id;
// 	param.role_id = role->GetUID();
// 
// 	Scene::RoleFilterFunc filter_func = [](Role *role, void *param)->bool
// 	{
// 		if (nullptr != param)
// 		{
// 			FitParam *fit_param = (FitParam *)param;
// 			return role->GetGuildID() == fit_param->guild_id && role->GetUID() != fit_param->role_id;
// 		}
// 
// 		return false;
// 	};
// 
// 	m_scene->SendToRole((const char *)&gms, sizeof(gms), filter_func, (void *)&param);
	Guild * guild = GuildManager::Instance().GetGuild(sos_guild_id);
	if (nullptr != guild)
	{
		guild->SendMessageToGuildAll(Protocol::GUILD_NOTIFY_TYPE_MEMBER_SOS,(const char*)&gms,sizeof(gms));
	}

	this->SyncRoleInfo(role);

	return true;
}

bool SpecialGongChengZhan::IsPosInArea(const Posi *pos, const Posi *area_zuo_xia_pos,const Posi *area_you_shang_pos)
{
	if(pos->x >= area_zuo_xia_pos->x && pos->x <= area_you_shang_pos->x)
	{
		if(pos->y >= area_zuo_xia_pos->y && pos->y <= area_you_shang_pos->y)
		{
			return true;
		}
	}
	return false;
}

bool SpecialGongChengZhan::IsShouRole(GuildItem *guild_item)
{
	if (NULL == guild_item) return false;
	return guild_item->guild_id == m_shou_guild_id;
}

void SpecialGongChengZhan::NotifySideSystemMsg(int notice_type)
{
	if (NOTICE_TYPE_PO_MENG == notice_type)
	{
		for(unsigned int i = 0; i < m_scene->RoleNum(); i++)
		{
			Role *temp_role = m_scene->GetRoleByIndex(i);
			if (NULL != temp_role)
			{
				int length_one;
				int sendlen = 0;
				static Protocol::SCSystemMsg sm;

				GuildItem *guild_item = this->GetGuildItem(temp_role->GetGuildID());
				if(NULL != guild_item)
				{	
					if (this->IsShouRole(guild_item))
					{	
						length_one = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_gcz_pomen_shou_notice);
					}
					else
					{
						length_one = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_gcz_pomen_gong_notice);
					}

					if (length_one > 0)
					{
						if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length_one, SYS_MSG_ACTIVE_NOTICE))
						{
							EngineAdapter::Instance().NetSend(temp_role->GetNetId(),(const char*)&sm, sendlen);
						}
					}
				}
			}
		}
	}
	else if(NOTICE_TYPE_PO_QI == notice_type)
	{
		for(unsigned int i = 0; i < m_scene->RoleNum(); i++)
		{
			Role *temp_role = m_scene->GetRoleByIndex(i);
			if (NULL != temp_role)
			{
				int sendlen = 0;
				static Protocol::SCSystemMsg sm;
				
				Guild *guild = GuildManager::Instance().GetGuild(m_shou_guild_id);
				if (NULL == guild) return;
				int length_one = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_gcz_zhanqi_notice, m_shou_guild_id, guild->GetGuildName());
				
				if (length_one > 0)
				{
					if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length_one, SYS_MSG_ACTIVE_NOTICE))
					{
						EngineAdapter::Instance().NetSend(temp_role->GetNetId(),(const char*)&sm, sendlen);
					}
				}
			}
		}
	}
}

void SpecialGongChengZhan::CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist)
{
	return;
	if (NULL != kill_role && NULL != dead_role)
	{
		JoinerItem *killer = this->GetJoinerItem(kill_role->GetUID());
		JoinerItem *dead = this->GetJoinerItem(dead_role->GetUID());
		if (NULL != killer && NULL != dead)
		{
			if (!is_assist && dead->lianzhan >= 3)
			{
				int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_stoplianzhan_notice,
					UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), 
					UidToInt(dead_role->GetUserId()), dead_role->GetName(), (int)dead_role->GetCamp(), dead->lianzhan);
				if (str_len > 0) 
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
				}
			}

			if (killer->lianzhan > 0)
			{
				int str_len = 0;
				int notice_type = SYS_MSG_ACTIVE_NOTICE;

				if (0 == killer->lianzhan % 3)
				{
					notice_type = SYS_MSG_ACTIVE_NOTICE;
					str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), 
						killer->lianzhan, killer->lianzhan);
				}

				if (str_len > 0) this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, notice_type);
			}
		}
	}
}

void SpecialGongChengZhan::OnFlagBeAttacked(Monster *monster)
{
	return;
	if (nullptr == monster)
	{
		return;
	}

	// 旗子首次被攻击、低血量传闻
	if (!m_flag_has_be_hit_notice_flag)
	{
		m_flag_has_be_hit_notice_flag = true;

		int content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_flag_first_be_hit_notice,
			monster->GetMonsterId());

		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, content_len, SYS_MSG_EVENT_TYPE_SPECIAL_NOTICE))
		{
			m_scene->SendToRole((const char*)&sm, sendlen, [this](Role *role)->bool {
				if (m_shou_guild_id == role->GetGuildID())
				{
					return true;
				}

				return false;
			});
		}
	}
	else
	{
		Attribute cur_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		Attribute max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);

		// 血量广播百分比
		static int low_hp_per_list[] = { 5000, 4000, 3000, 2000, 1000, 500, 100 };

		int hp_per = static_cast<int>(cur_hp * 1.0 / max_hp * 10000);

		for (UInt32 i = 0; i < sizeof(low_hp_per_list) / sizeof(low_hp_per_list[0]) && i < BIT_COUNT_OF_BYTE * 4; ++i)
		{
			if (hp_per < low_hp_per_list[i] && 0 == (m_flag_low_hp_broadcast_flag & (1 << i)))
			{
				m_flag_low_hp_broadcast_flag |= 1 << i;

				int content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_flag_be_attacked_notice,
					monster->GetMonsterId(), low_hp_per_list[i] / 100);

				int sendlen = 0;
				static Protocol::SCSystemMsg sm;

				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, content_len, SYS_MSG_EVENT_TYPE_SPECIAL_NOTICE))
				{
					m_scene->SendToRole((const char*)&sm, sendlen);
				}

				break;
			}
		}
	}
}

void SpecialGongChengZhan::SendGuildInfoStatisticAll()
{
	this->CalcGuildPersonalRank();

	for (std::map<int, GuildItem*>::iterator gd_it = m_guild_map.begin(); gd_it != m_guild_map.end(); ++gd_it)
	{
		this->SendGuildInfoStatistic(gd_it->first, Protocol::SCGuildInfoStatistic::NOTIFY_ACTIVITY_CLOSE);
	}
}

void SpecialGongChengZhan::SendGuildInfoStatistic(long long guild_id, int notify, Role *role )
{
	Protocol::SCGuildInfoStatistic sta;
	sta.notify_type = notify;
	sta.activity_type = ACTIVITY_TYPE_GONGCHENGZHAN;
	sta.guild_id = static_cast<int>(guild_id);
	sta.count = 0;
	std::map<int, GuildItem *>::iterator gd_it = m_guild_map.find(static_cast<int>(guild_id));
	if (gd_it != m_guild_map.end())
	{
		for (unsigned int i = 0; i < gd_it->second->member_joiner_vec.size() && sta.count < MAX_MEMBER_COUNT; ++i)
		{
			JoinerItem * joiner_item = gd_it->second->member_joiner_vec[i];
			if (NULL == joiner_item) continue;
			Protocol::SCGuildInfoStatistic::InfoItem & info_item = sta.info_list[sta.count];
			info_item.uid = joiner_item->uid;
			F_STRNCPY(info_item.user_name, joiner_item->role_name, sizeof(GameName));
			info_item.guild_post = joiner_item->guild_post;
			info_item.kill_role = joiner_item->kill_num;
			info_item.kill_target = joiner_item->kill_flags;
			info_item.hurt_roles = joiner_item->hurt_roles;
			info_item.hurt_targets = joiner_item->hurt_flags;
			sta.count++;
		}
	}

	int lenth = sizeof(sta) - sizeof(Protocol::SCGuildInfoStatistic::InfoItem)*(MAX_MEMBER_COUNT - sta.count);
	if (role != NULL)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sta, lenth);
	}
	else
	{
		m_scene->SendToRole((const char *)&sta, lenth, [guild_id](Role *role)->bool {
			if (guild_id == role->GetGuildID())
				return true;
			return false;
		});
	}
}

void SpecialGongChengZhan::SyncGuildMvpInfo(GuildID guild_id, Role* role)
{
	std::map<int, GuildItem*>::iterator gd_it = m_guild_map.find(guild_id);
	if(gd_it != m_guild_map.end())
	{
		GuildID guild_id = gd_it->first;
		Protocol::SCGuildMvpInfo mvp;
		mvp.activity_type = ACTIVITY_TYPE_GONGCHENGZHAN;
		mvp.uid = 0;
		memset(mvp.user_name, 0, sizeof(mvp.user_name));
		if (gd_it->second->member_joiner_vec.size() > 0)
		{
			mvp.uid = gd_it->second->member_joiner_vec[0]->uid;
			F_STRNCPY(mvp.user_name, gd_it->second->member_joiner_vec[0]->role_name, sizeof(GameName));
		}

		if (NULL != role)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&mvp, sizeof(mvp));
		}
		else {
			m_scene->SendToRole((const char *)&mvp, sizeof(mvp), [guild_id](Role * role)->bool {
				if (guild_id == role->GetGuildID())
					return true;
				return false;
			});
		}
	}
}

void SpecialGongChengZhan::SendResultInGuildChat()
{
	// 内部不再排序，外部调用前保证已经是在排序后
	for (std::map<int, GuildItem*>::iterator gd_it = m_guild_map.begin(); gd_it != m_guild_map.end(); ++gd_it)
	{
		if (gd_it->second->member_joiner_vec.size() > 0)
		{
			int mvp_uid = gd_it->second->member_joiner_vec[0]->uid; // 排序后的mvp
			int kill_role_uid = 0;  // 最高击杀玩家者
			int kill_flag_uid = 0;  // 最高击杀雕像者
			int hurt_role_uid = 0;  // 最高伤害玩家者
			int hurt_flag_uid = 0;  // 最高伤害雕像者
			static GameName tmp_role_name[5]; memset(tmp_role_name, 0, sizeof(tmp_role_name));
			
			int hight_kill_role = 0;
			int hight_kill_flag = 0;
			long long hight_hurt_role = 0;
			long long hight_hurt_flag = 0;
			
			for (auto vec_it = gd_it->second->member_joiner_vec.begin(); vec_it != gd_it->second->member_joiner_vec.end(); ++vec_it)
			{
				const JoinerItem * item = *vec_it;
				if (NULL == item) continue;
				if (item->uid == 0) continue;

				if (item->kill_num > hight_kill_role) { kill_role_uid = item->uid;  hight_kill_role = item->kill_num; }
				if (item->kill_flags > hight_kill_flag) { kill_flag_uid = item->uid;  hight_kill_flag = item->kill_flags; }
				if (item->hurt_roles > hight_hurt_role) { hurt_role_uid = item->uid;  hight_hurt_role = item->hurt_roles; }
				if (item->hurt_flags > hight_hurt_flag) { hurt_flag_uid = item->uid;  hight_hurt_flag = item->hurt_flags; }
			}

			for (auto vec_it = gd_it->second->member_joiner_vec.begin(); vec_it != gd_it->second->member_joiner_vec.end(); ++vec_it)
			{
				const JoinerItem * item = *vec_it;
				if (NULL == item) continue;
				if (item->uid == 0) continue;
				
				if (item->uid == mvp_uid) { F_STRNCPY(tmp_role_name[0], item->role_name, sizeof(GameName)); }
				if (item->uid == kill_role_uid) { F_STRNCPY(tmp_role_name[1], item->role_name, sizeof(GameName)); }
				if (item->uid == kill_flag_uid) { F_STRNCPY(tmp_role_name[2], item->role_name, sizeof(GameName)); }
				if (item->uid == hurt_role_uid) { F_STRNCPY(tmp_role_name[3], item->role_name, sizeof(GameName)); }
				if (item->uid == hurt_flag_uid) { F_STRNCPY(tmp_role_name[4], item->role_name, sizeof(GameName)); }
			}
			
			// 发送聊天
			Guild *guild = GuildManager::Instance().GetGuild(gd_it->first);
			if (NULL != guild)
			{
				static char buff[1024]; memset(buff, 0, sizeof(buff));
				std::string sysmsg_buff;
				int msg_len = 0;

				if (mvp_uid > 0)
				{
					msg_len = gstr::SysMsgContent(buff, 1024, "guild_gongchengzhan_sysmsg_title", tmp_role_name[0]);
					if (msg_len > 0)
					{
						sysmsg_buff.append(buff);
					}
				}
				
				if (kill_flag_uid > 0)
				{
					msg_len = gstr::SysMsgContent(buff, 1024, "guild_gongchengzhan_sysmsg_killflag", tmp_role_name[2], hight_kill_flag);
					if (msg_len > 0)
					{
						sysmsg_buff.append(buff);
					}
				}

				if (kill_role_uid > 0)
				{
					msg_len = gstr::SysMsgContent(buff, 1024, "guild_gongchengzhan_sysmsg_killrole", tmp_role_name[1], hight_kill_role);
					if (msg_len > 0)
					{
						sysmsg_buff.append(buff);
					}
				}

				if (hurt_role_uid > 0)
				{
					msg_len = gstr::SysMsgContent(buff, 1024, "guild_gongchengzhan_sysmsg_hurtrole", tmp_role_name[3], hight_hurt_role);
					if (msg_len > 0)
					{
						sysmsg_buff.append(buff);
					}
				}

				//if (hurt_flag_uid > 0)
				//{
				//	msg_len = gstr::SysMsgContent(buff, 1024, "guild_gongchengzhan_sysmsg_hurtflag", tmp_role_name[4], hight_hurt_flag);
				//	if (msg_len > 0)
				//	{
				//		sysmsg_buff.append(buff);
				//	}
				//}

				int sysmsg_len = sysmsg_buff.size() - 10;  // 去掉最后一个换行，换行字符串占用10个字节
				if (sysmsg_len > 0 && sysmsg_len < (int)sizeof(buff) - (int)sizeof(Protocol::CSChannelChat))
				{
					Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)buff;
					cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
					cc->msg_length = sysmsg_len;
					F_STRNCPY(buff + sizeof(Protocol::CSChannelChat), sysmsg_buff.c_str(), sysmsg_len + 1);

					guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
bool SpecialGongChengZhan::IsWorshipDuring()
{
	const WorshipOtherConfig *other_cfg = LOGIC_CONFIG->GetWorshipConfig().GetOtherConfig(ACTIVITY_TYPE_GONGCHENGZHAN);
	if (nullptr == other_cfg)
	{
		return false;
	}

	time_t now = EngineAdapter::Instance().Time();

	return m_worship_start_time > 0 && now > m_worship_start_time && now < m_worship_start_time + other_cfg->worship_time;
}

void SpecialGongChengZhan::OnWorship(Role* role)
{
	if (nullptr == role)
	{
		return;
	}

	if (!this->IsWorshipDuring())
	{
		return;
	}

	const WorshipOtherConfig *other_cfg = LOGIC_CONFIG->GetWorshipConfig().GetOtherConfig(ACTIVITY_TYPE_GONGCHENGZHAN);
	if (nullptr == other_cfg)
	{
		return;
	}

	if (nullptr == role->GetScene() || role->GetScene()->GetSceneID() != other_cfg->worship_scene_id)
	{
		return;
	}

	//if (std::abs(role->GetPos().x - other_cfg->worship_pos.x) > other_cfg->worship_range || std::abs(role->GetPos().y - other_cfg->worship_pos.y) > other_cfg->worship_range)
	//{
	//	return;
	//}

	WorshipItem &item = m_worship_user_map[role->GetUniqueUserID()];
	if (item.manual_worship_times >= other_cfg->worship_click_times)
	{
		gstr::SendError(role, "worship_baiye_count_not_enouth");
		return;
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (item.next_can_manual_worship_timestamp > now_second)
	{
		return;
	}

	++item.manual_worship_times;
	item.next_can_manual_worship_timestamp = now_second + other_cfg->worship_click_cd;
	item.manual_worship_reward_timestamp = now_second + other_cfg->worship_gather_time;

	this->SendWorshipInfo(role);
	this->SyncWorshipStart(role);
}

void SpecialGongChengZhan::SendWorshipInfo(Role *role)
{
	if (nullptr == role) return;

	Protocol::SCRoleWorshipInfo rwi;

	WorshipItem &item = m_worship_user_map[role->GetUniqueUserID()];

	rwi.activity_type = ACTIVITY_TYPE_GONGCHENGZHAN;
	rwi.worship_times = item.manual_worship_times;
	rwi.next_worship_timestamp = item.next_can_manual_worship_timestamp;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rwi, sizeof(rwi));
}

void SpecialGongChengZhan::SyncWorshipStart(Role *role)
{
	if (nullptr == role) return;

	Protocol::SCRoleWorshipStart rws;
	rws.role_obj = role->GetId();
	rws.resever_sh = 0;

	m_scene->SendToRole((const char*)&rws, sizeof(rws));
}

void SpecialGongChengZhan::CheckWorshipReward(time_t now_second)
{
	UInt32 now_sec = static_cast<UInt32>(now_second);

	// 膜拜奖励
	for (auto &it : m_worship_user_map)
	{
		if (0 == it.second.manual_worship_reward_timestamp || now_sec < it.second.manual_worship_reward_timestamp)
		{
			continue;
		}

		it.second.manual_worship_reward_timestamp = 0;

		Role *temp_role = m_scene->GetRoleByUUID(it.first);
		if (nullptr == temp_role)
		{
			continue;
		}

		// 给予奖励
		int reward_gold_bind = LOGIC_CONFIG->GetWorshipConfig().GetReardGoldBind();
		temp_role->GetKnapsack()->GetMoney()->AddGoldBind(reward_gold_bind, "SpecialGongChengZhan::CheckWorshipReward");

	}
}

void SpecialGongChengZhan::SyncFlagInfo(Role * role)
{
	Protocol::SCGongChengZhanFlagInfo info;
	info.max_hp = m_flag_max_hp;
	info.cur_hp = m_flag_cur_hp;

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&info, sizeof(info));
	}
	else
	{
		m_scene->SendToRole((const char*)&info, sizeof(info));
	}
}
