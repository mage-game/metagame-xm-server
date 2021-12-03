#include "usermail.hpp"

#include "mailmanager.hpp"

#include "protocal/msgscmail.hpp"
#include "global/rmibackobjdef.h"
#include "internalcomm.h"
#include "engineadapter.h"

#include "servercommon/servercommon.h"
#include "servercommon/errornum.h"
#include "servercommon/struct/structcommon.h"
#include "world.h"

#include "servercommon/internalprotocal/ggprotocal.h"
#include "gamelog.h"

#include "other/route/mailroute.hpp"
#include "obj/character/role.h"
#include "item/knapsack.h"

UserMail::UserMail(UserMailManager *usermail_manager, const UserID & user_id)
	: m_usermail_manager(usermail_manager), m_user_id(user_id), m_data_state(MAIL_STATE_INVALID), m_get_data_time(0),
	  m_lock_mail_count(0), m_mail_count(0), m_is_change(false), m_last_get_list_time(0),
	  m_last_save_time(0), m_last_get_sys_mail_time(0), m_last_check_state_time(0), m_last_active_time(0)
{
	memset(m_mail_list, 0, sizeof(m_mail_list));

	this->Release();
}

UserMail::~UserMail()
{
	this->Release();
}

void UserMail::Release()
{
	for (int i = 0; i < MAX_MAIL_NUM; ++i)
	{
		if (NULL != m_mail_list[i])
		{
			delete m_mail_list[i];
		}
	}
	memset(m_mail_list, 0, sizeof(m_mail_list));

	m_data_state = MAIL_STATE_INVALID;

	m_is_change = false;
	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));
	memset(m_old_state, 0, sizeof(m_old_state));
}

void UserMail::Update(time_t now)
{
	if (now - m_last_save_time >= MAIL_SAVE_DB_INTERVAL && m_is_change && MAIL_STATE_LOAD_OK == m_data_state)
	{
		if (this->SaveMailList())
		{
			m_last_save_time = now;
			this->ClearDirtyMark();
		}
	}

	if (now - m_last_get_sys_mail_time >= GET_SYSTEM_MAIL_INTERVAL && MAIL_STATE_LOAD_OK == m_data_state)
	{
		m_last_get_sys_mail_time = now;
		this->LoadSystemMailList();
	}

	if (now - m_last_check_state_time >= MAX_MAIL_CHECK_STATE_INTERVAL)  // 保证异常时候 能够自恢复
	{
		m_last_check_state_time = now;

		if (MAIL_STATE_LOAD_ING == m_data_state && now - m_get_data_time >= MAX_GET_MAIL_TIMEOUT_INTERVAL)
		{
			m_data_state = MAIL_STATE_INVALID;
		}
	}
}

bool UserMail::SaveMailList()
{
	if (!this->CheckStateReady() && !m_is_change) return false;

	RMISaveMailBackObjectImpl *smbo = new RMISaveMailBackObjectImpl();

	static MailListParam mail_param;
	this->GetChangeMailList(&mail_param);

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return gc.SaveMailsAsyn(UidToInt(m_user_id), mail_param, smbo);
}

void UserMail::GetChangeMailList(MailListParam *p)
{
	if (!m_is_change)
	{
		p->count = 0;
		return;
	}

	int count = 0;

	for (int i = 0; i < MAX_MAIL_NUM && count < MAX_MAIL_NUM; ++i)
	{
		if (m_dirty_mark[i])
		{
			if (NULL == m_mail_list[i] || m_mail_list[i]->Invalid())
			{
				if (!m_old_state[i]) continue;

				p->mail_list[count].change_state = structcommon::CS_DELETE;
				p->mail_list[count].mail_param.mail_index = i;
			}
			else 
			{
				p->mail_list[count].change_state = m_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT;
				p->mail_list[count].mail_param = m_mail_list[i]->mail_param;
			}

			++ count;
		}
	}

	p->count = count;
}

