#include "offlineregistermanager.hpp"
#include "offlineregister.hpp"

#include "world.h"
#include "engineadapter.h"
#include "global/rmibackobjdef.h"
#include "internalcomm.h"

#include "global/rank/personrank.hpp"
#include "global/rank/rankmanager.hpp"

#include "servercommon/string/globalstr.h"
#include "obj/character/role.h"
#include "global/usercache/usercache.hpp"
#include "other/event/eventhandler.hpp"
#include "protocal/msgguild.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/guild/guildmanager.hpp"
#include "friend/friendmanager.h"
#include "other/roleactivity/roleactivity.hpp"
#include "other/qingyuan/qingyuan.hpp"

OfflineRegisterManager::OfflineRegisterManager()
	: m_data_state(OFFLINE_REGISTER_STATE_INVALID), m_last_save_time(0), m_last_remove_time(0)
{
	memset(&m_change_param, 0, sizeof(m_change_param));	
}

OfflineRegisterManager::~OfflineRegisterManager()
{
	for (OfflineRegisterMapIt it = m_offline_register_map.begin(), end = m_offline_register_map.end(); it != end; ++ it)
	{
		delete it->second;
	}

	m_offline_register_map.clear();
}

void OfflineRegisterManager::OnServerStart()
{
	this->LoadOfflineRegister(0);	
}

void OfflineRegisterManager::OnServerStop()
{
	this->CheckSaveOfflineRegister(EngineAdapter::Instance().Time() + MAX_SAVE_OFFLINE_REGISTER_INTERVAL * 2); 
}

void OfflineRegisterManager::Update(unsigned long interval, time_t now_second)
{
	if (OFFLINE_REGISTER_STATE_LOAD_FINISH != m_data_state) return;

	{
		if (now_second >= m_last_remove_time + MAX_REMOVE_OFFLINE_REGISTER_INTERVAL)
		{
			for (OfflineRegisterMapIt it = m_offline_register_map.begin(); it != m_offline_register_map.end();)
			{
				if (it->second->CanRemove(static_cast<unsigned int>(now_second)))
				{
					this->OnOfflineRegisterChange(it->second, structcommon::CS_DELETE);

					delete it->second;
					m_offline_register_map.erase(it++);
				}
				else 
				{
					++ it;
				}
			}

			m_last_remove_time = now_second;
		}
	}
	
	{
		if (now_second >= m_last_save_time + MAX_SAVE_OFFLINE_REGISTER_INTERVAL)
		{
			this->CheckSaveOfflineRegister(now_second);
			m_last_save_time = now_second;
		}
	}
}

void OfflineRegisterManager::OnEvaluateRole(const UserID &target_user_id, int add_charm)
{
	if (add_charm <= 0)
	{
		return;
	}

	OfflineRegister *offline_reg = this->CreateOfflineRegister(target_user_id);
	if (NULL == offline_reg) return;

	offline_reg->AddIncreaseCharm(add_charm);

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);
}

void OfflineRegisterManager::OnGMEvaluateRole(const UserID &target_user_id, short rank_type, bool is_admire, int times)
{
	GameName user_name; memset(user_name, 0, sizeof(user_name));
	char user_sex = 0, user_camp = 0;
	if (!RankManager::Instance().GetPersonRank()->GetRankUserInfo(rank_type, target_user_id, user_name, &user_sex, &user_camp)) return;

	OfflineRegister *offline_reg = this->CreateOfflineRegister(target_user_id);
	if (NULL == offline_reg) return;

	if (is_admire)
	{
		offline_reg->AddIncreaseCharm(times);
	}

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);
}

void OfflineRegisterManager::OnDivorceReq(const UserID &target_user_id, unsigned int last_divorce_time)
{
	OfflineRegister *offline_reg = this->CreateOfflineRegister(target_user_id);
	if (NULL == offline_reg) return;

	offline_reg->SetLastDivorceTime(last_divorce_time);

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);
}

