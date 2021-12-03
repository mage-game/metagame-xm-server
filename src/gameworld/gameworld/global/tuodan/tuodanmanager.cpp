#include "tuodanmanager.hpp"
#include "servercommon/tuodandef.hpp"
#include "global/rmibackobjdef.h"
#include "engineadapter.h"
#include "internalcomm.h"
#include "serverlogic.h"
#include "world.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "obj/character/role.h"
#include "global/usercache/usercache.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/errornum.h"
#include "other/event/eventhandler.hpp"
#include "other/qingyuan/qingyuan.hpp"

TuodanManager::TuodanManager() : m_is_loaded(false), m_last_save_time(0)
{
}	

TuodanManager::~TuodanManager()
{
}

TuodanManager & TuodanManager::Instance()
{
	static TuodanManager umm;
	return umm;
}

void TuodanManager::OnServerStart()
{
	RMIInitTuodanBackObjectImpl *backobj = new RMIInitTuodanBackObjectImpl();

	RMIGlobalClient ac;
	ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	ac.InitTuodanAsyn(0, backobj);
}

void RMIInitTuodanBackObjectImpl::InitTuodanRet(int ret, const TuodanParam &tuodan_param)
{
	TuodanManager::Instance().Init(ret, tuodan_param);
}

void TuodanManager::OnServerStop()
{
	this->CheckSaveToudan(EngineAdapter::Instance().Time() + MAX_SAVE_TUODAN_INTERVAL * 2);

	for (std::map<int, TuodanItem*>::iterator it = m_tuodan_map.begin(); it != m_tuodan_map.end(); it++)
	{
		delete it->second;
	}
	m_tuodan_map.clear();
}

void TuodanManager::Init(int ret, const TuodanParam &tuodan_param)
{
	int next_tuodan_id_from = 0; 

	if (0 != ret) 
	{
		printf("RMIInitTuodanBackObjectImpl::Init Failed \n");
		ServerLogic::GetInstServerLogic()->StopServer();
	}
	else 
	{
		for (int i = 0; i < tuodan_param.count && i < MAX_ONCE_TUODAN_COUNT; i++)
		{
			if (tuodan_param.tuodan_list[i].index > next_tuodan_id_from)
			{
				next_tuodan_id_from = tuodan_param.tuodan_list[i].index;
			}

			if (tuodan_param.tuodan_list[i].uid > 0)
			{
				TuodanItem *toudan_item = new TuodanItem();
				toudan_item->uid = tuodan_param.tuodan_list[i].uid;
				toudan_item->create_time = tuodan_param.tuodan_list[i].create_time;
				F_STRNCPY(toudan_item->notice, tuodan_param.tuodan_list[i].notice, sizeof(toudan_item->notice));
				m_tuodan_map[tuodan_param.tuodan_list[i].uid] = toudan_item;
			}
		}	
	}

	if (tuodan_param.count >= MAX_ONCE_TUODAN_COUNT)
	{
		RMIInitTuodanBackObjectImpl *backobj = new RMIInitTuodanBackObjectImpl();

		RMIGlobalClient ac;
		ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
		ac.InitTuodanAsyn(next_tuodan_id_from, backobj);
	}
	else 
	{
		m_is_loaded = true;
		printf("Init Tuodan Succ, Tuodan Count: %d\n", static_cast<int>(m_tuodan_map.size()));
	}
}

void TuodanManager::Update(unsigned long interval, time_t now_second)
{
	if (!this->IsLoadFinish()) return;

	this->CheckSaveToudan(now_second);
}

void TuodanManager::OnMarry(Role *role)
{
	if (NULL == role) return;

	this->OnRemoveTuodan(role);
}

void TuodanManager::OnSetSex(Role *role)
{
	if (NULL == role) return;

	this->SendSingleTuodanToWorld(role, Protocol::TUODAN_UPDATE);
}

void TuodanManager::TuodanOpera(Role *role, int req_type, TuodanNotice notice)
{
	if (NULL == role) return;

	if (Protocol::TUODAN_DELETE == req_type)
	{
		this->OnRemoveTuodan(role);
	}
	else
	{
		this->OnUpdateTuodan(role, notice);
	}
}

void TuodanManager::SendSingleTuodanToWorld(Role *role, int change_type)
{
	if (!m_is_loaded) return;
	if (NULL == role) return;

	static Protocol::SCSingleTuodanInfo single_tuodan;
	single_tuodan.change_type = change_type;

	TuodanItem *tuodan_item = this->GetTuodanItem(role->GetUID());
	if (NULL == tuodan_item || tuodan_item->Invalid()) return;

	this->PackTuodanInfo(tuodan_item, &single_tuodan.tuodan_info);

	World::GetInstWorld()->SendToAllGateway((const char*)&single_tuodan, sizeof(single_tuodan));
}

