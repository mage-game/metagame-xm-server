#include "activityshadow.hpp"
#include <algorithm>

#include "math.h"

#include "world.h"
#include "gstr.h"

#include "servercommon/userprotocal/msgsystem.h"

#include "scene/speciallogic/worldspecial/specialzhuxie.hpp"
#include "scene/speciallogic/worldspecial/specialshuijing.hpp"
#include "scene/speciallogic/worldspecial/specialqunxianluandou.hpp"
#include "scene/speciallogic/worldspecial/specialgongchengzhan.hpp"
#include "scene/speciallogic/worldspecial/specialxianmengzhan.hpp"
#include "scene/speciallogic/campspecial/specialcampstation.hpp"
#include "scene/speciallogic/worldspecial/specialnationalboss.hpp"
#include "scene/speciallogic/worldspecial/specialyizhandaodi.hpp"
#include "scene/speciallogic/worldspecial/specialwuxingguaji.hpp"
#include "scene/speciallogic/worldspecial/specialcrossxiuluotower.hpp"
#include "scene/speciallogic/worldspecial/specialguildbattlenew.hpp"
#include "scene/speciallogic/worldspecial/specialmillionaire.hpp"
#include "scene/speciallogic/worldspecial/specialxingzuoyiji.hpp"
#include "scene/speciallogic/worldspecial/specialcrossguildbattle.hpp"
#include "scene/speciallogic/worldspecial/specialcrossdarknight.hpp"
#include "scene/speciallogic/worldspecial/specialcrossfishing.hpp"
#include "scene/speciallogic/doublesidefbspecial/specialnightfight.hpp"
#include "scene/speciallogic/worldspecial/specialguildquestion.hpp"
#include "scene/speciallogic/doublesidefbspecial/specialmessbattle.hpp"
#include "scene/speciallogic/worldspecial/specialcrossliekunfb.hpp"
#include "scene/speciallogic/worldspecial/specialgiftharvest.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "config/activityconfig/zhuxieconfig.hpp"

#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "other/event/eventhandler.hpp"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"

#include "item/itempool.h"
#include "monster/monsterpool.h"

#include "internalcomm.h"
#include "gamelog.h"
#include "config/sharedconfig/sharedconfig.h"
#include "gameworld/globalconfig/globalconfig.h"
#include "gameworld/monster/monsterpool.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "servercommon/noticenum.h"
#include "global/activity/activitymanager.hpp"
#include "global/rank/rankmanager.hpp"
#include "servercommon/serverconfig/crossxiuluotowercommonconfig.hpp"
#include "global/cross/crossactivity/crossrecordmanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/serverconfig/cross1v1commonconfig.hpp"
#include "servercommon/serverconfig/crosshotspringconfig.hpp"
#include "servercommon/serverconfig/crosstuanzhanconfig.hpp"
#include "servercommon/serverconfig/crosspastureconfig.hpp"
#include "servercommon/serverconfig/crossbianjingzhidiconfig.h"
#include "scene/speciallogic/worldspecial/huangchenghuizhanmgr.hpp"
#include "scene/speciallogic/worldspecial/campdefendmanager.hpp"
#include "scene/speciallogic/worldspecial/specialcrosstuanzhan.hpp"
#include "scene/speciallogic/worldspecial/specialcrosspasture.hpp"
#include "servercommon/serverconfig/crossbossconfig.hpp"
#include "scene/speciallogic/worldspecial/specialcrossboss.hpp"
#include "scene/speciallogic/worldspecial/specialcrosshotspring.hpp"
#include "scene/speciallogic/worldspecial/specialtianjiangcaibao.h"
#include "scene/speciallogic/worldspecial/specialwanglingexplore.h"
#include "scene/speciallogic/worldspecial/specialterritorywar.hpp"
#include "scene/speciallogic/worldspecial/specialcrossshuijing.hpp"
#include "scene/guildshadow/guildshadow.hpp"
#include "servercommon/serverconfig/crossshuijingconfig.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "global/usercache/usercache.hpp"
#include "global/guild/roleguild.hpp"
#include "config/activityconfig/millionaireconfig.hpp"
#include "config/activityconfig/territorywarconfig.hpp"
#include "other/shenzhouweapon/shenzhouweaponconfig.hpp"
#include "other/shenzhouweapon/shenzhouweapon.hpp"
#include "config/activityconfig/moshenjianglinconfig.hpp"
#include "config/activityconfig/guajiactivityconfig.hpp"
#include "config/activityconfig/qunxianluandouconfig.hpp"
#include "config/activityconfig/guildquestionconfig.hpp"
#include "config/activityconfig/activityswitchconfig.hpp"
#include "item/knapsack.h"
#include "other/cross/rolecross.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"
#include "servercommon/serverconfig/crossdarknightconfig.hpp"
#include "other/shenshou/shenshou.hpp"
#include "other/fb/nightfight/nightfightfbconfig.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "protocal/msgchatmsg.h"
#include "other/fb/messbattle/messbattleconfig.hpp"
#include "servercommon/serverconfig/crossliekunfbconfig.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "gamecommon.h"
#include "global/cross/crossmanager.h"
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "servercommon/serverconfig/crossmizangbossconfig.hpp"
#include "scene/speciallogic/worldspecial/specialcrossmizangboss.hpp"
#include "servercommon/serverconfig/crossyoumingbossconfig.hpp"
#include "scene/speciallogic/worldspecial/specialcrossyoumingboss.hpp"
#include "scene/speciallogic/worldspecial/specialcorssbianjingzhidi.h"
#include "other/capability/capability.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/fabao/fabaomanager.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/yaoshi/yaoshimanager.hpp"
#include "other/upgrade/upgrade.hpp"
#include "other/upgrade/upgrademanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/footprint/footprint.hpp"
#include "other/mask/maskmanager.hpp"
#include "other/xiannv/xiannvmanager.hpp"
#include "chat/chatmanager.h"
#include "other/route/guildroute.hpp"
#include "scene/speciallogic/worldspecial/specialcrosshusongshuijing.hpp"
#include "servercommon/serverconfig/husongshuijingconfig.hpp"


ActivityShadow::ActivityShadow() 
	: m_next_monster_invade_time(0), m_next_millionaire_refresh_box_time(0), m_next_millionaire_refresh_boss_time(0), m_moshenjianglin_boss_id(0), m_guaji_reward_next_timetamp(0), m_cross_xiuluo_tower_rank_count(0), 
	m_cross_xiuluo_tower_max_layer(0), m_cross_1v1_fight_start_time(0), m_cross_1v1_last_match_time(0), m_cross_1v1_next_match_time(0), m_shenzhou_weapon_gather_num(0),
	m_shenzhou_weapon_next_refresh_gather_time(0), m_xiuluotower_boss_refresh_time(0), m_xiuluotower_boss_count(0), m_xiuluotower_server_daily_drop_title_num(0),
	m_xiuluotower_server_daily_drop_mount_num(0), m_xiuluotower_server_daily_gold_box_num(0), m_guild_battle_daily_drop_wing_num(0),
	m_question_state(QUESTION_STATE_INVALID), m_next_question_state_time(0), m_question_index(0), m_question_id(0), m_question_answer(0), m_time_count(0), m_guild_question_state(0), m_next_give_reward_timestamp(0), m_guild_question_state_change_timestamp(0),
	m_next_refresh_crossboss_time(0), m_next_refresh_cross_mizang_boss_time(0), m_next_refresh_cross_youming_boss_time(0)
{
	memset(m_csa_sub_activity_state_list, 0, sizeof(m_csa_sub_activity_state_list)); 
}

ActivityShadow::~ActivityShadow()
{

}

ActivityShadow & ActivityShadow::Instance()
{
	static ActivityShadow shadow;
	return shadow;
}

bool ActivityShadow::SetActivityStatus(ActivityStatus *as)
{
	int activity_type = as->activity_type;
	if (ACTIVITY_TYPE_INVALID < activity_type && activity_type < ACTIVITY_TYPE_MAX)
	{
		m_activity_status[activity_type].status = as->status;
		if (as->activity_begin_time > 0) m_activity_status[activity_type].activity_begin_time = as->activity_begin_time;
		if (as->activity_end_time > 0) m_activity_status[activity_type].activity_end_time = as->activity_end_time;
	}
	else if (SPECIAL_ACTIVITY_TYPE_INVALID < activity_type && activity_type < SPECIAL_ACTIVITY_TYPE_RESVERED)
	{
		int special_activity_type = activity_type - SPECIAL_ACTIVITY_TYPE_INVALID;
		if (0 < special_activity_type && special_activity_type < SPECIAL_ACTIVITY_TYPE_MAX)
		{
			m_special_activity_status[special_activity_type].status = as->status;
			m_special_activity_status[special_activity_type].activity_begin_time = as->activity_begin_time;
			m_special_activity_status[special_activity_type].activity_end_time = as->activity_end_time;
		}
	}
	else if (RAND_ACTIVITY_TYPE_BEGIN <= activity_type && activity_type < RAND_ACTIVITY_TYPE_END)
	{
		int rand_activity_type = activity_type - RAND_ACTIVITY_TYPE_BEGIN;
		if (0 <= rand_activity_type && rand_activity_type < RAND_ACTIVITY_TYPE_MAX)
		{
			m_rand_activity_status[rand_activity_type].status = as->status;
			m_rand_activity_status[rand_activity_type].activity_begin_time = as->activity_begin_time;
			m_rand_activity_status[rand_activity_type].activity_end_time = as->activity_end_time;
		}
	}
	else if (CROSS_RAND_ACTIVITY_TYPE_BEGIN <= activity_type && activity_type < CROSS_RAND_ACTIVITY_TYPE_END)
	{
		int cross_rand_activity_index = activity_type - CROSS_RAND_ACTIVITY_TYPE_BEGIN;
		if (0 <= cross_rand_activity_index && cross_rand_activity_index < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT)
		{
			m_cross_rand_activity_status[cross_rand_activity_index].status = as->status;
			m_cross_rand_activity_status[cross_rand_activity_index].activity_begin_time = as->activity_begin_time;
			m_cross_rand_activity_status[cross_rand_activity_index].activity_end_time = as->activity_end_time;
		}
	}
	else
	{
		return false;
	}

	return true;
}

