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
		long long system_mail_id;	// ������ɾ��ϵͳ�ʼ�

		int check_uid;				// ��������globalʱУ��ʹ�log
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