void TuodanManager::SendAllTuodan(Role *role)
{
	if (!m_is_loaded || NULL == role) return;

	static Protocol::SCAllTuodanInfo all_tuodan;

	all_tuodan.tuodan_list_count = 0;
	for (std::map<int, TuodanItem*>::iterator it = m_tuodan_map.begin(); it != m_tuodan_map.end(); it++)
	{
		if (it->second->Invalid()) continue;
		
		this->PackTuodanInfo(it->second, &all_tuodan.tuodan_list[all_tuodan.tuodan_list_count]);

		++all_tuodan.tuodan_list_count;

		if (all_tuodan.tuodan_list_count >= MAX_TUODAN_COUNT)
		{
			break;
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&all_tuodan, sizeof(all_tuodan));
}

void TuodanManager::PackTuodanInfo(TuodanItem* tuodan_item, Protocol::UserTuodanInfo *tuodan_info)
{
	if (NULL == tuodan_item || NULL == tuodan_info)
	{
		return;
	}

	UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(tuodan_item->uid);
	if (NULL == user_node) return;

	tuodan_info->uid = tuodan_item->uid;
	tuodan_info->create_time = tuodan_item->create_time;
	F_STRNCPY(tuodan_info->name, user_node->user_name, sizeof(GameName));
	tuodan_info->prof = user_node->prof;
	tuodan_info->avatar_timestamp = user_node->avatar_timestamp;
	tuodan_info->capability = user_node->capability;
	tuodan_info->level = user_node->level;
	tuodan_info->sex = user_node->sex;
	F_STRNCPY(tuodan_info->notice, tuodan_item->notice, sizeof(TuodanNotice));
}

bool TuodanManager::OnToudanChange(TuodanItem *toudan_item, int change_state)
{
	if (NULL == toudan_item || !this->CheckSaveToudan(EngineAdapter::Instance().Time())) return false;

	if (m_change_param.count < MAX_ONCE_TUODAN_COUNT)
	{
		TuodanParam::DBTuodanItem *db_toudan_item = &m_change_param.tuodan_list[m_change_param.count];
		db_toudan_item->change_state = change_state;
		db_toudan_item->uid = toudan_item->uid;
		db_toudan_item->create_time = toudan_item->create_time;
		F_STRNCPY(db_toudan_item->notice, toudan_item->notice, sizeof(db_toudan_item->notice));

		++ m_change_param.count;

		return true; 
	}

	return false;
}

TuodanItem *TuodanManager::GetTuodanItem(int uid)
{
	std::map<int, TuodanItem*>::iterator it = m_tuodan_map.find(uid);
	if (it == m_tuodan_map.end())
	{
		return NULL;
	}

	return it->second;
}

void TuodanManager::OnUpdateTuodan(Role *role, TuodanNotice notice)
{
	if (NULL == role) return;

// 	if (role->GetQingyuan()->GetTodayTuodanTimes() >= MAX_ONE_DAY_TUODAN_TIMES)
// 	{
// 		role->NoticeNum(errornum::EN_QINGYUAN_TUODAN_TIMES_MAX);
// 		return;
// 	}

	if (role->IsMarry())
	{
		role->NoticeNum(errornum::EN_MARRIAGE_SEEKING_MARRIED);
		return;
	}

	TuodanItem *toudan_item = this->GetTuodanItem(role->GetUID());
	if (NULL == toudan_item)
	{
		TuodanItem *toudan_item = new TuodanItem();
		toudan_item->uid = role->GetUID();
		toudan_item->create_time = (unsigned int)EngineAdapter::Instance().Time();
		F_STRNCPY(toudan_item->notice, notice, sizeof(toudan_item->notice));

		m_tuodan_map[toudan_item->uid] = toudan_item;
		this->OnToudanChange(toudan_item, structcommon::CS_INSERT);
	}
	else
	{
		F_STRNCPY(toudan_item->notice, notice, sizeof(toudan_item->notice));
		this->OnToudanChange(toudan_item, structcommon::CS_UPDATE);
	}

	int length = 0;
	if (FEMALE == role->GetSex())
	{
		length =  SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_qingyuan_tuodan1, role->GetUID(), role->GetName(), (int)role->GetCamp());
	}
	else
	{
		length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_qingyuan_tuodan2, role->GetUID(), role->GetName(), (int)role->GetCamp());
	}
	
	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
	}

	role->NoticeNum(errornum::EN_QINGYUAN_TUODAN_SUCCESS);

	role->GetQingyuan()->AddTodayTuodanTimes();

	this->SendSingleTuodanToWorld(role, Protocol::TUODAN_UPDATE);

	EventHandler::Instance().OnUpdateTuodan(role);
}

void TuodanManager::OnRemoveTuodan(Role *role)
{
	if (role == nullptr) return;
	TuodanItem *toudan_item = this->GetTuodanItem(role->GetUID());
	if (NULL == toudan_item)
	{
		return;
	}

	this->OnToudanChange(toudan_item, structcommon::CS_DELETE);

	this->SendSingleTuodanToWorld(role, Protocol::TUODAN_DELETE);

	delete toudan_item;
	m_tuodan_map.erase(role->GetUID());
}

bool TuodanManager::CheckSaveToudan(time_t now_second)
{
	if (now_second >= m_last_save_time + MAX_SAVE_TUODAN_INTERVAL || m_change_param.count >= MAX_ONCE_TUODAN_COUNT) 
	{
		m_last_save_time = now_second;

		if (m_change_param.count > 0)
		{
			RMISaveTuodanBackObjectImpl *backobj = new RMISaveTuodanBackObjectImpl();
			RMIGlobalClient ac;
			ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());

			if (!ac.SaveTuodanAsyn(m_change_param, backobj)) return false;

			m_change_param.count = 0;
		}
	}

	return true; 
}
