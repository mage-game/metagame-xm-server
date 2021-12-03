#ifndef __MSG_SC_MAIL_HPP_
#define __MSG_SC_MAIL_HPP_

#include "servercommon/userprotocal/msgheader.h"

#include "servercommon/maildef.hpp"
#include "servercommon/struct/itemlistparam.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{

	// S -> C

	class SCMailSendAck						//  9500提取附件返回
	{
	public:
		SCMailSendAck();
		MessageHeader			header;

		int						ret;
	};

	class SCMailDeleteAck					// 9501删除邮件返回
	{
	public:
		SCMailDeleteAck();
		MessageHeader			header;

		int						mail_index; 
		int						ret;		// 0是成功 其他失败 
	};

	class SCMailLockAck						// 9502锁邮件返回
	{
	public:
		SCMailLockAck();
		MessageHeader			header;

		int						mail_index; 
		int						ret;		// 0是成功 其他失败 
	};

	class SCMailUnlockAck					// 9503解锁邮件返回
	{
	public:
		SCMailUnlockAck();
		MessageHeader			header;

		int						mail_index; 
		int						ret;		// 0是成功 其他失败 
	};

	class SCMailListAck						// 9504邮件列表返回
	{
	public:
		SCMailListAck();
		MessageHeader			header;
		
		int						is_first;
		int						count;
		MailBrief				mails[MAX_MAIL_SEND_LIST_SIZE];
	};

	class SCMailDetailAck					// 9505邮件详细信息
	{
	public:
		SCMailDetailAck();
		MessageHeader			header;

		int						mail_index; 
		MailContentParam		content_param;
	};

	class SCFetchAttachmentAck				// 9506提取邮件附件返回
	{
	public:
		SCFetchAttachmentAck();
		MessageHeader			header;

		int						mail_index; 
		int						item_index;
		int						ret;
	};
	
	class SCRecvNewMail						// 9507新邮件通知
	{
	public:
		SCRecvNewMail();
		MessageHeader			header;

		MailBrief mail_brief; 
	};

	class SCHasUnReadMail					// 9508未读取邮件通知
	{
	public:
		SCHasUnReadMail();
		MessageHeader			header;

		short					unread_num;
		short					chongzhi_mail_num;
	};
}

#pragma pack(pop)

#endif