void UserMail::ClearDirtyMark()
{
	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));

	for (int i = 0; i < MAX_MAIL_NUM; ++i)
	{
		m_old_state[i] = (NULL != m_mail_list[i] && !m_mail_list[i]->Invalid());
	}

	m_is_change = false;
}

bool UserMail::CheckStateReady()
{
	if (MAIL_STATE_LOAD_OK != m_data_state) 
	{
		if (MAIL_STATE_LOAD_ING != m_data_state) 
		{
			this->LoadMailList();

			m_data_state = MAIL_STATE_LOAD_ING;
			m_get_data_time = EngineAdapter::Instance().Time();
		}

		return false;
	}

	return true;
}

void UserMail::LoadMailList()
{
	RMIInitMailsBackObjectImpl *gmbo = new RMIInitMailsBackObjectImpl();
	gmbo->user_id = m_user_id;

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	gc.InitMailsAsyn(UidToInt(m_user_id), gmbo);
}

void RMIInitMailsBackObjectImpl::InitMailsRet(int ret, const MailListParam &mail_param)
{
	UserMail *user_mail = UserMailManager::Instance().GetUserMail(user_id);
	if (NULL == user_mail)
	{
		gamelog::g_log_serious_error.printf(LL_CRITICAL, "[RMIInitMailsBackObjectImpl::InitMailsRet] user_mail not existed, uid:%d", UidToInt(user_id));
		return;
	}

	user_mail->InitMailList(ret, mail_param);
}

void UserMail::InitMailList(int result, const MailListParam &mail_param)
{
	if (MAIL_STATE_LOAD_ING != m_data_state) return;    // 当前状态必须是Loading  

	if (result < 0) 
	{ 
		m_data_state = MAIL_STATE_INVALID; 
		return; 
	} 

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	static MailItem mail_list_temp[MAX_MAIL_NUM];
	memset(mail_list_temp, 0, sizeof(mail_list_temp));

	int mail_temp_count = 0;
	if (m_mail_count > 0) 
	{
		for (int i = 0; i < MAX_MAIL_NUM; ++i)
		{
			if (NULL != m_mail_list[i] && !m_mail_list[i]->Invalid())
			{
				memcpy(&mail_list_temp[mail_temp_count], m_mail_list[i], sizeof(mail_list_temp[0]));
				delete m_mail_list[i];
				m_mail_list[i] = NULL;
				++ mail_temp_count;
			}
		}
	}
	this->Release();

	m_mail_count = 0; m_lock_mail_count = 0;
	for (int i = 0; i < mail_param.count && i < MAX_MAIL_NUM; ++i)
	{
		int index = mail_param.mail_list[i].mail_param.mail_index;
		if (index < 0 || index >= MAX_MAIL_NUM)
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[UserMail::InitMailList] Wrong index, index:%d", index);
			continue;
		}

		if (static_cast<int>(now - mail_param.mail_list[i].mail_param.status.recv_time) > MAX_MAIL_RESERVER_TIME && !mail_param.mail_list[i].mail_param.status.is_lock) 
		{
			m_old_state[index] = true;
			m_dirty_mark[index] = true;
			m_is_change = true;
		}
		else 
		{
			if (NULL != m_mail_list[index])
			{
				gamelog::g_log_serious_error.printf(LL_CRITICAL, "[UserMail::InitMailList] Repeat index, index:%d", index);
				continue;
			}

			m_mail_list[index] = new MailItem();

			memcpy(&m_mail_list[index]->mail_param, &mail_param.mail_list[i].mail_param, sizeof(m_mail_list[0]->mail_param));
			m_old_state[index] = true;

			++ m_mail_count;
			if (0 != m_mail_list[index]->mail_param.status.is_lock) ++ m_lock_mail_count;
		}
	}

	m_data_state = MAIL_STATE_LOAD_OK;
	m_last_save_time = EngineAdapter::Instance().Time();
	m_last_get_sys_mail_time = m_last_save_time - GET_SYSTEM_MAIL_INTERVAL + 2;

	for (int i = 0; i < mail_temp_count; ++i)	
	{
		if (!mail_list_temp[i].Invalid())
		{
			this->AddMail(mail_list_temp[i].mail_param, false);
		}
	}

	this->CheckUnReadMail();
}

