#include "spacemanager.hpp"

#include "world.h"

#include "engineadapter.h"
#include "global/rmibackobjdef.h"
#include "internalcomm.h"

#include "obj/character/role.h"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/commondata.hpp"

#include "global/usercache/usercache.hpp"

#include "global/offlineregister/offlineregistermanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/spaceconfig.hpp"
#include "friend/friendmanager.h"
#include "item/knapsack.h"

SpaceManager::SpaceManager()
	: m_data_state(SPACE_STATE_INVALID), m_last_save_time(0), m_last_remove_time(0), inc_seq(0)
{
	memset(&m_change_param, 0, sizeof(m_change_param));	
}

SpaceManager::~SpaceManager()
{

}

SpaceManager & SpaceManager::Instance()
{
	static SpaceManager sm;
	return sm;
}

bool SpaceManager::IsLoadFinish()
{
	return (SPACE_STATE_LOAD_FINISH == m_data_state);
}

void SpaceManager::SyncSpaceMsgInfoToRole(Role *role, int targe_uid, int msg_type, bool is_new_notice)
{
	if (NULL == role) 
	{
		return;
	}

	static Protocol::SCRoleSpaceMsgInfo rsmi;
	rsmi.type = msg_type;
	rsmi.target_uid = targe_uid;
	rsmi.msg_count = 0;
	rsmi.is_new_notice = is_new_notice;
	rsmi.last_seek_gift_timestamp = role->GetCommonDataParam()->space_last_seek_gift_timestamp;
	rsmi.last_seek_msg_timestamp = role->GetCommonDataParam()->space_last_seek_msg_timestamp;

	std::map<long long, SpaceRecord> *record_map = this->GetUserTypeRecordMap(targe_uid, msg_type);
	if (NULL != record_map)
	{
		for (std::map<long long, SpaceRecord>::iterator s_it = record_map->begin(); s_it != record_map->end(); ++ s_it)
		{
			SpaceRecord &record = s_it->second;
			
			if (is_new_notice)  //如果是新通知，则查看过的不再发
			{	
				if (Protocol::SPACE_MSG_TYPE_GIFT == msg_type && role->GetCommonDataParam()->space_last_seek_gift_timestamp >= record.happen_timestamp)
				{
					continue;
				}

				if (Protocol::SPACE_MSG_TYPE_LIUYAN == msg_type && role->GetCommonDataParam()->space_last_seek_msg_timestamp >= record.happen_timestamp)
				{
					continue;
				}
			}

			if (record.type == msg_type && rsmi.msg_count < SPACE_MESSAGE_MAX)  
			{
				rsmi.msg_list[rsmi.msg_count].record_unq_id = record.record_unq_id;
				rsmi.msg_list[rsmi.msg_count].owner_uid = record.owner_uid;
				rsmi.msg_list[rsmi.msg_count].from_uid = record.from_uid;
				rsmi.msg_list[rsmi.msg_count].to_uid = record.to_uid;
				rsmi.msg_list[rsmi.msg_count].happen_timestamp = record.happen_timestamp;
				rsmi.msg_list[rsmi.msg_count].happen_space_uid = record.happen_space_uid;
				rsmi.msg_list[rsmi.msg_count].type = record.type;
				rsmi.msg_list[rsmi.msg_count].is_huifu = record.is_huifu;

				F_STRNCPY(rsmi.msg_list[rsmi.msg_count].msg, record.msg, sizeof(SpaceMessage));

				UserCacheNode *owner_cache = UserCacheManager::Instance().GetUserNode(record.owner_uid);
				if (NULL != owner_cache)
				{
					owner_cache->GetName(rsmi.msg_list[rsmi.msg_count].owner_name);
				}
				UserCacheNode *from_cache = UserCacheManager::Instance().GetUserNode(record.from_uid);
				if (NULL != from_cache)
				{
					from_cache->GetName(rsmi.msg_list[rsmi.msg_count].from_name);

					rsmi.msg_list[rsmi.msg_count].from_prof = from_cache->prof;
					rsmi.msg_list[rsmi.msg_count].from_sex = from_cache->sex;
					rsmi.msg_list[rsmi.msg_count].from_avatar_timestamp = from_cache->avatar_timestamp;
				}
				UserCacheNode *to_cache = UserCacheManager::Instance().GetUserNode(record.to_uid);
				if (NULL != to_cache)
				{
					to_cache->GetName(rsmi.msg_list[rsmi.msg_count].to_name);
				}

				UserCacheNode *happen_space_ower_cache = UserCacheManager::Instance().GetUserNode(record.happen_space_uid);
				if (NULL != happen_space_ower_cache)
				{
					happen_space_ower_cache->GetName(rsmi.msg_list[rsmi.msg_count].happen_space_ower_name);
				}

				++ rsmi.msg_count;
			}
		}
	}

	int sendlen = sizeof(rsmi) - (SPACE_MESSAGE_MAX - rsmi.msg_count) * sizeof(rsmi.msg_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&rsmi, sendlen);
}

