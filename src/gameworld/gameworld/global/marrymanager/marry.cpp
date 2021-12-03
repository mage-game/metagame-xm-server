#include "marry.h"

#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/errornum.h"

#include "engineadapter.h"

#include "world.h"
#include "obj/character/role.h"
#include "global/mail/mailmanager.hpp"

Marry::Marry()
	: m_status(MARRY_STATUS_INVALID), m_next_status_time(0)
{

}

Marry::~Marry()
{

}

void Marry::OnMarryStart(const MarryReserve &marry_reserve, int total_marry_count, unsigned int marry_start_time)
{
	m_status = MARRY_STATUS_INVALID;
	m_next_status_time = marry_start_time;

	m_marry_info.marry_status = m_status;
	m_marry_info.marry_level = marry_reserve.marry_level;
	m_marry_info.total_marry_count = total_marry_count;
	m_marry_info.marry_start_time = marry_start_time;
	memcpy(m_marry_info.uid_list, marry_reserve.uid_list, sizeof(m_marry_info.uid_list));
	memcpy(m_marry_info.user_name_list, marry_reserve.user_name_list, sizeof(m_marry_info.user_name_list));
	memcpy(m_marry_info.camp_list, marry_reserve.camp_list, sizeof(m_marry_info.camp_list));

	UNSTD_STATIC_CHECK(MARRY_COUPLE_USER_LIMIT_NUM == 2);
	m_hunyan_invite_list[0].uid = marry_reserve.uid_list[0];
	m_hunyan_invite_list[1].uid = marry_reserve.uid_list[1];
}

void Marry::OnUpdate(unsigned long interval, time_t now_second)
{
	if (0 != m_next_status_time && now_second >= m_next_status_time)
	{
		this->OnStatusChange(++m_status);
	}
}

void Marry::OnUserLogin(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (MARRY_STATUS_INVALID != m_status && MARRY_STATUS_FINISH != m_status)
	{
		this->NotifyMarryStatusAllUser(role);
		this->NotifyHunyanInviteList(role);
	}
}

void Marry::OnHunyanInviteUser(int invite_uid, int be_invited_uid, bool is_free_invite)
{
	if (MARRY_STATUS_HUN_YAN_ZHUN_BEI != m_status && MARRY_STATUS_HUN_YAN != m_status) return;

	for (int i = 0; i < MARRY_COUPLE_USER_LIMIT_NUM; i++)
	{
		if (m_hunyan_invite_list[i].uid == invite_uid)
		{
			if (m_hunyan_invite_list[i].count >= 15 && is_free_invite)
			{
				return;
			}

			if (m_hunyan_invite_list[i].count < MARRY_HUNYAN_INVITE_USER_MAX_NUM)
			{
				int index = m_hunyan_invite_list[i].count;
				if (index < 0 || index >= MARRY_HUNYAN_INVITE_USER_MAX_NUM) return;

				m_hunyan_invite_list[i].be_invite_uid_list[index] = be_invited_uid;
				++ m_hunyan_invite_list[i].count;

				static MailContentParam mail_content_param; mail_content_param.Reset();

				/*ItemConfigData *reward_item = GLOBAL_CONFIG->GetOtherConfig()->GetMarryHunyanInviteRewardItem(m_marry_info.marry_level);
				if (NULL == reward_item) return;

				mail_content_param.item_list[0].item_id = reward_item->item_id;
				mail_content_param.item_list[0].num = reward_item->num;
				mail_content_param.item_list[0].is_bind = reward_item->is_bind;

				int length1 = SNPRINTF(mail_content_param.subject, sizeof(mail_content_param.subject), gamestring::g_marry_hunyan_invite_subject);
				int length2 = SNPRINTF(mail_content_param.contenttxt, sizeof(mail_content_param.contenttxt), gamestring::g_marry_hunyan_invite_content, 
					m_marry_info.uid_list[0], m_marry_info.user_name_list[0], m_marry_info.camp_list[0], 
					m_marry_info.uid_list[1], m_marry_info.user_name_list[1], m_marry_info.camp_list[1]);
				if (length1 > 0 && length2 > 0)
				{
					UserMailManager::Instance().MailToUser(MAIL_TYPE_SYSTEM, IntToUid(be_invited_uid), SYSTEM_MAIL_REASON_INVALID, mail_content_param);
				}*/

				Role *user = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(invite_uid));
				if (NULL != user)
				{
					this->NotifyHunyanInviteList(user);
				}
		
				break;
			}
		}
	}
}

void Marry::OnGmForceToNextStatus()
{
	if (MARRY_STATUS_INVALID != m_status && MARRY_STATUS_FINISH != m_status)
	{
		m_next_status_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}
}

void Marry::OnHunyanEnterReq(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (MARRY_STATUS_HUN_YAN != m_status) return;

	bool is_find = false;

	for (int i = 0; i < MARRY_COUPLE_USER_LIMIT_NUM; i++)
	{
		if (m_hunyan_invite_list[i].uid != role->GetUID())
		{
			for (int j = 0; j < m_hunyan_invite_list[i].count && j < MARRY_HUNYAN_INVITE_USER_MAX_NUM; j++)
			{
				if (m_hunyan_invite_list[i].be_invite_uid_list[j] == role->GetUID())
				{
					is_find = true;
					break;
				}
			}
		}
		else
		{
			is_find = true;
		}
	}

	if (!is_find) 
	{
		role->NoticeNum(errornum::EN_MARRY_HUNYAN_NOT_BE_INVITED);
		return;
	}

	/*ggprotocal::MarryHunyanEnter mhe;
	GlobalServer::GetInstGlobalServer()->SendToUserGameWorld(user.GetUserId(), (const char *)&mhe, sizeof(mhe));*/
}