void ActivityShadow::OnSyncActivityStatus(ActivityStatus *as)
{
	if (!this->SetActivityStatus(as)) return;

	UNSTD_STATIC_CHECK(36 == ACTIVITY_TYPE_MAX);

	switch (as->activity_type)
	{
	case ACTIVITY_TYPE_ZHUXIE:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnZhuxieClose();
			}

			m_zhuxie_usermap.clear(); 

			for (int i = 0; i < ACTIVITY_ROOM_MAX; i++)
			{
				SpecialZhuXie *zhuxie_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialZhuXie(this->GetFBKeyByRoomIndex(i), true);
				if (NULL != zhuxie_logic)
				{
					zhuxie_logic->OnActivityStatusChange(as->status);
				}
			}
		}
		break;

	case ACTIVITY_TYPE_MONSTER_INVADE:
		{
			if (ACTIVITY_STATUS_OPEN == as->status)
			{
				m_next_monster_invade_time = EngineAdapter::Instance().Time();
			}
			else if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				m_next_monster_invade_time = 0;
			}
		}
		break;

	case ACTIVITY_TYPE_NATIONAL_BOSS:
		{
			SpecialNationalBoss *nb_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialNationBoss(this->GetFBKeyByRoomIndex(0), true);
			if (NULL != nb_logic)
			{
				nb_logic->OnActivityStatusChange(as->status);
			}
		}
		break;

	case ACTIVITY_TYPE_QUNXIANLUANDOU:
		{
			// 结束的时候保证先发排行信息
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				for (int i = 0; i < ACTIVITY_ROOM_MAX; i++)
				{
					SpecialQunXianLuanDou *qunxianluandou_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialQunXianLuanDou(this->GetFBKeyByRoomIndex(i), true);
					if (NULL != qunxianluandou_logic)
					{
						qunxianluandou_logic->CheckSendFirstRank();
					}
				}
			}

			for (int i = 0; i < ACTIVITY_ROOM_MAX; i++)
			{
				SpecialQunXianLuanDou *qunxianluandou_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialQunXianLuanDou(this->GetFBKeyByRoomIndex(i), true);
				if (NULL != qunxianluandou_logic)
				{
					qunxianluandou_logic->OnActivityStatusChange(as->status);
				}
			}

			if(ACTIVITY_STATUS_CLOSE == as->status)
			{
				if (0 == WorldStatus::Instance().GetOpenServerQunxianID() && RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_OPEN_SERVER_BATTLE))
				{
					UserID first_use_id = INVALID_USER_ID;
					RankManager::Instance().GetPersonRank()->GetRankTopUser(PERSON_RANK_TYPE_QUNXIANLUANDOU, 1, &first_use_id);
					if (INVALID_USER_ID != first_use_id)
					{
						UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(UidToInt(first_use_id));
						if (NULL != user_node)
						{
							WorldStatus::Instance().SetOpenServerQunxianID(UidToInt(first_use_id));
							MailContentParam contentparam; contentparam.Reset();

							ItemConfigData reward_list[MAX_ATTACHMENT_ITEM_NUM];
							int reward_count = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetOpenServerReward(reward_list);
							for (int item_count = 0; item_count < reward_count && item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
							{
								if (reward_list[item_count].num > 0)
								{
									contentparam.AddItem(reward_list[item_count].item_id, reward_list[item_count].num, reward_list[item_count].is_bind);
									LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_QUNXIANLUANDOU, user_node->uid, user_node->user_name, NULL, user_node->level, reward_list[item_count].item_id, "Reward", NULL, reward_list[item_count].num, reward_list[item_count].is_bind, 0, 0);
								}
							}

							int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_qunxianluandou_qunxian_open_server);

							if (length_1 > 0)
							{
								MailRoute::MailToUser(IntToUid(user_node->uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
							}
						}
					}
				}
			}
		}
		break;
	
	case ACTIVITY_TYPE_GONGCHENGZHAN:
		{
			SpecialGongChengZhan *gongchengzhan_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialGongChengZhan(this->GetFBKeyByRoomIndex(0), true);	// 只创建一个副本
			if (NULL != gongchengzhan_logic)
			{
				gongchengzhan_logic->OnActivityStatusChange(as->status);
			}
		}
		break;

	case ACTIVITY_TYPE_XIANMENGZHAN:
		{
			SpecialXianMengZhan *xianmengzhan_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialXianMengZhan(this->GetFBKeyByRoomIndex(0), true);
			if (NULL != xianmengzhan_logic)
			{
				xianmengzhan_logic->OnActivityStatusChange(as->status);
			}
		}
		break;

	case ACTIVITY_TYPE_YIZHANDAODI:
		{
			//for (int i = 0; i < ACTIVITY_ROOM_MAX; i++)
			{
				SpecialYiZhanDaoDi *yizhandaodi_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialYiZhanDaoDi(this->GetFBKeyByRoomIndex(0), true);
				if (NULL != yizhandaodi_logic)
				{
					yizhandaodi_logic->OnActivityStatusChange(as->status);
				}
			}
			if (ACTIVITY_STATUS_OPEN == as->status)
			{
				WorldStatus::Instance().ClearYiZhanDaoUserTitle();
			}
		}
		break;
	
	case ACTIVITY_TYPE_MOSHENJIANGLIN:
		{
			if (ACTIVITY_STATUS_OPEN == as->status)	
			{
				this->OnMoShenJiangLinOpen();
			}
			else if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnMoShenJiangLinClose();
			}
		}
		break;

	case ACTIVITY_TYPE_GUAJI:
		{
			if (ACTIVITY_STATUS_OPEN == as->status)	
			{
				this->OnGuaJiActivityOpen();
			}
			else if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnGuaJiActivityClose();
			}
		}
		break;

	case ACTIVITY_TYPE_WUXING_GUAJI:
		{
			for (int i = 0; i < ACTIVITY_ROOM_MAX; i++)
			{
				SpecialWuXingGuaJi *wuxingguaji_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialWuXingGuaJi(this->GetFBKeyByRoomIndex(i), true);
				if (NULL != wuxingguaji_logic)
				{
					wuxingguaji_logic->OnActivityStatusChange(as->status);
				}
			}
		}
		break;

	case ACTIVITY_TYPE_SHUIJING:
		{
			SpecialShuiJing *shuijing_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialShuiJing(this->GetFBKeyByRoomIndex(0), true);
			if (NULL != shuijing_logic)
			{
				shuijing_logic->OnActivityStatusChange(as->status);
			}
		}
		break;

	case ACTIVITY_TYPE_HUANGCHENGHUIZHAN:
		{
			HuangchenghuizhanMgr::Instance().OnActivityStatusChange(as->status);
		}
		break;

	case ACTIVITY_TYPE_CAMP_DEFEND1:
	case ACTIVITY_TYPE_CAMP_DEFEND2:
	case ACTIVITY_TYPE_CAMP_DEFEND3:
		{
			CampDefendMgr::Instance().OnActivityStatusChange(as->status, as->activity_type);
		}
		break;

	case ACTIVITY_TYPE_GUILDBATTLE:
		{
			SpecialGuildBattleNew * guildbattle_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialGuildBattle(this->GetFBKeyByRoomIndex(0), true);	
			if (NULL != guildbattle_logic)
			{
				guildbattle_logic->OnActivityStatusChange(as->status);
			}
		}
		break;

	case ACTIVITY_TYPE_TIANJIANGCAIBAO:
		{
			SpecialTianJiangCaiBao *tjcb_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialTianJiangCaiBao(this->GetFBKeyByRoomIndex(0), true);
			if (NULL != tjcb_logic)
			{
				tjcb_logic->OnActivityStatusChange(as->status);
			}
		}
		break;

	case ACTIVITY_TYPE_MILLIONAIRE:
		{
			SpecialMillionaire *mallionaire_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialMillionaire(this->GetFBKeyByRoomIndex(0), true);
			if (NULL != mallionaire_logic)
			{
				mallionaire_logic->OnActivityStatusChange(as->status);

				if (ACTIVITY_STATUS_OPEN == as->status)
				{
					unsigned int now_sce = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					m_next_millionaire_refresh_box_time = now_sce;

					const OtherCfg *other_cfg = LOGIC_CONFIG->GetMillionaireConfig().GetOhterCfg();
					if (NULL != other_cfg)
					{
						m_next_millionaire_refresh_boss_time = now_sce + other_cfg->boss_first_refresh_time;
					}

					this->SendMillionaireBoxNoticeNextTime(NULL);
				}
				else if (ACTIVITY_STATUS_CLOSE == as->status)
				{
					WorldStatus::Instance().OnActivityFinish(ACTIVITY_FIND_TYPE_MILLIONAIRE);
					m_next_millionaire_refresh_box_time = 0;
					m_next_millionaire_refresh_boss_time = 0;
				}
			}
		}
		break;

	case ACTIVITY_TYPE_GUILD_BOSS:
		{
			if (ACTIVITY_STATUS_OPEN == as->status)
			{

			}
			else if (ACTIVITY_STATUS_CLOSE == as->status)
			{

			}
		}
		break;

	
	case ACTIVITY_TYPE_WANGLINGEXPLORE:
		{
			SpecialWangLingExplore *wanglingexplore_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialWangLingExplore(this->GetFBKeyByRoomIndex(0), true);	
			if (NULL != wanglingexplore_logic)
			{
				wanglingexplore_logic->OnActivityStatusChange(as->status);
			}
		}
		break;

	case ACTIVITY_TYPE_TERRITORY_WAR:
		{
			for (int i = 0; i < ACTIVITY_ROOM_MAX; i++)
			{
				SpecialTerritoryWar *territorywar_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialTerritoryWar(this->GetFBKeyByRoomIndex(i), true);
				if (NULL != territorywar_logic)
				{
					territorywar_logic->OnActivityStatusChange(as->status);
				}
			}

			if(ACTIVITY_STATUS_CLOSE == as->status)
			{
				WorldStatus::Instance().OnActivityFinish(ACTIVITY_FIND_TYPE_TERRITORYWAR);

				GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
				if (NULL != guild_rank)
				{
					guild_rank->ReInitGuildRank(GUILD_RANK_TYPE_TERRITORYWAR);
				}

				if (0 == WorldStatus::Instance().GetOpenServerTerritoryWarID() && RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_OPEN_SERVER_BATTLE))
				{
					GuildID first_guild_id = INVALID_GUILD_ID;
					RankManager::Instance().GetGuildRank()->GetRankTopGuild(GUILD_RANK_TYPE_TERRITORYWAR, 1, &first_guild_id);
					if (INVALID_GUILD_ID != first_guild_id)
					{
						Guild * guild = GuildManager::Instance().GetGuild(first_guild_id);
						if (NULL != guild)
						{
							int tuanzhang = guild->GetGuildMemberManager().GetTuanZhangUid();

							UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(tuanzhang);
							if (NULL != user_node)
							{
								WorldStatus::Instance().SetOpenServerTerritoryWarID(tuanzhang);
								MailContentParam contentparam; contentparam.Reset();

								const TerritroyWarOtherCfg& other_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetOtherCfg();
								for (int item_count = 0; item_count < other_cfg.huizhang_extra_reward_item_count && item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
								{
									if (other_cfg.huizhang_extra_reward[item_count].num > 0)
									{
										contentparam.AddItem(other_cfg.huizhang_extra_reward[item_count].item_id, other_cfg.huizhang_extra_reward[item_count].num, other_cfg.huizhang_extra_reward[item_count].is_bind);
										LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_TERRITORYWAR, user_node->uid, user_node->user_name, NULL, user_node->level, other_cfg.huizhang_extra_reward[item_count].item_id, "Close", NULL, other_cfg.huizhang_extra_reward[item_count].num, other_cfg.huizhang_extra_reward[item_count].is_bind, 0, 0);
									}
								}

								int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_territorywar_rankreward_open_server);

								if (length_1 > 0)
								{
									MailRoute::MailToUser(IntToUid(user_node->uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
								}
							}
						}
					}
				}
			}
		}
		break;

	case ACTIVITY_TYPE_GUILD_YUNBIAO:
	{
		// 仙盟运镖活动结束时清除镖车
		if (ACTIVITY_STATUS_CLOSE == as->status)
		{
			GuildShadow::Instance().OnForceGuildYunBiaoEnd();
		}
	}
	break;

	case ACTIVITY_TYPE_GUILD_FB:
	{
		if (ACTIVITY_STATUS_OPEN == as->status)
		{
			GuildRoute::Instance().OnGuildFbActivityOpen();
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnRaUpgradeMountRankClose();
			}
		}
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnRaUpgradeHaloRankClose();
			}
		}
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnRaUpgradeWingRankClose();
			}
		}
		break;

	//新增时装、神兵、法宝、足迹
	case RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK:
	{
		if (ACTIVITY_STATUS_CLOSE == as->status)
		{
			this->OnRaUpgradeShizhuangRankClose();
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK:
	{
		if (ACTIVITY_STATUS_CLOSE == as->status)
		{
			this->OnRaUpgradeShenbinRankClose();
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK:
	{
		if (ACTIVITY_STATUS_CLOSE == as->status)
		{
			this->OnRaUpgradeFabaoRankClose();
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK:
	{
		if (ACTIVITY_STATUS_CLOSE == as->status)
		{
			this->OnRaUpgradeFootprintRankClose();
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnRaUpgradeShengongRankClose();
			}
		}
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnRaUpgradeShenyiRankClose();
			}
		}
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK:
	{
		if (ACTIVITY_STATUS_CLOSE == as->status)
		{
			this->OnRaUpgradeQilinbiRankClose();
		}
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK:
	{
		if (ACTIVITY_STATUS_CLOSE == as->status)
		{
			this->OnRaUpgradeToushiRankClose();
		}
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK:
	{
		if (ACTIVITY_STATUS_CLOSE == as->status)
		{
			this->OnRaUpgradeYaoshiRankClose();
		}
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK:
	{
		if (ACTIVITY_STATUS_CLOSE == as->status)
		{
			this->OnRaUpgradeLingTongRankClose();
		}
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK:
	{
		if (ACTIVITY_STATUS_CLOSE == as->status)
		{
			this->OnRaUpgradeLingGongRankClose();
		}
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK:
	{
		if (ACTIVITY_STATUS_CLOSE == as->status)
		{
			this->OnRaUpgradeLingQiRankClose();
		}
	}
	break;

	case RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnRaEuqipStrenghtLevelRankClose();
			}
		}
		break;

	case RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnRaStoneTotalLevelRankClose();
			}
		}
		break;

	case RAND_ACTIVITY_TYPE_XIANNV_RANK:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnRAXianNvTotalRankClose();
			}
		}
		break;

	case RAND_ACTIVITY_TYPE_JINGLING_RANK:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnRAJingLingTotalRankClose();
			}
		}
		break;

	case RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnRAFightMountTotalRankClose();
			}
		}
		break;

	case RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnRATotalCapRankClose();
			}
		}
		break;
		
	case ACTIVITY_TYPE_XINGZUOYIJI:
		{
			SpecialXingzuoYiji *xingzuoyiji_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialXingzuoYiji(this->GetFBKeyByRoomIndex(0), true);	
			if (NULL != xingzuoyiji_logic)
			{
				xingzuoyiji_logic->OnActivityStatusChange(as->status);
			}
		}
		break;

		case ACTIVITY_TYPE_GUILD_QUESTION:
		{
			if (ACTIVITY_STATUS_STANDY == as->status)
			{
				this->OnGuildQuestionActivityStandy();
			}
			else if (ACTIVITY_STATUS_OPEN == as->status)
			{
				this->OnGuildQuestionActivityStart();
			}
			else if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnGuildQuestionActivityClose();
			}
		}
		break;

		case ACTIVITY_TYPE_MESS_BATTLE_FB:
		{
			if (ACTIVITY_STATUS_OPEN == as->status)
			{
				WorldStatus2::Instance().ClearMessBattleActiviyTopRank();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_IMAGE_COMPETITION:
		{
			if (ACTIVITY_STATUS_OPEN == as->status)
			{
				int rank_type = 0, rank_id = 0;
				LOGIC_CONFIG->GetRandActivityCfg().GetImageCompetitionRankInfo(&rank_type, &rank_id);

				if (rank_type > IMAGE_COMPETITION_TYPE_INVALID && rank_type < IMAGE_COMPETITION_TYPE_MAX)
				{
					RankManager::Instance().ClearPersonRankType(PERSON_RANK_TYPE_IMAGE_COMPETITION);
					WorldStatus2::Instance().SetImageCompetitionTypeAndId(rank_type, rank_id);
				}
				this->SendImageCompetitionInfo();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnRaUpgradeFlypetRankClose();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK:
		{
			if (ACTIVITY_STATUS_CLOSE == as->status)
			{
				this->OnRaUpgradeWeiyanRankClose();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_GIFT_HARVEST:
		{
			SpecialGiftHarvest * giftharvest_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialGiftHarvest(this->GetFBKeyByRoomIndex(0), true);
			if (NULL != giftharvest_logic)
			{
				giftharvest_logic->OnActivityStatusChange(as->status, as->data);
			}
		}
		break;
	}

	if (ACTIVITY_STATUS_CLOSE == as->status)
	{
		WorldStatus::Instance().OnRecordActivityClose(as->activity_type);
	}
}

void ActivityShadow::OnSyncCrossActivityStatus(int cross_type, crossgameprotocal::CrossGameSyncActivityStatus *cgsas)
{
	if (cgsas->cross_activity_type <= CROSS_ACTIVITY_TYPE_INVALID || cgsas->cross_activity_type >= CROSS_ACTIVITY_TYPE_RESERVED) 
	{
		return;
	}
	
	// 跨服战区类型不匹配
	if (!CrossConfig::Instance().CheckCrossServerHasCrossActivity(cgsas->cross_activity_type, cross_type))
	{
		return;
	}

	// 有些活动在某些条件下不需要同步活动状态
	if (!CheckSyncCrossActivityStatusBefore(cgsas->cross_activity_type, cgsas->activity_status))
	{
		return;
	}

	// 设置跨服活动的状态
	ActivityStatus *cross_activity_status = &m_cross_activity_status[cgsas->cross_activity_type - CROSS_ACTIVITY_TYPE_INVALID];
	ActivityStatus old_cross_activity_status = *cross_activity_status;

	unsigned int delta_time_second = cgsas->cross_now_scecond - static_cast<unsigned int>(EngineAdapter::Instance().Time());

	cross_activity_status->status = cgsas->activity_status;
	cross_activity_status->activity_begin_time = cgsas->status_begin_time - delta_time_second;
	cross_activity_status->activity_end_time = cgsas->status_end_time - delta_time_second;
	cross_activity_status->data = cgsas->activity_special_param;

	if (old_cross_activity_status.status != cross_activity_status->status ||
		old_cross_activity_status.data != cross_activity_status->data)
	{
		CrossRecordManager::Instance().OnCrossActivityStatusChange(cgsas->cross_activity_type, old_cross_activity_status.status, cross_activity_status->status);
		this->OnCrossActivityStatusChange(cgsas->cross_activity_type, old_cross_activity_status.status, cross_activity_status->status, cgsas->status_end_time);
	}

	this->SyncCrossActivityStatusToUser(cgsas->cross_activity_type);
}

bool ActivityShadow::IsActivtyOpen(int activity_type)
{
	if (activity_type <= ACTIVITY_TYPE_INVALID || activity_type >= ACTIVITY_TYPE_MAX) return false;

	return ACTIVITY_STATUS_OPEN == m_activity_status[activity_type].status;
}

bool ActivityShadow::IsActivtyStandy(int activity_type)
{
	if (activity_type <= ACTIVITY_TYPE_INVALID || activity_type >= ACTIVITY_TYPE_MAX) return false;

	return ACTIVITY_STATUS_STANDY == m_activity_status[activity_type].status;
}

bool ActivityShadow::IsActivtyClose(int activity_type)
{
	if (activity_type <= ACTIVITY_TYPE_INVALID || activity_type >= ACTIVITY_TYPE_MAX) return false;

	return ACTIVITY_STATUS_CLOSE == m_activity_status[activity_type].status;
}

bool ActivityShadow::IsCrossRandActivityOpen(int cross_rand_activity_type)
{
	if (cross_rand_activity_type < CROSS_RAND_ACTIVITY_TYPE_BEGIN || cross_rand_activity_type >= CROSS_RAND_ACTIVITY_TYPE_END)
	{
		return false;
	}

	return (ACTIVITY_STATUS_OPEN == m_cross_rand_activity_status[cross_rand_activity_type - CROSS_RAND_ACTIVITY_TYPE_BEGIN].status);
}

unsigned int ActivityShadow::GetActivityBeginTime(int activity_type)
{
	if (activity_type <= ACTIVITY_TYPE_INVALID || activity_type >= ACTIVITY_TYPE_MAX) return 0;

	return m_activity_status[activity_type].activity_begin_time;
}

unsigned int ActivityShadow::GetActivityEndTime(int activity_type)
{
	if (activity_type <= ACTIVITY_TYPE_INVALID || activity_type >= ACTIVITY_TYPE_MAX) return 0;

	return m_activity_status[activity_type].activity_end_time;
}

unsigned int ActivityShadow::GetRandActivityBeginTime(int activity_type)
{
	if (activity_type < RAND_ACTIVITY_TYPE_BEGIN || activity_type >= RAND_ACTIVITY_TYPE_END) return 0;

	return m_rand_activity_status[activity_type - RAND_ACTIVITY_TYPE_BEGIN].activity_begin_time;
}

unsigned int ActivityShadow::GetRandActivityEndTime(int activity_type)
{
	if (activity_type < RAND_ACTIVITY_TYPE_BEGIN || activity_type >= RAND_ACTIVITY_TYPE_END) return 0;

	return m_rand_activity_status[activity_type - RAND_ACTIVITY_TYPE_BEGIN].activity_end_time;
}

unsigned int ActivityShadow::GetSpecialActivityBeginTime(int special_activity_type)
{
	special_activity_type = special_activity_type - SPECIAL_ACTIVITY_TYPE_INVALID;
	if (special_activity_type <= 0 || special_activity_type >= SPECIAL_ACTIVITY_TYPE_MAX) return 0;

	return m_special_activity_status[special_activity_type].activity_begin_time;
}

unsigned int ActivityShadow::GetSpecialActivityEndTime(int special_activity_type)
{
	special_activity_type = special_activity_type - SPECIAL_ACTIVITY_TYPE_INVALID;
	if (special_activity_type <= 0 || special_activity_type >= SPECIAL_ACTIVITY_TYPE_MAX) return 0;

	return m_special_activity_status[special_activity_type].activity_end_time;
}

bool ActivityShadow::IsSpecialActivityOpen(int special_activity_type)
{
	special_activity_type = special_activity_type - SPECIAL_ACTIVITY_TYPE_INVALID;
	if (special_activity_type <= 0 || special_activity_type >= SPECIAL_ACTIVITY_TYPE_MAX) return false;

	return ACTIVITY_STATUS_OPEN == m_special_activity_status[special_activity_type].status;
}

bool ActivityShadow::IsInSpecialActivityOpenTime(int special_activity_type, unsigned int timestamp)
{
	special_activity_type = special_activity_type - SPECIAL_ACTIVITY_TYPE_INVALID;
	if (special_activity_type <= 0 || special_activity_type >= SPECIAL_ACTIVITY_TYPE_MAX) return false;

	if (m_special_activity_status[special_activity_type].activity_begin_time <= timestamp && 
		timestamp < m_special_activity_status[special_activity_type].activity_end_time)
	{
		return true;
	}

	return false;
}

bool ActivityShadow::IsInGameOpenActivityTime(unsigned int timestamp)
{
	
	return false;
}

bool ActivityShadow::IsInCombineActivityTime(unsigned int timestamp)
{
	
	return false;
}

bool ActivityShadow::IsInCurrentCrossRandActivityOpenTime(int cross_rand_activity_type, unsigned int timestamp)
{
	if (cross_rand_activity_type < CROSS_RAND_ACTIVITY_TYPE_BEGIN || cross_rand_activity_type >= CROSS_RAND_ACTIVITY_TYPE_END)
	{
		return false;
	}

	int act_index = cross_rand_activity_type - CROSS_RAND_ACTIVITY_TYPE_BEGIN;
	return (timestamp >= m_cross_rand_activity_status[act_index].activity_begin_time && timestamp <= m_cross_rand_activity_status[act_index].activity_end_time);
}

bool ActivityShadow::IsRandActivityOpen(int rand_activity_type)
{
	rand_activity_type = rand_activity_type - RAND_ACTIVITY_TYPE_BEGIN;
	if (rand_activity_type < 0 || rand_activity_type >= RAND_ACTIVITY_TYPE_MAX) return false;

	return ACTIVITY_STATUS_OPEN == m_rand_activity_status[rand_activity_type].status;
}

bool ActivityShadow::IsInRandActivityOpenTime(int rand_activity_type, unsigned int timestamp)
{
	rand_activity_type = rand_activity_type - RAND_ACTIVITY_TYPE_BEGIN;
	if (rand_activity_type < 0 || rand_activity_type >= RAND_ACTIVITY_TYPE_MAX) return false;

	if (m_rand_activity_status[rand_activity_type].activity_begin_time <= timestamp && timestamp < m_rand_activity_status[rand_activity_type].activity_end_time)
	{
		return true;
	}

	return false;
}

bool ActivityShadow::HadOpenActivityBefore(int rand_activity_type)
{
	rand_activity_type = rand_activity_type - RAND_ACTIVITY_TYPE_BEGIN;
	if (rand_activity_type < 0 || rand_activity_type >= RAND_ACTIVITY_TYPE_MAX) return false;

	if (m_rand_activity_status[rand_activity_type].activity_begin_time > 0 && m_rand_activity_status[rand_activity_type].activity_end_time > 0)
	{
		return true;
	}

	return false;
}

void ActivityShadow::Update(unsigned long interval)
{	
	time_t now = EngineAdapter::Instance().Time();

// 	if (this->IsActivtyOpen(ACTIVITY_TYPE_MONSTER_INVADE))
// 	{
// 		if (m_next_monster_invade_time > 0 && now >= m_next_monster_invade_time)
// 		{
// 			LOGIC_CONFIG->GetMonsterInvadeConfig().RefreshAll();
// 
// 			m_next_monster_invade_time = now + LOGIC_CONFIG->GetMonsterInvadeConfig().GetRefreshIntervalS();
// 			
	// 			this->SendMonsterInvadeNoticeNextTime(NULL);
	// 
	// 			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_monster_invade_refresh);
	// 			if (sendlen > 0) 
	// 			{
	// 				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER);
	// 			}
	// 		}
	// 	}

	if (this->IsActivtyOpen(ACTIVITY_TYPE_MILLIONAIRE))
	{
		if (m_next_millionaire_refresh_box_time > 0 && static_cast<unsigned int>(now) > m_next_millionaire_refresh_box_time)
		{
			LOGIC_CONFIG->GetMillionaireConfig().RefreshAll();

			m_next_millionaire_refresh_box_time = static_cast<unsigned int>(now) + LOGIC_CONFIG->GetMillionaireConfig().GetRefreshIntervalS();

			this->SendMillionaireBoxNoticeNextTime(NULL);

// 			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_millionaire_box_refresh);
// 			if (sendlen > 0) 
// 			{
// 				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE);
// 			}
		}

		if (m_next_millionaire_refresh_boss_time > 0 && static_cast<unsigned int>(now) > m_next_millionaire_refresh_boss_time)
		{
			const OtherCfg *other_cfg = LOGIC_CONFIG->GetMillionaireConfig().GetOhterCfg();
			if (NULL != other_cfg && other_cfg->exp_boss_id > 0)
			{
				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg->scene_id, 1);
				if (NULL != scene)
				{
					MONSTERPOOL->CreateMonster(other_cfg->exp_boss_id, scene, other_cfg->boss_born_pos);
					m_next_millionaire_refresh_boss_time = 0;
					this->SendMillionaireBoxNoticeNextTime(NULL);

					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_millionaire_boss_refresh,
						other_cfg->exp_boss_id);
					if (length > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
					}
				}
			}
		}
	}

	if (this->IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_1V1))
	{
		unsigned int now = (unsigned int)(EngineAdapter::Instance().Time());

		if (m_cross_1v1_fight_start_time > 0 && now >= m_cross_1v1_fight_start_time + 3)
		{
			// 一轮战斗开始，就清掉匹配信息，不让再进了
//			m_cross_1v1_match_map.clear();
			m_cross_1v1_fight_start_time = 0;
		}
	}

	if (this->IsActivtyOpen(ACTIVITY_TYPE_GUILD_QUESTION))
	{
		this->OnGuildQuestionActivityUpdate();
	}

	// 猎鲲副本同步区域人数回原服
	if (this->IsCrossActivityStandby(CROSS_ACTIVITY_TYPE_LIEKUN_FB)||this->IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_LIEKUN_FB))
	{
		if (!CrossConfig::Instance().IsHiddenServer())
		{
			return;
		}

		crossgameprotocal::GameCrossLieKunFBInfoSync gclkfbis;

		for (int i = 0; i < LIEKUN_ZONE_TYPE_COUNT; ++i)
		{
			const CrossLieKunFbZoneCfg *zone_cfg = CrossLieKunFbConfig::Instance().GetCrossLieKunFbZoneCfg(i);
			if (NULL == zone_cfg)
			{
				continue;
			}

			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(zone_cfg->scene_id, CROSS_PUBLIC_SCENE_KEY);
			if (NULL != scene && Scene::SCENE_TYPE_CROSS_LIEKUN_FB == scene->GetSceneType())
			{
				SpecialCrossLieKunFb *special_lkfb = dynamic_cast<SpecialCrossLieKunFb*>(scene->GetSpecialLogic());
				if (nullptr != special_lkfb)
					gclkfbis.scene_list[i].role_num = special_lkfb->GetSceneInfo();
			}
		}
		InternalComm::Instance().SendToCross((const char *)&gclkfbis, sizeof(gclkfbis), CROSS_ACTIVITY_TYPE_LIEKUN_FB);
	}

	// 神州六器，每日刷采集物
	this->ShenzhouWeaponCalcNextFleshGatherTime(now);
		
	this->CheckGuaJiActivityReward(now);

	// 检查跨服BOSS刷新提醒
	this->CheckCrossBossRefreshNotice(now);
}
void ActivityShadow::OnDayChange(unsigned int old_dayid, unsigned int now_day_id)
{
	if (old_dayid != now_day_id)
	{
		m_xiuluotower_server_daily_drop_title_num = 0;
		m_xiuluotower_server_daily_drop_mount_num = 0;
		m_xiuluotower_server_daily_gold_box_num = 0;
		m_guild_battle_daily_drop_wing_num = 0;

		m_collect_treasure_record_deq.clear();      // 随机活动 -- 聚宝盆（新）

		this->OnImageCompetitionAccount();         // 形象比拼每日结算
		this->SendImageCompetitionInfo();
		m_cross_guildbattle_map.clear();
		m_guildbattle_user_rank_map.clear();
	}
}

void ActivityShadow::OnRolePickBigCoin(Role *role, BigCoinObj *bigcoin)
{
	if (NULL == role || NULL == bigcoin) return;
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SpecialZhuXieUser * ActivityShadow::GetZhuXieUser(const UserID &user_id, int world_level)
{
	SpecialZhuXieUserMapIt it = m_zhuxie_usermap.find(user_id);
	if (it == m_zhuxie_usermap.end()) // 不存在 插入新的用户
	{
		SpecialZhuXieUser zhuxie_user;
		zhuxie_user.user_id = user_id;
		
		ZhuXieTaskInfo zhuxie_task_list[ZHUXIE_TASK_MAX];
		LOGIC_CONFIG->GetZhuXieConfig().GetZhuXieTaskList(zhuxie_task_list, world_level);
		for (int i = 0; i < ZHUXIE_TASK_MAX; i++)
		{
			if (zhuxie_task_list[i].task_id > 0)
			{
				zhuxie_user.zhuxietask_list.push_back(zhuxie_task_list[i]);
			}
		}

		m_zhuxie_usermap[user_id] = zhuxie_user;
	}

	if (it == m_zhuxie_usermap.end()) it = m_zhuxie_usermap.find(user_id); 

	if (it != m_zhuxie_usermap.end())
	{
		return &it->second;
	}

	return NULL;
}

void ActivityShadow::OnUserLogin(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (this->IsActivtyOpen(ACTIVITY_TYPE_MONSTER_INVADE))
	{
		this->SendMonsterInvadeNoticeNextTime(role);
	}

	if (this->IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		this->SendCSASubActivityState(role);
	}

	if (this->IsActivtyOpen(ACTIVITY_TYPE_MILLIONAIRE))
	{
		this->SendMillionaireBoxNoticeNextTime(role);
	}

	for (int cross_activity_type = CROSS_ACTIVITY_TYPE_INVALID + 1; cross_activity_type < CROSS_ACTIVITY_TYPE_RESERVED; ++cross_activity_type)
	{
		ActivityShadow::Instance().SyncCrossActivityStatusToUser(cross_activity_type, role);
	}

	this->SendImageCompetitionInfo(role);
}

void ActivityShadow::SendMonsterInvadeNoticeNextTime(Role *role)
{
	Protocol::SCMonsterInvadeTimeNotice scmin;
	scmin.next_monster_invade_time = static_cast<unsigned int>(m_next_monster_invade_time);

	if (NULL == role)
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&scmin, sizeof(scmin));
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&scmin, sizeof(scmin));
	}
}

void ActivityShadow::SendMillionaireBoxNoticeNextTime(Role *role)
{
	Protocol::SCMillionaireTimeNotice scmin;
	scmin.next_millionaire_box_refresh_time = m_next_millionaire_refresh_box_time;
	scmin.next_millionaire_boss_refresh_time = m_next_millionaire_refresh_boss_time;

	if (NULL == role)
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&scmin, sizeof(scmin));
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&scmin, sizeof(scmin));
	}
}

