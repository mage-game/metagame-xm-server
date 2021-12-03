#ifndef __MAILINITPARAM_HPP__
#define __MAILINITPARAM_HPP__

#include "common/tlvprotocol.h"

#include "servercommon/serverdef.h"
#include "servercommon/maildef.hpp"

class MailListParam
{
public:
	bool Serialize(TLVSerializer &outstram) const;
	bool Unserialize(TLVUnserializer &instream);

	struct MailAttr
	{
		char change_state;
		long long system_mail_id;	// 仅用于删除系统邮件

		int check_uid;				// 仅用于在global时校验和打log
		MailParam mail_param;
	};

	int count;
	MailAttr mail_list[MAX_MAIL_NUM];
};

class SystemMailEventParam
{
public:
	bool Serialize(TLVSerializer &outstram) const;
	bool Unserialize(TLVUnserializer &instream);

	int condition;
	long long param;
	MailContentParam contentparam;
};

#endif
