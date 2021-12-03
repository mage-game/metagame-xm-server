/******************************************************************************************
FileName: maildef.hpp
Author: 
Description: 邮件系统定义 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang	2011/07/17/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __MAIL_ITEM_HPP__
#define __MAIL_ITEM_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/maildef.hpp"

class MailItem
{
public:
	MailItem();
	~MailItem();

	bool OperTimeIsValid();

	bool HasAttachment() const;
	void ClearAttachment();
	bool Invalid();

	void * operator new(size_t c);
	void operator delete(void *m);

public:
	MailParam mail_param;
	unsigned int last_fetch_time;
};

#endif