void SpaceManager::OnGetRoleSpaceMsgInfo(Role *role, Protocol::CSGetRoleSpaceMsgInfo *grsmi)
{
	if (NULL == role || grsmi->target_uid <= 0) return;
	if (grsmi->msg_type <= Protocol::SPACE_MSG_TYPE_INVALID || grsmi->msg_type >= Protocol::SPACE_MSG_TYPE_MAX) return;
	
	if (0 != grsmi->is_seek_byhand)
	{
		if (Protocol::SPACE_MSG_TYPE_LIUYAN == grsmi->msg_type)
		{
			role->GetCommonDataParam()->space_last_seek_msg_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		}
		else if (Protocol::SPACE_MSG_TYPE_GIFT == grsmi->msg_type)
		{
			role->GetCommonDataParam()->space_last_seek_gift_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		}
	}

	this->SyncSpaceMsgInfoToRole(role, grsmi->target_uid, grsmi->msg_type, false);
}

void SpaceManager::OnKeyCaiAllFriend(Role *role)
{
	if (NULL == role) return;

	FriendListParam friend_list_param;
	role->GetFriendManager()->GetFriendList(&friend_list_param);

	static Protocol::CSSpaceOper cmd;
	
	for (int i = 0; i < friend_list_param.count; ++ i)
	{
		cmd.who_space_uid = friend_list_param.friend_list[i].friend_uid;
		cmd.from_uid = role->GetUID();
		cmd.to_uid = cmd.who_space_uid;
		cmd.msg_type = Protocol::SPACE_MSG_TYPE_CAI;
		cmd.is_huifu = false;
		this->OnOperSpace(role, &cmd, false);
	}
}