void OfflineRegisterManager::OnUserFirstEnterScene(Role *user)
{
	if (NULL == user)
	{
		return;
	}

	bool is_change = false;

	UserID user_id = user->GetUserId();

	OfflineRegister *offline_register = this->GetOfflineRegister(user_id);
	if (NULL == offline_register) return;

	if (0 != offline_register->GetLastDivorceTime())
	{
		is_change = true;

		user->GetQingyuan()->OnDivorce(offline_register->GetLastDivorceTime());
		offline_register->SetLastDivorceTime(0);
	}

	if (0 != offline_register->GetIncreaseCharm())
	{
		is_change = true;

		user->GetFriendManager()->AddAllCharm(offline_register->GetIncreaseCharm());
		offline_register->ClearIncreaseCharm();
	}

	if (0 != offline_register->GetChongzhiCmdFlag())
	{
		is_change = true;

		user->GetRoleActivity()->CheckChongzhiExtraRewardByFlag(offline_register->GetChongzhiCmdFlag());
		offline_register->ClearChongzhiCmdFlag();
	}

	if (0 != offline_register->GetRedPaperConsumeGold())
	{
		is_change = true;

		user->AddRedPaperConsumeGold(offline_register->GetRedPaperConsumeGold(), "OfflineRigister");
		offline_register->ClearRedPaperConsumeGold();
	}

	if (0 != offline_register->GetRedPaperLuckyValue())
	{
		is_change = true;

		user->AddRedPaperLuckyValue(offline_register->GetRedPaperLuckyValue());
		offline_register->ClearRedPaperLuckyValue();
	}

	if (0 != offline_register->GetSpaceCaiCount())
	{
		is_change = true;

		user->SpaceCaiOrBeCai(offline_register->GetSpaceCaiCount(), false);	
		offline_register->ClearSpaceCaiCount();
	}

	if (0 != offline_register->GetSpaceGiftCount())
	{
		is_change = true;

		user->SpaceGiftOrBeGift(offline_register->GetSpaceGiftCount(), false);	
		offline_register->ClearSpaceGiftCount();
	}

	if (-1 != offline_register->GetAuthorityType())
	{
		is_change = true;
		user->SetAuthority(static_cast<char>(offline_register->GetAuthorityType()));

		offline_register->ClearAuthortiyType();
	}

	if (offline_register->ConsumeItem(user))
	{
		is_change = true;
	}

	if (offline_register->ConsumeGold(user))
	{
		is_change = true;
	}

// 	if (PersonProfessItem* profess_list = offline_register->GetProfessByOther())
// 	{
// 		is_change = true;
// 		int count = offline_register->GetProfessCount();
// 		for (int i = 0; i < count && i < PERSON_PROFESS_MAX_NUM; i++)
// 		{
// 			user->GetQingyuan()->OnProfessFrom(profess_list[i].other_role_id, profess_list[i]);
// 		}
// 		offline_register->ClearProfess();
// 	}

	if (offline_register->GetForbidTalkTimestamp() != 0)
	{
		is_change = true;
		int forbid_time = offline_register->GetForbidTalkTimestamp() - static_cast<unsigned int>(EngineAdapter::Instance().Time());
		if (forbid_time < 0)
		{
			forbid_time = 0;
		}
		//if (forbid_time > 0)
		{
			user->ForbidTalk(forbid_time, "Command", "Command");
		}

		offline_register->ClearForbidTalkTimestamp();
	}

	if (is_change)
	{
		this->OnOfflineRegisterChange(offline_register, structcommon::CS_UPDATE);
	}
}

void OfflineRegisterManager::OnChongzhiCmd(const UserID &target_user_id, int chongzhi_num)
{
	OfflineRegister *offline_reg = this->CreateOfflineRegister(target_user_id);
	if (NULL == offline_reg) return;

	offline_reg->OnChongzhiCmd(chongzhi_num);

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);
}

void OfflineRegisterManager::OnRedPaperConsumeGold(const UserID &target_user_id, int gold_num)
{
	OfflineRegister *offline_reg = this->CreateOfflineRegister(target_user_id);
	if (NULL == offline_reg) return;

	offline_reg->OnRedPaperConsumeGold(gold_num);

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);
}

void OfflineRegisterManager::OnAddRedPaperLuckyValue(const UserID &target_user_id, int lucky_value)
{
	OfflineRegister *offline_reg = this->CreateOfflineRegister(target_user_id);
	if (NULL == offline_reg) return;

	offline_reg->OnAddRedPaperLuckyValue(lucky_value);

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);
}

void OfflineRegisterManager::OnSpaceBeCai(const UserID &target_user_id)
{
	OfflineRegister *offline_reg = this->CreateOfflineRegister(target_user_id);
	if (NULL == offline_reg) return;

	offline_reg->OnSpaceBeCai();

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);
}

void OfflineRegisterManager::OnSpaceBeGift(const UserID &target_user_id)
{
	OfflineRegister *offline_reg = this->CreateOfflineRegister(target_user_id);
	if (NULL == offline_reg) return;

	offline_reg->OnSpaceBeGift();

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);
}

void OfflineRegisterManager::OnQingyuanBlessDaysChange(const UserID &target_user_id, int bless_days)
{
	OfflineRegister *offline_reg = this->CreateOfflineRegister(target_user_id);
	if (NULL == offline_reg) return;

	offline_reg->SetQingyuanBlessDays(bless_days);

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);
}

