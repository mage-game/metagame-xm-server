#include "rankmanager.hpp"
#include "servercommon/rankdef.hpp"

#include "global/rmibackobjdef.h"
#include "engineadapter.h"
#include "internalcomm.h"

#include "serverlogic.h"
#include "world.h"
#include "gameworld/global/rank/topranker.hpp"

//#include "globalserver/guild/guild.hpp"

#include "servercommon/string/globalstr.h"
#include "servercommon/serverconfig/crossconfig.hpp"

#include "obj/character/role.h"
#include "global/worldstatus/worldstatus.hpp"
#include "global/usercache/usercache.hpp"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/cross/rolecross.h"

RankManager::RankManager()
	: m_rank_data_next_save_time(0)
{
	m_person_rank.SetRankManager(this);
	//m_team_rank.SetRankManager(this);
	m_guild_rank.SetRankManager(this);
}	

RankManager::~RankManager()
{

}

RankManager & RankManager::Instance()
{
	static RankManager umm;
	return umm;
}

void RankManager::OnServerStart()
{
	{
		RMIInitPersonRankBackObjectImpl *backobj = new RMIInitPersonRankBackObjectImpl();

		RMIGlobalClient ac;
		ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
		ac.InitPersonRankAsyn(backobj);
	}
	
	{
		/*RMIInitTeamRankBackObjectImpl *backobj = new RMIInitTeamRankBackObjectImpl();

		RMIGlobalClient ac;
		ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
		ac.InitTeamRankAsyn(backobj);*/
	}
}

void RankManager::OnServerStop()
{
	this->Save();
}

void RankManager::Update(unsigned long interval, time_t now_second)
{
	if (!this->IsLoadFinish()) return;

	if (m_rank_data_next_save_time > 0 && now_second > m_rank_data_next_save_time) 
	{
		this->Save();
		m_rank_data_next_save_time = EngineAdapter::Instance().Time() + RANK_DATA_SAVE_TO_DB_TIME;
	}

	m_person_rank.Update(now_second);
	//m_team_rank.Update(now_second);
	m_guild_rank.Update(now_second);
}

void RankManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{	
	World::GetInstWorld()->GetSceneManager()->ForceAllOnlineRoleSyncData();

	PersonRankItem * charm_top1_item = RankManager::Instance().GetPersonRank()->GetTopOneItemByRankType(PERSON_RANK_TYPE_DAY_CHARM);
	if (NULL != charm_top1_item)
	{
		if (WorldStatus::Instance().GetCommonData().charm_top1_title_uid != UidToInt(charm_top1_item->user_id))
		{
			int old_charm_top1_uid = WorldStatus::Instance().GetCommonData().charm_top1_title_uid;
			WorldStatus::Instance().GetCommonData().charm_top1_title_uid = UidToInt(charm_top1_item->user_id);

			Role * charm_top1_role = World::GetInstWorld()->GetSceneManager()->GetRole(charm_top1_item->user_id);
			if (NULL != charm_top1_role)
			{
				charm_top1_role->GetTitleManager()->OnChangeCharmRankTitle(SYNC_REASON_RANK);
			}

			Role * old_charm_top1_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_charm_top1_uid));
			if (NULL != old_charm_top1_role)
			{
				old_charm_top1_role->GetTitleManager()->OnChangeCharmRankTitle(SYNC_REASON_RANK);
			}

			UserCacheNode * charm_top1_node = UserCacheManager::Instance().GetUserNode(charm_top1_item->user_id);
			if (NULL != charm_top1_node)
			{
				static MailContentParam contentparam;
				contentparam.Reset();
				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_man_charm_rank_title);
				if (FEMALE == charm_top1_node->sex)
				{
					length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_girl_charm_rank_title);
				}

				if (length > 0)
				{
					MailRoute::MailToUser(charm_top1_item->user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}

			UserCacheNode * old_charm_top1_node = UserCacheManager::Instance().GetUserNode(old_charm_top1_uid);
			if (NULL != old_charm_top1_node)
			{
				static MailContentParam contentparam;
				contentparam.Reset();
				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_lose_charm_rank_title);

				if (length > 0)
				{
					MailRoute::MailToUser(IntToUid(old_charm_top1_uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
	}

	PersonRankItem *world_question_top1_item = RankManager::Instance().GetPersonRank()->GetTopOneItemByRankType(PERSON_RANK_TYPE_WORLD_RIGHT_ANSWER);
	if (NULL != world_question_top1_item)
	{
		if (WorldStatus::Instance().GetCommonData().world_question_top1_title_uid != UidToInt(world_question_top1_item->user_id))
		{
			int old_world_question_top1_uid = WorldStatus::Instance().GetCommonData().world_question_top1_title_uid;
			WorldStatus::Instance().GetCommonData().world_question_top1_title_uid = UidToInt(world_question_top1_item->user_id);

			Role *world_question_top1_role = World::GetInstWorld()->GetSceneManager()->GetRole(world_question_top1_item->user_id);
			if (NULL != world_question_top1_role)
			{
				world_question_top1_role->GetTitleManager()->OnChangeWorldQuestionRankTitle(SYNC_REASON_RANK);
			}

			Role *old_world_question_top1_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_world_question_top1_uid));
			if (NULL != old_world_question_top1_role)
			{
				old_world_question_top1_role->GetTitleManager()->OnChangeWorldQuestionRankTitle(SYNC_REASON_RANK);
			}

			MailContentParam contentparam;
			contentparam.Reset();
			UserCacheNode *wold_question_top1_node = UserCacheManager::Instance().GetUserNode(world_question_top1_item->user_id);
			if (NULL != wold_question_top1_node)
			{
				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_add_world_question_rank_title);
				if (length > 0)
				{
					MailRoute::MailToUser(world_question_top1_item->user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}

			contentparam.Reset();
			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_lose_world_question_rank_title);
			if (length > 0)
			{
				MailRoute::MailToUser(IntToUid(old_world_question_top1_uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}

	{
		this->ClearPersonRankType(PERSON_RANK_TYPE_RED_PAPER_LUCKY_VALUE); // 红包幸运榜需要每天清零
		this->ClearPersonRankType(PERSON_RANK_TYPE_WORLD_RIGHT_ANSWER);	 // 每日答题榜需要每天清零
		this->ClearPersonRankType(PERSON_RANK_TYPE_DAY_CHARM);			 // 每日魅力榜需要每天清零
	}
}

void RankManager::OnWeekChange()
{
	/*this->HonourRankReward();
	this->Rank1v1Reward();

	m_person_rank.ClearRank(PERSON_RANK_TYPE_HONOUR);
	m_person_rank.ClearRank(PERSON_RANK_TYPE_1V1);*/
}

void RankManager::OnUserLogin(Role *user)
{
	m_person_rank.OnUserLogin(user);

	TopRanker::Instance().OnUserLogin(user);
}

void RMIInitPersonRankBackObjectImpl::InitPersonRankRet(int ret, const PersonRankParam &person_rank_param)
{
	if (ret < 0) 
	{
		printf("RMIInitPersonRankBackObjectImpl::Init Failed \n");
		ServerLogic::GetInstServerLogic()->StopServer();
	}
	else 
	{
		PersonRank *person_rank = RankManager::Instance().GetPersonRank();
		if (NULL == person_rank) return;

		person_rank->Init(person_rank_param);
		RankManager::Instance().LoadPersonRankSucc();
	}
}

void RMIInitTeamRankBackObjectImpl::InitTeamRankRet(int ret, const TeamRankParam &team_rank_param)
{
	if (ret < 0) 
	{
		printf("RMIInitTeamRankBackObjectImpl::Init Failed \n");
		ServerLogic::GetInstServerLogic()->StopServer();
	}
	else 
	{
		/*TeamRank *team_rank = RankManager::Instance().GetTeamRank();
		if (NULL == team_rank) return;

		team_rank->Init(team_rank_param);
		RankManager::Instance().LoadTeamRankSucc();*/
	}
}

void RankManager::LoadPersonRankSucc()
{
	if (this->IsLoadFinish()) 
	{
		this->LoadAllRankSucc();
	}
}

void RankManager::LoadTeamRankSucc()
{
	if (this->IsLoadFinish())
	{
		this->LoadAllRankSucc();
	}
}

void RankManager::LoadAllRankSucc()
{
	m_rank_data_next_save_time = EngineAdapter::Instance().Time() + RANK_DATA_SAVE_TO_DB_TIME;
}

void RankManager::Save()
{
	if (!this->IsLoadFinish()) return;

	{
		static PersonRankParam person_rank_param;

		m_person_rank.GetChangeRankItemList(&person_rank_param);
		if (0 != person_rank_param.count)
		{
			RMISavePersonRankBackObjectImpl *backobj = new RMISavePersonRankBackObjectImpl();
			RMIGlobalClient ac;
			ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());

			bool ret = ac.SavePersonRankAsyn(person_rank_param, backobj);
			if (ret)
			{
				m_person_rank.ClearDirtyMark();
			}
		}
	}
	
	{
		/*static TeamRankParam team_rank_param;

		m_team_rank.GetChangeRankItemList(&team_rank_param);
		if (0 != team_rank_param.count)
		{
			RMISaveTeamRankBackObjectImpl *backobj = new RMISaveTeamRankBackObjectImpl();
			RMIGlobalClient ac;
			ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());

			bool ret = ac.SaveTeamRankAsyn(team_rank_param, backobj);
			if (ret)
			{
				m_team_rank.ClearDirtyMark();
			}
		}*/
	}
}

void RankManager::OnGetPersonRankList(Role *user, Protocol::CSGetPersonRankList *rank_list_req)
{
	if (NULL == user || NULL == rank_list_req) return;

	m_person_rank.OnGetRankList(user, *rank_list_req);

	if (CrossConfig::Instance().IsHiddenServer())
	{
		static crossgameprotocal::CrossGameRankInfoReq cgrir;
		cgrir.uuid = UniqueUserIDToLongLong(user->GetRoleCross()->GetCrossOriginalUniqueUserID());
		cgrir.is_personal_rank = 1;
		cgrir.is_personal_rank_top_user = 0;
		cgrir.rank_type = rank_list_req->rank_type;

		InternalComm::Instance().SendToCross((const char *)&cgrir, sizeof(cgrir));
	}
}

void RankManager::OnGetPersonRankTopUser(Role *user, Protocol::CSGetPersonRankTopUser *top_user_req)
{
	if (NULL == user || NULL == top_user_req) return;

	m_person_rank.OnGetRankTopUser(user, *top_user_req);

	if (CrossConfig::Instance().IsHiddenServer())
	{
		static crossgameprotocal::CrossGameRankInfoReq cgrir;
		cgrir.uuid = UniqueUserIDToLongLong(user->GetRoleCross()->GetCrossOriginalUniqueUserID());
		cgrir.is_personal_rank = 1;
		cgrir.is_personal_rank_top_user = 1;
		cgrir.rank_type = top_user_req->rank_type;

		InternalComm::Instance().SendToCross((const char *)&cgrir, sizeof(cgrir));
	}
}

void RankManager::SyncPersonBaseInfo(Role *user)
{
	if (NULL == user) return;

	m_person_rank.OnBaseInfoChange(user);
}

void RankManager::OnSyncPersonRankInfo(const UserID &user_id, SyncPersonRankInfo *person_rank_info)
{
	if (NULL == person_rank_info) return;

	m_person_rank.OnRankInfoChange(user_id, *person_rank_info);
}

void RankManager::OnGetGuildRankList(Role *user, Protocol::CSGetGuildRankList *rank_list_req)
{
	if (NULL == user || NULL == rank_list_req) return;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		static crossgameprotocal::CrossGameRankInfoReq cgrir;
		cgrir.uuid = UniqueUserIDToLongLong(user->GetRoleCross()->GetCrossOriginalUniqueUserID());
		cgrir.is_personal_rank = 0;
		cgrir.is_personal_rank_top_user = 0;
		cgrir.rank_type = rank_list_req->rank_type;

		InternalComm::Instance().SendToCross((const char *)&cgrir, sizeof(cgrir));
	}
	else
	{
		m_guild_rank.OnGetRankList(user, *rank_list_req);
	}
}

void RankManager::OnGetTeamRankList(Role *user, Protocol::CSGetTeamRankList *rank_list_req)
{
	if (NULL == user || NULL == rank_list_req) return;

	//m_team_rank.OnGetRankList(user, *rank_list_req);
}

void RankManager::SyncTeamRankInfo(gglobalprotocal::SyncTeamRankInfo *team_rank_info)
{
	if (NULL == team_rank_info) return;

	//m_team_rank.OnRankInfoChange(*team_rank_info);
}

void RankManager::SyncPersonRankBattleFieldRankInfo(gglobalprotocal::SyncLastBattleFieldRankInfo *last_battlefield_rank_info)
{
	if (NULL == last_battlefield_rank_info) return;
	m_person_rank.OnLastBattleFieldRankChange(*last_battlefield_rank_info);
}

void RankManager::SyncGuildRankInfo(gglobalprotocal::SyncGuildRankInfo *guild_rank_info)
{
	if (NULL == guild_rank_info) return;
	m_guild_rank.OnGuildRankInfoChange(*guild_rank_info);
}

void RankManager::OnCrossRankInfoReq(crossgameprotocal::CrossGameRankInfoReq *cross_rank_info_req)
{
	if (NULL == cross_rank_info_req) return;

	if (0 != cross_rank_info_req->is_personal_rank)
	{
		m_person_rank.OnCrossRankInfoReq(*cross_rank_info_req);
	}
	else
	{
		m_guild_rank.OnCrossRankInfoReq(*cross_rank_info_req);
	}
}

void RankManager::Snapshot()
{
	m_person_rank.Snapshot();
	m_guild_rank.Snapshot();
}

void RankManager::ClearPersonRankType(int rank_type)
{
	m_person_rank.ClearRank(rank_type);
}

void RankManager::ClearGuildRankType(int rank_type)
{
	m_guild_rank.ClearRank(rank_type);
}

bool RankManager::IsLoadFinish()
{
	return m_person_rank.IsRankLoadFinish() /*&& m_team_rank.IsRankLoadFinish()*/;
}

//void RankManager::HonourRankReward()
//{
//	int reward_count = GLOBAL_CONFIG->GetRankConfig()->GetHonourRankRewardCount();
//	if (reward_count > RankConfig::MAX_HONOUR_RANK_REWARD_COUNT) return;
//
//	UserID user_id_list[RankConfig::MAX_HONOUR_RANK_REWARD_COUNT];
//
//	int count = m_person_rank.GetRankTopUser(PERSON_RANK_TYPE_HONOUR, reward_count, user_id_list);
//
//	for (int i = 0; i < count && i < reward_count; ++i)
//	{
//		ItemConfigData *reward_item = GLOBAL_CONFIG->GetRankConfig()->GetHonourRankRewardItem(i);
//		if (NULL == reward_item) continue;
//
//		static MailContentParam contentparam; contentparam.Reset();
//
//		contentparam.item_list[0].item_id = reward_item->item_id;
//		contentparam.item_list[0].is_bind = reward_item->is_bind ? 1 : 0;
//		contentparam.item_list[0].num = reward_item->num;
//
//		int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_honour_rank_reward_subject);
//		int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_honour_rank_reward_content, i + 1, reward_item->item_id);
//		if (length1 > 0 && length2 > 0)
//		{
//			GlobalServer::GetInstGlobalServer()->GetUserMailManager()->MailToUser(MAIL_TYPE_SYSTEM, user_id_list[i], SYSTEM_MAIL_REASON_INVALID, contentparam);
//		}
//	}
//}
//
//void RankManager::Rank1v1Reward()
//{
//	static const int MAX_REWARD_COUNT = 100;
//
//	UserID user_id_list[MAX_REWARD_COUNT];
//	int count = m_person_rank.GetRankTopUser(PERSON_RANK_TYPE_1V1, MAX_REWARD_COUNT, user_id_list);
//
//	for (int i = 0; i < count && i < MAX_REWARD_COUNT; ++i)
//	{
//		if (0 == i)
//		{
//			const PersonRankItem *temp_rank_item = this->GetPersonRank()->GetPersonRankItem(PERSON_RANK_TYPE_1V1, user_id_list[0], 0);
//			if (NULL == temp_rank_item || temp_rank_item->rank_value <= 1000) 
//			{
//				break;
//			}
//		}
//
//		int rank = i + 1;
//
//		int reward_honor = GLOBAL_CONFIG->GetRankConfig()->Get1v1RewardHonor(rank);
//		if (reward_honor <= 0) 
//		{
//			break;
//		}
//
//		static MailContentParam contentparam; contentparam.Reset();
//
//		contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_BATTLEFIELDHONOR] = reward_honor;
//
//		int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_1v1_rank_reward_subject);
//		int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_1v1_rank_reward_content, rank, reward_honor);
//		if (length1 > 0 && length2 > 0)
//		{
//			GlobalServer::GetInstGlobalServer()->GetUserMailManager()->MailToUser(MAIL_TYPE_SYSTEM, user_id_list[i], SYSTEM_MAIL_REASON_INVALID, contentparam);
//		}
//	}
//}