void ActivityShadow::OnGather(Role *role, int gather_id, ObjID obj_id)
{
	if (this->IsActivtyOpen(ACTIVITY_TYPE_GUILD_QUESTION))
	{
		long long add_exp = 0;
		const GuildQuestionOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg();
		if (Scene::SCENE_TYPE_GUILD_QUESTION == role->GetScene()->GetSceneType() && gather_id == other_cfg.gather_id)
		{
			GuildQuestionPlayerInfoMapIt it = m_guild_question_player_info_map.find(role->GetUserId());
			if (it != m_guild_question_player_info_map.end())
			{
				if (it->second.gather_count < other_cfg.gather_count_limit)
				{
					//策划需求，修改增加经验规则（2018-1-3）
					add_exp = static_cast<long long>(LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg().exp_factor_gather * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));
					it->second.exp += add_exp;
					it->second.gather_count ++;
					this->SendGuildQuestionPlayerInfo(role);
					if (other_cfg.gather_reward_item.item_id > 0)
					{
						role->GetKnapsack()->PutOrMail(other_cfg.gather_reward_item, PUT_REASON_GUILD_QUESTION_GATHER);
					}
				}
				//策划需求，修改增加经验规则（2018-1-3）
				role->AddExp(add_exp, "OnGuildQuestionActivityGather", Role::EXP_ADD_REASON_DEFAULT);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ActivityShadow::OnZhuxieClose()
{
	for (SpecialZhuXieUserMapIt it = m_zhuxie_usermap.begin(); it != m_zhuxie_usermap.end(); ++it)
	{
		SpecialZhuXieUser *zhuxie_user = &(it->second);

		for (std::vector<ZhuXieTaskInfo>::iterator task_it = zhuxie_user->zhuxietask_list.begin(); task_it != zhuxie_user->zhuxietask_list.end(); ++task_it)
		{
			ZhuXieTaskInfo &taskinfo = *task_it;

			{
				// 诛邪任务日志
				if (taskinfo.IsFinish())
				{
					gamelog::g_log_roleactivity.printf(LL_INFO, "ActivityZhuxie::JoinRecord user[%d, %s], level[%d], complete_task_id[%d]",
						UidToInt(zhuxie_user->user_id), zhuxie_user->user_name, zhuxie_user->role_level, task_it->task_id);

					LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_ZHUXIE_TASK, UidToInt(zhuxie_user->user_id), zhuxie_user->user_name, NULL, zhuxie_user->role_level, task_it->task_id, NULL, NULL);
				}
			}

			if (!taskinfo.IsFinish() || taskinfo.IsFetchReward())
			{
				continue;
			}

// 			ZhuXieReward task_reward;
// 			if (!LOGIC_CONFIG->GetZhuXieConfig().GetZhuXieTaskReward(taskinfo.task_id, zhuxie_user->role_level, &task_reward))
// 			{
// 				continue;
// 			}
// 
// 			taskinfo.is_fetch_reward = 1;
// 
// 			static MailContentParam reward_mail; 
// 			reward_mail.Reset();
// 
// 			long long reward_exp = task_reward.exp;
// 			reward_exp += WorldShadow::Instance().GetWorLeveldExtraExp(it->second.role_level, reward_exp);
// 			if (reward_exp > INT_MAX)
// 			{
// 				reward_exp = INT_MAX;
// 			}
// 
// 			reward_mail.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_EXP] = static_cast<int>(reward_exp);
// 			reward_mail.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_BIND_COIN] = task_reward.bind_coin;
// 
// 			reward_mail.item_list[0].item_id = task_reward.reward_item.item_id;
// 			reward_mail.item_list[0].num = task_reward.reward_item.num;
// 			reward_mail.item_list[0].is_bind = (task_reward.reward_item.is_bind ? 1 : 0);
// 			const ItemBase *itembase = ITEMPOOL->GetItem(task_reward.reward_item.item_id);
// 			if (NULL != itembase) reward_mail.item_list[0].invalid_time = itembase->CalInvalidTime();
// 
// 			int l1 = SNPRINTF(reward_mail.subject, sizeof(reward_mail.subject), gamestring::g_zhuxie_task_reward_subject);
// 			int l2 = SNPRINTF(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), gamestring::g_zhuxie_task_reward_content);
// 
// 			if (l1 > 0 && l2 > 0)
// 			{
// 				MailRoute::MailToUser(zhuxie_user->user_id, SYSTEM_MAIL_REASON_INVALID, reward_mail);
// 			}
		}
	}
}

void ActivityShadow::OnRoleHurtMonster(Role *role, Monster *monster, Attribute hurt)
{
	if (NULL == role || NULL == monster || 0 == hurt)
	{
		return;
	}

	if (!this->IsActivtyOpen(ACTIVITY_TYPE_MOSHENJIANGLIN))
	{
		return;
	}

	hurt = abs(hurt);

	const tm *local_tm = EngineAdapter::Instance().LocalTime();
	if (NULL != local_tm)
	{
		MoShenJiangLinBossCfg *boss_cfg = LOGIC_CONFIG->GetMoShenJiangLinConfig().GetMoShenJiangLinBossCfg(local_tm->tm_wday);
		if (NULL != boss_cfg && m_moshenjianglin_boss_id == monster->GetMonsterId())
		{
			double add_reward = 0;
			if (boss_cfg->base_xianhun > 0)
			{
				add_reward = log((long double)hurt) / log((long double)8);
				add_reward = pow(add_reward, 0.5);
				add_reward = add_reward * role->GetLevel() / 2.4;
				role->AddXianhun(static_cast<int>(add_reward),"MoshenJianglin");
			}

			if (boss_cfg->base_yuanli > 0)
			{
				add_reward = log((long double)hurt) / log((long double)25);
				add_reward = pow(add_reward, 0.5);
				add_reward = add_reward * role->GetLevel() / 0.8;
				role->AddYuanli(static_cast<int>(add_reward),"MoshenJianglin");
			}

			if (boss_cfg->base_tongbi > 0)
			{
				add_reward = log((long double)hurt) / log((long double)7);
				add_reward = pow(add_reward, 2);
				add_reward = add_reward * role->GetLevel() / 2.1;
				role->GetKnapsack()->GetMoney()->AddCoinBind(static_cast<int>(add_reward), "MoshenJianglin");
			}
		}
	}
}

void ActivityShadow::OnKillMonster(Role *role, Monster *monster)
{
	if (NULL == role || NULL == monster)
	{
		return;
	}

	if (!this->IsActivtyOpen(ACTIVITY_TYPE_MOSHENJIANGLIN))
	{
		return;
	}

	if (m_moshenjianglin_boss_id == monster->GetMonsterId())
	{
		ActivityManager::Instance().OnForceToNextState(ACTIVITY_TYPE_MOSHENJIANGLIN);
	}
}

void ActivityShadow::OnMoShenJiangLinOpen()
{
	const tm *local_tm = EngineAdapter::Instance().LocalTime();
	if (NULL != local_tm)
	{
		MoShenJiangLinBossCfg *boss_cfg = LOGIC_CONFIG->GetMoShenJiangLinConfig().GetMoShenJiangLinBossCfg(local_tm->tm_wday);
		if (NULL != boss_cfg)
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(boss_cfg->scene_id, 0);
			if (NULL != scene) 
			{
				int boss_id = boss_cfg->GetBossIDByWorldLv(WorldShadow::Instance().GetWorldLevel());

				Monster *monster = MONSTERPOOL->CreateMonster(boss_id, scene, boss_cfg->pos);
				if (NULL != monster)
				{
					monster->GetEffectList()->AddSpecialBuff(EBT_MIANYI_XUANYUN);
					monster->GetEffectList()->AddSpecialBuff(EBT_MIANYI_DINGSHEN);
					monster->GetEffectList()->AddSpecialBuff(EBT_MIANYI_CHIHUAN);

					m_moshenjianglin_boss_id = boss_id;
				}
			}
		}
	}
}

void ActivityShadow::OnMoShenJiangLinClose()
{
	m_moshenjianglin_boss_id = 0;
}

void ActivityShadow::OnRaUpgradeMountRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_MOUNT, RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK);
}

//新增时装、神兵、法宝、足迹
void ActivityShadow::OnRaUpgradeShizhuangRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_SHIZHUANG, RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK);
}

void ActivityShadow::OnRaUpgradeShenbinRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_SHIZHUANG_WUQI, RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK);
}

void ActivityShadow::OnRaUpgradeFabaoRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_FABAO, RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK);
}

void ActivityShadow::OnRaUpgradeFootprintRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_FOOTPRINT, RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK);
}

void ActivityShadow::OnRaUpgradeFlypetRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_FEICHONG,RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK);
}

void ActivityShadow::OnRaUpgradeWeiyanRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_WEIYAN, RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK);
}

void ActivityShadow::OnRaUpgradeHaloRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_HALO, RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK);
}

void ActivityShadow::OnRaUpgradeWingRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_WING, RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK);
}

void ActivityShadow::OnRaUpgradeShengongRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_SHENGONG, RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK);
}

void ActivityShadow::OnRaUpgradeShenyiRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_SHENYI, RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK);
}

void ActivityShadow::OnRaUpgradeQilinbiRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_QILINBI, RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK);
}

void ActivityShadow::OnRaUpgradeToushiRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_TOUSHI, RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK);
}

void ActivityShadow::OnRaUpgradeYaoshiRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_YAOSHI, RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK);
}

void ActivityShadow::OnRaUpgradeLingTongRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_LINGTONG, RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK);
}

void ActivityShadow::OnRaUpgradeLingGongRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_LINGGONG, RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK);
}

void ActivityShadow::OnRaUpgradeLingQiRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_LINGQI, RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK);
}

void ActivityShadow::OnRaEuqipStrenghtLevelRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_EQUIP_STRENGTH_LEVEL, RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK);
}

void ActivityShadow::OnRaStoneTotalLevelRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_STONE_TOATAL_LEVEL, RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK);
}

void ActivityShadow::OnRAXianNvTotalRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_XIANNV_CAPABILITY, RAND_ACTIVITY_TYPE_XIANNV_RANK);
}

void ActivityShadow::OnRAJingLingTotalRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_CAPABILITY_JINGLING, RAND_ACTIVITY_TYPE_JINGLING_RANK);
}

void ActivityShadow::OnRAFightMountTotalRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_FIGHT_MOUNT, RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK);
}

void ActivityShadow::OnRATotalCapRankClose()
{
	this->OnRaUpgradeRankClose(PERSON_RANK_TYPE_CAPABILITY_ALL, RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK);
}

int ActivityShadow::OnRaUpgradeRankFiltrate(int ra_type, int rank_type, int count, PersonRankItem *item_list)
{
	PersonRankItem user_item_list[UPGRADE_RANK_GET_MAX_ROLE_NUM];
	int rank_count = RankManager::Instance().GetPersonRank()->GetRankTopItem(rank_type, UPGRADE_RANK_GET_MAX_ROLE_NUM, user_item_list);
	int limit_grade = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerMaxCon3(ra_type, true);    //比拼排行榜限制上榜阶数
	int real_count = 0;
	if (limit_grade < 0) return real_count;
	for (int i = 0; i < count && i < rank_count && real_count < UPGRADE_RANK_GET_MAX_ROLE_NUM && i < UPGRADE_RANK_GET_MAX_ROLE_NUM; i++)
	{
		if (user_item_list[i].rank_value > limit_grade)
		{
			item_list[real_count] = user_item_list[i];
			real_count++;
		}
	}

	return real_count;
}

void ActivityShadow::OnRaUpgradeRankClose(int rank_type, int ra_type)
{
	PersonRankItem user_item_list[UPGRADE_RANK_GET_MAX_ROLE_NUM];
	int rank_count = 0; 

	if (ra_type == RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK
		|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK
		|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK
		|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK
		|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK || ra_type == RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK
		|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK)
	{
		rank_count = this->OnRaUpgradeRankFiltrate(ra_type, rank_type, UPGRADE_RANK_GET_MAX_ROLE_NUM, user_item_list);
	}
	else
	{
		rank_count = RankManager::Instance().GetPersonRank()->GetRankTopItem(rank_type, UPGRADE_RANK_GET_MAX_ROLE_NUM, user_item_list);
	}

	for (int i = 0; i < rank_count && i < UPGRADE_RANK_GET_MAX_ROLE_NUM; i++)
	{
		int seq = 0;
		int reward_item_count = 0;
		const ItemConfigData *reward_item_list = NULL;
		int cond = 100;

		const RandActivityOpenServerRewardItemConfig *cfg = NULL;
		for (int k = 1; k < RAND_ACTIVITY_OPEN_SERVER_REWARD_COUNT_MAX; k++)
		{
			if (ra_type == RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK
				|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK 
				|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK
				|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK
				|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK || ra_type == RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK
				|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK)
			{
				cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(ra_type, k, true);	//全民冲榜的奖励均为昨天的奖励
			}
			else
			{
				cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(ra_type, k);
			}
			
			if (NULL == cfg || cfg->cond2 != 0) break;

			if (i < cfg->cond1 && cond > cfg->cond1)
			{
				if (((RAND_ACTIVITY_TYPE_JINGLING_RANK == ra_type || RAND_ACTIVITY_TYPE_XIANNV_RANK == ra_type) &&
					user_item_list[i].rank_value >= cfg->cond3) || user_item_list[i].flexible_int >= cfg->cond3)
				{
					seq = cfg->seq;
					reward_item_count = cfg->reward_item_count;
					reward_item_list = cfg->reward_item_list;
					cond = cfg->cond1;
				}
			}
		}

		if (NULL != reward_item_list)
		{
			MailContentParam contentparam;
			for (int k = 0; k < reward_item_count && k < MAX_ATTACHMENT_ITEM_NUM; k++)
			{
				const ItemBase *item_base = ITEMPOOL->GetItem(reward_item_list[k].item_id);
				if (NULL != item_base)
				{
					contentparam.item_list[contentparam.item_count].item_id = reward_item_list[k].item_id;
					contentparam.item_list[contentparam.item_count].num = reward_item_list[k].num;
					contentparam.item_list[contentparam.item_count].is_bind = reward_item_list[k].is_bind;
					contentparam.item_list[contentparam.item_count].invalid_time = item_base->CalInvalidTime();
					contentparam.item_count++;
				}
			}

			int length = 0;
			switch (ra_type)
			{
			case RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_mount_rank_content, i + 1);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_halo_rank_content, i + 1);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_wing_rank_content, i + 1);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_shengong_rank_content, i + 1);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_shenyi_rank_content, i + 1);
				break;

			case RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_equip_strength_level_rank_content, i + 1);
				break;

			case RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_stone_total_level_rank_content, i + 1);
				break;

			case RAND_ACTIVITY_TYPE_XIANNV_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_xiannv_cap_rank_content, i + 1);
				break;

			case RAND_ACTIVITY_TYPE_JINGLING_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_jingling_cap_rank_content, i + 1);
				break;

			case RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_fight_mount_rank_content, i + 1);
				break;

			case RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_total_cap_rank_content, i + 1);
				break;

			//新增时装、神兵、法宝、足迹
			case RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_shizhuang_rank_content, i + 1);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_shenbing_rank_content, i + 1);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_fabao_rank_content, i + 1);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_footprint_rank_content, i + 1);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_qilinbi_rank_content, i + 1);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_toushi_rank_content, i + 1);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_yaoshi_rank_content, i + 1);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_lingtong_rank_content, i + 1);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_linggong_rank_content, i + 1);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_lingqi_rank_content, i + 1);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_flypet_rank_content, i + 1);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_weiyan_rank_content, i + 1);
				break;
			}


			if (length > 0)
			{
				MailRoute::MailToUser(user_item_list[i].user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
			}

			gamelog::g_log_roleactivity.printf(LL_INFO, "OnRaUpgradeRankClose user[%d], ra_type[%d], rank[%d] seq[%d]",
				UidToInt(user_item_list[i].user_id), ra_type, i, seq);
		}
	}
}

bool ActivityShadow::IsGuaJiActivityExtraReward(Role *role, int *extra_reward_times)
{
	if (NULL == role || NULL == extra_reward_times)
	{
		return false;
	}

	if (this->IsActivtyOpen(ACTIVITY_TYPE_GUAJI)) // 如果挂机活动开启了，并且角色在挂机场景
	{
		if (NULL != role->GetScene() && LOGIC_CONFIG->GetGuajiActivityConfig().IsGuaJiSceneID(role->GetScene()->GetSceneID()))
		{
			*extra_reward_times = LOGIC_CONFIG->GetGuajiActivityConfig().GetExtraTimes();

			return true;
		}
	}

	return false;
}

void ActivityShadow::OnCSASubActivitySyncState(gglobalprotocal::CombineServerActivitySyncState *csass)
{
	memcpy(m_csa_sub_activity_state_list, csass->sub_activity_state_list, sizeof(m_csa_sub_activity_state_list));
	this->SendCSASubActivityState(NULL);
}

bool ActivityShadow::IsCSASubActivityNoStart(int sub_type)
{
	if (sub_type <= CSA_SUB_TYPE_INVALID || sub_type >= CSA_SUB_TYPE_MAX) return false;

	return CSA_SUB_ACTIVITY_STATE_NO_START == m_csa_sub_activity_state_list[sub_type];
}

bool ActivityShadow::IsCSASubActivityOpen(int sub_type)
{
	if (sub_type <= CSA_SUB_TYPE_INVALID || sub_type >= CSA_SUB_TYPE_MAX) return false;

	return CSA_SUB_ACTIVITY_STATE_OPEN == m_csa_sub_activity_state_list[sub_type];
}

bool ActivityShadow::IsCSASubActivityFinish(int sub_type)
{
	if (sub_type <= CSA_SUB_TYPE_INVALID || sub_type >= CSA_SUB_TYPE_MAX) return false;

	return CSA_SUB_ACTIVITY_STATE_FINISH == m_csa_sub_activity_state_list[sub_type];
}


bool ActivityShadow::IsCrossActivityStandby(int cross_activity_type)
{
	if (cross_activity_type <= CROSS_ACTIVITY_TYPE_INVALID || cross_activity_type >= CROSS_ACTIVITY_TYPE_RESERVED) 
	{
		return false;
	}

	return (ACTIVITY_STATUS_STANDY == m_cross_activity_status[cross_activity_type - CROSS_ACTIVITY_TYPE_INVALID].status);
}

bool ActivityShadow::IsCrossActivityOpen(int cross_activity_type)
{
	if (cross_activity_type <= CROSS_ACTIVITY_TYPE_INVALID || cross_activity_type >= CROSS_ACTIVITY_TYPE_RESERVED) 
	{
		return false;
	}

	return (ACTIVITY_STATUS_OPEN == m_cross_activity_status[cross_activity_type - CROSS_ACTIVITY_TYPE_INVALID].status);
}

bool ActivityShadow::IsCrossActivityClose(int cross_activity_type)
{
	if (cross_activity_type <= CROSS_ACTIVITY_TYPE_INVALID || cross_activity_type >= CROSS_ACTIVITY_TYPE_RESERVED) 
	{
		return false;
	}

	return (ACTIVITY_STATUS_CLOSE == m_cross_activity_status[cross_activity_type - CROSS_ACTIVITY_TYPE_INVALID].status);
}

unsigned int ActivityShadow::GetCrossActivityBeginTime(int cross_activity_type)
{
	if (cross_activity_type <= CROSS_ACTIVITY_TYPE_INVALID || cross_activity_type >= CROSS_ACTIVITY_TYPE_RESERVED) 
	{
		return false;
	}

	return m_cross_activity_status[cross_activity_type - CROSS_ACTIVITY_TYPE_INVALID].activity_begin_time;
}

unsigned int ActivityShadow::GetCrossActivityEndTime(int cross_activity_type)
{
	if (cross_activity_type <= CROSS_ACTIVITY_TYPE_INVALID || cross_activity_type >= CROSS_ACTIVITY_TYPE_RESERVED)
	{
		return false;
	}

	return m_cross_activity_status[cross_activity_type - CROSS_ACTIVITY_TYPE_INVALID].activity_end_time;
}

void ActivityShadow::OnCrossXiuluoTowerStatusChange(int from_status, int to_status)
{
	if (ACTIVITY_STATUS_STANDY == to_status)
	{
		m_cross_xiuluo_tower_user_map.clear();
	}
	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_cross_xiuluo_tower_rank_count = 0;
		m_cross_xiuluo_tower_rank_title_count = 0;
		memset(m_cross_xiuluo_tower_rank_list, 0, sizeof(m_cross_xiuluo_tower_rank_list));
		memset(m_cross_xiuluo_tower_rank_title_list, 0, sizeof(m_cross_xiuluo_tower_rank_title_list));
		m_cross_xiuluo_tower_max_layer = 0;
		WorldStatus2::Instance().ClearCrossXiuluoTowerRankTitle();
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		static crossgameprotocal::GameCrossSyncXiuluoTowerResult gcsxltr;
		gcsxltr.user_result_count = 0;

		for (CrossXiuluoTowerUserMapIt it = m_cross_xiuluo_tower_user_map.begin(); it != m_cross_xiuluo_tower_user_map.end(); it++)
		{
			if (gcsxltr.user_result_count >= crossgameprotocal::XIULUO_TOWER_USER_RESULT_MAX)
			{
				break;
			}

			gcsxltr.user_result_list[gcsxltr.user_result_count].uuid = it->second.uuid;
			gcsxltr.user_result_list[gcsxltr.user_result_count].max_layer = static_cast<char>(it->second.max_layer);
			gcsxltr.user_result_list[gcsxltr.user_result_count].rank_pos = it->second.rank_pos;
			gcsxltr.user_result_list[gcsxltr.user_result_count].kill_role_count = it->second.kill_role_count;
			gcsxltr.user_result_list[gcsxltr.user_result_count].kill_boss_count = it->second.kill_boss_count;
			gcsxltr.user_result_list[gcsxltr.user_result_count].user_level = it->second.level;
			gcsxltr.user_result_list[gcsxltr.user_result_count].reward_cross_honor = it->second.kill_reward_cross_honor + it->second.score_reward_cross_honor;

			++gcsxltr.user_result_count;
		}

		int senlen = sizeof(gcsxltr) - (crossgameprotocal::XIULUO_TOWER_USER_RESULT_MAX - gcsxltr.user_result_count) * sizeof(gcsxltr.user_result_list[0]);
		InternalComm::Instance().SendToCross((const char*)&gcsxltr, senlen, CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER);

		if (m_cross_xiuluo_tower_rank_title_count == 1)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "xiuluo_tower_rank_title1", m_cross_xiuluo_tower_rank_title_list[0].user_name);
			if (lenth > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
			}
		}
		else if (m_cross_xiuluo_tower_rank_title_count == 2)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "xiuluo_tower_rank_title2", m_cross_xiuluo_tower_rank_title_list[0].user_name,
				m_cross_xiuluo_tower_rank_title_list[1].user_name);
			if (lenth > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
			}
		}
		else if (m_cross_xiuluo_tower_rank_title_count >= 1)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "xiuluo_tower_rank_title3", m_cross_xiuluo_tower_rank_title_list[0].user_name,
				m_cross_xiuluo_tower_rank_title_list[1].user_name , m_cross_xiuluo_tower_rank_title_list[2].user_name);
			if (lenth > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
			}
		}

	}

	for (int i = 0; i < CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER; i++)
	{
		int scene_id = CrossXiuluoTowerCommonConfig::Instance().GetLayerSceneId(i);

		SpecialCrossXiuluoTower *xiuluo_tower = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossXiuluoTower(scene_id, CROSS_ACTIVITY_XIULUO_TOWER_SCENE_KEY, true);
		if (NULL != xiuluo_tower)
		{
			xiuluo_tower->OnActivityStatusChange(to_status);
		}
	}

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		if (CrossConfig::Instance().IsHiddenServer())
		{
			for (int i = 0; i < m_cross_xiuluo_tower_rank_title_count && i < CROSS_ACTIVITY_XIULUO_TOWER_RANK_TITLE_NUM; ++i)
			{
				crossgameprotocal::GameCrossSyncXiuluoTowerRankTitle gcrt;
				gcrt.rank = i + 1;
				gcrt.uuid = m_cross_xiuluo_tower_rank_title_list[i].uuid;
				InternalComm::Instance().SendToCross((const char *)&gcrt, sizeof(gcrt), CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER);
			}
		}
		m_cross_xiuluo_tower_user_map.clear();
		m_cross_xiuluo_tower_rank_count = 0;
		m_cross_xiuluo_tower_rank_title_count = 0;
		memset(m_cross_xiuluo_tower_rank_list, 0, sizeof(m_cross_xiuluo_tower_rank_list));
		memset(m_cross_xiuluo_tower_rank_title_list, 0, sizeof(m_cross_xiuluo_tower_rank_title_list));
		m_cross_xiuluo_tower_max_layer = 0;
	}
}

CrossXiuluoTowerUser * ActivityShadow::GetCrossXiuluoTowerUser(long long uuid)
{
	CrossXiuluoTowerUserMapIt it = m_cross_xiuluo_tower_user_map.find(uuid);
	if (it == m_cross_xiuluo_tower_user_map.end())
	{
		CrossXiuluoTowerUser user;
		user.uuid = uuid;
		m_cross_xiuluo_tower_user_map[uuid] = user;
	}

	if (it == m_cross_xiuluo_tower_user_map.end())
	{
		it = m_cross_xiuluo_tower_user_map.find(uuid);
	}

	if (it != m_cross_xiuluo_tower_user_map.end())
	{
		return &it->second;
	}

	return NULL;
}

CrossXiuluoTowerUser * ActivityShadow::RandGetCrossXiuluoTowerUser(long long uuid)
{
	if (m_cross_xiuluo_tower_user_map.size() <= 0) return NULL;

	CrossXiuluoTowerUserMapIt it = m_cross_xiuluo_tower_user_map.begin();
	int advance = RandomNum(static_cast<int>(m_cross_xiuluo_tower_user_map.size()));
	std::advance(it, advance);

	if (it->first == uuid)
	{
		// 刚好随机到要排除的
		if (it != m_cross_xiuluo_tower_user_map.begin())
		{
			-- it;
			return &it->second;
		}
		else if (++ it != m_cross_xiuluo_tower_user_map.end())
		{
			return &it->second;
		}

		return NULL;
	}
	
	return &it->second;
}