bool UserMail::LoadSystemMailList()
{
	RMIGetSystemMailsBackObjectImpl *gsmbo = new RMIGetSystemMailsBackObjectImpl();
	gsmbo->user_id = m_user_id;

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return gc.GetSystemMailsAsyn(UidToInt(m_user_id), gsmbo);
}

void RMIGetSystemMailsBackObjectImpl::GetSystemMailsRet(int ret, const MailListParam &mail_param)
{
	if (ret < 0 || mail_param.count <= 0) return;

	UserMail *user_mail = UserMailManager::Instance().GetUserMail(user_id);
	if (NULL == user_mail) return;

	user_mail->RemoveSystemMailList(mail_param);
	user_mail->AddSystemMailList(mail_param);
}

void UserMail::AddSystemMailList(const MailListParam &mail_param)
{
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	UNSTD_STATIC_CHECK(3 == MAX_ATTACHMENT_ITEM_NUM);

	for (int i = 0; i < MAX_MAIL_NUM && i < mail_param.count; ++i)
	{
		const MailParam &sysmail_param = mail_param.mail_list[i].mail_param;

		if (mail_param.mail_list[i].check_uid != UidToInt(m_user_id))
		{
			gamelog::g_log_mail.printf(LL_INFO, "mail->add system mail[id:%d] error[recver[ uid:%d ], check[uid:%d], mail_param[%s]",
				(int)mail_param.mail_list[i].system_mail_id, UidToInt(m_user_id), (int)mail_param.mail_list[i].check_uid, MailParamLog(&sysmail_param));
			continue;
		}

		if (static_cast<int>(now - mail_param.mail_list[i].mail_param.status.recv_time) > MAX_MAIL_RESERVER_TIME)
		{
			gamelog::g_log_mail.printf(LL_INFO, "mail->add system mail[id:%d] timeout[recver[ uid:%d ], mail_param[%s]",
				(int)mail_param.mail_list[i].system_mail_id, UidToInt(m_user_id), MailParamLog(&sysmail_param));
			continue;
		}

		if (this->AddMail(sysmail_param, true))
		{
			gamelog::g_log_mail.printf(LL_INFO, "mail->add system mail[id:%d] succ[recver[ uid:%d ], mail_param[%s]",
				(int)mail_param.mail_list[i].system_mail_id, UidToInt(m_user_id), MailParamLog(&sysmail_param));
		}
		else
		{
			gamelog::g_log_mail.printf(LL_INFO, "mail->add system mail[id:%d] fail[recver[ uid:%d ], mail_param[%s]",
				(int)mail_param.mail_list[i].system_mail_id, UidToInt(m_user_id), MailParamLog(&sysmail_param));
		}
	}
}

void UserMail::RemoveSystemMailList(const MailListParam &mail_param)
{
	RMIRemoveSystemMailsBackObjectImpl *smbo = new RMIRemoveSystemMailsBackObjectImpl();
	smbo->user_id = m_user_id;

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	gc.RemoveSystemMailsAsyn(UidToInt(m_user_id), mail_param, smbo);
}

MailItem * UserMail::GetFreeMailItemSlot(int kind)
{
	if (kind <= MAIL_TYPE_INVALID || kind >= MAIL_TYPE_MAX) return NULL;

	for (int i = 0; i < MAX_MAIL_NUM; ++i)
	{
		if (NULL == m_mail_list[i])
		{
			m_mail_list[i] = new MailItem();
			m_mail_list[i]->mail_param.mail_index = i;
			++ m_mail_count;
			return m_mail_list[i];
		}
	}

	// CmpPriority 这个函数要求邮件必须满 

	MailItem *mail_tmp = m_mail_list[0];

	for (int i = 0; i < MAX_MAIL_NUM; ++i)
	{
		mail_tmp = this->CmpPriority(mail_tmp, m_mail_list[i]);
	}

	return mail_tmp;
}

