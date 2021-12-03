#include "mailmanager.hpp"
#include "usermail.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/string/gameworldstr.h"

#include "engineadapter.h"
#include "internalcomm.h"
#include "world.h"
#include "obj/character/role.h"
#include "global/rmibackobjdef.h"
#include "global/usercache/usercache.hpp"
#include "global/droplog/droplogmanager.hpp"

static struct MailReasonText
{
	MailSubject subject;
	MailContentTxt contenttxt;
} mail_reason_text_map[SYSTEM_MAIL_REASON_MAX];

static void INIT_MAIL_REASON_TEXT(int reason, const char *subject, const char *content)
{
	if (reason <= SYSTEM_MAIL_REASON_INVALID || reason >= SYSTEM_MAIL_REASON_MAX) return;

	STRNCPY(mail_reason_text_map[reason].subject, subject, sizeof(MailSubject));
	STRNCPY(mail_reason_text_map[reason].contenttxt, content, sizeof(MailContentTxt));
}

static void InitMailReasonText()
{
	//////////////////////////////////////////////////    拍卖系统邮件    //////////////////////////////////////////////////
	INIT_MAIL_REASON_TEXT(SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_FULL, gamestring::g_publicsale_item_full_subject, gamestring::g_publicsale_item_full_content);
	INIT_MAIL_REASON_TEXT(SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_CANCEL, gamestring::g_publicsale_item_cancel_subject, gamestring::g_publicsale_item_cancel_content);
	INIT_MAIL_REASON_TEXT(SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_EXPIRE, gamestring::g_publicsale_item_expire_subject, gamestring::g_publicsale_item_expire_content);
	INIT_MAIL_REASON_TEXT(SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_BUY_SUCC, gamestring::g_publicsale_buy_succ_subject, gamestring::g_publicsale_buy_succ_content);
	INIT_MAIL_REASON_TEXT(SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_MONEY_BUY_FAIL, gamestring::g_publicsale_buy_fail_subject, gamestring::g_publicsale_buy_fail_content);
	INIT_MAIL_REASON_TEXT(SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_MONEY_SALE_SUCC, gamestring::g_publicsale_sell_succ_subject, gamestring::g_publicsale_sell_succ_content);

	//////////////////////////////////////////////////    军团系统邮件    //////////////////////////////////////////////////
	INIT_MAIL_REASON_TEXT(SYSTEM_MAIL_REASON_GUILD_CREATE, gamestring::g_create_guild_mail_subject, gamestring::g_create_guild_mail_content);
	INIT_MAIL_REASON_TEXT(SYSTEM_MAIL_REASON_GUILD_DIMISS, gamestring::g_dimiss_guild_mail_subject, gamestring::g_dimiss_guild_mail_content);
	INIT_MAIL_REASON_TEXT(SYSTEM_MAIL_REASON_GUILD_BEKICKED, gamestring::g_bekicked_guild_mail_subject, gamestring::g_bekicked_guild_mail_content);

	//////////////////////////////////////////////////    邮件系统邮件    //////////////////////////////////////////////////
	INIT_MAIL_REASON_TEXT(SYSTEM_MAIL_REASON_MAIL_RECEIVER_FULL, gamestring::g_mail_receiver_full_subject, gamestring::g_mail_receiver_full_content);

	//////////////////////////////////////////////////    一站到底    //////////////////////////////////////////////////
	INIT_MAIL_REASON_TEXT(SYSTEM_MAIL_REASON_YIZHANDAODI_LUCKY_REWARD, gamestring::g_yizhandaodi_lucky_reward_subject, gamestring::g_yizhandaodi_lucky_reward_content);

	//////////////////////////////////////////////////    五行挂机    //////////////////////////////////////////////////
	INIT_MAIL_REASON_TEXT(SYSTEM_MAIL_REASON_WUXINGGUAJI_LUCKY_REWARD, gamestring::g_wuxingguaji_lucky_reward_subject, gamestring::g_wuxingguaji_lucky_reward_content);

	//////////////////////////////////////////////////    日常任务完成奖励    //////////////////////////////////////////////////
	INIT_MAIL_REASON_TEXT(SYSTEM_MAIL_REASON_DAILY_TASK_FINISH_REWARD, gamestring::g_daily_task_finish_subject, gamestring::g_daily_task_finish_content);
}

const char * GetMailReasonSubject(int reason)
{
	static MailSubject subject = {0};
	return (reason <= SYSTEM_MAIL_REASON_INVALID || reason >= SYSTEM_MAIL_REASON_MAX) ? subject : mail_reason_text_map[reason].subject;
}