void ActivityShadow::CheckCrossXiuluoTowerRank(long long uuid)
{
	CrossXiuluoTowerUser *user = this->GetCrossXiuluoTowerUser(uuid);
	if (NULL == user) return;

	if (m_cross_xiuluo_tower_rank_count >= CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM)
	{
		m_cross_xiuluo_tower_rank_count = CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM;

		if (user->max_layer < m_cross_xiuluo_tower_rank_list[m_cross_xiuluo_tower_rank_count - 1].max_layer)
		{
			return;
		}
	}

	int temp_index = 0;
	for (temp_index = 0; temp_index < m_cross_xiuluo_tower_rank_count && temp_index < CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM; temp_index ++)
	{
		if (m_cross_xiuluo_tower_rank_list[temp_index].uuid == user->uuid)
		{
			if (m_cross_xiuluo_tower_rank_list[temp_index].max_layer == user->max_layer)
			{
				return;
			}

			break;
		}
	}

	if (temp_index >= m_cross_xiuluo_tower_rank_count)
	{
		if (m_cross_xiuluo_tower_rank_count < CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM) m_cross_xiuluo_tower_rank_count ++;
		temp_index = m_cross_xiuluo_tower_rank_count - 1;
	}

	for (int i = 0; i < m_cross_xiuluo_tower_rank_count && i < CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM; i++)
	{
		if (user->max_layer > m_cross_xiuluo_tower_rank_list[i].max_layer || m_cross_xiuluo_tower_rank_list[i].uuid <= 0)
		{
			for (int j = temp_index; j >= i + 1; j--)
			{
				m_cross_xiuluo_tower_rank_list[j] = m_cross_xiuluo_tower_rank_list[j - 1];
			}

			m_cross_xiuluo_tower_rank_list[i].uuid = uuid;
			m_cross_xiuluo_tower_rank_list[i].max_layer = user->max_layer;
			m_cross_xiuluo_tower_rank_list[i].finish_time = user->finish_time;
			m_cross_xiuluo_tower_rank_list[i].prof = user->prof;
			m_cross_xiuluo_tower_rank_list[i].camp = user->camp;
			F_STRNCPY(m_cross_xiuluo_tower_rank_list[i].user_name, user->user_name, sizeof(GameName));

			if (user->max_layer >= CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER - 1)
			{
				user->rank_pos = i + 1;
			}

			for (int i = 0; i < CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER; i++)
			{
				int scene_id = CrossXiuluoTowerCommonConfig::Instance().GetLayerSceneId(i);

				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, CROSS_ACTIVITY_XIULUO_TOWER_SCENE_KEY);
				if (NULL != scene && Scene::SCENE_TYPE_CROSS_XIULUO_TOWER == scene->GetSceneType())
				{
					SpecialCrossXiuluoTower *xiuluo_tower = dynamic_cast<SpecialCrossXiuluoTower*>(scene->GetSpecialLogic());
					if (nullptr != xiuluo_tower)
						xiuluo_tower->RefreshXiuluoTowerRankInfo();
				}
			}

			break;
		}
	}
}

void ActivityShadow::SetCrossXiuluoTowerRankTitle(long long uuid)
{
	if (m_cross_xiuluo_tower_rank_title_count >= CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM)
	{
		return;
	}
	CrossXiuluoTowerUser *user = this->GetCrossXiuluoTowerUser(uuid);
	if (NULL == user) return;

	for (int i = 0; i < m_cross_xiuluo_tower_rank_title_count && i < CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM; ++i)
	{
		if (m_cross_xiuluo_tower_rank_title_list[i].uuid == uuid)
		{
			return;
		}
	}
	m_cross_xiuluo_tower_rank_title_list[m_cross_xiuluo_tower_rank_title_count].uuid = uuid;
	m_cross_xiuluo_tower_rank_title_list[m_cross_xiuluo_tower_rank_title_count].finish_time = user->finish_time;
	F_STRNCPY(m_cross_xiuluo_tower_rank_title_list[m_cross_xiuluo_tower_rank_title_count].user_name, user->user_name, sizeof(GameName));
	m_cross_xiuluo_tower_rank_title_count++;

	for (int i = 0; i < CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER; i++)
	{
		int scene_id = CrossXiuluoTowerCommonConfig::Instance().GetLayerSceneId(i);

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, CROSS_ACTIVITY_XIULUO_TOWER_SCENE_KEY);
		if (NULL != scene && Scene::SCENE_TYPE_CROSS_XIULUO_TOWER == scene->GetSceneType() && NULL != scene->GetSpecialLogic())
		{
			SpecialCrossXiuluoTower *xiuluo_tower = dynamic_cast<SpecialCrossXiuluoTower*>(scene->GetSpecialLogic());
			if (nullptr != xiuluo_tower)
				xiuluo_tower->SendRankTitleList();
		}
	}
}

int ActivityShadow::GetCrossXiuluoTowerRankList(Protocol::SCCrossXiuluoTowerRankInfo::RankInfo rank_list[CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM])
{
	for (int i = 0; i < m_cross_xiuluo_tower_rank_count && i < CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM; i++)
	{
		rank_list[i].finish_time = static_cast<unsigned short>(m_cross_xiuluo_tower_rank_list[i].finish_time);
		rank_list[i].max_layer = static_cast<short>(m_cross_xiuluo_tower_rank_list[i].max_layer);
		F_STRNCPY(rank_list[i].user_name, m_cross_xiuluo_tower_rank_list[i].user_name, sizeof(GameName));
		rank_list[i].prof = m_cross_xiuluo_tower_rank_list[i].prof;
		rank_list[i].camp = m_cross_xiuluo_tower_rank_list[i].camp;
	}

	return m_cross_xiuluo_tower_rank_count;
}

int ActivityShadow::GetCrossXiuluoTowerRankTitleList(Protocol::SCCossXiuluoTowerRankTitleInfo::RankInfo titl_rank[CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM])
{
	for (int i = 0; i < m_cross_xiuluo_tower_rank_title_count && i < CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM; ++i)
	{
		titl_rank[i].uuid = m_cross_xiuluo_tower_rank_title_list[i].uuid;
		titl_rank[i].finish_time = m_cross_xiuluo_tower_rank_title_list[i].finish_time;
		F_STRNCPY(titl_rank[i].user_name, m_cross_xiuluo_tower_rank_title_list[i].user_name, sizeof(GameName));
	}
	return m_cross_xiuluo_tower_rank_title_count;
}

CrossActivity1v1FightUnit* ActivityShadow::GetCross1v1FightMatchUnit(long long uuid)
{
	std::map<long long, CrossActivity1v1FightUnit>::iterator iter = m_cross_1v1_match_map.find(uuid);
	if (iter == m_cross_1v1_match_map.end())
	{
		return NULL;
	}

	return &iter->second;
}

void ActivityShadow::ClearRoleMatchInfo(long long uuid)
{
	std::map<long long, CrossActivity1v1FightUnit>::iterator iter = m_cross_1v1_match_map.find(uuid);
	if (iter != m_cross_1v1_match_map.end())
	{
		m_cross_1v1_match_map.erase(iter);
	}
}

void ActivityShadow::OnCross1v1MatchInfo(crossgameprotocal::CrossGameSyncCross1v1MatchInfo *cgscmi)
{
	// 匹配信息战斗结束后清除
//	m_cross_1v1_match_map.clear();

	// 战斗开始时间只有匹配成功后才能赋值
	unsigned int now = (unsigned int)(EngineAdapter::Instance().Time());
	m_cross_1v1_fight_start_time = now + Cross1V1CommonConfig::Instance().GetFightWaitTime();
	m_cross_1v1_last_match_time = now;
	m_cross_1v1_next_match_time = now + cgscmi->m_next_match_left_time;

	for (int i = 0; i < cgscmi->unit_count && i < crossgameprotocal::CrossGameSyncCross1v1MatchInfo::MAX_FIGHT_UNIT_COUNT; ++i)
	{
		CrossActivity1v1FightUnit &unit = cgscmi->fight_unit_list[i];

		if (unit.uuid1 > 0 && unit.uuid2 > 0)
		{
			m_cross_1v1_match_map[unit.uuid1] = unit;
			m_cross_1v1_match_map[unit.uuid2] = unit;
		}
		else if (unit.uuid1 > 0 && unit.uuid2 == 0)
		{
			m_cross_1v1_match_map[unit.uuid1] = unit;
		}
	}
}

bool ActivityShadow::CrossActivityForceNextState(int activity_type)
{
	if (LOGIC_CONFIG->GetActivitySwitchConfig().IsBanActivity(activity_type)) return false;

	crossgameprotocal::GameCrossActivityForceNextState gcafns;
	gcafns.activity_type = activity_type;
	InternalComm::Instance().SendToCross((const char *)&gcafns, sizeof(gcafns), activity_type);
	return true;
}

void ActivityShadow::OnGuaJiActivityOpen()
{
	m_guaji_reward_next_timetamp = EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetGuajiActivityConfig().GetRewardInterval();
}

void ActivityShadow::OnGuaJiActivityClose()
{
	m_guaji_reward_next_timetamp = 0;
}

void ActivityShadow::CheckGuaJiActivityReward(time_t now_second)
{
	if (!this->IsActivtyOpen(ACTIVITY_TYPE_GUAJI))
	{
		return;
	}
	
	if (0 != m_guaji_reward_next_timetamp && now_second >= m_guaji_reward_next_timetamp)
	{
		m_guaji_reward_next_timetamp = EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetGuajiActivityConfig().GetRewardInterval();

		const int scene_count = LOGIC_CONFIG->GetGuajiActivityConfig().GetGuaJiSceneIdCount();
		
		for (int j = 0; j < scene_count; ++ j)
		{
			const int scene_id = LOGIC_CONFIG->GetGuajiActivityConfig().GetGuaJiSceneIdByIndex(j);

			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
			if (NULL != scene)
			{
				for (int i = 0; i < static_cast<int>(scene->RoleNum()); ++ i)
				{
					Role *role = scene->GetRoleByIndex(i);
					if (NULL != role)
					{
						int reward_xianhun = (role->GetLevel() * role->GetLevel() + 70 * role->GetLevel() + 10000) / 60;
						role->AddXianhun(reward_xianhun, "GuajiActivityReward");
					}
				}
			}
		}
	}
}

void ActivityShadow::SendCSASubActivityState(Role *role)
{
	static Protocol::SCCSASubActivityState csasas;
	memcpy(csasas.sub_activity_state_list, m_csa_sub_activity_state_list, sizeof(csasas.sub_activity_state_list));

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&csasas, sizeof(csasas));
	}
	else
	{
		World::GetInstWorld()->SendToAllGateway((const char *)&csasas, sizeof(csasas));
	}
}

void ActivityShadow::SyncCrossActivityStatusToUser(int cross_activity_type, Role *user)
{
	if (cross_activity_type <= CROSS_ACTIVITY_TYPE_INVALID || cross_activity_type >= CROSS_ACTIVITY_TYPE_RESERVED) 
	{
		return;
	}

	ActivityStatus *cross_activity_status = &m_cross_activity_status[cross_activity_type - CROSS_ACTIVITY_TYPE_INVALID];

	static Protocol::SCActivityStatus as;
	as.activity_type = static_cast<short>(cross_activity_type);
	as.status = static_cast<char>(cross_activity_status->status);
	as.is_broadcast = (NULL == user ? 1 : 0);
	as.next_status_switch_time = cross_activity_status->activity_end_time;

	if (NULL == user)
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&as, sizeof(as));
	}
	else
	{
		EngineAdapter::Instance().NetSend(user->GetNetId(), (const char*)&as, sizeof(as));
	}
}

bool ActivityShadow::CheckSyncCrossActivityStatusBefore(int cross_activity_type, int to_status)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return true;
	}

	switch (cross_activity_type)
	{
	case CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB:
	{
		// 夜战王城(怒战九霄)
		const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();
		const int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
		const int open_cross_day = other_cfg.open_cross_begin_day;

		// 本服开启活动中，不同步跨服的活动状态
		if (opengame_day < open_cross_day)
		{
			gamelog::g_log_activity.printf(LL_WARNING, "ActivityShadow::CheckSyncCrossActivityStatusBefore. Frobid sync crossactivity[%d] status[%d].", 
				cross_activity_type, to_status);

			return false;
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB:
	{
		//乱斗战场
		const MessBattleFbOtherConfig & other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();
		const int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
		const int open_cross_day = other_cfg.open_cross_begin_day;

		//本服开启活动中，不同步跨服的活动状态
		if (opengame_day < open_cross_day)
		{
			gamelog::g_log_activity.printf(LL_WARNING, "ActivityShadow::CheckSyncCrossActivityStatusBefore. Frobid sync crossactivity[%d] status[%d].",
				cross_activity_type, to_status);

			return false;
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI:
	{
		// 跨服边境之地
		const int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
		const int activity_open_day = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiOtherCfg().server_open_day;

		// 开服天数限制，不同步跨服的活动状态
		if (opengame_day < activity_open_day)
		{
			gamelog::g_log_activity.printf(LL_WARNING, "ActivityShadow::CheckSyncCrossActivityStatusBefore. Frobid sync crossactivity[%d] status[%d]",
				cross_activity_type, to_status);

			return false;
		}
	}
	break;

	default:
		break;
	}

	return true;
}

void ActivityShadow::OnCrossActivityStatusChange(int cross_activity_type, int from_status, int to_status, unsigned int end_time)
{
	if (ACTIVITY_STATUS_OPEN == to_status)  // 只在开启的时候清理，在活动结束时不清理，因为活动结束后还需要用到，自己在用完后调用DeleteCrossUserInfoOnBack接口
	{
		m_syncuserinfo_onback_map.clear();
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		WorldStatus::Instance().OnRecordActivityClose(cross_activity_type);
	}

	switch (cross_activity_type)
	{
	case CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER:
		{
			this->OnCrossXiuluoTowerStatusChange(from_status, to_status);
		}
		break;

	case CROSS_ACTIVITY_TYPE_CROSS_1V1:
		{
			if (ACTIVITY_STATUS_OPEN == to_status)
			{
				m_cross_1v1_match_map.clear();
				m_cross_1v1_last_match_time = 0;
				m_cross_1v1_fight_start_time = 0;
				m_cross_1v1_next_match_time = 0;
			}
			else if (ACTIVITY_STATUS_CLOSE == to_status)
			{
				m_cross_1v1_match_map.clear();
				m_cross_1v1_fight_start_time = 0;
				m_cross_1v1_last_match_time = 0;
				m_cross_1v1_next_match_time = 0;
			}
		}
		break;

	case CROSS_ACTIVITY_TYPE_TUANZHAN:
		{
			int scene_id = CrossTuanzhanConfig::Instance().GetOtherCfg().scene_id;

			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, CROSS_TUANZHAN_SCENE_KEY);
			if (NULL == scene)
			{
				World::GetInstWorld()->GetSceneManager()->CreateFB(scene_id, CROSS_TUANZHAN_SCENE_KEY, &scene);
			}

			if (NULL != scene && Scene::SCENE_TYPE_CROSS_TUANZHAN == scene->GetSceneType())
			{
				SpecialCrossTuanzhan *cross_tuanzhan = dynamic_cast<SpecialCrossTuanzhan*>(scene->GetSpecialLogic());
				if (nullptr == cross_tuanzhan)
				{
					return;
				}
				if (ACTIVITY_STATUS_OPEN == to_status)
				{
					cross_tuanzhan->OnActivityOpen();
				}
				else if (ACTIVITY_STATUS_CLOSE == to_status)
				{
					cross_tuanzhan->OnActivityClose();
				}
			}
		}
		break;

	case CROSS_ACTIVITY_TYPE_PASTURE:
		{
			int scene_id = CrossPastureConfig::Instance().GetOtherCfg().scene_id;

			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, CROSS_PUBLIC_SCENE_KEY);
			if (NULL == scene)
			{
				World::GetInstWorld()->GetSceneManager()->CreateFB(scene_id, CROSS_PUBLIC_SCENE_KEY, &scene);
			}

			if (NULL != scene && Scene::SCENE_TYPE_CROSS_PASTURE == scene->GetSceneType())
			{
				SpecialCrossPasture *cross_pasture = dynamic_cast<SpecialCrossPasture*>(scene->GetSpecialLogic());
				if (nullptr == cross_pasture)
				{
					return;
				}
				if (ACTIVITY_STATUS_OPEN == to_status)
				{
					cross_pasture->OnActivityOpen();
				}
				else if (ACTIVITY_STATUS_CLOSE == to_status)
				{
					cross_pasture->OnActivityClose();
				}
			}
		}
		break;

	case CROSS_ACTIVITY_TYPE_CROSS_BOSS:
		{
			if (!CrossConfig::Instance().IsHiddenServer())
			{
				return;
			}

			int scene_id_list[CROSS_BOSS_SCENE_MAX] = {0};
			int count = CrossBossConfig::Instance().GetSceneIdList(scene_id_list);
			for (int i = 0; i < count && i < CROSS_BOSS_SCENE_MAX; ++ i)
			{
				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);
				if (NULL == scene)
				{
					World::GetInstWorld()->GetSceneManager()->CreateFB(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY, &scene);
				}

				if (NULL != scene && Scene::SCENE_TYPE_CROSS_BOSS == scene->GetSceneType())
				{
					SpecialCrossBoss *cross_boss = dynamic_cast<SpecialCrossBoss*>(scene->GetSpecialLogic());
					if (nullptr == cross_boss)
					{
						return;
					}
					if (ACTIVITY_STATUS_OPEN == to_status)
					{
						cross_boss->OnActivityOpen();
					}
					else if (ACTIVITY_STATUS_CLOSE == to_status)
					{
						cross_boss->OnActivityClose();
					}
				}
			}
		}
		break;

	case CROSS_ACTIVITY_TYPE_HOT_SPRING:
		{
			int scene_id = CrossHotSpringConfig::Instance().GetSceneID();

			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, CROSS_HS_SCENE_KEY);
			if (NULL == scene)
			{
				World::GetInstWorld()->GetSceneManager()->CreateFB(scene_id, CROSS_BOSS_SCENE_KEY, &scene);
			}

			if (NULL != scene && Scene::SCENE_TYPE_HOTSPRING == scene->GetSceneType())
			{
				SpecialCrossHotspring *hot_spring = dynamic_cast<SpecialCrossHotspring*>(scene->GetSpecialLogic());
				if (nullptr != hot_spring)
				{
					hot_spring->OnActivityStatusChange(to_status);
				}
			}
		}
		break;

	case CROSS_ACTIVITY_TYPE_SHUIJING:
		{
			int scene_id = CrossShuiJingConfig::Instance().GetOtherCfg().scene_id;
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, CROSS_PUBLIC_SCENE_KEY);
			if (NULL == scene)
			{
				World::GetInstWorld()->GetSceneManager()->CreateFB(scene_id, CROSS_PUBLIC_SCENE_KEY, &scene);
			}

			if (NULL != scene && Scene::SCENE_TYPE_CROSS_SHUIJING == scene->GetSceneType())
			{
				SpecialCrossShuiJing *cross_shuijing = dynamic_cast<SpecialCrossShuiJing*>(scene->GetSpecialLogic());
				if (nullptr == cross_shuijing)
				{
					return;
				}
				if (ACTIVITY_STATUS_OPEN == to_status)
				{
					cross_shuijing->OnActivityOpen();
				}
				else if (ACTIVITY_STATUS_CLOSE == to_status)
				{
					cross_shuijing->OnActivityClose();
				}
			}
		}
		break;

	case CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE:
	{
		crossgameprotocal::GameCrossGuildBattleInfoSync cmd;
		crossgameprotocal::GameCrossGuildBattleRankLogSync log_cmd;

		int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
		CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);

		std::vector<SpecialCrossGuildBattle *> effective_special_vec;

		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);
			if (NULL == scene)
			{
				World::GetInstWorld()->GetSceneManager()->CreateFB(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY, &scene);
			}

			if (NULL != scene && Scene::SCENE_TYPE_CROSS_GUILDBATTLE == scene->GetSceneType())
			{
				SpecialCrossGuildBattle *cross_guildbattle = dynamic_cast<SpecialCrossGuildBattle*>(scene->GetSpecialLogic());
				if (nullptr == cross_guildbattle)
				{
					continue;
				}
				
				if (ACTIVITY_STATUS_OPEN == to_status)
				{
					WorldStatus::Instance().ResetCrossGuildBattleInfo();
				}
				else if (ACTIVITY_STATUS_CLOSE == to_status)
				{
					cross_guildbattle->GetGuildRankList(cmd.scene_list[i].rank_list, log_cmd.scene_list[i].rank_list);
					if (NULL != cross_guildbattle)
					{
						effective_special_vec.push_back(cross_guildbattle);
					}

					this->SortRoleScoreRank();

					//同步数据
					cmd.count = 0;

					for (auto it = m_guildbattle_user_score_rank_map.begin(); it != m_guildbattle_user_score_rank_map.end() && cmd.count < CROSS_GUILDBATTLE_MAX_USER_SCORE_RANK_NUM; it++)
					{
						auto vec_it = it->second;
						if (vec_it.empty())
						{
							continue;
						}

						cmd.user_core_rank_list[cmd.count].guild_id = vec_it[0].guild_id;
						cmd.user_core_rank_list[cmd.count].role_id = vec_it[0].role_id;
						cmd.user_core_rank_list[cmd.count].score = vec_it[0].score;

						cmd.count++;
					}
				}
				cross_guildbattle->OnActivityStatusChange(to_status);
			}
		}

		if (ACTIVITY_STATUS_STANDY == to_status)
		{
			m_cross_guildbattle_map.clear();
			m_guildbattle_user_rank_map.clear();
		}
		else if (ACTIVITY_STATUS_OPEN == to_status)				//用于同步到scenemanager触发活动开启江湖传闻
		{
			GuildManager::Instance().OnCrossGuildBattleBegin();

			{
				static ActivityStatus ggas;
				ggas.activity_type = (short)CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE;
				ggas.status = (short)to_status;
				ggas.activity_begin_time = 0;
				ggas.activity_end_time = 0;
				ggas.data = 0;

				World::GetInstWorld()->GetSceneManager()->OnSyncActivityStatus(&ggas);
			}
		}

		if (ACTIVITY_STATUS_CLOSE == to_status && CrossConfig::Instance().IsHiddenServer())
		{
			InternalComm::Instance().SendToCross((const char *)&cmd, sizeof(cmd), cross_activity_type);
			InternalComm::Instance().SendToCross((const char *)&log_cmd, sizeof(log_cmd), cross_activity_type);

			this->SyncRoleTopToGame();		//同步玩家积分第一名回原服

			// 下发日志数据给各个场景的各个玩家
			if (!effective_special_vec.empty())
			{
				Protocol::SCCrossGuildBattleGetRankInfoResp role_log_cmd;
				role_log_cmd.send_type = Protocol::SCCrossGuildBattleGetRankInfoResp::ST_ACT_END;
				memcpy(&role_log_cmd.scene_list, &log_cmd.scene_list, sizeof(role_log_cmd.scene_list));
				const int length = (int)sizeof(role_log_cmd);

				for (std::vector<SpecialCrossGuildBattle *>::iterator it = effective_special_vec.begin(); it != effective_special_vec.end(); ++it)
				{
					(*it)->SendRankLogToRole((const char *)&role_log_cmd, length);
				}
			}
			this->SendCrossGuildBattleInfoStatisticAll();
			this->SendResultInGuildChat();
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_DARK_NIGHT:
	{
		int scene_id = CrossDarkNightConfig::Instance().GetOtherCfg().scene_id;

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, CROSS_HS_SCENE_KEY);
		if (NULL == scene)
		{
			World::GetInstWorld()->GetSceneManager()->CreateFB(scene_id, CROSS_BOSS_SCENE_KEY, &scene);
		}

		if (NULL != scene && Scene::SCENE_TYPE_CROSS_DARK_NIGHT == scene->GetSceneType())
		{
			SpecialCrossDarkNight *darknight = dynamic_cast<SpecialCrossDarkNight*>(scene->GetSpecialLogic());
			if (nullptr == darknight)
			{
				return;
			}
			if (ACTIVITY_STATUS_OPEN == to_status)
			{
				darknight->OnActivityOpen();
			}
			else if (ACTIVITY_STATUS_CLOSE == to_status)
			{
				darknight->OnActivityClose();
			}
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_FISHING:
	{
		SpecialCrossFishing* special_cross_fishing = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossFishing(CROSS_PUBLIC_SCENE_KEY, true);
		if (special_cross_fishing != nullptr)
		{
			special_cross_fishing->OnActivityStatusChange(to_status);
			if (ACTIVITY_STATUS_OPEN == to_status)
			{
				special_cross_fishing->SetEndTime(end_time);
			}
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB:
	{
		const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();

		for (int i = 0; i < CROSS_NIGHT_FIGHT_MAX_SCENE_NUM; ++i)
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.scene_id, Scene::SCENE_TYPE_NIGHT_FIGHT_FB + i);
			if (NULL == scene)
			{
				continue;
			}

			if (NULL != scene && Scene::SCENE_TYPE_NIGHT_FIGHT_FB == scene->GetSceneType())
			{
				SpecialNightFight *cross_nightfight = dynamic_cast<SpecialNightFight*>(scene->GetSpecialLogic());
				if (nullptr != cross_nightfight)
				{
					cross_nightfight->OnActivityStatusChange(to_status);
				}
			}
		}
		if (ACTIVITY_STATUS_OPEN == to_status)
		{
			WorldStatus2::Instance().ClearNightFightActivityTopRank();
		}
	}
	break;
	case CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB:
	{
		const MessBattleFbOtherConfig& other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();
		for (int i = 0; i < CROSS_MESS_BATTLE_MAX_SCENE_NUM; ++i)
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.scene_id, Scene::SCENE_TYPE_MESS_BATTLE_FB + i);
			if (NULL == scene)
			{
				break;
			}

			if (NULL != scene && Scene::SCENE_TYPE_MESS_BATTLE_FB == scene->GetSceneType())
			{
				SpecialMessBattle *cross_mess_battle = dynamic_cast<SpecialMessBattle*>(scene->GetSpecialLogic());
				if (nullptr != cross_mess_battle)
				{
					cross_mess_battle->OnActivityStatusChange(to_status);
				}
			}
		}
		if (ACTIVITY_STATUS_OPEN == to_status)
		{
			WorldStatus2::Instance().ClearMessBattleActiviyTopRank();
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_LIEKUN_FB:  // 猎鲲副本(灵鲲之战)
	{
		const CrossLieKunFbZoneCfg *zone_cfg = NULL;

		if (CrossConfig::Instance().IsHiddenServer())
		{
			// 隐藏服才需要创建副本
			for (int i = 0; i < LIEKUN_ZONE_TYPE_COUNT; ++i)
			{
				zone_cfg = CrossLieKunFbConfig::Instance().GetCrossLieKunFbZoneCfg(i);
				if (zone_cfg && zone_cfg->scene_id > 0)
				{
					Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(zone_cfg->scene_id, CROSS_PUBLIC_SCENE_KEY);
					if (NULL == scene && ACTIVITY_STATUS_OPEN == to_status)
					{
						World::GetInstWorld()->GetSceneManager()->CreateFB(zone_cfg->scene_id, CROSS_PUBLIC_SCENE_KEY, &scene);
					}

					if (NULL != scene && Scene::SCENE_TYPE_CROSS_LIEKUN_FB == scene->GetSceneType())
					{
						SpecialCrossLieKunFb *cross_liekun = dynamic_cast<SpecialCrossLieKunFb*>(scene->GetSpecialLogic());
						if (nullptr == cross_liekun)
						{
							continue;
						}
						if (ACTIVITY_STATUS_OPEN == to_status)
						{
							cross_liekun->OnActivityOpen();
						}
						else if (ACTIVITY_STATUS_CLOSE == to_status)
						{
							cross_liekun->OnActivityClose();
						}
					}
				}
			}
		}

		if (ACTIVITY_STATUS_OPEN == to_status || ACTIVITY_STATUS_CLOSE == to_status)
		{
			m_cross_liekunfb_user_map.clear();
			WorldStatus2::Instance().ClearCrossLieKunFBGuildId(ACTIVITY_STATUS_OPEN == to_status);
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_CROSS_MIZANG_BOSS:
	{
		if (!CrossConfig::Instance().IsHiddenServer())
		{
			return;
		}

		int scene_id_list[CROSS_MIZANG_BOSS_SCENE_MAX] = { 0 };
		int count = CrossMizangBossConfig::Instance().GetSceneIdList(scene_id_list);
		for (int i = 0; i < count && i < CROSS_MIZANG_BOSS_SCENE_MAX; ++i)
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);
			if (NULL == scene)
			{
				World::GetInstWorld()->GetSceneManager()->CreateFB(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY, &scene);
			}

			if (NULL != scene && Scene::SCENE_TYPE_CROSS_MIZANG_BOSS == scene->GetSceneType())
			{
				SpecialCrossMiZangBoss *cross_boss = dynamic_cast<SpecialCrossMiZangBoss*>(scene->GetSpecialLogic());
				if (nullptr == cross_boss)
				{
					return;
				}
				if (ACTIVITY_STATUS_OPEN == to_status)
				{
					cross_boss->OnActivityOpen();
				}
				else if (ACTIVITY_STATUS_CLOSE == to_status)
				{
					cross_boss->OnActivityClose();
				}
			}
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_CROSS_YOUMING_BOSS:
	{
		if (!CrossConfig::Instance().IsHiddenServer())
		{
			return;
		}

		int scene_id_list[CROSS_YOUMING_BOSS_SCENE_MAX] = { 0 };
		int count = CrossYoumingBossConfig::Instance().GetSceneIdList(scene_id_list);
		for (int i = 0; i < count && i < CROSS_YOUMING_BOSS_SCENE_MAX; ++i)
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);
			if (NULL == scene)
			{
				World::GetInstWorld()->GetSceneManager()->CreateFB(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY, &scene);
			}

			if (NULL != scene && Scene::SCENE_TYPE_CROSS_YOUMING_BOSS == scene->GetSceneType())
			{
				SpecialCrossYouMingBoss *cross_boss = dynamic_cast<SpecialCrossYouMingBoss*>(scene->GetSpecialLogic());
				if (nullptr == cross_boss)
				{
					return;
				}
				if (ACTIVITY_STATUS_OPEN == to_status)
				{
					cross_boss->OnActivityOpen();
				}
				else if (ACTIVITY_STATUS_CLOSE == to_status)
				{
					cross_boss->OnActivityClose();
				}
			}
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI:  // 跨服边境之地
	{
		if (CrossConfig::Instance().IsHiddenServer())
		{
			// 隐藏服才需要创建副本
			const CrossBianJingZhiDiOtherCfg & other_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiOtherCfg();
			if (other_cfg.m_sceneid > 0)
			{
				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.m_sceneid, CROSS_PUBLIC_SCENE_KEY);
				if (NULL == scene && (ACTIVITY_STATUS_OPEN == to_status || ACTIVITY_STATUS_STANDY == to_status))
				{
					World::GetInstWorld()->GetSceneManager()->CreateFB(other_cfg.m_sceneid, CROSS_PUBLIC_SCENE_KEY, &scene);
				}

				if (NULL != scene && Scene::SCENE_TYPE_CROSS_BIANJINGZHIDI == scene->GetSceneType())
				{
					SpecialCrossBianJingZhiDi *cross_special_bjzd = dynamic_cast<SpecialCrossBianJingZhiDi*>(scene->GetSpecialLogic());
					if (nullptr != cross_special_bjzd)
					{
						cross_special_bjzd->OnActivityStatusChange(to_status);
					}
				}
			}
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_HUSONG_SHUIJING:
	{
		if (CrossConfig::Instance().IsHiddenServer())
		{
			const HusongSJOtherCfg & other_cfg = HusongShuijingConfig::Instance().GetOtherCfg();
			if (other_cfg.scene_id > 0)
			{
				Scene * scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.scene_id, CROSS_PUBLIC_SCENE_KEY);
				if(NULL == scene && (ACTIVITY_STATUS_OPEN == to_status || ACTIVITY_STATUS_STANDY == to_status))
				{
					World::GetInstWorld()->GetSceneManager()->CreateFB(other_cfg.scene_id, CROSS_PUBLIC_SCENE_KEY, &scene);
				}

				if (NULL != scene && Scene::SCENE_TYPE_CROSS_HUSONG_SHUIJING == scene->GetSceneType())
				{
					SpecialCrossHusongShuijing * cross_special_hssj = dynamic_cast<SpecialCrossHusongShuijing *>(scene->GetSpecialLogic());
					if (nullptr != cross_special_hssj)
					{
						cross_special_hssj->OnActivityStatusChange(to_status);
					}
				}
			}
		}
	}
	break;

	}
}

void ActivityShadow::ShenzhouWeaponCalcNextFleshGatherTime(time_t now)
{
	if (m_shenzhou_weapon_next_refresh_gather_time != 0 && now < m_shenzhou_weapon_next_refresh_gather_time)
	{
		return;
	}

	time_t today_zero_time = GetZeroTime(now);
	time_t today_sec = now - today_zero_time;

	const ShenzhouOtherCfg &other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();
	time_t today_begin_refresh_time = today_zero_time + other_cfg.refresh_gather_begin_time * SECOND_PER_HOUR;
	time_t today_end_reflesh_time = today_zero_time + other_cfg.refresh_gather_end_time * SECOND_PER_HOUR;

	if (m_shenzhou_weapon_next_refresh_gather_time < today_begin_refresh_time)
	{
		m_shenzhou_weapon_next_refresh_gather_time = today_begin_refresh_time;

		if (now	> today_begin_refresh_time)
		{
			m_shenzhou_weapon_next_refresh_gather_time = now + (1800 - today_sec % 1800); // 计算下一次刷新时间，半小时刷一次
		}

		this->SynceShenzhouGatherInfoToAll();
	}
	else
	{
		if (now	 > m_shenzhou_weapon_next_refresh_gather_time)
		{
			this->ShenzhouWeaponReFreshGathers();

			if (m_shenzhou_weapon_next_refresh_gather_time < today_end_reflesh_time - 900)
			{
				m_shenzhou_weapon_next_refresh_gather_time = now + (1800 - today_sec % 1800); // 计算下一次刷新时间，半小时刷一次
			}
			else
			{
				// 最后一次刷新，直接将下次刷新时间设置成第二天开始刷新时间
				int next_day_refresh_interval = EngineAdapter::Instance().NextDayInterval(other_cfg.refresh_gather_begin_time, 0, 0);
				m_shenzhou_weapon_next_refresh_gather_time = now + next_day_refresh_interval;
			}
			
			this->SynceShenzhouGatherInfoToAll();
		}
	}
}

void ActivityShadow::ShenzhouWeaponReFreshGathers()
{
	std::vector<Posi> &poin_list = LOGIC_CONFIG->GetShenzhouWeaponCfg().GatherPointList();
	int point_num = static_cast<int>(poin_list.size());
	if (point_num <= 0) return;

	const ShenzhouOtherCfg &other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();
	int refresh_gather_num = other_cfg.gather_num;
	if (refresh_gather_num <= 0) return;

	if (point_num > SHENZHOU_WEAPON_MAX_MELT_GATHER_NUM)
	{
		point_num = SHENZHOU_WEAPON_MAX_MELT_GATHER_NUM;
	}

	if (refresh_gather_num > point_num)
	{
		refresh_gather_num = point_num;
	}

	static int tmp_index_list[SHENZHOU_WEAPON_MAX_MELT_GATHER_NUM]; // 下标数组
	for (int index = 0; index < SHENZHOU_WEAPON_MAX_MELT_GATHER_NUM; ++ index)
	{
		tmp_index_list[index] = index;
	}

	for (int index = 0; index < point_num; ++ index) // 打乱顺序
	{
		int rand_index = RandomNum(point_num);

		int tmp = tmp_index_list[index];
		tmp_index_list[index] = tmp_index_list[rand_index];
		tmp_index_list[rand_index] = tmp;
	}

//	bool is_notice = false;
	for (int index = 0; index < refresh_gather_num; ++ index) // 创建采集物
	{
		int point_index = tmp_index_list[index];
		if (point_index < 0 || point_index >= point_num) continue;

		Posi gather_pos = poin_list[point_index];
		{
			Scene *cur_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.gather_scene_id, 0); // 只查找普通场景，所以key用0
			if (NULL != cur_scene && cur_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, gather_pos.x, gather_pos.y))
			{
				GatherObj *gather_obj = new GatherObj();
				gather_obj->SetPos(gather_pos);
				gather_obj->Init(NULL, other_cfg.gather_id, other_cfg.gather_time_s * 1000, 0, true, other_cfg.gather_live_time_s);
				cur_scene->AddObj(gather_obj);
				this->ChangeShenzhouGatherNum(1);
//				is_notice = true;
			}
		}
	}

// 	if (is_notice)
// 	{
// 		int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenzhou_gather_refresh_content, 410);
// 		if (len > 0) 
// 		{
// 			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL);
// 		}
// 	}
		}

