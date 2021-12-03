/******************************************************************************************
FileName: usermail.hpp
Author: 
Description: 用户邮件管理器 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
luzejing    2011/07/17/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __MAIL_MANAGER_HPP__
#define __MAIL_MANAGER_HPP__

#include <map>

#include "servercommon/maildef.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"

class Role;
class UserMail;

class UserMailManager 
{
	friend class RMIInitMailsBackObjectImpl;
	friend class RMIGetSystemMailsBackObjectImpl;

public:
	static UserMailManager & Instance();
	
	void OnServerStart();
	void OnServerStop();

	void Update(unsigned long interval, time_t now_second);

	bool MailToUser(int mail_type, const UserID &recver_uid, int reason, const MailContentParam &contentparam);
	bool MailToMultUser(int mail_type, const UserID *recver_list, int recver_num, int reason, const MailContentParam &contentparam);
	bool MailToMultUser(int mail_type, const UserID &sender_uid, const GameName sender_name, const UserID *recver_list, int recver_num, int reason, const MailContentParam &contentparam);

	void OnSendMail(Role *role, MailSend *mail_send_req);
	void OnDeleteMail(Role *role, int mail_index);

	void OnLockMail(Role *role, int mail_index);
	void OnUnlockMail(Role *role, int mail_index);

	void OnFetchMailList(Role *role);
	void OnFetchMailDetail(Role *role, int mail_index);
	void OnFetchMailAttachment(Role *role, FetchAttachment *mail_attach_req);
	void OnFetchMailAttachmentAffirm(Role *role, FetchAttachmentAffirm *mail_attach_affirm);
	void OnCleanMail(Role *role);

	void OnUserLogin(Role *role);

	void LoadUserSystemMailListByCmd(const UserID &user_id);
	void LoadAllUserSystemMailListByCmd();

	void OneKeyFetchAttachment(Role *role);

private:
	UserMailManager();
	~UserMailManager();

	UserMail * GetUserMail(const UserID & user_id);
	bool SendMail(const UserID &sender_uid, const GameName sender_name, const UserID *recver_list, int recver_num, int mail_type, int reason, const MailContentParam &contentparam);

	typedef std::map<UserID, UserMail*> UserMailMap;
	typedef std::map<UserID, UserMail*>::iterator UserMailMapIt;
	UserMailMap m_user_mail_map;

	time_t m_last_update;
};

#endif  // __MAIL_MANAGER_HPP__