void SpaceManager::OnOperSpace(Role *role, Protocol::CSSpaceOper *so, bool is_notify)
{
	if (NULL == role || NULL == so || so->who_space_uid <= 0) return;
	if (so->msg_type <= Protocol::SPACE_MSG_TYPE_INVALID || so->msg_type >= Protocol::SPACE_MSG_TYPE_MAX) return;

	UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(so->to_uid);
	if (NULL == user_node)
	{
		return;
	}

	if (role->GetLevel() < 199)
	{
		role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
		return;
	}

	if (!role->GetFriendManager()->IsFriend(IntToUid(so->to_uid)) && (role->GetUID() != so->to_uid))
	{
		return;
	}

	static SpaceRecord sr;
	sr.record_unq_id = this->GetRecordUnqiueID();

	sr.happen_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	sr.happen_space_uid = so->who_space_uid;

	sr.type = so->msg_type;
	sr.is_huifu = so->is_huifu;
	F_STRNCPY(sr.msg, so->space_msg, sizeof(SpaceMessage));

	sr.owner_uid = so->who_space_uid;
	sr.from_uid = so->from_uid;
	sr.to_uid = so->to_uid;

	if (Protocol::SPACE_MSG_TYPE_LIUYAN == so->msg_type) // 留言 && 回复
	{
		this->AddSpaceRecord(so->who_space_uid, sr);
		
		//通知空间主人
		Role *who_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(so->who_space_uid));
		if (NULL != who_role)
		{
			this->SyncSpaceMsgInfoToRole(who_role, who_role->GetUID(), Protocol::SPACE_MSG_TYPE_LIUYAN, true);
		}
		
		if (0 != so->is_huifu && so->who_space_uid != so->to_uid) // 被回复的人不是空间主人 给被回复的人空间发一条消息
		{
			sr.record_unq_id = this->GetRecordUnqiueID(); 
			sr.owner_uid = so->to_uid;
			this->AddSpaceRecord(so->to_uid, sr);

			//通知被回复的人
			Role *woner_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(so->who_space_uid));
			if (NULL != woner_role)
			{
				this->SyncSpaceMsgInfoToRole(woner_role, woner_role->GetUID(), Protocol::SPACE_MSG_TYPE_LIUYAN, true);
			}
		}
	}
	else if (Protocol::SPACE_MSG_TYPE_CAI == so->msg_type)
	{
		if (!this->CheckCanCai(role, so->who_space_uid))
		{
			if (is_notify)
			{
				role->NoticeNum(errornum::EN_TODAY_U_CAI_TARGET_LIMIT);
			}

			return;
		}

		role->SpaceCaiOrBeCai(1, true);

		Role *who_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(so->who_space_uid));
		if (NULL != who_role)
		{
			who_role->SpaceCaiOrBeCai(1, false);
		}
		else 
		{
			OfflineRegisterManager::Instance().OnSpaceBeCai(IntToUid(so->who_space_uid));
		}

		this->AddSpaceRecord(so->who_space_uid, sr);

		if (is_notify)
		{
			role->NoticeNum(noticenum::NT_SPACE_CAI_SUCC);
		}
	}
	else if (Protocol::SPACE_MSG_TYPE_GIFT == so->msg_type)
	{
		const int gift_cost = LOGIC_CONFIG->GetSpaceConfig().GetGiftCost();
		if (!role->GetKnapsack()->GetMoney()->UseGold(gift_cost, "SpaceGift"))
		{
			if (is_notify)
			{
				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			}

			return;
		}

		role->SpaceGiftOrBeGift(1, true);

		Role *who_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(so->who_space_uid));
		if (NULL != who_role)
		{
			who_role->SpaceGiftOrBeGift(1, false);
		}
		else 
		{
			OfflineRegisterManager::Instance().OnSpaceBeGift(IntToUid(so->who_space_uid));
		}

		this->AddSpaceRecord(so->who_space_uid, sr);
		
		//通知收礼的人
		Role *woner_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(so->who_space_uid));
		if (NULL != woner_role)
		{
			this->SyncSpaceMsgInfoToRole(woner_role, woner_role->GetUID(), Protocol::SPACE_MSG_TYPE_GIFT, true);
		}

		if (is_notify)
		{
			role->NoticeNum(noticenum::NT_PRESENT_SUCC);
		}
	}
	else if (Protocol::SPACE_MSG_TYPE_XINQING == so->msg_type)
	{
		role->UpdateSpaceXinqing(so->space_msg);

		role->NoticeNum(errornum::EN_UPDATE_SPACE_XINQING_SUCC);
	}
}

void SpaceManager::OnRemoveRecord(Role *role, Protocol::CSSpaceRemoveRecord *srr)
{
	if (NULL == role) return;
	if (srr->msg_type != Protocol::SPACE_MSG_TYPE_LIUYAN) return;

	std::map<long long, SpaceRecord> *record_map = this->GetUserTypeRecordMap(role->GetUID(), srr->msg_type);
	if (NULL != record_map)
	{
		std::map<long long, SpaceRecord>::iterator it = record_map->find(srr->unq_id);
		if (it != record_map->end())
		{
			this->OnSpaceRecordChange(it->second, structcommon::CS_DELETE);

			record_map->erase(it);

			role->NoticeNum(errornum::EN_SPACE_REMOVE_RECORD_SUCC);
		}
	}

	Protocol::CSGetRoleSpaceMsgInfo grsmi;
	grsmi.msg_type = srr->msg_type;
	grsmi.target_uid = role->GetUID();
	this->OnGetRoleSpaceMsgInfo(role, &grsmi);
}

void SpaceManager::OnServerStart()
{
	this->LoadSpaceRecord(0);	
}

void SpaceManager::OnServerStop()
{
	this->CheckSaveSpaceRecord(EngineAdapter::Instance().Time() + MAX_SAVE_SPACE_INTERVAL * 2); 
}