SyncUserInfoOnBack * ActivityShadow::GetCrossUserInfoOnBack(long long uuid)
{
	SyncUserInfoOnBackMapIt it = m_syncuserinfo_onback_map.find(uuid);
	if (it == m_syncuserinfo_onback_map.end())
	{
		SyncUserInfoOnBack user;
		user.uuid = uuid;
		m_syncuserinfo_onback_map[uuid] = user;
	}

	if (it == m_syncuserinfo_onback_map.end())
	{
		it = m_syncuserinfo_onback_map.find(uuid);
	}

	if (it != m_syncuserinfo_onback_map.end())
	{
		return &it->second;
	}

	return NULL;
}

void ActivityShadow::DeleteCrossUserInfoOnBack(long long uuid)
{
	if (m_syncuserinfo_onback_map.end() != m_syncuserinfo_onback_map.find(uuid))
	{
		m_syncuserinfo_onback_map.erase(uuid);
	}
}

const CrossTeamFBUserInfo * ActivityShadow::GetCrossTeamFBUserInfo(long long uuid)
{
	CrossTeamFBUserInfoMapIt user_it = m_cross_team_fb_user_info.find(uuid);
	if (m_cross_team_fb_user_info.end() == user_it) return NULL;

	return &user_it->second;
}

void ActivityShadow::SyncCrossTeamFBRoomInfo(CrossTeamFBUserInfo info)
{
	CrossTeamFBUserInfoMapIt user_it = m_cross_team_fb_user_info.find(info.uuid);

	if (-1 == info.layer)
	{
		// 移除跨服用户
		if (m_cross_team_fb_user_info.end() == user_it)
		{
			// 用户不存在
			return;
		}

		m_cross_team_fb_user_info.erase(info.uuid);
	}
	else
	{
		if (m_cross_team_fb_user_info.end() != user_it)
		{
			// 跨服用户已存在，更新信息
			user_it->second = info;
		}
		else
		{
			m_cross_team_fb_user_info[info.uuid] = info;
		}
	}
}

void ActivityShadow::SendXiuLuoTowerAllRoleInfo()
{
	for (int i = 0; i < CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER; i++)
	{
		int scene_id = CrossXiuluoTowerCommonConfig::Instance().GetLayerSceneId(i);

		SpecialCrossXiuluoTower *xiuluo_tower = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossXiuluoTower(scene_id, CROSS_ACTIVITY_XIULUO_TOWER_SCENE_KEY);
		if (NULL != xiuluo_tower)
		{
			xiuluo_tower->SendAllRoleInfo();
		}
	}
}

void ActivityShadow::ChangeShenzhouGatherNum(int num)
{
	m_shenzhou_weapon_gather_num += num;

	if (m_shenzhou_weapon_gather_num < 0)
	{
		m_shenzhou_weapon_gather_num = 0;
	}
}

void ActivityShadow::SynceShenzhouGatherInfoToAll()
{
	const ShenzhouOtherCfg &other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();
	Scene *shenzhou_gather_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.gather_scene_id, 0); // 只查找普通场景，所以key用0
	if (NULL != shenzhou_gather_scene)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(shenzhou_gather_scene->RoleNum()); i++)
		{
			Role *temp_role = shenzhou_gather_scene->GetRoleByIndex(i);
			if (NULL != temp_role)
			{
				temp_role->GetShenzhouWeapon()->SendGatherInfo();
			}
		}
	}
}

CrossGuildBattleUserInfo * ActivityShadow::GetCrossGuildBattleUser(Role *role)
{
	if (NULL == role) return NULL;

	UniqueUserID uuid = role->GetUniqueUserID();
	CrossGuildBattleMap::iterator iter = m_cross_guildbattle_map.find(uuid);

	if (m_cross_guildbattle_map.end() == iter)
	{
		CrossGuildBattleUserInfo user_info;
		user_info.role_id = uuid;
		user_info.guild_post = role->GetGuildPost();
		role->GetName(user_info.user_name);

		m_cross_guildbattle_map.insert(std::make_pair(uuid, user_info));

		iter = m_cross_guildbattle_map.find(uuid);
		if (iter == m_cross_guildbattle_map.end())
		{
			return NULL;
		}

		UniqueUserID guild_id = this->GetGuildUniqueUserId(role);
		std::map<UniqueUserID, std::vector<CrossGuildBattleUserInfo*> >::iterator rank_it =  m_guildbattle_user_rank_map.find(guild_id);
		if (rank_it != m_guildbattle_user_rank_map.end())
		{
			rank_it->second.push_back(&iter->second);
		}
		else
		{
			std::vector<CrossGuildBattleUserInfo*> rank_vec;
			rank_vec.push_back(&iter->second);
			m_guildbattle_user_rank_map.insert(std::make_pair(guild_id, rank_vec));
		}

		return &iter->second;
	}
	else
	{
		return &iter->second;
	}

	return NULL;
}

void ActivityShadow::OnAddGuildBattleRoleScore(Role * role, int score)
{
	if (NULL == role) return;

	//UniqueUserID role_id = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
	CrossGuildBattleUserInfo* user = this->GetCrossGuildBattleUser(role);
	if (NULL == user) return;

	user->old_score = user->score;
	user->score += score;

	this->OnAddRoleScore(role, score);
	this->SortRoleScoreRank();

	const CrossGuildBattleScoreRewardCfg *reward_cfg = CrossGuildBattleConfig::Instance().GetScoreRewardCfg(user->old_score, user->score);
	if (NULL != reward_cfg)
	{
		role->AddExp(reward_cfg->exp, "crossguildbattle", Role::EXP_ADD_REASON_DEFAULT);
		role->GetShenshou()->AddHunalingScore(reward_cfg->convert_credit);
		role->AddCrossHonor(reward_cfg->cross_honor, "OnAddGuildBattleRoleScore");
		role->GetKnapsack()->PutListOrMail(reward_cfg->reward_item, PUT_REASON_CROSS_GUILDBATTLE_SCORE);
	}
}

void ActivityShadow::OnAddRoleScore(Role *role, int score)
{
	if (nullptr == role)
	{
		return;
	}

	UniqueUserID role_id = role->GetRoleCross()->GetCrossOriginalUniqueUserID();

	//auto rank_it = m_guildbattle_user_score_rank_map.find(role->GetGuildID());
	auto rank_it = m_guildbattle_user_score_rank_map.find(this->GetGuildUniqueUserId(role));
	if (rank_it != m_guildbattle_user_score_rank_map.end())
	{
		auto &score_vec = rank_it->second;
		auto vec_it = std::find_if(score_vec.begin(), score_vec.end(), [role_id](const CrossGuildBattleUserScoreRankInfo &cmp_info) {
			return role_id == cmp_info.role_id;
		});

		if (vec_it != score_vec.end())
		{
			(*vec_it).score += score;
		}
		else
		{
			CrossGuildBattleUserScoreRankInfo info;
			info.role_id = role_id;
			//info.guild_id = role->GetGuildID();
			info.guild_id = this->GetGuildUniqueUserId(role);
			info.score = score;
			score_vec.push_back(info);
		}
	}
	else
	{
		std::vector<CrossGuildBattleUserScoreRankInfo> score_vec;
		CrossGuildBattleUserScoreRankInfo info;
		info.role_id = role_id;
		//info.guild_id = role->GetGuildID();
		info.guild_id = this->GetGuildUniqueUserId(role);
		info.score = score;
		score_vec.push_back(info);

		//m_guildbattle_user_score_rank_map[role->GetGuildID()] = score_vec;
		m_guildbattle_user_score_rank_map[this->GetGuildUniqueUserId(role)] = score_vec;
	}
}

bool SortFuncCrossGuildBattleRank(const CrossGuildBattleUserInfo * user1, const CrossGuildBattleUserInfo * user2)
{
	if (user1 == NULL || user2 == NULL) return true;
	return user1->score > user2->score;
}

void ActivityShadow::SortRoleScoreRank()
{
	for (auto &rank_info_it : m_guildbattle_user_score_rank_map)
	{
		auto &score_vec = rank_info_it.second;
		std::sort(score_vec.begin(), score_vec.end());
	}

	for (auto &rank_info_it : m_guildbattle_user_rank_map)
	{
		auto &score_vec = rank_info_it.second;
		if (score_vec.size() > 0)
		{
			UniqueUserID old_mvp = score_vec[0]->role_id;
			std::sort(score_vec.begin(), score_vec.end(), &SortFuncCrossGuildBattleRank);
			if (old_mvp != score_vec[0]->role_id)
			{
				this->SyncCrossGuildBattleMvpInfo(rank_info_it.first);
			}
		}
	}
}

void ActivityShadow::SyncRoleTopToGame()
{
	this->SortRoleScoreRank();

	crossgameprotocal::CrossGameGuildBattleUserScoreRank cgmsg;
	cgmsg.count = 0;

	for (auto it = m_guildbattle_user_score_rank_map.begin(); it != m_guildbattle_user_score_rank_map.end() && cgmsg.count < CROSS_GUILDBATTLE_MAX_USER_SCORE_RANK_NUM; it++)
	{
		auto vec_it = it->second;
		if (vec_it.empty())
		{
			continue;
		}

		cgmsg.user_core_rank_list[cgmsg.count].guild_id = vec_it[0].guild_id;
		cgmsg.user_core_rank_list[cgmsg.count].role_id = vec_it[0].role_id;
		cgmsg.user_core_rank_list[cgmsg.count].score = vec_it[0].score;

		cgmsg.count ++;
	}

	int sendlen = sizeof(cgmsg) - (CROSS_GUILDBATTLE_MAX_USER_SCORE_RANK_NUM - cgmsg.count) * sizeof(CrossGuildBattleUserScoreRankInfo);

	InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_COMMON, 0, (const char *)&cgmsg, sendlen);

	m_guildbattle_user_score_rank_map.clear();
}

int ActivityShadow::GetGuildBattleRoleScore(Role *role)
{
	if (NULL == role) return 0;

	CrossGuildBattleMap::iterator iter = m_cross_guildbattle_map.find(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (m_cross_guildbattle_map.end() != iter)
	{
		return iter->second.score;
	}

	return 0;
}

int ActivityShadow::GetGuildBattleRoleSosTimes(Role *role)
{
	if (NULL == role) return 0;

	CrossGuildBattleMap::iterator iter = m_cross_guildbattle_map.find(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (m_cross_guildbattle_map.end() != iter)
	{
		return iter->second.sos_times;
	}

	return 0;
}

void ActivityShadow::OnCrossGuildBattleSpecialTimeNotice(Role *role)
{
	Protocol::SCCrossGuildBattleSpecialTimeNotice pro;
	pro.activty_status = this->CheckCrossGuildBattleSpecialTime() ? ACTIVITY_STATUS_OPEN : ACTIVITY_STATUS_CLOSE;

	if (nullptr != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));
	}
	else
	{
		World::GetInstWorld()->SendToAllGateway((const char *)&pro, sizeof(pro));
	}
}