const char * GetMailReasonContent(int reason)
{
	static MailContentTxt contenttxt = {0};
	return (reason <= SYSTEM_MAIL_REASON_INVALID || reason >= SYSTEM_MAIL_REASON_MAX) ? contenttxt : mail_reason_text_map[reason].contenttxt;
}

UserMailManager::UserMailManager() : m_last_update(0)
{
}

UserMailManager::~UserMailManager()
{
	for (UserMailMapIt it = m_user_mail_map.begin(), end = m_user_mail_map.end(); it != end; ++it)
	{
		if (NULL != it->second)
		{
			delete it->second;
		}
	}

	m_user_mail_map.clear();
}

UserMailManager & UserMailManager::Instance()
{
	static UserMailManager umm;
	return umm;
}

void UserMailManager::OnServerStart()
{
	InitMailReasonText();
}

void UserMailManager::OnServerStop()
{
	for (UserMailMapIt it = m_user_mail_map.begin(), end = m_user_mail_map.end(); it != end; ++it)
	{
		it->second->SaveMailList();	
	}
}

void UserMailManager::Update(unsigned long interval, time_t now_second)
{
	if (now_second >= m_last_update + 1)
	{
		m_last_update = now_second;

		for (UserMailMapIt it = m_user_mail_map.begin(), end = m_user_mail_map.end(); it != end; ++it)
		{
			UserMail *user_mail = it->second;

			if (now_second - user_mail->GetLastActiveTime() >= MAIL_UN_ACTIVE_TIMEOUT_INTERVAL && !user_mail->IsDirty())
			{
				Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_mail->GetUserID());
				if (NULL == role)  // 并且该用户不在线 就清理掉 
				{
					delete user_mail;
					m_user_mail_map.erase(it);
					break;
				}
				else 
				{
					user_mail->Update(now_second);
				}
			}
			else 
			{
				user_mail->Update(now_second);
			}
		}
	}	
}

UserMail * UserMailManager::GetUserMail(const UserID &user_id)
{
	if (user_id.db_index < 0 || user_id.role_id <= 0) return NULL;

	UserMail *user_mail = NULL;

	UserMailMapIt it = m_user_mail_map.find(user_id);
	if (it == m_user_mail_map.end())
	{
		user_mail = new UserMail(this, user_id);
		m_user_mail_map[user_id] = user_mail;
	}
	else 
	{
		user_mail = it->second;
	}

	user_mail->SetLastActiveTime(EngineAdapter::Instance().Time());

	return user_mail;
}

bool UserMailManager::MailToUser(int mail_type, const UserID &recver_uid, int reason, const MailContentParam &contentparam)
{
	UserID recver_list[1];
	recver_list[0] = recver_uid;
	return this->MailToMultUser(mail_type, recver_list, 1, reason, contentparam);
}

bool UserMailManager::MailToMultUser(int mail_type, const UserID *recver_list, int recver_num, int reason, const MailContentParam &contentparam)
{
	return this->MailToMultUser(mail_type, UserID(0, 0), "System", recver_list, recver_num, reason, contentparam);
}

bool UserMailManager::MailToMultUser(int mail_type, const UserID &sender_uid, const GameName sender_name, const UserID *recver_list, int recver_num, int reason, const MailContentParam &contentparam)
{
	return this->SendMail(sender_uid, sender_name, recver_list, recver_num, mail_type, reason, contentparam);
}

void UserMailManager::OnSendMail(Role *role, MailSend *mail_send_req)
{
	if (NULL == role)
	{
		return;
	}

	UserID recver_list[1];
	recver_list[0] = UserID(mail_send_req->recver_dbindex, mail_send_req->recver_roleid);

	this->SendMail(role->GetUserId(), mail_send_req->sender_name, recver_list, 1, MAIL_TYPE_PERSONAL,0, mail_send_req->contentparam);		
}

void UserMailManager::OnDeleteMail(Role *role, int mail_index)
{
	if (NULL == role)
	{
		return;
	}

	UserMail *user_mail = this->GetUserMail(role->GetUserId());
	if (NULL != user_mail)
	{
		user_mail->RemoveMail(mail_index);
	}
}

void UserMailManager::OnLockMail(Role *role, int mail_index)
{
	if (NULL == role)
	{
		return;
	}

	UserMail *user_mail = this->GetUserMail(role->GetUserId());
	if (NULL != user_mail)
	{
		user_mail->LockMail(mail_index);
	}
}

void UserMailManager::OnUnlockMail(Role *role, int mail_index)
{
	if (NULL == role)
	{
		return;
	}

	UserMail *user_mail = this->GetUserMail(role->GetUserId());
	if (NULL != user_mail)
	{
		user_mail->UnlockMail(mail_index);
	}
}