void SpaceManager::Update(unsigned long interval, time_t now_second)
{
	if (SPACE_STATE_LOAD_FINISH != m_data_state) return;

	{
		if (now_second >= m_last_remove_time + MAX_REMOVE_SPACE_INTERVAL)
		{
			for (UserSpaceRecordMapIt it = m_user_record_map.begin(); it != m_user_record_map.end(); ++ it)
			{
				UserSpaceRecord &user_spacerecord = it->second;

				{
					std::map<long long, SpaceRecord> &liuyan_map = user_spacerecord.liuyan_map;
					for (std::map<long long, SpaceRecord>::iterator s_it = liuyan_map.begin(); s_it != liuyan_map.end();)
					{
						SpaceRecord &record = s_it->second;
						if (record.CanRemove(static_cast<unsigned int>(now_second)))
						{
							this->OnSpaceRecordChange(record, structcommon::CS_DELETE);

							liuyan_map.erase(s_it++);
						}
						else
						{
							++ s_it;
						}
					}
				}

				{
					std::map<long long, SpaceRecord> &cai_map = user_spacerecord.cai_map;
					for (std::map<long long, SpaceRecord>::iterator s_it = cai_map.begin(); s_it != cai_map.end();)
					{
						SpaceRecord &record = s_it->second;
						if (record.CanRemove(static_cast<unsigned int>(now_second)))
						{
							this->OnSpaceRecordChange(record, structcommon::CS_DELETE);

							cai_map.erase(s_it++);
						}
						else
						{
							++ s_it;
						}
					}
				}

				{
					std::map<long long, SpaceRecord> &gift_map = user_spacerecord.gift_map;
					for (std::map<long long, SpaceRecord>::iterator s_it = gift_map.begin(); s_it != gift_map.end();)
					{
						SpaceRecord &record = s_it->second;
						if (record.CanRemove(static_cast<unsigned int>(now_second)))
						{
							this->OnSpaceRecordChange(record, structcommon::CS_DELETE);

							gift_map.erase(s_it++);
						}
						else
						{
							++ s_it;
						}
					}
				}
			}

			m_last_remove_time = now_second;
		}
	}

	{
		if (now_second >= m_last_save_time + MAX_SAVE_SPACE_INTERVAL)
		{
			this->CheckSaveSpaceRecord(now_second);
			m_last_save_time = now_second;
		}
	}
}

bool SpaceManager::LoadSpaceRecord(long long space_record_id_from)
{
	if (SPACE_STATE_LOAD_FINISH == m_data_state) return false;

	m_data_state = SPACE_STATE_LOADING;

	RMIInitSpaceRecordBackObjectImpl *impl = new RMIInitSpaceRecordBackObjectImpl();

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return gc.InitSpaceRecordAsyn(space_record_id_from, impl);
}

void RMIInitSpaceRecordBackObjectImpl::InitSpaceRecordRet(int ret, const SpaceParam &space_param)
{
	if (ret < 0 || space_param.count > SpaceParam::SPACE_MAX_RECORD_COUNT) 
	{
		printf(" RMIInitSpaceRecordBackObjectImpl::InitSpaceRecordRet Failed \n");
		EngineAdapter::Instance().StopGame();
		return;
	}

	long long next_id_from = 0;

	for (int i = 0; i < space_param.count; i++)
	{
		if (space_param.record_list[i].id_record > next_id_from)
		{
			next_id_from = space_param.record_list[i].id_record;
		}

		if (space_param.record_list[i].owner_uid <= 0) continue;

		if (!SpaceManager::Instance().AddSpaceRecord(space_param.record_list[i].owner_uid, space_param.record_list[i], true))
		{
			printf(" RMIInitSpaceRecordBackObjectImpl::InitSpaceRecordRet AddSpaceRecord %d Failed \n", space_param.record_list[i].owner_uid);
			EngineAdapter::Instance().StopGame();
			return;
		}
	}

	if (space_param.count > 0)	
	{
		SpaceManager::Instance().LoadSpaceRecord(next_id_from);
	}
	else
	{
		SpaceManager::Instance().LoadSpaceRecordSucc();
	}
}

bool SpaceManager::OnSpaceRecordChange(const SpaceRecord &space_record, int change_state)
{
	if (!this->CheckSaveSpaceRecord(EngineAdapter::Instance().Time())) return false;

	if (m_change_param.count < SpaceParam::SPACE_MAX_RECORD_COUNT)
	{
		memcpy(&m_change_param.record_list[m_change_param.count], &space_record, sizeof(SpaceRecord));
		m_change_param.record_list[m_change_param.count].change_state = change_state;

		++ m_change_param.count;

		return true; 
	}

	return false;
}