MailItem * UserMail::CmpPriority(MailItem *mail1, MailItem *mail2)
{
	if (NULL == mail1 || NULL == mail2) return NULL;

	if (mail1->mail_param.status.is_lock == mail2->mail_param.status.is_lock)
	{
		if (mail1->HasAttachment() == mail2->HasAttachment())
		{
			if (mail1->mail_param.status.is_read == mail2->mail_param.status.is_read)
			{
				if (mail1->mail_param.status.kind == mail2->mail_param.status.kind)
				{
					if (mail1->mail_param.status.recv_time > mail2->mail_param.status.recv_time) return mail2;
					return mail1;
				}
				else
				{
					if (mail1->mail_param.status.kind == MAIL_TYPE_PERSONAL && mail2->mail_param.status.kind != MAIL_TYPE_PERSONAL) return mail1;
					return mail2;
				}
			}
			else 
			{
				if (mail1->mail_param.status.is_read && !mail2->mail_param.status.is_read) return mail1;
				return mail2;
			}
		}
		else 
		{
			if (mail1->HasAttachment() && !mail2->HasAttachment()) return mail2;
			return mail1;
		}
	}
	else 
	{
		if (mail1->mail_param.status.is_lock && !mail2->mail_param.status.is_lock) return mail2;
		return mail1;
	}
}

bool UserMail::AddMail(const MailParam &new_param, bool send_to_client)
{
	bool mail_load_ok = this->CheckStateReady();  // 不返回 先插入 等DB数据回来再合并 

	bool ret = true;

	MailItem *mailitem = this->GetFreeMailItemSlot(new_param.status.kind);  // 邮箱已满
	if (NULL != mailitem)									
	{
		int index = mailitem->mail_param.mail_index;

		if (index >= 0 && index < MAX_MAIL_NUM)
		{
			memcpy(&mailitem->mail_param, &new_param, sizeof(mailitem->mail_param));
			mailitem->mail_param.mail_index = index;

			m_dirty_mark[index] = true;
			m_is_change = true;

			if (send_to_client && mail_load_ok) 
			{
				this->SendMailAck(mailitem, true);
			}

			ret = true;
		}
	}
	else
	{
		MailItem mail_item;
		mail_item.mail_param = new_param;
		
		if (send_to_client)
		{
			this->SendMailAck(&mail_item, false);	// TODO
		}

		ret = false;
	}

	UNSTD_STATIC_CHECK(3 == MAX_ATTACHMENT_ITEM_NUM);

	if (new_param.contentparam.coin > 0 || new_param.contentparam.coin_bind > 0 || new_param.contentparam.gold > 0 || new_param.contentparam.gold_bind > 0 || 
		new_param.contentparam.item_list[0].item_id > 0 || new_param.contentparam.item_list[1].item_id > 0 || new_param.contentparam.item_list[2].item_id > 0)
	{
		if (mail_load_ok) 
		{
			gamelog::g_log_mail.printf(LL_INFO, "mail->add mail[ret:%s, receiver[uid:%d], mail_param[%s]]", ret ? "succ" : "full", 
				UidToInt(m_user_id), MailParamLog(&new_param));
		}
	}

	return ret;
}