void Marry::OnMarryobjSyncPos(short pos_x, short pos_y)
{
	m_marry_info.marryobjpos_x = pos_x;
	m_marry_info.marryobjpos_y = pos_y;

	this->SyncMarryInfoToGameWorld();
}

void Marry::OnStatusChange(int to_status)
{
	switch (to_status)
	{
	case MARRY_STATUS_ZHUN_BEI:
		{
			this->OnMarryStatusToZhunBei();
		}
		break;

	case MARRY_STATUS_BAI_TIAN_DI:
		{
			this->OnMarryStatusToBaiTianDi();
		}
		break;

	case MARRY_STATUS_XUN_YOU:
		{
			this->OnMarryStatusToXunYou();
		}
		break;

	case MARRY_STATUS_HUN_YAN_ZHUN_BEI:
		{
			this->OnMarryStatusToHunYanZhunBei();
		}
		break;

	case MARRY_STATUS_HUN_YAN:
		{
			this->OnMarryStatusToHunYan();
		}
		break;

	case MARRY_STATUS_FINISH:
		{
			this->OnMarryStatusToFinish();
		}
		break;
	}

	m_marry_info.marry_status = m_status;
	if (MARRY_STATUS_FINISH != m_status) 
	{
		m_next_status_time = m_next_status_time + 100;
	}

	this->NotifyMarryStatusAllUser();
	this->SyncMarryInfoToGameWorld();
}


void Marry::OnMarryStatusToZhunBei()
{
	// 婚礼准备

	// 1: 发传闻
	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_marry_zhunbei_notice, 
					m_marry_info.uid_list[0], m_marry_info.user_name_list[0], m_marry_info.camp_list[0], 
					m_marry_info.uid_list[1], m_marry_info.user_name_list[1], m_marry_info.camp_list[1], m_marry_info.marry_level);
	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL, 0, 0);
	}
	
	// 2: 给新郎新娘发邮件
	static MailContentParam mail_content_param; mail_content_param.Reset();

	int length1 = SNPRINTF(mail_content_param.subject, sizeof(mail_content_param.subject), gamestring::g_marry_zhunbei_subject);
	int length2 = SNPRINTF(mail_content_param.contenttxt, sizeof(mail_content_param.contenttxt), gamestring::g_marry_zhunbei_content);
	if (length1 > 0 && length2 > 0)
	{
		for (int i = 0; i < MARRY_COUPLE_USER_LIMIT_NUM; i++)
		{
			UserMailManager::Instance().MailToUser(MAIL_TYPE_SYSTEM, IntToUid(m_marry_info.uid_list[i]), SYSTEM_MAIL_REASON_INVALID, mail_content_param);
		}
	}
}

void Marry::OnMarryStatusToBaiTianDi()
{
	// 拜天地开始
	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_marry_baitiandi_start, 
		m_marry_info.uid_list[0], m_marry_info.user_name_list[0], m_marry_info.camp_list[0], 
		m_marry_info.uid_list[1], m_marry_info.user_name_list[1], m_marry_info.camp_list[1]);
	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL, 0, 0);
	}
}

void Marry::OnMarryStatusToXunYou()								
{
	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_marry_baitiandi_finish, 
		m_marry_info.uid_list[0], m_marry_info.user_name_list[0], m_marry_info.camp_list[0], 
		m_marry_info.uid_list[1], m_marry_info.user_name_list[1], m_marry_info.camp_list[1]);
	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL, 0, 0);
	}
}

void Marry::OnMarryStatusToHunYanZhunBei()
{
	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_marry_hunyan_zhunbei_notice, 
		m_marry_info.uid_list[0], m_marry_info.user_name_list[0], m_marry_info.camp_list[0], 
		m_marry_info.uid_list[1], m_marry_info.user_name_list[1], m_marry_info.camp_list[1]);
	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL, 0, 0);
	}
}

void Marry::OnMarryStatusToHunYan()								
{

}

void Marry::OnMarryStatusToFinish()									
{
	m_next_status_time = 0;
	memset(m_hunyan_invite_list, 0, sizeof(m_hunyan_invite_list));
}

void Marry::SyncMarryInfoToGameWorld()
{
	/*static gglobalprotocal::MarryInfo mi;
	mi.marry_info = m_marry_info;
	GlobalServer::GetInstGlobalServer()->SendToAllGameWorld((const char*)&mi, sizeof(mi));*/
}

void Marry::NotifyMarryStatusAllUser(Role *role)
{
	/*static Protocol::SCMarryStatus ms;
	memcpy(ms.uid_list, m_marry_info.uid_list, sizeof(ms.uid_list));
	ms.marry_level = m_marry_info.marry_level;
	ms.marry_status = m_status;
	ms.next_status_time = m_next_status_time;
	ms.total_marry_count = m_marry_info.total_marry_count;

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ms, sizeof(ms));
	}
	else 
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&ms, sizeof(ms));
	}*/
}

void Marry::NotifyHunyanInviteList(Role *role)
{
	if (NULL == role) return;

	/*static Protocol::SCMarryHunyanInviteList mhil;

	for (int i = 0; i < MARRY_COUPLE_USER_LIMIT_NUM; i++)
	{
		if (role->GetUID() == m_hunyan_invite_list[i].uid)
		{
			mhil.invite_list = m_hunyan_invite_list[i];
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&mhil, sizeof(mhil));
			break;
		}
	}*/
}