bool ActivityShadow::CheckCrossGuildBattleSpecialTime()
{
	const CrossGuildBattleOtherCfg &other_cfg = CrossGuildBattleConfig::Instance().GetOtherCfg();

	const tm *local_time = EngineAdapter::Instance().LocalTime();

	int open_hour = other_cfg.special_open_time / 100;
	int open_min = other_cfg.special_open_time % 100;
	int end_hour = other_cfg.special_end_time / 100;
	int end_min = other_cfg.special_end_time % 100;

	if (local_time->tm_hour < open_hour)
	{
		return false;
	}

	if (local_time->tm_hour > end_hour)
	{
		return false;
	}

	if (local_time->tm_hour == open_hour && local_time->tm_min < open_min)
	{
		return false;
	}

	if (local_time->tm_hour == end_hour && local_time->tm_min >= end_min)
	{
		return false;
	}

	return true;
}

UniqueUserID ActivityShadow::GetGuildUniqueUserId(Role *role)
{
	if (NULL == role) return INVALID_UNIQUE_USER_ID;

	int plat_type = role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type;
	UniqueUserID guild_id(plat_type, IntToUid(role->GetGuildID()));

	return guild_id;
}

void ActivityShadow::SyncCrossGuildBattleSceneRoleNum(UniqueUserID guild_id)
{
	Protocol::SCCrossGuildBattleSceneGuilderNum sgn;

	sgn.guild_id = UniqueUserIDToLongLong(guild_id);

	int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
	CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);

	std::vector<SpecialCrossGuildBattle *> send_vec;
	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
	{
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);

		if (NULL != scene && Scene::SCENE_TYPE_CROSS_GUILDBATTLE == scene->GetSceneType())
		{
			SpecialCrossGuildBattle *cross_guildbattle = dynamic_cast<SpecialCrossGuildBattle*>(scene->GetSpecialLogic());
			if (nullptr == cross_guildbattle)
			{
				continue;
			}
			sgn.scene_info_list[i].scene_id = scene->GetSceneID();
			sgn.scene_info_list[i].guilder_num = cross_guildbattle->GetSceneGuildRoleNum(guild_id);
			send_vec.push_back(cross_guildbattle);
		}
	}

	for (unsigned int i = 0; i < send_vec.size(); ++i)
	{
		send_vec[i]->SendMsgToSceneGuild((const char*)&sgn,sizeof(sgn),guild_id);
	}
}

void ActivityShadow::OnCrossGuildBattlSosCost(Role * role)
{
	if (nullptr == role )
	{
		return ;
	}

	UniqueUserID role_id = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
	CrossGuildBattleUserInfo * user_info = GetCrossGuildBattleUser(role);
	if (NULL == user_info)
	{
		return;
	}
	//UniqueUserID sos_guild_id = this->GetGuildUniqueUserId(role);

	int cost = CrossGuildBattleConfig::Instance().GetSosCost(user_info->sos_times);
	if (cost > 0)
	{
		if (!role->GetKnapsack()->GetMoney()->UseAllGold(cost, __FUNCTION__))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return ;
		}
	}

	++user_info->sos_times;

	Protocol::SCCrossGuildBattleNotifyInfo cmd;
	cmd.notify_type = Protocol::SC_CROSS_GUILDBATTLE_NOTIFY_TYPE_SOS;
	cmd.param_1 = 0;
	cmd.param_2 = user_info->sos_times;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cmd, sizeof(cmd));

	crossgameprotocal::GameGameCrossGuildBattleSos pro;
	pro.guild_id = role->GetGuildID();
	pro.member_uid = UidToInt(role_id.user_id);
	role->GetName(pro.member_name);
	pro.member_camp = role->GetCamp();
	pro.member_scene_id = role->GetScene()->GetSceneID();
	pro.member_scene_key = role->GetScene()->GetSceneKey();
	pro.member_pos_x = role->GetPos().x;
	pro.member_pos_y = role->GetPos().y;
	UNSTD_STATIC_CHECK(MAX_SCENE_NAME_LENGTH == sizeof(pro.member_scene_name))
	F_STRNCPY(pro.member_scene_name, role->GetScene()->GetSceneName(), sizeof(pro.member_scene_name));
	pro.sos_type = GUILD_SOS_TYPE_CROSS_GUILD_BATTLE;
	pro.sos_sysmsg = 0;
	
	InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID, UniqueUserIDToLongLong(role_id),(const char *)&pro, sizeof(pro));
}

void ActivityShadow::SendCrossGuildBattleStatistic(UniqueUserID guild_id, int notify, Role *role)
{
	if (role != NULL)
	{
		guild_id = GetGuildUniqueUserId(role);
	}

	Protocol::SCGuildInfoStatistic sta;
	sta.notify_type = notify;
	sta.activity_type = CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE;
	sta.guild_id = UidToInt(guild_id.user_id);
	sta.count = 0;
	auto gd_it = m_guildbattle_user_rank_map.find(guild_id);
	if (gd_it != m_guildbattle_user_rank_map.end())
	{
		for (unsigned int i = 0; i < gd_it->second.size() && sta.count < MAX_MEMBER_COUNT; ++i)
		{
			CrossGuildBattleUserInfo * joiner_item = gd_it->second[i];
			if (NULL == joiner_item) continue;
			Protocol::SCGuildInfoStatistic::InfoItem & info_item = sta.info_list[sta.count];
			info_item.uid = UniqueUserIDToLongLong(joiner_item->role_id);
			F_STRNCPY(info_item.user_name, joiner_item->user_name, sizeof(GameName));
			info_item.guild_post = joiner_item->guild_post;
			info_item.kill_role = joiner_item->kill_role_num;
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
		int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
		CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);

		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);

			if (NULL != scene && Scene::SCENE_TYPE_CROSS_GUILDBATTLE == scene->GetSceneType())
			{
				SpecialCrossGuildBattle *cross_guildbattle = dynamic_cast<SpecialCrossGuildBattle*>(scene->GetSpecialLogic());
				if (nullptr == cross_guildbattle)
				{
					continue;
				}

				cross_guildbattle->SendMsgToSceneGuild((const char *)&sta, lenth, guild_id);
			}
		}
	}
}

void ActivityShadow::SendCrossGuildBattleInfoStatisticAll()
{
	for (auto gd_it = m_guildbattle_user_rank_map.begin(); gd_it != m_guildbattle_user_rank_map.end(); ++gd_it)
	{
		this->SendCrossGuildBattleStatistic(gd_it->first, Protocol::SCGuildInfoStatistic::NOTIFY_ACTIVITY_CLOSE);
	}
}

void ActivityShadow::SyncCrossGuildBattleMvpInfo(UniqueUserID guild_id, Role* role)
{
	auto gd_it = m_guildbattle_user_rank_map.find(guild_id);
	if (gd_it != m_guildbattle_user_rank_map.end())
	{
		Protocol::SCGuildMvpInfo mvp;
		mvp.activity_type = CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE;
		mvp.uid = 0;
		memset(mvp.user_name, 0, sizeof(mvp.user_name));
		if (gd_it->second.size() > 0)
		{
			mvp.uid = UniqueUserIDToLongLong(gd_it->second[0]->role_id);
			F_STRNCPY(mvp.user_name, gd_it->second[0]->user_name, sizeof(GameName));
		}
		if (NULL != role)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&mvp, sizeof(mvp));
		}
		else
		{
			int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
			CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);

			for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
			{
				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);

				if (NULL != scene && Scene::SCENE_TYPE_CROSS_GUILDBATTLE == scene->GetSceneType())
				{
					SpecialCrossGuildBattle *cross_guildbattle = dynamic_cast<SpecialCrossGuildBattle*>(scene->GetSpecialLogic());
					if (nullptr == cross_guildbattle)
					{
						continue;
					}

					cross_guildbattle->SendMsgToSceneGuild((const char *)&mvp, sizeof(mvp), guild_id);
				}
			}
		}
	}
}

void ActivityShadow::SendResultInGuildChat()
{
	for (auto gd_it = m_guildbattle_user_rank_map.begin(); gd_it != m_guildbattle_user_rank_map.end(); ++gd_it)
	{
		const UniqueUserID guild_id = gd_it->first;
		if (guild_id == INVALID_UNIQUE_USER_ID) continue;

		UniqueUserID mvp_uid = gd_it->second[0]->role_id; // 排序后的mvp
		UniqueUserID kill_role_uid = INVALID_UNIQUE_USER_ID;  // 最高击杀玩家者
		UniqueUserID kill_flag_uid = INVALID_UNIQUE_USER_ID;  // 最高击杀龙/凤珠者
		UniqueUserID hurt_role_uid = INVALID_UNIQUE_USER_ID;  // 最高伤害玩家者
		UniqueUserID hurt_flag_uid = INVALID_UNIQUE_USER_ID;  // 最高伤害龙/凤珠者
		static GameName tmp_role_name[5]; memset(tmp_role_name, 0, sizeof(tmp_role_name));

		int hight_kill_role = 0;
		int hight_kill_flag = 0;
		long long hight_hurt_role = 0;
		long long hight_hurt_flag = 0;

		for (unsigned int i = 0; i < gd_it->second.size(); ++i)
		{
			const CrossGuildBattleUserInfo * item = gd_it->second[i];
			if (NULL == item) continue;
			if (item->role_id == INVALID_UNIQUE_USER_ID) continue;

			if (item->kill_role_num > hight_kill_role) { kill_role_uid = item->role_id;  hight_kill_role = item->kill_role_num; }
			if (item->kill_flags > hight_kill_flag) { kill_flag_uid = item->role_id;  hight_kill_flag = item->kill_flags; }
			if (item->hurt_roles > hight_hurt_role) { hurt_role_uid = item->role_id;  hight_hurt_role = item->hurt_roles; }
			if (item->hurt_flags > hight_hurt_flag) { hurt_flag_uid = item->role_id;  hight_hurt_flag = item->hurt_flags; }
		}

		for (unsigned int i = 0; i < gd_it->second.size(); ++i)
		{
			const CrossGuildBattleUserInfo * item = gd_it->second[i];
			if (NULL == item) continue;
			if (item->role_id == INVALID_UNIQUE_USER_ID) continue;

			if (item->role_id == mvp_uid) { F_STRNCPY(tmp_role_name[0], item->user_name, sizeof(GameName)); }
			if (item->role_id == kill_role_uid) { F_STRNCPY(tmp_role_name[1], item->user_name, sizeof(GameName)); }
			if (item->role_id == kill_flag_uid) { F_STRNCPY(tmp_role_name[2], item->user_name, sizeof(GameName)); }
			if (item->role_id == hurt_role_uid) { F_STRNCPY(tmp_role_name[3], item->user_name, sizeof(GameName)); }
			if (item->role_id == hurt_flag_uid) { F_STRNCPY(tmp_role_name[4], item->user_name, sizeof(GameName)); }
		}

		// 发送聊天
		//Guild *guild = GuildManager::Instance().GetGuild(gd_it->first);
		//if (NULL != guild)
		{
			static char buff[1024]; memset(buff, 0, sizeof(buff));
			std::string sysmsg_buff;
			int msg_len = 0;

			if (mvp_uid != INVALID_UNIQUE_USER_ID)
			{
				msg_len = gstr::SysMsgContent(buff, 1024, "guild_crossguildbattle_sysmsg_title", tmp_role_name[0]);
				if (msg_len > 0)
				{
					sysmsg_buff.append(buff);
				}
			}

			if (kill_flag_uid != INVALID_UNIQUE_USER_ID)
			{
				msg_len = gstr::SysMsgContent(buff, 1024, "guild_crossguildbattle_sysmsg_killflag", tmp_role_name[2], hight_kill_flag);
				if (msg_len > 0)
				{
					sysmsg_buff.append(buff);
				}
			}

			if (kill_role_uid != INVALID_UNIQUE_USER_ID)
			{
				msg_len = gstr::SysMsgContent(buff, 1024, "guild_crossguildbattle_sysmsg_killrole", tmp_role_name[1], hight_kill_role);
				if (msg_len > 0)
				{
					sysmsg_buff.append(buff);
				}
			}

			if (hurt_role_uid != INVALID_UNIQUE_USER_ID)
			{
				msg_len = gstr::SysMsgContent(buff, 1024, "guild_crossguildbattle_sysmsg_hurtrole", tmp_role_name[3], hight_hurt_role);
				if (msg_len > 0)
				{
					sysmsg_buff.append(buff);
				}
			}

			//if (hurt_flag_uid != INVALID_UNIQUE_USER_ID)
			//{
			//	msg_len = gstr::SysMsgContent(buff, 1024, "guild_crossguildbattle_sysmsg_hurtflag", tmp_role_name[4], hight_hurt_flag);
			//	if (msg_len > 0)
			//	{
			//		sysmsg_buff.append(buff);
			//	}
			//}

// 			if ((int)sysmsg_buff.size() > 10 && sysmsg_buff.size() < sizeof(buff))
// 			{
// 				F_STRNCPY(buff, sysmsg_buff.c_str(), (int)sysmsg_buff.size());
// 				buff[(int)sysmsg_buff.size() - 10] = '\0';  // 去掉最后一个换行
// 				//guild->GuildSystemMsg(sysmsg_buff.size() - 1, buff);
// 				// 发送给在跨服的仙盟盟友
// 				static Protocol::SCSystemMsg sm;
// 				int sendlen = 0;
// 
// 				if (World::GetInstWorld()->GetSceneManager()->CreateSystemMsg(&sm, &sendlen, buff, (int)sysmsg_buff.size() - 10, SYS_MSG_ONLY_CHAT_GUILD))
// 				{
// 					World::GetInstWorld()->GetSceneManager()->SendToAllUser((const char*)&sm, sendlen, [guild_id](Role *tmp_role) -> bool
// 					{
// 						int plat_type = tmp_role->GetUniqueUserID().plat_type;
// 						UniqueUserID tmp_guild_id(plat_type, IntToUid(tmp_role->GetGuildID()));
// 						return guild_id == tmp_guild_id;
// 					});
// 				}
// 				
// 				// 发送给在原服的仙盟盟友
// 				static char buff_to_local[1204]; memset(buff_to_local, 0, sizeof(buff_to_local));
// 				crossgameprotocal::SyncCrossSystemMsgToOriginServer *scsmtom = (crossgameprotocal::SyncCrossSystemMsgToOriginServer *)buff_to_local;
// 				scsmtom->header.msg_type = crossgameprotocal::MT_CROSS_GAME_SYSTEM_MSG_TO_ORIGIN;
// 				scsmtom->uuid = UniqueUserIDToLongLong(guild_id);
// 				scsmtom->msg_type = SYS_MSG_ONLY_CHAT_GUILD;
// 				scsmtom->msg_length = sendlen;
// 				int send_len = sizeof(crossgameprotocal::SyncCrossSystemMsgToOriginServer) + sendlen;
// 				if (send_len > 0 && send_len < sizeof(buff_to_local))
// 				{
// 					F_STRNCPY(buff_to_local + sizeof(crossgameprotocal::SyncCrossSystemMsgToOriginServer), (const char*)&sm, sendlen + 1);
// 
// 					InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID, UniqueUserIDToLongLong(guild_id), buff_to_local, send_len);
// 				}
// 			}

			int sysmsg_len = sysmsg_buff.size() - 10;  // 去掉最后一个换行，换行字符串占用10个字节
			if (sysmsg_len > 0 && sysmsg_len < (int)sizeof(buff) - (int)sizeof(Protocol::CSChannelChat))
			{
				F_STRNCPY(buff, sysmsg_buff.c_str(), sysmsg_len + 1);

				ChatManager::Instance().OnGuildChatInCross(guild_id, chatdef::ORIGIN_TYPE_GUILD_SYSTEM_CHAT, buff, sysmsg_len + 1);
			}
		}
	}
}

void ActivityShadow::SendGuildQuestionGuildScoreRankInfo(Role *role)
{
	Protocol::SCGuildQuestionGuildRankInfo gri;
	gri.rank_count = 0;
	memset(gri.guild_rank_list, 0, sizeof(gri.guild_rank_list));

	for (GuildQuestionGuildRankVecIt it = m_guild_question_guild_rankvec.begin(); it != m_guild_question_guild_rankvec.end(); ++it)
	{
		if (it->guild_score <= 0)
		{
			continue;
		}

		gri.guild_rank_list[gri.rank_count].guild_id = it->guild_id;
		F_STRNCPY(gri.guild_rank_list[gri.rank_count].guild_name, it->guild_name, sizeof(GuildName));
		gri.guild_rank_list[gri.rank_count].guild_score = it->guild_score;

		++gri.rank_count;
		if (gri.rank_count >= GUILD_QUESTION_SCORE_RANK_MAX)
		{
			break;
		}
	}

	int sendlen = sizeof(gri) - (GUILD_QUESTION_SCORE_RANK_MAX - gri.rank_count) * sizeof(Protocol::SCGuildQuestionGuildRankInfo::RankInfo);

	// 给玩家发送排行信息
	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gri, sendlen);
	}
	else
	{
		const int scene_id = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg().scene_id;
		for (GuildQuestionGuildRankVecIt it = m_guild_question_guild_rankvec.begin(); it != m_guild_question_guild_rankvec.end(); ++it)
		{
			if (0 != it->guild_id)
			{
				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, it->guild_id);
				if (NULL != scene && Scene::SCENE_TYPE_GUILD_QUESTION == scene->GetSceneType())
				{
					for (int i = 0; i < (int)scene->RoleNum(); ++i)
					{
						Role *tmp_role = scene->GetRoleByIndex(i);
						if (NULL != tmp_role)
						{
							EngineAdapter::Instance().NetSend(tmp_role->GetNetId(), (const char*)&gri, sendlen);
						}
					}
				}
			}
		}
	}
}

void ActivityShadow::SendGuildQuestionPlayerInfo(Role *role, bool is_answer)
{
	if (NULL == role)
	{
		return;
	}

	static Protocol::SCGuildQuestionPlayerInfo pi;

	if (!is_answer)
	{
		GuildQuestionPlayerInfoMapIt it = m_guild_question_player_info_map.find(role->GetUserId());
		if (it != m_guild_question_player_info_map.end())
		{
			pi.uid = role->GetUID();
			F_STRNCPY(pi.name, role->GetName(), sizeof(pi.name));
			pi.exp = it->second.exp;
			pi.guild_gongxian = it->second.guild_gongxian;
			pi.guild_score = this->GetGuildQuestionGuildScore(role->GetGuildID());
			pi.gather_count = it->second.gather_count;
			pi.true_uid = 0;
			memset(pi.true_name, 0, sizeof(pi.true_name));

			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pi, sizeof(pi));
		}
	}
	else
	{
		Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
		if (NULL != guild)
		{
			static int member_uid_list[MAX_MEMBER_COUNT]; memset(member_uid_list, 0, sizeof(member_uid_list));

			int member_count = guild->GetGuildMemberManager().GetMemberList(member_uid_list);
			for (int i = 0; i < member_count && i < MAX_MEMBER_COUNT; ++i)
			{
				Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
				if (NULL != temp_role)
				{
					pi.uid = temp_role->GetUID();
					F_STRNCPY(pi.name, temp_role->GetName(), sizeof(pi.name));
					pi.exp = 0;
					pi.guild_gongxian = 0;
					pi.gather_count = 0;
					pi.guild_score = this->GetGuildQuestionGuildScore(temp_role->GetGuildID());
					pi.true_uid = role->GetUID();
					F_STRNCPY(pi.true_name, role->GetName(), sizeof(pi.true_name));

					GuildQuestionPlayerInfoMapIt it = m_guild_question_player_info_map.find(temp_role->GetUserId());
					if (it != m_guild_question_player_info_map.end())
					{
						pi.exp = it->second.exp;
						pi.guild_gongxian = it->second.guild_gongxian;
						pi.gather_count = it->second.gather_count;
					}

					EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char*)&pi, sizeof(pi));
				}
			}
		}
	}
}

void ActivityShadow::SendGuildQuestionQuestionInfo(GuildID guild_id, Role *role)
{
	if (!this->IsActivtyOpen(ACTIVITY_TYPE_GUILD_QUESTION))
	{
		return;
	}

	static Protocol::SCGuildQuestionQuestionInfo qi; qi.Reset();

	// 发送给帮派
	if (INVALID_GUILD_ID != guild_id)
	{
		Guild *guild = GuildManager::Instance().GetGuild(guild_id);
		if (NULL != guild)
		{
			GuildQuestionQuestionMapIt it = m_guild_question_question_map.find(guild_id);
			
			if (it != m_guild_question_question_map.end())
			{
				qi.question_index = it->second.question_number;
				qi.question_id = it->second.question_id;
				qi.question_end_timestamp = static_cast<unsigned int>(it->second.next_question_timestamp);
				LOGIC_CONFIG->GetGuildQuestionConfig().GetQuestionStr(it->second.question_id, qi.question_str);
			}
			
			// 如果尚未有该仙盟信息则答题未开始，仅发送答题状态
			qi.question_state = m_guild_question_state;
			qi.question_state_change_timestamp = static_cast<unsigned int>(m_guild_question_state_change_timestamp);

			static int member_uid_list[MAX_MEMBER_COUNT]; memset(member_uid_list, 0, sizeof(member_uid_list));

			int member_count = guild->GetGuildMemberManager().GetMemberList(member_uid_list);
			for (int i = 0; i < member_count && i < MAX_MEMBER_COUNT; ++i)
			{
				Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
				if (NULL != temp_role)
				{
					EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char*)&qi, sizeof(qi));
				}
			}
		}
	}
	// 发送给玩家
	else if (NULL != role && INVALID_GUILD_ID != role->GetGuildID())
	{
		Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
		if (NULL != guild)
		{
			GuildQuestionQuestionMapIt it = m_guild_question_question_map.find(role->GetGuildID());
			if (it != m_guild_question_question_map.end())
			{
				qi.question_state = m_guild_question_state;
				qi.question_state_change_timestamp = static_cast<unsigned int>(m_guild_question_state_change_timestamp);
				qi.question_index = it->second.question_number;
				qi.question_id = it->second.question_id;
				qi.question_end_timestamp = static_cast<unsigned int>(it->second.next_question_timestamp);
				LOGIC_CONFIG->GetGuildQuestionConfig().GetQuestionStr(it->second.question_id, qi.question_str);

				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&qi, sizeof(qi));
			}
		}
	}
	// 发送给所有人
	else if (0 == guild_id && NULL == role)
	{
		const GuildManager::GuildMap &guild_map = GuildManager::Instance().GetGuildMap();
		for (GuildManager::GuildMapConstIt it = guild_map.begin(), end = guild_map.end(); it != end; ++it)
		{
			Guild *guild = it->second;
			if (NULL == guild)
			{
				continue;
			}

			qi.question_state = m_guild_question_state;
			qi.question_state_change_timestamp = static_cast<unsigned int>(m_guild_question_state_change_timestamp);

			GuildQuestionQuestionMapIt ques_it = m_guild_question_question_map.find(guild->GetGuildID());
			if (ques_it != m_guild_question_question_map.end())
			{
				qi.question_index = ques_it->second.question_number;
				qi.question_id = ques_it->second.question_id;
				qi.question_end_timestamp = static_cast<unsigned int>(ques_it->second.next_question_timestamp);
				LOGIC_CONFIG->GetGuildQuestionConfig().GetQuestionStr(ques_it->second.question_id, qi.question_str);
			}

			// 成员逐个发送
			static int member_uid_list[MAX_MEMBER_COUNT];
			memset(member_uid_list, 0, sizeof(member_uid_list));

			int member_count = guild->GetGuildMemberManager().GetMemberList(member_uid_list);
			for (int i = 0; i < member_count && i < MAX_MEMBER_COUNT; ++i)
			{
				Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
				if (NULL != temp_role)
				{
					EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char*)&qi, sizeof(qi));
				}
			}
		}
	}
}