void UserMail::SendMailAck(const MailItem *mail_item, bool is_succ)
{
	if (NULL == mail_item) return;
	
	if (is_succ)
	{
		static Protocol::SCRecvNewMail new_mail;
		new_mail.mail_brief.mail_index = mail_item->mail_param.mail_index;
		new_mail.mail_brief.has_attachment = mail_item->HasAttachment() ? 1 : 0;
		new_mail.mail_brief.mail_status = mail_item->mail_param.status;
		F_STRNCPY(new_mail.mail_brief.subject, mail_item->mail_param.contentparam.subject, sizeof(new_mail.mail_brief.subject));
		World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&new_mail, sizeof(new_mail));
	}
	else
	{
		if (MAIL_TYPE_PERSONAL == mail_item->mail_param.status.kind)
		{
			// 这里 不能 循环发邮件
			gamelog::g_log_mail.printf(LL_INFO, "UserMail::SendMailAck receiver[%d] full sender[%d]", 
				UidToInt(m_user_id), mail_item->mail_param.status.sender_uid);

			/*static MailContentParam contentparam; contentparam.Reset();
			contentparam = mail_item->mail_param.contentparam;
			m_usermail_manager->MailToUser(MAIL_TYPE_SYSTEM, IntToUid(mail_item->mail_param.status.sender_uid), SYSTEM_MAIL_REASON_MAIL_RECEIVER_FULL, contentparam); */
		}
	}

	if (MAIL_TYPE_PERSONAL != mail_item->mail_param.status.kind) return; // 系统邮件不通知发件人  

	Protocol::SCMailSendAck send_ack;
	send_ack.ret = is_succ ? 0 : -1;
	World::GetInstWorld()->GetSceneManager()->SendToUser(IntToUid(mail_item->mail_param.status.sender_uid), (const char*)&send_ack, sizeof(send_ack));
}

bool UserMail::RemoveMail(int mail_index)
{
	if (!this->CheckStateReady()) return false;

	static Protocol::SCMailDeleteAck mda;

	mda.mail_index = mail_index;

	if (mail_index < 0 || mail_index >= MAX_MAIL_NUM || NULL == m_mail_list[mail_index] ||
		m_mail_list[mail_index]->Invalid() || 0 != m_mail_list[mail_index]->mail_param.status.is_lock ||
		m_mail_list[mail_index]->HasAttachment())
	{
		mda.ret = -1;
		World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&mda, sizeof(mda));
		return false;
	}

	gamelog::g_log_mail.printf(LL_INFO, "mail->remove mail[ret:succ, receiver[uid:%d], mail_param[%s]]", 
		UidToInt(m_user_id), MailParamLog(&m_mail_list[mail_index]->mail_param));

	delete m_mail_list[mail_index];
	m_mail_list[mail_index] = NULL;

	m_mail_count --;
	if (m_mail_count <= 0) m_mail_count = 0;

	m_dirty_mark[mail_index] = true;
	m_is_change = true;

	mda.ret = 0;
	World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&mda, sizeof(mda));

	return true;
}

bool UserMail::LockMail(int mail_index)
{
	if (!this->CheckStateReady()) return false;

	static Protocol::SCMailLockAck mla;
	mla.mail_index = mail_index;

	if (mail_index < 0 || mail_index >= MAX_MAIL_NUM || NULL == m_mail_list[mail_index] || 
		0 != m_mail_list[mail_index]->mail_param.status.is_lock || m_lock_mail_count >= MAX_LOCK_MAIL_NUM)
	{
		mla.ret = -1;
		World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&mla, sizeof(mla));
		return false;
	}

	m_mail_list[mail_index]->mail_param.status.is_lock = 1;
	++ m_lock_mail_count;

	m_dirty_mark[mail_index] = true;
	m_is_change = true;

	mla.ret = 0;
	World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&mla, sizeof(mla));

	return true;
}

