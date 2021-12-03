/******************************************************************************************
FileName: usermail.hpp
Author: 
Description: 用户邮件
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
luzejing    2011/07/17/ 09:04:05     1.0           创建
*****************************************************************************************/
#ifndef __USER_MAIL_HPP__
#define __USER_MAIL_HPP__

#include <map>

#include "servercommon/maildef.hpp"

#include "mailitem.hpp"
#include "servercommon/struct/global/maillistparam.hpp"

#include "inetworkmodule.h"

class UserMailManager;
class RMIGetSystemMailsBackObjectImpl;
class RMIInitMailsBackObjectImpl;
class Role;

class UserMail
{
	friend class RMIGetSystemMailsBackObjectImpl;
	friend class RMIInitMailsBackObjectImpl;

public:
	UserMail(UserMailManager *usermail_manager, const UserID &user_id);
	~UserMail();

	void * operator new(size_t c);
	void operator delete(void *m);

	const UserID & GetUserID() { return m_user_id; }

	void SetLastActiveTime(time_t now) { m_last_active_time = now; }
	time_t GetLastActiveTime() { return m_last_active_time; }

	void Update(time_t now);
	bool IsDirty() { return true == m_is_change; }
	bool SaveMailList();

	bool AddMail(const MailParam &new_param, bool send_to_client);
	bool RemoveMail(int mail_index);

	bool LockMail(int mail_index);
	bool UnlockMail(int mail_index);

	bool FetchMailList(bool force = false);
	bool FetchMailDetail(int mail_index);
	bool FetchAttachment(Role *role, int mail_index, int item_index, int is_last);
	bool FetchAttachmentAffirm(int mail_index, int result, int item_index, int is_last);
	bool CleanMail();
	void OneKeyFetchAttachment(Role *role);

	void LoadSystemMailListByCmd(bool force = false);

	void OnUserLogin(Role *role);

private:
	bool CheckStateReady();

	void LoadMailList();
	void InitMailList(int result, const MailListParam &mail_param);

	bool LoadSystemMailList();
	void AddSystemMailList(const MailListParam &mail_param);
	void RemoveSystemMailList(const MailListParam &mail_param);

	MailItem * GetFreeMailItemSlot(int kind);
	MailItem * CmpPriority(MailItem *mail1, MailItem *mail2);

	void GetChangeMailList(MailListParam *p);
	void ClearDirtyMark();

	void SendMailAck(const MailItem *mail_item, bool is_succ);

	void Release();

private:
	void CheckUnReadMail();

	UserMailManager *m_usermail_manager;

	UserID m_user_id;

	int m_data_state;
	time_t m_get_data_time;

	int m_lock_mail_count;
	int m_mail_count;
	MailItem *m_mail_list[MAX_MAIL_NUM];

	bool m_old_state[MAX_MAIL_NUM];
	bool m_dirty_mark[MAX_MAIL_NUM];
	bool m_is_change;

	time_t m_last_get_list_time;			// 上次客户端拉取邮件列表的时间
	time_t m_last_save_time;				// 上次回写DB时间 
	time_t m_last_get_sys_mail_time;		// 上次拉取运营邮件的时间
	time_t m_last_check_state_time;			// 上次检查状态时间 
	time_t m_last_active_time;				// 上次活跃时间 
};

#endif // __USER_MAIL_HPP__