static bool GuildQuestionScoreSort(const GuildQuestionGuildRank &guild_1, const GuildQuestionGuildRank &guild_2)
{
	if (guild_1.guild_score != guild_2.guild_score)
	{
		return guild_1.guild_score > guild_2.guild_score;
	}

	return guild_1.answer_true_timestamp < guild_2.answer_true_timestamp;
}

void ActivityShadow::GuildQuestionRefreshGuildScore(GuildID guild_id, unsigned int now_second)
{
	if (0 == guild_id)
	{
		return;
	}

	for (GuildQuestionGuildRankVecIt it = m_guild_question_guild_rankvec.begin(); it != m_guild_question_guild_rankvec.end(); ++it)
	{
		if (guild_id == it->guild_id)
		{
			it->answer_true_timestamp = now_second;
			it->guild_score += LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg().guild_score;
			std::sort(m_guild_question_guild_rankvec.begin(), m_guild_question_guild_rankvec.end(), GuildQuestionScoreSort);
			break;
		}
	}
}

int ActivityShadow::GetGuildQuestionGuildScore(GuildID guild_id)
{
	for (GuildQuestionGuildRankVecIt it = m_guild_question_guild_rankvec.begin(); it != m_guild_question_guild_rankvec.end(); ++it)
	{
		if (guild_id == it->guild_id)
		{
			return it->guild_score;
		}
	}

	return 0;
}

int ActivityShadow::GetGuildQuestionGuildRank(GuildID guild_id)
{
	int rank = 0;
	for (GuildQuestionGuildRankVecIt it = m_guild_question_guild_rankvec.begin(); it != m_guild_question_guild_rankvec.end(); ++it)
	{
		rank++;
		if (guild_id == it->guild_id)
		{
			return rank;
		}
	}

	return rank;
}

void ActivityShadow::OnGuildQuestionEnterSceneReq(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (!this->IsActivtyStandy(ACTIVITY_TYPE_GUILD_QUESTION) &&!this->IsActivtyOpen(ACTIVITY_TYPE_GUILD_QUESTION))
	{
		gstr::SendError(role, "guild_question_not_open");
		return;
	}

	if (0 == role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	if (Scene::SCENE_TYPE_GUILD_QUESTION == role->GetScene()->GetSceneType())
	{
		gstr::SendError(role, "guild_already_in_guild_question");
		return;
	}

	if (!role->GetRoleStatusManager()->CanEnterFB()) return;

	const GuildQuestionOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg();

	SpecialGuildQuestion *guild_question = World::GetInstWorld()->GetSceneManager()->GetSpecialGuildQuestion(other_cfg.scene_id, role->GetGuildID(), true);
	if (NULL != guild_question)
	{
		this->AddGuildQuestionInfo(role);

		if (other_cfg.scene_id > 0)
		{
			// 活动参与日志
			gamelog::g_log_roleactivity.printf(LL_INFO, "ActivityShadow::OnGuildQuestionEnterSceneReq user[%d, %s], level[%d], camp[%d], activity_type[%d]",
				role->GetUID(), role->GetName(), role->GetLevel(), role->GetCamp(), ACTIVITY_TYPE_GUILD_QUESTION);

			World::GetInstWorld()->GetSceneManager()->GoTo(role, other_cfg.scene_id, role->GetGuildID(), gamecommon::GetDisRandPos(other_cfg.enter_pos, 5));
		}
	}
}

void ActivityShadow::OnJoinGuildQuestionActivity(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (this->IsRoleInGuildQuestionActivityState(role)) // 角色参与了这次答题 才允许加入
	{
		GuildQuestionPlayerInfo *guild_question_player = this->GetGuildQuestionPlayerInfo(role->GetUserId(), role->GetName());
		if (NULL != guild_question_player)
		{
			this->SendGuildQuestionPlayerInfo(role);
			this->SendGuildQuestionGuildScoreRankInfo(role);
		}

		EventHandler::Instance().OnJoinActivity(role, ACTIVITY_TYPE_GUILD_QUESTION);
	}

	this->SendGuildQuestionQuestionInfo(0, role);
}

bool ActivityShadow::IsRoleInGuildQuestionActivityState(Role *role) // 答题活动状态
{
	if (!this->IsActivtyStandy(ACTIVITY_TYPE_GUILD_QUESTION) && !this->IsActivtyOpen(ACTIVITY_TYPE_GUILD_QUESTION))
	{
		return false;
	}

	// 不限制答题场景
// 	if (NULL != role && NULL != role->GetScene() && Scene::SCENE_TYPE_GUILD_QUESTION == role->GetScene()->GetSceneType())
// 	{
// 		return true;
// 	}

	return true;
}

bool ActivityShadow::IsCanGatherGuildQuestion(Role *role)
{
	if (NULL == role || !this->IsActivtyOpen(ACTIVITY_TYPE_GUILD_QUESTION))
	{
		return false;
	}

	GuildQuestionPlayerInfoMapIt it = m_guild_question_player_info_map.find(role->GetUserId());
	if (it == m_guild_question_player_info_map.end())
	{
		return false;
	}

	const GuildQuestionOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg();

	if (it->second.gather_count >= other_cfg.gather_count_limit)
	{
		gstr::SendError(role, "gather_count_limit");
		return false;
	}

	return true;
}

GuildQuestionPlayerInfo * ActivityShadow::GetGuildQuestionPlayerInfo(const UserID &user_id, const GameName user_name)
{
	GuildQuestionPlayerInfoMapIt it = m_guild_question_player_info_map.find(user_id);
	if (it == m_guild_question_player_info_map.end()) // 不存在 插入新的用户
	{
		GuildQuestionPlayerInfo guild_question_player;
		guild_question_player.user_id = user_id;
		F_STRNCPY(guild_question_player.user_name, user_name, sizeof(GameName));

		m_guild_question_player_info_map[user_id] = guild_question_player;
	}

	if (it == m_guild_question_player_info_map.end()) it = m_guild_question_player_info_map.find(user_id);

	if (it != m_guild_question_player_info_map.end())
	{
		it->second.user_id = user_id;
		return &it->second;
	}

	return NULL;
}

bool ActivityShadow::OnGuildQuestionAnswerQuestion(Role *role, const char* buff, int total_len)
{
	if (NULL == role || 0 == role->GetGuildID()) return false;

	if (!this->IsRoleInGuildQuestionActivityState(role))
	{
		return false;
	}

	// 参加活动
	this->OnJoinGuildQuestionActivity(role);

	GuildQuestionPlayerInfoMapIt map_it = m_guild_question_player_info_map.find(role->GetUserId());
	if (map_it == m_guild_question_player_info_map.end())
	{
		return false;
	}

	// 找到自己的帮派
	int scene_id = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg().scene_id;
	SpecialGuildQuestion *guild_question = World::GetInstWorld()->GetSceneManager()->GetSpecialGuildQuestion(scene_id, role->GetGuildID(), true);
	if (NULL == guild_question)
	{
		return false;
	}

	GuildQuestionQuestionMapIt ques_it = m_guild_question_question_map.find(role->GetGuildID());
	if (ques_it == m_guild_question_question_map.end())
	{
		return false;
	}

	// 已经有人答对，无效
	if (ques_it->second.is_already_answer_true)
	{
		return false;
	}

	// 添加答题信息
	this->AddGuildQuestionInfo(role);

	GuildQuestionStr true_str; memset(true_str, 0, sizeof(true_str));
	LOGIC_CONFIG->GetGuildQuestionConfig().GetAnswerStr(ques_it->second.question_id, true_str);

	// 检查回答是否正确
	char *msg_content = (char *)buff + sizeof(Protocol::SCChannelChat);
	int msg_length = total_len - (int)sizeof(Protocol::SCChannelChat);

	int true_str_len = strlen(true_str);
	if (msg_length < true_str_len) // 暂时处理输入一个汉字也能回答正确的BUG。UTF8的编码暂不清楚
	{
		return false;
	}

	for (int i = 0; i < msg_length && i < (int)sizeof(true_str); ++i)
	{
		if (msg_content[i] != true_str[i])
		{
			return false;
		}
	}

	ques_it->second.is_already_answer_true = true;
	ques_it->second.question_right_num++;

	// 成就
//	EventHandler::Instance().OnAnswerGuildQuestion(role, true);

	// 回答正确，刷新帮派积分，排行榜
	this->GuildQuestionRefreshGuildScore(role->GetGuildID(), static_cast<unsigned int>(EngineAdapter::Instance().Time()));
	this->SendGuildQuestionGuildScoreRankInfo();
	this->SendGuildQuestionGuildScoreRankInfo(role);

	const GuildQuestionRewardConfig *reward_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetRewardCfg(role->GetLevel());
	if (NULL != reward_cfg)
	{
		// 给予奖励
		{	// 自己

			++map_it->second.right_num;
			//map_it->second.exp += reward_cfg->answer_right_myself_addexp;

			//策划需求，修改增加经验规则（2018-1-3）
			long long add_exp = static_cast<long long>(LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg().right_guild_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));
			map_it->second.exp += add_exp;

			map_it->second.guild_gongxian += reward_cfg->answer_right_myself_addgongxian;

			this->SendGuildQuestionPlayerInfo(role, true);

			//role->AddExp(reward_cfg->answer_right_myself_addexp, "OnGuildQuestionAnswerQuestion");

			//策划需求，修改增加经验规则（2018-1-3）
			role->AddExp(add_exp, "OnGuildQuestionAnswerQuestion", Role::EXP_ADD_REASON_DEFAULT);
			role->AddGuildGongxian(reward_cfg->answer_right_myself_addgongxian, "OnGuildQuestionAnswerQuestion");
		}

		{	// 在线同帮派其他所有人
			long long add_exp = 0;

// 			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, role->GetGuildID());
// 			if (NULL != scene && Scene::SCENE_TYPE_GUILD_QUESTION == scene->GetSceneType())
// 			{
// 			}

			GuildID guild_id = role->GetGuildID();
			std::set<Role*> role_list;

			World::GetInstWorld()->GetSceneManager()->GetAllRole(role_list, [guild_id](Role *cmp_role) -> bool {
				return cmp_role->GetGuildID() == guild_id;
			});

			for (auto member_role : role_list)
			{
				if (NULL == member_role)
				{
					continue;
				}

				if (member_role->GetUID() == role->GetUID())
				{
					continue;
				}

				GuildQuestionPlayerInfoMapIt member_it = m_guild_question_player_info_map.find(member_role->GetUserId());
				if (member_it != m_guild_question_player_info_map.end())
				{
					//member_it->second.exp += reward_cfg->answer_right_give_member_addexp;

					//策划需求，修改增加经验规则（2018-1-3）
					add_exp = static_cast<long long>(LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg().right_guild_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(member_role->GetLevel(), 0)));
					member_it->second.exp += add_exp;
					member_it->second.guild_gongxian += reward_cfg->answer_right_give_member_addgongxian;

					this->SendGuildQuestionPlayerInfo(member_role);
				}

				//member_role->AddExp(reward_cfg->answer_right_give_member_addexp, "OnGuildQuestionAnswerQuestion");
				member_role->AddExp(add_exp, "OnGuildQuestionAnswerQuestion", Role::EXP_ADD_REASON_DEFAULT);
				member_role->AddGuildGongxian(reward_cfg->answer_right_give_member_addgongxian, "OnGuildQuestionAnswerQuestion");
			}
		}

		{
			//刷新个人正确答题数排行
			bool is_find = false;
			for (GuilldQuestionPersonRankVecIt it = m_guild_question_person_rankvec.begin(); it != m_guild_question_person_rankvec.end(); ++it)
			{
				if (it->user_id == role->GetUID())
				{
					is_find = true;
					it->right_num = map_it->second.right_num;

					break;
				}
			}

			if (!is_find)
			{
				GuildQuestionPersonRank person_rank;
				person_rank.user_id = role->GetUID();
				role->GetName(person_rank.user_name);
				GuildShadow::Instance().GetGuildName(role->GetGuildID(), person_rank.guild_name);
				person_rank.right_num = map_it->second.right_num;

				m_guild_question_person_rankvec.push_back(person_rank);
			}

			std::sort(m_guild_question_person_rankvec.begin(), m_guild_question_person_rankvec.end(), std::less<GuildQuestionPersonRank>());
		}
	}

	time_t now = EngineAdapter::Instance().Time();
	const GuildQuestionOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg();

	// 小于答对时间下限，则修改刷新间隔
	if (now - ques_it->second.cur_question_timestamp < other_cfg.answer_right_interval_s)
	{
		ques_it->second.next_question_timestamp = ques_it->second.cur_question_timestamp + other_cfg.answer_right_interval_s;
	}
	// 否则马上刷下一题
	else
	{
		ques_it->second.next_question_timestamp = now;
	}

	// 答对最后一题时发群聊系统公告
	if(ques_it->second.question_number >= other_cfg.question_total_num)
	{
		this->BroadcastGuildquestionEnd(ques_it->second);
		ques_it->second.is_aready_broadcast = true;
	}

	this->SendGuildQuestionQuestionInfo(ques_it->second.guild_id);

	return true;
}

void ActivityShadow::OnGuildQuestionActivityStandy()
{
	m_guild_question_player_info_map.clear();
	m_guild_question_guild_rankvec.clear();
	m_guild_question_question_map.clear();
}

void ActivityShadow::OnGuildQuestionActivityStart()
{
	const int scene_id = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg().scene_id;
	for (GuildQuestionGuildRankVecIt it = m_guild_question_guild_rankvec.begin(); it != m_guild_question_guild_rankvec.end(); ++it)
	{
		SpecialGuildQuestion *guild_question = World::GetInstWorld()->GetSceneManager()->GetSpecialGuildQuestion(scene_id, it->guild_id);
		if (NULL != guild_question)
		{
			guild_question->OnActivityStart();
		}
	}

	const GuildQuestionOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg();
	time_t now = EngineAdapter::Instance().Time();

	// 切换到等待状态 通知客户端进入等待 
	m_guild_question_state_change_timestamp = now + other_cfg.begin_time;
	m_next_give_reward_timestamp = 0;
	m_guild_question_state = GUILD_QUESTION_STATE_STANDY;

	this->SendGuildQuestionQuestionInfo();
}

void ActivityShadow::OnGuildQuestionActivityClose()
{
	// 给帮派排行奖励
	this->RewardAllGuildQuestionGuildOnEnd();

	const int scene_id = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg().scene_id;
	int i = 0;

	for (GuildQuestionGuildRankVecIt it = m_guild_question_guild_rankvec.begin(); it != m_guild_question_guild_rankvec.end(); ++it)
	{
		i++;

		SpecialGuildQuestion *guild_question = World::GetInstWorld()->GetSceneManager()->GetSpecialGuildQuestion(scene_id, it->guild_id);
		if (NULL != guild_question)
		{
			guild_question->OnActivityClose();
		}

		//江湖传闻
		if (INVALID_GUILD_ID != it->guild_id)
		{
			int temp_max_score = 0;

			Role *role = NULL;
			for (auto &map_it : m_guild_question_player_info_map)
			{
				role = World::GetInstWorld()->GetSceneManager()->GetRole(map_it.first);
				if (role == NULL) continue;
				
				if (role->GetGuildID() == it->guild_id)
				{
					if (map_it.second.right_num > temp_max_score)
					{
						temp_max_score = map_it.second.right_num;
					}
				}
			}
			if (role != NULL)
			{
				EventHandler::Instance().OnAddGuildRumorEvent(it->guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_GUILD_QUESTION_RESULT, i);
			}
		}
	}

	m_guild_question_state_change_timestamp = 0;
	m_next_give_reward_timestamp = 0;
	m_guild_question_state = 0;
	m_guild_question_player_info_map.clear();
	m_guild_question_guild_rankvec.clear();
	m_guild_question_question_map.clear();
}

void ActivityShadow::OnGuildQuestionActivityUpdate()
{
	time_t now_second = EngineAdapter::Instance().Time();
	if (GUILD_QUESTION_STATE_STANDY == m_guild_question_state && m_guild_question_state_change_timestamp > 0 && now_second >= m_guild_question_state_change_timestamp)
	{
		m_guild_question_state = GUILD_QUESTION_STATE_START;
		m_next_give_reward_timestamp = now_second + LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg().reward_interval_s;

		// 放出题目
		const GuildManager::GuildMap &guild_map = GuildManager::Instance().GetGuildMap();
		for (GuildManager::GuildMapConstIt it = guild_map.begin(), end = guild_map.end(); it != end; ++it)
		{
			Guild *guild = it->second;
			if (NULL != guild)
			{
				int question_id = LOGIC_CONFIG->GetGuildQuestionConfig().RandQuestionID();
				const GuildQuestionOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg();

				GuildQuestionQuestion ques;
				ques.guild_id = guild->GetGuildID();
				ques.question_id = question_id;
				ques.question_number = 1;
				ques.question_set.insert(question_id);
				ques.is_already_answer_true = false;
				ques.cur_question_timestamp = now_second;
				ques.next_question_timestamp = now_second + other_cfg.next_question_interval;

				m_guild_question_question_map[guild->GetGuildID()] = ques;

				this->SendGuildQuestionQuestionInfo(guild->GetGuildID());
			}
		}
	}

	if ((GUILD_QUESTION_STATE_START == m_guild_question_state || GUILD_QUESTION_STATE_CLOSE == m_guild_question_state) &&
		0 != m_next_give_reward_timestamp && now_second > m_next_give_reward_timestamp)
	{
		m_next_give_reward_timestamp = now_second + LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg().reward_interval_s;

		// 隔一段时间场景内玩家就给奖励

		long long add_exp = 0;

		const int scene_id = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg().scene_id;
		for (GuildQuestionGuildRankVecIt it = m_guild_question_guild_rankvec.begin(); it != m_guild_question_guild_rankvec.end(); ++it)
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, it->guild_id);
			if (NULL != scene && Scene::SCENE_TYPE_GUILD_QUESTION == scene->GetSceneType())
			{
				for (int i = 0; i < (int)scene->RoleNum(); ++i)
				{
					Role *role = scene->GetRoleByIndex(i);
					if (NULL != role && this->IsRoleInGuildQuestionActivityState(role))
					{
						const GuildQuestionRewardConfig *reward_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetRewardCfg(role->GetLevel());
						if (NULL == reward_cfg)
						{
							continue;
						}

						GuildQuestionPlayerInfoMapIt map_it = m_guild_question_player_info_map.find(role->GetUserId());
						if (map_it != m_guild_question_player_info_map.end())
						{
							//map_it->second.exp += reward_cfg->exp;

							//策划需求，修改增加经验规则（2018-1-3）
							add_exp = static_cast<long long>(LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg().exp_factor_wait * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));
							map_it->second.exp += add_exp;
							map_it->second.guild_gongxian += reward_cfg->gongxian;

							this->SendGuildQuestionPlayerInfo(role);
						}

						role->AddExp(add_exp, "OnGuildQuestionActivityUpdate", Role::EXP_ADD_REASON_DEFAULT);
						role->AddGuildGongxian(reward_cfg->gongxian, "OnGuildQuestionActivityUpdate");
					}
				}
			}
		}
	}

	// 是否到了出题时间
	if ((GUILD_QUESTION_STATE_START == m_guild_question_state || GUILD_QUESTION_STATE_CLOSE == m_guild_question_state) &&
		!m_guild_question_question_map.empty())
	{
		for (GuildQuestionQuestionMapIt ques_it = m_guild_question_question_map.begin(); ques_it != m_guild_question_question_map.end(); ++ques_it)
		{
			GuildQuestionQuestion &question_info = ques_it->second;
			const GuildQuestionOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg();
			if (0 != question_info.next_question_timestamp && now_second > question_info.next_question_timestamp &&
				question_info.question_number < other_cfg.question_total_num)
			{
				question_info.cur_question_timestamp = now_second;
				question_info.next_question_timestamp = now_second + other_cfg.next_question_interval;
				question_info.is_already_answer_true = false;
				++question_info.question_number;

				int loop_times = 0;
				while (loop_times < 1000)
				{
					++loop_times;

					question_info.question_id = LOGIC_CONFIG->GetGuildQuestionConfig().RandQuestionID();
					std::set<int>::iterator set_it = question_info.question_set.find(question_info.question_id);
					if (set_it == question_info.question_set.end())
					{
						question_info.question_set.insert(question_info.question_id);
						break;
					}
				}

				this->SendGuildQuestionQuestionInfo(ques_it->second.guild_id);
			}
		}
	}
}

void ActivityShadow::RewardAllGuildQuestionGuildOnEnd()
{
	std::sort(m_guild_question_guild_rankvec.begin(), m_guild_question_guild_rankvec.end(), GuildQuestionScoreSort);

	const int NEED_BROADCAST_NUM = 3;	// 前三名
	int count = 0;
	GuildName guildname_list[NEED_BROADCAST_NUM];
	memset(guildname_list, 0, sizeof(guildname_list));

	// 奖励
	for (int i = 0; i < (int)m_guild_question_guild_rankvec.size() && i < GUILD_QUESTION_SCORE_RANK_MAX; ++i)
	{
		if (m_guild_question_guild_rankvec[i].guild_score <= 0)
		{
			continue;
		}

		if (i < NEED_BROADCAST_NUM)
		{
			++count;
			F_STRNCPY(guildname_list[i], m_guild_question_guild_rankvec[i].guild_name, sizeof(GuildName));
		}

		Guild *guild = GuildManager::Instance().GetGuild(m_guild_question_guild_rankvec[i].guild_id);
		if (NULL != guild)
		{
			// 团长
// 			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(guild->GetTuanzhuangRoleId()));
// 			if (NULL != role)
// 			{
// 				//红包
//				role->GetRoleGuild()->AddGuildRedPaperAuthority(PAPER_TYPE_GUILD_QUESTION, i + 1);
// 				continue;
// 			}

			// 副团长
// 			bool is_find_fu_tuangzhang = false;
// 			{
				static int member_uid_list[MAX_MEMBER_COUNT];
				memset(member_uid_list, 0, sizeof(member_uid_list));

				int member_count = guild->GetGuildMemberManager().GetMemberList(member_uid_list);
// 				for (int j = 0; j < member_count && j < MAX_MEMBER_COUNT; ++j)
// 				{
// 					Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[j]));
// 					if (NULL != temp_role && GUILD_POST_FU_TUANGZHANG == temp_role->GetGuildPost())
// 					{
//						is_find_fu_tuangzhang = true;

						//红包
//						temp_role->GetRoleGuild()->AddGuildRedPaperAuthority(PAPER_TYPE_GUILD_QUESTION, i + 1);
//						break;
// 					}
// 				}

				// 都不在线
// 				if (!is_find_fu_tuangzhang)
// 				{
// 					for (int k = 0; k < member_count && k < MAX_MEMBER_COUNT; ++k)
// 					{
// 						Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[k]));
// 						if (NULL != temp_role)
// 						{
// 							// 红包
// 							temp_role->GetRoleGuild()->AddGuildRedPaperAuthority(PAPER_TYPE_GUILD_QUESTION, i + 1);
// 							break;
// 						}
// 					}
// 				}

				// 邮件奖励
				const GuildQuestionRankRewardConfig *reward_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetRankRewardCfg(i + 1);
				if (NULL != reward_cfg)
				{
					// 工资奖励
					guild->AddGuildGongzi(reward_cfg->gongzi);

					if (reward_cfg->has_reward)
					{
						for (int index = 0; index < member_count && index < MAX_MEMBER_COUNT; ++index)
						{
							gamecommon::SendMultiMailToRole(IntToUid(member_uid_list[index]), reward_cfg->reward_item_list, reward_cfg->reward_item_count, NULL, false, "guild_question_reward_mail", i + 1);
						}
					}
				}
		}
	}

	// 传闻
	{
		int reward_bind_gold[NEED_BROADCAST_NUM]; memset(reward_bind_gold, 0, sizeof(reward_bind_gold));
		for (int i = 0; i < count && i < NEED_BROADCAST_NUM; ++i)
		{
			const GuildQuestionRankRewardConfig *reward_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetRankRewardCfg(i + 1);
			if (NULL != reward_cfg)
			{
				reward_bind_gold[i] = reward_cfg->reward_bind_gold;
			}
		}

		if (1 == count)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "guild_question_reward_notice1", guildname_list[0]);
			if (lenth > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
			}
		}
		else if (2 == count)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "guild_question_reward_notice2", guildname_list[0], guildname_list[1]);
			if (lenth > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
			}
		}
		else if (3 == count)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "guild_question_reward_notice3",
				guildname_list[0], guildname_list[1], guildname_list[2]);
			if (lenth > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
			}
		}

		{
			//正确答题数最多传闻
			if (!m_guild_question_person_rankvec.empty())
			{
				GuilldQuestionPersonRankVecIt person_it = m_guild_question_person_rankvec.begin();
				int sendlen = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "guildquestion_top_answer", person_it->guild_name, person_it->user_name);
				if (sendlen > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_ROLL_2);
				}

				m_guild_question_person_rankvec.clear();
			}
		}
	}

	// 群聊公告
	for(auto it : m_guild_question_question_map)
	{
		if (!it.second.is_aready_broadcast)
		{
			this->BroadcastGuildquestionEnd(it.second);
		}
	}
}