void OfflineRegisterManager::OnGetQingyuanBlessDays(const UserID &target_user_id, int *bless_days)
{
	OfflineRegister *offline_reg = this->CreateOfflineRegister(target_user_id);
	if (NULL == offline_reg) return;

	bless_days = offline_reg->GetQingyuanBlessDays();
}

GuildBoxInfo *OfflineRegisterManager::GetUserGuildBoxInfo(const UserID &user_id, bool is_force)
{
	OfflineRegister *offline_reg = is_force ? this->CreateOfflineRegister(user_id) : this->GetOfflineRegister(user_id);
	if (NULL == offline_reg) return NULL;

	return offline_reg->GetGuildBoxInfo();
}

void OfflineRegisterManager::OnGuildBoxInfoChange(const UserID &user_id)
{
	OfflineRegister *offline_reg = this->GetOfflineRegister(user_id);
	if (NULL == offline_reg) return;

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);
}

void OfflineRegisterManager::GuildBoxResetAll()
{
	const unsigned int dayid = EngineAdapter::Instance().DayID();
	for (OfflineRegisterMapIt it = m_offline_register_map.begin(); it != m_offline_register_map.end(); it++)
	{
		GuildBoxInfo *box_info = it->second->GetGuildBoxInfo();
		if (0 != box_info->dayid && dayid != box_info->dayid)
		{
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(it->first);
			if (NULL != role)
			{	
				for (int i = 0; i < MAX_GUILD_BOX_COUNT; i++)
				{
					GuildManager::Instance().GuildBoxMialReward(role, i);
				}
				
				box_info->Reset();
				this->OnOfflineRegisterChange(it->second, structcommon::CS_UPDATE);
				GuildManager::Instance().GuildBoxSendInfo(role, NULL);
			}
		}
	}
}

bool OfflineRegisterManager::OnSetAuthorityType(int uid, int authority_type)
{
	UserID user_id = IntToUid(uid);
	OfflineRegister *offline_reg = this->CreateOfflineRegister(user_id);
	if (nullptr == offline_reg) return false;

	offline_reg->OnSetAuthorityType(authority_type);

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);

	return true;
}

bool OfflineRegisterManager::OnSetConsumeItem(int uid, unsigned short item_id, unsigned short item_num)
{
	UserID user_id = IntToUid(uid);
	OfflineRegister *offline_reg = this->CreateOfflineRegister(user_id);
	if (nullptr == offline_reg) return false;

	offline_reg->OnSetConsumeItem(item_id, item_num);

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);

	return true;
}

bool OfflineRegisterManager::OnSetConsumeGold(int uid, int num)
{
	UserID user_id = IntToUid(uid);
	OfflineRegister *offline_reg = this->CreateOfflineRegister(user_id);
	if (nullptr == offline_reg) return false;

	offline_reg->OnSetConsumeGold(num);

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);

	return true;
}

bool OfflineRegisterManager::OnSetConsumeBindGold(int uid, int num)
{
	UserID user_id = IntToUid(uid);
	OfflineRegister *offline_reg = this->CreateOfflineRegister(user_id);
	if (nullptr == offline_reg) return false;

	offline_reg->OnSetConsumeBindGold(num);

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);

	return true;
}

bool OfflineRegisterManager::OnSetForbidTalk(int uid, unsigned int forbid_talk_end_timestamp)
{
	UserID user_id = IntToUid(uid);
	OfflineRegister *offline_reg = this->CreateOfflineRegister(user_id);
	if (nullptr == offline_reg) return false;

	offline_reg->SetForbidTalkTimestamp(forbid_talk_end_timestamp);

	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);

	return true;
}

// void OfflineRegisterManager::OnProfessFrom(const UserID &target_user_id, Role* profess_role, PersonProfessItem& item)
// {
// 	OfflineRegister *offline_reg = this->CreateOfflineRegister(target_user_id);
// 	if (NULL == offline_reg) return;
// 
// 	offline_reg->OnProfessFrom(profess_role, item);
// 
// 	this->OnOfflineRegisterChange(offline_reg, structcommon::CS_UPDATE);
// }

OfflineRegister * OfflineRegisterManager::GetOfflineRegister(const UserID &user_id)
{
	OfflineRegisterMapIt it = m_offline_register_map.find(user_id);
	if (it != m_offline_register_map.end())
	{
		return it->second;
	}

	return NULL;
}

OfflineRegister * OfflineRegisterManager::CreateOfflineRegister(const UserID &user_id)
{
	OfflineRegister *offline_reg = this->GetOfflineRegister(user_id);
	if (NULL == offline_reg)
	{
		offline_reg = new OfflineRegister(user_id);
		m_offline_register_map[user_id] = offline_reg;

		this->OnOfflineRegisterChange(offline_reg, structcommon::CS_INSERT);
	}

	return offline_reg;
}