bool UserMail::UnlockMail(int mail_index)
{
	if (!this->CheckStateReady()) return false;

	static Protocol::SCMailUnlockAck mua;

	mua.mail_index = mail_index;

	if (mail_index < 0 || mail_index >= MAX_MAIL_NUM || NULL == m_mail_list[mail_index] || 
		m_mail_list[mail_index]->Invalid() || 0 == m_mail_list[mail_index]->mail_param.status.is_lock)
	{
		mua.ret = -1;
		World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&mua, sizeof(mua));
		return false;
	}

	m_mail_list[mail_index]->mail_param.status.is_lock = 0;

	-- m_lock_mail_count;
	if (m_lock_mail_count <= 0) m_lock_mail_count = 0;	// 有可能会小于0吗？

	m_dirty_mark[mail_index] = true;
	m_is_change = true;

	mua.ret = 0;
	World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&mua, sizeof(mua));

	return true;
}

bool UserMail::FetchMailList(bool force)
{
	if (!this->CheckStateReady()) return false;

	time_t now = EngineAdapter::Instance().Time();	
	if (!force && now - m_last_get_list_time < MIN_GET_MAIL_LIST_INTERVAL) return false;
	m_last_get_list_time = now;

	static Protocol::SCMailListAck mla;
	mla.is_first = 1;

	if (m_mail_count <= 0)
	{
		mla.count = 0;
		unsigned int length = sizeof(Protocol::SCMailListAck) - (MAX_MAIL_SEND_LIST_SIZE * sizeof(MailBrief));
		World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&mla, length);
	}
	else 
	{
		int count = 0;

		for (int i = 0; i < MAX_MAIL_NUM && count < MAX_MAIL_NUM; ++i)
		{
			if (NULL == m_mail_list[i] || m_mail_list[i]->Invalid()) continue;

			mla.mails[count].mail_index = m_mail_list[i]->mail_param.mail_index;
			mla.mails[count].has_attachment = m_mail_list[i]->HasAttachment() ? 1 : 0;
			mla.mails[count].mail_status = m_mail_list[i]->mail_param.status;
			F_STRNCPY(mla.mails[count].subject, m_mail_list[i]->mail_param.contentparam.subject, sizeof(mla.mails[count].subject));

			++ count;
			if (count >= MAX_MAIL_SEND_LIST_SIZE)
			{
				mla.count = count;
				unsigned int length = sizeof(Protocol::SCMailListAck) - (MAX_MAIL_SEND_LIST_SIZE - count) * sizeof(MailBrief);
				World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&mla, length);

				count = 0;
				mla.is_first = 0;
			}
		}

		if (count > 0)
		{
			mla.count = count;
			unsigned int length = sizeof(Protocol::SCMailListAck) - (MAX_MAIL_SEND_LIST_SIZE - count) * sizeof(MailBrief);
			World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&mla, length);
		}
	}

	return true;
}

bool UserMail::FetchMailDetail(int mail_index)
{
	if (!this->CheckStateReady()) return false;

	if (mail_index < 0 || mail_index >= MAX_MAIL_NUM || NULL == m_mail_list[mail_index] || m_mail_list[mail_index]->Invalid())
	{
		return false;
	}

	static Protocol::SCMailDetailAck mda;
	mda.mail_index = mail_index;
	mda.content_param = m_mail_list[mail_index]->mail_param.contentparam;
	World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&mda, sizeof(mda));
	
	if (!m_mail_list[mail_index]->mail_param.status.is_read) // 这里不再通知客户端设置成已读 客户端自己设置成已读 
	{
		m_mail_list[mail_index]->mail_param.status.is_read = 1;
		m_dirty_mark[mail_index] = true;
		m_is_change = true;
	}

	return true;
}

