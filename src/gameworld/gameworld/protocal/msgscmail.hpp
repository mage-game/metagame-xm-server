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

	class SCMailSendAck						//  9500��ȡ��������
	{
	public:
		SCMailSendAck();
		MessageHeader			header;

		int						ret;
	};

	class SCMailDeleteAck					// 9501ɾ���ʼ�����
	{
	public:
		SCMailDeleteAck();
		MessageHeader			header;

		int						mail_index; 
		int						ret;		// 0�ǳɹ� ����ʧ�� 
	};

	class SCMailLockAck						// 9502���ʼ�����
	{
	public:
		SCMailLockAck();
		MessageHeader			header;

		int						mail_index; 
		int						ret;		// 0�ǳɹ� ����ʧ�� 
	};

	class SCMailUnlockAck					// 9503�����ʼ�����
	{
	public:
		SCMailUnlockAck();
		MessageHeader			header;

		int						mail_index; 
		int						ret;		// 0�ǳɹ� ����ʧ�� 
	};

	class SCMailListAck						// 9504�ʼ��б���
	{
	public:
		SCMailListAck();
		MessageHeader			header;
		
		int						is_first;
		int						count;
		MailBrief				mails[MAX_MAIL_SEND_LIST_SIZE];
	};

	class SCMailDetailAck					// 9505�ʼ���ϸ��Ϣ
	{
	public:
		SCMailDetailAck();
		MessageHeader			header;

		int						mail_index; 
		MailContentParam		content_param;
	};

	class SCFetchAttachmentAck				// 9506��ȡ�ʼ���������
	{
	public:
		SCFetchAttachmentAck();
		MessageHeader			header;

		int						mail_index; 
		int						item_index;
		int						ret;
	};
	
	class SCRecvNewMail						// 9507���ʼ�֪ͨ
	{
	public:
		SCRecvNewMail();
		MessageHeader			header;

		MailBrief mail_brief; 
	};

	class SCHasUnReadMail					// 9508δ��ȡ�ʼ�֪ͨ
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