void ActivityShadow::BroadcastGuildquestionEnd(const GuildQuestionQuestion &question_info)
{
	const GuildQuestionOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg();
	int answer_right_rate = 0;
	if (other_cfg.question_total_num != 0)
	{
		answer_right_rate = question_info.question_right_num * 100 / other_cfg.question_total_num;
	}

	int rank = this->GetGuildQuestionGuildRank(question_info.guild_id);

	int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "guild_question_answer_max",
		answer_right_rate, rank);

	if (length > 0)
	{
		Guild *guild = GuildManager::Instance().GetGuild(question_info.guild_id);
		if (NULL != guild)
		{
			guild->GuildSystemMsg(length, gamestring::GAMESTRING_BUF);
		}
	}
}

void ActivityShadow::GuildQuestionQuickSetQuestion(GuildID guild_id)
{
	if (INVALID_GUILD_ID == guild_id)
	{
		return;
	}

	if (!this->IsActivtyOpen(ACTIVITY_TYPE_GUILD_QUESTION))
	{
		return;
	}

	// 必须要在答题状态，因为在准备状态是可以给到题目的
	if (GUILD_QUESTION_STATE_START == m_guild_question_state || GUILD_QUESTION_STATE_CLOSE == m_guild_question_state)
	{
		GuildQuestionQuestionMapIt it = m_guild_question_question_map.find(guild_id);
		if (it != m_guild_question_question_map.end())
		{
			return;
		}

		// 出题
		Guild *guild = GuildManager::Instance().GetGuild(guild_id);
		if (NULL != guild)
		{
			int question_id = LOGIC_CONFIG->GetGuildQuestionConfig().RandQuestionID();
			const GuildQuestionOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg();

			time_t now = EngineAdapter::Instance().Time();

			GuildQuestionQuestion ques;
			ques.guild_id = guild->GetGuildID();
			ques.question_id = question_id;
			ques.question_number = 1;
			ques.is_already_answer_true = false;
			ques.question_set.insert(question_id);
			ques.cur_question_timestamp = now;
			ques.next_question_timestamp = now + other_cfg.next_question_interval;

			m_guild_question_question_map[guild->GetGuildID()] = ques;

			this->SendGuildQuestionQuestionInfo(guild->GetGuildID());
		}
	}
	// 如果创建仙盟时还在准备状态,则只通知状态
	else if (GUILD_QUESTION_STATE_STANDY == m_guild_question_state)
	{
		this->SendGuildQuestionQuestionInfo(guild_id);
	}
}

void ActivityShadow::AddGuildQuestionInfo(Role *role)
{
	bool is_first_join = true; // 帮派是新加入答题

	for (GuildQuestionGuildRankVecIt it = m_guild_question_guild_rankvec.begin(); it != m_guild_question_guild_rankvec.end(); ++it)
	{
		if (role->GetGuildID() == it->guild_id)
		{
			is_first_join = false;
			break;
		}
	}

	if (is_first_join)
	{
		GuildQuestionGuildRank info;
		info.guild_id = role->GetGuildID();
		GuildShadow::Instance().GetGuildName(role->GetGuildID(), info.guild_name);
		info.answer_true_timestamp = 0;
		info.guild_score = 0;

		m_guild_question_guild_rankvec.push_back(info);
	}
}

CrossLieKunFBUser * ActivityShadow::GetCrossLieKunFBUser(long long uuid)
{
	CrossLieKunFBUserMapIt it = m_cross_liekunfb_user_map.find(uuid);
	if (it == m_cross_liekunfb_user_map.end())
	{
		CrossLieKunFBUser user;
		user.uuid = uuid;
		m_cross_liekunfb_user_map[uuid] = user;
	}

	if (it == m_cross_liekunfb_user_map.end())
	{
		it = m_cross_liekunfb_user_map.find(uuid);
	}

	if (it != m_cross_liekunfb_user_map.end())
	{
		return &it->second;
	}

	return NULL;
}

void ActivityShadow::ClearCrossLieKunFBUserGatherTimes(int zone, int boss_type)
{
	for (CrossLieKunFBUserMapIt it = m_cross_liekunfb_user_map.begin(); it != m_cross_liekunfb_user_map.end(); it++)
	{
		it->second.gather_times[zone][boss_type] = 0;
	}
}

void ActivityShadow::AddRACollectTreasureRecord(CollectTreasureRollRecord record_item)
{
	if (record_item.role_id == 0)
	{
		return;
	}
	m_collect_treasure_record_deq.push_back(record_item);
	
	if ((int)(m_collect_treasure_record_deq.size()) > RAND_ACTIVITY_COLLECT_TREASURE_MAX_RECORD_COUNT)
	{
		m_collect_treasure_record_deq.pop_front();
	}
}

void ActivityShadow::GetRACollectTreasureRecord(CollectTreasureRollRecord record_list[RAND_ACTIVITY_COLLECT_TREASURE_MAX_RECORD_COUNT], short &count)
{
	for (int i = 0; i < (int)(m_collect_treasure_record_deq.size()) && count < RAND_ACTIVITY_COLLECT_TREASURE_MAX_RECORD_COUNT; ++i)
	{
		record_list[count].role_id = m_collect_treasure_record_deq[i].role_id;
		F_STRNCPY(record_list[count].role_name, m_collect_treasure_record_deq[i].role_name, sizeof(GameName));
		record_list[count].roll_mul = m_collect_treasure_record_deq[i].roll_mul;

		++count;
	}
}

void ActivityShadow::CheckCrossBossRefreshNotice(time_t now)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

// 	if (now > m_next_refresh_crossboss_time)
// 	{
// 		// 启动时不通知. 只通知一个场景的就行
// 		if (0 != m_next_refresh_crossboss_time)
// 		{
// 			int scene_id = CrossBossConfig::Instance().GetSceneIdFirst();
// 			if (scene_id > 0)
// 			{
// 				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, CROSS_PUBLIC_SCENE_KEY);
// 				if (NULL != scene && Scene::SCENE_TYPE_CROSS_BOSS == scene->GetSceneType())
// 				{
// 					SpecialCrossBoss *cross_boss = dynamic_cast<SpecialCrossBoss*>(scene->GetSpecialLogic());
// 					if (NULL != cross_boss)
// 					{
// 						cross_boss->CreateBossBroadcast(scene->GetSceneID(), 0, 0, nullptr);
// 					}
// 				}
// 			}
// 		}
// 
// 		// 计算下一次的时间戳
// 		int interval = CrossBossConfig::Instance().GetCrossBossNextFlushTimeInterval((unsigned int)now);
// 		m_next_refresh_crossboss_time = now + interval;
// 	}

	// 如果BOSS是单个刷新的话就不能再这样进行提醒了
// 	if (now > m_next_refresh_cross_mizang_boss_time)
// 	{
// 		// 启动时不通知. 只通知一个场景的就行
// 		if (0 != m_next_refresh_cross_mizang_boss_time)
// 		{
// 			int scene_id = CrossMizangBossConfig::Instance().GetSceneIdFirst();
// 			if (scene_id > 0)
// 			{
// 				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, CROSS_PUBLIC_SCENE_KEY);
// 				if (NULL != scene && Scene::SCENE_TYPE_CROSS_MIZANG_BOSS == scene->GetSceneType())
// 				{
// 					SpecialCrossMiZangBoss *cross_boss = dynamic_cast<SpecialCrossMiZangBoss*>(scene->GetSpecialLogic());
// 					if (NULL != cross_boss)
// 					{
// 						cross_boss->CreateBossBroadcast(scene->GetSceneID(), 0, 0, nullptr);
// 					}
// 				}
// 			}
// 		}
// 
// 		// 计算下一次的时间戳
// 		int interval = CrossMizangBossConfig::Instance().GetCrossBossNextFlushTimeInterval((unsigned int)now);
// 		m_next_refresh_cross_mizang_boss_time = now + interval;
// 	}

// 	if (now > m_next_refresh_cross_youming_boss_time)
// 	{
// 		// 启动时不通知. 只通知一个场景的就行
// 		if (0 != m_next_refresh_cross_youming_boss_time)
// 		{
// 			int scene_id = CrossYoumingBossConfig::Instance().GetSceneIdFirst();
// 			if (scene_id > 0)
// 			{
// 				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, CROSS_PUBLIC_SCENE_KEY);
// 				if (NULL != scene && Scene::SCENE_TYPE_CROSS_YOUMING_BOSS == scene->GetSceneType())
// 				{
// 					SpecialCrossYouMingBoss *cross_boss = dynamic_cast<SpecialCrossYouMingBoss*>(scene->GetSpecialLogic());
// 					if (NULL != cross_boss)
// 					{
// 						cross_boss->CreateBossBroadcast(scene->GetSceneID(), 0, 0, nullptr);
// 					}
// 				}
// 			}
// 		}
// 
// 		// 计算下一次的时间戳
// 		int interval = CrossYoumingBossConfig::Instance().GetCrossBossNextFlushTimeInterval((unsigned int)now);
// 		m_next_refresh_cross_youming_boss_time = now + interval;
// 	}
}

bool ActivityShadow::GetImageCompetitionRankInfo(Role *role, int *rank_value, long long *flexible_ll)
{
	if (role == nullptr || rank_value == nullptr || flexible_ll == nullptr)
	{
		return false;
	}

	int rank_type = 0, rank_id = 0;

	WorldStatus2::Instance().GetImageCompetitionTypeAndId(&rank_type, &rank_id);

	if (rank_type <= IMAGE_COMPETITION_TYPE_INVALID || rank_type >= IMAGE_COMPETITION_TYPE_MAX)
	{
		return false;
	}

	switch (rank_type)
	{
	case IMAGE_COMPETITION_TYPE_MOUNT:
	{
		*rank_value = role->GetMountManager()->GetSpecialImageLevelById(rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_MOUNT);
	}
	break;

	case IMAGE_COMPETITION_TYPE_WING:
	{
		*rank_value = role->GetWing()->GetSpecialImageLevelById(rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_WING);
	}
	break;

	case IMAGE_COMPETITION_TYPE_FIGHT_MOUNT:
	{
		*rank_value = role->GetFightMountManager()->GetSpecialImageLevelById(rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_FIGHT_MOUNT);
	}
	break;

	case IMAGE_COMPETITION_TYPE_SHIZHUANG:
	{
		*rank_value = role->GetShizhuang()->GetSpecialImageLevelById(SHIZHUANG_TYPE_BODY, rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHIZHUANG);
	}
	break;

	case IMAGE_COMPETITION_TYPE_SHENBING:
	{
		*rank_value = role->GetShizhuang()->GetSpecialImageLevelById(SHIZHUANG_TYPE_WUQI, rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHIZHUANG_WUQI);
	}
	break;

	case IMAGE_COMPETITION_TYPE_FABAO:
	{
		*rank_value = role->GetFabaoManager()->GetSpecialImageLevelById(rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_FABAO);
	}
	break;

	case IMAGE_COMPETITION_TYPE_XIANNV_HALO:
	{
		*rank_value = role->GetShengong()->GetSpecialImageLevelById(rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENGONG);
	}
	break;

	case IMAGE_COMPETITION_TYPE_QILINBI:
	{
		*rank_value = role->GetQiLinBiManager()->GetSpecialImageLevelById(rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_QILINBI);
	}
	break;

	case IMAGE_COMPETITION_TYPE_TOUSHI:
	{
		*rank_value = role->GetTouShiManager()->GetSpecialImageLevelById(rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_TOUSHI);
	}
	break;

	case IMAGE_COMPETITION_TYPE_YAOSHI:
	{
		*rank_value = role->GetYaoShiManager()->GetSpecialImageLevelById(rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_YAOSHI);
	}
	break;

	case IMAGE_COMPETITION_TYPE_LINGTONG:
	{
		*rank_value = role->GetUpgradeManager()->GetSpecialImageLevelById(UPGRADE_TYPE_2, rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_2);
	}
	break;

	case IMAGE_COMPETITION_TYPE_LINGGONG:
	{
		*rank_value = role->GetUpgradeManager()->GetSpecialImageLevelById(UPGRADE_TYPE_3, rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_3);
	}
	break;

	case IMAGE_COMPETITION_TYPE_LINGMOUNT:
	{
		*rank_value = role->GetUpgradeManager()->GetSpecialImageLevelById(UPGRADE_TYPE_4, rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_4);
	}
	break;

	case IMAGE_COMPETITION_TYPE_HALO:
	{
		*rank_value = role->GetHalo()->GetSpecialImageLevelById(rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_HALO);
	}
	break;

	case IMAGE_COMPETITION_TYPE_FEICHONG:
	{
		*rank_value = role->GetUpgradeManager()->GetSpecialImageLevelById(UPGRADE_TYPE_8, rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_8);
	}
	break;

	case IMAGE_COMPETITION_TYPE_WEIYAN:
	{
		*rank_value = role->GetUpgradeManager()->GetSpecialImageLevelById(UPGRADE_TYPE_5, rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_5);
	}
	break;

	case IMAGE_COMPETITION_TYPE_FOOTPRINT:
	{
		*rank_value = role->GetFootPrint()->GetSpecialImageLevelById(rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_FOOTPRINT);
	}
	break;

	case IMAGE_COMPETITION_TYPE_SHENYI:
	{
		*rank_value = role->GetShenyi()->GetSpecialImageLevelById(rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENYI);
	}
	break;

	case IMAGE_COMPETITION_TYPE_MASK:
	{
		*rank_value = role->GetMaskManager()->GetSpecialImageLevelById(rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_MASK);
	}
	break;

	case IMAGE_COMPETITION_TYPE_WEIBA:
	{
		*rank_value = role->GetUpgradeManager()->GetSpecialImageLevelById(UPGRADE_TYPE_7, rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_7);
	}
	break;

	case IMAGE_COMPETITION_TYPE_SHOUHUAN:
	{
		*rank_value = role->GetUpgradeManager()->GetSpecialImageLevelById(UPGRADE_TYPE_6, rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_6);
	}
	break;

	case IMAGE_COMPETITION_TYPE_XIANNV:
	{
		*rank_value = role->GetXiannvManager()->GetSpecialImageLevelById(rank_id);
		*flexible_ll = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_XIANNV);
	}
	break;

	default:
		break;
	}

	return *rank_value > 0;
}

void ActivityShadow::OnImageCompetitionAccount()
{
	int rank_type = 0, rank_id = 0;

	WorldStatus2::Instance().GetImageCompetitionTypeAndId(&rank_type, &rank_id);

	if (rank_type <= IMAGE_COMPETITION_TYPE_INVALID || rank_type >= IMAGE_COMPETITION_TYPE_MAX)
	{
		return;
	}
	const int yesterday = -1;
	const RandActivityImageCompetition *img_competition_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetImageCompetitionStruct(yesterday);
	if (nullptr == img_competition_cfg)
	{
		return;
	}

	static const int IMAGE_COMPETITION_MAX_ROLE_NUM = 100;

	PersonRankItem user_item_list[IMAGE_COMPETITION_MAX_ROLE_NUM];
	int rank_count = RankManager::Instance().GetPersonRank()->GetRankTopItem(PERSON_RANK_TYPE_IMAGE_COMPETITION, IMAGE_COMPETITION_MAX_ROLE_NUM, user_item_list);

	for (int i = 0; i < rank_count && i < IMAGE_COMPETITION_MAX_ROLE_NUM; ++i)
	{
		if (INVALID_USER_ID == user_item_list[i].user_id)
		{
			continue;
		}
		const RandActivityImageCompetitionReward::RewardCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetCompetitionRankReward(i , user_item_list[i].rank_value);
		if (reward_cfg == nullptr) continue;

		if (i != reward_cfg->rank_pos)
		{
			gamecommon::SendMultiMailToRole(user_item_list[i].user_id, reward_cfg->reward_list, reward_cfg->reward_count, NULL, false, "image_competition_reward_mail_1", (int)img_competition_cfg->item_id, (int)user_item_list[i].rank_value, i + 1, reward_cfg->rank_pos + 1);
		}
		else
		{
			gamecommon::SendMultiMailToRole(user_item_list[i].user_id, reward_cfg->reward_list, reward_cfg->reward_count, NULL, false, "image_competition_reward_mail", (int)img_competition_cfg->item_id, (int)user_item_list[i].rank_value, i + 1, i + 1);
		}

		gamelog::g_log_roleactivity.printf(LL_INFO, "ActivityShadow::OnImageCompetitionAccount user[%d], rank_type[%d], rank_id[%d], rank_pos[%d], reward_1[%s], reward_2[%s], reward_3[%s]",
			UidToInt(user_item_list[i].user_id), rank_type, rank_id, i, ItemConfigDataLog(&reward_cfg->reward_list[0], NULL), 
			ItemConfigDataLog(&reward_cfg->reward_list[1], NULL), ItemConfigDataLog(&reward_cfg->reward_list[2], NULL));
	}

	{ // 结算完清空排行榜数据
		WorldStatus2::Instance().SetImageCompetitionTypeAndId(0, 0);
		RankManager::Instance().ClearPersonRankType(PERSON_RANK_TYPE_IMAGE_COMPETITION);

		// 如果活动还开，那就设置一下今天的排行类型
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_IMAGE_COMPETITION))
		{
			rank_type = 0; rank_id = 0;
			LOGIC_CONFIG->GetRandActivityCfg().GetImageCompetitionRankInfo(&rank_type, &rank_id);
			if (rank_type > IMAGE_COMPETITION_TYPE_INVALID && rank_type < IMAGE_COMPETITION_TYPE_MAX)
			{
				WorldStatus2::Instance().SetImageCompetitionTypeAndId(rank_type, rank_id);
			}
		}
	}
}

void ActivityShadow::SendImageCompetitionInfo(Role *role)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_IMAGE_COMPETITION))
	{
		return;
	}

	Protocol::SCImageCompetitionInfo ici;
	ici.opengame_day = LOGIC_CONFIG->GetRandActivityCfg().GetImageCompetitionOpengameDay();
	if (nullptr == role)
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&ici, sizeof(ici));
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ici, sizeof(ici));
	}
}

void ActivityShadow::OnCrossGuildSOSReq(Role * role, Protocol::CSSendGuildSosReq *sgsr)
{
	if (nullptr == role || nullptr == sgsr)
	{
		return;
	}

	Scene *scene = role->GetScene();
	if (nullptr == scene)
	{
		return;
	}

	if (!role->IsInCross())
	{
		return;
	}

	switch (sgsr->sos_type)
	{
	case GUILD_SOS_TYPE_CROSS_BOSS:
	{
		SpecialCrossBoss *special_logic = dynamic_cast<SpecialCrossBoss *>(scene->GetSpecialLogic());
		if (nullptr == special_logic || Scene::SCENE_TYPE_CROSS_BOSS != special_logic->GetSceneType())
		{
			return;
		}
	}
	break;

	case GUILD_SOS_TYPE_CROSS_MIZANG_BOSS:
	{
		SpecialCrossMiZangBoss *special_logic = dynamic_cast<SpecialCrossMiZangBoss *>(scene->GetSpecialLogic());
		if (nullptr == special_logic || Scene::SCENE_TYPE_CROSS_MIZANG_BOSS != special_logic->GetSceneType())
		{
			return;
		}
	}
	break;

	case GUILD_SOS_TYPE_CROSS_VIP_BOSS:
	{
		if (!Scene::IsBossFamilyScene(scene->GetSceneID()))
		{
			return;
		}
	}
	break;

	default:
		return;
	}

	UniqueUserID role_id = role->GetRoleCross()->GetCrossOriginalUniqueUserID();

	crossgameprotocal::GameGameCrossGuildBattleSos pro;
	pro.guild_id = role->GetGuildID();
	pro.member_uid = UidToInt(role_id.user_id);
	role->GetName(pro.member_name);
	pro.member_camp = role->GetCamp();
	pro.member_scene_id = role->GetScene()->GetSceneID();
	pro.member_scene_key = role->GetScene()->GetSceneKey();
	pro.member_pos_x = role->GetPos().x;
	pro.member_pos_y = role->GetPos().y;
	UNSTD_STATIC_CHECK(MAX_SCENE_NAME_LENGTH == sizeof(pro.member_scene_name))
	F_STRNCPY(pro.member_scene_name, role->GetScene()->GetSceneName(), sizeof(pro.member_scene_name));
	pro.sos_type = sgsr->sos_type;
	pro.sos_sysmsg = 1;

	InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID, UniqueUserIDToLongLong(role_id), (const char *)&pro, sizeof(pro));
}

void ActivityShadow::OnCrossAddGuildEnemy(Role *be_killer, Role *killer)
{
	// 目前是跨服才生效
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (nullptr == killer || nullptr == be_killer)
	{
		return;
	}

	// 20190418需求-同一个服的不能算
	if (killer->GetRoleCross()->GetOriginPlatType() == be_killer->GetRoleCross()->GetOriginPlatType() &&
		killer->GetRoleCross()->GetOriginMergeServerID() == be_killer->GetRoleCross()->GetOriginMergeServerID())
	{
		return;
	}

	// 死者没有公会不能算
	GuildID be_killer_guild_id = be_killer->GetGuildID();
	if (be_killer_guild_id <= 0)
	{
		return;
	}

	// 同一个公会不能算
	if (this->GetGuildUniqueUserId(be_killer) == this->GetGuildUniqueUserId(killer))
	{
		return;
	}

	long long killer_uuid = UniqueUserIDToLongLong(killer->GetUniqueUserID());
	long long be_killer_uuid = UniqueUserIDToLongLong(be_killer->GetUniqueUserID());
	
	// 发送给原服
	crossgameprotocal::GameGameCrossGuildMemberBeKill ggcgmbk;
	ggcgmbk.killer_uuid = killer_uuid;
	killer->GetName(ggcgmbk.killer_name);
	ggcgmbk.be_killer_uuid = be_killer_uuid;
	ggcgmbk.be_killer_guild_id = be_killer_guild_id;

	InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID, be_killer_uuid, (const char *)&ggcgmbk, sizeof(ggcgmbk));
}

void ActivityShadow::OnGetGuildEnemyListFromOriginal(Role *role)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (nullptr == role)
	{
		return;
	}

	GuildID guild_id = role->GetGuildID();
	if (guild_id <= 0)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());

	// 发送给原服
	crossgameprotocal::CrossGameGetGuildEnemyList cgggel;
	cgggel.request_uuid = uuid;
	cgggel.guild_id = guild_id;
	InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID, uuid, (const char *)&cgggel, sizeof(cgggel));

}