bool UserMail::FetchAttachment(Role *role, int mail_index, int item_index, int is_last)
{
	if (NULL == role)
	{
		return false;
	}

	if (!this->CheckStateReady()) return false;

	static Protocol::SCFetchAttachmentAck fmaa;
	fmaa.mail_index = mail_index;
	fmaa.item_index = item_index;

	if (mail_index < 0 || mail_index >= MAX_MAIL_NUM || NULL == m_mail_list[mail_index] || 
		m_mail_list[mail_index]->Invalid() || !m_mail_list[mail_index]->HasAttachment())
	{
		fmaa.ret = -1;
		World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&fmaa, sizeof(fmaa));

		gamelog::g_log_mail.printf(LL_INFO, "mail -> fetch attachment[ ret:fail, uid:%d, mail_index:%d item_index:%d]", UidToInt(m_user_id), mail_index, item_index);

		return false;
	}
	else 
	{
		if (!m_mail_list[mail_index]->OperTimeIsValid())  // 不允许频繁对一封邮件进行提取操作 提取流程是异步的 (客户端连续发多次提取邮件的包 就有可能提取多分邮件) 
		{
			gamelog::g_log_mail.printf(LL_INFO, "mail->fetch attachment[ ret:fail, oper time is invalid uid:%d, mail_index:%d item_index:%d]", UidToInt(m_user_id), mail_index, item_index);
			return false;
		}

		static ggprotocal::MailAttachment attachment;
		attachment.sender_uid = m_mail_list[mail_index]->mail_param.status.sender_uid;
		attachment.mail_index = mail_index;
		attachment.item_index = item_index;
		attachment.is_last = is_last;
		F_STRNCPY(attachment.subject, m_mail_list[mail_index]->mail_param.contentparam.subject, sizeof(attachment.subject));
		F_STRNCPY(attachment.contenttxt, m_mail_list[mail_index]->mail_param.contentparam.contenttxt, sizeof(attachment.contenttxt));

		if (item_index >= 0)
		{
			if (item_index >= MAX_ATTACHMENT_ITEM_NUM || m_mail_list[mail_index]->mail_param.contentparam.item_list[item_index].Invalid())
			{
				fmaa.ret = -1;
				World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&fmaa, sizeof(fmaa));

				gamelog::g_log_mail.printf(LL_INFO, "mail->fetch attachment[ret:fail, uid:%d, mail_index:%d item_index:%d]", UidToInt(m_user_id), mail_index, item_index);

				return false;
			}

			attachment.item_count = 1;
			attachment.item_list[0] = m_mail_list[mail_index]->mail_param.contentparam.item_list[item_index];
		}
		else 
		{
			if (MAIL_FETCH_ALL == item_index)
			{
				attachment.coin = m_mail_list[mail_index]->mail_param.contentparam.coin;
				attachment.coin_bind = m_mail_list[mail_index]->mail_param.contentparam.coin_bind;
				attachment.gold = m_mail_list[mail_index]->mail_param.contentparam.gold;
				attachment.gold_bind = m_mail_list[mail_index]->mail_param.contentparam.gold_bind;

				attachment.item_count = 0;
				for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
				{
					if (!m_mail_list[mail_index]->mail_param.contentparam.item_list[i].Invalid())
					{
						attachment.item_list[attachment.item_count ++] = m_mail_list[mail_index]->mail_param.contentparam.item_list[i];
					}
				}
			}

			if (MAIL_FETCH_ALL == item_index || MAIL_FETCH_VIRTUAL == item_index)
			{
				attachment.virtual_item_list = m_mail_list[mail_index]->mail_param.contentparam.virtual_item_list;
			}
		}

		if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(attachment.item_count))
		{
			role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return false;
		}

		MailRoute::Instance().OnMailAttachment(role, &attachment);

		return true;
	}
}