bool SpaceManager::AddSpaceRecord(int uid, const SpaceRecord &space_record, bool is_init)
{
	if (space_record.record_unq_id <= 0 || space_record.owner_uid <= 0 || space_record.type <= Protocol::SPACE_MSG_TYPE_INVALID || 
		space_record.type >= Protocol::SPACE_MSG_TYPE_MAX)
	{
		return false;
	}

	UserSpaceRecordMapIt it = m_user_record_map.find(uid);
	if (it == m_user_record_map.end())
	{
		UserSpaceRecord user_record;
		m_user_record_map[uid] = user_record;
	}

	it = m_user_record_map.find(uid);
	if (it == m_user_record_map.end())
	{
		return false;
	}

	int max_size = 0;
	std::map<long long, SpaceRecord> *record_map = it->second.GetMsgMap(space_record.type, &max_size);
	if (NULL != record_map)
	{
		if (record_map->find(space_record.record_unq_id) != record_map->end())  // 不允许重复的UnqID
		{
			return false;
		}

		if (static_cast<int>(record_map->size()) >= max_size)  // 条数超过限制 找出最小时间戳的 删除
		{
			//if (is_init) // 理论上不可能 出现了就停服
			//{
			//	printf("SpaceManager::AddSpaceRecord Init too much! \n");
			//	return false;
			//}
			//else 
			//{
				long long min_timestamp = 0;
				std::map<long long, SpaceRecord>::iterator min_it = record_map->begin();

				for (std::map<long long, SpaceRecord>::iterator s_it = record_map->begin(); s_it != record_map->end(); ++ s_it)
				{
					if (0 == min_timestamp || s_it->second.happen_timestamp < min_timestamp)
					{
						min_timestamp = s_it->second.happen_timestamp;
						min_it = s_it;
					}
				}

				if (min_it != record_map->end())
				{
					this->OnSpaceRecordChange(min_it->second, structcommon::CS_DELETE);
					record_map->erase(min_it);
				}
			//}
		}

		record_map->insert(std::make_pair(space_record.record_unq_id, space_record));

		if (!is_init) // 初始化不能再插入DB
		{
			this->OnSpaceRecordChange(space_record, structcommon::CS_INSERT);
		}
	}

	return true;
}

void SpaceManager::LoadSpaceRecordSucc()
{
	m_data_state = SPACE_STATE_LOAD_FINISH;
}

long long SpaceManager::GetRecordUnqiueID()
{
	return TranslateInt64(static_cast<int>(EngineAdapter::Instance().Time()), ++ inc_seq);
}

std::map<long long, SpaceRecord> * SpaceManager::GetUserTypeRecordMap(int uid, int msg_type)
{
	UserSpaceRecordMapIt it = m_user_record_map.find(uid);
	if (it != m_user_record_map.end())
	{
		if (Protocol::SPACE_MSG_TYPE_LIUYAN == msg_type)
		{
			return &it->second.liuyan_map;
		}
		else if (Protocol::SPACE_MSG_TYPE_CAI == msg_type)
		{
			return &it->second.cai_map;
		}
		else if (Protocol::SPACE_MSG_TYPE_GIFT == msg_type)
		{
			return &it->second.gift_map;
		}
	}

	return NULL;
}

bool SpaceManager::CheckSaveSpaceRecord(time_t now_second)
{
	if (now_second >= m_last_save_time + MAX_SAVE_SPACE_INTERVAL || m_change_param.count >= SpaceParam::SPACE_MAX_RECORD_COUNT) 
	{
		if (m_change_param.count > 0)
		{
			RMISaveSpaceRecordBackObjectImpl *impl = new RMISaveSpaceRecordBackObjectImpl();

			RMIGlobalClient gc;
			gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB()); 

			if (!gc.SaveSpaceRecordAsyn(m_change_param, impl)) return false;

			m_change_param.count = 0;
		}
	}

	return true; 
}

bool SpaceManager::CheckCanCai(Role *role, int target_uid)
{	
	std::map<long long, SpaceRecord> *record_map = this->GetUserTypeRecordMap(target_uid, Protocol::SPACE_MSG_TYPE_CAI);
	if (NULL != record_map)
	{
		for (std::map<long long, SpaceRecord>::iterator it = record_map->begin(); it != record_map->end(); ++ it)
		{
			if (it->second.from_uid == role->GetUID()) // 如果已经踩过 
			{
				if (0 == GetDayIndex(it->second.happen_timestamp, EngineAdapter::Instance().Time()))  // 并且时间为今天
				{
					return false;
				}
			}
		}
	}

	return true;
}













