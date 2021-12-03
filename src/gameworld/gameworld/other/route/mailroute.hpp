#ifndef __MAILSEND_HPP__
#define __MAILSEND_HPP__


#include "servercommon/crossdef.hpp"
#include "servercommon/maildef.hpp"
#include "servercommon/serverdef.h"

#include "protocal/msgmail.hpp"
#include "servercommon/internalprotocal/ggprotocal.h"

class Role;
struct UserCacheNode;

typedef bool (UserCacheFilterFunc)(const UserCacheNode &user_node, void **func_param);

class MailRoute
{
public:
	static MailRoute & Instance();
	
	void SendMail(Role *role, Protocol::CSMailSend *mailsend);
	void RemoveMail(Role *role, int mail_index);
	void LockMail(Role *role, int mail_index);
	void UnlockMail(Role *role, int mail_index);
	void GetMailList(Role *role);
	void ReadMail(Role *role, int mail_index);
	void FetchMailAttachment(Role *role, Protocol::CSMailFetchAttachment *attachment);
	void OnMailAttachment(Role *role, ggprotocal::MailAttachment *attachment);
	void CleanMail(Role *role);

	static void MailToUser(const UserID &user_id, int reason, const MailContentParam &contentparam, int mail_kind = MAIL_TYPE_SYSTEM);
	static void MailToUser(const UserID &user_id, MailSubject subject, MailContentTxt contenttxt, int mail_kind = MAIL_TYPE_SYSTEM);

	static void MailToMultUser(UserID *user_list, int user_num , int reason, const MailContentParam &contentparam, int mail_kind = MAIL_TYPE_SYSTEM);
	static void MailToMultUser(UserID *user_list, int user_num , MailSubject subject, MailContentTxt contenttxt, int mail_kind = MAIL_TYPE_SYSTEM);

	static void MailToAllUser(int reason, const MailContentParam &contentparam, UserCacheFilterFunc filter_func = NULL, void **func_param = NULL);
	static void MailToAllUser(int reason, MailSubject subject, MailContentTxt contenttxt, UserCacheFilterFunc filter_func = NULL, void **func_param = NULL);

	static void MailToCrossUser(const UniqueUserID &unique_user_id, int reason, const MailContentParam &contentparam);
	static void MailToRole(Role *role, int reason, const MailContentParam &contentparam);

	static bool FastMail(int uid, const ItemConfigData &item_configdata, const char *content_str, const char *subject_str = NULL);
	static bool FastMailList(int uid, const ItemConfigData item_configdata_list[MAX_ATTACHMENT_ITEM_NUM], const char *content_str, const char *subject_str = NULL);

private:
	MailRoute() {}
	~MailRoute() {}
};

bool UserCacheLevelFilter(const UserCacheNode &user_node, void **func_param);
bool UserCacheCampFilter(const UserCacheNode &user_node, void **func_param);

#endif