void UserMailManager::OnFetchMailList(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	UserMail *user_mail = this->GetUserMail(role->GetUserId());
	if (NULL != user_mail)
	{
		user_mail->FetchMailList();
	}
}

void UserMailManager::OnFetchMailDetail(Role *role, int mail_index)
{
	if (NULL == role)
	{
		return;
	}

	UserMail *user_mail = this->GetUserMail(role->GetUserId());
	if (NULL != user_mail)
	{
		user_mail->FetchMailDetail(mail_index);
	}
}

void UserMailManager::OnFetchMailAttachment(Role *role, FetchAttachment *mail_attach_req)
{
	if (NULL == role)
	{
		return;
	}

	UserMail *user_mail = this->GetUserMail(role->GetUserId());
	if (NULL != user_mail)
	{
		user_mail->FetchAttachment(role, mail_attach_req->mail_index, mail_attach_req->item_index, mail_attach_req->is_last);
	}
}

void UserMailManager::OnFetchMailAttachmentAffirm(Role *role, FetchAttachmentAffirm *mail_attach_affirm)
{
	if (NULL == role)
	{
		return;
	}

	UserMail *user_mail = this->GetUserMail(role->GetUserId());
	if (NULL != user_mail)
	{
		user_mail->FetchAttachmentAffirm(mail_attach_affirm->mail_index, mail_attach_affirm->result, mail_attach_affirm->item_index, mail_attach_affirm->is_last);
	}
}

void UserMailManager::OnCleanMail(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	UserMail *user_mail = this->GetUserMail(role->GetUserId());
	if (NULL != user_mail)
	{
		user_mail->CleanMail();
	}
}

void UserMailManager::OnUserLogin(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	UserMail *user_mail = this->GetUserMail(role->GetUserId());
	if (NULL != user_mail)
	{
		user_mail->OnUserLogin(role);
	}
}

bool UserMailManager::SendMail(const UserID &sender_uid, const GameName sender_name, const UserID *recver_list, int recver_num,
							   int mail_type, int reason, const MailContentParam &contentparam)
{
	if (NULL == recver_list || recver_num <= 0) return false;

	static MailParam mail_param; 

	for (auto &item_data : contentparam.item_list)
	{
		DropLogManager::Instance().AddLogGetItem(item_data);
	}
	if (DropLogManager::Instance().HookSendMail(recver_list, recver_num, contentparam))
	{
		return true;
	}
	// 更新status 
	mail_param.status.sender_uid = UidToInt(sender_uid);
	F_STRNCPY(mail_param.status.sender_name, sender_name, sizeof(mail_param.status.sender_name));
	mail_param.status.recv_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	mail_param.status.kind = mail_type;
	mail_param.status.is_lock = 0;
	mail_param.status.is_read = 0;

	UserCacheNode * user = UserCacheManager::Instance().GetUserNode(sender_uid);
	if (NULL !=user)
	{
			mail_param.status.vip_level = user->vip_level;
	}
	else
	{
		mail_param.status.vip_level = 0;
	}

	// 更新content
	mail_param.contentparam = contentparam;
	if (SYSTEM_MAIL_REASON_INVALID != reason)
	{
		F_STRNCPY(mail_param.contentparam.subject, GetMailReasonSubject(reason), sizeof(mail_param.contentparam.subject));
		F_STRNCPY(mail_param.contentparam.contenttxt, GetMailReasonContent(reason), sizeof(mail_param.contentparam.contenttxt));
	}

	for (int i = 0; i < recver_num; ++i)
	{
		UserMail *user_mail = this->GetUserMail(recver_list[i]);
		if (NULL == user_mail) continue;

		user_mail->AddMail(mail_param, true);
	}

	return true;
}

void UserMailManager::LoadUserSystemMailListByCmd(const UserID &user_id)
{
	UserMail *user_mail = this->GetUserMail(user_id);
	if (NULL != user_mail) 
	{
		user_mail->LoadSystemMailListByCmd(true);
	}
}

void UserMailManager::LoadAllUserSystemMailListByCmd()
{
	for (UserMailMapIt iter = m_user_mail_map.begin(); iter != m_user_mail_map.end(); ++ iter)
	{
		UserMail *user_mail = iter->second;
		if (NULL != user_mail) 
		{
			user_mail->LoadSystemMailListByCmd(false);
		}
	}
}

void UserMailManager::OneKeyFetchAttachment(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	UserMail *user_mail = this->GetUserMail(role->GetUserId());
	if (NULL != user_mail)
	{
		user_mail->OneKeyFetchAttachment(role);
	}
}