bool OfflineRegisterManager::LoadOfflineRegister(long long offline_reg_id_from)
{
	if (OFFLINE_REGISTER_STATE_LOAD_FINISH == m_data_state) return false;

	m_data_state = OFFLINE_REGISTER_STATE_LOADING;

	RMIInitOfflineRegisterBackObjectImpl *impl = new RMIInitOfflineRegisterBackObjectImpl();

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return gc.InitOfflineRegisterAsyn(offline_reg_id_from, impl);
}

void RMIInitOfflineRegisterBackObjectImpl::InitOfflineRegisterRet(int ret, const OfflineRegisterParam &offline_reg_param)
{
	if (ret < 0 || offline_reg_param.count > OfflineRegisterParam::MAX_OFFLINE_REGISTER_COUNT) 
	{
		printf(" RMIInitOfflineRegisterBackObjectImpl::InitOfflineRegisterRet Failed \n");
		EngineAdapter::Instance().StopGame();
		return;
	}

	long long next_id_from = 0;

	for (int i = 0; i < offline_reg_param.count; i++)
	{
		if (offline_reg_param.offline_register_list[i].id_offline_register > next_id_from)
		{
			next_id_from = offline_reg_param.offline_register_list[i].id_offline_register;
		}

		if (offline_reg_param.offline_register_list[i].role_id <= 0) continue;

		UserID user_id = IntToUid(offline_reg_param.offline_register_list[i].role_id);

		if (!OfflineRegisterManager::Instance().AddOfflineRegister(user_id, offline_reg_param.offline_register_list[i]))
		{
			printf(" RMIInitOfflineRegisterBackObjectImpl::InitOfflineRegisterRet AddOfflineRegister Failed, u_id[%d] \n", UidToInt(user_id));
			EngineAdapter::Instance().StopGame();
			return;
		}
	}

	if (offline_reg_param.count > 0)	
	{
		OfflineRegisterManager::Instance().LoadOfflineRegister(next_id_from);
	}
	else
	{
		OfflineRegisterManager::Instance().LoadOfflineRegisterSucc();
	}
}

bool OfflineRegisterManager::OnOfflineRegisterChange(OfflineRegister *offline_reg, int change_state)
{
	if (NULL == offline_reg || !this->CheckSaveOfflineRegister(EngineAdapter::Instance().Time())) return false;

	if (m_change_param.count < OfflineRegisterParam::MAX_OFFLINE_REGISTER_COUNT)
	{
		OfflineRegisterParam::OfflineRegisterAttr *offline_reg_attr = &m_change_param.offline_register_list[m_change_param.count];

		offline_reg_attr->change_state = change_state;
		offline_reg->GetAttr(offline_reg_attr);

		++ m_change_param.count;

		return true; 
	}

	return false;
}

bool OfflineRegisterManager::AddOfflineRegister(const UserID &user_id, const OfflineRegisterParam::OfflineRegisterAttr &offline_reg_attr)
{
	OfflineRegisterMapIt it = m_offline_register_map.find(user_id);
	if (it != m_offline_register_map.end())
	{
		if (it->second->GetOffLineRegisterID() < offline_reg_attr.id_offline_register)
		{
			it->second->Init(offline_reg_attr);
		}
	}
	else
	{
		OfflineRegister* offline_reg = new OfflineRegister();
		offline_reg->Init(offline_reg_attr);
		m_offline_register_map[user_id] = offline_reg;
	}

	return true;
}

void OfflineRegisterManager::LoadOfflineRegisterSucc()
{
	m_data_state = OFFLINE_REGISTER_STATE_LOAD_FINISH;
}

bool OfflineRegisterManager::CheckSaveOfflineRegister(time_t now_second)
{
	if (now_second >= m_last_save_time + MAX_SAVE_OFFLINE_REGISTER_INTERVAL || m_change_param.count >= OfflineRegisterParam::MAX_OFFLINE_REGISTER_COUNT) 
	{
		if (m_change_param.count > 0)
		{
			RMISaveOfflineRegisterBackObjectImpl *impl = new RMISaveOfflineRegisterBackObjectImpl();

			RMIGlobalClient gc;
			gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB()); 

			if (!gc.SaveOfflineRegisterAsyn(m_change_param, impl)) return false;

			m_change_param.count = 0;
		}
	}

	return true; 
}

bool OfflineRegisterManager::IsLoadFinish()
{
	return (OFFLINE_REGISTER_STATE_LOAD_FINISH == m_data_state);
}

OfflineRegisterManager & OfflineRegisterManager::Instance()
{
	static OfflineRegisterManager orm;
	return orm;
}