bool UserMail::FetchAttachmentAffirm(int mail_index, int result, int item_index, int is_last)
{
	if (0 != result) return false;
	if (mail_index < 0 || mail_index >= MAX_MAIL_NUM || NULL == m_mail_list[mail_index] || 
		m_mail_list[mail_index]->Invalid() || !m_mail_list[mail_index]->HasAttachment())
	{
		return false;
	}

	MailParam &mail_param = m_mail_list[mail_index]->mail_param;

	gamelog::g_log_mail.printf(LL_INFO, "mail->fetch attachment affirm[ret:succ, recver[uid:%d], item_index[%d], mail_param[%s]",
		UidToInt(m_user_id), item_index, MailParamLog(&mail_param));

	if (item_index >= 0)
	{
		if (item_index >= MAX_ATTACHMENT_ITEM_NUM) return false;

		m_mail_list[mail_index]->mail_param.contentparam.item_list[item_index].Reset();
	}
	else
	{
		if (MAIL_FETCH_ALL == item_index)
		{
			m_mail_list[mail_index]->ClearAttachment();
		}
		else if (MAIL_FETCH_VIRTUAL == item_index)
		{
			m_mail_list[mail_index]->mail_param.contentparam.virtual_item_list.Reset();
		}
	}

	m_dirty_mark[mail_index] = true;
	m_is_change = true;

	m_mail_list[mail_index]->mail_param.status.is_read = 1;

	static Protocol::SCFetchAttachmentAck fmaa;
	fmaa.mail_index = mail_index;
	fmaa.item_index = item_index;
	fmaa.ret = 0;
	World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&fmaa, sizeof(fmaa));
	
	return true;
}

bool UserMail::CleanMail()
{
	if (!this->CheckStateReady()) return false;

	for (int index = 0; index < MAX_MAIL_NUM; index++)
	{
		if (NULL == m_mail_list[index] || m_mail_list[index]->Invalid() || 0 != m_mail_list[index]->mail_param.status.is_lock ||
			m_mail_list[index]->HasAttachment() || !m_mail_list[index]->mail_param.status.is_read) 
		{
			continue;
		}
		
		delete m_mail_list[index];
		m_mail_list[index] = NULL;

		m_mail_count --;
		if (m_mail_count <= 0) m_mail_count = 0;

		m_dirty_mark[index] = true;
		m_is_change = true;
	}

	return this->FetchMailList(true);
}

void UserMail::OneKeyFetchAttachment(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (!this->CheckStateReady()) 
	{
		return;
	}

	for (int mail_index = 0; mail_index < MAX_MAIL_NUM; ++ mail_index)
	{
		if (NULL == m_mail_list[mail_index] || m_mail_list[mail_index]->Invalid() || !m_mail_list[mail_index]->HasAttachment())
		{
			continue;
		}

		if (!this->FetchAttachment(role, mail_index, MAIL_FETCH_ALL, 0))
		{
			break;
		}
	}
}

void UserMail::LoadSystemMailListByCmd(bool force)
{
	if (force)
	{
		m_last_get_sys_mail_time = 0;
	}
	else 
	{
		m_last_get_sys_mail_time = EngineAdapter::Instance().Time() - GET_SYSTEM_MAIL_INTERVAL + RandomNum(15);
	}
}

void UserMail::OnUserLogin(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	this->CheckUnReadMail();
}

void UserMail::CheckUnReadMail()
{
	if (!this->CheckStateReady()) return;

	if (m_mail_count > 0)
	{
		int unread_num = 0;
		int chongzhi_mail_num = 0;

		for (int i = 0; i < MAX_MAIL_NUM ; ++i)
		{
			if (NULL == m_mail_list[i] || m_mail_list[i]->Invalid()) continue;

			if (0 == m_mail_list[i]->mail_param.status.is_read)
			{
				++ unread_num;
			}

			if (MAIL_TYPE_CHONGZHI == m_mail_list[i]->mail_param.status.kind)
			{
				if (m_mail_list[i]->HasAttachment() || 0 == m_mail_list[i]->mail_param.status.is_read)
				{
					++ chongzhi_mail_num;
				}
			}
		}

		if (unread_num > 0 || chongzhi_mail_num > 0)
		{
			Protocol::SCHasUnReadMail hurm;
			hurm.unread_num = unread_num;
			hurm.chongzhi_mail_num = chongzhi_mail_num;
			World::GetInstWorld()->GetSceneManager()->SendToUser(m_user_id, (const char*)&hurm, sizeof(hurm));
		}
	}
}

